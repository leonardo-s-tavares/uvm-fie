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

#ifndef uvm_waitable_watcher_H_
#define uvm_waitable_watcher_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "uvm_watcher.h"
#include "uvm_base_fault_env.h"

template <class T>
class uvm_default_event;

class uvm_waitable_watcher_if : public uvm_watcher {
  public:
    virtual ~uvm_waitable_watcher_if(){}
};

template<class T, class W>
class uvm_waitable_watcher : public uvm::uvm_component,
                             public uvm_waitable_watcher_if
{
 public:
  T* holder;
  uvm_default_event<T>* evt_var;  

  uvm_waitable_watcher(uvm::uvm_component_name name)
  : uvm::uvm_component( name ), evt_var(0){}

  UVM_COMPONENT_UTILS(uvm_waitable_watcher<T,W>);

  void set_evt_var(uvm_default_event<T>* evt){
    this->evt_var = evt;
  }

  void build_phase(uvm::uvm_phase& phase)
  {
    uvm::uvm_component::build_phase(phase);
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    // uvm_base_fault_env* parent = (uvm_base_fault_env*) this->get_parent();
    uvm_base_fault_env* parent = dynamic_cast<uvm_base_fault_env*>(this->get_parent());


    std::map<uvm_waitable_watcher_if*, uvm_var*>::iterator it = parent->waiting_vars.find(this);
    if(it == parent->waiting_vars.end())
      UVM_FATAL(name(), "Couldn't find var to wait! Simulation aborted!");

    uvm_var* var_= it->second;
    // std::cout << "escope: " << var_->escope << " name: "<< var_->name << '\n';
    // if(std::is_same<*T, sc_core::sc_out<sc_dt::sc_int<16> > >::value)
    //   std::cout << "asasasdad" << '\n';
    // std::cout << " T ::: " << typeid(T).type()  << '\n';
    if (!uvm::uvm_config_db< T * >::get(0, var_->escope, var_->name, holder))
      UVM_FATAL(name(), "Virtual interface not defined! Simulation aborted!");
  }

  void run_phase(uvm::uvm_phase& phase)
  {
    while(1){
      W::wait(*holder);
      if(evt_var) evt_var->notify();
      uvm_base_fault_env* parent = (uvm_base_fault_env*) this->get_parent();
      parent->engine->notify(this);
      std::cout << "RRRRRRRRRRRRRRRRRRRRRRRR" << std::endl;
    }
  }
};

#endif /* uvm_waitable_watcher_H_ */
