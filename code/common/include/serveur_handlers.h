//
// Created by sanchezj on 20/12/23.
//

#include "messages.h"
#include "bd.h"

#ifndef SERVEUR_HANDLERS_H
#define SERVEUR_HANDLERS_H

int handler(Commande* commande, sqlite3* db);

#endif //SERVEUR_HANDLERS_H