#include "systemc.h"

//enum ofc_fsm {
//  ofcs0, ofcs1, ofcs2
//};
#define 	ofcs0 0 // Created by the Converter tool to get and send faults to an enum.
#define ofcs1 1 // Created by the Converter tool to get and send faults to an enum.
#define ofcs2 2 // Created by the Converter tool to get and send faults to an enum.

template <int data_width_p>
struct flow_out: sc_module{
    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i, ack_i, rok_i;
    sc_out<sc_bv<1>> val_o, rd_o;
    sc_in<sc_bv<data_width_p>> data_i;
    sc_out<sc_bv<data_width_p>> data_o;
//    sc_signal<ofc_fsm> state;
    sc_signal<sc_uint<32>> state;

    void flow_out_logic(){
        if((rst_i.read() != "1") && (clk_i.posedge())){
//            switch(state){
            switch(state.read()){ // Created by the Converter tool to get and send faults to an enum.
                case ofcs0:
                    val_o.write("0");
                    rd_o.write("0");
                    if (ack_i.read()=="0" && rok_i.read()=="1"){
                        state.write(ofcs1);
                    } else {
                        state.write(ofcs0);
                    }
                    break;
                case ofcs1:
                    val_o.write("1");
                    rd_o.write("0");
                    if(ack_i.read()=="1"){
                        state.write(ofcs2);
                    }else{
                        state.write(ofcs1);
                    }
                    break;
                case ofcs2:
                    val_o.write("0");
                    rd_o.write("1");
                    if(ack_i.read()=="0" && rok_i.read()=="1"){
                        state.write(ofcs1);
                    }else {
                        state.write(ofcs0);
                    }
                    break;
                default:
                    val_o.write("0");
                    rd_o.write("0");
                    state.write(ofcs0);
                    break;
            }
            

        } else if(rst_i.read() == "1"){
            state.write(ofcs0);
        }
        data_o.write(data_i.read());
        /*
        if(state.read()==ofcs1){
            val_o.write("1");
        } else {
            val_o.write("0");
        }
        if(state.read()==ofcs2){
            rd_o.write("1");
        } else {
            rd_o.write("0");
        }  */
    }

    SC_CTOR(flow_out){
        SC_METHOD(flow_out_logic);
        sensitive << clk_i << rst_i << ack_i << rok_i << data_i;
    }




};
