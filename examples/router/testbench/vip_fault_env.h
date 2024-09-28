#ifndef VIP_FAULT_ENV_H_
#define VIP_FAULT_ENV_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include <fi.h>

#define data_width 8

/*enum ifc_fsm {
	ifcs0, ifcs1, ifcs2, ifcs3, ifcs4, ifcs5, ifcs6, ifcs7
};

enum ic_fsm {
	s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13
};

enum oc_fsm {
	oc_s0, oc_s1, oc_s2, oc_s3, oc_s4, oc_s5, oc_s6, oc_s7
};

enum ofc_fsm {
	ofcs0, ofcs1, ofcs2
};*/

class vip_fault_env : public uvm_base_fault_env
{
public:
  sc_dt::sc_uint<data_width> zero;
  sc_dt::sc_uint<data_width> one;
  sc_dt::sc_uint<32> zero_32bit;
  sc_dt::sc_uint<32> one_32bit;
  sc_dt::sc_uint<64> zero_64bit;
  sc_dt::sc_uint<64> one_64bit;

  UVM_COMPONENT_UTILS(vip_fault_env);

  vip_fault_env( uvm::uvm_component_name name ) : uvm_base_fault_env( name ){
    zero       = 0;
    one        = 1;
    zero_32bit = 0;
    one_32bit  = 1;
    zero_64bit = 0;
    one_64bit  = 1;
  }
  
  // Function to inject random faults in random fault variables.
  void inject_random_fault() {
    using namespace uvm_dsl;
    using namespace sc_core;
    using namespace sc_dt;
    srand(time(NULL)); // To allow true randomization.
    #define VARS 52 // Number of variables in the file.
    
    FILE *fault_variable_library; // Pointer to the file with information about fault variables.
    char fault_variable[VARS][3][50] = {{{0}}}; // Matrix that will store the information about fault variables.
    sc_dt::sc_uint<32> random_value_32bit = rand(); // To initialize with a 32bit random value that will be the faulty value injected.
    sc_dt::sc_uint<8> random_value_8bit = rand(); // To initialize with a 8bit random value that will be the faulty value injected.
    int i, j, k; // Loop variables.
    char cur_char; // To store temporarily the current character from the file.
    
    fault_variable_library = fopen("/home/uvm-fi/examples/router/testbench/variables_to_inject_faults.txt", "r"); // To open the file with fault variables.
    if(fault_variable_library == NULL) {
      std::cout<<"Fault variable library file not found. Will exit."<<std::endl;
      exit(1);
    }
    for (i = 0; i < VARS; i++) { // Loop to go to every line of the fault variable library file.
      for (j = 0; j < 3; j++) { // Loop to go to every fault variable information.
        for(k = 0; k < 49; k++) { // Loop to go to each character of each fault variable information.
          cur_char = fgetc(fault_variable_library); // Write the current character in a temporary variable.
          if(cur_char != ',' && cur_char != '\n' && cur_char != EOF) { // Check if the character is a separator (special character).
            fault_variable[i][j][k] = cur_char; // Write the current character in the matrix.
          } else break; // If is a special character (a separator, end of line or end of file), break the current information reading.
        }
        if(cur_char == '\n' || cur_char == EOF) break; // If the end of the line or file was found, go to the next line or exit the reading.
      }
      if(cur_char == EOF) break; // If the end of the file was found, exit the reading.
    }
    i = rand() % VARS; // To sort a variable to inject the fault.
    if(strcmp(fault_variable[i][1], "sc_signal<sc_uint<8>>") == 0) { // If the fault variable type is a sc_signal<sc_uint<8>>:
      *engine << fm(evt<sc_signal<sc_int<1>>*>(fault_variable[i][0], "*"), var<sc_signal<sc_uint<8>>*>(fault_variable[i][2], "*").set(cap(random_value_8bit))); // To trigger the fault in a sc_signal<sc_uint<8>>.
    } else if(strcmp(fault_variable[i][1], "sc_signal<sc_uint<32>>") == 0) { // If the fault variable type is a sc_signal<sc_uint<32>>:
      *engine << fm(evt<sc_signal<sc_int<1>>*>(fault_variable[i][0], "*"), var<sc_signal<sc_uint<32>>*>(fault_variable[i][2], "*").set(cap(random_value_32bit))); // To trigger the fault in a sc_signal<sc_uint<32>>.
    } else { // If the type of the variable to inject faults isn't listed:
      std::cout<<"ERROR: Fault variable type not recognized: "<<fault_variable[i][1]<<". Will skip this fault injection."; // It's not one of the fault types currently recognized.
    }
    
    fclose(fault_variable_library);
  }

