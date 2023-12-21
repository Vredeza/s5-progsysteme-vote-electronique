//
// Created by sanchezj on 20/12/23.
//

#include "../common/include/messages.h"
#include "../common/include/serveur_handlers.h"
#include "../common/include/bd.h"
#include "stdio.h"

int NOPHandler(Commande* commande){
    printf("Je suis le handler NOP, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int ajoutElecteurHandler(Commande* commande){
    printf("Je suis le handler AjoutElecteur, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int suppressionElecteurHandler(Commande* commande){
    printf("Je suis le handler SuppressionElecteur, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int estPresentElecteurHandler(Commande* commande){
    printf("Je suis le handler EstPrésent, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int listeElecteurHandler(Commande* commande){
    printf("Je suis le handler ListeElecteurs, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int majElecteurHandler(Commande* commande){
    printf("Je suis le handler MAJElecteur, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int Handler(Commande* commande){
    printf("Je suis le handler XXXXXX, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int ajoutElectionHandler(Commande* commande){
    printf("Je suis le handler ajoutElectionHandler, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int suppressionElectionHandler(Commande* commande){
    printf("Je suis le handler suppressionElectionHandler, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int lectureElectionHandler(Commande* commande){
    printf("Je suis le handler lectureElectionHandler, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int listeElectionHandler(Commande* commande){
    printf("Je suis le handler listeElectionHandler, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int majElectionHandler(Commande* commande){
    printf("Je suis le handler majElectionHandler, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int resultatElectionHandler(Commande* commande){
    printf("Je suis le handler resultatElectionHandler, j'ai reçu une commande de signature %s\n", commande->signature);
    return 0;
}

int ajoutVoteHandler(Commande* commande){
    printf("Je suis le handler ajoutVoteHandler, j'ai reçu une commande de signature %s\n", commande->signature);
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
        case AJOUT_ELECTION: return ajoutElectionHandler(commande);
            break;
        case SUPPRIME_ELECTION: return suppressionElectionHandler(commande);
            break;
        case LIRE_ELECTION: return lectureElectionHandler(commande);
            break;
        case LISTE_ELECTION: return listeElectionHandler(commande);
            break;
        case METTRE_A_JOUR_ELECTION: return majElectionHandler(commande);
            break;
        case RESULTAT_ELECTION: return resultatElectionHandler(commande);
            break;
        case AJOUT_VOTE: return ajoutVoteHandler(commande);
            break;
        default:
            printf("Commande non-reconnue");
            return 1;
    }
}