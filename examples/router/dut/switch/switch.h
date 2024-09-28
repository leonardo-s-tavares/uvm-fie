#ifndef SWITCH
#define SWITCH

#include "systemc.h"

template<int data_width_p>
SC_MODULE(sc_switch){
    sc_in<sc_bv<4>> sel_i;
    sc_in<sc_bv<data_width_p-1>> data3_i, data2_i, data1_i, data0_i;
    sc_out<sc_bv<data_width_p-1>> data_o;
    sc_bv<data_width_p-1> sel3, sel2, sel1, sel0;

    void logic(){
        for(int i=0; i<data_width_p-1; i++){
            sel3[i] = sel_i.read().get_bit(3);
            sel2[i] = sel_i.read().get_bit(2);
            sel1[i] = sel_i.read().get_bit(1);
            sel0[i] = sel_i.read().get_bit(0);
        }

        //cout << sel3 << ":" << sel2 << ":" << sel1 << ":" << sel0 << endl;
        //cout << data3_i << ":" << data2_i << ":" << data1_i << ":" << data0_i << endl;

        data_o.write((data3_i.read() & sel3) | (data2_i.read() & sel2) | (data1_i.read() & sel1) | (data0_i.read() & sel0));
    }

    SC_CTOR(sc_switch){
        SC_METHOD(logic);
        sensitive << data3_i << data2_i << data1_i << data0_i << sel_i;
    }
};

#endif