#include "systemc.h"

//enum ifc_fsm {
//  ifcs0, ifcs1, ifcs2, ifcs3, ifcs4, ifcs5, ifcs6, ifcs7
//};
#define ifcs0 0 // Created by the Converter tool to get and send faults to an enum.
#define ifcs1 1 // Created by the Converter tool to get and send faults to an enum.
#define ifcs2 2 // Created by the Converter tool to get and send faults to an enum.
#define ifcs3 3 // Created by the Converter tool to get and send faults to an enum.
#define ifcs4 4 // Created by the Converter tool to get and send faults to an enum.
#define ifcs5 5 // Created by the Converter tool to get and send faults to an enum.
#define ifcs6 6 // Created by the Converter tool to get and send faults to an enum.
#define ifcs7 7 // Created by the Converter tool to get and send faults to an enum.

template <int data_width_p>
struct flow_in: sc_module{
    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i, val_i, wok_i;
    sc_out<sc_bv<1>> ack_o, wr_o;
    sc_in<sc_bv<data_width_p>> data_i;
    sc_out<sc_bv<data_width_p>> data_o;
//    sc_signal<ifc_fsm> state;
    sc_signal<sc_uint<32>> state;

    void flow_in_logic(){
        if(rst_i.read() != "1" && clk_i.posedge()){
//            switch(state){
            switch(state.read()){ // Created by the Converter tool to get and send faults to an enum.
                case ifcs0:
                wr_o.write("0");
                ack_o.write("0");
                if(val_i.read() == "1" and wok_i.read() == "1"){
                    state.write(ifcs1);
                } else {
                    state.write(ifcs0);
                }
                break;
                case ifcs1:
                ack_o.write("1");
                wr_o.write("1");
                if(val_i.read() == "0"){
                    state.write(ifcs0);
                } else {
                    state.write(ifcs2);
                }
                break;
                case ifcs2:
                ack_o.write("1");
                wr_o.write("0");
                if(val_i.read() == "0"){
                    state.write(ifcs0);
                } else {
                    state.write(ifcs2);
                } 
                break;
                default:
                    wr_o.write("0");
                    ack_o.write("0");
                    state.write(ifcs0);
                    break;

            }
        }
        data_o.write(data_i.read());
        /*
        if((state.read() == ifcs1) or (state.read() == ifcs2)){
            ack_o.write("1");
        }else{
            ack_o.write("0");
        }
        if((state.read() == ifcs1)){
            wr_o.write("1");
        }else{
            wr_o.write("0");
        } */
    }

    SC_CTOR(flow_in){
        SC_METHOD(flow_in_logic);
        sensitive << clk_i << rst_i << val_i << wok_i << data_i;
    }




};
