#include "../common/include/votechain.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "serveur_handlers.h"
#include "serveur_vote.h"

int pushCommande(Commande* commande){
    return handle(commande);
}