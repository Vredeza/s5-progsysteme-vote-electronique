#include "../common/include/bd.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <gmp.h>
#include "../common/include/crypto.h"

const char *electeur_create = "CREATE TABLE IF NOT EXISTS Electeur(id INTEGER PRIMARY KEY, numeroID BLOB);";

const char *election_create = "CREATE TABLE IF NOT EXISTS Election(\
    id INTEGER PRIMARY KEY, \
    identifiant BLOB ,\
    question TEXT CHECK(length(question) <= 256),\
    dateDebut TEXT, \
    dateFin TEXT, \
    status TEXT CHECK(status IN('canceled', 'active', 'closed')));\
                              ";

const char *vote_create = "CREATE TABLE IF NOT EXISTS Vote(id INTEGER PRIMARY KEY,\
    idVotant INTEGER,\
    idElection INTEGER,\
    timestamp TEXT,\
    ballot BLOB ,\
    hashValidation TEXT CHECK(length(hashValidation) <= 256) );";

int database_exists(const char *path)
{
    struct stat buffer;
    return (stat(path, &buffer));
}

sqlite3 *database_open(const char *path)
{
    sqlite3 *db = NULL;
    int rc = sqlite3_open(path, &db);
    if (rc != SQLITE_OK)
    {
        // Gérer l'erreur
        sqlite3_close(db);
        return NULL;
    }
    return db;
};

int database_close(sqlite3 *db)
{
    return (sqlite3_close(db) == SQLITE_OK) ? 0 : 1;
};

int database_init(sqlite3 *db)
{
    printf("database_init\n");

    const char *requests[3] = {electeur_create, election_create, vote_create};
    int i;
    for (i = 0; i < 3; i++) // TODO
    {
        const char *sql = requests[i];
        char *errMsg = 0;
        int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK)
        {
            // Gérer l'erreur
            if (errMsg)
            {
                sqlite3_free(errMsg);
            }
            sqlite3_close(db);
            return 1;
        }
    }
    return 0;
};

void createElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Electeur (numeroID) VALUES (?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        const char *data = numeroID;
        // int size = strlen(data) + 1; // +1 pour le caractère nul de fin

        sqlite3_bind_blob(stmt, 1, data, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur ajouté avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void readElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Electeur WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Supposons que numeroID est la première colonne
            const char *id = sqlite3_column_blob(stmt, 0);
            printf("Electeur: %s\n", id);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de peremption: %s\n", sqlite3_errmsg(db));
    }
}

/*void listeElecteur(sqlite3* db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Electeur;";



}*/

int listeElecteur(sqlite3 *db, char ***tableauElecteurs, int *nombreElecteurs) {

    sqlite3_stmt *stmt;
    const char *sql = "SELECT numeroID FROM Electeur;";

    // Prépare la requête SQL
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Initialise le tableau d'électeurs
    *tableauElecteurs = NULL;
    *nombreElecteurs = 0;

    // Exécute la requête
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Réalise une réallocation du tableau d'électeurs
        *tableauElecteurs = realloc(*tableauElecteurs, (*nombreElecteurs + 1) * sizeof(char *));

        if (!(*tableauElecteurs)) {
            fprintf(stderr, "Erreur lors de l'allocation de mémoire pour les électeurs\n");
            return -1;
        }

        // Réalise une allocation de mémoire pour le numéroID
        (*tableauElecteurs)[*nombreElecteurs] = malloc(10);

        if (!(*tableauElecteurs)[*nombreElecteurs]) {
            fprintf(stderr, "Erreur lors de l'allocation de mémoire pour le numéroID\n");
            return -1;
        }

        // Récupère le numéroID de la base de données et le stocke dans le tableau d'électeurs
        strcpy((*tableauElecteurs)[*nombreElecteurs], sqlite3_column_text(stmt, 0));
        (*nombreElecteurs)++;
    }

    // Finalise la requête préparée
    sqlite3_finalize(stmt);
    return 0;
}

int electeurExists(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Electeur WHERE numeroID = ?;";
    int result = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = sqlite3_column_int(stmt, 0) > 0;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return result;
}

void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Electeur SET numeroID = ? WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, newNumeroID, size1, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, 2, oldNumeroID, size2, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de la mise à jour: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur mis à jour avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void deleteElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Electeur WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de la suppression: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur supprimé avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Electeur WHERE numeroID = ?;";
    int id = -1; // Valeur par défaut si l'ID n'est pas trouvé

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0); // Récupère l'ID de la première colonne
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return id;
}

