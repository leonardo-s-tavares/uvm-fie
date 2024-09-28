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

#include "uvm_capture.h"
#include "../core/uvm_visitor.h"

uvm_capture::uvm_capture(uvm_var_types::type type) : uvm_value(type) {}
uvm_capture::~uvm_capture() {}
void uvm_capture::visit(uvm_visitor& visitor) {
  visitor.visit_capture(this);
}

