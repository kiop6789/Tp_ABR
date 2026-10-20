
## Introduction

### Objectifs

Ce TP a trois objectifs :

- implémenter en langage C les opérations fondamentales d'un arbre binaire de recherche (insertion, recherche et parcours) ;
- analyser l'impact de l'ordre d'insertion des valeurs sur la structure de l'arbre ;
- développer une approche critique face à la génération de code par une IA et à sa validation.

### Contexte

Un arbre binaire de recherche (ABR) est une structure de données arborescente qui organise les données de façon hiérarchique. Sa propriété clé est la suivante : pour chaque nœud, toutes les valeurs du sous-arbre gauche sont inférieures à la valeur du nœud, et toutes celles du sous-arbre droit lui sont supérieures. Cette propriété rend la recherche, l'insertion et la suppression efficaces **en moyenne**. Le TP montre aussi que cette efficacité dépend de la forme de l'arbre, donc de l'ordre d'insertion.

### Travail réalisé

J'ai implémenté un ABR d'entiers en C dans un seul fichier, `main.c`, avec :

- la structure `NoeudABR` et les fonctions `creerNoeud`, `inserer`, `rechercher`, `libererArbre` ;
- les trois parcours récursifs : `parcoursInfixe`, `parcoursPrefixe` et `parcoursPostfixe` ;
- des fonctions d'aide pour les tests (`afficherParcours`, `testerRecherche`, `hauteur`, `testerSequence`).

Le programme est testé sur quatre séquences d'insertion : une séquence équilibrée, deux séquences dégénérées (croissante et décroissante) et une séquence aléatoire de 12 tirages. Pour chacune, il affiche les trois parcours et la hauteur de l'arbre, effectue deux recherches de valeurs présentes et deux de valeurs absentes, puis libère la mémoire. Ce rapport présente les sorties du programme, l'analyse des formes d'arbres obtenues (sections 1 à 4) et une réflexion sur l'utilisation de l'IA (Étape 4).

### Compilation et conventions

- Compilation : `gcc -Wall -Wextra -o abr main.c` (aucun warning), puis `./abr`.
- Les doublons sont ignorés à l'insertion (l'énoncé ne précise pas ce cas).
- La hauteur est comptée en **niveaux** (arbre vide = 0, un seul nœud = 1) ; hauteur en arêtes = niveaux - 1.

---

## Séquence 1 : équilibrée (50, 30, 70, 20, 40, 60, 80)

### Sortie du programme

![[Pasted image 20261015115806.png]]
### Forme de l'arbre

![[Pasted image 20261015115520.png]]

### Réponse

Sur la sequence 1 on a un abre **complet et parfaitement équilibré** : chaque nœud interne a deux enfants et toutes les feuilles (20, 40, 60, 80) sont au même niveau. Sa hauteur est de **3 niveaux** avec 2 arêtes, soit log2(7+1) = 3. Le parcours infixe donne bien les valeurs en ordre croissant, ce qui confirme la propriété de l'ABR.

---

## Séquence 2 : dégénérée croissante (10, 20, 30, 40, 50, 60, 70)

### Sortie du programme

![[Pasted image 20261015113959.png]]
### Forme de l'arbre
![[Pasted image 20261015115600.png]]


### Réponse

Chaque nouvelle valeur est plus grande que toutes les précédentes : elle est donc toujours insérée comme **enfant droit** du dernier nœud. L'arbre est une chaîne qui penche entièrement à droite, de hauteur **7 niveaux** (autant que de nœuds).

Cela fait penser à une **liste chaînée** : chaque nœud n'a qu'un seul successeur et on doit parcourir les éléments un par un.

La complexité de la recherche passe de O(log n) (arbre équilibré) à **O(n)** dans le pire cas : pour trouver 70, il faut visiter les 7 nœuds, comme dans une liste. Un ABR ne garantit donc pas de bonnes performances sans mécanisme d'équilibrage (AVL, arbres rouge-noir).

---

## Séquence 3 : dégénérée décroissante (70, 60, 50, 40, 30, 20, 10)

### Sortie du programme
![[Pasted image 20261015115850.png]]
### Forme de l'arbre
![[Pasted image 20261015115654.png]]

