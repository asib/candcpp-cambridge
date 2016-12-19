#include <stdio.h>

class LinkList {
private:
  class Item {
    public:
      int mValue;
      Item *mNext;

      Item(int v, Item *n) {
        mValue = v;
        mNext = n;
      }
  };

  Item *mHead;

public:
  LinkList();
  LinkList(int values[], int length);
  LinkList(const LinkList& l);
  ~LinkList();
  LinkList& operator=(const LinkList& l);
  int pop();
};

LinkList::LinkList() {
  mHead = NULL;
}

LinkList::LinkList(int values[], int length) {
  Item *current = NULL, *previous = NULL;
  for (int i = 0; i < length; i++) {
    previous = current;
    current = new Item(values[i], NULL);
    if (previous != NULL) previous->mNext = current;
    if (i == 0) mHead = current;
  }
}

LinkList::LinkList(const LinkList& l) {
  *this = l;
}

LinkList::~LinkList() {
  Item *i = mHead;
  while (i != NULL) {
    Item *tmp = i;
    i = i->mNext;
    delete tmp;
  }
}

LinkList& LinkList::operator=(const LinkList& l) {
  Item *current = NULL, *previous = NULL;
  Item *i = l.mHead;
  while (i != NULL) {
    previous = current;
    current = new Item(i->mValue, NULL);
    if (previous != NULL) previous->mNext = current;
    if (i == l.mHead) mHead = current;
    i = i->mNext;
  }

  return *this;
}

int LinkList::pop() {
  if (mHead == NULL) return -1;

  int t = mHead->mValue;
  mHead = mHead->mNext;
  return t;
}

int main() {
  int test[] = {1,2,3,4,5};
  LinkList l1(test+1,4), l2(test,5);
  LinkList l3=l2, l4;
  l4=l1;
  printf("%d %d %d\n",l1.pop(),l3.pop(),l4.pop());
  return 0;
}
