//
// ====================================================================================================
// Introduction
// ====================================================================================================
//
// L’objectif de ce TP est de définir une classe proposant des fonctionnalités proches de `std::array<T,N>`
// (la version C++ d’un tableau C de type `T[N]`). L’implantation sera cependant un peu plus subtile dans
// le cas où le tableau est grand pour éviter qu’il ne fasse déborder la pile et pour éviter que certaines
// opérations comme `move` ou `swap` soient trop coûteuses.
// 
// Les trois classes template définies ci-dessous auront une signature du type :
// 
//      template < typename T , std :: size_t N >
//      class my_new_array {
//      ...
//      };
//
// ====================================================================================================
// Partie 1 : petits tableaux
// ====================================================================================================
// 
// Définissez une classe template `small_array<T,N>` contenant un champ privé ayant le type `T[N]`. Ajoutez
// les versions par défaut de toutes les méthodes spéciales : constructeur par défaut, constructeur par copie,
// constructeur par transfert, affectation par copie, affectation par transfert, destructeur.
// 
// Ajoutez deux opérateurs à la classe permettant d’accéder aux éléments comme si c’était un simple
// tableau :
// 
//      T & small_array <T ,N >:: operator []( std :: size_t i );
//      T const & small_array <T ,N >:: operator []( std :: size_t i ) const ;
// 
// Question : pourquoi faut-il définir deux opérateurs crochets quasiment identiques ? (Voir le code de test
// ci-dessous pour un indice.)
//
//      Parce que l'un est utilisé pour les objets constants (const) et l'autre pour les objets non constants. Cela permet de garantir que les objets constants ne peuvent pas être modifiés.
// 
// Ajoutez à ces opérateurs des assertions pour empêcher le programme de continuer son exécution en cas
// d’accès hors des bornes du tableau.
// 
// Question : est-il possible de marquer ces opérateurs comme étant noexcept ?
// 
//      Non, car les assertions peuvent provoquer une interruption du programme si l'accès est hors des bornes, ce qui n'est pas compatible avec noexcept.
// 
// Testez votre classe en utilisant le `main()` initial.
// 
// Question : est-ce que votre code affiche des valeurs surprenantes pour les cases autres que la deuxième ?
// Si oui, c’est normal (et sinon, c’est un coup de chance). Pourquoi ?
// 
//      Parce que le tableau n'est pas initialisé, donc les valeurs sont indéterminées (elles peuvent être n'importe quelle valeur résiduelle en mémoire).
// 
// Ajoutez deux méthodes qui se comportent comme les opérateurs crochets, mais qui lèvent cette fois des
// exceptions quand les accès ont lieu hors des bornes :
// 
//      T & small_array <T ,N >:: at ( std :: size_t i );
//      T const & small_array <T ,N >:: at ( std :: size_t i ) const ;
// 
// Testez vos nouvelles méthodes en modifiant le code de `main()`.
//
// ====================================================================================================
// Partie 2 : grands tableaux
// ====================================================================================================
// 
// Testez votre classe avec le code suivant :
// 
//      int main() {
//        small_array < int , N > t ;
//        t[2] = 42;
//      }
// 
// Question : pourquoi le programme plante-t-il ?
// 
//      En enlevant -O3 des paramètres de compilation, le programme plante car le tableau est trop grand pour être alloué sur la pile, ce qui provoque un débordement de pile (stack overflow).
// 
// Définissez une classe template `large_array<T,N> `dont le champ privé a maintenant le type suivant :
// `std::unique_ptr < small_array <T ,N > >`.
// 
// Ajoutez des opérateurs crochets et des méthodes at permettant d’accéder aux éléments du tableau.
// 
// Question : pourquoi le constructeur par défaut fourni par le compilateur ne convient-il pas ?
// 
//      Parce que le constructeur par défaut du compilateur ne peut pas initialiser correctement le std::unique_ptr avec un nouvel objet small_array.
// 
// Définissez un constructeur par défaut et testez votre classe avec le code suivant :
//   
//      int main() {
//        large_array < int , N > t ;
//        t [2] = 42;
//      }
// 
// Les versions du constructeur par copie et de l’opérateur d’affectation par copie fournies par le compilateur
// ne conviennent pas non plus. Définissez des versions adaptées à `large_array`.
// 
// Complétez votre code de test en vous inspirant de celui utilisé pour les petits tableaux afin de vérifier
// que votre constructeur par copie fonctionne correctement.
// 
// Fournissez une méthode `swap` qui échange le contenu de deux tableaux larges en temps constant :
// 
//      void large_array <T ,N >::swap( large_array & );
// 
// Proposez une variante de l’opérateur d’affectation par copie qui fournisse une garantie plus forte concernant
// les exceptions : si une exception est levée lors de la copie, le tableau original est rendu inchangé
// plutôt qu’à moitié modifié. (Note : cette garantie n’est pas fournie par small_array.)
// 
// Question : quel est l’inconvénient de cette variante ?
// 
//      L'inconvénient est qu'elle nécessite une allocation temporaire, ce qui peut être coûteux en termes de performance et de mémoire.
// 
// Une fonction template incorrecte n’est généralement pas détectée par le compilateur tant qu’elle n’est
// pas utilisée par du code non-template. Modifiez le code de test afin que l’opérateur d’affectation par
// copie soit lui-aussi utilisé, de même pour la méthode swap.
// 
// ====================================================================================================
// Partie 3 : tableaux malins
// ====================================================================================================
// 
// Définissez un type template qui se résout vers `small_array<T,N>` s’il est suffisamment petit (inférieur à
// 16 octets par exemple) et vers `large_array<T,N>` sinon.
// 
// Testez votre type avec le code suivant en faisant varier la taille passée en paramètre. On pourra ajouter
// une assertion dans le constructeur de `large_array` pour s’assurer qu’il n’est pas appelé avec un petit `N`.
// 
//      int main() {
//        my_array < int , N > t ;
//        t [2] = 42;
//      }
// 