### Réponse

C'est l'image **miroir** de la séquence précédente : chaque valeur est plus petite que les précédentes, donc toujours insérée comme **enfant gauche**. L'arbre est une chaîne penchée à gauche, avec la même hauteur (7 niveaux) et la même complexité de recherche O(n).

Comparaison des parcours :

- Le parcours **infixe** est identique dans les deux cas (10 à 70), ce qui est normal : il donne toujours l'ordre croissant, quelle que soit la forme de l'arbre.
- Le **préfixe** de l'une est le **postfixe** de l'autre, et inversement (en séquence croissante, préfixe = 10...70 et postfixe = 70...10 ; en séquence décroissante c'est l'inverse).

La forme dépend donc de l'**ordre d'insertion**, pas des valeurs : le même ensemble {10, ..., 70} donne un arbre de hauteur 3 ou de hauteur 7 selon l'ordre.

---

## Séquence 4 : aléatoire (12 tirages entre 1 et 100)

Les valeurs sont générées avec `srand(2026)` puis `rand() % 100 + 1`. La graine est fixée pour que la sortie soit reproductible (les valeurs exactes peuvent différer sur une autre machine ou une autre bibliothèque C).

### Sortie du programme

![[Pasted image 20261015120008.png]]

Les valeurs 0 et 101 sont hors de l'intervalle 1-100 : elles sont donc forcément absentes. Remarque : **73 est tiré deux fois**. Comme les doublons sont ignorés, l'arbre ne contient que **11 nœuds** pour 12 tirages (visible dans le parcours infixe).

### Forme de l'arbre (vérifiée à la main)

![[Pasted image 20261015115713.png]]
### Réponse

L'arbre aléatoire a une forme **intermédiaire**, nettement plus proche de l'équilibré que du dégénéré : 11 nœuds pour une hauteur de 5 niveaux, contre 4 niveaux au minimum possible (2^4 - 1 = 15 ≥ 11) et 11 au maximum. Il est un peu déséquilibré (branche 24-28-31, branche 73-70-54-64) mais ni en chaîne ni parfait.

C'est le comportement typique : pour des insertions aléatoires, la hauteur moyenne d'un ABR est en O(log n), donc la recherche reste en moyenne en O(log n). Les cas dégénérés correspondent à des entrées déjà triées, qui sont fréquentes en pratique, d'où l'intérêt des arbres auto-équilibrés.

| Séquence     | Nœuds | Hauteur (niveaux) | Recherche (pire cas) |
| ------------ | ----- | ----------------- | -------------------- |
| Équilibrée   | 7     | 3                 | O(log n)             |
| Croissante   | 7     | 7                 | O(n)                 |
| Décroissante | 7     | 7                 | O(n)                 |
| Aléatoire    | 11    | 5                 | O(log n) en moyenne  |

---

## Étape 4 : Réflexion sur l'utilisation de l'IA

