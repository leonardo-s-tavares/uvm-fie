#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

// Router's current address.
#define cur_addr_x 1
#define cur_addr_y 1

// To set the packet destination address, according to the DUT's rules.
void get_address(char ch_in, int *x, int *y)
{
  char chs_out[5]; // Will have the possible destination addresses relative to the router's address (x=1, y=1).
  int ch_out; // Will have the destination address sorted.
  
  switch(ch_in) { // Each case will define the possible set of destination addresses, according to the router's rules and considering the current address.
  case 'L':
    chs_out[0] = 'X'; chs_out[1] = 'N'; chs_out[2] = 'E'; chs_out[3] = 'S'; chs_out[4] = 'W';
    break;
  case 'N':
    chs_out[0] = 'L'; chs_out[1] = 'X'; chs_out[2] = 'X'; chs_out[3] = 'S'; chs_out[4] = 'X';
    break;
  case 'E':
    chs_out[0] = 'L'; chs_out[1] = 'N'; chs_out[2] = 'X'; chs_out[3] = 'S'; chs_out[4] = 'W';
    break;
  case 'S':
    chs_out[0] = 'L'; chs_out[1] = 'N'; chs_out[2] = 'X'; chs_out[3] = 'X'; chs_out[4] = 'X';
    break;
  case 'W':
    chs_out[0] = 'L'; chs_out[1] = 'N'; chs_out[2] = 'E'; chs_out[3] = 'S'; chs_out[4] = 'X';
    break;
  default:
    UVM_FATAL("get_address()", "non-existent ch_in.");
  }
  do {
    ch_out = rand() % 5; // To sort a destination address.
  } while(chs_out[ch_out] == 'X'); // The number sorted needs to be valid, according to the rules set above ('X' = invalid).
  switch(ch_out) { // Will define the x and y addresses based on the number sorted to ch_out.
  case 0: // L
    (*x) = cur_addr_x;
    (*y) = cur_addr_y;
    break;
  case 1: // N
    (*x) = cur_addr_x;
    (*y) = cur_addr_y + 1;
    break;
  case 2: // E
    (*x) = cur_addr_x + 1;
    (*y) = cur_addr_y;
    break;
  case 3: // S
    (*x) = cur_addr_x;
    (*y) = cur_addr_y - 1;
    break;
  case 4: // W
    (*x) = cur_addr_x - 1;
    (*y) = cur_addr_y;
    break;
  default:
    UVM_FATAL("get_address()", "non-existent ch_out.");
  }
}

// Class sequence to send packets to input channel L.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_l_in : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_l_in( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_l_in<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_l_in", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address('L', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_l_in", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel N.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_n_in : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_n_in( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_n_in<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_n_in", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address('N', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_n_in", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel E.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_e_in : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_e_in( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_e_in<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_e_in", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address('E', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_e_in", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel S.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_s_in : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_s_in( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_s_in<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_s_in", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address('S', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_s_in", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel W.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_w_in : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_w_in( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_w_in<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_w_in", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address('W', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_w_in", uvm::UVM_MEDIUM);
  }

};

// ================================================================ TO TEST ALL TO L OUT ================================================================

