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

#ifndef UVM_EXPR_H_
#define UVM_EXPR_H_

#include <systemc>
#include <uvm>

class uvm_visitor;

class uvm_expr {
public:
 uvm_expr() {};
 virtual void visit(uvm_visitor& visitor) = 0;
 virtual ~uvm_expr() {};
};

#endif /* UVM_EXPR_H_ */
