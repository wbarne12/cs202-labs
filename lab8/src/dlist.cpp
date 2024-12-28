/* 
 * More detail in lab write up and notes:
 * Write-up:
 *   https://web.eecs.utk.edu/~jplank/plank/classes/cs202/Labs/Lab8/
 * Notes:
 *   https://web.eecs.utk.edu/~jplank/plank/classes/cs202/Notes/Linked/index.html */
#include <iostream>
#include "dlist.hpp"
using namespace std;

// D node
Dnode* Dnode::Next() {
  return flink;
}

Dnode* Dnode::Prev() {
  return blink;
}

// D list methods
Dlist::Dlist() {
  sentinel = new Dnode;

  sentinel->flink = sentinel;
  sentinel->blink = sentinel;

  size = 0;
}

// Copy constructor
Dlist::Dlist(const Dlist &d) {
  sentinel = new Dnode;

  sentinel->flink = sentinel;
  sentinel->blink = sentinel;

  size = 0;

  // IMPORTANT!! Wish i didn't forgoet about adding this >:(
  Clear();

  for (Dnode* it = d.Begin(); it != d.End(); it = it->Next()) {
    Push_Back(it->s);
  }
}

Dlist& Dlist::operator= (const Dlist &d) {
  // Clear any current dlist and then copy from d.
  while (! Empty()) {
    Pop_Front();
  }
  for (Dnode* i = d.Begin(); i != d.End(); i = i->Next()) {
    Push_Back(i->s); 
  }

  return *this;
}

Dlist::~Dlist() {
  while (! Empty()) {
    Pop_Front();
  }
  delete sentinel;
}

void Dlist::Clear() {
  while (! Empty()) {
    Pop_Front();
  }

  size = 0;
} 

bool Dlist::Empty() const {
  return (size == 0);
} 

size_t Dlist::Size() const {
  return size;
}

/* Put new strings on the front or back of the list */

void Dlist::Push_Front(const string &s) {
  Insert_After(s, sentinel);
}

void Dlist::Push_Back(const string &s) {
  Insert_Before(s, sentinel);
}

/* Remove and return the first or last element of the list */
string Dlist::Pop_Front() {
  string front = Begin()->s;
  Erase(Begin());
  return front;
}

string Dlist::Pop_Back() {
  string last = Rbegin()->s;
  Erase(Rbegin());
  return last;
}

// Pointer to the first node on the list 
Dnode* Dlist::Begin() const {
  return sentinel->flink;
}

// Pointer to "one past" the last node on the list.
Dnode* Dlist::End() const {
  return sentinel;
}

// Pointer to the last node on the list
Dnode* Dlist::Rbegin() const {
  return sentinel->blink;
}   

// Pointer to "one before" the first node on the list.
Dnode* Dlist::Rend() const {
  return sentinel;
}     

void Dlist::Insert_Before(const string &s, Dnode *n) {
  // Assign s
  // new_node flink points to n
  // new_node blink is same a n blink
  Dnode* new_node = new Dnode;
  new_node->s = s;
  new_node->flink = n;
  new_node->blink = n->blink;

  // n flink = new_node
  // Blink of n points to new node
  n->blink->flink = new_node;
  n->blink = new_node;
  size++;
}

void Dlist::Insert_After(const std::string &s, Dnode *n) {
  // Same logic as Inser_before, just siligtly different. 
  Dnode* new_node = new Dnode;
  new_node->s = s;
  new_node->blink = n;
  new_node->flink = n->flink;

  n->flink->blink = new_node;
  n->flink = new_node;

  size++;
}

void Dlist::Erase(Dnode *n) {
  
  if (n == sentinel) return; 

  // Reassign the pointers
  n->blink->flink = n->flink;
  n->flink->blink = n->blink;

  delete n;
  size--;
}