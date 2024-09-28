#include <systemc>
#include <uvm>

#include "out_subscriber.h"
#include "vip_packet.h"
#include "scoreboard.h"

using namespace uvm;
using namespace sc_core;

out_subscriber::out_subscriber( uvm_component_name name ) : uvm_subscriber<vip_packet>( name )
{
  std::cout << sc_time_stamp() << ": constructor " << name << std::endl;
}

// To call the correspondant write function, write_out(), when write() is called by the monitor_out.
void out_subscriber::write(const vip_packet& p)
{
  uvm_object* obj;
  scoreboard* sb;

  uvm::uvm_config_db<uvm_object*>::get(this, "", "sb", obj);

  sb = dynamic_cast<scoreboard*>(obj);

  sb->write_out(p);
}
