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

  Q3 - Quelles contraintes devez vous résoudre pour pouvoir utiliser ce template
  dans votre version des algorithmes ?
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// ECRIVEZ VOTRE CODE ICI
template <typename Container, typename UnaryFunction>
void range_for_each(Container& c, UnaryFunction f) {
    std::for_each(c.begin(), c.end(), f);
}

template <typename ContainerIn, typename ContainerOut, typename UnaryFunction>
void range_transform(const ContainerIn& c1, ContainerOut& c2, UnaryFunction f) {
    std::transform(c1.begin(), c1.end(), c2.begin(), f);
}

template <typename Container, typename T>
T range_accumulate(const Container& c, T init) {
    return std::accumulate(c.begin(), c.end(), init);
}

template <typename Container, typename Predicate>
auto range_find_if(const Container& c, Predicate p) {
    return std::find_if(c.begin(), c.end(), p);
}

template <typename T> void print_table(T tab) {
  std::cout << "[ ";
  for (auto e : tab) {
    std::cout << e << ", ";
  }
  std::cout << " ]( size = " << tab.size() << " )\n";
}

template <typename T>
class ProxyVector {
public:
    ProxyVector(T* begin_ptr, T* end_ptr): m_begin(begin_ptr), m_end(end_ptr) {}

    T* begin() {return m_begin;}
    T* end() {return m_end;}
    
    const T* begin() const { return m_begin; }
    const T* end()   const { return m_end; }

    T& operator[](std::size_t i) {
        return m_begin[i];
    }

    std::size_t size() const {
        return static_cast<std::size_t>(m_end - m_begin);
    }

private:
    T* m_begin;
    T* m_end;
};

int main(int, char **) {

  // Q1

  std::vector<int> v_in{1, 2, 3, 4, 5};
  std::vector<int> v_out(5);
  auto square = [](auto x) { return x * x; };

  std::cout<<"range_for_each :"<<std::endl;
  range_for_each(v_in, [](int &n) { n++; });
  print_table(v_in);

  std::for_each(v_in.begin(), v_in.end(), [](int &n) { n++; });
  print_table(v_in);

  std::cout<<"range_transform :"<<std::endl;
  range_transform(v_in, v_out, square);
  print_table(v_in);
  print_table(v_out);

  transform(v_in.begin(), v_in.end(), v_out.begin(), square);
  print_table(v_in);
  print_table(v_out);

  std::cout<<"range_accumulate :"<<std::endl;
  int i = 0;
  std::cout<<"i = "<<range_accumulate(v_in, i)<<std::endl;
  std::cout<<"i = "<<std::accumulate(v_in.begin(), v_in.end(), i)<<std::endl;

  std::cout<<"range_find_if :"<<std::endl;
  auto it = range_find_if(v_in, [](auto x) {return x == 3;});
    if (it != v_in.end())
        std::cout << "v_in contains 3\n"<<std::endl;
    else
        std::cout << "v_in does not contain 3\n"<<std::endl;
  it = std::find_if(v_in.begin(), v_in.end(), [](auto x) {return x == 3;});
    if (it != v_in.end())
        std::cout << "v_in contains 3\n"<<std::endl;
    else
        std::cout << "v_in does not contain 3\n"<<std::endl;
  
  //std::cout<<std::find_if(v_in.begin(), v_in.end(), [](auto x) {return x == 2;});


  // Q2

  std::vector<double> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  ProxyVector<double> pv(v.data() + 1, v.data() + 7);
  print_table(pv);

  // Q3

  double acc;
  std::cout << range_accumulate(pv, acc) << '\n';

}
