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

#ifndef UVM_VAR_H_
#define UVM_VAR_H_

#include <systemc>
#include <uvm>
#include <string>
#include "uvm_value.h"

class uvm_watcher;
class uvm_base_fault_env;

class uvm_var : public uvm_value {
public:
  std::string name;
  std::string escope;
  uvm_var_types::type set_type;

  uvm_var(std::string name_, std::string escope_, uvm_var_types::type type_);
  uvm_var(std::string name_, std::string escope_,
          uvm_var_types::type type_, uvm_var_types::type set_type_);
  virtual ~uvm_var();
  void visit(uvm_visitor& visitor);
  virtual void* get() = 0;
  virtual void set(void* var_) = 0;
  virtual uvm_watcher* create_watcher(uvm_base_fault_env* env) = 0;
};

#endif
