#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>  // pour std::accumulate

/*
 * Q1 : Surcharges de fonctions pour for_each, transform, accumulate, find_if
 * en passant directement des conteneurs.
 */

// 1) range_for_each
template <typename Container, typename UnaryFunction>
void range_for_each(Container& c, UnaryFunction f) {
    std::for_each(std::begin(c), std::end(c), f);
}

// 2) range_transform
template <typename ContainerIn, typename ContainerOut, typename UnaryOperation>
void range_transform(const ContainerIn& in, ContainerOut& out, UnaryOperation op) {
    std::transform(std::begin(in), std::end(in), std::begin(out), op);
}

// 3) range_accumulate
template <typename Container, typename T>
T range_accumulate(const Container& c, T init) {
    return std::accumulate(std::begin(c), std::end(c), init);
}

// (Surcharge facultative pour permettre une opération binaire personnalisée)
template <typename Container, typename T, typename BinaryOperation>
T range_accumulate(const Container& c, T init, BinaryOperation op) {
    return std::accumulate(std::begin(c), std::end(c), init, op);
}

// 4) range_find_if
template <typename Container, typename Predicate>
auto range_find_if(Container& c, Predicate p) {
    return std::find_if(std::begin(c), std::end(c), p);
}

/*
 * Q2 : Implémentation d'un pseudo-conteneur à partir de deux pointeurs.
 *      => ProxyVector<T>
 * Il doit fournir :
 *    - begin()
 *    - end()
 *    - operator[]
 *    - size()
 * 
 * Contraintes pour être compatible avec nos surcharges :
 *    - Avoir un begin() / end() valides
 *    - Savoir donner le type d'itérateur (ici T*)
 *    - Gérer la taille (size_t size()) pour des besoins éventuels
 *    - Respecter le type retourné par operator[]
 *    - S'assurer de la validité de [begin, end)
 *    - Éviter toute copie de données (on ne possède pas la mémoire, juste un "proxy")
 */

template <typename T>
class ProxyVector {
public:
    // Constructeur à partir de deux pointeurs
    ProxyVector(T* begin_ptr, T* end_ptr)
        : m_begin(begin_ptr), m_end(end_ptr) {}

    // Méthodes begin() / end()
    T* begin() { return m_begin; }
    T* end()   { return m_end; }

    // Surcharges const si on veut pouvoir l'utiliser sur des const ProxyVector
    const T* begin() const { return m_begin; }
    const T* end()   const { return m_end; }

    // Accès par indice
    T& operator[](std::size_t i) {
        return m_begin[i];
    }

    const T& operator[](std::size_t i) const {
        return m_begin[i];
    }

    // Taille
    std::size_t size() const {
        return static_cast<std::size_t>(m_end - m_begin);
    }

private:
    T* m_begin;
    T* m_end;
};

/*
 * Petite fonction d'affichage générique d'un conteneur.
 */
template <typename Container>
void print_table(const Container& tab) {
    std::cout << "[ ";
    for (auto&& e : tab) {
        std::cout << e << ", ";
    }
    std::cout << " ] ( size = " << tab.size() << " )\n";
}

// Exemple de structure "toto" pour démonstration éventuelle
struct toto {
    double a, b;
    int c;
};

int main() {
    // ------------------------------------------------------------------
    // Exemple 1 : range_transform avec un std::vector<int>
    // ------------------------------------------------------------------
    std::vector<int> v_in{1, 2, 3, 4, 5};
    std::vector<int> v_out(v_in.size());

    auto square = [](int x) { return x * x; };

    range_transform(v_in, v_out, square);
    print_table(v_in);   // [ 1, 2, 3, 4, 5,  ] ( size = 5 )
    print_table(v_out);  // [ 1, 4, 9, 16, 25,  ] ( size = 5 )

    // ------------------------------------------------------------------
    // Vérification contre l'appel "classique" de std::transform
    // ------------------------------------------------------------------
    std::transform(v_in.begin(), v_in.end(), v_out.begin(), square);
    print_table(v_in);   // [ 1, 2, 3, 4, 5,  ] ( size = 5 )
    print_table(v_out);  // [ 1, 4, 9, 16, 25,  ] ( size = 5 )

    // ------------------------------------------------------------------
    // Exemple 2 : Création d'un ProxyVector sur une partie d'un std::vector<double>
    // ------------------------------------------------------------------
    std::vector<double> v_double{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // On fait pointer ProxyVector sur la portion [v_double.data() + 1, v_double.data() + 7)
    // c'est-à-dire sur {2, 3, 4, 5, 6, 7} du tableau initial
    ProxyVector<double> pv(v_double.data() + 1, v_double.data() + 7);

    // Affichage de pv
    print_table(pv); // [ 2, 3, 4, 5, 6, 7, ] ( size = 6 )

    // ------------------------------------------------------------------
    // Exemple 3 : Accumulate sur le ProxyVector
    // ------------------------------------------------------------------
    double initValue = 0.0;
    double result = range_accumulate(pv, initValue);
    std::cout << "Somme partielle dans pv = " << result << '\n';
    // Somme = 2+3+4+5+6+7 = 27

    // ------------------------------------------------------------------
    // Exemple 4 : range_find_if
    // ------------------------------------------------------------------
    // Cherchons le premier élément > 4 dans pv
    auto it = range_find_if(pv, [](double x) { return x > 4.0; });
    if (it != pv.end()) {
        std::cout << "Premier élément > 4 dans pv = " << *it << std::endl;
    } else {
        std::cout << "Aucun élément > 4 dans pv.\n";
    }

    // ------------------------------------------------------------------
    // Exemple 5 : range_for_each
    // ------------------------------------------------------------------
    std::cout << "Multiplication par 10 de chaque élément de pv...\n";
    range_for_each(pv, [](double& x) { x *= 10; });
    print_table(pv); // [ 20, 30, 40, 50, 60, 70, ] ( size = 6 )

    return 0;
}
