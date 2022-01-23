#ifndef ARBRES_H
#define ARBRES_H
#include "listes.h"
#include <stdbool.h>
#include <stdio.h>
/* Structure d'un noeud d'arbre */
struct noeud_s;
typedef struct noeud_s noeud;

/* Un arbre binaire est défini comme une référence vers le noeud racine de l'arbre.
 * Un arbre binaire vide est représenté par une référence NULL.
 * Un arbre binaire est une structure de données récursive.
 * Si il n'est pas vide, ses fils sont des arbres.
 */
typedef noeud* arbre;

struct noeud_s {
    char* valeur;
    arbre gauche;
    arbre droit;
};

typedef struct liste_arbre liste_arbre_t;

struct cellule_arbre {
  arbre value;
  int niveau;
  struct cellule_arbre* suivant;
};

typedef struct cellule_arbre cellule_arbre_t;

struct liste_arbre {
        cellule_arbre_t *tete;
};

typedef struct liste_arbre liste_arbre_t;

typedef struct liste_kurama liste_t_kurama;
/* Type de liste à compléter selon votre besoin. */


struct cellule_kurama {
        arbre *ab;
        struct cellule_kurama* suivant;
};

typedef struct cellule_kurama cellule_t_kurama;

struct liste_kurama {
        cellule_t_kurama *tete;
};

typedef struct liste_kurama liste_t_kurama;

/* Crée un nouveau nœud et initialise ses champs à null */
noeud* nouveau_noeud (void);

/* Construit un arbre depuis le fichier ouvert f.
 * Fonction récursive (s'appelle elle-même pour la lecture des fils
 * gauche et droit).
 */
arbre lire_arbre (FILE *f);

void enfiler(liste_arbre_t* l, arbre a, int niv);
arbre defiler(liste_arbre_t* l, int* x);
void ajouter_kurama(arbre *a,liste_t_kurama *L);
void cree_liste_espece(arbre *a, liste_t *L);
bool compare(liste_t *L , liste_t *T);
void arbre_a_dot(arbre a, FILE* f);


/* Macros pour de l'affichage uniquement si DEBUG est != 0 */
extern int DEBUG;

#define debug(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#endif
