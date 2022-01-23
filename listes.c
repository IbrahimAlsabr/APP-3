#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"


void init_liste_vide(liste_t *L) 
{
  cellule_t *cel = malloc(sizeof(cellule_t));
  cel = NULL;
  L->tete = cel;
}

/* retourne 0 si OK, 1 sinon  */
int ajouter_tete(liste_t* L, string c) 
{ 
  cellule_t *cel = malloc(sizeof(cellule_t));
  cel->val = c;
  cel->suivant = L->tete;
  L->tete = cel;
  if (L->tete->val != c)
  {
    return 1;
  }
    return 0;
}

void ajouter_normale(liste_t *L, string c)
{
  if (L->tete == NULL) 
  {
    ajouter_tete(L, c);
  } 
  else
  {
    cellule_t *cel = malloc(sizeof(cellule_t));
    cel->val = c;
    cel->suivant=NULL;
    cellule_t *pointeur = L->tete;
    while (pointeur->suivant != NULL) 
    {
      pointeur = pointeur->suivant;
    }
      pointeur->suivant=cel;
  }
}

int compteur(liste_t *L) 
{
  int res = 0;
  if (L->tete == NULL) 
  {
    return res;
  } 
  else 
  {
    cellule_t *pointeur = L->tete;
    while (pointeur != NULL) 
    {
      res++;
      pointeur = pointeur->suivant;
    }
  }
  return res;
}

