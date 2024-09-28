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

#ifndef SRC_UVM_DSL_UVM_DSL_VAR_H_
#define SRC_UVM_DSL_UVM_DSL_VAR_H_

#include "../uvm_expr/uvm_var_types.h"
#include "../uvm_expr/uvm_var_tpl.h"
#include "../uvm_expr/uvm_default_event.h"
#include "uvm_dsl_node.h"
#include "uvm_dsl_faults.h"
#include "uvm_dsl_faults_factories.h"
#include "uvm_dsl_constants.h"

namespace uvm_dsl {

template<class T> class var : public node<var<T>, T> {
private:
  const char* name;
  const char* scope;
public:
  explicit var(const char* name, const char* scope="*") : name(name),
                                                          scope(scope) {}
  uvm_var* build() const {
    return new uvm_var_tpl<T>(name, scope);
  }

  template<class Arg> typename fault_set_factory<T, Arg>::type
  set(const Arg& arg) const {
    return fault_set_factory<T, Arg>::run(*this, arg);
  }
};

template<class T> struct evt : public node<evt<T>, bool> {
private:
  const char* name;
  const char* scope;
public:
  explicit evt(const char* name, const char* scope="*") : name(name),
                                                          scope(scope) {}
  uvm_expr* build() const {
    return new uvm_default_event<T>(name, scope);
  }
};

} // namespace uvm_dsl

#endif /* SRC_UVM_DSL_UVM_DSL_VAR_H_ */
