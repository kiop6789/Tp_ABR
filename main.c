#include <stdio.h>
#include <stdlib.h>

/* Structure d'un nœud de l'ABR */
typedef struct NoeudABR {
  int valeur;
  struct NoeudABR *gauche;
  struct NoeudABR *droite;
} NoeudABR;

/* Crée un nouveau nœud (enfants initialisés à NULL) */
NoeudABR *creerNoeud(int valeur) {
  NoeudABR *n = (NoeudABR *)malloc(sizeof(NoeudABR));
  if (n == NULL) {
    fprintf(stderr, "Erreur : allocation memoire echouee\n");
    exit(EXIT_FAILURE);
  }
  n->valeur = valeur;
  n->gauche = NULL;
  n->droite = NULL;
  return n;
}

/* Insère une valeur dans l'ABR (les doublons sont ignorés) */
NoeudABR *inserer(NoeudABR *racine, int valeur) {
  if (racine == NULL)
    return creerNoeud(valeur);

  if (valeur < racine->valeur)
    racine->gauche = inserer(racine->gauche, valeur);
  else if (valeur > racine->valeur)
    racine->droite = inserer(racine->droite, valeur);
  /* valeur == racine->valeur : doublon, on ne fait rien */
  return racine;
}

/* Recherche une valeur : retourne le nœud trouvé, ou NULL si absente */
NoeudABR *rechercher(NoeudABR *racine, int valeur) {
  if (racine == NULL)
    return NULL;
  if (valeur == racine->valeur)
    return racine;
  if (valeur < racine->valeur)
    return rechercher(racine->gauche, valeur);
  return rechercher(racine->droite, valeur);
}

/* Parcours infixe : gauche -> racine -> droite (ordre croissant) */
void parcoursInfixe(NoeudABR *racine) {
  if (racine == NULL)
    return;
  parcoursInfixe(racine->gauche);
  printf("%d ", racine->valeur);
  parcoursInfixe(racine->droite);
}

/* Parcours préfixe : racine -> gauche -> droite */
void parcoursPrefixe(NoeudABR *racine) {
  if (racine == NULL)
    return;
  printf("%d ", racine->valeur);
  parcoursPrefixe(racine->gauche);
  parcoursPrefixe(racine->droite);
}

/* Parcours postfixe : gauche -> droite -> racine */
void parcoursPostfixe(NoeudABR *racine) {
  if (racine == NULL)
    return;
  parcoursPostfixe(racine->gauche);
  parcoursPostfixe(racine->droite);
  printf("%d ", racine->valeur);
}

/* Libère tout l'arbre (parcours postfixe : les enfants avant la racine) */
void libererArbre(NoeudABR *racine) {
  if (racine == NULL)
    return;
  libererArbre(racine->gauche);
  libererArbre(racine->droite);
  free(racine);
}

/* Hauteur de l'arbre en nombre de niveaux (arbre vide = 0) */
int hauteur(NoeudABR *racine) {
  if (racine == NULL)
    return 0;
  int hg = hauteur(racine->gauche);
  int hd = hauteur(racine->droite);
  return 1 + (hg > hd ? hg : hd);
}

/* Affiche les 3 parcours de l'arbre */
void afficherParcours(NoeudABR *racine) {
  printf("Infixe   : ");
  parcoursInfixe(racine);
  printf("\n");
  printf("Préfixe  : ");
  parcoursPrefixe(racine);
  printf("\n");
  printf("Postfixe : ");
  parcoursPostfixe(racine);
  printf("\n");
}

/* Affiche si une valeur est trouvée ou non dans l'arbre */
void testerRecherche(NoeudABR *racine, int valeur) {
  NoeudABR *r = rechercher(racine, valeur);
  if (r != NULL)
    printf("Recherche de %d : trouvée\n", valeur);
  else
    printf("Recherche de %d : absente\n", valeur);
}

/* Exécute un test complet sur une séquence :
 * insertion, parcours, hauteur, 2 recherches positives, 2 négatives, libération
 */
void testerSequence(const char *titre, int seq[], int n, int presents[2],
                    int absents[2]) {
  NoeudABR *racine = NULL;

  printf("\n=== %s ===\n", titre);
  for (int i = 0; i < n; i++)
    racine = inserer(racine, seq[i]);

  afficherParcours(racine);
  printf("Hauteur  : %d niveaux\n", hauteur(racine));

  testerRecherche(racine, presents[0]);
  testerRecherche(racine, presents[1]);
  testerRecherche(racine, absents[0]);
  testerRecherche(racine, absents[1]);

  libererArbre(racine);
}

int main(void) {
  /* Séquence 1 : équilibrée */
  int seq1[] = {50, 30, 70, 20, 40, 60, 80};
  int p1[] = {30, 70}, a1[] = {10, 90};
  testerSequence("Séquence équilibrée", seq1, 7, p1, a1);

  /* Séquence 2 : dégénérée croissante */
  int seq2[] = {10, 20, 30, 40, 50, 60, 70};
  int p2[] = {70, 40}, a2[] = {5, 80};
  testerSequence("Séquence dégénérée croissante", seq2, 7, p2, a2);

  /* Séquence 3 : dégénérée décroissante */
  int seq3[] = {70, 60, 50, 40, 30, 20, 10};
  int p3[] = {10, 40}, a3[] = {5, 80};
  testerSequence("Séquence dégénérée décroissante", seq3, 7, p3, a3);

  /* Séquence 4 : aléatoire (12 entiers entre 1 et 100, graine fixe pour
   * obtenir toujours la même sortie dans le rapport) */
  int seq4[12];
  int n4 = 12;
  srand(2026);
  printf("\nValeurs aléatoires générées : ");
  for (int i = 0; i < n4; i++) {
    seq4[i] = rand() % 100 + 1;
    printf("%d ", seq4[i]);
  }
  printf("\n");
  /* présentes : les 2 premières valeurs tirées ; absentes : 0 et 101, hors de
   * l'intervalle 1-100 donc forcément absentes */
  int p4[] = {seq4[0], seq4[1]}, a4[] = {0, 101};
  testerSequence("Séquence aléatoire", seq4, n4, p4, a4);

  return 0;
}
