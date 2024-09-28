/*
 *  Copyright 2019 uvm-fi
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "uvm_eval_visitor.h"

template<class T> bool   gt(void* l, void* r) {return (*(T*)l) > (*(T*)r);}
template<class T> bool  gte(void* l, void* r) {return (*(T*)l) >= (*(T*)r);}
template<class T> bool   lt(void* l, void* r) {return (*(T*)l) < (*(T*)r);}
template<class T> bool  lte(void* l, void* r) {return (*(T*)l) <= (*(T*)r);}
template<class T> bool   eq(void* l, void* r) {return (*(T*)l) == (*(T*)r);}
template<class T> bool  neq(void* l, void* r) {return (*(T*)l) != (*(T*)r);}
template<class T> bool and_(void* l, void* r) {return (*(T*)l) && (*(T*)r);}
template<class T> bool  or_(void* l, void* r) {return (*(T*)l) || (*(T*)r);}


#define SELF_BIN_IMPL(name, op) \
template<class T> void* name(void* left, void* right) { \
  T* ptr = (T*)malloc(sizeof(T)); \
  *ptr = (*(T*)left) op (*(T*)right); \
  return ptr; \
}
SELF_BIN_IMPL(band, &);
SELF_BIN_IMPL(bor, |);
SELF_BIN_IMPL(xor_, ^);
SELF_BIN_IMPL(plus, +);
SELF_BIN_IMPL(minus, -);
SELF_BIN_IMPL(divide, /);
SELF_BIN_IMPL(mult, *);


#define FILL_MAP_BITWISE(fname) \
  map.insert(std::make_pair(INT, &fname<from_value<INT>::type>)); \
  map.insert(std::make_pair(UINT, &fname<from_value<UINT>::type>)); \
  map.insert(std::make_pair(CHAR, &fname<from_value<CHAR>::type>)); \
  map.insert(std::make_pair(UCHAR, &fname<from_value<UCHAR>::type>)); \
  map.insert(std::make_pair(SHORT, &fname<from_value<SHORT>::type>)); \
  map.insert(std::make_pair(USHORT, &fname<from_value<USHORT>::type>)); \
  map.insert(std::make_pair(LONG, &fname<from_value<LONG>::type>)); \
  map.insert(std::make_pair(ULONG, &fname<from_value<ULONG>::type>)); \
  map.insert(std::make_pair(LONGLONG, &fname<from_value<LONGLONG>::type>)); \
  map.insert(std::make_pair(ULONGLONG, &fname<from_value<ULONGLONG>::type>)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_INT_BASE+(0x1<<1)), &fname<sc_dt::sc_int<(0x1<<1)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_INT_BASE+(0x1<<2)), &fname<sc_dt::sc_int<(0x1<<2)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_INT_BASE+(0x1<<3)), &fname<sc_dt::sc_int<(0x1<<3)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_INT_BASE+(0x1<<4)), &fname<sc_dt::sc_int<(0x1<<4)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_INT_BASE+(0x1<<5)), &fname<sc_dt::sc_int<(0x1<<5)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_INT_BASE+(0x1<<6)), &fname<sc_dt::sc_int<(0x1<<6)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_UINT_BASE+(0x1<<1)), &fname<sc_dt::sc_int<(0x1<<1)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_UINT_BASE+(0x1<<2)), &fname<sc_dt::sc_int<(0x1<<2)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_UINT_BASE+(0x1<<3)), &fname<sc_dt::sc_int<(0x1<<3)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_UINT_BASE+(0x1<<4)), &fname<sc_dt::sc_int<(0x1<<4)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_UINT_BASE+(0x1<<5)), &fname<sc_dt::sc_int<(0x1<<5)> >)); \
  map.insert(std::make_pair((uvm_var_types::type)(_SC_UINT_BASE+(0x1<<6)), &fname<sc_dt::sc_int<(0x1<<6)> >));

#define FILL_MAP_INTEGRAL(fname) \
  FILL_MAP_BITWISE(fname) \
  map.insert(std::make_pair(BOOL, &fname<bool>)); \

#define FILL_MAP(fname) \
  FILL_MAP_INTEGRAL(fname) \
  map.insert(std::make_pair(FLOAT, &fname<from_value<FLOAT>::type>)); \
  map.insert(std::make_pair(DOUBLE, &fname<from_value<DOUBLE>::type>));

#define MAP_HELPER(name, fname) \
class map_helper_##name { \
  public: \
    std::map<uvm_var_types::type, bool (*)(void*, void*)> map; \
    map_helper_##name() { \
      using namespace uvm_var_types; \
      FILL_MAP(fname) \
    } \
}; \
map_helper_##name the_map_helper_##name;

MAP_HELPER( gt,   gt);
MAP_HELPER( lt,   lt);
MAP_HELPER(gte,  gte);
MAP_HELPER(lte,  lte);
MAP_HELPER( eq,   eq);
MAP_HELPER(neq,  neq);
MAP_HELPER(and, and_);
MAP_HELPER( or,  or_);

#define MAP_HELPER_BITWISE(name, fname) \
class map_helper_##name { \
  public: \
    std::map<uvm_var_types::type, void* (*)(void*, void*)> map; \
    map_helper_##name() { \
        using namespace uvm_var_types; \
        FILL_MAP_BITWISE(fname) \
    } \
}; \
map_helper_##name the_map_helper_##name;

MAP_HELPER_BITWISE(band, band);
MAP_HELPER_BITWISE(bor, bor);
MAP_HELPER_BITWISE(xor, xor_);

#define MAP_HELPER_ARITH(name) \
class map_helper_##name { \
  public: \
    std::map<uvm_var_types::type, void* (*)(void*, void*)> map; \
    map_helper_##name() { \
        using namespace uvm_var_types; \
        FILL_MAP(name) \
    } \
}; \
map_helper_##name the_map_helper_##name;

MAP_HELPER_ARITH(plus);
MAP_HELPER_ARITH(minus);
MAP_HELPER_ARITH(divide);
MAP_HELPER_ARITH(mult);

uvm_eval_visitor::uvm_eval_visitor(): child_type(uvm_var_types::BOOL), value(NULL){}

template<class Node, class Map>
void uvm_eval_visitor::visit_bool_bin_op(Node* node, const Map& map) {
  node->left->visit(*this);
  uvm_var_types::type child_type_left = this->child_type;
  void* value_left = this->value;

  node->right->visit(*this);
  if(child_type_left != this->child_type){
      UVM_FATAL("uvm_eval_visitor", "Type missmatch at visit_bool_bin_op! Simulation aborted!");
  }
  typename Map::const_iterator it = map.find(child_type_left);
  if(it == map.end()){
      UVM_FATAL("uvm_eval_visitor", "Unknown type! Simulation aborted!");
  }
  bool* result = (bool*) malloc(sizeof(bool));
  // get the function pointer, get the function, call the functions
  *result = (*it->second)(value_left, this->value);
  free(value_left);
  free(this->value);
  this->value = result;
  this->child_type = uvm_var_types::BOOL;
}

#define FI_LIB_X(name) \
  void uvm_eval_visitor::visit_##name(uvm_##name * node) { \
    visit_bool_bin_op(node, the_map_helper_##name.map); \
  }
FI_LIB_UVM_BOOL_BIN_OPS
#undef FI_LIB_X

template<class Node, class Map>
void uvm_eval_visitor::visit_bin_op(Node* node, const Map& map) {
  node->left->visit(*this);
  uvm_var_types::type child_type_left = this->child_type;
  void* value_left = this->value;

  node->right->visit(*this);
  if(child_type_left != this->child_type){
      UVM_FATAL("uvm_eval_visitor", "Type missmatch at visit_bin_op! Simulation aborted!");
  }
  typename Map::const_iterator it = map.find(child_type_left);
  if(it == map.end()){
      UVM_FATAL("uvm_eval_visitor", "Unknown type! Simulation aborted!");
  }
  // get the function pointer, get the function, call the functions
  void* result = (*it->second)(value_left, this->value);
  free(value_left);
  free(this->value);
  this->value = result;
  //this->child_type remains unchanged
}

#define FI_LIB_X(name) \
  void uvm_eval_visitor::visit_##name(uvm_##name * node) { \
    visit_bin_op(node, the_map_helper_##name.map); \
  }
FI_LIB_UVM_SELF_BIN_OPS
#undef FI_LIB_X

void uvm_eval_visitor::visit_value(uvm_value* node) {
  this->child_type = node->type;
  this->value = node->get();
}
