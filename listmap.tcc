// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $

#include "listmap.h"
#include "trace.h"

// Constructor is done, destructor neeeds to be fixed, insert: put in some pointers, find: write a search, erase: ; *, ++, --, =, != are done.

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);

   while (not empty()) {
      listmap<Key,Value,Less>::iterator itor = begin();
      //cout << "Deleting node: " << *itor << endl;
      erase (itor);
   }
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);

   node* newNode = new node (nullptr, nullptr, pair);
   if (empty()) {
      newNode->prev = anchor();
      newNode->next = anchor();
      anchor()->next = newNode;
      anchor_.prev = newNode;
   }
   else
   {
      node* preNode = anchor()->next;
      do {
         if (less(newNode->value.first, preNode->value.first)) {
            break;
         }
         if (less(preNode->value.first, newNode->value.first))
         {
            preNode = preNode->next;
         }
      } while (preNode != anchor() );

      preNode = preNode->prev;
      newNode->prev = preNode;
      newNode->next = preNode->next;
      preNode->next = newNode;
      newNode->next->prev = newNode;
      if(preNode == anchor())
      {
         anchor_.next = newNode;
      }
      if (newNode->next == anchor())
      {
         anchor_.prev = newNode;
      }
   }
   return iterator(newNode);
}

//
// listmap::findKey (const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) /*const*/ {
   TRACE ('l', that);
   node* current = anchor();
   //node* head = anchor_.next;

   if(not empty())
   {
      do {
         current = current->next;
         if (current == anchor()) {
            return end();
         }
         if(current->value.first == that) {
            break;
         }
      } while(current != anchor());
   }

   return iterator(current);
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);

   // sanity check
   if(position.where == end().where)
   {
      return end();
   }

   node* current = position.where;
   node* nextNode = current->next;
   node* prevNode = current->prev;

   prevNode->next = nextNode;
   nextNode->prev = prevNode;
   if (prevNode == anchor())
   {
      anchor_.next = nextNode;
   }
   if (nextNode == anchor())
   {
      anchor_.prev = prevNode;
   }
   delete current;
   return iterator(nextNode);
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// void listmap<Key,Value,Less>::iterator::erase()
//
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::erase() {
   listmap<Key,Value,Less> thisMap;
   if (where == thisMap.end().where)
   {
      return;
   }

   listmap<Key,Value,Less>::node* current = where;
   node* nextNode = where->next;
   node* prevNode = where->prev;

   where->prev->next = where->next;
   where->next->prev = where->prev;
   if (where->prev == thisMap.end().where)
   {
      thisMap.end().where->next = where->next;
   }
   if (where->next == thisMap.end().where)
   {
      thisMap.end().where->prev = where->prev;
   }
   delete where;
};


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

