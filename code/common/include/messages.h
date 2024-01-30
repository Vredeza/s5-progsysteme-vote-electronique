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
    char identifiant[ENTITY_ID_SIZE]; // à changer selon filtres
} ListeElecteursCmd;

typedef struct
{
    char NewIdentifiant[ENTITY_ID_SIZE];
    char OldIdentifiant[ENTITY_ID_SIZE];
} MAJElecteurCmd;

typedef struct
{
    Election *election;
} AjoutElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} LireElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE]; // inutile
} ListeElectionsCmd;

typedef struct
{
    Election election;
} MAJElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} ResultatElectionCmd;

typedef struct
{
    Vote vote;
} AjoutVoteCmd;

typedef struct
{
    char message[256];
} MessageRetour;

typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    LISTE_ELECTEUR,
    METTRE_A_JOUR_ELECTEUR,

    AJOUT_ELECTION,
    SUPPRIME_ELECTION,
    LIRE_ELECTION,
    LISTE_ELECTION,
    METTRE_A_JOUR_ELECTION,
    RESULTAT_ELECTION,

    AJOUT_VOTE,

    MESSAGE_RETOUR
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
        AjoutElectionCmd ajoutElection;
        SupprimeElectionCmd supprimeElection;
        LireElectionCmd lireElection;
        ListeElectionsCmd listeElection;
        MAJElectionCmd mettreAJourElection;
        ResultatElectionCmd resultatElection;
        AjoutVoteCmd ajoutVote;
        MessageRetour messageRetour;
    } commande;
} Commande;

#endif