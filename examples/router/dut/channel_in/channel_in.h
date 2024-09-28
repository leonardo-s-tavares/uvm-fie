#include <systemc.h>
#include "../flow_in/flow_in.h"
#include "../buffering/buffering.h"
#include "../switch/switch.h"
#include "../routing/routing.h"


template<int data_width_p, int deph_p>
struct channel_in: sc_module{

    int pos_x;
    int pos_y;

    void set_position(int x, int y){
        this->pos_x = x;
        this->pos_y = y;
    }
    
    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i;
    sc_in<sc_bv<data_width_p>> data_i;
    sc_in<sc_bv<1>> val_i;
    sc_out<sc_bv<1>> ack_o;

    sc_in<sc_uint<32>>  x_id_p, y_id_p;
    sc_signal<sc_uint<32>>  x_id_p_fault_target_from_in, y_id_p_fault_target_from_in; // Created by the Converter tool to get and send faults to an sc_in.

    sc_out<sc_bv<1>> req_l_o, req_n_o, req_e_o, req_s_o, req_w_o;
    sc_in<sc_bv<4>> gnt_i;

    sc_out<sc_bv<1>> rok_o;
    sc_in<sc_bv<4>> rd_i;
    sc_out<sc_bv<data_width_p>> data_o;

    sc_signal<sc_bv<1>> rok_w, rd_w, wok_w, wr_w;
    sc_signal<sc_bv<data_width_p>> data_in_w, data_out_w;
    sc_signal<sc_bv<1>> frame_w;
    sc_signal<sc_bv<data_width_p-1>> ic_data_w;

    sc_signal<sc_bv<1>> rd3_w, rd2_w, rd1_w, rd0_w, rdout_w;


    flow_in<data_width_p> *flow_in0;
    buffering<data_width_p, deph_p> *buffering0;
    sc_switch<2> *sc_switch0;
    routing<data_width_p> *routing0;

    void adjust(){
        x_id_p_fault_target_from_in.write(x_id_p.read()); // Created by the Converter tool to get and send faults to an sc_in.
        y_id_p_fault_target_from_in.write(y_id_p.read()); // Created by the Converter tool to get and send faults to an sc_in.
        frame_w.write(data_out_w.read().get_bit(data_width_p-1));
        ic_data_w.write(data_out_w.read().range(data_width_p-2, 0));
        rok_o.write(rok_w.read());
        data_o.write(data_out_w.read());
        rd3_w.write(rd_i.read().get_bit(3));
        rd2_w.write(rd_i.read().get_bit(2));
        rd1_w.write(rd_i.read().get_bit(1));
        rd0_w.write(rd_i.read().get_bit(0));
        //rd_w.write(rdout_w.read().get_bit(0));
    }



    SC_CTOR(channel_in){
        SC_METHOD(adjust);
        sensitive << clk_i << rst_i << data_i << val_i << gnt_i << rd_i;


        flow_in0 = new flow_in<data_width_p>("flow_in");
        buffering0 = new buffering<data_width_p, deph_p>("fifo");
        sc_switch0 = new sc_switch<2>("cs1");
        routing0 = new routing<data_width_p>("ic");

        routing0->set_position(pos_x,pos_y);

        flow_in0->clk_i(clk_i);
        flow_in0->rst_i(rst_i);
        flow_in0->data_i(data_i);
        flow_in0->val_i(val_i);
        flow_in0->ack_o(ack_o);
        flow_in0->data_o(data_in_w);
        flow_in0->wok_i(wok_w);
        flow_in0->wr_o(wr_w);

        buffering0->clk_i(clk_i);
        buffering0->rst_i(rst_i);
        buffering0->rok_o(rok_w);
        buffering0->rd_i(rd_w);
        buffering0->data_o(data_out_w);
        buffering0->wok_o(wok_w);
        buffering0->wr_i(wr_w);
        buffering0->data_i(data_in_w);

        sc_switch0->sel_i(gnt_i);
        sc_switch0->data0_i(rd0_w);
        sc_switch0->data1_i(rd1_w);
        sc_switch0->data2_i(rd2_w);
        sc_switch0->data3_i(rd3_w);
        sc_switch0->data_o(rd_w);

        routing0->x_id_p0(x_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        routing0->y_id_p0(y_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        routing0->clk_i(clk_i);
        routing0->rst_i(rst_i);
        routing0->frame_i(frame_w);
        routing0->data_i(ic_data_w);
        routing0->rok_i(rok_w);
        routing0->rd_i(rd_w);
        routing0->req_l_o(req_l_o);
        routing0->req_n_o(req_n_o);
        routing0->req_e_o(req_e_o);
        routing0->req_s_o(req_s_o);
        routing0->req_w_o(req_w_o);

    }
};
