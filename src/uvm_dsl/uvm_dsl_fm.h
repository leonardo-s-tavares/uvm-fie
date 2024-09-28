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

#ifndef SRC_UVM_DSL_UVM_DSL_FM_H_
#define SRC_UVM_DSL_UVM_DSL_FM_H_

#include <utility>

class uvm_eca_engine;

namespace uvm_dsl {

template<class Expr, class Injector> struct fault_model {
  fault_model(const Expr& expr, const Injector& inj) : expr(expr),
                                                       injector(inj) {}
  fault_model(const fault_model& other) : expr(other.expr),
                                          injector(other.injector) {}
  ~fault_model() {}

  Expr expr;
  Injector injector;
};

template<class Expr, class Injector>
fault_model<Expr, Injector>
fm(const Expr& expr, const Injector& injector) {
  return fault_model<Expr, Injector>(expr, injector);
}

template<class Expr, class Injector>
uvm_eca_engine& operator<<(uvm_eca_engine& eca,
                           const fault_model<Expr, Injector>& fm) {
  eca.register_fault_condition(fm.expr.build(), fm.injector.build());
  return eca;
}

template<class Expr, class T>
uvm_eca_engine& operator<<(uvm_eca_engine& eca,
                           const fault_model<Expr, T*>& fm) {
  eca.register_fault_condition(fm.expr.build(), fm.injector);
  return eca;
}

} //namespace uvm_dsl

#endif /* SRC_UVM_DSL_UVM_DSL_FM_H_ */
