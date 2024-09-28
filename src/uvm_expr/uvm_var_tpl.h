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

#ifndef UVM_VAR_TPL_H_
#define UVM_VAR_TPL_H_

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
class uvm_var_tpl : public uvm_var {
public:
  uvm_var_tpl(std::string name_, std::string escope_,
              uvm_var_types::type type_) : uvm_var(name_, escope_, type_){}
  uvm_var_tpl(std::string name_, std::string escope_)
      : uvm_var(name_, escope_, uvm_var_types::to_value<T>::value) { }
  void* get(){
    T* out = (T*) malloc(sizeof(T));
    T* ptr = 0;
    if (!uvm_config_db<T*>::get(0, this->escope, this->name, ptr))
        UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    *out = *ptr;
    return out;
  }

  void set(void* var_){
    T* ptr = 0;
    if (!uvm_config_db<T*>::get(0, this->escope, this->name, ptr))
        UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    *ptr = *((T*) var_);
  }

  uvm_watcher* create_watcher(uvm_base_fault_env* env){
    std::cout << "++++ UVM_VAR_TPL : create_watcher - generic" << '\n';
    uvm_var_watcher* w = new uvm_var_watcher(this);
    env->watchers.push_back(w);
    return w;
  }
};

template<class T>
class uvm_var_tpl < sc_core::sc_signal< T > * > : public uvm_var {
public:
  uvm_var_tpl(std::string name_, std::string escope_)
: uvm_var(name_, escope_, uvm_var_types::SIGNAL,
                          uvm_var_types::to_value<T>::value) { }
  void* get(){
    sc_core::sc_signal< T > ** out = (sc_core::sc_signal< T > **) malloc(sizeof(sc_core::sc_signal< T >*));
    sc_core::sc_signal< T > ** ptr = 0;
    if (!uvm_config_db< sc_core::sc_signal< T > **>::get(0, this->escope, this->name, ptr))
      UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    *out = *ptr;
    return out;
  }
  void set(void* var_){
    std::cout << "UVM VARRR SETTT" << '\n';
    sc_core::sc_signal< T > ** ptr = 0;
    if (!uvm_config_db< sc_core::sc_signal< T > **>::get(0, this->escope, this->name, ptr))
      UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    (*ptr)->write(*((T*) var_));
    x++;
    const_cast<sc_core::sc_event*>(&((*ptr)->default_event()))->cancel();
    const_cast<sc_core::sc_event*>(&((*ptr)->default_event()))->notify();
    sc_core::wait(1, sc_core::SC_NS);

    // ((*ptr)->default_event()).cancel();

    // (&((*ptr)->default_event()))->cancel();
    // (&((*ptr)->default_event())).notify(sc_core::SC_ZERO_TIME);

  }
  uvm_watcher* create_watcher(uvm_base_fault_env* env){
    std::cout << "++++ UVM_VAR_TPL : create_watcher - sc_signal" << '\n';
    uvm_waitable_watcher_if* watcher = uvm_waitable_watcher<sc_core::sc_signal< T > *, uvm_default_waiter>::type_id::create("waitable_watcher_"+this->name, env);
    env->waiting_vars.insert(std::make_pair(watcher,this));
    return watcher;
  }
};

template<class T>
class uvm_var_tpl < sc_core::sc_in< T > * > : public uvm_var {
public:
  uvm_var_tpl(std::string name_, std::string escope_)
      : uvm_var(name_, escope_, uvm_var_types::SC_IN,
                                uvm_var_types::to_value<T>::value) { }
  void* get(){
    sc_core::sc_in< T > ** out = (sc_core::sc_in< T > **) malloc(sizeof(sc_core::sc_in< T >*));
    sc_core::sc_in< T > ** ptr = 0;
    if (!uvm_config_db< sc_core::sc_in< T > **>::get(0, this->escope, this->name, ptr))
      UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    *out = *ptr;
    return out;
  }
  void set(void* var_){
      UVM_FATAL(this->name, "Not possible write at an sc_in port! Simulation aborted!");
  }
  uvm_watcher* create_watcher(uvm_base_fault_env* env){
    std::cout << "++++ UVM_VAR_TPL : create_watcher - sc_in" << '\n';
    uvm_waitable_watcher_if* watcher = uvm_waitable_watcher<sc_core::sc_in< T > *, uvm_default_waiter>::type_id::create("waitable_watcher_"+this->name, env);
    env->waiting_vars.insert(std::make_pair(watcher,this));
    return watcher;
  }
};


template<class T>
class uvm_var_tpl < sc_core::sc_out< T > * > : public uvm_var {
public:
  uvm_var_tpl(std::string name_, std::string escope_)
: uvm_var(name_, escope_, uvm_var_types::SC_OUT,
                          uvm_var_types::to_value<T>::value) { }
  void* get(){
    sc_core::sc_out< T > ** out = (sc_core::sc_out< T > **) malloc(sizeof(sc_core::sc_out< T >*));
    sc_core::sc_out< T > ** ptr = 0;
    if (!uvm_config_db< sc_core::sc_out< T > **>::get(0, this->escope, this->name, ptr))
      UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    *out = *ptr;
    return out;
  }
  void set(void* var_){

    UVM_FATAL(this->name, "Not possible write at an sc_in port! Simulation aborted!");

    sc_core::sc_out< T > ** ptr = 0;
    if (!uvm_config_db< sc_core::sc_out< T > **>::get(0, this->escope, this->name, ptr))
      UVM_FATAL(this->name, "Virtual MEMORY not defined! Simulation aborted!");
    (*ptr)->write(*((T*) var_));
  }
  uvm_watcher* create_watcher(uvm_base_fault_env* env){
    std::cout << "++++ UVM_VAR_TPL : create_watcher - sc_out" << '\n';
    uvm_waitable_watcher_if* watcher = uvm_waitable_watcher<sc_core::sc_out< T > *, uvm_default_waiter>::type_id::create("waitable_watcher_"+this->name, env);
    env->waiting_vars.insert(std::make_pair(watcher,this));
    return watcher;
  }
};

#endif