  void build_phase(uvm::uvm_phase& phase)
  {
    using namespace uvm_dsl;
    using namespace sc_core;
    using namespace sc_dt;
  
    std::cout << sc_time_stamp() << ": build_phase " << name() << std::endl;
    uvm_base_fault_env::build_phase(phase);
    
    inject_random_fault(); // To inject a random fault in a random fault variable.
    
    /*
    // 1) < sc_in
    // UVM_FATAL /home/uvm-fi/src/uvm_expr/uvm_var_tpl.h(119) @ 1 ns: reporter [y_id_p] Not possible write at an sc_in port! Simulation aborted!
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_x_id_p", "*"), var<sc_signal<sc_uint<32>>*>("x_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_y_id_p", "*"), var<sc_signal<sc_uint<32>>*>("y_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  
	  // 2) < sc_in
	  // UVM_FATAL /home/uvm-fi/src/uvm_expr/uvm_var_tpl.h(119) @ 1 ns: reporter [win0_y_id_p] Not possible write at an sc_in port! Simulation aborted!
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_x_id_p", "*"), var<sc_signal<sc_uint<32>>*>("lin0_x_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_x_id_p", "*"), var<sc_signal<sc_uint<32>>*>("nin0_x_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_x_id_p", "*"), var<sc_signal<sc_uint<32>>*>("ein0_x_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_x_id_p", "*"), var<sc_signal<sc_uint<32>>*>("sin0_x_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_x_id_p", "*"), var<sc_signal<sc_uint<32>>*>("win0_x_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_y_id_p", "*"), var<sc_signal<sc_uint<32>>*>("lin0_y_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_y_id_p", "*"), var<sc_signal<sc_uint<32>>*>("nin0_y_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_y_id_p", "*"), var<sc_signal<sc_uint<32>>*>("ein0_y_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_y_id_p", "*"), var<sc_signal<sc_uint<32>>*>("sin0_y_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_y_id_p", "*"), var<sc_signal<sc_uint<32>>*>("win0_y_id_p_fault_target_from_in", "*").set(cap(zero_32bit)));
	  
	  // 3) < enum
	  // FAILED.
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_flow_in0_state", "*"), var<sc_signal<sc_uint<32>>*>("lin0_flow_in0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_flow_in0_state", "*"), var<sc_signal<sc_uint<32>>*>("nin0_flow_in0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_flow_in0_state", "*"), var<sc_signal<sc_uint<32>>*>("ein0_flow_in0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_flow_in0_state", "*"), var<sc_signal<sc_uint<32>>*>("sin0_flow_in0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_flow_in0_state", "*"), var<sc_signal<sc_uint<32>>*>("win0_flow_in0_state", "*").set(cap(one_32bit)));
	  
	  // 4) < sc_signal<sc_bv>
	  // FAILED.
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_buffering0_fifo_r_0", "*"), var<sc_signal<sc_uint<data_width>>*>("lin0_buffering0_fifo_r_0", "*").set(cap(one)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_buffering0_fifo_r_0", "*"), var<sc_signal<sc_uint<data_width>>*>("nin0_buffering0_fifo_r_0", "*").set(cap(one)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_buffering0_fifo_r_0", "*"), var<sc_signal<sc_uint<data_width>>*>("ein0_buffering0_fifo_r_0", "*").set(cap(one)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_buffering0_fifo_r_0", "*"), var<sc_signal<sc_uint<data_width>>*>("sin0_buffering0_fifo_r_0", "*").set(cap(one)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_buffering0_fifo_r_0", "*"), var<sc_signal<sc_uint<data_width>>*>("win0_buffering0_fifo_r_0", "*").set(cap(one)));
	  
	  // 5)
	  // PASSED.
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_buffering0_rd_ptr_r", "*"), var<sc_signal<sc_uint<32>>*>("lin0_buffering0_rd_ptr_r", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_buffering0_rd_ptr_r", "*"), var<sc_signal<sc_uint<32>>*>("nin0_buffering0_rd_ptr_r", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_buffering0_rd_ptr_r", "*"), var<sc_signal<sc_uint<32>>*>("ein0_buffering0_rd_ptr_r", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_buffering0_rd_ptr_r", "*"), var<sc_signal<sc_uint<32>>*>("sin0_buffering0_rd_ptr_r", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_buffering0_rd_ptr_r", "*"), var<sc_signal<sc_uint<32>>*>("win0_buffering0_rd_ptr_r", "*").set(cap(one_32bit)));
	  
	  // 6) < sc_in
	  // UVM_FATAL /home/uvm-fi/src/uvm_expr/uvm_var_tpl.h(119) @ 1 ns: reporter [win0_routing0_y_id_p0] Not possible write at an sc_in port! Simulation aborted!
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_routing0_x_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("lin0_routing0_x_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_routing0_x_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("nin0_routing0_x_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_routing0_x_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("ein0_routing0_x_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_routing0_x_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("sin0_routing0_x_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_routing0_x_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("win0_routing0_x_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_routing0_y_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("lin0_routing0_y_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_routing0_y_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("nin0_routing0_y_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_routing0_y_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("ein0_routing0_y_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_routing0_y_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("sin0_routing0_y_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_routing0_y_id_p0", "*"), var<sc_signal<sc_uint<32>>*>("win0_routing0_y_id_p0_fault_target_from_in", "*").set(cap(zero_32bit)));
	  
	  // 7) < enum
	  // FAILED.
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lin0_routing0_state", "*"), var<sc_signal<sc_uint<32>>*>("lin0_routing0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nin0_routing0_state", "*"), var<sc_signal<sc_uint<32>>*>("nin0_routing0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_ein0_routing0_state", "*"), var<sc_signal<sc_uint<32>>*>("ein0_routing0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sin0_routing0_state", "*"), var<sc_signal<sc_uint<32>>*>("sin0_routing0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_win0_routing0_state", "*"), var<sc_signal<sc_uint<32>>*>("win0_routing0_state", "*").set(cap(one_32bit)));
	  
	  // 8) < enum
	  // FAILED.
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lout0_arbitration0_state", "*"), var<sc_signal<sc_uint<32>>*>("lout0_arbitration0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nout0_arbitration0_state", "*"), var<sc_signal<sc_uint<32>>*>("nout0_arbitration0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_eout0_arbitration0_state", "*"), var<sc_signal<sc_uint<32>>*>("eout0_arbitration0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sout0_arbitration0_state", "*"), var<sc_signal<sc_uint<32>>*>("sout0_arbitration0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_wout0_arbitration0_state", "*"), var<sc_signal<sc_uint<32>>*>("wout0_arbitration0_state", "*").set(cap(one_32bit)));
	  
	  // 9) < enum
	  // FAILED.
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_lout0_flow_out0_state", "*"), var<sc_signal<sc_uint<32>>*>("lout0_flow_out0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_nout0_flow_out0_state", "*"), var<sc_signal<sc_uint<32>>*>("nout0_flow_out0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_eout0_flow_out0_state", "*"), var<sc_signal<sc_uint<32>>*>("eout0_flow_out0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_sout0_flow_out0_state", "*"), var<sc_signal<sc_uint<32>>*>("sout0_flow_out0_state", "*").set(cap(one_32bit)));
	  *engine << fm(evt<sc_signal<sc_int<1>>*>("trg_wout0_flow_out0_state", "*"), var<sc_signal<sc_uint<32>>*>("wout0_flow_out0_state", "*").set(cap(one_32bit)));
	  */
  }

};

#endif /* VIP_FAULT_ENV_H_ */
