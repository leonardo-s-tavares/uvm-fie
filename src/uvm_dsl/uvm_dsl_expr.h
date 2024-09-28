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

#ifndef SRC_UVM_DSL_UVM_DSL_EXPR_H_
#define SRC_UVM_DSL_UVM_DSL_EXPR_H_

#include "../uvm_expr/uvm_expr.h"

namespace uvm_dsl {

class expr : public uvm_expr {
private:
  uvm_expr* m_delegate;
public:
  template<class Node> expr(Node root) : m_delegate(root.build()) {}
  inline virtual ~expr() {delete m_delegate;}
  virtual void visit(uvm_visitor& visitor) {m_delegate->visit(visitor);}
};

template<class Node>
::uvm_expr* to_expr(Node node) {
  return node.build();
}

} // namespace uvm_dsl

#endif /* SRC_UVM_DSL_UVM_DSL_EXPR_H_ */
