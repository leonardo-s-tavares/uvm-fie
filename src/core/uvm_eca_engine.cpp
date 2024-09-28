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

#include "uvm_eca_engine.h"

#include "uvm_faults/uvm_fault.h"
#include "uvm_expr/uvm_expr.h"
#include "uvm_watcher.h"
#include "uvm_watcher_extractor.h"
#include "uvm_eval_visitor.h"

typedef std::map<uvm_watcher*, uvm_expr*>::iterator wt_it_t;
typedef std::map<uvm_expr*, uvm_fault*>::iterator et_it_t;
volatile bool fi_enable = false;


uvm_eca_engine::uvm_eca_engine(uvm_base_fault_env* env_) : env(env_) {}

uvm_eca_engine::~uvm_eca_engine() {
  for (wt_it_t it=watcher_target.begin(), e=watcher_target.end(); it != e; ++it)
    delete it->first;
  watcher_target.clear();
  for (et_it_t it=expr_trigger.begin(), e=expr_trigger.end(); it != e; ++it) {
    delete it->first;
    delete it->second;
  }
  expr_trigger.clear();
}

void uvm_eca_engine::register_fault_condition(uvm_expr* condition, uvm_fault* fault){
  this->extract_watchers(condition);
  this->expr_trigger.insert(std::make_pair(condition, fault));
}

void uvm_eca_engine::notify(uvm_watcher* watcher) {
    // std::cout << "++++ UVM_ECA_ENGINE ++++" << '\n';
    if(!fi_enable) return;
    std::map<uvm_watcher*, uvm_expr*>::iterator it = this->watcher_target.find(watcher);
    if (it != this->watcher_target.end()) {
      std::pair<uvm_watcher*, uvm_expr*> entry = *it;
      // std::cout << "UVM_ECA_ENGINE: if (it != this->watcher_target.end()) " << '\n';
      if (this->evaluate(entry.second)){
        // std::cout << "UVM_ECA_ENGINE: if (this->evaluate(entry.second)){ " << '\n';
        std::map<uvm_expr*, uvm_fault*>::iterator it_faults = this->expr_trigger.find(entry.second);
        std::pair<uvm_expr*, uvm_fault*> entry_faults = *it_faults;
        entry_faults.second->inject();
      }
    }
}

bool uvm_eca_engine::evaluate(uvm_expr* condition){
  uvm_eval_visitor visitor;
  condition->visit(visitor);
  if(visitor.child_type != uvm_var_types::BOOL){
    UVM_FATAL("uvm_eca_engine", "Conditions must be boolean! Simulation aborted!");
  }
  bool result =  *((bool*) visitor.value);
  free(visitor.value);
  return result;
}

void uvm_eca_engine::extract_watchers(uvm_expr* condition) {
  uvm_watcher_extractor ex(this, condition);
  condition->visit(ex);
}
void uvm_eca_engine::register_watcher(uvm_watcher* var_, uvm_expr* condition){
    std::cout << "UVM_ECA_ENGINE ::register_watcher " << '\n';
    this->watcher_target.insert(std::make_pair(var_, condition));
}
