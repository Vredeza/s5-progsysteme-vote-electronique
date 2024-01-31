#include "../common/include/votechain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/include/circular_buffer.h"
#include "../common/include/serveur_handlers.h"
#include "../common/include/serveur_vote.h"

#define TAILLE_TABLEAU_COMMANDES 8
#define TAILLE_TABLEAU_MESSAGES 8

sqlite3 *db;
CircularBuffer tableauCommandes, tableauMessages;
pthread_t traitementPid;

void pushCommande(Commande *commande){
    enqueue(&tableauCommandes, commande);
}

void pullCommande(Commande *commande){
    dequeue(&tableauMessages, commande);
}

int serverInit(const char *db_path){
    // initialisation de la base de données
    int needInit = database_exists(db_path);
    db = database_open(db_path);
    if (needInit == -1)
    {
        printf("La base de données n'existe pas.\n");
        printf("Initilisation de la base de données.\n");
        database_init(db);
    }

    // initialisation des tableaux circulaires
    initCircularBuffer(&tableauCommandes, TAILLE_TABLEAU_COMMANDES);
    initCircularBuffer(&tableauMessages, TAILLE_TABLEAU_MESSAGES);

    // exécution du thread de traitement
    if (pthread_create(&traitementPid, NULL, traitementThread, NULL) != 0){
        printf("Erreur lors de la création du thread\n");
        return 1;
    }
    return 0;
}

int serverStop(){
    if (pthread_cancel(traitementPid) != 0) {
        printf("Erreur lors de l'arrêt du thread\n");
        return 1;
    }
    printf("Serveur brutalement tué\n");
    return 0;
}

void* traitementThread(void* arg){
    while (1){
        Commande commande;
        dequeue(&tableauCommandes, &commande);
        char *messageRetour;
        handler(&commande, db, &messageRetour);

        Commande *commandeRetour = (Commande *)malloc(sizeof(Commande));
        memset(commandeRetour, 0, sizeof(Commande));
        commandeRetour->type = MESSAGE_RETOUR;
        strcpy(commandeRetour->signature, commande.signature);
        strcpy(commandeRetour->commande.messageRetour.message, messageRetour);

        enqueue(&tableauMessages, commandeRetour);
    }
}