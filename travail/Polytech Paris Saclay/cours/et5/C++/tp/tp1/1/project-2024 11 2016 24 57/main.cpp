#include "stack.hpp"

#include <cassert>
#include <iostream>

int main() 
{
  {
    stack_t<int> stack;

    stack.push(1);
    stack.push(2);
    stack.push(3);

    stack.top() = 4;

    assert(stack.top() == 4);
    stack.pop();

    assert(stack.top() == 2);
    stack.pop();

    assert(stack.top() == 1);
    stack.pop();
  }

  {
    stack_t<int> stack;
    stack_t<int> const &stack_cref = stack;

    stack.push(1);
    stack.push(2);
    stack.push(3);

    assert(stack_cref.size() == 3);

    stack.top() = 4;

    stack_t<int> const stack_copy = stack;

    assert(stack_copy.top() == 4);

    assert(stack_cref.top() == 4);
    stack.pop();

    assert(stack_cref.top() == 2);
    stack.pop();

    assert(stack_cref.top() == 1);
    stack.pop();
  }

  std::cout << "Tests passés avec succès.\n"; 
  return 0;
}
