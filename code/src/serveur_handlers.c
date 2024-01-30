//
// Created by sanchezj on 20/12/23.
//

#include "../common/include/serveur_handlers.h"
#include "stdio.h"
#include "string.h"
#include <glib.h>
#include <stdlib.h>
#include <sys/stat.h>

void erreur(char* message) {
    printf("Erreur : %s\n", message);
}

int NOPHandler(){
    printf("Je suis le handler NOP, j'ai reçu une commande.\n");
    return 0;
}

/**
 * @return Positif si existe, négatif sinon
 */
int electionExiste(Election* election, sqlite3* db) {
    return Election_getIdFromNumeroID(db, election->identifiant, strlen(election->identifiant));
}

int ajoutElecteurHandler(AjoutElecteurCmd* commande, sqlite3* db, char* messageRetour){

    int electeur_extiste = electeurExists(db, commande->identifiant, strlen(commande->identifiant));

    if (!electeur_extiste) {
        printf("Création de l'électeur %s\n", commande->identifiant);
        createElecteur(db, commande->identifiant, strlen(commande->identifiant));
    } else {
        erreur("L'electeur existe déjà.");
        return 1;
    }

    return 0;
}

int suppressionElecteurHandler(SupprimeElecteurCmd * commande, sqlite3* db, char* messageRetour){

    int electeur_extiste = electeurExists(db, commande->identifiant, strlen(commande->identifiant));

    if (electeur_extiste){
        deleteElecteur(db, commande->identifiant, strlen(commande->identifiant));
        printf("Electeur supprimé.\n");
    } else {
        erreur("L'electeur n'existe pas.");
        return 1;
    }

    return 0;
}

int estPresentElecteurHandler(EstPresentCmd * commande, sqlite3* db, char* messageRetour){

    int electeur_extiste = electeurExists(db, commande->identifiant, strlen(commande->identifiant));

    if (electeur_extiste) {
        printf("L'electeur %s existe.\n", commande->identifiant);
    }
    else {
        printf("L'electeur %s n'existe pas.\n", commande->identifiant);
    }

    return 0;
}

int listeElecteurHandler(Commande * commande, sqlite3* db, char* messageRetour){

    char **tableauElecteurs;
    int nombreElecteurs;

    int result = listeElecteur(db, &tableauElecteurs, &nombreElecteurs);

    if (result) {
        // Utilisez le tableau d'électeurs comme nécessaire
        for (int i = 0; i < nombreElecteurs; i++) {
            //messageRetour += "Électeur numéroID : "+tableauElecteurs[i]+"\n";;
            printf("Électeur numéroID : %s\n", tableauElecteurs[i]);
            // N'oubliez pas de libérer la mémoire allouée pour chaque élément du tableau
            free(tableauElecteurs[i]);
        }

        // Libère la mémoire allouée pour le tableau d'électeurs
        free(tableauElecteurs);
    }else {
        erreur("Une erreur est survenue\n");
        return 1;
    }
    return 0;
}

int majElecteurHandler(MAJElecteurCmd * commande, sqlite3* db, char* messageRetour){
    //printf("Je suis le handler MAJElecteur, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int ajoutElectionHandler(AjoutElectionCmd * commande, sqlite3* db, char* messageRetour){
    int election_existe = electionExiste(commande->election, db);

    if (election_existe == -1) {
        createElection(
                db,
                commande->election->identifiant,
                strlen(commande->election->identifiant),
                commande->election->question,
                commande->election->dateDebut,
                commande->election->dateFin,
                status_str[commande->election->status]
        );
        printf("L'élection %s a bien été créée\n", commande->election->identifiant);

    } else {
        erreur("L'élection existe déjà.\n");
        return 1;
    }

    return 0;
}

int suppressionElectionHandler(SupprimeElectionCmd * commande, sqlite3* db, char* messageRetour){

    int election_id = Election_getIdFromNumeroID(db, commande->identifiant, strlen(commande->identifiant));

    if (election_id != -1) {
        deleteElection(
                db,
                election_id
        );
        printf("L'élection %s a bien été supprimée\n", commande->identifiant);

    } else {
        erreur("L'élection n'existe pas.\n");
        return 1;
    }

    return 0;

}

int lectureElectionHandler(LireElectionCmd * commande, sqlite3* db, char* messageRetour){
    //printf("Je suis le handler lectureElectionHandler, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int listeElectionHandler(ListeElectionsCmd * commande, sqlite3* db, char* messageRetour){
    //printf("Je suis le handler listeElectionHandler, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int majElectionHandler(MAJElectionCmd * commande, sqlite3* db, char* messageRetour){
    //printf("Je suis le handler majElectionHandler, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int resultatElectionHandler(ResultatElectionCmd * commande, sqlite3* db, char* messageRetour){

    return 0;
}

int ajoutVoteHandler(AjoutVoteCmd * commande, sqlite3* db, char* messageRetour){
    //printf("Je suis le handler ajoutVoteHandler, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

/*
 * Gère une commande passée en paramètre.
 * Stratégie : délègue le travail à un handler spécifique. Voir command pattern.
 */
int handler(Commande* commande, sqlite3* db, char* messageRetour){
    switch (commande->type) {
        case NOP: return NOPHandler();
            break;
        case AJOUT_ELECTEUR: return ajoutElecteurHandler((AjoutElecteurCmd*)&(commande->commande), db, messageRetour);
            break;
        case SUPPRIME_ELECTEUR: return suppressionElecteurHandler((SupprimeElecteurCmd *)&(commande->commande), db, messageRetour);
            break;
        case EST_PRESENT: return estPresentElecteurHandler((EstPresentCmd *)&(commande->commande), db, messageRetour);
            break;
        case LISTE_ELECTEUR: return listeElecteurHandler((ListeElecteursCmd *)&(commande->commande), db, messageRetour);
            break;
        case METTRE_A_JOUR_ELECTEUR: return majElecteurHandler((MAJElecteurCmd *)&(commande->commande), db, messageRetour);
            break;
        case AJOUT_ELECTION: return ajoutElectionHandler((AjoutElectionCmd *)&(commande->commande), db, messageRetour);
            break;
        case SUPPRIME_ELECTION: return suppressionElectionHandler((SupprimeElectionCmd *)&(commande->commande), db, messageRetour);
            break;
        case LIRE_ELECTION: return lectureElectionHandler((LireElectionCmd *)&(commande->commande), db, messageRetour);
            break;
        case LISTE_ELECTION: return listeElectionHandler((ListeElectionsCmd *)&(commande->commande), db, messageRetour);
            break;
        case METTRE_A_JOUR_ELECTION: return majElectionHandler((MAJElectionCmd *)&(commande->commande), db, messageRetour);
            break;
        case RESULTAT_ELECTION: return resultatElectionHandler((ResultatElectionCmd *)&(commande->commande), db, messageRetour);
            break;
        case AJOUT_VOTE: return ajoutVoteHandler((AjoutVoteCmd *)&(commande->commande), db, messageRetour);
            break;
        default:
            printf("Commande non-reconnue\n");
            return 1;
    }
}

