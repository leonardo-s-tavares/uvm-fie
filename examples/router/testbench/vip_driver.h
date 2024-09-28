#ifndef VIP_DRIVER_H_
#define VIP_DRIVER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "vip_packet.h"
#include "vip_ch_if.h"

template <class REQ>
class vip_driver : public uvm::uvm_driver<REQ>
{
 public:
  // Signals and virtual interface.
  sc_core::sc_signal<sc_dt::sc_int<1>> *trigger_i;
  sc_core::sc_clock                    *clk_i;
  sc_core::sc_signal<sc_dt::sc_bv<1>>  *rst_i;
  vip_ch_if*                           router_ch_vif;

  vip_driver( uvm::uvm_component_name name ) : uvm::uvm_driver<REQ>(name)
  {}

  UVM_COMPONENT_PARAM_UTILS(vip_driver<REQ>);

  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << this->name() << std::endl;

    uvm_driver<REQ>::build_phase(phase);

    // To reference the local pointers (signals and virtual interface) to the real ones from the top.
    if (!uvm_config_db<sc_core::sc_signal<sc_dt::sc_int<1>>*>::get(this, "*", "trigger_i", trigger_i))
      UVM_FATAL(this->name(), "trigger_i not defined! Simulation aborted!");
    if (!uvm_config_db<sc_core::sc_clock*>::get(this, "*", "clk_i", clk_i))
      UVM_FATAL(this->name(), "clk_i not defined! Simulation aborted!");
    if (!uvm_config_db<sc_core::sc_signal<sc_dt::sc_bv<1>>*>::get(this, "*", "rst_i", rst_i))
      UVM_FATAL(this->name(), "rst_i not defined! Simulation aborted!");
    if (!uvm::uvm_config_db<vip_ch_if*>::get(this, "*", "router_ch_if", router_ch_vif))
      UVM_FATAL(this->name(), "Virtual interface router_ch_if not defined! Simulation aborted!");
  }

  void run_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << this->name() << " " << phase.get_name() << "..." << std::endl;

    REQ req, rsp; // Packets from the sequencer.
    
    // Reset the DUT before starting the test.
    trigger_i->write(0);
    rst_i->write("1");
    sc_core::wait(clk_i->posedge_event());
    trigger_i->write(1);
    rst_i->write("0");
    
    while(true) // To execute all sequences.
    {
      this->seq_item_port->get_next_item(req); // Request next sequence item from the sequencer.
      drive_transfer(req); // To send the sequence item to the DUT.
      rsp.set_id_info(req);
      this->seq_item_port->item_done(); // To finish the handshake with the sequencer.
      this->seq_item_port->put_response(rsp);
    }
  }
  
  virtual void drive_transfer(REQ& p) {
    sc_core::wait(rand() % 50, sc_core::SC_NS); // To randomize the time between each packet sent.
    for(int i = 0; i < 4; i++) { // To send the 4 flits of a packet. 
      router_ch_vif->val.write("0"); // Start the handshake with valid = 0.
      std::cout<<"data_sent["<<i<<"] = "<<p.data[i]<<std::endl;
      router_ch_vif->data.write(p.data[i]); // Send the flit [i] to the DUT.
      sc_core::wait(clk_i->posedge_event()); // Wait until next posedge to avoid race condition.
      router_ch_vif->val.write("1"); // Set valid to 1 to indicate that the flit can be read by the DUT.
      while(router_ch_vif->ack.read() != "1") // Waits ack = 1 from the DUT to indicate that the flit was read.
        sc_core::wait(clk_i->posedge_event());
      router_ch_vif->val.write("0"); // Set valid to 0 to complete the handshake.
      while(router_ch_vif->ack.read() != "0") // Waits ack return to 0.
        sc_core::wait(clk_i->posedge_event());
    }
    router_ch_vif->data.write(0);
    std::cout << sc_core::sc_time_stamp() << ": " << this->name() << " sent " << std::endl;
  }
  
};

#endif