#include <iostream>
#include <cassert>
#include <stdexcept>
#include <memory>

template <typename T, std::size_t N>
class small_array {
private:
    T data[N];

public:
    // Constructeur par défaut
    small_array() = default;

    // Constructeur par copie
    small_array(const small_array&) = default;

    // Constructeur par transfert
    small_array(small_array&&) = default;

    // Affectation par copie
    small_array& operator=(const small_array&) = default;

    // Affectation par transfert
    small_array& operator=(small_array&&) = default;

    // Destructeur
    ~small_array() = default;

    // Opérateurs d'accès
    T& operator[](std::size_t i) {
        assert(i < N && "Index out of bounds");
        return data[i];
    }

    const T& operator[](std::size_t i) const {
        assert(i < N && "Index out of bounds");
        return data[i];
    }

    // Méthodes at avec exceptions
    T& at(std::size_t i) {
        if (i >= N) throw std::out_of_range("Index out of bounds");
        return data[i];
    }

    const T& at(std::size_t i) const {
        if (i >= N) throw std::out_of_range("Index out of bounds");
        return data[i];
    }
};

template < typename T , std :: size_t N >
class large_array {
private:
    std::unique_ptr<small_array<T, N>> data;

public:
    // Constructeur par défaut
    large_array() : data(std::make_unique<small_array<T, N>>()) {}

    // Constructeur par copie
    large_array(const large_array& other) : data(std::make_unique<small_array<T, N>>(*other.data)) {}

    // Affectation par copie avec garantie forte
    large_array& operator=(const large_array& other) {
        if (this != &other) {
            auto temp = std::make_unique<small_array<T, N>>(*other.data);
            data = std::move(temp);
        }
        return *this;
    }

    // Constructeur par transfert
    large_array(large_array&&) = default;

    // Affectation par transfert
    large_array& operator=(large_array&&) = default;

    // Destructeur
    ~large_array() = default;

    // Opérateurs d'accès
    T& operator[](std::size_t i) {
        assert(i < N && "Index out of bounds");
        return (*data)[i];
    }

