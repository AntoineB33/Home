
#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>

/*

  À la fin de la séance, envoyez le *Short link* de votre travail à l'adresse
  électronique qui vous a été donnée. Mettez en titre de votre email le nom de la séance,
  sous la forme :
  
       ET5 - C++ Avancé - Travaux Pratiques n°... - NOM Prénom

  Comme contenu du message électronique :

       - [NOM Prénom ](https://godbolt.org/z/...)

*/

/*

  Le but de ce TP est de mettre en place un petit système d'EXPRESSION TEMPLATES.

  Les EXPRESSION TEMPLATES sont une technique d'optimisation de calcul numérique qui
  utilisent la méta-programmation pour construire une représentation légère d'une formule
  arbitraire sous la forme d'un ARBRE DE SYNTAXE ABSTRAITE.

  Une fois construit à la compilation, cet arbre devient exploitable à la compilation 
  ou à l'exécution pour effectuer des calculs de divers types. 

  Répondez aux questions dans l'ordre en complétant le code.

*/

namespace et
{
    
  //---------------------------------------------------------------------------------------
  // Q1 - Définissez un concept expr qui est valide si un type T fournit un membre T::is_expr()
  template<typename T>
  concept expr = requires( /* ???? */ )
  {
    // le concept demande que T aie une méthode is_expr qui renvoie un bool
    { T::is_expr() } -> std::convertible_to<bool>;
  };

  //---------------------------------------------------------------------------------------
  /*
    Le premier élément fondamental d'un systeme d'EXPRESSION TEMPLATE est la classe
    `terminal`. Un TERMINAL représente une feuille de l'ARBRE DE SYNTAXE. Dans notre cas,
    nos terminaux sont numérotés statiquement pour représenter différentes variables.

    Q2. Complétez l'implémentation de la structure template `terminal` ci-dessous en suivant les demandes
  */
  //---------------------------------------------------------------------------------------
  template<int ID>
  struct terminal 
  {
    // Faite en sorte que terminal vérifie le concept expr
    static constexpr bool is_expr() { return true; }

    std::ostream & print(std::ostream & os) const
    {
      // Pour terminal<I>, affiche "arg<I>" et renvoit os.
      return os << "arg<" << ID << ">";
    }

    template<typename... Args>
    constexpr auto operator()(Args &&... args) const
    {
      // Construit un tuple de tout les args et renvoit le ID-eme via std::get
      // Veillez à bien respecter le fait que args est une reference universelle
      return std::get<ID>(std::forward_as_tuple(args...));
    }
  };

  // Generateur de variable numérotée
  template<int ID>
  inline constexpr auto arg =  terminal<ID>{};

  // Les variables _0, _1 et _2 sont prédéfinies
  inline constexpr auto _0  = arg<0>;
  inline constexpr auto _1  = arg<1>;
  inline constexpr auto _2  = arg<2>;

  //---------------------------------------------------------------------------------------
  /*
    Le deuxieme élément  d'un systeme d'EXPRESSION TEMPLATE est la classe de noeud. 
    Un NODE représente un opérateur ou une fonction dans l'ARBRE DE SYNTAXE. 

    Il est défini par le type de l'OPERATION effectuée au passage du noeud et d'une
    liste variadique de ses sous-nodes.

    Q3 Complétez l'implémentation de la structure template node ci dessous en suivant les demandes
  */
  //---------------------------------------------------------------------------------------
  template<typename Op, typename... Children>
  struct node
  {
    // Faite en sorte que node vérifie le concept expr
    static constexpr bool is_expr() { return true; }

    // Construisez un node à partir d'une instande de Op et d'une liste variadique de Children
    // Ce constructeur sera constexpr4
    constexpr node(Op op, Children... children) : op(op), children(children...) {}

    // L'operateur() de node permet d'avaluer le sous-arbre courant de manière 
    // récursive. Les paramètres args... représentent dans l'ordre les valeurs des
    // variables contenus dans le sous arbre.
    // Par exemple, le node {op_add, terminal<1>, termnal<0>} appelant operator()(4, 9)
    // doit renvoyer op_add(9, 4);
    // Renseignez vous sur std::apply pour vous simplifier la vie
    // Pensez qu'un node contient potentiellement d'autre node.
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
        return std::apply(
            [this, &args...](auto&&... children) { return op(children(std::forward<Args>(args)...)...); },
            children
        );
    }

    // Affiche un node en demandant à Op d'afficher les sous arbres
    std::ostream& print(std::ostream& os) const
    {
        std::apply(
            [&](auto&&... children) { op.print(os, children.print(os)...); },
            children
        );
        return os;
    }
    
    // Op est stockée par valeur
    // les Children... sont stockées dans un tuple
    Op  op;
    std::tuple<Children...>  children;
  };

  //----------------------------------------------
  /*
    add_ est un exemple de type d'operation passable à un node
    Il fournit un operator() qui effectue le calcul et une fonction
    print qui affiche le node.
  */
  //----------------------------------------------
  struct add_ 
  {
    constexpr auto operator()(auto a, auto b) const
    {
      return a + b;
    }

    std::ostream& print(std::ostream& os, auto a, auto b) const
    {
      return os << a << " + " << b ;
    }
  };
  
  // On lui associe un operator+ qui consomme des expr et renvoit le node
  template<expr L, expr R>
  constexpr auto operator+(L l, R r)
  {
    return node{add_{}, l, r};
  }

  /*
    Q4. Sur le modèle de add_, implémentez 
      - mul_ et un operator* pour la multiplication
      - abs_ et une fonction abs pour le calcul de la valeur absolue
      - fma_ et une fonction fma(a,b,c) qui calcul a*b+c
  */

    struct mul_ {
        constexpr auto operator()(auto a, auto b) const { return a * b; }
        std::ostream& print(std::ostream& os, auto a, auto b) const { return os << "(" << a << " * " << b << ")"; }
    };

    struct abs_ {
        constexpr auto operator()(auto a) const { return std::abs(a); }
        std::ostream& print(std::ostream& os, auto a) const { return os << "|" << a << "|"; }
    };

    struct fma_ {
        constexpr auto operator()(auto a, auto b, auto c) const { return a * b + c; }
        std::ostream& print(std::ostream& os, auto a, auto b, auto c) const {
            return os << "(" << a << " * " << b << " + " << c << ")";
        }
    };

    template<expr L, expr R>
    constexpr auto operator*(L l, R r) { return node{mul_{}, l, r}; }

    template<expr A>
    constexpr auto abs(A a) { return node{abs_{}, a}; }

    template<expr A, expr B, expr C>
    constexpr auto fma(A a, B b, C c) { return node{fma_{}, a, b, c}; }

  
}

int main()
{
  // Q5. Le mini exemple ci dessous doit fonctionner. Complétez le avec une série de tests
  // exhaustif de tous les cas qui vous paraissent nécessaire.

  constexpr auto f = et::fma(et::_1, abs(et::_2),et::_0);
  f.print(std::cout) << "\n";
  std::cout << f(1,2,-3) << "\n";

  // Q6. Appliquez la fonction `f` à des vecteurs ou des matrices. Les operateurs + et *
  // doivent être effectués élément par élément (produit de hadamard).

  /* ???? */ 
}
