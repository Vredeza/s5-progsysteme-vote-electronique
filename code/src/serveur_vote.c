#include "../common/include/votechain.h"
#include <stdio.h>

#include "../common/include/circular_buffer.h"
#include "../common/include/serveur_handlers.h"
#include "../common/include/serveur_vote.h"

#define TAILLE_TABLEAU 8

sqlite3 *db;
CircularBuffer tableauEntree;
pthread_t traitementPid;
int stopTraitement = 0;

void pushCommande(Commande* commande){
    enqueue(&tableauEntree, commande);
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

    // initialisation du tableau de commandes
    initCircularBuffer(&tableauEntree, TAILLE_TABLEAU);

    // Exécution du thread de traitement
    if (pthread_create(&traitementPid, NULL, traitementThread, NULL) != 0){
        printf("Erreur lors de la création du thread\n");
        return 1;
    }

    // Attente de la fin du thread de traitement
    if (pthread_join(traitementPid, NULL) != 0){
        fprintf(stderr, "Erreur lors du join du thread\n");
        return 1;
    }
}

void* traitementThread(void* arg){
    CircularBuffer* cbMessage = (CircularBuffer*) arg;
    while (!stopTraitement) {
        Commande commande = *dequeue(&tableauEntree);
        int reponse = handler(&commande, db);
        sprintf(commande.commande.messageRetour.message, "Réponse: %d", reponse);
        enqueue(cbMessage, &commande);
    }
    pthread_exit(NULL);
}