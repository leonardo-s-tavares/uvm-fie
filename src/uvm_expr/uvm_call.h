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

#ifndef SRC_UVM_EXPR_UVM_CALL_H_
#define SRC_UVM_EXPR_UVM_CALL_H_

#include "../uvm_dsl/uvm_dsl_node.h"
#include "uvm_value.h"

struct uvm_call : public uvm_value {
  uvm_call(uvm_var_types::type type);
  virtual ~uvm_call();

  void visit(uvm_visitor& visitor);
  virtual void visit_args(uvm_visitor& visitor) = 0;
};

template<class T> struct base_uvm_call : public uvm_call {
  base_uvm_call() : uvm_call(uvm_var_types::to_value<T>::value) {}
  virtual ~base_uvm_call() {}
};

#define FI_LIB_RT_N uvm_call0
#define FI_LIB_DSL_N basic_call0
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call1
#define FI_LIB_DSL_N basic_call1
#define FI_LIB_NS FI_LIB_XL(1)
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call2
#define FI_LIB_DSL_N basic_call2
#define FI_LIB_NS FI_LIB_X(1) FI_LIB_XL(2)
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call3
#define FI_LIB_DSL_N basic_call3
#define FI_LIB_NS FI_LIB_X(1) FI_LIB_X(2) FI_LIB_XL(3)
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call4
#define FI_LIB_DSL_N basic_call4
#define FI_LIB_NS FI_LIB_X(1) FI_LIB_X(2) FI_LIB_X(3) FI_LIB_XL(4)
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call5
#define FI_LIB_DSL_N basic_call5
#define FI_LIB_NS FI_LIB_X(1) FI_LIB_X(2) FI_LIB_X(3) FI_LIB_X(4)  FI_LIB_XL(5)
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call6
#define FI_LIB_DSL_N basic_call6
#define FI_LIB_NS FI_LIB_X(1) FI_LIB_X(2) FI_LIB_X(3) FI_LIB_X(4)  FI_LIB_X(5) FI_LIB_XL(6)
#include "uvm_call_generate.h"

#define FI_LIB_RT_N uvm_call7
#define FI_LIB_DSL_N basic_call7
#define FI_LIB_NS FI_LIB_X(1) FI_LIB_X(2) FI_LIB_X(3) FI_LIB_X(4)  FI_LIB_X(5) FI_LIB_X(6) FI_LIB_XL(7)
#include "uvm_call_generate.h"



#endif /* SRC_UVM_EXPR_UVM_CALL_H_ */
