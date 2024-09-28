#ifndef VIP_CH_IF_H_
#define VIP_CH_IF_H_

#include <systemc>
#include <uvm>

#define link_width_p 8

using namespace uvm;

class vip_ch_if // Generic interface to represent a channel.
{
 public:
  sc_core::sc_signal<sc_dt::sc_bv<link_width_p>> data;
  sc_core::sc_signal<sc_dt::sc_bv<1>> val;
  sc_core::sc_signal<sc_dt::sc_bv<1>> ack;

  vip_ch_if() {}
};


#endif /* VIP_CH_IF_H_ */
