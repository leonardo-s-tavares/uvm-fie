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

#ifndef UVM_FAULT_SET_H_
#define UVM_FAULT_SET_H_

#include <systemc>
#include <uvm>
#include <vector>
#include "uvm_fault.h"
#include "../uvm_expr/uvm_var.h"

class uvm_fault_set : public uvm_fault
{
public:
  uvm_var* var;
  std::vector<char> bytes;
  void* ptr;
  std::size_t len;
  uvm_fault_set(uvm_var* var_, void* copy_bytes, void* ptr_bytes,
                std::size_t length);
  uvm_fault_set(uvm_var* var_, void* bytes, std::size_t length);
  virtual ~uvm_fault_set();
  void inject();
};

#endif /* UVM_FAULT_ */