// To set the packet destination address, according to the DUT's rules.
void get_address_to_l_out(char ch_in, int *x, int *y)
{
  char chs_out[5]; // Will have the possible destination addresses relative to the router's address (x=1, y=1).
  int ch_out; // Will have the destination address sorted.
  
  switch(ch_in) { // Each case will define the possible set of destination addresses, according to the router's rules and considering the current address.
  case 'L':
    chs_out[0] = 'X'; chs_out[1] = 'N'; chs_out[2] = 'E'; chs_out[3] = 'S'; chs_out[4] = 'W';
    break;
  case 'N':
    chs_out[0] = 'L'; chs_out[1] = 'X'; chs_out[2] = 'X'; chs_out[3] = 'X'; chs_out[4] = 'X';
    break;
  case 'E':
    chs_out[0] = 'L'; chs_out[1] = 'X'; chs_out[2] = 'X'; chs_out[3] = 'X'; chs_out[4] = 'X';
    break;
  case 'S':
    chs_out[0] = 'L'; chs_out[1] = 'X'; chs_out[2] = 'X'; chs_out[3] = 'X'; chs_out[4] = 'X';
    break;
  case 'W':
    chs_out[0] = 'L'; chs_out[1] = 'X'; chs_out[2] = 'X'; chs_out[3] = 'X'; chs_out[4] = 'X';
    break;
  default:
    UVM_FATAL("get_address()", "non-existent ch_in.");
  }
  do {
    ch_out = rand() % 5; // To sort a destination address.
  } while(chs_out[ch_out] == 'X'); // The number sorted needs to be valid, according to the rules set above ('X' = invalid).
  switch(ch_out) { // Will define the x and y addresses based on the number sorted to ch_out.
  case 0: // L
    (*x) = cur_addr_x;
    (*y) = cur_addr_y;
    break;
  case 1: // N
    (*x) = cur_addr_x;
    (*y) = cur_addr_y + 1;
    break;
  case 2: // E
    (*x) = cur_addr_x + 1;
    (*y) = cur_addr_y;
    break;
  case 3: // S
    (*x) = cur_addr_x;
    (*y) = cur_addr_y - 1;
    break;
  case 4: // W
    (*x) = cur_addr_x - 1;
    (*y) = cur_addr_y;
    break;
  default:
    UVM_FATAL("get_address_to_l_out()", "non-existent ch_out.");
  }
}

// Class sequence to send packets to input channel N.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_n_in_to_l_out : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_n_in_to_l_out( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_n_in_to_l_out<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_n_in_to_l_out", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address_to_l_out('N', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_n_in_to_l_out", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel E.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_e_in_to_l_out : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_e_in_to_l_out( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_e_in_to_l_out<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_e_in_to_l_out", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address_to_l_out('E', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_e_in_to_l_out", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel S.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_s_in_to_l_out : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_s_in_to_l_out( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_s_in_to_l_out<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_s_in_to_l_out", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address_to_l_out('S', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_s_in_to_l_out", uvm::UVM_MEDIUM);
  }

};

// Class sequence to send packets to input channel W.
template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class sequence_w_in_to_l_out : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  sequence_w_in_to_l_out( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
    uvm_sequence_base::set_automatic_phase_objection(1);
  }

  UVM_OBJECT_PARAM_UTILS(sequence_w_in_to_l_out<REQ,RSP>);
  
  void body()
  {
    REQ* req;
    RSP* rsp;
    int x, y;

    UVM_INFO(this->get_name(), "Starting sequence_w_in_to_l_out", uvm::UVM_MEDIUM);

    for(int i = 0; i < 10; i++) // Each sequence will send 10 packets.
    {
      std::cout << "Sequence " << i << ": " << std::endl;
      
      req = new REQ();
      rsp = new RSP();
      
      get_address_to_l_out('W', &x, &y); // To set the packet destination address, according to the DUT's rules.
      
      req->data[0] = 0b10000000 ^ (x << 4) ^ y; // Writing the control bit and the x and y addresses into the first flit.
      for(int j = 1; j < 4; j++){ // The payload flits (2nd, 3rd and 4th) will have the data.
        req->data[j] = rand() % 0b01111111 + 1; // The data will be a random number between 1 and (2^8)-1.
      }
      req->data[3] ^= 0b10000000; // Writing the control bit into the last flit.
      // TO DO: Ask João Pedro why the flit with parity bit is 8 bits and not 9.

      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      // Handshake protocol with the driver through the TLM port.
      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing sequence_w_in_to_l_out", uvm::UVM_MEDIUM);
  }

};

#endif /* SEQUENCE_H_ */
