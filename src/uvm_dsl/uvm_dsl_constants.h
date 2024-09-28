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

#ifndef SRC_UVM_DSL_UVM_DSL_CONSTANTS_H_
#define SRC_UVM_DSL_UVM_DSL_CONSTANTS_H_

#include "uvm_dsl_node.h"
#include "../uvm_expr/uvm_var_ct_tpl.h"
// #include "../uvm_expr/uvm_int.h"
//#include "../uvm_expr/uvm_bool.h"
//#include "../uvm_expr/uvm_float.h"
//#include "../uvm_expr/uvm_double.h"

namespace uvm_dsl {

template<class T, class BuiltType>
struct basic_constant : public node<basic_constant<T, BuiltType>, T > {
  T value;
  basic_constant(const T& value) : value(value) {}
  static basic_constant create(const T& value) {return basic_constant(value);}
  uvm_expr* build() const { return new BuiltType(value); }
};

template<class T>
struct wrap_constant {typedef basic_constant<T, uvm_var_ct_tpl<T> > type;};
typedef wrap_constant<int>::type int_;
typedef wrap_constant<bool>::type bool_;


// template<class T>
// struct wrap_constant {typedef basic_constant<bool, uvm_var_tpl<T> > type;};

// template<>
// struct wrap_constant<int> {typedef basic_constant<int, uvm_var_tpl> type;};
// typedef wrap_constant<int>::type int_;
// template<>
// struct wrap_constant<bool> {typedef basic_constant<bool, uvm_bool> type;};
// typedef wrap_constant<bool>::type bool_;
//template<>
//struct wrap_constant<float> {typedef basic_constant<float, uvm_float> type;};
//typedef wrap_constant<float>::type float_;
//template<>
//struct wrap_constant<double> {typedef basic_constant<double, uvm_double> type;};
//typedef wrap_constant<double>::type double_;

template<class T> typename wrap_constant<T>::type
cnst(T value) {
  return typename wrap_constant<T>::type(value);
}


} // namespace uvm_dsl



#endif /* SRC_UVM_DSL_UVM_DSL_CONSTANTS_H_ */
