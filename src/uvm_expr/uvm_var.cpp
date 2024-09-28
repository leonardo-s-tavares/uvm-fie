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

#include "uvm_var.h"
#include "../core/uvm_visitor.h"

uvm_var::uvm_var(std::string name_, std::string escope_,
                 uvm_var_types::type type_) : uvm_value(type_),
                                              name(name_),
                                              escope(escope_),
                                              set_type(type_) { }
uvm_var::uvm_var(std::string name_, std::string escope_,
                 uvm_var_types::type type_,
                 uvm_var_types::type set_type_) : uvm_value(type_),
                                                  name(name_),
                                                  escope(escope_),
                                                  set_type(set_type_) {}
uvm_var::~uvm_var() {}
void uvm_var::visit(uvm_visitor& visitor) {visitor.visit_var(this);}
