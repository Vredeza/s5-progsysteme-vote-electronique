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

void init(){
    // j'initialise le serveur
}