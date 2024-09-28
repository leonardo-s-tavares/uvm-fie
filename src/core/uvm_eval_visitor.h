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

#ifndef UVM_EVAL_VISITOR
#define UVM_EVAL_VISITOR

#include <systemc>
#include <uvm>
#include "uvm_visitor.h"

class uvm_eval_visitor : public uvm_visitor {

  public:
    uvm_var_types::type child_type;
    void* value;
    uvm_eval_visitor();

#define FI_LIB_X(name) \
    void visit_##name(uvm_##name * node);
    FI_LIB_UVM_BIN_OPS
#undef FI_LIB_X

    void visit_value(uvm_value* node);
    void visit_var(uvm_var* node) {visit_value(node);}
    void visit_capture(uvm_capture* node) {visit_value(node);}
    void visit_call(uvm_call* node) {visit_value(node);}

  private:
    template<class Node, class Map>
    void visit_bool_bin_op(Node* node, const Map& map);
    template<class Node, class Map>
    void visit_bin_op(Node* node, const Map& map);
};

#endif /* uvm_eval_visitor */
