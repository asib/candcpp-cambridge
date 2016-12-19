#include <iostream>

class EmptyStack{};

template<class T> class Stack {
  struct Item {
    T val;
    Item *next;
    Item(T v): val(v), next(0) {}
  };

  Item *head;

  Stack(const Stack& s) {
    *this = s;
  }
  Stack& operator=(const Stack& s) {
    Item *current = 0, *previous = 0;
    Item *i = s.head;
    while (i) {
      previous = current;
      current = new Item(i->val);
      if (previous) previous->next = current;
      if (i == s.head) head = current;
      i = i->next;
    }

    return *this;
  }

public:
  Stack(): head(0) {}
  ~Stack() {
    Item *i = head;
    while (i) {
      Item *tmp = i;
      i = i->next;
      delete tmp;
    }
  }
  T pop();
  void push(T val);
};

template<class T> T Stack<T>::pop() {
  if (!head) throw EmptyStack();
  T val = head->val;
  head = head->next;
  return val;
}

template<class T> void Stack<T>::push(T val) {
  Item *newHead = new Item(val);
  newHead->next = head;
  head = newHead;
}

int main(void) {
  Stack<int> s;
  s.push(5);
  s.push(50);
  try {
    std::cout << s.pop() << std::endl << s.pop() << std::endl << s.pop();
  } catch (EmptyStack) {
    std::cout << "Pop'd from empty stack!" << std::endl;
  }

  return 0;
}
