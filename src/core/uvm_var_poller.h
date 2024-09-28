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

#ifndef UVM_VAR_POLLER_H_
#define UVM_VAR_POLLER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

class uvm_var_poller : public uvm::uvm_component
{
 public:
  uvm_var_poller( uvm::uvm_component_name name );

  void build_phase(uvm::uvm_phase& phase);

  void run_phase(uvm::uvm_phase& phase);

  UVM_COMPONENT_UTILS(uvm_var_poller);

};

#endif /* uvm_base_fault_env_H_ */
