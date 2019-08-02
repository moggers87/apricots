// General Doubly Linked List Class Header
// Author: M.D.Snellgrove
// Date: 19/1/2002
// History: Original 18/8/2000
//          falsekill function added 27/2/2001
//          operator() function added 14/1/2002
//          clearlist added 19/2/2002

// List node definition

template <class T> struct node{
  node <T> *next;
  node <T> *prev;
  T value;
};

// Linkedlist class header

template <class T> class linkedlist{
  private:
    node <T> *next_step;
    node <T> *current_step;
    node <T> *head;
    node <T> *create_node(node <T> *, node <T> *, T);
  public:
    linkedlist();
    node <T> *add(T);
    void reset();
    bool next();
    T& data();
    T& operator() ();
    void falsekill(linkedlist <T> &);
    void falsekill(node <T> *);
    void kill();
    void kill(node <T> *);
    void clearlist();
};

// General Doubly Linked List Class Definition

#include <iostream>
#include <cstdlib>
using namespace std;

// Default Constructor

template <class T> linkedlist <T> :: linkedlist(){
  head = 0;
  next_step = 0;
  current_step = 0;
}

// Node Creator

template <class T> node <T> * linkedlist <T> :: create_node(
                   node <T> *nex, node <T> *pre, T val){

  node <T> *newnode = new node <T>;

  if (newnode == 0){
    cerr << "ERROR: Not enough memory to allocate data" << endl;
    exit(EXIT_FAILURE);
  }

  newnode->next = nex;
  newnode->prev = pre;
  newnode->value = val;
  return newnode;

}

// Automatic Node Adder

template <class T> node <T> * linkedlist <T> :: add(T val){

  node <T> *newnode = create_node(head, 0, val);

  if (head != 0) head->prev = newnode;

  head = newnode;

  return newnode;

}

// List-Pointer Resetter

template <class T> void linkedlist <T> :: reset(){

  current_step = 0;
  next_step = head;

}

// List-Pointer Stepper

template <class T> bool linkedlist <T> :: next(){

  current_step = next_step;

  if (current_step != 0){
    next_step = current_step->next;
    return true;
  }

  return false;

}

// List data-reference supplier

template <class T> T& linkedlist <T> :: data(){

  if (current_step == 0){
    cerr << "ERROR: Attempt to reference nonexistant node" << endl;
    exit(EXIT_FAILURE);
  }

  return current_step->value;

}

// List data-reference supplier by operator()

template <class T> T& linkedlist <T> :: operator() (){

  if (current_step == 0){
    cerr << "ERROR: Attempt to reference nonexistant node" << endl;
    exit(EXIT_FAILURE);
  }

  return current_step->value;

}


// False Node Deleter

template <class T> void linkedlist <T> :: falsekill(node <T> *dienode){
 
  if (dienode == 0){
    cerr << "ERROR: Attempt to kill nonexistant node" << endl;
    exit(EXIT_FAILURE);
  }
 
  if (dienode == head) head = dienode->next;
  if (dienode == current_step) current_step = 0;
  if (dienode == next_step) next_step = dienode->next;

}

template <class T> void linkedlist <T> :: falsekill(linkedlist <T> &a){

  falsekill(a.current_step);

}

// Node Deleter

template <class T> void linkedlist <T> :: kill(node <T> *dienode){
 
  if (dienode == 0){
    cerr << "ERROR: Attempt to kill nonexistant node" << endl;
    exit(EXIT_FAILURE);
  }
 
  if (dienode->next != 0)
    dienode->next->prev = dienode->prev;
  
  if (dienode->prev != 0)
    dienode->prev->next = dienode->next;

  if (dienode == head) head = dienode->next;
  if (dienode == current_step) current_step = 0;
  if (dienode == next_step) next_step = dienode->next;

  delete dienode;

}

template <class T> void linkedlist <T> :: kill(){

  kill(current_step);

}

// Clear entire list

template <class T> void linkedlist <T> :: clearlist(){

  reset();
  while (next()) kill();

}
