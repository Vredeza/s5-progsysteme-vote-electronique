//
// Created by sanchezj on 20/12/23.
//

#ifndef SERVEUR_VOTE_H
#define SERVEUR_VOTE_H

#include "./serveur_handlers.h"

void pushCommande(Commande* commande);
int serverInit(const char *db_path);
void* traitementThread(void* arg);

#endif //SERVEUR_VOTE_H
