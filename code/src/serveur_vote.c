#include "../common/include/votechain.h"
#include <stdio.h>

#include "../common/include/circular_buffer.h"
#include "../common/include/serveur_handlers.h"
#include "../common/include/serveur_vote.h"

#define TAILLE_TABLEAU 8

sqlite3 *db;
CircularBuffer tableauEntree;
int stopTraitement = 0;

void pushCommande(Commande* commande){
    enqueue(&tableauEntree, commande);
}

void serverInit(const char *db_path){
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
}

void* traitementThread(void* arg){
    while (!stopTraitement) {
        Commande commande = *dequeue(&tableauEntree);
        int reponse = handler(&commande, db);
        //TODO mettre la réponse dans le buffer de sortie
        printf("Réponse: %d", reponse);
    }
    return NULL;
}