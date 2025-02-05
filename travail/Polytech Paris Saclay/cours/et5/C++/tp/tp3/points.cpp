#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <vector>

// Représentation d'un point de dimension `dim`
template <std::size_t dim> 
struct point { 
  double coords[dim]; 
};

// Alias pour un vecteur de points de dimension `dim`
template <std::size_t dim> 
using points = std::vector<point<dim>>;

//------------------------------------------------------------------------
// PARTIE 1
//------------------------------------------------------------------------

// 1) Création d'un point aléatoire dans [0,1]^dim
template <std::size_t dim> 
point<dim> randomPoint() {
  point<dim> p;
  for (std::size_t i = 0; i < dim; ++i) {
    p.coords[i] = static_cast<double>(std::rand()) / RAND_MAX;
  }
  return p;
}

// 2) Surcharge de l'opérateur << pour afficher un point
template <std::size_t dim>
std::ostream &operator<<(std::ostream &out, point<dim> const &p) {
  out << '(';
  for (std::size_t i = 0; i < dim; ++i) {
    out << p.coords[i];
    if (i + 1 < dim) out << ',';
  }
  out << ')';
  return out;
}

// Génération d'un vecteur de `n` points aléatoires en une seule instruction
template <std::size_t dim> 
points<dim> randomPoints(std::size_t n) {
  points<dim> pts;
  // Attention à bien tout mettre en une seule instruction :
  std::generate_n(std::back_inserter(pts), n, randomPoint<dim>);
  return pts;
}

// Surcharge de l'opérateur << pour afficher un vecteur de points
template <std::size_t dim>
std::ostream &operator<<(std::ostream &out, points<dim> const &pts) {
  // En une seule instruction également :
  std::for_each(pts.begin(), pts.end(), [&](auto const &p){ out << p << '\n'; });
  return out;
}

//------------------------------------------------------------------------
// PARTIE 2
//------------------------------------------------------------------------

// Distance euclidienne entre deux points de même dimension
template <std::size_t dim>
double dist(point<dim> const &p1, point<dim> const &p2) {
  double sum = 0.;
  for (std::size_t i = 0; i < dim; ++i) {
    double d = p1.coords[i] - p2.coords[i];
    sum += d * d;
  }
  return std::sqrt(sum);
}

// Longueur du chemin fermé passant par tous les points dans l'ordre (std::for_each)
template <std::size_t dim> 
double pathLength(points<dim> const &pts) {
  if (pts.size() < 2) return 0.0;
  double length = 0.0;
  std::size_t n = pts.size();
  std::size_t i = 0;

  // On accumule la distance de pts[i] à pts[(i+1) % n]
  std::for_each(pts.begin(), pts.end(), [&](auto const &p) mutable {
    length += dist(p, pts[(i + 1) % n]);
    ++i;
  });
  return length;
}

// Variante utilisant std::accumulate
template <std::size_t dim> 
double pathLength2(points<dim> const &pts) {
  if (pts.size() < 2) return 0.0;
  return std::accumulate(pts.begin(), pts.end(), 0.0,
                         [&, i = 0ULL](double total, auto const &p) mutable {
                           double d = dist(p, pts[(i + 1) % pts.size()]);
                           ++i;
                           return total + d;
                         });
}

//------------------------------------------------------------------------
// PARTIE 3
//------------------------------------------------------------------------

// Fonction quantize : tronque les coordonnées à 0,1 près
template <std::size_t dim> 
point<dim> quantize(point<dim> const &p1) {
  point<dim> p2;
  for (std::size_t i = 0; i < dim; ++i) {
    // Par exemple, floor( x * 10 ) / 10
    p2.coords[i] = std::floor(p1.coords[i] * 10.0) / 10.0;
  }
  return p2;
}

// Comparaison lexicographique stricte sur les versions quantifiées
template <std::size_t dim> 
bool lt(point<dim> const &p1, point<dim> const &p2) {
  point<dim> qp1 = quantize<dim>(p1);
  point<dim> qp2 = quantize<dim>(p2);

  for (std::size_t i = 0; i < dim; ++i) {
    if (qp1.coords[i] < qp2.coords[i]) return true;
    if (qp1.coords[i] > qp2.coords[i]) return false;
  }
  return false; // si elles sont égales, on renvoie false
}

// Comme on ne peut pas mettre directement une fonction comme comparateur
// de std::set, on définit une classe-foncteur :
template <std::size_t dim>
struct ltComparator {
  bool operator()(point<dim> const &p1, point<dim> const &p2) const {
    return lt<dim>(p1, p2);
  }
};

// On redéfinit point_set en utilisant cette classe-foncteur
template <std::size_t dim> 
using point_set = std::set<point<dim>, ltComparator<dim>>;

//------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------
int main() {
  // Pour avoir des valeurs pseudo-aléatoires différentes à chaque exécution :
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  // Test de la création/affichage d'un point aléatoire
  std::cout << "un point : " << randomPoint<3>() << '\n';

  // Test de la création/affichage d'un vecteur de points
  points<2> pts = randomPoints<2>(4);
  std::cout << "un vecteur de points :\n" << pts;

  // Test de pathLength et pathLength2
  points<2> triangle = { {0., 0.}, {1., 0.}, {0., 1.} };
  std::cout << "périmètre du triangle : " 
            << pathLength(triangle) << ' ' 
            << pathLength2(triangle) << '\n';

  // Tests de la comparaison lexicographique (avec quantize)
  std::pair<point<2>, point<2>> ppts[4] = {
    {{0.15, 0.72}, {0.43, 0.09}},
    {{0.27, 0.57}, {0.21, 0.63}},
    {{0.34, 0.72}, {0.36, 0.73}},
    {{0.43, 0.09}, {0.15, 0.72}}
  };
  for (auto const &pp : ppts) {
    char const *s = lt<2>(pp.first, pp.second) ? " <  " : " >= ";
    std::cout << pp.first << s << pp.second << '\n';
  }

  // Création de 10,000 points en dimension 4 et insertion dans un set
  points<4> pts2 = randomPoints<4>(10000);
  point_set<4> pset;
  std::copy(pts2.begin(), pts2.end(), std::inserter(pset, pset.begin()));

  // Affichage de la taille de l'ensemble résultant
  std::cout << "taille de l'ensemble : " << pset.size() << '\n';

  return 0;
}
