#include <systemc.h>
#include "../flow_out/flow_out.h"
#include "../switch/switch.h"
#include "../arbitration/arbitration.h"


template<int data_width_p>
struct channel_out: sc_module{

    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i;

    sc_out<sc_bv<data_width_p>> data_o;
    sc_out<sc_bv<1>> val_o;
    sc_in<sc_bv<1>> ack_i;

    sc_in<sc_bv<4>> req_i;
    sc_out<sc_bv<4>> gnt_o;

    sc_in<sc_bv<4>> rok_i;
    sc_out<sc_bv<1>> rd_o;
    sc_in<sc_bv<data_width_p>> data3_i, data2_i, data1_i, data0_i;

    sc_signal<sc_bv<1>> rok_w;
    sc_signal<sc_bv<4>> gnt_w;
    sc_signal<sc_bv<data_width_p>> data_w;

    sc_signal<sc_bv<1>> rok0_w, rok1_w, rok2_w, rok3_w;

    flow_out<data_width_p> *flow_out0;
    sc_switch<data_width_p+1> *data_switch;
    sc_switch<2> *rok_switch;
    arbitration *arbitration0;




    void logic(){
        gnt_o.write(gnt_w.read());
        rok0_w.write(rok_i.read().get_bit(0));
        rok1_w.write(rok_i.read().get_bit(1));
        rok2_w.write(rok_i.read().get_bit(2));
        rok3_w.write(rok_i.read().get_bit(3));
        //cout << "rok: " << rok_i.read() << endl << "gnt: " << gnt_w.read() << endl;
    }


    SC_CTOR(channel_out){
        SC_METHOD(logic);
        sensitive << clk_i << rst_i << ack_i << req_i << rok_i << data3_i << data2_i << data1_i << data0_i;

        flow_out0 = new flow_out<data_width_p>("flow_out");
        data_switch = new sc_switch<data_width_p+1>("data_switch");
        rok_switch = new sc_switch<2>("rok_switch");
        arbitration0 = new arbitration("arbitration");

        flow_out0->clk_i(clk_i);
        flow_out0->rst_i(rst_i);
        flow_out0->data_o(data_o);
        flow_out0->val_o(val_o);
        flow_out0->ack_i(ack_i);
        flow_out0->data_i(data_w);
        flow_out0->rok_i(rok_w);
        flow_out0->rd_o(rd_o);

        rok_switch->sel_i(gnt_w);
        rok_switch->data3_i(rok3_w);
        rok_switch->data2_i(rok2_w);
        rok_switch->data1_i(rok1_w);
        rok_switch->data0_i(rok0_w);
        rok_switch->data_o(rok_w);

        data_switch->sel_i(gnt_w);
        data_switch->data3_i(data3_i);
        data_switch->data2_i(data2_i);
        data_switch->data1_i(data1_i);
        data_switch->data0_i(data0_i);
        data_switch->data_o(data_w);

        arbitration0->clk_i(clk_i);
        arbitration0->rst_i(rst_i);
        arbitration0->req_i(req_i);
        arbitration0->gnt_o(gnt_w);

    }
};