J'ai utilisé un assistant d'IA générative (Claude, d'Anthropic) pendant ce TP.

### 1.  quelles requêtes ai-je formulées ?

Je ne lui ai pas demandé « fais-moi tout le TP » d'un coup. J'ai d'abord demandé de **travailler étape par étape et de façon interactive**, en lui donnant l'énoncé. Ma démarche a suivi ces étapes :

1. **Structure et `creerNoeud`** : l'assistant m'a proposé le début du code, avec la vérification du retour de `malloc`.
2. **`inserer`** : je l'ai écrite moi-même, puis soumise à l'assistant pour relecture.
3. **`rechercher`** : j'ai eu du mal (j'ai recopié la structure d'`inserer`). Après plusieurs essais guidés par des indices, j'ai demandé la solution, que j'ai ensuite relue et comprise.
4. **Parcours et `libererArbre`** : écrits par moi à partir d'un modèle, corrigés ensuite sur les points ci-dessous.
5. **`main` et tests** : j'ai écrit le test de la séquence équilibrée, puis j'ai demandé à l'assistant de me le recorriger en m'expliquant les erreurs que j'ai commise.

### 2. Avantages et inconvénients

**Avantages**

- Le travail étape par étape m'a permis de comprendre la logique récursive avant de coder (cas de base, appel sur le sous-arbre, ordre des trois lignes d'un parcours).
- Les messages du compilateur, parfois difficiles à lire (erreurs en cascade), ont été expliqués rapidement : une seule faute pouvait en provoquer six.
- Gain de temps pour les tests : factorisation avec `testerSequence` au lieu de copier-coller quatre fois le même bloc (ce qui m'avait déjà causé des doublons de fonctions).

**Inconvénients**

- Quand je demande la solution directement (cas de `rechercher` et de la version finale du `main`), j'apprends moins que si je l'écris moi-même : j'ai dû relire et me faire expliquer le code pour être sûr de le comprendre.
- Le code peut compiler et être faux : l'IA et moi avons rencontré des erreurs qui ne se voient pas à la compilation (voir ci-dessous).
- Il faut vérifier les choix implicites faits dans le code (ex. traitement des doublons) et ne pas faire confiance aveuglément à ce qui est produit, rapport compris.
- Risque de dépendance : sans relecture personnelle, on ne s'aperçoit pas des erreurs.

### 3. Ai-je dû corriger, adapter ou améliorer le code ? Pourquoi et comment ?

Oui, à plusieurs reprises, dans les deux sens (mon code corrigé avec l'aide de l'IA, et choix d'amélioration par rapport à l'énoncé) :

|Problème|Pourquoi c'était un problème|Correction|
|---|---|---|
|`inserer` refaisait le `malloc` à la main|Code dupliqué, et pas de test d'échec de l'allocation|Appel à `creerNoeud(valeur)`|
|`creerNoeud(int valeur)` écrit dans un **appel**|Erreur de syntaxe : le type ne s'écrit que dans la déclaration|`creerNoeud(valeur)`|
|`Noeud *n = (NoeudABR)malloc(...)`|Type inconnu et cast sans `*` : six erreurs en cascade|`NoeudABR *n = (NoeudABR *)malloc(...)`|
|`EXIT_FAILURE;` seul|Instruction sans effet : le programme continuait avec un pointeur `NULL`|`exit(EXIT_FAILURE);`|
|`parcoursInfixe` appelé dans le préfixe et le postfixe|Compile, mais affiche un résultat faux, sans aucune erreur|Chaque fonction s'appelle elle-même|
|Appels récursifs sur `racine` au lieu de `racine->gauche`/`->droite`|Récursion infinie, donc plantage|Descendre dans un sous-arbre à chaque appel|
|`afficherParcours` et `testerRecherche` collées deux fois|Erreur « redéfinition » à la compilation|Suppression des doublons|

Améliorations ajoutées : commentaires sur chaque fonction, `racine = NULL` après la libération, fonction `hauteur` pour mesurer la forme des arbres, et choix documenté d'**ignorer les doublons** (non précisé dans l'énoncé), ce qui explique que la séquence aléatoire contienne 11 nœuds pour 12 tirages.

### 4. Comment ai-je validé la correction du code ?

- **Compilation** avec `gcc -Wall -Wextra` : aucune erreur ni warning.
- **Prédiction à la main** des trois parcours de la séquence équilibrée avant de lancer le programme, puis comparaison avec la sortie réelle : identiques. J'ai fait de même en redessinant à la main l'arbre de la séquence aléatoire.
- **Propriété de l'ABR** : le parcours infixe est croissant dans les quatre séquences.
- **Cohérence des formes** : hauteurs 3, 7, 7 et 5, conformes à ce qu'on attend (équilibré, deux chaînes, intermédiaire). Le préfixe de la séquence croissante est le postfixe de la décroissante, comme prévu par la théorie.
- **Recherches** : deux valeurs présentes et deux absentes par séquence, toutes conformes.
- **Mémoire** : exécution sous `valgrind --leak-check=full` : aucune erreur, aucune fuite, ce qui valide `libererArbre`.

Ces vérifications ont été faites sur le code quel que soit son auteur (moi ou l'IA) : un code généré qui compile n'est pas pour autant correct, et c'est le test qui permet de le savoir.
