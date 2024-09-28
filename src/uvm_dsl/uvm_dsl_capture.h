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

#ifndef SRC_UVM_DSL_UVM_DSL_CAPTURE_H_
#define SRC_UVM_DSL_UVM_DSL_CAPTURE_H_

#include "uvm_dsl_node.h"
#include "../uvm_expr/uvm_capture.h"
#include "../uvm_expr/uvm_capture_tpl.h"

#define FI_LIB_UVM_CALL_GEN_DSL
#include "../uvm_expr/uvm_call.h"

namespace uvm_dsl {

template<class T>
struct basic_capture : public node<basic_capture<T>, T> {
  T& value;
  basic_capture(T& value) : value(value) {}
  static basic_capture create(T& value) {return basic_capture(value);}
  uvm_expr* build() const { return new uvm_capture_tpl<T>(value); }
};

template<class T>
struct basic_unif_capture : public node<basic_unif_capture<T>, T> {
  T min, max;
  basic_unif_capture(T min, T max) : min(min), max(max) {}
  static basic_unif_capture create(T min, T max) {return basic_unif_capture(min, max);}
  uvm_expr* build() const { return new uvm_unif_tpl<T>(min, max); }
};


template<class T> struct wrap_capture {
  typedef basic_capture<T> type;
};

template<class T> struct wrap_unif_capture {
  typedef basic_unif_capture<T> type;
};

template<class T> typename wrap_capture<T>::type
cap(T& value) {
  return typename wrap_capture<T>::type(value);
}

template<class T> typename wrap_unif_capture<T>::type
unif(T min, T max) {
  return typename wrap_unif_capture<T>::type(min, max);
}

} // namespace uvm_dsl



#endif /* SRC_UVM_DSL_UVM_DSL_CAPTURE_H_ */
