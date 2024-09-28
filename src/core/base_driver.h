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

#ifndef BASE_DRIVER_H_
#define BASE_DRIVER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

extern volatile bool fi_enable;

template <class REQ>
class base_driver : public uvm::uvm_driver<REQ>
{
 public:

  base_driver( uvm::uvm_component_name name ) : uvm::uvm_driver<REQ>(name)
  {
    fi_enable = false;
  }

  virtual ~base_driver(){}

  UVM_COMPONENT_PARAM_UTILS(base_driver<REQ>);

  void run_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << this->name() << " " << phase.get_name() << "..." << std::endl;

    REQ req, rsp;

    while(true)
    {
      pre_transfer(req);
      enable_fi();
      drive_transfer(req);
      disable_fi();
      post_transfer(req,rsp);
    }
  }
  void enable_fi(){
    __sync_bool_compare_and_swap(&fi_enable, false, true);
  }
  void disable_fi(){
    __sync_bool_compare_and_swap(&fi_enable, true, false);
  }

  virtual void pre_transfer(REQ& req) = 0;
  virtual void drive_transfer(const REQ& req) = 0;
  virtual void post_transfer(REQ& req ,REQ& rsp) = 0;

};

#endif /* VIP_DRIVER_H_ */
