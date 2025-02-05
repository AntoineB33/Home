#pragma once

#include <algorithm>
#include <vector>

// =============================================================================
// ZERO-CONTAINERS - CONSIGNES GENERALES
//
// Implementez les conteneurs suivants en ecrivant un minimum de code a l'aide
// de std::vector comme seul container, et des algorithmes de la bibliotheque
// standard. Vous devez ecrire le moins de code possible en suivant la regle du
// zero; par exemple vous ne *devez pas* implementer vous-meme les semantiques
// de transfert ou de copie.
//
// Soyez flemmard-e-s, et n'implementez que les fonctions qui sont decrites pour
// chaque conteneur.
// =============================================================================

// stack_t

//  stack_t &push(T val)
// Rajoute un element en-haut de la pile.

//  T &top()
// Permet d'acceder et de modifier l'element en-haut de la pile.

//  T const &top() const
// Permet d'acceder al'element en-haut de la pile sans le modifier.

//  stack_t &pop()
// Supprime l'element en-haut de la pile.

//  std::size_t size() const
// Renvoie la taille de la pile.

template <typename T> struct stack_t {
  // écrire votre code ici
  private:
    std::vector<T> data_ ;

  public:
    stack_t &push(T val) {
        data_.push_back(val);
        return *this;
    }

    T &top() {
        return data_.back();
    }

    T const &top() const{
        return data_.back();
    }

    stack_t &pop() {
        data_.pop_back();
        return *this;
    }

    std::size_t size() const {
        return data_.size();
    }
};

