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

#ifndef UVM_DEFAULT_EVENT_H_
#define UVM_DEFAULT_EVENT_H_

#include <systemc>
#include <uvm>
#include <string>

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
#include "../core/uvm_default_waiter.h"

template< class T>
class uvm_default_event : public uvm_var {
public:
  bool event_flag;
  uvm_default_event(std::string name_, std::string escope_) : uvm_var(name_, escope_, uvm_var_types::BOOL){
    event_flag = false;
  }
  void* get(){
    bool* out = (bool*) malloc(sizeof(bool));
    // *out = true;
    *out = event_flag;
    event_flag = false;
    return out;
  }

  void notify(){
    event_flag = true;
  }

  void set(void* var_){}

  uvm_watcher* create_watcher(uvm_base_fault_env* env){
    std::cout << "++++ UVM_DEFAULT_EVENT : create_watcher " << '\n';
    uvm_waitable_watcher<T, uvm_default_waiter>* watcher = uvm_waitable_watcher< T, uvm_default_waiter>::type_id::create("waitable_watcher_"+this->name, env);
    watcher->set_evt_var(this);
    env->waiting_vars.insert(std::make_pair(watcher,this));
    return watcher;
  }
};

#endif
