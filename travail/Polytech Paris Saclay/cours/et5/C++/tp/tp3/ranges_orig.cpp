/*
  Q1 - Pour les algorithmes suivant de la bibliothèque standard
  - for_each
  - transform
  - accumulate
  - find_if

  Ecrivez un jeu de template de fonction permettant de passer directement des
  conteneurs à ces algorithmes.

  Par ex:

  std::vector<int> a = ... ,b = ...;

  // equivalent a std::transform(a.begin(),a.end(),b.begin(), ... );
  range_transform(a,b, [](int x) { return -x; });

  Ajoutez des tests dans la fonction `main()` pour vérifier ces quatres
  algorithmes.

  Q2 - Implémenter un template de structure permettant de transformer
  deux pointeurs décrivant un bloc de mémoire pré-existant en un objet
  ayant une interface similaire à celle de vector en fournissant :
  - begin
  - end
  - operator[]
  - size

  Quelles contraintes devez vous résoudre pour pouvoir utiliser ce template
  dans votre version des algorithmes ?
*/

#include <iostream>
#include <vector>
#include <algorithm>

// ECRIVEZ VOTRE CODE ICI

template <typename T> void print_table(T tab) {
  std::cout << "[ ";
  for (auto e : tab) {
    std::cout << e << ", ";
  }
  std::cout << " ]( size = " << tab.size() << " )\n";
}

struct toto {
  double a, b;
  int c;
};

int main(int, char **) {

  std::vector<int> v_in{1, 2, 3, 4, 5};
  std::vector<int> v_out(5);
  auto square = [](auto x) { return x * x; };

  range_transform(v_in, v_out, square);
  print_table(v_in);
  print_table(v_out);

  transform(v_in.begin(), v_in.end(), v_out.begin(), square);
  print_table(v_in);
  print_table(v_out);

  std::vector<double> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  ProxyVector<double> pv(v.data() + 1, v.data() + 7);
  print_table(pv);

  double acc;
  std::cout << range_accumulate(pv, acc) << '\n';
}
