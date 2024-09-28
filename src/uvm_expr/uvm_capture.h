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

#ifndef SRC_UVM_EXPR_UVM_CAPTURE_H_
#define SRC_UVM_EXPR_UVM_CAPTURE_H_

#include <systemc>
#include <uvm>
#include "uvm_value.h"
#include "uvm_var_types.h"

class uvm_capture : public uvm_value {
public:
  uvm_capture(uvm_var_types::type type);
  virtual ~uvm_capture();
  void visit(uvm_visitor& visitor);
  virtual void* get() = 0;
  virtual void* peek() = 0;
  virtual void set(void* value) = 0;
};

#endif /* SRC_UVM_EXPR_UVM_CAPTURE_H_ */
