#include "../common/include/circular_buffer.h"
#include "../common/include/serveur_vote.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* enqueueThread(void* arg) {
    CircularBuffer* cb = (CircularBuffer*)arg;
    Commande commandeAjoutBeatrice = {
            AJOUT_ELECTEUR,
            "111111",
            {"beatrice"}
    };
    Commande commandeSuppressionBeatrice = {
            SUPPRIME_ELECTEUR,
            "222222",
            {"beatrice"}
    };
    Commande commandeAjoutFrederika = {
            AJOUT_ELECTEUR,
            "333333",
            {"frederika"}
    };
    Commande commandeEstCeQueBeatriceExiste = {
            EST_PRESENT,
            "444444",
            {"beatrice"}
    };
    enqueue(cb, &commandeAjoutBeatrice);
    usleep(rand() % 400000);
    enqueue(cb, &commandeSuppressionBeatrice);
    usleep(rand() % 400000);
    enqueue(cb, &commandeAjoutFrederika);
    usleep(rand() % 400000);
    enqueue(cb, &commandeEstCeQueBeatriceExiste);
    return NULL;
}

void* dequeueThread(void* arg) {
    CircularBuffer* cb = (CircularBuffer*)arg;
    for (int i = 0; i < 4; ++i) {
        usleep(rand() % 800000 + 400000);
        Commande *commande;
        dequeue(cb, commande);
        printf("Dequeued: %u %s\n", commande->type, commande->commande.ajoutElecteur.identifiant);
        printf("Réponse reçue: %s\n", commande->commande.messageRetour.message);
    }
    return NULL;
}

int main() {
    srand((unsigned int)time(NULL));
    CircularBuffer cbCommandes, cbMessages;
    initCircularBuffer(&cbCommandes, 4);
    initCircularBuffer(&cbMessages, 4);

    printf("isFull: %d\n", isFull(&cbCommandes));
    printf("isEmpty: %d\n", isEmpty(&cbCommandes));

    pthread_t threadEnqueue, threadDequeue, threadTraiter;
    pthread_create(&threadEnqueue, NULL, enqueueThread, &cbCommandes);
    pthread_create(&threadDequeue, NULL, dequeueThread, &cbCommandes);
    pthread_create(&threadTraiter, NULL, traitementThread, &cbMessages);

    pthread_join(threadEnqueue, NULL);
    pthread_join(threadDequeue, NULL);
    pthread_join(threadTraiter, NULL);

    printf("isFull: %d\n", isFull(&cbCommandes));
    printf("isEmpty: %d\n", isEmpty(&cbCommandes));

    cleanupCircularBuffer(&cbCommandes);
    cleanupCircularBuffer(&cbMessages);
    return 0;
}