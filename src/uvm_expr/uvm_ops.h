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

#ifndef SRC_UVM_EXPR_UVM_OPS_H_
#define SRC_UVM_EXPR_UVM_OPS_H_

#include <systemc>
#include <uvm>
#include "uvm_expr.h"

#define FI_LIB_UVM_BOOL_BIN_OPS \
  FI_LIB_X(gt) \
  FI_LIB_X(lt) \
  FI_LIB_X(gte) \
  FI_LIB_X(lte) \
  FI_LIB_X(eq) \
  FI_LIB_X(neq) \
  FI_LIB_X(and) \
  FI_LIB_X(or)

#define FI_LIB_UVM_SELF_BIN_OPS \
  FI_LIB_X(band) \
  FI_LIB_X(bor) \
  FI_LIB_X(xor) \
  FI_LIB_X(plus) \
  FI_LIB_X(minus) \
  FI_LIB_X(divide) \
  FI_LIB_X(mult)

#define FI_LIB_UVM_BIN_OPS \
  FI_LIB_UVM_BOOL_BIN_OPS \
  FI_LIB_UVM_SELF_BIN_OPS


#define FI_LIB_X(name) \
  class uvm_##name : public uvm_expr { \
public: \
  uvm_expr* left; \
  uvm_expr* right; \
  uvm_##name(uvm_expr* left, uvm_expr* right) : left(left), right(right) {} \
  virtual ~uvm_##name(){}; \
  void visit(uvm_visitor& visitor); \
};
FI_LIB_UVM_BIN_OPS
#undef FI_LIB_X

#endif /* SRC_UVM_EXPR_UVM_OPS_H_ */
