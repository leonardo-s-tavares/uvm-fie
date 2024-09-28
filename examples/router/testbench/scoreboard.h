#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <systemc>
#include <uvm>
#include <vector>

#include "in_subscriber.h"
#include "out_subscriber.h"
#include "vip_packet.h"

// Set codes to identify each test.
#define TEST              0
#define TEST_ALL_TO_L_OUT 1

#define BUF_SIZE 100

class scoreboard : public uvm::uvm_scoreboard
{
 public:
  // To receive what is the current test running.
  int current_test;
 
  // Analysis ports, one for each input/output monitor.
  uvm::uvm_analysis_export<vip_packet> l_in_imp;
  uvm::uvm_analysis_export<vip_packet> n_in_imp;
  uvm::uvm_analysis_export<vip_packet> e_in_imp;
  uvm::uvm_analysis_export<vip_packet> s_in_imp;
  uvm::uvm_analysis_export<vip_packet> w_in_imp;
  uvm::uvm_analysis_export<vip_packet> l_out_imp;
  uvm::uvm_analysis_export<vip_packet> n_out_imp;
  uvm::uvm_analysis_export<vip_packet> e_out_imp;
  uvm::uvm_analysis_export<vip_packet> s_out_imp;
  uvm::uvm_analysis_export<vip_packet> w_out_imp;
  
  // Listeners, one for each input/output monitor.
  in_subscriber*  l_in_listener;
  in_subscriber*  n_in_listener;
  in_subscriber*  e_in_listener;
  in_subscriber*  s_in_listener;
  in_subscriber*  w_in_listener;
  out_subscriber* l_out_listener;
  out_subscriber* n_out_listener;
  out_subscriber* e_out_listener;
  out_subscriber* s_out_listener;
  out_subscriber* w_out_listener;
  
  // Index controllers of packets sent by the drivers or received from the DUT.
  int index_l_in  = BUF_SIZE - 1;
  int index_n_in  = BUF_SIZE - 1;
  int index_e_in  = BUF_SIZE - 1;
  int index_s_in  = BUF_SIZE - 1;
  int index_w_in  = BUF_SIZE - 1;
  int index_l_out = BUF_SIZE - 1;
  int index_n_out = BUF_SIZE - 1;
  int index_e_out = BUF_SIZE - 1;
  int index_s_out = BUF_SIZE - 1;
  int index_w_out = BUF_SIZE - 1;
  
  // Represents the DUT's buffers in the input channels.
  vip_packet buffer_l[BUF_SIZE];
  vip_packet buffer_n[BUF_SIZE];
  vip_packet buffer_e[BUF_SIZE];
  vip_packet buffer_s[BUF_SIZE];
  vip_packet buffer_w[BUF_SIZE];
  
  // To store the start/end time of each packet.
  // Columns: [0] = Start time; [1] End time; [2] = Address; [3] = First payload (identifier); [4] = For internal control.
  int packet_info_l_in[100][5]  = {{0}};
  int packet_info_n_in[100][5]  = {{0}};
  int packet_info_e_in[100][5]  = {{0}};
  int packet_info_s_in[100][5]  = {{0}};
  int packet_info_w_in[100][5]  = {{0}};
  int packet_info_l_out[100][5] = {{0}};
  int packet_info_n_out[100][5] = {{0}};
  int packet_info_e_out[100][5] = {{0}};
  int packet_info_s_out[100][5] = {{0}};
  int packet_info_w_out[100][5] = {{0}};

  scoreboard( uvm::uvm_component_name name ) : uvm::uvm_scoreboard( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
  }

  UVM_COMPONENT_UTILS(scoreboard);

