#include "arbres.h"
#include "listes.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

noeud* nouveau_noeud(void)
{
    noeud *n = (noeud*)malloc(sizeof(noeud));
    assert (n!=NULL);
    n->valeur = NULL;
    n->gauche = NULL;
    n->droit  = NULL;
    return n;
}


/* buffer pour lire les caractères des espèces sous forme de "mots" (words) */
#define MAX_WORD_SIZE 255
char buffer[MAX_WORD_SIZE+1];

/* Variable globale qui contient le prochain caractère à traiter */
static char next_char = ' ';

/* Supprime tous les espaces, tabulations, retour à la ligne */
#define GLOB(f) \
    while(isspace(next_char)) { \
        next_char = fgetc(f);\
    }


/* Fonction récursive qui lit un sous-arbre */
/* Appelée une fois à la racine (debut du fichier), puis récursivement
 * pour chaque nœud interne rencontré. */
arbre lire_arbre (FILE *f)
{
    arbre racine;

    GLOB(f); /* lit dans next_char le premier caractère non vide */

    if (next_char == '/') {
        next_char = ' '; /* on ne garde pas '/' en mémoire */
        return NULL;
    }

    if (next_char == ')') {
        return NULL;
    }

    if (next_char != '(') {
        fprintf(stderr, "Error while reading binary tree : '(' or ')' expected at position %ld\n", ftell(f));
        exit (1);
    }

    /* On remplit le buffer tant qu'on lit des caractères alphanumériques */
    char *p = buffer; /* début du buffer */
    next_char = ' '; GLOB(f);

    do {
        *p = next_char;       /* sauvegarde du char courant */
        next_char = fgetc(f);
        p++;
        assert (p < buffer + MAX_WORD_SIZE);
    } while (! isspace (next_char) && next_char != '(' && next_char != ')');
    /* on arrète si le char suivant est un espace ou une parenthèse */
    *p='\0'; /* on ferme la chaîne de caractères dans le buffer */

    racine = nouveau_noeud();
    racine->valeur = strdup(buffer); /* dupliquer le mot lu */

    GLOB(f);

    if (next_char == ')') {
        next_char = ' '; /* on est sur une feuille, on prépare la lecture du prochain nœud */
    }
    else {
        racine->gauche = lire_arbre (f); /* appel récursif pour le fils gauche */
        racine->droit  = lire_arbre (f); /* idem pour le droit */

        GLOB(f); /* lit jusqu'au ')' fermant */

        if (next_char != ')') {
            fprintf(stderr, "Error while reading binary tree: ')' expected\n");
            exit(1);
        }
        next_char = ' '; /* on ne garde pas la parenthèse en mémoire */
    }
    return racine;
}


void enfiler(liste_arbre_t* l, arbre a, int niv)
{
  cellule_arbre_t *cell = malloc(sizeof(cellule_arbre_t));
  if (l->tete == NULL)
  {
    cell->value = a;
    cell->niveau = niv;
    cell->suivant = NULL;
    l->tete = cell;
  }
  else {
    cellule_arbre_t *cel;
    cel = l->tete;
    while (cel->suivant != NULL)
    {
      cel = cel->suivant;
    }
    cell->value = a;
    cell->niveau = niv;
    cell->suivant = NULL;
    cel->suivant = cell;
  }
}

arbre defiler(liste_arbre_t* l, int* x)
{
  assert(l->tete != NULL);
  cellule_arbre_t* cel = l->tete;
  *x = l->tete->niveau;
  l->tete = l->tete->suivant;
  return cel->value;
}


void cree_liste_espece(arbre *a, liste_t *L) {
  if ((*a) != NULL) {
    if ((*a)->gauche == NULL && (*a)->droit == NULL) {
      ajouter_normale(L, (*a)->valeur);
    }
    if ((*a)->gauche != NULL) {
      cree_liste_espece(&(*a)->gauche, L);
    }
    if ((*a)->droit != NULL) {
      cree_liste_espece(&(*a)->droit, L);
    }
  }
}
void ajouter_kurama(arbre *a, liste_t_kurama *L) {

  cellule_t_kurama *cel = malloc(sizeof(cellule_t_kurama));
  cel->ab = a;
  cel->suivant = NULL;
  if (L->tete == NULL) {
    L->tete = cel;
  } else {
    cellule_t_kurama *pointeur;
    pointeur = L->tete;
    while (pointeur->suivant != NULL) {
      pointeur = pointeur->suivant;
    }
    pointeur->suivant = cel;
  }
}

bool compare(liste_t *L, liste_t *T) 
{
  if (L->tete == NULL && T->tete == NULL) 
  {
    return true;
  }
  else 
  {
    cellule_t *cel = L->tete;
    cellule_t *cel2 = T->tete;
    while (cel != NULL && cel2 != NULL) 
    {
      if (strcmp(cel->val, cel2->val) != 0) 
      {
        return false;
      }

      cel = cel->suivant;
      cel2 = cel2->suivant;
    }
  }
  return true;
}

/* acte I-B */
void arbre_a_dot(arbre a, FILE* f)
{
  if (a != NULL)
  {
    if (a->droit == NULL && a->gauche == NULL)
    {
      fprintf(f,"%s\n",a->valeur);
    }
    else
    {
      if (a->gauche != NULL)
      {
        fprintf(f,"%s -> %s [label = \"non\"]\n",a->valeur,a->gauche->valeur);
        arbre_a_dot(a->gauche,f);
      }
      if (a->droit != NULL)
      {
        fprintf(f,"%s -> %s [label = \"oui\"]\n",a->valeur,a->droit->valeur);
        arbre_a_dot(a->droit,f);
      }
    }
  }
}