void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Election (identifiant, question, dateDebut, dateFin, status) VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, identifiant, sizeId, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, question, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, dateDebut, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, dateFin, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, status, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Election WHERE identifiant = ?;";
    int id = -1; // Valeur par défaut si l'ID n'est pas trouvé

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0); // Récupère l'ID de la première colonne
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return id;
}

void readElection(sqlite3 *db, int id, Election *election)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Election WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Récupérer les valeurs depuis la base de données et les stocker dans la structure Election
            snprintf(election->identifiant, sizeof(election->identifiant), "%s", sqlite3_column_text(stmt, 1));
            snprintf(election->question, sizeof(election->question), "%s", sqlite3_column_text(stmt, 2));
            snprintf(election->dateDebut, sizeof(election->dateDebut), "%s", sqlite3_column_text(stmt, 3));
            snprintf(election->dateFin, sizeof(election->dateFin), "%s", sqlite3_column_text(stmt, 4));

            const char *statusStr = (const char *)sqlite3_column_text(stmt, 5);
            if (strcmp(statusStr, "canceled") == 0)
                election->status = canceled;
            else if (strcmp(statusStr, "active") == 0)
                election->status = active;
            else if (strcmp(statusStr, "closed") == 0)
                election->status = closed;
            else
                fprintf(stderr, "Erreur : Statut inconnu\n");
        }
        else
        {
            fprintf(stderr, "Aucun enregistrement trouvé pour l'ID %d\n", id);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void listeElection(sqlite3 *db, Election **elections, int *numElections)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Election;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            *numElections = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
        return;
    }

    *elections = (Election *)malloc(*numElections * sizeof(Election));
    if (*elections == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return;
    }

    sql = "SELECT id FROM Election;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        int i = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            readElection(db, id, &((*elections)[i]));
            i++;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
        free(*elections);
        return;
    }
}

void updateElection(sqlite3 *db, int id, const char *question)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Election SET question = ? WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, question, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de la mise à jour: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void deleteElection(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Election WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de la suppression: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

// usecases election

void Election_castVote(sqlite3 *db, int idVotant, int idElection, mpz_t encrypted_ballot, int ballotSize, const char *hashValidation)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Vote (idVotant, idElection, timestamp, ballot, hashValidation) VALUES (?, ?, datetime('now'), ?, ?);";

    // Convert mpz_t to binary for storing
    size_t size;
    unsigned char *ballot_data = (unsigned char *)mpz_export(NULL, &size, 1, 1, 0, 0, encrypted_ballot);
    ballotSize = (int)size; // Update the size of the binary data

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, idVotant);
        sqlite3_bind_int(stmt, 2, idElection);
        sqlite3_bind_blob(stmt, 3, ballot_data, ballotSize, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, hashValidation, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Vote ajouté avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    // Free the allocated memory for ballot_data
    free(ballot_data);
}

//
void Election_processVotes(sqlite3 *db, int electionId, mpz_t lambda, mpz_t mu, mpz_t n, int *p_option0, int *p_option1, int *p_totalvotes)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT ballot FROM Vote WHERE idElection = ?;";
    *p_totalvotes = 0;
    *p_option0 = 0;
    *p_option1 = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, electionId);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const void *ballotBlob = sqlite3_column_blob(stmt, 0);
            int blobSize = sqlite3_column_bytes(stmt, 0);

            // Decrypt le vote
            mpz_t encrypted_ballot, decrypted_ballot;
            mpz_init(encrypted_ballot);
            mpz_import(encrypted_ballot, blobSize, 1, 1, 0, 0, ballotBlob);

            mpz_init(decrypted_ballot);
            decrypt(decrypted_ballot, encrypted_ballot, lambda, mu, n);

            char vote = (char)mpz_get_ui(decrypted_ballot);
            if (vote)
            {
                *p_option1 = *p_option1 + 1;
            }
            else
            {
                *p_option0 = *p_option0 + 1;
            }

            *p_totalvotes = *p_totalvotes + 1;

            mpz_clears(encrypted_ballot, decrypted_ballot, NULL);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}


//int Election_resultat(sqlite3 *db, int localid, int *numberOption0, int *numberOption1, int *totalVotes)