  void build_phase(uvm::uvm_phase& phase)
  {
    uvm::uvm_scoreboard::build_phase(phase);
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;
    
    // Get the current test running.
    uvm::uvm_config_db<int>::get(this, "*", "current_test", current_test);

    // Reference of this scoreboard to the listeners.
    uvm::uvm_config_db<uvm_object*>::set(this, "l_in_listener",  "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "n_in_listener",  "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "e_in_listener",  "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "s_in_listener",  "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "w_in_listener",  "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "l_out_listener", "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "n_out_listener", "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "e_out_listener", "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "s_out_listener", "sb", this);
    uvm::uvm_config_db<uvm_object*>::set(this, "w_out_listener", "sb", this);
    
    l_in_listener = in_subscriber::type_id::create("l_in_listener", this);
    assert(l_in_listener);
    n_in_listener = in_subscriber::type_id::create("n_in_listener", this);
    assert(n_in_listener);
    e_in_listener = in_subscriber::type_id::create("e_in_listener", this);
    assert(e_in_listener);
    s_in_listener = in_subscriber::type_id::create("s_in_listener", this);
    assert(s_in_listener);
    w_in_listener = in_subscriber::type_id::create("w_in_listener", this);
    assert(w_in_listener);
    l_out_listener = out_subscriber::type_id::create("l_out_listener", this);
    assert(l_out_listener);
    n_out_listener = out_subscriber::type_id::create("n_out_listener", this);
    assert(n_out_listener);
    e_out_listener = out_subscriber::type_id::create("e_out_listener", this);
    assert(e_out_listener);
    s_out_listener = out_subscriber::type_id::create("s_out_listener", this);
    assert(s_out_listener);
    w_out_listener = out_subscriber::type_id::create("w_out_listener", this);
    assert(w_out_listener);
  }
  
  void connect_phase(uvm::uvm_phase& phase)
  {
    // Connections with the listeners through analysis ports.
    l_in_imp.connect(l_in_listener->analysis_export);
    n_in_imp.connect(n_in_listener->analysis_export);
    e_in_imp.connect(e_in_listener->analysis_export);
    s_in_imp.connect(s_in_listener->analysis_export);
    w_in_imp.connect(w_in_listener->analysis_export);
    l_out_imp.connect(l_out_listener->analysis_export);
    n_out_imp.connect(n_out_listener->analysis_export);
    e_out_imp.connect(e_out_listener->analysis_export);
    s_out_imp.connect(s_out_listener->analysis_export);
    w_out_imp.connect(w_out_listener->analysis_export);
  }
  
  // Generic algorithm to insert in a queue.
  void queue(vip_packet buffer[BUF_SIZE], const vip_packet& p, int *index_in, int *index_out) {
    if(((*index_out) == 0 && (*index_in) == BUF_SIZE-1) || ((*index_out) == (*index_in)+1)) {
      UVM_ERROR(this->get_name(), "Queue overflow.");
      return;
    }
    if((*index_out) == -1) {
      (*index_out) = 0;
      (*index_in) = 0;
    } else {
      if((*index_in) == BUF_SIZE-1)
        (*index_in) = 0;
      else 
        (*index_in)++;
    }
    buffer[(*index_in)] = p;
  }
  
  // Generic algorithm to unqueue from a queue.
  void unqueue(vip_packet buffer[BUF_SIZE], int *index_in, int *index_out) {
    if((*index_out) == -1) {
      UVM_ERROR(this->get_name(), "Queue underflow.");
      return;
    }
    if((*index_out) == (*index_in)) {
      (*index_out) = -1;
      (*index_in) = -1;
    } else {
      if((*index_out) == BUF_SIZE-1)
        (*index_out) = 0;
      else
        (*index_out)++;
    }
  }
  
  // To store the packets sent by the driver in the scoreboard's buffers.
  void store_pkt(const vip_packet& p) {
    int addr = p.data[0].to_int() & 0b01111111; // Get the address from the first flit.
    switch(addr) { // Currently working with specific addresses, not randomized ones.
    case 0b0010001: // L
      queue(buffer_l, p, &index_l_in, &index_l_out);
      break;
    case 0b0010010: // N
      queue(buffer_n, p, &index_n_in, &index_n_out);
      break;
    case 0b0100001: // E
      queue(buffer_e, p, &index_e_in, &index_e_out);
      break;
    case 0b0010000: // S
      queue(buffer_s, p, &index_s_in, &index_s_out);
      break;
    case 0b0000001: // W
      queue(buffer_w, p, &index_w_in, &index_w_out);
      break;
    default:
      UVM_ERROR(this->get_name(), "Address not recognized.");
      std::cout<<"Addr = "<<addr<<std::endl;
    }
  }
  
