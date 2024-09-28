#include <uvm>
#include <systemc>
#include <string>
#include <iostream>

#include "testbench.h"
#include "sequence.h"

// Set codes to identify each test.
#define TEST              0
#define TEST_ALL_TO_L_OUT 1

class test: public uvm::uvm_test
{
public:
  testbench* tb;

  test(uvm::uvm_component_name name) : uvm_test(name), tb(0)
  {}

  UVM_COMPONENT_UTILS(test);

  virtual void build_phase(uvm::uvm_phase& phase )
    {
      uvm::uvm_test::build_phase(phase);

      tb = testbench::type_id::create("tb", this);
      assert(tb);
      
      // Send the information that the current test running is TEST.
      uvm::uvm_config_db<int>::set(this, "*", "current_test", TEST);
      
      // Start the sequences, a sequence for each input channel.
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_l_in.sequencer.run_phase", "default_sequence", sequence_l_in<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_n_in.sequencer.run_phase", "default_sequence", sequence_n_in<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_e_in.sequencer.run_phase", "default_sequence", sequence_e_in<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_s_in.sequencer.run_phase", "default_sequence", sequence_s_in<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_w_in.sequencer.run_phase", "default_sequence", sequence_w_in<vip_packet>::type_id::get());
      }

    virtual void run_phase( uvm::uvm_phase& phase )
    {
      // Raise and drop objection is necessary to prevent the test to finish before all sequences are completed.
      phase.raise_objection(this);
      
      UVM_INFO( get_name(), "** UVM TEST STARTED **", UVM_NONE);
      sc_core::wait(1000, sc_core::SC_NS); // Duration of the test.
      
      phase.drop_objection(this);
    }
};

class test_all_to_l_out: public uvm::uvm_test
{
public:
  testbench* tb;

  test_all_to_l_out(uvm::uvm_component_name name) : uvm_test(name), tb(0)
  {}

  UVM_COMPONENT_UTILS(test_all_to_l_out);

  virtual void build_phase(uvm::uvm_phase& phase )
    {
      uvm::uvm_test::build_phase(phase);

      tb = testbench::type_id::create("tb", this);
      assert(tb);
      
      // Send the information that the current test running is TEST_ALL_TO_L_OUT.
      uvm::uvm_config_db<int>::set(this, "*", "current_test", TEST_ALL_TO_L_OUT);
      
      // Start the sequences, a sequence for each input channel.
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_n_in.sequencer.run_phase", "default_sequence", sequence_n_in_to_l_out<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_e_in.sequencer.run_phase", "default_sequence", sequence_e_in_to_l_out<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_s_in.sequencer.run_phase", "default_sequence", sequence_s_in_to_l_out<vip_packet>::type_id::get());
      uvm::uvm_config_db<uvm_object_wrapper*>::set(this, "tb.agent_w_in.sequencer.run_phase", "default_sequence", sequence_w_in_to_l_out<vip_packet>::type_id::get());
    }

    virtual void run_phase( uvm::uvm_phase& phase )
    {
      // Raise and drop objection is necessary to prevent the test to finish before all sequences are completed.
      phase.raise_objection(this);
      
      UVM_INFO( get_name(), "** UVM TEST STARTED **", UVM_NONE);
      sc_core::wait(1000, sc_core::SC_NS); // Duration of the test.
      
      phase.drop_objection(this);
    }
};
