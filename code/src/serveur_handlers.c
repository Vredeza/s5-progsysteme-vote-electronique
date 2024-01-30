//
// Created by sanchezj on 20/12/23.
//

#include "../common/include/serveur_handlers.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>


/**
 * Formate une chaîne de caractères, à la manière de printf().
 * @param formatString La chaîne de caractères (avec des %s, par exemple).
 * @param ... Les valeurs qui remplaceront les ancres dans la chaîne de caractères.
 * @return La chaîne de caractères formatée.
 */
char* format(const char* formatString, ...) {
    // Initialiser la liste variable d'arguments
    va_list args;
    va_start(args, formatString);

    // Déterminer la taille nécessaire pour la chaîne formatée
    int bufferSize = vsnprintf(NULL, 0, formatString, args);

    // Terminer l'utilisation de la liste variable d'arguments
    va_end(args);

    // Allouer de la mémoire pour la chaîne formatée
    char* formattedMessage = (char*)malloc(bufferSize + 1);

    // Réinitialiser la liste variable d'arguments
    va_start(args, formatString);

    // Formater la chaîne et stocker le résultat dans formattedMessage
    vsnprintf(formattedMessage, bufferSize + 1, formatString, args);

    // Terminer l'utilisation de la liste variable d'arguments
    va_end(args);

    return formattedMessage;
}

/**
 * Envoi le message de retour au buffer approprié
 * @param message Le message a renvoyer.
 */
void retour(char* message, char* messageRetour){

    messageRetour = (char *)malloc(strlen(message) + 1);
    strcpy(messageRetour, message);

    printf("%s", messageRetour);

    free(message);
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
        retour(format("Création de l'électeur %s", commande->identifiant), messageRetour);
        createElecteur(db, commande->identifiant, strlen(commande->identifiant));
    } else {
        retour("L'electeur existe déjà.", messageRetour);
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
        retour("L'electeur n'existe pas.",messageRetour);
        return 1;
    }

    return 0;
}

int estPresentElecteurHandler(EstPresentCmd * commande, sqlite3* db, char* messageRetour){

    int electeur_extiste = electeurExists(db, commande->identifiant, strlen(commande->identifiant));

    if (electeur_extiste) {
        retour(format("L'electeur %s existe.", commande->identifiant), messageRetour);
    }
    else {
        retour(format("L'electeur %s n'existe pas.", commande->identifiant), messageRetour);
    }

    return 0;
}

int listeElecteurHandler(ListeElecteursCmd * commande, sqlite3* db, char* messageRetour){

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
        return 1;
    }
    return 0;
}

int majElecteurHandler(MAJElecteurCmd * commande, sqlite3* db, char* messageRetour){
    int nouvelleIDDejaExistant= electeurExists(db, commande->NewIdentifiant, strlen(commande->NewIdentifiant));

    if (nouvelleIDDejaExistant) {
        retour(format("L'Id %s est déjà occupé.", commande->NewIdentifiant), messageRetour);
    }
    else {
        updateElecteur(db, commande->OldIdentifiant, strlen(commande->OldIdentifiant), commande->NewIdentifiant, strlen(commande->NewIdentifiant));
        retour("L'Id a été modifié", messageRetour);
    }

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
        retour(format("L'élection %s a bien été créée", commande->election->identifiant), messageRetour);

    } else {
        retour("L'élection existe déjà.", messageRetour);
        return 1;
    }

    return 0;
}

int suppressionElectionHandler(SupprimeElectionCmd * commande, sqlite3* db, char* messageRetour){

    int election_id = Election_getIdFromNumeroID(db, commande->identifiant, strlen(commande->identifiant));

    if (election_id == -1) { // si ça existe pass
        retour("L'élection n'existe pas.", messageRetour);
        return 1;
    } else {
        deleteElection(
                db,
                election_id
        );
        retour(format("L'élection %s a bien été supprimée", commande->identifiant), messageRetour);
    }

    return 0;

}

int lectureElectionHandler(LireElectionCmd * commande, sqlite3* db, char* messageRetour){
    int election_id = Election_getIdFromNumeroID(db, commande->identifiant, strlen(commande->identifiant));

    if (election_id == -1) { // si ça existe pass
        retour("L'élection n'existe pas.", messageRetour);
        return 1;
    } else {
        Election election;
        readElection(db, election_id, &election);

        retour(format(
                "ID: %s,\n"
                "Question: %s,\n"
                "Date de début: %s,\n"
                "Date de fin: %s,\n"
                "Statut: %s\n",
                election.identifiant,
                election.question,
                election.dateDebut,
                election.dateFin,
                status_str[election.status]
                ),
               messageRetour
                );
    }
    return 0;
}

int listeElectionHandler(ListeElectionsCmd * commande, sqlite3* db, char* messageRetour){

    Election *elections;
    int numElections;
    listeElection(db, &elections, &numElections);

    for (int i = 0; i < numElections; i++) {
        retour(format(
                "ID: %s,\n"
                "Question: %s,\n"
                "Date de début: %s,\n"
                "Date de fin: %s,\n"
                "Statut: %s\n",
                elections[i].identifiant,
                elections[i].question,
                elections[i].dateDebut,
                elections[i].dateFin,
                status_str[elections[i].status]
        ),
               messageRetour);
    }

    return 0;
}

int majElectionHandler(MAJElectionCmd * commande, sqlite3* db, char* messageRetour){

    int election_id = Election_getIdFromNumeroID(db, commande->election.identifiant, strlen(commande->election.identifiant));

    if (election_id == -1) { // si ça existe pass
        retour("L'élection n'existe pas.", messageRetour);
        return 1;
    } else {
        updateElection(db, election_id, commande->election.question);

        retour(format("L'élection %s a bien été modifié", commande->election.identifiant), messageRetour);
    }

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
            retour("Message non reconnu", messageRetour);
            return 1;
    }
}