  // To check if the packet received from the DUT is in one of the scoreboard's buffer.
  int check_buf(const vip_packet& p, const char direction) {
    for(int i = 0; i < BUF_SIZE; i++) { // Search in the entire buffer.
      int match = 0; // To count the number of flits matching.
      for(int j = 0; j < 4; j++) { // To compare flit by flit.
        switch(direction) { // To choose the correct buffer.
        case 'L':
          if(buffer_l[i].data[j] == p.data[j]) // If the buffer's flit is equal to the flit from the DUT, match++.
            match++;
          break;
        case 'N':
          if(buffer_n[i].data[j] == p.data[j]) // If the buffer's flit is equal to the flit from the DUT, match++.
            match++;
          break;
        case 'E':
          if(buffer_e[i].data[j] == p.data[j]) // If the buffer's flit is equal to the flit from the DUT, match++.
            match++;
          break;
        case 'S':
          if(buffer_s[i].data[j] == p.data[j]) // If the buffer's flit is equal to the flit from the DUT, match++.
            match++;
          break;
        case 'W':
          if(buffer_w[i].data[j] == p.data[j]) // If the buffer's flit is equal to the flit from the DUT, match++.
            match++;
          break;
        default:
          UVM_ERROR(this->get_name(), "Address not recognized.");
        }
      }
      if(match == 4) { // If all flits matched, we consider that the packet was correctly sent/received, with no data corruption.
        switch(direction) { // To choose the correct buffer.
        case 'L':
          unqueue(buffer_l, &index_l_in, &index_l_out);
          break;
        case 'N':
          unqueue(buffer_n, &index_n_in, &index_n_out);
          break;
        case 'E':
          unqueue(buffer_e, &index_e_in, &index_e_out);
          break;
        case 'S':
          unqueue(buffer_s, &index_s_in, &index_s_out);
          break;
        case 'W':
          unqueue(buffer_w, &index_w_in, &index_w_out);
          break;
        default:
          UVM_ERROR(this->get_name(), "Address not recognized.");
        }
        return 1;
      }
    }
    return 0; // If none of the packets in the buffer matched entirely the packet from the DUT, we consider that the packet was not correctly sent/received, or it has data corruption.
  }
  
  // To check if the packet was correctly sent/received.
  void check_pkt(const vip_packet& p) {
    int addr = p.data[0].to_int() & 0b01111111; // Get the address from the first flit.
    switch(addr) {
    case 0b0010001: // L
      if(check_buf(p, 'L')) { // Check if the packet received is in the scoreboard's buffer.
        UVM_INFO(this->get_name(), "Packet sent through channel L is OK.", uvm::UVM_HIGH);
      } else {
        UVM_ERROR(this->get_name(), "Packet sent through channel L is wrong.");
      }
      break;
    case 0b0010010: // N
      if(check_buf(p, 'N')) { // Check if the packet received is in the scoreboard's buffer.
        UVM_INFO(this->get_name(), "Packet sent through channel N is OK.", uvm::UVM_HIGH);
      } else {
        UVM_ERROR(this->get_name(), "Packet sent through channel N is wrong.");
      }
      break;
    case 0b0100001: // E
      if(check_buf(p, 'E')) { // Check if the packet received is in the scoreboard's buffer.
        UVM_INFO(this->get_name(), "Packet sent through channel E is OK.", uvm::UVM_HIGH);
      } else {
        UVM_ERROR(this->get_name(), "Packet sent through channel E is wrong.");
      }
      break;
    case 0b0010000: // S
      if(check_buf(p, 'S')) { // Check if the packet received is in the scoreboard's buffer.
        UVM_INFO(this->get_name(), "Packet sent through channel S is OK.", uvm::UVM_HIGH);
      } else {
        UVM_ERROR(this->get_name(), "Packet sent through channel S is wrong.");
      }
      break;
    case 0b0000001: // W
      if(check_buf(p, 'W')) { // Check if the packet received is in the scoreboard's buffer.
        UVM_INFO(this->get_name(), "Packet sent through channel W is OK.", uvm::UVM_HIGH);
      } else {
        UVM_ERROR(this->get_name(), "Packet sent through channel W is wrong.");
      }
      break;
    default:
      UVM_ERROR(this->get_name(), "Address not recognized.");
      std::cout<<"Addr = "<<addr<<std::endl;
    }
  }
  
