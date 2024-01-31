#include "../common/include/serveur_vote.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define V if (verbose)
#define ID_SIZE 10
#define BALLOT_SIZE 1

void printCommandeAddresss(Commande *commande) {
    printf("Adresse mémoire de la commande : %p\n", (void *)commande);
}
int main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    // Définition des options
    gboolean verbose = FALSE;
    gchar *file_path = NULL;
    GOptionEntry entries[] = {
            {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
            {"file", 'f', 0, G_OPTION_ARG_STRING, &file_path, "Chemin du fichier", "FILE"},
            {NULL}};

    // Création d'un contexte d'option
    context = g_option_context_new("- Test d'utilisation du serveur");
    g_option_context_add_main_entries(context, entries, NULL);

    // Analyse des options de ligne de commande
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    // Affichage des résultats
    if (verbose)
    {
        g_print("Mode verbeux activé\n");
    }
    if (file_path != NULL) {
        serverInit(file_path);

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

        // je pousse toutes les commandes dans le buffer de commandes
        pushCommande(&commandeAjoutBeatrice);
        pushCommande(&commandeAjoutBeatrice);// Affichera une erreur parce que Béatrice existe déjà
        pushCommande(&commandeEstCeQueBeatriceExiste);
        pushCommande(&commandeSuppressionBeatrice);
        pushCommande(&commandeSuppressionBeatrice);
        pushCommande(&commandeAjoutFrederika);
        pushCommande(&commandeEstCeQueBeatriceExiste);

        // on attend que le serveur réfléchisse
        sleep(3);

        // on tire 7 messages (le 7 coincide avec le nombre de commandes qu'on a poussé) du buffer de messages et on print
        for (int i = 0; i < 7; i++) {
            Commande *commande = malloc(sizeof(Commande));
            pullCommande(commande);
            printf("Retour: %s\n", commande->commande.messageRetour.message);
            free(commande);
        }

        // on assassine le serveur
        serverStop();
    }
    else
    {
        g_print("Aucun chemin de fichier fourni\n");
    }

    // Libération de la mémoire
    g_free(file_path);
    g_option_context_free(context);
    return 0;
}