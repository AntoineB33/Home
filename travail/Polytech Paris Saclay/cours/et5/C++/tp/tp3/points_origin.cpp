/*

Les types suivants seront manipulés lors de ce TP :

  template<std::size_t dim> struct point { double coords[dim]; };
  template<std::size_t dim> using points = std::vector<point<dim>>;

## Partie 1 - Création et affichage de points

Définissez une fonction pour créer un point avec des coordonnées
aléatoires comprises entre 0 et 1. Elle aura la signature suivante :

  template<std::size_t dim> point<dim> randomPoint();

Définissez une fonction pour afficher les coordonnées d'un point sur le
*stream* passé en argument. Elle aura la signature suivante :

  template<std::size_t dim>
  std::ostream &operator<<(std::ostream &out, point<dim> const &p);

Testez votre code à l'aide d'une instruction comme la suivante :

  std::cout << randomPoint<3>() << '\n';

Dans `randomPoints`, combinez l'algorithme `std::generate_n`, l'itérateur renvoyé par `std::back_inserter`
et la fonction `randomPoint` pour remplir un vecteur de type `points<4>` avec 5 points aléatoires.(Note : le code obtenu ne doit pas dépasser une instruction.)

Définissez une fonction pour afficher les coordonnées d'un vecteur de points sur un
*stream* passé en argument, en combinant l'algorithme `std::for_each` avec une fonction anonyme affichant un point sur la sortie standard pour afficher le contenu du vecteur précédent.
(Note : le code obtenu ne doit pas dépasser une instruction.)

## Partie 2 - Longueur de chemin

Définissez une fonction calculant la distance euclidienne entre deux
points de même dimension :

  template<std::size_t dim>
  double dist(point<dim> const &p1, point<dim> const &p2);

Définissez une fonction `pathLength` prenant un vecteur de type
`points` en argument et renvoyant la longueur du chemin fermé
passant par tous les points de ce vecteur (dans l'ordre du
vecteur). Utilisez l'algorithme `std::for_each` pour cela.

Proposez une variante de `pathLength` qui utilise l'algorithme
`std::accumulate`.

Testez vos deux fonctions en affichant le périmètre du triangle suivant
(soit environ 3.414) :

  points<2> triangle = { {0.,0.}, {1.,0.}, {0.,1.} };

## Partie 3 - Échantillonage

Définissez une fonction `quantize` qui prend un point en
argument et renvoie un point dont les coordonnées ont été arrondies par
défaut à 0.1 près. Par exemple, pour le point (0.234, 0.456, 0.678),
la fonction doit renvoyer (0.2, 0.4, 0.6).

Définissez un opérateur de comparaison stricte `lt` qui prend
deux points de même dimension en argument et effectue une comparaison
lexicographique stricte sur leurs versions arrondies par `quantize`.
Vérifiez que votre fonction donne les résultats suivants :
- (0.15,0.72) < (0.43,0.09
- (0.27,0.57) < (0.21,0.63)
- not (0.34,0.72) < (0.36,0.73)
- not (0.43,0.09) < (0.15,0.72)

Essayez de définir un type d'ensemble de points ordonnés par
`lt` de la façon suivante :

  template<std::size_t dim>
  using point_set = std::set<point<dim>, lt<dim>>;

Malheureusement, le langage C++ n'autorise pas d'utiliser
directement une fonction comme deuxième paramètre template de
`std::set`. Définissez donc une classe template ayant une
unique méthode `operator()` qui appelle `lt` pour
comparer deux points et utilisez cette classe pour définir
`point_set`.

Créez un vecteur de 10,000 points aléatoires en dimension 4. Combinez
l'algorithme `std::copy` et la fonction
`std::inserter` pour transformer le vecteur en un ensemble
ordonné de type `point_set`. Affichez la taille de l'ensemble
résultant.

*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <vector>

template <std::size_t dim> struct point { double coords[dim]; };

template <std::size_t dim> using points = std::vector<point<dim>>;

template <std::size_t dim> point<dim> randomPoint() {
  // ECRIVEZ VOTRE CODE ICI
}

template <std::size_t dim>
std::ostream &operator<<(std::ostream &out, point<dim> const &p) {
  // ECRIVEZ VOTRE CODE ICI
}

template <std::size_t dim> points<dim> randomPoints(std::size_t n) {
  points<dim> pts;
  // ECRIVEZ VOTRE CODE ICI
  return pts;
}

template <std::size_t dim>
std::ostream &operator<<(std::ostream &out, points<dim> const &pts) {
  // ECRIVEZ VOTRE CODE ICI
  return out;
}

template <std::size_t dim>
double dist(point<dim> const &p1, point<dim> const &p2) {
  // ECRIVEZ VOTRE CODE ICI
}

template <std::size_t dim> double pathLength(points<dim> const &pts) {
  // ECRIVEZ VOTRE CODE ICI
  // avec std::foreach
}

template <std::size_t dim> double pathLength2(points<dim> const &pts) {
  // ECRIVEZ VOTRE CODE ICI
  // avec std::accumulate
}

template <std::size_t dim> point<dim> quantize(point<dim> const &p1) {
  // ECRIVEZ VOTRE CODE ICI
}

template <std::size_t dim> bool lt(point<dim> const &p1, point<dim> const &p2) {
  // ECRIVEZ VOTRE CODE ICI
}

template <std::size_t dim> using point_set = std::set<point<dim>, lt<dim>>;

int main() {

  std::cout << "un point : " << randomPoint<3>() << '\n';
  points<2> pts = randomPoints<2>(4);
  std::cout << "un vecteur de points :\n" << pts;
  points<2> triangle = {{0., 0.}, {1., 0.}, {0., 1.}};
  std::cout << "périmètre du triangle : " << pathLength(triangle) << ' '
            << pathLength2(triangle) << '\n';
  std::pair<point<2>, point<2>> ppts[4] = {{{0.15, 0.72}, {0.43, 0.09}},
                                           {{0.27, 0.57}, {0.21, 0.63}},
                                           {{0.34, 0.72}, {0.36, 0.73}},
                                           {{0.43, 0.09}, {0.15, 0.72}}};
  for (auto const &pp : ppts) {
    char const *s = lt(pp.first, pp.second) ? " <  " : " >= ";
    std::cout << pp.first << s << pp.second << '\n';
  }
  points<4> pts2 = randomPoints<4>(10000);
  point_set<4> pset;
  std::copy(pts2.begin(), pts2.end(), std::inserter(pset, pset.begin()));
  std::cout << "taille de l'ensemble : " << pset.size() << '\n';
}
