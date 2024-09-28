#ifndef VIP_AGENT_H_
#define VIP_AGENT_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "vip_sequencer.h"
#include "vip_driver.h"
#include "vip_monitor_in.h"
#include "vip_monitor_out.h"
#include "vip_packet.h"

class vip_agent : public uvm::uvm_agent
{
 public:
  vip_sequencer<vip_packet>* sequencer;
  vip_driver<vip_packet>*    driver;
  vip_monitor_in*            monitor_in;
  vip_monitor_out*           monitor_out;

  vip_agent( uvm::uvm_component_name name )
    : uvm_agent(name), sequencer(0), driver(0), monitor_in(0), monitor_out(0)
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
  }

  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    uvm::uvm_agent::build_phase(phase);

    if (get_is_active() == uvm::UVM_ACTIVE)
    {
      UVM_INFO(get_name(), "is set to UVM_ACTIVE", UVM_NONE);

      // An active agent will have a sequencer, a driver and a monitor to read the input channels.
      sequencer = vip_sequencer<vip_packet>::type_id::create("sequencer", this);
      assert(sequencer);
      driver = vip_driver<vip_packet>::type_id::create("driver", this);
      assert(driver);
      monitor_in = vip_monitor_in::type_id::create("monitor_in", this);
      assert(monitor_in);

    } else {
      UVM_INFO(get_name(), "is set to UVM_PASSIVE", UVM_NONE);
      
      // A passive agent will have only a monitor to read the output channels.
      monitor_out = vip_monitor_out::type_id::create("monitor_out", this);
      assert(monitor_out);
      
    }
    
  }


  void connect_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": connect_phase " << name() << std::endl;

    if (get_is_active() == uvm::UVM_ACTIVE)
    {
      // Connection between the sequencer and the driver with a TLM port.
      driver->seq_item_port.connect(sequencer->seq_item_export);
    }
  }

  UVM_COMPONENT_UTILS(vip_agent);

};

#endif /* VIP_AGENT_H_ */