    const T& operator[](std::size_t i) const {
        assert(i < N && "Index out of bounds");
        return (*data)[i];
    }

    // Méthodes at avec exceptions
    T& at(std::size_t i) {
        if (i >= N) throw std::out_of_range("Index out of bounds");
        return (*data)[i];
    }

    const T& at(std::size_t i) const {
        if (i >= N) throw std::out_of_range("Index out of bounds");
        return (*data)[i];
    }

    // Méthode swap
    void swap(large_array& other) noexcept {
        data.swap(other.data);
    }
};
template <typename T, std::size_t N>
using my_array = std::conditional_t<(sizeof(T) * N <= 16), small_array<T, N>, large_array<T, N>>;

int main() {
    std::size_t const n = 4;
    std::size_t const N = 1000 * 1000 * 10;
    // std::size_t const n = N;

    // small_array < int , N > t ;
    my_array < int , n > t;
    t[2] = 42;
    my_array < int , n > tc(t);
    my_array<int, n> td = std::move(t);
    my_array < int , n > const u = t ;
    for ( std :: size_t i = 0; i < 4; ++ i ) {
        std::cout << "[ " << i << " ] = " << t [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << u [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << tc [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << td [ i ] << "\n ";
    }
    // t[n] = 0; // assertion failed !

    // Test de la méthode at avec un accès valide
    try {
        std::cout << "t.at(2) = " << t.at(2) << '\n'; // Accès valide
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    // Test de la méthode at avec un accès hors des bornes
    // try {
    //     std::cout << "t.at(n) = " << t.at(n) << '\n'; // Accès hors des bornes
    // } catch (const std::out_of_range& e) {
    //     std::cerr << "Exception: " << e.what() << '\n';
    // }

    // Test de la méthode at avec un tableau constant
    try {
        std::cout << "u.at(2) = " << u.at(2) << '\n'; // Accès valide
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    // Test de la méthode at avec un accès hors des bornes sur un tableau constant
    // try {
    //     std::cout << "u.at(n) = " << u.at(n) << '\n'; // Accès hors des bornes
    // } catch (const std::out_of_range& e) {
    //     std::cerr << "Exception: " << e.what() << '\n';
    // }


    my_array < int , N > t2 ;
    t2[2] = 42;
    my_array < int , N > t2c(t2);
    my_array<int, N> t2d = std::move(t2);
    /*my_array < int , N > const u2 = t2 ;
    for ( std :: size_t i = 0; i < 4; ++ i ) {
        std::cout << "[ " << i << " ] = " << t2 [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << u2 [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << t2c [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << t2d [ i ] << "\n ";
    }
    // t2[N] = 0; // assertion failed !

    // Test de la méthode at avec un accès valide
    try {
        std::cout << "t2.at(2) = " << t2.at(2) << '\n'; // Accès valide
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    // Test de la méthode at avec un accès hors des bornes
    // try {
    //     std::cout << "t2.at(N) = " << t2.at(N) << '\n'; // Accès hors des bornes
    // } catch (const std::out_of_range& e) {
    //     std::cerr << "Exception: " << e.what() << '\n';
    // }

    // Test de la méthode at avec un tableau constant
    try {
        std::cout << "u2.at(2) = " << u2.at(2) << '\n'; // Accès valide
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    // Test de la méthode at avec un accès hors des bornes sur un tableau constant
    // try {
    //     std::cout << "u2.at(N) = " << u2.at(N) << '\n'; // Accès hors des bornes
    // } catch (const std::out_of_range& e) {
    //     std::cerr << "Exception: " << e.what() << '\n';
    // }


    // Test de la méthode swap entre t2 et t3
    my_array < int , N > t3 ;
    t3[2] = 42;
    t2.swap(t3);
    for ( std :: size_t i = 0; i < 4; ++ i ) {
        std::cout << "[ " << i << " ] = " << t2 [ i ] << "\n ";
        std::cout << "[ " << i << " ] = " << t3 [ i ] << "\n ";
    }*/


    return 0;
}