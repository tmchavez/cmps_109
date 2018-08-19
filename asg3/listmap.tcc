#include "listmap.h"
#include "debug.h"

//
///////////////////////////////////////////////////////////////////
//// Operations on listmap::node.
///////////////////////////////////////////////////////////////////
//

//
//// listmap::node::node (link*, link*, const value_type&)
//

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
///////////////////////////////////////////////////////////////////
//// Operations on listmap.
///////////////////////////////////////////////////////////////////
////
//
////
//// listmap::~listmap()

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   while(anchor_.next != anchor()){
     //erase the first node till only the null ptr anchor remains
     erase(begin());
   }
}

//
//// iterator listmap::insert (const value_type&)
////

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   
   iterator c;
   for(c = begin(); c != end() and less(c->first,pair.first); ++c){
     if(c != end() and not less(c->first,pair.first)){
       c->second = pair.second;
       return c;  //
     }
   }
   //key not found, make new nodei
   //iterator c either points to end or where alphabetically appropiate
   node* new1 = new node(c.where, c.where->prev, pair);
   c.where->prev->next = new1;
   c.where->prev = new1;
   DEBUGF ('l', &pair << "->" << pair);
   return iterator(new1);
}

//
//// listmap::find(const key_type&)
////

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   DEBUGF ('l', that);
   iterator d;
   for(d = begin(); d != end(); ++d){
     if(d->first == that){
       //return on key match
       return d;
     }
   }
   //return end
   return end();
}

//
//// iterator listmap::erase (iterator position)
////
//

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   DEBUGF ('l', &*position);
   //make no pointers point to position
   position.where->prev->next = position.where->next;
   position.where->next->prev = position.where->prev; 
   //iterator ret = position.where->next;
   delete position.where; 
   return end();
}




//
///////////////////////////////////////////////////////////////////
//// Operations on listmap::iterator.
///////////////////////////////////////////////////////////////////
////
//
////
//// listmap::value_type& listmap::iterator::operator*()
////

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   DEBUGF ('l', where);
   return where->value;
}

//
//// listmap::value_type* listmap::iterator::operator->()
////

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   DEBUGF ('l', where);
   return &(where->value);
}

//
//// listmap::iterator& listmap::iterator::operator++()
//

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   DEBUGF ('l', where);
   where = where->next;
   return *this;
}

//
//// listmap::iterator& listmap::iterator::operator--()

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   DEBUGF ('l', where);
   where = where->prev;
   return *this;
}

//
//// bool listmap::iterator::operator== (const iterator&)
//

template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
//// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}
