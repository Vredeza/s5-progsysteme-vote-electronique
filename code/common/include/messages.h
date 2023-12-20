#ifndef MESSAGE_H
#define MESSAGE_H

#include "protocol.h"

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} AjoutElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} EstPresentCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} ListeElecteursCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} MAJElecteurCmd;

typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    LISTE_ELECTEUR,
    METTRE_A_JOUR_ELECTEUR
} CommandType;

//--
typedef struct
{
    CommandType type;
    char signature[256]; // la signature de la commande
    union
    {
        AjoutElecteurCmd ajoutElecteur;
        SupprimeElecteurCmd supprimeElecteur;
        EstPresentCmd estPresent;
        ListeElecteursCmd listeElecteur;
        MAJElecteurCmd mettreAJourElecteur;
    } commande;
} Commande;

#endif