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

#ifndef SRC_UVM_DSL_UVM_DSL_FAULT_FACTORIES_H_
#define SRC_UVM_DSL_UVM_DSL_FAULT_FACTORIES_H_

#include "../uvm_faults/uvm_fault_set_expr.h"
#include "priv/is_base_of.h"
#include "uvm_dsl_constants.h"

namespace uvm_dsl {

template<class T, class Expr> struct fault_set;
template<class T> class var;

template<class T, class Arg,
         bool is_expr=priv::is_base_of<abstract_node, Arg>::value>
struct fault_set_factory {
  typedef fault_set<T, Arg> type;
  static type run(const var<T>& var, const Arg& expr) {
    return type(var, expr);
  }
};

template<class T, class Arg>
struct fault_set_factory<T, Arg, false> {
  typedef fault_set<T, typename wrap_constant<T>::type> type;
  static type run(const var<T>& var, const Arg& arg) {
    return type(var, cnst(arg));
  }
};

} // namespace uvm_dsl

#endif /* SRC_UVM_DSL_UVM_DSL_FAULT_FACTORIES_H_ */
