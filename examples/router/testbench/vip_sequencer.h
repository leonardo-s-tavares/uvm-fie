#ifndef VIP_SEQUENCER_H_
#define VIP_SEQUENCER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

template <class REQ>
class vip_sequencer : public uvm::uvm_sequencer<REQ> // Generic sequencer, with nothing special.
{
 public:
  vip_sequencer( uvm::uvm_component_name name ) : uvm::uvm_sequencer<REQ>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
  }

  UVM_COMPONENT_PARAM_UTILS(vip_sequencer<REQ>);

};

#endif /* VIP_SEQUENCER_H_ */
