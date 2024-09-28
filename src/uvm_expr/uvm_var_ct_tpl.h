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

#ifndef UVM_VAR_CT_TPL_H_
#define UVM_VAR_CT_TPL_H_

#include <systemc>
#include <uvm>
#include <string>
#include <list>
#include "uvm_var.h"
#include "uvm_expr.h"
#include "uvm_var_types.h"
#include "../core/uvm_base_fault_env.h"
#include "../core/uvm_waitable_watcher.h"
#include "../core/uvm_watcher.h"
#include "../core/uvm_var_watcher.h"
#include "../core/uvm_default_waiter.h"

template< class T>
class uvm_var_ct_tpl : public uvm_var {
public:
  T value;
  uvm_var_ct_tpl( const T& value)
      : uvm_var("", "", uvm_var_types::to_value<T>::value), value(value) { }
  void* get(){
    T* out = (T*) malloc(sizeof(T));
    *out = value;
    return out;
  }

  void set(void* var_){;
    UVM_FATAL(this->name, "Constant cannot be set! Simulation aborted!");
  }

  uvm_watcher* create_watcher(uvm_base_fault_env* env){
    return NULL;
  }
};

#endif
