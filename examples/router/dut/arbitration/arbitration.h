#include <systemc.h>

//enum oc_fsm {
//  oc_s0, oc_s1, oc_s2, oc_s3, oc_s4, oc_s5, oc_s6, oc_s7
//};
#define oc_s0 0 // Created by the Converter tool to get and send faults to an enum.
#define oc_s1 1 // Created by the Converter tool to get and send faults to an enum.
#define oc_s2 2 // Created by the Converter tool to get and send faults to an enum.
#define oc_s3 3 // Created by the Converter tool to get and send faults to an enum.
#define oc_s4 4 // Created by the Converter tool to get and send faults to an enum.
#define oc_s5 5 // Created by the Converter tool to get and send faults to an enum.
#define oc_s6 6 // Created by the Converter tool to get and send faults to an enum.
#define oc_s7 7 // Created by the Converter tool to get and send faults to an enum.

struct arbitration: sc_module{
    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i;
    sc_in<sc_bv<4>> req_i;
    sc_out<sc_bv<4>> gnt_o;
//    sc_signal<oc_fsm> state;
    sc_signal<sc_uint<32>> state;
    sc_signal<sc_bv<4>> gnt_w;

    void logic(){
        if(rst_i.read() != "1" && clk_i.posedge()){
//            switch (state)
            switch (state.read()) // Created by the Converter tool to get and send faults to an enum.
            {
            case oc_s0:
                if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                }else if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                }else if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                }else if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                } else {
                    state.write(oc_s0);
                }
                break;
            case oc_s1:
                if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                }else if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                }else if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                }else if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                } else {
                    state.write(oc_s2);
                }
                break;
            case oc_s2:
                if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                }else if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                }else if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                }else if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                } else {
                    state.write(oc_s2);
                }
                break;
            case oc_s3:
                if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                }else if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                }else if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                }else if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                } else {
                    state.write(oc_s4);
                }
                break;
            case oc_s4:
                if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                }else if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                }else if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                }else if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                } else {
                    state.write(oc_s4);
                }
                break;
            case oc_s5:
                if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                }else if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                }else if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                }else if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                } else {
                    state.write(oc_s6);
                }
                break;
            case oc_s6:
                if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                }else if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                }else if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                }else if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                } else {
                    state.write(oc_s6);
                }
                break;
            case oc_s7:
                if(req_i.read()[0]=="1"){
                    state.write(oc_s7);
                }else if(req_i.read()[3]=="1"){
                    state.write(oc_s1);
                }else if(req_i.read()[2]=="1"){
                    state.write(oc_s3);
                }else if(req_i.read()[1]=="1"){
                    state.write(oc_s5);
                } else {
                    state.write(oc_s0);
                }
                break;
            default:
                state.write(oc_s0);
                break;
            }
        }else if(rst_i.read() == "1"){
            state.write(oc_s0);
        }
        if(state.read()==oc_s1){
            gnt_w.write("1000");
        } else if(state.read()==oc_s3){
            gnt_w.write("0100");
        } else if(state.read()==oc_s5){
            gnt_w.write("0010");
        } else if(state.read()==oc_s7){
            gnt_w.write("0001");
        } else {
            gnt_w.write(0);
        }
        gnt_o.write((gnt_w.read() & req_i.read()));
    }
    
    SC_CTOR(arbitration){
        SC_METHOD(logic);
        sensitive << clk_i << rst_i << req_i;
    }
};
