#include "../common/include/votechain.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../common/include/serveur_handlers.h"
#include "../common/include/serveur_vote.h"

#define TAILLE_FILE_ENTREE = 8

int pushCommande(Commande* commande){
    return handler(commande);
}

void serverInit(const char *db_path){
    int needInit = database_exists(db_path);
    sqlite3 *db = database_open(db_path);
    if (needInit == -1)
    {
        printf("La base de données n'existe pas.\n");
        printf("Initilisation de la base de données.\n");
        database_init(db);
    }
}