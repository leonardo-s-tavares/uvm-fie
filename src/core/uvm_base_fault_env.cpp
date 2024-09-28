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

#include "uvm_eca_engine.h"
#include "uvm_base_fault_env.h"
#include "uvm_var_poller.h"

uvm_base_fault_env::uvm_base_fault_env( uvm::uvm_component_name name ) : uvm_component( name ){
  std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
}

void uvm_base_fault_env::build_phase(uvm::uvm_phase& phase)
{
  std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;
  uvm::uvm_component::build_phase(phase);
  engine = new uvm_eca_engine(this);
  poller = uvm_var_poller::type_id::create("poller", this);
  assert(poller);
}
