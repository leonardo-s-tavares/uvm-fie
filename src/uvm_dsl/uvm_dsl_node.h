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

#ifndef SRC_UVM_DSL_UVM_DSL_NODE_H_
#define SRC_UVM_DSL_UVM_DSL_NODE_H_

#include "../uvm_expr/uvm_var_types.h"
#include "../uvm_expr/uvm_expr.h"

namespace uvm_dsl {

class abstract_node {};

template<class Content, class Type> struct node : public abstract_node {
  typedef Content content_type;
  typedef Type type;
  static const uvm_var_types::type type_value = uvm_var_types::to_value<type>::value;

  ::uvm_expr* build() const { return content_type::build(); }
};

} // namespace uvm_dsl

#endif /* SRC_UVM_DSL_UVM_DSL_NODE_H_ */
