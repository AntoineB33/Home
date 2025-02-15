
#include <iostream>
#include <utility>
#include <cassert>

/*

Le Système international d'unités (abrégé en SI), inspiré du 
système métrique, est le système d'unités le plus largement 
employé au monde.

Le Système international comporte sept unités de base, 
destinées à mesurer des grandeurs physiques indépendantes
 et possédant chacune un symbole :

  - La masse, mesurée en kilogramme (kg).
  - Le temps, mesuré en second (s).
  - La longueur, mesuré en mètre (m).
  - La température, mesurée en kelvin (K).
  - L'intensité électrique, mesurée en ampère (A).
  - La quantité de matière, mesurée en mole (mol).
  - L'intensité lumineuse, mesurée en candela (cd). 

A partir de ces unités de bases, il est possible de construire 
des unités dérivées. Par exemple : 

  - La frequence, exprimée en s^-1
  - La vitesse, exprimée en m.s^-1
  - L'energie, exprimée en kg.m^2.s^−2 

Ces unités dérivées sont toutes des produits de 
puissance des sept unités de base.

On se propose d'implémenter un systeme permettant de calculer des
grandeurs avec des unités en utilisant le type des objets
pour empecher des erreurs comme "kg + m".

Ressources
- https://fr.wikipedia.org/wiki/Syst%C3%A8me_international_d%27unit%C3%A9s#Unit%C3%A9s_d%C3%A9riv%C3%A9es

*/

/*
  ETAPE 1 - STRUCTURE UNIT

  Ecrivez une structure `unit` qui prend en parametre template un type `T`
  et 7 entiers, chacune représentant la puissance d'une unité de base.
  Vous les ordonnerez de façon à suivre l'ordre de la liste donnée plus haut.

  Cette structure contient une valeur de type `T`.
*/

template<typename T, int Mass, int Time, int Length, int Temperature, int ElectricIntensity, int N, int LightIntensity>
struct unit
{
  T value;
};

/*

  ETAPE 2 - UNITES ET UNITES DERIVEES
  En utilisant le modele ci dessous, définissez des types
  representant les grandeurs demandées.

*/

// Masse : kg
template<typename T>
using mass = unit<T,1,0,0,0,0,0,0>;

// Longeur : m
template<typename T>
using length = unit<T,0,0,1,0,0,0,0>;

// Vitesse m.s-1
template<typename T>
using speed = unit<T,0,-1,1,0,0,0,0>;

// Force exprimée en Newton
template<typename T>
using newton = unit<T,1,-2,1,0,0,0,0>;

// conductance électrique
template<typename T>
using siemens = unit<T,-1,3,-2,0,0,0,0>;

/*

  ETAPE 3 - OPERATIONS

  Ecrivez les operateurs +,-,* et / entre deux unités 
  du même type T mais avec des unités cohérentes.

  On peut multiplier ou diviser du temps par une masse mais
  on ne peut pas ajouter des Kelvin à des metres.

*/

template<typename T, int Mass, int Time, int Length, int Temperature, int ElectricIntensity, int N, int LightIntensity>
unit<T,Mass,Time,Length,Temperature,ElectricIntensity,N,LightIntensity> operator+(unit<T,Mass,Time,Length,Temperature,ElectricIntensity,N,LightIntensity> a, unit<T,Mass,Time,Length,Temperature,ElectricIntensity,N,LightIntensity> b)
{
    return {a.value + b.value};
}

template<typename T, int Mass, int Time, int Length, int Temperature, int ElectricIntensity, int N, int LightIntensity>
unit<T,Mass,Time,Length,Temperature,ElectricIntensity,N,LightIntensity> operator-(unit<T,Mass,Time,Length,Temperature,ElectricIntensity,N,LightIntensity> a, unit<T,Mass,Time,Length,Temperature,ElectricIntensity,N,LightIntensity> b)
{
    return {a.value - b.value};
}

template<typename T, int Mass1, int Time1, int Length1, int Temperature1, int ElectricIntensity1, int N1, int LightIntensity1,
                     int Mass2, int Time2, int Length2, int Temperature2, int ElectricIntensity2, int N2, int LightIntensity2>
unit<T, Mass1 + Mass2, Time1 + Time2, Length1 + Length2, Temperature1 + Temperature2, ElectricIntensity1 + ElectricIntensity2, N1 + N2, LightIntensity1 + LightIntensity2> 
operator*(unit<T, Mass1, Time1, Length1, Temperature1, ElectricIntensity1, N1, LightIntensity1> a, 
          unit<T, Mass2, Time2, Length2, Temperature2, ElectricIntensity2, N2, LightIntensity2> b)
{
    return {a.value * b.value};
}

