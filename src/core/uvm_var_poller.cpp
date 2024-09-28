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
#include <tlm.h>
#include <uvm>
#include <list>

#include "uvm_base_fault_env.h"
#include "uvm_var_poller.h"
#include "uvm_eca_engine.h"

uvm_var_poller::uvm_var_poller( uvm::uvm_component_name name ) : uvm_component( name ){
  std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
}

void uvm_var_poller::build_phase(uvm::uvm_phase& phase)
{
  std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;
  uvm::uvm_component::build_phase(phase);
}

void uvm_var_poller::run_phase(uvm::uvm_phase& phase)
{
  uvm_base_fault_env* parent = (uvm_base_fault_env*) this->get_parent();

  std::cout << sc_core::sc_time_stamp() << ": run_phase " << name() << std::endl;

  while(1){
    sc_core::wait(1, sc_core::SC_NS);
    for (std::list<uvm_watcher*>::iterator it = parent->watchers.begin(); it != parent->watchers.end(); it++){
      parent->engine->notify(*it);
      std::cout << "OWWWWWWWWWWWWWW" << std::endl;
    }
  }
}
