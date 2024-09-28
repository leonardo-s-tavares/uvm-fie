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

#ifndef UVM_VISITOR_H_
#define UVM_VISITOR_H_

#include <systemc>
#include <uvm>

using namespace uvm;

#include "uvm_expr/uvm_ops.h"
#include "uvm_expr/uvm_var.h"
#include "uvm_expr/uvm_capture.h"
#include "uvm_expr/uvm_call.h"

class uvm_visitor
{
public:
   virtual ~uvm_visitor() {}
#define FI_LIB_X(name) virtual void visit_##name(uvm_##name *) = 0;
   FI_LIB_UVM_BOOL_BIN_OPS
#undef FI_LIB_X
   virtual void visit_var(uvm_var* var_) = 0;
   virtual void visit_capture(uvm_capture* var_) = 0;
   virtual void visit_call(uvm_call* var_) = 0;
};

#endif /* UVM_EXPR_H_ */
