#ifndef IN_SUBSCRIBER_H_
#define IN_SUBSCRIBER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "vip_packet.h"

class in_subscriber : public uvm::uvm_subscriber<vip_packet>
{
 public:
  in_subscriber( uvm::uvm_component_name name );

  UVM_COMPONENT_UTILS(in_subscriber);

  virtual void write(const vip_packet& p);
};

#endif /* IN_SUBSCRIBER_H_ */
