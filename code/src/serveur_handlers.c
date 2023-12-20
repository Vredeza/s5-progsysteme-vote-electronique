//
// Created by sanchezj on 20/12/23.
//

#include "../common/include/messages.h"
#include "serveur_handlers.h"
#include "stdio.h"

int NOPHandler(Commande* commande){
    printf("Je suis le handler NOP, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int ajoutElecteurHandler(Commande* commande){
    printf("Je suis le handler AjoutElecteur, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int suppressionElecteurHandler(Commande* commande){
    printf("Je suis le handler SuppressionElecteur, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int estPresentElecteurHandler(Commande* commande){
    printf("Je suis le handler EstPrésent, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int listeElecteurHandler(Commande* commande){
    printf("Je suis le handler ListeElecteurs, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int majElecteurHandler(Commande* commande){
    printf("Je suis le handler MAJElecteur, j'ai reçu une commande de signature %s", commande->signature);
    return 0;
}

int handler(Commande* commande){
    switch (commande->type) {
        case NOP: return NOPHandler(commande);
            break;
        case AJOUT_ELECTEUR: return ajoutElecteurHandler(commande);
            break;
        case SUPPRIME_ELECTEUR: return suppressionElecteurHandler(commande);
            break;
        case EST_PRESENT: return estPresentElecteurHandler(commande);
            break;
        case LISTE_ELECTEUR: return listeElecteurHandler(commande);
            break;
        case METTRE_A_JOUR_ELECTEUR: return majElecteurHandler(commande);
            break;
        default:
            printf("Commande non-reconnue");
            return 1;
    }
}