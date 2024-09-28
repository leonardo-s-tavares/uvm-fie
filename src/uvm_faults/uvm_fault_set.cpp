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

#include <systemc>
#include <uvm>

#include "uvm_fault_set.h"

uvm_fault_set::uvm_fault_set(uvm_var* var_, void* copy_bytes, void* ptr_bytes,
                             std::size_t length)
    : var(var_), bytes((char*) copy_bytes, (char*)copy_bytes+length),
      ptr(ptr_bytes), len(length) {}
uvm_fault_set::uvm_fault_set(uvm_var* var_, void* bytes,
                             std::size_t length)
    : var(var_), bytes((char*) bytes, (char*)bytes+length),
      ptr(0), len(length) {}

uvm_fault_set::~uvm_fault_set() {
  //does not take ownership of var nor ptr
}

void uvm_fault_set::inject(){
  var->set(&bytes[0]);
  std::cout << sc_core::sc_time_stamp() << ": UVM_FI - SET FAULT ACTIVE" << std::endl;
}
