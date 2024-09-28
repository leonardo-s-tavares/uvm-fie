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

#ifndef UVM_WATCHER_EXTRACTOR
#define UVM_WATCHER_EXTRACTOR

#include <systemc>
#include <uvm>
#include "uvm_visitor.h"
#include "uvm_expr/uvm_expr.h"

class uvm_var_watcher;
class uvm_eca_engine;

class uvm_watcher_extractor : public uvm_visitor {
  private:
     uvm_eca_engine* engine;
     uvm_expr* condition;

  public:
    uvm_watcher_extractor(uvm_eca_engine* engine, uvm_expr* condition);

#define FI_LIB_X(name) \
    void visit_##name(uvm_##name * node);
    FI_LIB_UVM_BIN_OPS
#undef FI_LIB_X

    void visit_var(uvm_var* var_);
    void visit_call(uvm_call* call_);
    void visit_capture(uvm_capture* cap_);
private:
    template<class Node> void visit_bin(Node* node);
};

#endif /* UVM_WATCHER_EXTRACTOR */
