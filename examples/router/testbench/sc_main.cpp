#include <systemc>
#include <uvm>

#include <iostream>

#include "vip_ch_if.h"
#include "testbench.h"
#include "test.h"
#include "../dut/router/router.h"

// Router's parameters.
#define data_width   8
#define addr_x       1 
#define addr_y       1
#define depth        6

extern volatile bool fi_enable;

int sc_main(int, char*[])
{
  srand((unsigned) time(NULL));
  
  // Setting the router.
  router<data_width> *router0 = new router<data_width>("router");
  sc_signal<sc_uint<32>> x_id, y_id;
  x_id.write(addr_x);
  y_id.write(addr_y);
  
  // Global clock with a period of 1ns.
  sc_core::sc_clock clk("clk", 1, SC_NS);
  sc_core::sc_clock *clk_i = &clk;
  
  // Global reset.
  sc_core::sc_signal<sc_bv<1>> rst;
  sc_core::sc_signal<sc_bv<1>> *rst_i = &rst;
  
  sc_core::sc_signal<sc_dt::sc_int<1>> trigger;
  sc_core::sc_signal<sc_dt::sc_int<1>> *trigger_i = &trigger;
  
  // Interfaces, one for each input and each output channel.
  vip_ch_if* router_ch_if_l_in  = new vip_ch_if();
  vip_ch_if* router_ch_if_n_in  = new vip_ch_if();
  vip_ch_if* router_ch_if_e_in  = new vip_ch_if();
  vip_ch_if* router_ch_if_s_in  = new vip_ch_if();
  vip_ch_if* router_ch_if_w_in  = new vip_ch_if();
  vip_ch_if* router_ch_if_l_out = new vip_ch_if();
  vip_ch_if* router_ch_if_n_out = new vip_ch_if();
  vip_ch_if* router_ch_if_e_out = new vip_ch_if();
  vip_ch_if* router_ch_if_s_out = new vip_ch_if();
  vip_ch_if* router_ch_if_w_out = new vip_ch_if();

  // Connections between the DUT and the signals/interfaces.
  router0->clk_i       (clk);
  router0->rst_i       (rst);
  router0->x_id_p      (x_id);
  router0->y_id_p      (y_id);
  router0->l_in_data_i (router_ch_if_l_in->data);
  router0->l_in_val_i  (router_ch_if_l_in->val);
  router0->l_in_ack_o  (router_ch_if_l_in->ack);
  router0->n_in_data_i (router_ch_if_n_in->data);
  router0->n_in_val_i  (router_ch_if_n_in->val);
  router0->n_in_ack_o  (router_ch_if_n_in->ack);
  router0->e_in_data_i (router_ch_if_e_in->data);
  router0->e_in_val_i  (router_ch_if_e_in->val);
  router0->e_in_ack_o  (router_ch_if_e_in->ack);
  router0->s_in_data_i (router_ch_if_s_in->data);
  router0->s_in_val_i  (router_ch_if_s_in->val);
  router0->s_in_ack_o  (router_ch_if_s_in->ack);
  router0->w_in_data_i (router_ch_if_w_in->data);
  router0->w_in_val_i  (router_ch_if_w_in->val);
  router0->w_in_ack_o  (router_ch_if_w_in->ack);
  router0->l_out_data_o(router_ch_if_l_out->data);
  router0->l_out_val_o (router_ch_if_l_out->val);
  router0->l_out_ack_i (router_ch_if_l_out->ack);
  router0->n_out_data_o(router_ch_if_n_out->data);
  router0->n_out_val_o (router_ch_if_n_out->val);
  router0->n_out_ack_i (router_ch_if_n_out->ack);
  router0->e_out_data_o(router_ch_if_e_out->data);
  router0->e_out_val_o (router_ch_if_e_out->val);
  router0->e_out_ack_i (router_ch_if_e_out->ack);
  router0->s_out_data_o(router_ch_if_s_out->data);
  router0->s_out_val_o (router_ch_if_s_out->val);
  router0->s_out_ack_i (router_ch_if_s_out->ack);
  router0->w_out_data_o(router_ch_if_w_out->data);
  router0->w_out_val_o (router_ch_if_w_out->val);
  router0->w_out_ack_i (router_ch_if_w_out->ack);
  
  // Reference the signals outside the interfaces to be visible to the UVM components.
  uvm::uvm_config_db<sc_core::sc_clock*           >::set(0, "*", "clk_i", clk_i);
  uvm::uvm_config_db<sc_core::sc_signal<sc_bv<1>>*>::set(0, "*", "rst_i", rst_i);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_int<1>>*>::set(0, "*", "trigger_i", trigger_i);
  
  // Reference interfaces to be visible, each one for each correspondant agent (virtual interfaces).
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_l_in.*",  "router_ch_if", router_ch_if_l_in);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_n_in.*",  "router_ch_if", router_ch_if_n_in);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_e_in.*",  "router_ch_if", router_ch_if_e_in);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_s_in.*",  "router_ch_if", router_ch_if_s_in);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_w_in.*",  "router_ch_if", router_ch_if_w_in);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_l_out.*", "router_ch_if", router_ch_if_l_out);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_n_out.*", "router_ch_if", router_ch_if_n_out);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_e_out.*", "router_ch_if", router_ch_if_e_out);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_s_out.*", "router_ch_if", router_ch_if_s_out);
  uvm::uvm_config_db<vip_ch_if*>::set(0, "*.tb.agent_w_out.*", "router_ch_if", router_ch_if_w_out);
  
  // To dump waveforms.
  sc_trace_file *fp;
  fp=sc_create_vcd_trace_file("wave_router");
  
  // Signals to be dumped.
  sc_trace(fp, router0->clk_i,        "clk");
  sc_trace(fp, router0->rst_i,        "rst");
  sc_trace(fp, router0->l_in_data_i,  "l_in_data_i");
  sc_trace(fp, router0->l_in_val_i,   "l_in_val_i");
  sc_trace(fp, router0->l_in_ack_o,   "l_in_ack_o");
  sc_trace(fp, router0->n_in_data_i,  "n_in_data_i");
  sc_trace(fp, router0->n_in_val_i,   "n_in_val_i");
  sc_trace(fp, router0->n_in_ack_o,   "n_in_ack_o");
  sc_trace(fp, router0->e_in_data_i,  "e_in_data_i");
  sc_trace(fp, router0->e_in_val_i,   "e_in_val_i");
  sc_trace(fp, router0->e_in_ack_o,   "e_in_ack_o");
  sc_trace(fp, router0->s_in_data_i,  "s_in_data_i");
  sc_trace(fp, router0->s_in_val_i,   "s_in_val_i");
  sc_trace(fp, router0->s_in_ack_o,   "s_in_ack_o");
  sc_trace(fp, router0->w_in_data_i,  "w_in_data_i");
  sc_trace(fp, router0->w_in_val_i,   "w_in_val_i");
  sc_trace(fp, router0->w_in_ack_o,   "w_in_ack_o");
  sc_trace(fp, router0->l_out_data_o, "l_out_data_o");
  sc_trace(fp, router0->l_out_val_o,  "l_out_val_o");
  sc_trace(fp, router0->l_out_ack_i,  "l_out_ack_i");
  sc_trace(fp, router0->n_out_data_o, "n_out_data_o");
  sc_trace(fp, router0->n_out_val_o,  "n_out_val_o");
  sc_trace(fp, router0->n_out_ack_i,  "n_out_ack_i");
  sc_trace(fp, router0->e_out_data_o, "e_out_data_o");
  sc_trace(fp, router0->e_out_val_o,  "e_out_val_o");
  sc_trace(fp, router0->e_out_ack_i,  "e_out_ack_i");
  sc_trace(fp, router0->s_out_data_o, "s_out_data_o");
  sc_trace(fp, router0->s_out_val_o,  "s_out_val_o");
  sc_trace(fp, router0->s_out_ack_i,  "s_out_ack_i");
  sc_trace(fp, router0->w_out_data_o, "w_out_data_o");
  sc_trace(fp, router0->w_out_val_o,  "w_out_val_o");
  sc_trace(fp, router0->w_out_ack_i,  "w_out_ack_i");
  sc_trace(fp, trigger, "trigger");
  sc_trace(fp, router0->x_id_p_fault_target_from_in, "x_id_p_fault_target_from_in");
  sc_trace(fp, router0->y_id_p_fault_target_from_in, "y_id_p_fault_target_from_in");
  sc_trace(fp, router0->lin0->x_id_p_fault_target_from_in, "lin0_x_id_p_fault_target_from_in");
  sc_trace(fp, router0->nin0->x_id_p_fault_target_from_in, "nin0_x_id_p_fault_target_from_in");
  sc_trace(fp, router0->ein0->x_id_p_fault_target_from_in, "ein0_x_id_p_fault_target_from_in");
  sc_trace(fp, router0->sin0->x_id_p_fault_target_from_in, "sin0_x_id_p_fault_target_from_in");
  sc_trace(fp, router0->win0->x_id_p_fault_target_from_in, "win0_x_id_p_fault_target_from_in");
  sc_trace(fp, router0->lin0->y_id_p_fault_target_from_in, "lin0_y_id_p_fault_target_from_in");
  sc_trace(fp, router0->nin0->y_id_p_fault_target_from_in, "nin0_y_id_p_fault_target_from_in");
  sc_trace(fp, router0->ein0->y_id_p_fault_target_from_in, "ein0_y_id_p_fault_target_from_in");
  sc_trace(fp, router0->sin0->y_id_p_fault_target_from_in, "sin0_y_id_p_fault_target_from_in");
  sc_trace(fp, router0->win0->y_id_p_fault_target_from_in, "win0_y_id_p_fault_target_from_in");
  sc_trace(fp, router0->lin0->flow_in0->state, "lin0_flow_in0_state");
  sc_trace(fp, router0->nin0->flow_in0->state, "nin0_flow_in0_state");
  sc_trace(fp, router0->ein0->flow_in0->state, "ein0_flow_in0_state");
  sc_trace(fp, router0->sin0->flow_in0->state, "sin0_flow_in0_state");
  sc_trace(fp, router0->win0->flow_in0->state, "win0_flow_in0_state");
  sc_trace(fp, router0->lin0->buffering0->fifo_r[0], "lin0_buffering0_fifo_r_0");
  sc_trace(fp, router0->nin0->buffering0->fifo_r[0], "nin0_buffering0_fifo_r_0");
  sc_trace(fp, router0->ein0->buffering0->fifo_r[0], "ein0_buffering0_fifo_r_0");
  sc_trace(fp, router0->sin0->buffering0->fifo_r[0], "sin0_buffering0_fifo_r_0");
  sc_trace(fp, router0->win0->buffering0->fifo_r[0], "win0_buffering0_fifo_r_0");
  sc_trace(fp, router0->lin0->buffering0->rd_ptr_r, "lin0_buffering0_rd_ptr_r");
  sc_trace(fp, router0->nin0->buffering0->rd_ptr_r, "nin0_buffering0_rd_ptr_r");
  sc_trace(fp, router0->ein0->buffering0->rd_ptr_r, "ein0_buffering0_rd_ptr_r");
  sc_trace(fp, router0->sin0->buffering0->rd_ptr_r, "sin0_buffering0_rd_ptr_r");
  sc_trace(fp, router0->win0->buffering0->rd_ptr_r, "win0_buffering0_rd_ptr_r");
  sc_trace(fp, router0->lin0->routing0->x_id_p0_fault_target_from_in, "lin0_routing0_x_id_p0_fault_target_from_in");
  sc_trace(fp, router0->nin0->routing0->x_id_p0_fault_target_from_in, "nin0_routing0_x_id_p0_fault_target_from_in");
  sc_trace(fp, router0->ein0->routing0->x_id_p0_fault_target_from_in, "ein0_routing0_x_id_p0_fault_target_from_in");
  sc_trace(fp, router0->sin0->routing0->x_id_p0_fault_target_from_in, "sin0_routing0_x_id_p0_fault_target_from_in");
  sc_trace(fp, router0->win0->routing0->x_id_p0_fault_target_from_in, "win0_routing0_x_id_p0_fault_target_from_in");
  sc_trace(fp, router0->lin0->routing0->y_id_p0_fault_target_from_in, "lin0_routing0_y_id_p0_fault_target_from_in");
  sc_trace(fp, router0->nin0->routing0->y_id_p0_fault_target_from_in, "nin0_routing0_y_id_p0_fault_target_from_in");
  sc_trace(fp, router0->ein0->routing0->y_id_p0_fault_target_from_in, "ein0_routing0_y_id_p0_fault_target_from_in");
  sc_trace(fp, router0->sin0->routing0->y_id_p0_fault_target_from_in, "sin0_routing0_y_id_p0_fault_target_from_in");
  sc_trace(fp, router0->win0->routing0->y_id_p0_fault_target_from_in, "win0_routing0_y_id_p0_fault_target_from_in");
  sc_trace(fp, router0->lin0->routing0->state, "lin0_routing0_state");
  sc_trace(fp, router0->nin0->routing0->state, "nin0_routing0_state");
  sc_trace(fp, router0->ein0->routing0->state, "ein0_routing0_state");
  sc_trace(fp, router0->sin0->routing0->state, "sin0_routing0_state");
  sc_trace(fp, router0->win0->routing0->state, "win0_routing0_state");
  sc_trace(fp, router0->lout0->arbitration0->state, "lout0_arbitration0_state");
  sc_trace(fp, router0->nout0->arbitration0->state, "nout0_arbitration0_state");
  sc_trace(fp, router0->eout0->arbitration0->state, "eout0_arbitration0_state");
  sc_trace(fp, router0->sout0->arbitration0->state, "sout0_arbitration0_state");
  sc_trace(fp, router0->wout0->arbitration0->state, "wout0_arbitration0_state");
  sc_trace(fp, router0->lout0->flow_out0->state, "lout0_flow_out0_state");
  sc_trace(fp, router0->nout0->flow_out0->state, "nout0_flow_out0_state");
  sc_trace(fp, router0->eout0->flow_out0->state, "eout0_flow_out0_state");
  sc_trace(fp, router0->sout0->flow_out0->state, "sout0_flow_out0_state");
  sc_trace(fp, router0->wout0->flow_out0->state, "wout0_flow_out0_state");
  
  // Fault injection - Trigger events.
  sc_core::sc_signal<sc_dt::sc_int<1>> *ptr_trg = &(trigger);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_int<1>>**>::set(0, "*", "trg_*", &ptr_trg);
  
  // Fault injection - Targets.
  sc_core::sc_signal<sc_dt::sc_uint<32>> *x_id_p_fault_target_from_in = &(router0->x_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *y_id_p_fault_target_from_in = &(router0->y_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_x_id_p_fault_target_from_in = &(router0->lin0->x_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_x_id_p_fault_target_from_in = &(router0->nin0->x_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_x_id_p_fault_target_from_in = &(router0->ein0->x_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_x_id_p_fault_target_from_in = &(router0->sin0->x_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_x_id_p_fault_target_from_in = &(router0->win0->x_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_y_id_p_fault_target_from_in = &(router0->lin0->y_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_y_id_p_fault_target_from_in = &(router0->nin0->y_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_y_id_p_fault_target_from_in = &(router0->ein0->y_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_y_id_p_fault_target_from_in = &(router0->sin0->y_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_y_id_p_fault_target_from_in = &(router0->win0->y_id_p_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_flow_in0_state = &(router0->lin0->flow_in0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_flow_in0_state = &(router0->nin0->flow_in0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_flow_in0_state = &(router0->ein0->flow_in0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_flow_in0_state = &(router0->sin0->flow_in0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_flow_in0_state = &(router0->win0->flow_in0->state);
  sc_core::sc_signal<sc_dt::sc_uint<data_width>> *lin0_buffering0_fifo_r_0 = &(router0->lin0->buffering0->fifo_r[0]);
  sc_core::sc_signal<sc_dt::sc_uint<data_width>> *nin0_buffering0_fifo_r_0 = &(router0->nin0->buffering0->fifo_r[0]);
  sc_core::sc_signal<sc_dt::sc_uint<data_width>> *ein0_buffering0_fifo_r_0 = &(router0->ein0->buffering0->fifo_r[0]);
  sc_core::sc_signal<sc_dt::sc_uint<data_width>> *sin0_buffering0_fifo_r_0 = &(router0->sin0->buffering0->fifo_r[0]);
  sc_core::sc_signal<sc_dt::sc_uint<data_width>> *win0_buffering0_fifo_r_0 = &(router0->win0->buffering0->fifo_r[0]);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_buffering0_rd_ptr_r = &(router0->lin0->buffering0->rd_ptr_r);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_buffering0_rd_ptr_r = &(router0->nin0->buffering0->rd_ptr_r);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_buffering0_rd_ptr_r = &(router0->ein0->buffering0->rd_ptr_r);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_buffering0_rd_ptr_r = &(router0->sin0->buffering0->rd_ptr_r);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_buffering0_rd_ptr_r = &(router0->win0->buffering0->rd_ptr_r);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_routing0_x_id_p0_fault_target_from_in = &(router0->lin0->routing0->x_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_routing0_x_id_p0_fault_target_from_in = &(router0->nin0->routing0->x_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_routing0_x_id_p0_fault_target_from_in = &(router0->ein0->routing0->x_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_routing0_x_id_p0_fault_target_from_in = &(router0->sin0->routing0->x_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_routing0_x_id_p0_fault_target_from_in = &(router0->win0->routing0->x_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_routing0_y_id_p0_fault_target_from_in = &(router0->lin0->routing0->y_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_routing0_y_id_p0_fault_target_from_in = &(router0->nin0->routing0->y_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_routing0_y_id_p0_fault_target_from_in = &(router0->ein0->routing0->y_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_routing0_y_id_p0_fault_target_from_in = &(router0->sin0->routing0->y_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_routing0_y_id_p0_fault_target_from_in = &(router0->win0->routing0->y_id_p0_fault_target_from_in);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lin0_routing0_state = &(router0->lin0->routing0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nin0_routing0_state = &(router0->nin0->routing0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *ein0_routing0_state = &(router0->ein0->routing0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sin0_routing0_state = &(router0->sin0->routing0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *win0_routing0_state = &(router0->win0->routing0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lout0_arbitration0_state = &(router0->lout0->arbitration0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nout0_arbitration0_state = &(router0->nout0->arbitration0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *eout0_arbitration0_state = &(router0->eout0->arbitration0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sout0_arbitration0_state = &(router0->sout0->arbitration0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *wout0_arbitration0_state = &(router0->wout0->arbitration0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *lout0_flow_out0_state = &(router0->lout0->flow_out0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *nout0_flow_out0_state = &(router0->nout0->flow_out0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *eout0_flow_out0_state = &(router0->eout0->flow_out0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *sout0_flow_out0_state = &(router0->sout0->flow_out0->state);
  sc_core::sc_signal<sc_dt::sc_uint<32>> *wout0_flow_out0_state = &(router0->wout0->flow_out0->state);
  
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "x_id_p_fault_target_from_in", &x_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "y_id_p_fault_target_from_in", &y_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_x_id_p_fault_target_from_in", &lin0_x_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_x_id_p_fault_target_from_in", &nin0_x_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_x_id_p_fault_target_from_in", &ein0_x_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_x_id_p_fault_target_from_in", &sin0_x_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_x_id_p_fault_target_from_in", &win0_x_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_y_id_p_fault_target_from_in", &lin0_y_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_y_id_p_fault_target_from_in", &nin0_y_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_y_id_p_fault_target_from_in", &ein0_y_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_y_id_p_fault_target_from_in", &sin0_y_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_y_id_p_fault_target_from_in", &win0_y_id_p_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_flow_in0_state", &lin0_flow_in0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_flow_in0_state", &nin0_flow_in0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_flow_in0_state", &ein0_flow_in0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_flow_in0_state", &sin0_flow_in0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_flow_in0_state", &win0_flow_in0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<data_width>>**>::set(0, "*", "lin0_buffering0_fifo_r_0", &lin0_buffering0_fifo_r_0);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<data_width>>**>::set(0, "*", "nin0_buffering0_fifo_r_0", &nin0_buffering0_fifo_r_0);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<data_width>>**>::set(0, "*", "ein0_buffering0_fifo_r_0", &ein0_buffering0_fifo_r_0);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<data_width>>**>::set(0, "*", "sin0_buffering0_fifo_r_0", &sin0_buffering0_fifo_r_0);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<data_width>>**>::set(0, "*", "win0_buffering0_fifo_r_0", &win0_buffering0_fifo_r_0);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_buffering0_rd_ptr_r", &lin0_buffering0_rd_ptr_r);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_buffering0_rd_ptr_r", &nin0_buffering0_rd_ptr_r);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_buffering0_rd_ptr_r", &ein0_buffering0_rd_ptr_r);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_buffering0_rd_ptr_r", &sin0_buffering0_rd_ptr_r);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_buffering0_rd_ptr_r", &win0_buffering0_rd_ptr_r);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_routing0_x_id_p0_fault_target_from_in", &lin0_routing0_x_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_routing0_x_id_p0_fault_target_from_in", &nin0_routing0_x_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_routing0_x_id_p0_fault_target_from_in", &ein0_routing0_x_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_routing0_x_id_p0_fault_target_from_in", &sin0_routing0_x_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_routing0_x_id_p0_fault_target_from_in", &win0_routing0_x_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_routing0_y_id_p0_fault_target_from_in", &lin0_routing0_y_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_routing0_y_id_p0_fault_target_from_in", &nin0_routing0_y_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_routing0_y_id_p0_fault_target_from_in", &ein0_routing0_y_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_routing0_y_id_p0_fault_target_from_in", &sin0_routing0_y_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_routing0_y_id_p0_fault_target_from_in", &win0_routing0_y_id_p0_fault_target_from_in);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lin0_routing0_state", &lin0_routing0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nin0_routing0_state", &nin0_routing0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "ein0_routing0_state", &ein0_routing0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sin0_routing0_state", &sin0_routing0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "win0_routing0_state", &win0_routing0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lout0_arbitration0_state", &lout0_arbitration0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nout0_arbitration0_state", &nout0_arbitration0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "eout0_arbitration0_state", &eout0_arbitration0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sout0_arbitration0_state", &sout0_arbitration0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "wout0_arbitration0_state", &wout0_arbitration0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "lout0_flow_out0_state", &lout0_flow_out0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "nout0_flow_out0_state", &nout0_flow_out0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "eout0_flow_out0_state", &eout0_flow_out0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "sout0_flow_out0_state", &sout0_flow_out0_state);
  uvm::uvm_config_db<sc_core::sc_signal<sc_dt::sc_uint<32>>**>::set(0, "*", "wout0_flow_out0_state", &wout0_flow_out0_state);
  
  // Configure if faults will be injected or not.
  fi_enable = true;
  
  // Start one of the tests.
  //uvm::run_test("test"); // Generates random packets to random output channels.
  uvm::run_test("test_all_to_l_out"); // Generates random packets to channel l_out.
  
  // Close the file with the waves.
  sc_close_vcd_trace_file(fp);
  
  return 0;
}
