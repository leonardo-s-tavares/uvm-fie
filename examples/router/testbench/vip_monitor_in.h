#ifndef VIP_MONITOR_IN_H_
#define VIP_MONITOR_IN_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "vip_ch_if.h"

class vip_monitor_in : public uvm::uvm_monitor
{
 public:
  // Analysis port to communicate with the scoreboard.
  uvm::uvm_analysis_port<vip_packet> item_collected_port;

  // Signals and virtual interface.
  sc_core::sc_clock                   *clk_i;
  sc_core::sc_signal<sc_dt::sc_bv<1>> *rst_i; 
  vip_ch_if*                           router_ch_vif;
  
  int packet_info[100][5] = {{0}}; // To store the start/end time of each packet.
  // Columns: [0] = Start time; [1] End time; [2] = Address; [3] = First payload (identifier); [4] = For internal control.
  
  vip_monitor_in(uvm::uvm_component_name name) : uvm_monitor(name), item_collected_port("item_collected_port"), router_ch_vif(0)
  {}

  UVM_COMPONENT_UTILS(vip_monitor_in);

  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    uvm::uvm_monitor::build_phase(phase);
    
    // To reference the local pointers (signals and virtual interface) to the real ones from the top.
    if (!uvm_config_db<sc_core::sc_clock*>::get(this, "*", "clk_i", clk_i))
      UVM_FATAL(this->name(), "clk_i not defined! Simulation aborted!");
    if (!uvm_config_db<sc_core::sc_signal<sc_dt::sc_bv<1>>*>::get(this, "*", "rst_i", rst_i))
      UVM_FATAL(this->name(), "rst_i not defined! Simulation aborted!");
    if (!uvm::uvm_config_db<vip_ch_if*>::get(this, "*", "router_ch_if", router_ch_vif))
      UVM_FATAL(this->name(), "Virtual interface router_ch_if not defined! Simulation aborted!");
  }

  void run_phase( uvm::uvm_phase& phase )
  {
    vip_packet p; // Packet to send to the scoreboard.

    for(int i = 0; true; i++)// Monitor DUT's input channels continuously until the end of the test (defined by the class test).
    {
      for(int j = 0; j < 4; j++) { // To read the 4 flits of each packet.
        while(router_ch_vif->val.read() != "1" || router_ch_vif->data.read() == "0") // While valid is 0 or the flit is "empty", waits.
          sc_core::wait(clk_i->posedge_event());
        p.data[j] = router_ch_vif->data.read(); // When valid is 1 and the flit has data != "0", the flit is read.
        if(j == 0) { // If the first flit was just received by the DUT:
          packet_info[i][0] = int(sc_core::sc_time_stamp().value())/1000; // Register the time that it was captured.
          packet_info[i][2] = router_ch_vif->data.read().to_int() & 0b01111111; // Register the address of the packet.
        }
        if(j == 1) // When going to the first payload data:
          packet_info[i][3] = router_ch_vif->data.read().to_int() & 0b01111111; // Register the data, to use as an identifier.
        if(j == 3) // If the last flit was just received by the DUT:
          packet_info[i][1] = int(sc_core::sc_time_stamp().value())/1000; // Register the time that it was captured.
        while(router_ch_vif->val.read() != "0") // Waits valid return to 0.
          sc_core::wait(clk_i->posedge_event());
      }
      std::cout << sc_core::sc_time_stamp() << ": " << name() << " received " << std::endl;
      item_collected_port.write(p); // After the flits sent by the driver are successfully read, the packet can be sent to the scoreboard.
    }
  }
  
};

#endif /* VIP_MONITOR_IN_H_ */
