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

#include "uvm_watcher_extractor.h"
#include "uvm_eca_engine.h"

uvm_watcher_extractor::uvm_watcher_extractor(uvm_eca_engine* engine, uvm_expr* condition) : engine(engine), condition(condition) {}

template<class Node> void uvm_watcher_extractor::visit_bin(Node* node) {
  node->left->visit(*this);
  node->right->visit(*this);
}

#define FI_LIB_X(name) \
    void uvm_watcher_extractor::visit_##name(uvm_##name * node) { \
      visit_bin(node); \
    }
    FI_LIB_UVM_BIN_OPS
#undef FI_LIB_X

void uvm_watcher_extractor::visit_var(uvm_var* var_) {
  uvm_watcher* w = var_->create_watcher(engine->env);

  if(w){
  	this->engine->register_watcher(w, this->condition);
	std::cout << "++++++ uvm_watcher_extractor::visit_var" << '\n';
  }
}

void uvm_watcher_extractor::visit_call(uvm_call* call_) {
  call_->visit_args(*this);
}

void uvm_watcher_extractor::visit_capture(uvm_capture* cap_) {
  std::cout << "++++++ uvm_watcher_extractor::visit_capture" << '\n';
}
