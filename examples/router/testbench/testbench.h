#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "vip_agent.h"
#include "vip_packet.h"
#include "scoreboard.h"
#include "sequence.h"

#include "vip_fault_env.h"

class testbench : public uvm::uvm_env
{
 public:
  // Input and output agents, an agent for each channel.
  vip_agent* agent_l_in;
  vip_agent* agent_n_in;
  vip_agent* agent_e_in;
  vip_agent* agent_s_in;
  vip_agent* agent_w_in;
  vip_agent* agent_l_out;
  vip_agent* agent_n_out;
  vip_agent* agent_e_out;
  vip_agent* agent_s_out;
  vip_agent* agent_w_out;

  scoreboard* scoreboard0;
  
  vip_fault_env* fault_env;

  UVM_COMPONENT_UTILS(testbench);

  testbench( uvm::uvm_component_name name ) : uvm::uvm_env(name)
  {
   std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
  }

  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    uvm::uvm_env::build_phase(phase);

    agent_l_in = vip_agent::type_id::create("agent_l_in", this);
    assert(agent_l_in);
    agent_n_in = vip_agent::type_id::create("agent_n_in", this);
    assert(agent_n_in);
    agent_e_in = vip_agent::type_id::create("agent_e_in", this);
    assert(agent_e_in);
    agent_s_in = vip_agent::type_id::create("agent_s_in", this);
    assert(agent_s_in);
    agent_w_in = vip_agent::type_id::create("agent_w_in", this);
    assert(agent_w_in);
    agent_l_out = vip_agent::type_id::create("agent_l_out", this);
    assert(agent_l_out);
    agent_n_out = vip_agent::type_id::create("agent_n_out", this);
    assert(agent_n_out);
    agent_e_out = vip_agent::type_id::create("agent_e_out", this);
    assert(agent_e_out);
    agent_s_out = vip_agent::type_id::create("agent_s_out", this);
    assert(agent_s_out);
    agent_w_out = vip_agent::type_id::create("agent_w_out", this);
    assert(agent_w_out);

    scoreboard0 = scoreboard::type_id::create("scoreboard0", this);
    assert(scoreboard0);

    fault_env = vip_fault_env::type_id::create("fault_env", this);
    assert(fault_env);
    
    // Set the input agents as active (with driver, sequencer and monitor) and the output agents as passive (with monitor only).
    uvm::uvm_config_db<int>::set(this, "agent_l_in",  "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<int>::set(this, "agent_n_in",  "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<int>::set(this, "agent_e_in",  "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<int>::set(this, "agent_s_in",  "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<int>::set(this, "agent_w_in",  "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<int>::set(this, "agent_l_out", "is_active", uvm::UVM_PASSIVE);
    uvm::uvm_config_db<int>::set(this, "agent_n_out", "is_active", uvm::UVM_PASSIVE);
    uvm::uvm_config_db<int>::set(this, "agent_e_out", "is_active", uvm::UVM_PASSIVE);
    uvm::uvm_config_db<int>::set(this, "agent_s_out", "is_active", uvm::UVM_PASSIVE);
    uvm::uvm_config_db<int>::set(this, "agent_w_out", "is_active", uvm::UVM_PASSIVE);

  }

  void connect_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": connect_phase " << name() << std::endl;

    // Connection between the monitors and the scoreboard with TLM analysis ports.
    agent_l_in->monitor_in->item_collected_port.connect(scoreboard0->l_in_imp);
    agent_n_in->monitor_in->item_collected_port.connect(scoreboard0->n_in_imp);
    agent_e_in->monitor_in->item_collected_port.connect(scoreboard0->e_in_imp);
    agent_s_in->monitor_in->item_collected_port.connect(scoreboard0->s_in_imp);
    agent_w_in->monitor_in->item_collected_port.connect(scoreboard0->w_in_imp);
    agent_l_out->monitor_out->item_collected_port.connect(scoreboard0->l_out_imp);
    agent_n_out->monitor_out->item_collected_port.connect(scoreboard0->n_out_imp);
    agent_e_out->monitor_out->item_collected_port.connect(scoreboard0->e_out_imp);
    agent_s_out->monitor_out->item_collected_port.connect(scoreboard0->s_out_imp);
    agent_w_out->monitor_out->item_collected_port.connect(scoreboard0->w_out_imp);
  }
  
  void extract_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": extract_phase " << name() << std::endl;
    
    // To copy all the packet_info matrices from the monitors to the scoreboard.
    for(int i = 0; i < 100; i++) {
      for(int j = 0; j < 5; j++) {
        scoreboard0->packet_info_l_in[i][j]  = agent_l_in->monitor_in->packet_info[i][j];
        scoreboard0->packet_info_n_in[i][j]  = agent_n_in->monitor_in->packet_info[i][j];
        scoreboard0->packet_info_e_in[i][j]  = agent_e_in->monitor_in->packet_info[i][j];
        scoreboard0->packet_info_s_in[i][j]  = agent_s_in->monitor_in->packet_info[i][j];
        scoreboard0->packet_info_w_in[i][j]  = agent_w_in->monitor_in->packet_info[i][j];
        scoreboard0->packet_info_l_out[i][j] = agent_l_out->monitor_out->packet_info[i][j];
        scoreboard0->packet_info_n_out[i][j] = agent_n_out->monitor_out->packet_info[i][j];
        scoreboard0->packet_info_e_out[i][j] = agent_e_out->monitor_out->packet_info[i][j];
        scoreboard0->packet_info_s_out[i][j] = agent_s_out->monitor_out->packet_info[i][j];
        scoreboard0->packet_info_w_out[i][j] = agent_w_out->monitor_out->packet_info[i][j];
      }
    }
  }
};

#endif /* TESTBENCH_H_ */
