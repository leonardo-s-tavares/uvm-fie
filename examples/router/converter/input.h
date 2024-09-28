#include <systemc.h>

//enum ic_fsm {
//  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13
//};
#define s0 0 // Created by the Converter tool to get and send faults to an enum.
#define s1 1 // Created by the Converter tool to get and send faults to an enum.
#define s2 2 // Created by the Converter tool to get and send faults to an enum.
#define s3 3 // Created by the Converter tool to get and send faults to an enum.
#define s4 4 // Created by the Converter tool to get and send faults to an enum.
#define s5 5 // Created by the Converter tool to get and send faults to an enum.
#define s6 6 // Created by the Converter tool to get and send faults to an enum.
#define s7 7 // Created by the Converter tool to get and send faults to an enum.
#define s8 8 // Created by the Converter tool to get and send faults to an enum.
#define s9 9 // Created by the Converter tool to get and send faults to an enum.
#define s10 10 // Created by the Converter tool to get and send faults to an enum.
#define s11 11 // Created by the Converter tool to get and send faults to an enum.
#define s12 12 // Created by the Converter tool to get and send faults to an enum.
#define s13 13 // Created by the Converter tool to get and send faults to an enum.
  
template<int width_p>
struct routing: sc_module{

    int x_id_p;
    int y_id_p;

    void set_position(int x, int y){
        this->x_id_p = x;
        this->y_id_p = y;
    }

    sc_in_clk clk_i;
    sc_in<sc_uint<32>> x_id_p0, y_id_p0;
    sc_in<sc_bv<1>> rst_i;
    sc_in<sc_bv<1>> frame_i;
    sc_in<sc_bv<width_p-1>> data_i;
    sc_in<sc_bv<1>> rok_i;
    sc_in<sc_bv<1>> rd_i;
    sc_out<sc_bv<1>> req_l_o;
    sc_out<sc_bv<1>> req_n_o;
    sc_out<sc_bv<1>> req_e_o;
    sc_out<sc_bv<1>> req_s_o;
    sc_out<sc_bv<1>> req_w_o;

//    sc_signal<ic_fsm> state;
    sc_signal<sc_uint<32>> state;
    sc_uint<64> x_dst_v;
    sc_uint<64> y_dst_v;

    void logic(){     
        if(rst_i.read() != "1" && clk_i.posedge()){
//            switch (state)
            switch (state.read()) // Created by the Converter tool to get and send faults to an enum.
            {
            case s0:
                if(frame_i.read()=="1" and rok_i.read()=="1"){
                    y_dst_v = data_i.read().range(width_p/2-1, 0).to_uint();
                    x_dst_v = data_i.read().range(width_p-2, width_p/2).to_uint();
                    //cout << x_dst_v << "|" << y_dst_v << endl;
                    //cout << x_id_p0 << "|" << y_id_p0 << endl;
                    if(x_dst_v != x_id_p0){
                        if(x_dst_v > x_id_p0){
                            state.write(s1);
                        }else {
                            state.write(s2);
                        }
                        
                    }else if(y_dst_v != y_id_p0){

                        if(y_dst_v > y_id_p0){
                            state.write(s3);
                        }else{
                            state.write(s4);
                        }
                    } else {
                        state.write(s5);
                    }
                } else {
                    state.write(s0);
                }
                break;
            
            case s1:
                if(frame_i.read()=="0" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s9);
                } else {
                    state.write(s1);
                }
                break;
            case s9:
                if(frame_i.read()=="1" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s0);
                } else {
                    state.write(s9);
                }
                break;
            case s2:
                if(frame_i.read()=="0" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s10);
                } else {
                    state.write(s2);
                }
                break;
            case s10:
                if(frame_i.read()=="1" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s0);
                } else {
                    state.write(s10);
                }
                break;
            case s3:
                if(frame_i.read()=="0" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s11);
                } else {
                    state.write(s3);
                }
                break;
            case s11:
                if(frame_i.read()=="1" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s0);
                } else {
                    state.write(s11);
                }
                break;
            case s4:
                if(frame_i.read()=="0" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s12);
                } else {
                    state.write(s4);
                }
                break;
            case s12:
                if(frame_i.read()=="1" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s0);
                } else {
                    state.write(s12);
                }
                break;
            case s5:
                if(frame_i.read()=="0" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s13);
                } else {
                    state.write(s5);
                }
                break;
            case s13:
                if(frame_i.read()=="1" and rd_i.read()=="1" and rok_i.read()=="1"){
                    state.write(s0);
                } else {
                    state.write(s13);
                }
                break;
            default:
                state.write(s0);
                break;
            }
            
        }else if(rst_i.read() == "1"){
            state.write(s0);
        }
        if(state.read()==s5 or state.read()==s13){
                req_l_o.write("1");
            } else {
                req_l_o.write("0");
            }
            if(state.read()==s3 or state.read()==s11){
                req_n_o.write("1");
            } else {
                req_n_o.write("0");
            }
            if(state.read()==s1 or state.read()==s9){
                req_e_o.write("1");
            } else {
                req_e_o.write("0");
            }
            if(state.read()==s4 or state.read()==s12){
                req_s_o.write("1");
            } else {
                req_s_o.write("0");
            }
            if(state.read()==s2 or state.read()==s10){
                req_w_o.write("1");
            } else {
                req_w_o.write("0");
            }
    }
    SC_CTOR(routing){
        SC_METHOD(logic);
        sensitive << clk_i << rst_i << data_i << rok_i << rd_i << frame_i;
    }
};
