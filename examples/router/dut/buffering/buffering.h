#include "systemc.h"
//#include "math.h"
//#include <cmath>


template <int WIDTH, int DEPTH>
struct buffering: sc_module{
    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i, rd_i, wr_i;

    sc_out<sc_bv<1>> rok_o, wok_o;

    sc_in<sc_bv<WIDTH>> data_i;
    sc_out<sc_bv<WIDTH>> data_o;
    
//    sc_signal<sc_bv<WIDTH>> fifo_r[DEPTH];
    sc_signal<sc_uint<WIDTH>> fifo_r[DEPTH]; // Created by the Converter tool to get and send faults to an sc_uint.
    sc_signal<sc_uint<32>> rd_ptr_r;

    void fifo_ring_logic(){
        if(rst_i.read()=="1"){
            rd_ptr_r.write(0);
        }else if(clk_i.posedge()){
            if(wr_i.read()=="1" and rd_ptr_r.read() != DEPTH){
                fifo_r[0].write(data_i.read());
                //cout << fifo_r[0].read() << "\t";
                for(int i=1; i<DEPTH; i++){
                    fifo_r[i].write(fifo_r[i-1].read());
                    //cout << fifo_r[i-1].read() << "\t";
                }
                //cout << rd_ptr_r.read() << "\t";
                //cout << "\n";
                if(!(rd_i.read()=="1" and rd_ptr_r.read() != 0)){
                    rd_ptr_r.write(rd_ptr_r.read()+1);
                }
            } else if(rd_i.read()=="1" and rd_ptr_r.read() != 0){
                rd_ptr_r.write(rd_ptr_r.read()-1);
            }
        }
        if(rd_ptr_r.read()!=0){
            rok_o.write("1");
        }else{
            rok_o.write("0");
        }
        if(rd_ptr_r.read()!=DEPTH){
            wok_o.write("1");
        }else{
            wok_o.write("0");
        }
        if(rd_ptr_r.read()==0){
            data_o.write(fifo_r[rd_ptr_r.read()].read());
        }else{
            data_o.write(fifo_r[rd_ptr_r.read()-1].read());
        }
    }


    SC_CTOR(buffering){
        SC_METHOD(fifo_ring_logic);
        sensitive << clk_i << rst_i << rd_i << wr_i << data_i;
    }
};
