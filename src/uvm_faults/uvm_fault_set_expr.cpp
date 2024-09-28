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

#include <systemc>
#include <uvm>
#include "uvm_fault_set_expr.h"
#include "../core/uvm_eval_visitor.h"

uvm_fault_set_expr::uvm_fault_set_expr(uvm_var* var, uvm_expr* expr)
    : var(var), expr(expr) {
}

uvm_fault_set_expr::~uvm_fault_set_expr() {
  delete var;
  delete expr;
}

void uvm_fault_set_expr::inject() {
  uvm_eval_visitor v;
  expr->visit(v);
  if (v.child_type != var->set_type)
    UVM_FATAL("uvm_fault_set_expr", "Type Mismatch");
  var->set(v.value);
  free(v.value);
}
