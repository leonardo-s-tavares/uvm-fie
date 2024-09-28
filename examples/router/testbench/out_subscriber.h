#ifndef OUT_SUBSCRIBER_H_
#define OUT_SUBSCRIBER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "vip_packet.h"

class out_subscriber : public uvm::uvm_subscriber<vip_packet>
{
 public:
  out_subscriber( uvm::uvm_component_name name );

  UVM_COMPONENT_UTILS(out_subscriber);

  virtual void write(const vip_packet& p);
};

#endif /* OUT_SUBSCRIBER_H_ */
