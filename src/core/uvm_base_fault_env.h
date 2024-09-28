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

#ifndef UVM_BASE_FAULT_ENV_H_
#define UVM_BASE_FAULT_ENV_H_

#include <systemc>
#include <tlm.h>
#include <uvm>
#include <map>
#include <list>

class uvm_eca_engine;
class uvm_var_poller;
class uvm_var;
class uvm_waitable_watcher_if;
class uvm_watcher;

class uvm_base_fault_env : public uvm::uvm_component
{
 public:
  uvm_eca_engine* engine;
  uvm_var_poller* poller;

  std::list<uvm_watcher*> watchers;

  std::map<uvm_waitable_watcher_if*, uvm_var*> waiting_vars;

  uvm_base_fault_env( uvm::uvm_component_name name );

  void build_phase(uvm::uvm_phase& phase);

  UVM_COMPONENT_UTILS(uvm_base_fault_env);

};

#endif /* uvm_base_fault_env_H_ */
