#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Client {
    int idClient;
    char nom[30];
    struct Client *suivant;
} Client;

typedef struct Transaction {
    char type[20];
    float montant;
    struct Transaction *suivant;
} Transaction;

typedef struct {
    int numCompte;
    float solde;
    Client *client;            
    Transaction *historique;
} Compte;


Compte comptes[100];
int nbComptes = 0;

Client *listeClients = NULL;


Client* rechercherClient(int id) {
    Client *tmp = listeClients;
    while (tmp != NULL) {
        if (tmp->idClient == id)
            return tmp;
        tmp = tmp->suivant;
    }
    return NULL;
}

Client* ajouterClient() {
    Client *c = (Client *)malloc(sizeof(Client));

    printf("ID client: ");
    scanf("%d", &c->idClient);

    if (rechercherClient(c->idClient) != NULL) {
        printf("Client deja existant\n");
        free(c);
        return NULL;
    }

    printf("Nom client: ");
    scanf("%s", c->nom);

    c->suivant = listeClients;
    listeClients = c;

    printf("Client ajoute avec succes\n");
    return c;
}


int rechercherCompte(int num, int i) {
    if (i == nbComptes)
        return -1;
    if (comptes[i].numCompte == num)
        return i;
    return rechercherCompte(num, i + 1);
}

void ajouterTransaction(Transaction **t, char type[], float montant) {
    Transaction *n = (Transaction *)malloc(sizeof(Transaction));
    strcpy(n->type, type);
    n->montant = montant;
    n->suivant = *t;
    *t = n;
}

void ajouterCompte() {
    Compte c;
    int idClient;

    printf("Numero du compte: ");
    scanf("%d", &c.numCompte);

    if (rechercherCompte(c.numCompte, 0) != -1) {
        printf("Compte deja existant\n");
        return;
    }

    printf("ID du client: ");
    scanf("%d", &idClient);

    Client *cl = rechercherClient(idClient);
    if (cl == NULL) {
        printf("Client introuvable, creation du client\n");
        cl = ajouterClient();
        if (cl == NULL)
            return;
    }

    c.client = cl;
    c.solde = 0;
    c.historique = NULL;

    comptes[nbComptes++] = c;
    printf("Compte cree avec succes\n");
}

void depot(int num, float montant) {
    int i = rechercherCompte(num, 0);
    if (i == -1) {
        printf("Compte introuvable\n");
        return;
    }

    comptes[i].solde += montant;
    ajouterTransaction(&comptes[i].historique, "Depot", montant);
    printf("Depot effectue\n");
}

void retrait(int num, float montant) {
    int i = rechercherCompte(num, 0);
    if (i == -1 || comptes[i].solde < montant) {
        printf("Operation impossible\n");
        return;
    }

    comptes[i].solde -= montant;
    ajouterTransaction(&comptes[i].historique, "Retrait", montant);
    printf("Retrait effectue\n");
}

void afficherTransactions(Transaction *t) {
    while (t != NULL) {
        printf("%s : %.2f\n", t->type, t->montant);
        t = t->suivant;
    }
}

void afficherCompte(int num) {
    int i = rechercherCompte(num, 0);
    if (i == -1) {
        printf("Compte introuvable\n");
        return;
    }

    printf("\nCompte: %d\n", comptes[i].numCompte);
    printf("Client: %s (ID %d)\n",
           comptes[i].client->nom,
           comptes[i].client->idClient);
    printf("Solde: %.2f\n", comptes[i].solde);
    printf("Historique:\n");
    afficherTransactions(comptes[i].historique);
}

void afficherClients() {
    Client *tmp = listeClients;
    if (!tmp) {
        printf("Aucun client\n");
        return;
    }

    while (tmp) {
        printf("ID: %d | Nom: %s\n", tmp->idClient, tmp->nom);
        tmp = tmp->suivant;
    }
}


int main() {
    int choix, num;
    float montant;

    do {
        printf("\n MENU BANCAIRE \n");
        printf("1. Ajouter client\n");
        printf("2. Ajouter compte\n");
        printf("3. Depot\n");
        printf("4. Retrait\n");
        printf("5. Afficher compte\n");
        printf("6. Afficher clients\n");
        printf("0. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterClient();
                break;
            case 2:
                ajouterCompte();
                break;
            case 3:
                printf("Numero compte: ");
                scanf("%d", &num);
                printf("Montant: ");
                scanf("%f", &montant);
                depot(num, montant);
                break;
            case 4:
                printf("Numero compte: ");
                scanf("%d", &num);
                printf("Montant: ");
                scanf("%f", &montant);
                retrait(num, montant);
                break;
            case 5:
                printf("Numero compte: ");
                scanf("%d", &num);
                afficherCompte(num);
                break;
            case 6:
                afficherClients();
                break;
            case 0:
                printf("Au revoir\n");
                break;
            default:
                printf("Choix invalide\n");
        }
    } while (choix != 0);

    return 0;
}

