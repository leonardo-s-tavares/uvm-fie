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

#ifndef SRC_UVM_DSL_UVM_DSL_FAULTS_H_
#define SRC_UVM_DSL_UVM_DSL_FAULTS_H_

#include "../uvm_faults/uvm_fault_set_expr.h"
#include "priv/is_base_of.h"
#include "uvm_dsl_var.h"
#include "uvm_dsl_constants.h"

namespace uvm_dsl {

template<class T> class var;

template<class T, class Expr>
struct fault_set {
  fault_set(const var<T>& tgt, const Expr& expr) : tgt(tgt), value(expr) {}
  fault_set(const fault_set& other) : tgt(other.tgt), value(other.value) {}

  uvm_fault_set_expr* build() const {
    return new uvm_fault_set_expr(tgt.build(), value.build());
  }

  var<T> tgt;
  Expr value;
};


} // namespace uvm_dsl


#endif /* SRC_UVM_DSL_UVM_DSL_FAULTS_H_ */
