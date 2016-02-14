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
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   node* current = anchor();
   node* newNode = new node (nullptr, nullptr, pair);
   if (empty() /*current->next == current->prev*/) {
      newNode->prev = newNode;
      newNode->next = newNode;
      anchor()->next = newNode;
      anchor_.prev = newNode;
   }
   else {
      node* head = anchor_.next;
      node* tail = anchor_.prev;
      if (head == tail) {
         newNode->prev = head;
         newNode->next = head->next;
         if (newNode->next == head && less(newNode->value.first, head->value.first)) {
            anchor_.next = newNode;
         }
         if (newNode->prev == tail && less(tail->value.first, newNode->value.first)) {
            anchor_.prev = newNode;
         }
      }
      else if (head != tail && head->next == tail) {
         if (less(pair.first, head->value.first) or less(tail->value.first, pair.first)) {
            newNode->prev = tail;
            newNode->next = head;
            if (newNode->next == head && less(newNode->value.first, head->value.first)) {
               anchor_.next = newNode;
            }
            if (newNode->prev == tail && less(tail->value.first, newNode->value.first)) {
               anchor_.prev = newNode;
            }
         }
         else {
            newNode->prev = head;
            newNode->next = tail;
         }
      }
      else {
         current = head;
         while (current->next != head and less(current->value.first, pair.first)) {
            current = current->next;
         }
         newNode->prev = current;
         newNode->next = current->next;

         if (newNode->next == head && less(newNode->value.first, head->value.first)) {
            anchor_.next = newNode;
         }
         if (newNode->prev == tail && less(tail->value.first, newNode->value.first)) {
            anchor_.prev = newNode;
         }
      }

   }
   return iterator(newNode);
}

//
// listmap::findKey (const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::findKey (const key_type& that) /*const*/ {
   TRACE ('l', that);
   node* current = anchor();
   node* head = anchor_.next;
   node* tail = anchor_.prev;

   if(not empty() /*current->prev != current->next*/)
   {
      if (head != tail && head->next == tail) {
         if (head->value.first == that) {
            current = head;
         }
         if (tail->value.first == that) {
            current = tail;
         }
         else {
            return end();
         }
      }
      else {
         do {
            current = current->next;
            if(current->value.first == that) {
               break;
            }
         } while(current->next != head);
         if (current->value.first != that) {
            return end();
         }
      }
   }

   return iterator(current);
}

//
// iterator listmap::findValue (const mapped_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::findValue(const mapped_type& that) {
   TRACE ('l', that);
   node* current = anchor();
   if (not empty() /*current->prev != current->next*/) {
      do {
         current = current->next;
         if (current->value.second == that) {
            break;
         }
      } while (current->next != anchor()->next);
      if (current->value.second != that) {
         return end();
      }
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
   node* current = position.where;

   current->prev->next = current->next;
   current->next->prev = current->prev;
   delete position.where;
   return iterator(current->next);
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

