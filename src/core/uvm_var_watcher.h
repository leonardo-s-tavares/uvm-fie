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

#ifndef UVM_VAR_WATCHER
#define UVM_VAR_WATCHER

#include <systemc>
#include <uvm>

class uvm_var;
#include "uvm_watcher.h"

class uvm_var_watcher : public uvm_watcher{
  public:
  uvm_var* var;
  uvm_var_watcher(uvm_var* var_);
  virtual ~uvm_var_watcher();
};

#endif /* UVM_WATCHER */
