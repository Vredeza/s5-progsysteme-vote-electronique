#include "../common/include/circular_buffer.h"
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
    }
    return NULL;
}

int main() {
    srand((unsigned int)time(NULL));
    CircularBuffer cb;
    initCircularBuffer(&cb, 4);

    printf("isFull: %d\n", isFull(&cb));
    printf("isEmpty: %d\n", isEmpty(&cb));

    pthread_t enqueueThreadID, dequeueThreadID;
    pthread_create(&enqueueThreadID, NULL, enqueueThread, &cb);
    pthread_create(&dequeueThreadID, NULL, dequeueThread, &cb);
    pthread_join(enqueueThreadID, NULL);
    pthread_join(dequeueThreadID, NULL);

    printf("isFull: %d\n", isFull(&cb));
    printf("isEmpty: %d\n", isEmpty(&cb));

    cleanupCircularBuffer(&cb);
    return 0;
}