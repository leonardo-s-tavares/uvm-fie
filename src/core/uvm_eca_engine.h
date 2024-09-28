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

#ifndef UVM_ECA_ENGINE
#define UVM_ECA_ENGINE

#include <systemc>
#include <uvm>
#include <map>

class uvm_fault;
class uvm_expr;
class uvm_watcher;
class uvm_base_fault_env;

class uvm_eca_engine
{
  private:
     std::map<uvm_watcher*, uvm_expr*> watcher_target;
     std::map<uvm_expr*, uvm_fault*> expr_trigger;
     void extract_watchers(uvm_expr* condition);
     bool evaluate(uvm_expr* condition);

  public:
    uvm_base_fault_env* env;
    uvm_eca_engine(uvm_base_fault_env* env_);
    virtual ~uvm_eca_engine();
    void register_fault_condition(uvm_expr* condition, uvm_fault* fault);
    void register_watcher(uvm_watcher* var_, uvm_expr* condition);
    void notify(uvm_watcher* watcher);
};

#endif /*UVM_ECA_ENGINE*/
