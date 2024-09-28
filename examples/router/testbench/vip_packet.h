#ifndef VIP_PACKET_H
#define VIP_PACKET_H

#include <systemc>
#include <tlm.h>
#include <uvm>
#include <vector>

#define link_width_p 8 // The flits with be 8-bit wide.

class vip_packet : public uvm::uvm_sequence_item
{
 public:
  UVM_OBJECT_UTILS(vip_packet);

  vip_packet(const std::string& name = "packet") { }
  virtual ~vip_packet() { }

 public:
  sc_dt::sc_bv<link_width_p> data[4]; // Each packet will have 4 flits. The first one will have the address, and the next ones will have the data.
};

#endif /* VIP_PACKET_H_ */
