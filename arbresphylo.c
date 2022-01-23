#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"

void analyse_arbre(arbre racine, int *nb_esp, int *nb_carac) 
{
  int nb_espg = 0;
  int nb_caracg = 0;
  int nb_espd = 0;
  int nb_caracd = 0;
  if (racine != NULL) 
  {
    if (racine->droit != NULL && racine->gauche != NULL) 
    {
      analyse_arbre(racine->gauche, &nb_espg, &nb_caracg);
      analyse_arbre(racine->droit, &nb_espd, &nb_caracd);
      nb_caracd = nb_caracd + 1;
    } 
    else 
    {
      if (racine->gauche == NULL && racine->droit == NULL) 
      {
        nb_espd = nb_espd + 1;
      } 
      else 
      {
        if (racine->gauche == NULL) 
        {
          analyse_arbre(racine->droit, &nb_espd, &nb_caracd);
        } 
        else 
        {
          analyse_arbre(racine->gauche, &nb_espg, &nb_caracg);
        }
          nb_caracg = nb_caracg + 1;
      }
    }
  }
        *nb_carac = nb_caracd + nb_caracg;
        *nb_esp = nb_espd + nb_espg;
}



/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */

int rechercher_espece(arbre racine, char *espece, liste_t *seq) 
{
  if (racine != NULL)
  {
    if (strcmp(racine->valeur, espece) == 0)
    {
      return 0;
    }
    else
    {
      if (racine->droit != NULL)
      {
        int res = rechercher_espece(racine->droit, espece, seq);
        if (res == 0)
        {
          ajouter_tete(seq,racine->valeur);
          return 0;
        }
      }

      if (racine->gauche != NULL)
      {
        int res = rechercher_espece(racine->gauche, espece, seq);
        if(res == 0)
        {
          return 0;
        }
      }
    }
  }

  return 1;
}



/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un 
 * message d'erreur.
 */

int ajouter_espece(arbre *a, char *espece, cellule_t *seq) 
{
  if (*a == NULL)
  {
    if (seq != NULL)
    {
      (*a) = nouveau_noeud();
      (*a)->valeur = seq->val;
      return ajouter_espece(&((*a)->droit), espece, seq->suivant);
    }
    else {
      (*a) = nouveau_noeud();
      (*a)->valeur = espece;
      return 0;
    }
  }
  else
  {
    if (seq == NULL)
    {
      if ((*a)->gauche != NULL)
      {
        printf("Ne peut ajouter %s: possède les mêmes caractères que %s",espece,(*a)->gauche->valeur);
      }
      else
      {
        if ((*a)->droit == NULL && (*a)->gauche == NULL)
        {
          printf("Ne peut ajouter %s: possède les mêmes caractères que %s",espece,(*a)->valeur);
        }
        else
        {
          (*a)->gauche = nouveau_noeud();
          (*a)->gauche->valeur = espece;
          return 0;
        }
      }
    }
    else
    {
      if (strcmp((*a)->valeur, seq->val) == 0)
      {
        return ajouter_espece(&((*a)->droit), espece, seq->suivant);
      }
      else
      {
        if ((*a)->droit == NULL && (*a)->gauche == NULL)
        {
          string tmp;
          tmp = (*a)->valeur;
          (*a)->valeur = seq->val;
          (*a)->gauche = nouveau_noeud();
          (*a)->gauche->valeur = tmp;
          return ajouter_espece(&((*a)->droit), espece, seq->suivant);
        }
        else
        {
          return ajouter_espece(&((*a)->gauche), espece, seq);
        }
      }
    }
  }
  return 1;
}


