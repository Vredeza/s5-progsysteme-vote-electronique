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
        Commande *commande = dequeue(cb);
        printf("Dequeued: %u %s\n", commande->type, commande->commande.ajoutElecteur.identifiant);
    }
    return NULL;
}

int main() {
    srand((unsigned int)time(NULL));
    CircularBuffer cb, cbMessages;
    initCircularBuffer(&cb, 4);
    initCircularBuffer(&cbMessages, 4);

    printf("isFull: %d\n", isFull(&cb));
    printf("isEmpty: %d\n", isEmpty(&cb));

    pthread_t threadEnqueue, threadDequeue, threadTraiter, threadMessages;
    pthread_create(&threadEnqueue, NULL, enqueueThread, &cbCommandes);
    pthread_create(&threadDequeue, NULL, dequeueThread, &cbCommandes);
    pthread_create(&threadTraiter, NULL, traitementThread, &cbMessages);
    pthread_create(&threadMessages, NULL, messageThread, &cbMessages);

    pthread_join(threadEnqueue, NULL);
    pthread_join(threadDequeue, NULL);
    pthread_join(threadTraiter, NULL);
    pthread_join(threadMessages, NULL);

    printf("isFull: %d\n", isFull(&cb));
    printf("isEmpty: %d\n", isEmpty(&cb));

    cleanupCircularBuffer(&cb);
    cleanupCircularBuffer(&cbMessages);
    return 0;
}