#include "../common/include/votechain.h"
#include "../common/include/crypto.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gmp.h>

#define ID_SIZE 10
#define BALLOT_SIZE sizeof(mpz_t)

void encrypt_vote(mpz_t encrypted_vote, char vote, mpz_t n, mpz_t g) {
    mpz_t m;
    mpz_init_set_ui(m, (unsigned long)vote);
    encrypt(encrypted_vote, m, n, g);
    mpz_clear(m);
}

void decrypt_vote(mpz_t decrypted_vote, mpz_t encrypted_vote, mpz_t lambda, mpz_t mu, mpz_t n) {
    decrypt(decrypted_vote, encrypted_vote, lambda, mu, n);
}

int main(int argc, char *argv[]) {
    GError *error = NULL;
    GOptionContext *context;

    // Définition des options
    gboolean verbose = FALSE;
    gchar *file_path = NULL;
    GOptionEntry entries[] = {
            {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
            {"file", 'f', 0, G_OPTION_ARG_STRING, &file_path, "Chemin du fichier", "FILE"},
            {NULL}};

    // Création d'un contexte d'option
    context = g_option_context_new("- test pour la création de la base de donnée");
    g_option_context_add_main_entries(context, entries, NULL);

    // Analyse des options de ligne de commande
    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }
    if (verbose) {
        g_print("Mode verbeux activé\n");
    }
    if (file_path != NULL) {
        g_print("Chemin du fichier: %s\n", file_path);
        int needInit = database_exists(file_path);
        struct stat buffer;
        g_assert(needInit == -1);
        sqlite3 *db = database_open(file_path);
        g_assert_nonnull(db);
        if (needInit == -1) {
            g_print("initilisation de la base de données");
            database_init(db);
        }

        // Creation d'une election
        char idElection[ID_SIZE] = "EXXXXXXXXX";
        const char *question = "This is a binary question";
        createElection(db, idElection, ID_SIZE, question, "01/01/2023", "02/01/2023", "active");
        int election_LocalId = Election_getIdFromNumeroID(db, idElection, ID_SIZE);
        g_assert(election_LocalId == 1);

        // Creation des electeurs
        char idElecteur1[ID_SIZE] = "PXXXXXXXX1";
        char idElecteur2[ID_SIZE] = "PXXXXXXXX2";

        createElecteur(db, idElecteur1, ID_SIZE);
        createElecteur(db, idElecteur2, ID_SIZE);
        int electeur_n01 = getIdFromNumeroID(db, idElecteur1, ID_SIZE);
        int electeur_n02 = getIdFromNumeroID(db, idElecteur2, ID_SIZE);

        // Initialize and generate crypto keys
        mpz_t n, lambda, g, mu, encrypted_ballot;
        mpz_inits(n, lambda, g, mu, NULL);
        generate_keys(n, lambda, g, mu);

        // Check if keys are generated and print their values
        if (mpz_sgn(n) != 0 && mpz_sgn(lambda) != 0 && mpz_sgn(g) != 0 && mpz_sgn(mu) != 0) {
            gmp_printf("Keys generated successfully.\n");
            gmp_printf("n: %Zd\n", n);
            gmp_printf("lambda: %Zd\n", lambda);
            gmp_printf("g: %Zd\n", g);
            gmp_printf("mu: %Zd\n", mu);
        } else {
            g_print("Error: Key generation failed.\n");
            // Handle key generation failure
        }

        // Encrypt and cast votes
        mpz_init(encrypted_ballot);
        encrypt_vote(encrypted_ballot, 0x01, n, g); // Encrypt vote 1
        Election_castVote(db, electeur_n01, election_LocalId, mpz_export(NULL, NULL, 1, BALLOT_SIZE, 0, 0, encrypted_ballot), BALLOT_SIZE, "TODO");
        encrypt_vote(encrypted_ballot, 0x00, n, g); // Encrypt vote 2
        Election_castVote(db, electeur_n02, election_LocalId, mpz_export(NULL, NULL, 1, BALLOT_SIZE, 0, 0, encrypted_ballot), BALLOT_SIZE, "TODO");
        mpz_clear(encrypted_ballot);

        // Retrieve and decrypt votes
        sqlite3_stmt *stmt;
        const char *sql = "SELECT ballot FROM Vote WHERE idElection = ?;";
        int no = 0, yes = 0, total = 0;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, election_LocalId);

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                const void *ballotBlob = sqlite3_column_blob(stmt, 0);
                int blobSize = sqlite3_column_bytes(stmt, 0);

                // Decrypt the vote
                mpz_t decrypted_ballot;
                mpz_init(decrypted_ballot);
                mpz_import(encrypted_ballot, blobSize, 1, 1, 0, 0, ballotBlob);
                decrypt_vote(decrypted_ballot, encrypted_ballot, lambda, mu, n);

                char vote = (char)mpz_get_ui(decrypted_ballot);
                if (vote == 1) {
                    yes++;
                } else {
                    no++;
                }
                total++;

                mpz_clear(decrypted_ballot);
            }

            sqlite3_finalize(stmt);
        } else {
            fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
        }

        // Display results
        g_print("Total votes: %d, Yes: %d, No: %d\n", total, yes, no);


        //Election_processVotes(db, election_LocalId, lambda, mu, n, &no, &yes, &total);
        // Cleanup
        mpz_clears(n, lambda, g, mu, NULL);
        database_close(db);
    }

    g_free(file_path);
    g_option_context_free(context);
    return 0;
}