  // To print and store in the scoreboard's buffers the packets sent by the driver.
  void write_in(const vip_packet& p)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << name() << " Packet sent " << std::endl;
    for(int i=0; i<4; i++) {
      std::cout << "data_in[" << i << "] = " << p.data[i] <<'\n';
    }
    store_pkt(p);
  }

  // To print and check the packets received from the DUT.
  void write_out(const vip_packet& p)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << name() << " Packet received " << std::endl;
    for(int i=0; i<4; i++) {
      std::cout << "data_out[" << i << "] = " << p.data[i] <<'\n';
    }
    check_pkt(p);
  }
  
  // To check if the number of packets sent is equal to the number of packets received.
  void check_indexes(char direction, int index_in, int index_out)
  {
    if(index_in != index_out) {
      UVM_ERROR(this->get_name(), "Indexes did not matched.");
      std::cout << "index_" << direction << "_in = " << index_in << "; index_" << direction << "_out = " << index_out;
    }
  }
  
  // To print the packet_info matrices.
  void print_packet_info(char direction, char in_or_out, int packet_info[100][5])
  { 
    UVM_INFO(this->get_name(), "print_packet_info(packet_info):", uvm::UVM_LOW);
    std::cout << direction << (in_or_out == 'I' ? "_in" : "_out") << ": Start time; End time; Address; First payload" << '\n';
    for(int i = 0; packet_info[i][0] != 0; i++) {
      for(int j = 0; j < 4; j++) {
        std::cout << packet_info[i][j] << ' ';
      }
      std::cout << '\n';
    }
  }
  
  // To check if the DUT is following the Round Robin algorithm when choosing the order of packets.
  void check_arbitration(char direction, int packet_info_out[100][5])
  {
    int packet_info_out_expected[100][5] = {0};
    int addr;
    
    for(int i = 0; packet_info_out[i][0] != 0; i++) {
      packet_info_out_expected[i][0] = packet_info_out[i][0];
      packet_info_out_expected[i][1] = packet_info_out[i][1];
      packet_info_out_expected[i][2] = packet_info_out[i][2];
    }
    
    switch(direction) {
    case 'L':
      addr = 0b0010001;
      break;
    case 'N':
      addr = 0b0010010;
      break;
    case 'E':
      addr = 0b0100001;
      break;
    case 'S':
      addr = 0b0010000;
      break;
    case 'W':
      addr = 0b0000001;
      break;
    }
    
    for(int i = 0; i < 100; i++) {
      packet_info_l_in[i][4] = (packet_info_l_in[i][2] != addr ? 1 : 0);
      packet_info_n_in[i][4] = (packet_info_n_in[i][2] != addr ? 1 : 0);
      packet_info_e_in[i][4] = (packet_info_e_in[i][2] != addr ? 1 : 0);
      packet_info_s_in[i][4] = (packet_info_s_in[i][2] != addr ? 1 : 0);
      packet_info_w_in[i][4] = (packet_info_w_in[i][2] != addr ? 1 : 0);
    }
    
    int l_i = 0;
    int n_i = 0;
    int e_i = 0;
    int s_i = 0;
    int w_i = 0;
    int next_ch = (current_test == TEST_ALL_TO_L_OUT ? 1 : 0);
    for(int i = 0; packet_info_out[i][0] != 0; i++) {
      while((packet_info_l_in[l_i][0] <= packet_info_out[i][0] - 7) && (packet_info_l_in[l_i][4] == 1))
        l_i++;
      while((packet_info_n_in[n_i][0] <= packet_info_out[i][0] - 7) && (packet_info_n_in[n_i][4] == 1))
        n_i++;
      while((packet_info_e_in[e_i][0] <= packet_info_out[i][0] - 7) && (packet_info_e_in[e_i][4] == 1))
        e_i++;
      while((packet_info_s_in[s_i][0] <= packet_info_out[i][0] - 7) && (packet_info_s_in[s_i][4] == 1))
        s_i++;
      while((packet_info_w_in[w_i][0] <= packet_info_out[i][0] - 7) && (packet_info_w_in[w_i][4] == 1))
        w_i++;
      
      for(int j = 0; j < 5; j++) {
        switch(next_ch) {
        case 0:
          next_ch = 1;
          if((packet_info_l_in[l_i][0] <= packet_info_out[i][0] - 5) && (packet_info_l_in[l_i][2] == addr) && (packet_info_l_in[l_i][4] == 0)) {
            packet_info_out_expected[i][3] = packet_info_l_in[l_i][3];
            packet_info_l_in[l_i][4] = 1;
            goto END;
          }
          break;
        case 1:
          next_ch = 2;
          if((packet_info_n_in[n_i][0] <= packet_info_out[i][0] - 5) && (packet_info_n_in[n_i][2] == addr) && (packet_info_n_in[n_i][4] == 0)) {
            packet_info_out_expected[i][3] = packet_info_n_in[n_i][3];
            packet_info_n_in[n_i][4] = 1;
            goto END;
          }
          break;
        case 2:
          next_ch = 3;
          if((packet_info_e_in[e_i][0] <= packet_info_out[i][0] - 5) && (packet_info_e_in[e_i][2] == addr) && (packet_info_e_in[e_i][4] == 0)) {
            packet_info_out_expected[i][3] = packet_info_e_in[e_i][3];
            packet_info_e_in[e_i][4] = 1;
            goto END;
          }
          break;
        case 3:
          next_ch = 4;
          if((packet_info_s_in[s_i][0] <= packet_info_out[i][0] - 5) && (packet_info_s_in[s_i][2] == addr) && (packet_info_s_in[s_i][4] == 0)) {
            packet_info_out_expected[i][3] = packet_info_s_in[s_i][3];
            packet_info_s_in[s_i][4] = 1;
            goto END;
          }
          break;
        case 4:
          next_ch = (current_test == TEST_ALL_TO_L_OUT ? 1 : 0);
          if((packet_info_w_in[w_i][0] <= packet_info_out[i][0] - 5) && (packet_info_w_in[w_i][2] == addr) && (packet_info_w_in[w_i][4] == 0)) {
            packet_info_out_expected[i][3] = packet_info_w_in[w_i][3];
            packet_info_w_in[w_i][4] = 1;
            goto END;
          }
          break;
        }
      }
      END: asm("NOP");
    }
    
    for(int i = 0; packet_info_out[i][0] != 0; i++)
      if(packet_info_out[i][3] != packet_info_out_expected[i][3]) {
        UVM_ERROR(this->get_name(), "Packet received don't match the packet expected.");
        std::cout << "Packet info: Start time = " << packet_info_out[i][0] << "; End time = " << packet_info_out[i][1] << "; Address = " << packet_info_out[i][2] << "; First payload = " << packet_info_out[i][3] << '\n';
        std::cout << "First payload expected = " << packet_info_out_expected[i][3] << '\n';
      }
  }
  
  void check_buffer_order(char direction, vip_packet buffer[BUF_SIZE], int packet_info_out[100][5])
  {
    for(int i = 0; buffer[i].data[1].to_int() != 0 || packet_info_out[i][3] != 0; i++) {
      if(buffer[i].data[1].to_int() != packet_info_out[i][3]) {
        UVM_INFO(this->get_name(), "SB's buffer and packet_info_out did not match here (arbiter is acting out of order): ", uvm::UVM_HIGH);
        std::cout << "buffer[" << i << "] = " << buffer[i].data[1].to_int() << "; packet_info_out[" << i << "] = " << packet_info_out[i][3] << '\n';
      }
    }
  }
  
  void check_phase(uvm::uvm_phase& phase)
  {
    check_indexes('L', index_l_in, index_l_out);
    check_indexes('N', index_n_in, index_n_out);
    check_indexes('E', index_e_in, index_e_out);
    check_indexes('S', index_s_in, index_s_out);
    check_indexes('W', index_w_in, index_w_out);
    
    // For debug purposes, if necessary.
    /*print_packet_info('l', 'I', packet_info_l_in);
    print_packet_info('n', 'I', packet_info_n_in);
    print_packet_info('e', 'I', packet_info_e_in);
    print_packet_info('s', 'I', packet_info_s_in);
    print_packet_info('w', 'I', packet_info_w_in);
    print_packet_info('l', 'O', packet_info_l_out);
    print_packet_info('n', 'O', packet_info_n_out);
    print_packet_info('e', 'O', packet_info_e_out);
    print_packet_info('s', 'O', packet_info_s_out);
    print_packet_info('w', 'O', packet_info_w_out);*/
    
    check_arbitration('L', packet_info_l_out);
    check_arbitration('N', packet_info_n_out);
    check_arbitration('E', packet_info_e_out);
    check_arbitration('S', packet_info_s_out);
    check_arbitration('W', packet_info_w_out);
    
    check_buffer_order('L', buffer_l, packet_info_l_out);
    check_buffer_order('N', buffer_n, packet_info_n_out);
    check_buffer_order('E', buffer_e, packet_info_e_out);
    check_buffer_order('S', buffer_s, packet_info_s_out);
    check_buffer_order('W', buffer_w, packet_info_w_out);
  }

};

#endif /* SCOREBOARD_H_ */