void afficher_par_niveau(arbre racine, FILE* fout)
{
  liste_arbre_t file = {.tete = NULL};
  int niv = 1; // le niveau actuel
  int x; // le niveau du caracteristique gere
  enfiler(&file, racine, niv);
  while(file.tete != NULL)
  {
    arbre tree = defiler(&file, &x);
    if (tree != NULL && (tree->gauche != NULL || tree->droit != NULL))
    {
      if (x != niv)
      {
        fprintf(fout, "\n%s ", tree->valeur);
        niv = x;
      }
      else
      {
        fprintf(fout, "%s ", tree->valeur);
      }
      if (tree->gauche != NULL && tree->droit != NULL)
      {
        enfiler(&file, tree->gauche, niv + 1);
        enfiler(&file, tree->droit, niv + 1);
      }
      else
      {
        if (tree->gauche != NULL)
        {
          enfiler(&file, tree->gauche, niv + 1);
        }
        if (tree->droit != NULL)
        {
          enfiler(&file, tree->droit, niv + 1);
        }
      }
    }
  }
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */

// Acte 4
int rechercher_espece_2(arbre *racine, char *espece, liste_t_kurama *seq) 
{
  if (racine != NULL) 
  {
    if (strcmp((*racine)->valeur, espece) == 0) 
    {
      return 0;
    } else 
    {
      if ((*racine)->gauche != NULL) 
      {
        int res = rechercher_espece_2(&(*racine)->gauche, espece, seq);
        if (res == 0) 
        {
          ajouter_kurama(racine, seq);
          return 0;
        }
      }

      if ((*racine)->droit != NULL) 
      {
        int res = rechercher_espece_2(&(*racine)->droit, espece, seq);
        if (res == 0) 
        {
          ajouter_kurama(racine, seq);
          return 0;
        }
      }
    }
  }
  return 1;
}

int ajouter_carac(arbre *arb, char *carac, cellule_t *seq) 
{
  if ((*arb) == NULL || seq == NULL) 
  {
    return 0;
  }

  if ((*arb)->gauche == NULL && (*arb)->droit == NULL) 
  {
    arbre tree = nouveau_noeud();
    (tree)->valeur = (*arb)->valeur;
    (tree)->gauche = (*arb)->gauche;
    (tree)->droit = (*arb)->droit;
    (*arb)->valeur = carac;
    (*arb)->gauche = NULL;
    (*arb)->droit = tree;
    return 1;
  }
  int nb_esp;
  int nb_carac;
  analyse_arbre(*arb, &nb_esp, &nb_carac);
  liste_t_kurama *L = malloc(sizeof(liste_t_kurama));
  L->tete = NULL;
  int pre = rechercher_espece_2(arb, seq->val, L);
  if (pre == -1) 
  {
    return -1;
  } 

  liste_t *B = malloc(sizeof(liste_t));
  B->tete = seq;
  cellule_t_kurama *ptr_1 = L->tete;
  cellule_t_kurama *ptr_2 = L->tete;
  if (seq->suivant == NULL) 
  /*cas speciale , kayen ghir une seul esepce 'voir si
    elle est est colle a la racine ne contient que elle'*/
  {
    /*cas ou l'arbre ne contien qu un seul noeud et une seul espece :*/
    if (L->tete->suivant == NULL) 
    {
      if ((*arb)->droit != NULL && (*arb)->gauche == NULL) 
      {
        if (strcmp((*arb)->droit->valeur, seq->val) == 0) 
        {
          arbre tree = nouveau_noeud();
          (tree)->valeur = (*arb)->valeur;
          (tree)->gauche = (*arb)->gauche;
          (tree)->droit = (*arb)->droit;
          (*arb)->valeur = carac;
          (*arb)->gauche = NULL;
          (*arb)->droit = tree;
        }
        printf("on a %s ensuite %s", (*arb)->valeur, (*arb)->droit->valeur);
        return 1;
      }

      if ((*arb)->gauche != NULL && (*arb)->droit == NULL) 
      {
        if (strcmp((*arb)->gauche->valeur, seq->val) == 0) 
        {
          arbre tree = nouveau_noeud();
          (tree)->valeur = (*arb)->valeur;
          (tree)->gauche = (*arb)->gauche;
          (tree)->droit = (*arb)->droit;
          (*arb)->valeur = carac;
          (*arb)->gauche = NULL;
          (*arb)->droit = tree;

          return 1;
        }
      }

      if (strcmp((*arb)->droit->valeur, seq->val) == 0) 
      {
        arbre tree = nouveau_noeud();

        (tree)->valeur = carac;
        (tree)->gauche = NULL;
        tree->droit = (*ptr_1->ab)->droit;
        (*ptr_1->ab)->droit = tree;
        return 1;
      }
      if (strcmp((*arb)->gauche->valeur, seq->val) == 0) 
      {
        arbre tree = nouveau_noeud();
        (tree)->valeur = carac;
        (tree)->gauche = NULL;
        tree->droit = (*ptr_1->ab)->gauche;
        (*ptr_1->ab)->gauche = tree;
        return 1;
      }
    } 
    else
    {
      if ((*arb)->droit != NULL) 
      {
        if (strcmp((*ptr_1->ab)->droit->valeur, seq->val) == 0) 
        {
          liste_t *SS = malloc(sizeof(liste_t));
          SS->tete = NULL;
          cree_liste_espece(arb, SS);

          if (compteur(SS) == 1) 
          {
            arbre tree = nouveau_noeud();
            (tree)->valeur = (*arb)->valeur;
            (tree)->gauche = (*arb)->gauche;
            (tree)->droit = (*arb)->droit;
            (*arb)->valeur = carac;
            (*arb)->gauche = NULL;
            (*arb)->droit = tree;
          } else {
            arbre tree = nouveau_noeud();
            (tree)->valeur = carac;
            (tree)->gauche = NULL;
            tree->droit = (*ptr_1->ab)->droit;
            (*ptr_1->ab)->droit = tree;
          }
          return 1;
        }
      }
      if ((*arb)->gauche != NULL) 
      {
        if (strcmp((*ptr_1->ab)->gauche->valeur, seq->val) == 0) 
        {
          /*faire ici la meme chose que en haut si on a qu une espece dans tout l'arbre */
          arbre tree = nouveau_noeud();
          (tree)->valeur = carac;
          (tree)->gauche = NULL;
          tree->droit = (*ptr_1->ab)->gauche;
          (*ptr_1->ab)->gauche = tree;
          return 1;
        }
      } 

      if (strcmp((*ptr_1->ab)->droit->valeur, seq->val) == 0) 
      {
        arbre tree = nouveau_noeud();
        (tree)->valeur = carac;
        (tree)->gauche = NULL;
        tree->droit = (*ptr_1->ab)->droit;
        (*ptr_1->ab)->droit = tree;
        return 1;
      }

      if (strcmp((*ptr_1->ab)->gauche->valeur, seq->val) == 0)
      {
        arbre tree = nouveau_noeud();
        (tree)->valeur = carac;
        (tree)->gauche = NULL;
        tree->droit = (*ptr_1->ab)->gauche;
        (*ptr_1->ab)->gauche = tree;
        return 1;
      }
    }
  }

  while (ptr_1 != NULL) 
  {
    liste_t *S = malloc(sizeof(liste_t));
    S->tete = NULL;
    cree_liste_espece(ptr_1->ab, S);

    if (compteur(B) == compteur(S)) 
    {
      if (compare(S, B)) 
      {
        if (ptr_1->suivant == NULL) 
        {
          arbre tree = nouveau_noeud();
          (tree)->valeur = (*arb)->valeur;
          (tree)->gauche = (*arb)->gauche;
          (tree)->droit = (*arb)->droit;
          (*arb)->valeur = carac;
          (*arb)->gauche = NULL;
          (*arb)->droit = tree;
          return 1;
        }

        if (ptr_1->suivant != NULL) 
        {
          arbre tree = nouveau_noeud();
          (tree)->valeur = carac;
          (tree)->gauche = NULL;

          if ((*ptr_1->suivant->ab)->gauche == (*ptr_1->ab)) 
          {
            (tree)->droit = (*ptr_1->suivant->ab)->gauche;
            (*ptr_1->suivant->ab)->gauche = tree;
            analyse_arbre(*arb, &nb_esp, &nb_carac);
            return 1;
          } 
          
          else if ((*ptr_1->suivant->ab)->droit == (*ptr_1->ab)) 
          {
            (tree)->droit = (*ptr_1->suivant->ab)->droit;
            (*ptr_1->suivant->ab)->droit = tree;
            analyse_arbre(*arb, &nb_esp, &nb_carac);
            return 1;
          }
        }
      }
    }
    ptr_2 = ptr_1;
    ptr_1 = ptr_1->suivant;
  }

  return 0;
}


void affiche_arbre_aux(noeud *n_1,noeud *n_2) 
{
  if (n_1!= NULL) 
  {
    if (n_1->gauche == NULL && n_1->droit == NULL) 
    {
      liste_t L={ .tete=NULL };
      init_liste_vide(&L) ;
      printf("%s :",n_1->valeur);
      rechercher_espece(n_2,n_1->valeur, &L);
      cellule_t *ptr_1 = L.tete;
      
      while (ptr_1 != NULL)
      {
        printf("%s ",ptr_1->val);
        ptr_1 = ptr_1->suivant;
      }
        printf("\n");
    } 
    else 
    {
      if (n_1->gauche != NULL && n_1->droit != NULL) 
      {
        affiche_arbre_aux(n_1->gauche,n_2);
        affiche_arbre_aux(n_1->droit,n_2);
      } 
      else if (n_1->gauche != NULL) 
      {
        affiche_arbre_aux(n_1->gauche,n_2);
      } 
      else if (n_1->droit != NULL) 
      {
        affiche_arbre_aux(n_1->droit,n_2);
      }
    }
  }
}


void affiche_arbre(noeud *n)
{
  affiche_arbre_aux(n,n);
}