template<typename T, int Mass1, int Time1, int Length1, int Temperature1, int ElectricIntensity1, int N1, int LightIntensity1,
                     int Mass2, int Time2, int Length2, int Temperature2, int ElectricIntensity2, int N2, int LightIntensity2>
unit<T, Mass1 - Mass2, Time1 - Time2, Length1 - Length2, Temperature1 - Temperature2, ElectricIntensity1 - ElectricIntensity2, N1 - N2, LightIntensity1 - LightIntensity2> 
operator/(unit<T, Mass1, Time1, Length1, Temperature1, ElectricIntensity1, N1, LightIntensity1> a, 
          unit<T, Mass2, Time2, Length2, Temperature2, ElectricIntensity2, N2, LightIntensity2> b)
{
    return {a.value / b.value};
}


/*

  ETAPE 4 - SIMPLIFICATION

  A partir de C++20, n'importe quelle structure constexpr peut servir
  de parametres template.

  Définissez une structure spec contenant un tableau de 7 entiers et les opérations et
  constructeurs que vous jugerez nécessaire et implémentez dans un deuxieme fichier la
  totalité des opérations et types précédent en partant du principe que unit devient

  template<typename T, spec D> struct unit;

*/


struct spec {
    int values[7];
    constexpr spec operator+(const spec& other) const {
        return {values[0] + other.values[0], values[1] + other.values[1], values[2] + other.values[2],
                values[3] + other.values[3], values[4] + other.values[4], values[5] + other.values[5],
                values[6] + other.values[6]};
    }
    constexpr spec operator-(const spec& other) const {
        return {values[0] - other.values[0], values[1] - other.values[1], values[2] - other.values[2],
                values[3] - other.values[3], values[4] - other.values[4], values[5] - other.values[5],
                values[6] - other.values[6]};
    }
};

template<typename T, spec D>
struct unit2 {
    T value;
    constexpr unit2 operator+(const unit2& other) const { return {value + other.value}; }
    constexpr unit2 operator-(const unit2& other) const { return {value - other.value}; }
    template<spec D2>
    constexpr unit2<T, D + D2> operator*(const unit2<T, D2>& other) const { return {value * other.value}; }
    template<spec D2>
    constexpr unit2<T, D - D2> operator/(const unit2<T, D2>& other) const { return {value / other.value}; }
};

namespace si {
    constexpr spec MASS = {1,0,0,0,0,0,0};
    constexpr spec LENGTH = {0,0,1,0,0,0,0};
    constexpr spec TIME = {0,1,0,0,0,0,0};
    constexpr spec SPEED = LENGTH - TIME;

    using length = unit2<double, LENGTH>;
    using time = unit2<double, TIME>;
    using speed = unit2<double, SPEED>;
}

constexpr unit2<double, si::LENGTH> KM = {1000.0};
constexpr unit2<double, si::TIME> H = {3600.0};
constexpr unit2<double, si::TIME> Mi = {60.0};
constexpr unit2<double, si::MASS> KG = {1.0};

template<typename T, spec D>
unit2<T, D> operator*(T value, unit2<T, D> unit) { return {value * unit.value}; }

template<typename T, spec D>
constexpr bool operator==(const unit2<T, D>& a, const unit2<T, D>& b) {
    return a.value == b.value;
}

/*

  ETAPE 5 - CONSTANTES

  Ajouter ce test dans `main` :

    si::length l = 50.*KM ;
    si::time t1 = 10.*Mi, t2 = 20.*Mi ;
    si::speed s = l/(t1+t2) ;
    assert(s==(100.*KM/H));

  Compléter votre code pour que ce test passe.

*/

int main()
{
    // test étape 1
    unit<int, 1, 2, 3, 4, 5, 6, 7> u;
    u.value = 42;

    // test étape 2
    mass<int> m;
    length<int> l0;
    speed<int> s0;
    newton<int> n;
    siemens<int> si;

    // test étape 3
    mass<int> m1;
    mass<int> m2;
    m1.value = 4;
    m2.value = 5;
    auto m3 = m1 + m2;
    auto m4 = m1 - m2;
    auto m5 = m1 * m2;
    auto m6 = m1 / m2;


    si::length l = 50.*KM ;
    si::time t1 = 10.*Mi, t2 = 20.*Mi ;
    si::speed s = l/(t1+t2) ;
    assert(s==(100.*KM/H));
}
