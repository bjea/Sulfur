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
      cout << "Deleting node: " << *itor << endl;
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
   if (empty() /*current->next == current->prev*/) {
      newNode->prev = newNode;
      newNode->next = newNode;
      anchor()->next = newNode;
      anchor_.prev = newNode;
   }
   else
   {
      node* preNode = anchor()->next;

      do{
         if(less(preNode->value.first, newNode->value.first))
         {
            preNode = preNode->next;
         }
         else
         {
            // this is where we need to insert the new element in front of the 1st element.
            if(preNode == anchor()->next)
            {
               preNode = anchor();
            }
            // all other cases, we insert the new node after some existing node.
            else
            {
               preNode = preNode ->prev;
            }
         }
      }while(preNode != anchor()->next);

      if(preNode == anchor())
      {
         newNode->prev = anchor()->next->prev;
         newNode->next = anchor()->next;
         anchor()->next->prev = newNode;
         anchor()->next = newNode;
         anchor()->prev->next = newNode;
      }
      else
      {
         newNode->prev = preNode;
         newNode->next = preNode->next;
         newNode->next->prev = newNode;
         newNode->prev->next = newNode;
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
   node* head = anchor_.next;
   node* tail = anchor_.prev;

   if(not empty() /*current->prev != current->next*/)
   {
      if (head != tail && head->next == tail) {
         if (head->value.second == that) {
            current = head;
         }
         if (tail->value.second == that) {
            current = tail;
         }
         else {
            return end();
         }
      }
      else {
         do {
            current = current->next;
            if(current->value.second == that) {
               break;
            }
         } while(current->next != head);
         if (current->value.second != that) {
            return end();
         }
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
   node* nextNode = current->next;
   node* prevNode = current->prev;

   // we are erasing the last node
   if(current == nextNode)
   {
      anchor_.next = anchor();
      anchor_.prev = anchor();
      delete current;
      return end();
   }
   else
   {

      prevNode->next = nextNode;
      nextNode->prev = prevNode;

      // check if we are erasing the head node
      if (current == anchor_.next) {
         anchor_.next = nextNode;
      }
      // check if we are erasing the tail node
      if (current == anchor_.prev) {
         anchor_.prev = prevNode;
      }
      if (current /*position.where*/ != end().where) {
         //delete position.where;
         delete current;
         return iterator(nextNode);
      }
   }

   return end();
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
   listmap<Key,Value,Less> thisMap;
   while (where->next != thisMap.begin().where) {
      where = where->next;
   }
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

