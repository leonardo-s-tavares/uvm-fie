#include <systemc.h>
#include "../channel_in/channel_in.h"
#include "../channel_out/channel_out.h"
#include "../crossbar/crossbar.h"


template<int data_width_p>
struct router: sc_module{

    int pos_x;
    int pos_y;

    void set_position(int x, int y){
        this->pos_x = x;
        this->pos_y = y;
    }

    sc_in_clk clk_i;
    sc_in<sc_bv<1>> rst_i;
    sc_in<sc_uint<32>> x_id_p, y_id_p;
    sc_signal<sc_uint<32>> x_id_p_fault_target_from_in, y_id_p_fault_target_from_in; // Created by the Converter tool to get and send faults to an sc_in.

    sc_in<sc_bv<data_width_p>> l_in_data_i;
    sc_in<sc_bv<1>> l_in_val_i;
    sc_out<sc_bv<1>> l_in_ack_o;
    sc_out<sc_bv<data_width_p>> l_out_data_o;
    sc_out<sc_bv<1>> l_out_val_o;
    sc_in<sc_bv<1>> l_out_ack_i;

    sc_in<sc_bv<data_width_p>> n_in_data_i;
    sc_in<sc_bv<1>> n_in_val_i;
    sc_out<sc_bv<1>> n_in_ack_o;
    sc_out<sc_bv<data_width_p>> n_out_data_o;
    sc_out<sc_bv<1>> n_out_val_o;
    sc_in<sc_bv<1>> n_out_ack_i;

    sc_in<sc_bv<data_width_p>> e_in_data_i;
    sc_in<sc_bv<1>> e_in_val_i;
    sc_out<sc_bv<1>> e_in_ack_o;
    sc_out<sc_bv<data_width_p>> e_out_data_o;
    sc_out<sc_bv<1>> e_out_val_o;
    sc_in<sc_bv<1>> e_out_ack_i;

    sc_in<sc_bv<data_width_p>> s_in_data_i;
    sc_in<sc_bv<1>> s_in_val_i;
    sc_out<sc_bv<1>> s_in_ack_o;
    sc_out<sc_bv<data_width_p>> s_out_data_o;
    sc_out<sc_bv<1>> s_out_val_o;
    sc_in<sc_bv<1>> s_out_ack_i;

    sc_in<sc_bv<data_width_p>> w_in_data_i;
    sc_in<sc_bv<1>> w_in_val_i;
    sc_out<sc_bv<1>> w_in_ack_o;
    sc_out<sc_bv<data_width_p>> w_out_data_o;
    sc_out<sc_bv<1>> w_out_val_o;
    sc_in<sc_bv<1>> w_out_ack_i;

    sc_signal<sc_bv<1>> openl, openn, opene, opens, openw;

    sc_signal<sc_bv<1>> l_req_n_xin_w, l_req_e_xin_w, l_req_s_xin_w, l_req_w_xin_w, l_gnt_n_xin_w, l_gnt_e_xin_w, l_gnt_s_xin_w, l_gnt_w_xin_w, l_req_n_xout_w, l_req_e_xout_w, l_req_s_xout_w, l_req_w_xout_w, l_gnt_n_xout_w, l_gnt_e_xout_w, l_gnt_s_xout_w, l_gnt_w_xout_w, l_rok_w, l_rd_w;
    sc_signal<sc_bv<4>> gnt_w_l_i, rd_w_l_i, gnt_w_l_o, rok_w_l_o, req_w_l_o;
    sc_bv<4> gnt_ww_l_i, rd_ww_l_i, gnt_ww_l_o, rok_ww_l_o, req_ww_l_o;
    sc_signal<sc_bv<1>> n_req_l_xin_w, n_req_e_xin_w, n_req_s_xin_w, n_req_w_xin_w, n_gnt_l_xin_w, n_gnt_e_xin_w, n_gnt_s_xin_w, n_gnt_w_xin_w, n_req_l_xout_w, n_req_e_xout_w, n_req_s_xout_w, n_req_w_xout_w, n_gnt_l_xout_w, n_gnt_e_xout_w, n_gnt_s_xout_w, n_gnt_w_xout_w, n_rok_w, n_rd_w;
    sc_signal<sc_bv<4>> gnt_w_n_i, rd_w_n_i, gnt_w_n_o, rok_w_n_o, req_w_n_o;
    sc_bv<4> gnt_ww_n_i, rd_ww_n_i, gnt_ww_n_o, rok_ww_n_o, req_ww_n_o;
    sc_signal<sc_bv<1>> e_req_l_xin_w, e_req_n_xin_w, e_req_s_xin_w, e_req_w_xin_w, e_gnt_l_xin_w, e_gnt_n_xin_w, e_gnt_s_xin_w, e_gnt_w_xin_w, e_req_l_xout_w, e_req_n_xout_w, e_req_s_xout_w, e_req_w_xout_w, e_gnt_l_xout_w, e_gnt_n_xout_w, e_gnt_s_xout_w, e_gnt_w_xout_w, e_rok_w, e_rd_w;
    sc_signal<sc_bv<4>> gnt_w_e_i, rd_w_e_i, gnt_w_e_o, rok_w_e_o, req_w_e_o;
    sc_bv<4> gnt_ww_e_i, rd_ww_e_i, gnt_ww_e_o, rok_ww_e_o, req_ww_e_o;
    sc_signal<sc_bv<1>> s_req_l_xin_w, s_req_n_xin_w, s_req_e_xin_w, s_req_w_xin_w, s_gnt_l_xin_w, s_gnt_n_xin_w, s_gnt_e_xin_w, s_gnt_w_xin_w, s_req_l_xout_w, s_req_n_xout_w, s_req_e_xout_w, s_req_w_xout_w, s_gnt_l_xout_w, s_gnt_n_xout_w, s_gnt_e_xout_w, s_gnt_w_xout_w, s_rok_w, s_rd_w;
    sc_signal<sc_bv<4>> gnt_w_s_i, rd_w_s_i, gnt_w_s_o, rok_w_s_o, req_w_s_o;
    sc_bv<4> gnt_ww_s_i, rd_ww_s_i, gnt_ww_s_o, rok_ww_s_o, req_ww_s_o;
    sc_signal<sc_bv<1>> w_req_l_xin_w, w_req_n_xin_w, w_req_e_xin_w, w_req_s_xin_w, w_gnt_l_xin_w, w_gnt_n_xin_w, w_gnt_e_xin_w, w_gnt_s_xin_w, w_req_l_xout_w, w_req_n_xout_w, w_req_e_xout_w, w_req_s_xout_w, w_gnt_l_xout_w, w_gnt_n_xout_w, w_gnt_e_xout_w, w_gnt_s_xout_w, w_rok_w, w_rd_w;
    sc_signal<sc_bv<4>> gnt_w_w_i, rd_w_w_i, gnt_w_w_o, rok_w_w_o, req_w_w_o;
    sc_bv<4> gnt_ww_w_i, rd_ww_w_i, gnt_ww_w_o, rok_ww_w_o, req_ww_w_o;

    sc_signal<sc_bv<data_width_p>> l_data_w, n_data_w, e_data_w, s_data_w, w_data_w;

    channel_in<data_width_p, 6> *lin0;
    channel_out<data_width_p> *lout0;
    channel_in<data_width_p, 6> *nin0;
    channel_out<data_width_p> *nout0;
    channel_in<data_width_p, 6> *ein0;
    channel_out<data_width_p> *eout0;
    channel_in<data_width_p, 6> *sin0;
    channel_out<data_width_p> *sout0;
    channel_in<data_width_p, 6> *win0;
    channel_out<data_width_p> *wout0;


    crossbar *crossbar0;

    void connections(){
        x_id_p_fault_target_from_in.write(x_id_p.read()); // Created by the Converter tool to get and send faults to an sc_in.
        y_id_p_fault_target_from_in.write(y_id_p.read()); // Created by the Converter tool to get and send faults to an sc_in.
        //entradas
        
        gnt_ww_l_i[3] = n_gnt_l_xin_w.read().get_bit(0);
        gnt_ww_l_i[2] = e_gnt_l_xin_w.read().get_bit(0);
        gnt_ww_l_i[1] = s_gnt_l_xin_w.read().get_bit(0);
        gnt_ww_l_i[0] = w_gnt_l_xin_w.read().get_bit(0);
        gnt_w_l_i.write(gnt_ww_l_i);

        rd_ww_l_i[3] = n_rd_w.read().get_bit(0);
        rd_ww_l_i[2] = e_rd_w.read().get_bit(0);
        rd_ww_l_i[1] = s_rd_w.read().get_bit(0);
        rd_ww_l_i[0] = w_rd_w.read().get_bit(0);
        rd_w_l_i.write(rd_ww_l_i);

        gnt_ww_n_i[3] = l_gnt_n_xin_w.read().get_bit(0);
        gnt_ww_n_i[2] = e_gnt_n_xin_w.read().get_bit(0);
        gnt_ww_n_i[1] = s_gnt_n_xin_w.read().get_bit(0);
        gnt_ww_n_i[0] = w_gnt_n_xin_w.read().get_bit(0);
        gnt_w_n_i.write(gnt_ww_n_i);

        rd_ww_n_i[3] = l_rd_w.read().get_bit(0);
        rd_ww_n_i[2] = e_rd_w.read().get_bit(0);
        rd_ww_n_i[1] = s_rd_w.read().get_bit(0);
        rd_ww_n_i[0] = w_rd_w.read().get_bit(0);
        rd_w_n_i.write(rd_ww_n_i);

        gnt_ww_e_i[3] = l_gnt_e_xin_w.read().get_bit(0);
        gnt_ww_e_i[2] = n_gnt_e_xin_w.read().get_bit(0);
        gnt_ww_e_i[1] = s_gnt_e_xin_w.read().get_bit(0);
        gnt_ww_e_i[0] = w_gnt_e_xin_w.read().get_bit(0);
        gnt_w_e_i.write(gnt_ww_e_i);

        rd_ww_e_i[3] = l_rd_w.read().get_bit(0);
        rd_ww_e_i[2] = n_rd_w.read().get_bit(0);
        rd_ww_e_i[1] = s_rd_w.read().get_bit(0);
        rd_ww_e_i[0] = w_rd_w.read().get_bit(0);
        rd_w_e_i.write(rd_ww_e_i);

        gnt_ww_s_i[3] = l_gnt_s_xin_w.read().get_bit(0);
        gnt_ww_s_i[2] = n_gnt_s_xin_w.read().get_bit(0);
        gnt_ww_s_i[1] = e_gnt_s_xin_w.read().get_bit(0);
        gnt_ww_s_i[0] = w_gnt_s_xin_w.read().get_bit(0);
        gnt_w_s_i.write(gnt_ww_s_i);

        rd_ww_s_i[3] = l_rd_w.read().get_bit(0);
        rd_ww_s_i[2] = n_rd_w.read().get_bit(0);
        rd_ww_s_i[1] = e_rd_w.read().get_bit(0);
        rd_ww_s_i[0] = w_rd_w.read().get_bit(0);
        rd_w_s_i.write(rd_ww_s_i);

        gnt_ww_w_i[3] = l_gnt_w_xin_w.read().get_bit(0);
        gnt_ww_w_i[2] = n_gnt_w_xin_w.read().get_bit(0);
        gnt_ww_w_i[1] = e_gnt_w_xin_w.read().get_bit(0);
        gnt_ww_w_i[0] = s_gnt_w_xin_w.read().get_bit(0);
        gnt_w_w_i.write(gnt_ww_w_i);

        rd_ww_w_i[3] = l_rd_w.read().get_bit(0);
        rd_ww_w_i[2] = n_rd_w.read().get_bit(0);
        rd_ww_w_i[1] = e_rd_w.read().get_bit(0);
        rd_ww_w_i[0] = s_rd_w.read().get_bit(0);
        rd_w_w_i.write(rd_ww_w_i);

        //saidas

        l_gnt_n_xout_w.write(gnt_w_l_o.read().get_bit(3));
        l_gnt_e_xout_w.write(gnt_w_l_o.read().get_bit(2));
        l_gnt_s_xout_w.write(gnt_w_l_o.read().get_bit(1));
        l_gnt_w_xout_w.write(gnt_w_l_o.read().get_bit(0));
        /*
        gnt_ww_l_o[3] = l_gnt_n_xout_w.read().get_bit(0);
        gnt_ww_l_o[2] = l_gnt_e_xout_w.read().get_bit(0);
        gnt_ww_l_o[1] = l_gnt_s_xout_w.read().get_bit(0);
        gnt_ww_l_o[0] = l_gnt_w_xout_w.read().get_bit(0);
        gnt_w_l_o.write(gnt_ww_l_o);
        */
        rok_ww_l_o[3] = n_rok_w.read().get_bit(0);
        rok_ww_l_o[2] = e_rok_w.read().get_bit(0);
        rok_ww_l_o[1] = s_rok_w.read().get_bit(0);
        rok_ww_l_o[0] = w_rok_w.read().get_bit(0);
        rok_w_l_o.write(rok_ww_l_o);

        req_ww_l_o[3] = n_req_l_xout_w.read().get_bit(0);
        req_ww_l_o[2] = e_req_l_xout_w.read().get_bit(0);
        req_ww_l_o[1] = s_req_l_xout_w.read().get_bit(0);
        req_ww_l_o[0] = w_req_l_xout_w.read().get_bit(0);
        req_w_l_o.write(req_ww_l_o);

        n_gnt_l_xout_w.write(gnt_w_n_o.read().get_bit(3));
        n_gnt_e_xout_w.write(gnt_w_n_o.read().get_bit(2));
        n_gnt_s_xout_w.write(gnt_w_n_o.read().get_bit(1));
        n_gnt_w_xout_w.write(gnt_w_n_o.read().get_bit(0));
        /*
        gnt_ww_n_o[3] = n_gnt_l_xout_w.read().get_bit(0);
        gnt_ww_n_o[2] = n_gnt_e_xout_w.read().get_bit(0);
        gnt_ww_n_o[1] = n_gnt_s_xout_w.read().get_bit(0);
        gnt_ww_n_o[0] = n_gnt_w_xout_w.read().get_bit(0);
        gnt_w_n_o.write(gnt_ww_n_o);
        */
        rok_ww_n_o[3] = l_rok_w.read().get_bit(0);
        rok_ww_n_o[2] = e_rok_w.read().get_bit(0);
        rok_ww_n_o[1] = s_rok_w.read().get_bit(0);
        rok_ww_n_o[0] = w_rok_w.read().get_bit(0);
        rok_w_n_o.write(rok_ww_n_o);

        req_ww_n_o[3] = l_req_n_xout_w.read().get_bit(0);
        req_ww_n_o[2] = e_req_n_xout_w.read().get_bit(0);
        req_ww_n_o[1] = s_req_n_xout_w.read().get_bit(0);
        req_ww_n_o[0] = w_req_n_xout_w.read().get_bit(0);
        req_w_n_o.write(req_ww_n_o);

        e_gnt_l_xout_w.write(gnt_w_e_o.read().get_bit(3));
        e_gnt_n_xout_w.write(gnt_w_e_o.read().get_bit(2));
        e_gnt_s_xout_w.write(gnt_w_e_o.read().get_bit(1));
        e_gnt_w_xout_w.write(gnt_w_e_o.read().get_bit(0));

        //gnt_ww_e_o[3] = e_gnt_l_xout_w.read().get_bit(0);
        //gnt_ww_e_o[2] = e_gnt_n_xout_w.read().get_bit(0);
        //gnt_ww_e_o[1] = e_gnt_s_xout_w.read().get_bit(0);
        //gnt_ww_e_o[0] = e_gnt_w_xout_w.read().get_bit(0);
        //gnt_w_e_o.write(gnt_ww_e_o);

        rok_ww_e_o[3] = l_rok_w.read().get_bit(0);
        rok_ww_e_o[2] = n_rok_w.read().get_bit(0);
        rok_ww_e_o[1] = s_rok_w.read().get_bit(0);
        rok_ww_e_o[0] = w_rok_w.read().get_bit(0);
        rok_w_e_o.write(rok_ww_e_o);

        req_ww_e_o[3] = l_req_e_xout_w.read().get_bit(0);
        req_ww_e_o[2] = n_req_e_xout_w.read().get_bit(0);
        req_ww_e_o[1] = s_req_e_xout_w.read().get_bit(0);
        req_ww_e_o[0] = w_req_e_xout_w.read().get_bit(0);
        req_w_e_o.write(req_ww_e_o);
        
        s_gnt_l_xout_w.write(gnt_w_s_o.read().get_bit(3));
        s_gnt_n_xout_w.write(gnt_w_s_o.read().get_bit(2));
        s_gnt_e_xout_w.write(gnt_w_s_o.read().get_bit(1));
        s_gnt_w_xout_w.write(gnt_w_s_o.read().get_bit(0));
        
        /*
        gnt_ww_s_o[3] = s_gnt_l_xout_w.read().get_bit(0);
        gnt_ww_s_o[2] = s_gnt_n_xout_w.read().get_bit(0);
        gnt_ww_s_o[1] = s_gnt_e_xout_w.read().get_bit(0);
        gnt_ww_s_o[0] = s_gnt_w_xout_w.read().get_bit(0);
        gnt_w_s_o.write(gnt_ww_e_o);
        */

        rok_ww_s_o[3] = l_rok_w.read().get_bit(0);
        rok_ww_s_o[2] = n_rok_w.read().get_bit(0);
        rok_ww_s_o[1] = e_rok_w.read().get_bit(0);
        rok_ww_s_o[0] = w_rok_w.read().get_bit(0);
        rok_w_s_o.write(rok_ww_s_o);

        req_ww_s_o[3] = l_req_s_xout_w.read().get_bit(0);
        req_ww_s_o[2] = n_req_s_xout_w.read().get_bit(0);
        req_ww_s_o[1] = e_req_s_xout_w.read().get_bit(0);
        req_ww_s_o[0] = w_req_s_xout_w.read().get_bit(0);
        req_w_s_o.write(req_ww_s_o);

        w_gnt_l_xout_w.write(gnt_w_w_o.read().get_bit(3));
        w_gnt_n_xout_w.write(gnt_w_w_o.read().get_bit(2));
        w_gnt_e_xout_w.write(gnt_w_w_o.read().get_bit(1));
        w_gnt_s_xout_w.write(gnt_w_w_o.read().get_bit(0));

        /*
        gnt_ww_w_o[3] = w_gnt_l_xout_w.read().get_bit(0);
        gnt_ww_w_o[2] = w_gnt_n_xout_w.read().get_bit(0);
        gnt_ww_w_o[1] = w_gnt_e_xout_w.read().get_bit(0);
        gnt_ww_w_o[0] = w_gnt_s_xout_w.read().get_bit(0);
        gnt_w_w_o.write(gnt_ww_w_o);
        */

        rok_ww_w_o[3] = l_rok_w.read().get_bit(0);
        rok_ww_w_o[2] = n_rok_w.read().get_bit(0);
        rok_ww_w_o[1] = e_rok_w.read().get_bit(0);
        rok_ww_w_o[0] = s_rok_w.read().get_bit(0);
        rok_w_w_o.write(rok_ww_w_o);

        req_ww_w_o[3] = l_req_w_xout_w.read().get_bit(0);
        req_ww_w_o[2] = n_req_w_xout_w.read().get_bit(0);
        req_ww_w_o[1] = e_req_w_xout_w.read().get_bit(0);
        req_ww_w_o[0] = s_req_w_xout_w.read().get_bit(0);
        req_w_w_o.write(req_ww_w_o);

    }



    SC_CTOR(router){
        SC_METHOD(connections);
        sensitive << clk_i << rst_i;
        lin0 = new channel_in<data_width_p, 6>("lxin");
        nin0 = new channel_in<data_width_p, 6>("nxin");
        ein0 = new channel_in<data_width_p, 6>("exin");
        sin0 = new channel_in<data_width_p, 6>("sxin");
        win0 = new channel_in<data_width_p, 6>("wxin");
        lout0 = new channel_out<data_width_p>("lxout");
        nout0 = new channel_out<data_width_p>("nxout");
        eout0 = new channel_out<data_width_p>("exout");
        sout0 = new channel_out<data_width_p>("sxout");
        wout0 = new channel_out<data_width_p>("wxout");
        crossbar0 = new crossbar("xbar");

        lin0->set_position(pos_x,pos_y);
        nin0->set_position(pos_x,pos_y);
        ein0->set_position(pos_x,pos_y);
        sin0->set_position(pos_x,pos_y);
        win0->set_position(pos_x,pos_y);

        lin0->x_id_p(x_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        lin0->y_id_p(y_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        lin0->clk_i(clk_i);
        lin0->rst_i(rst_i);
        lin0->data_i(l_in_data_i);
        lin0->val_i(l_in_val_i);
        lin0->ack_o(l_in_ack_o);
        lin0->req_l_o(openl);
        lin0->req_n_o(l_req_n_xin_w);
        lin0->req_e_o(l_req_e_xin_w);
        lin0->req_s_o(l_req_s_xin_w);
        lin0->req_w_o(l_req_w_xin_w);
        lin0->gnt_i(gnt_w_l_i);
        lin0->rok_o(l_rok_w);
        lin0->rd_i(rd_w_l_i);
        lin0->data_o(l_data_w);

        nin0->x_id_p(x_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        nin0->y_id_p(y_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        nin0->clk_i(clk_i);
        nin0->rst_i(rst_i);
        nin0->data_i(n_in_data_i);
        nin0->val_i(n_in_val_i);
        nin0->ack_o(n_in_ack_o);
        nin0->req_l_o(n_req_l_xin_w);
        nin0->req_n_o(openn);
        nin0->req_e_o(n_req_e_xin_w);
        nin0->req_s_o(n_req_s_xin_w);
        nin0->req_w_o(n_req_w_xin_w);
        nin0->gnt_i(gnt_w_n_i);
        nin0->rok_o(n_rok_w);
        nin0->rd_i(rd_w_n_i);
        nin0->data_o(n_data_w);

        ein0->x_id_p(x_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        ein0->y_id_p(y_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        ein0->clk_i(clk_i);
        ein0->rst_i(rst_i);
        ein0->data_i(e_in_data_i);
        ein0->val_i(e_in_val_i);
        ein0->ack_o(e_in_ack_o);
        ein0->req_l_o(e_req_l_xin_w);
        ein0->req_n_o(e_req_n_xin_w);
        ein0->req_e_o(opene);
        ein0->req_s_o(e_req_s_xin_w);
        ein0->req_w_o(e_req_w_xin_w);
        ein0->gnt_i(gnt_w_e_i);
        ein0->rok_o(e_rok_w);
        ein0->rd_i(rd_w_e_i);
        ein0->data_o(e_data_w);

        sin0->x_id_p(x_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        sin0->y_id_p(y_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        sin0->clk_i(clk_i);
        sin0->rst_i(rst_i);
        sin0->data_i(s_in_data_i);
        sin0->val_i(s_in_val_i);
        sin0->ack_o(s_in_ack_o);
        sin0->req_l_o(s_req_l_xin_w);
        sin0->req_n_o(s_req_n_xin_w);
        sin0->req_e_o(s_req_e_xin_w);
        sin0->req_s_o(opens);
        sin0->req_w_o(s_req_w_xin_w);
        sin0->gnt_i(gnt_w_s_i);
        sin0->rok_o(s_rok_w);
        sin0->rd_i(rd_w_s_i);
        sin0->data_o(s_data_w);

        win0->x_id_p(x_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        win0->y_id_p(y_id_p_fault_target_from_in); // Created by the Converter tool to get and send faults to an sc_in.
        win0->clk_i(clk_i);
        win0->rst_i(rst_i);
        win0->data_i(w_in_data_i);
        win0->val_i(w_in_val_i);
        win0->ack_o(w_in_ack_o);
        win0->req_l_o(w_req_l_xin_w);
        win0->req_n_o(w_req_n_xin_w);
        win0->req_e_o(w_req_e_xin_w);
        win0->req_s_o(w_req_s_xin_w);
        win0->req_w_o(openw);
        win0->gnt_i(gnt_w_w_i);
        win0->rok_o(w_rok_w);
        win0->rd_i(rd_w_w_i);
        win0->data_o(w_data_w);

        lout0->clk_i(clk_i);
        lout0->rst_i(rst_i);
        lout0->data_o(l_out_data_o);
        lout0->val_o(l_out_val_o);
        lout0->ack_i(l_out_ack_i);
        lout0->req_i(req_w_l_o);
        lout0->gnt_o(gnt_w_l_o);
        lout0->rok_i(rok_w_l_o);
        lout0->rd_o(l_rd_w);
        lout0->data3_i(n_data_w);
        lout0->data2_i(e_data_w);
        lout0->data1_i(s_data_w);
        lout0->data0_i(w_data_w);

        nout0->clk_i(clk_i);
        nout0->rst_i(rst_i);
        nout0->data_o(n_out_data_o);
        nout0->val_o(n_out_val_o);
        nout0->ack_i(n_out_ack_i);
        nout0->req_i(req_w_n_o);
        nout0->gnt_o(gnt_w_n_o);
        nout0->rok_i(rok_w_n_o);
        nout0->rd_o(n_rd_w);
        nout0->data3_i(l_data_w);
        nout0->data2_i(e_data_w);
        nout0->data1_i(s_data_w);
        nout0->data0_i(w_data_w);

        eout0->clk_i(clk_i);
        eout0->rst_i(rst_i);
        eout0->data_o(e_out_data_o);
        eout0->val_o(e_out_val_o);
        eout0->ack_i(e_out_ack_i);
        eout0->req_i(req_w_e_o);
        eout0->gnt_o(gnt_w_e_o);
        eout0->rok_i(rok_w_e_o);
        eout0->rd_o(e_rd_w);
        eout0->data3_i(l_data_w);
        eout0->data2_i(n_data_w);
        eout0->data1_i(s_data_w);
        eout0->data0_i(w_data_w);

        sout0->clk_i(clk_i);
        sout0->rst_i(rst_i);
        sout0->data_o(s_out_data_o);
        sout0->val_o(s_out_val_o);
        sout0->ack_i(s_out_ack_i);
        sout0->req_i(req_w_s_o);
        sout0->gnt_o(gnt_w_s_o);
        sout0->rok_i(rok_w_s_o);
        sout0->rd_o(s_rd_w);
        sout0->data3_i(l_data_w);
        sout0->data2_i(n_data_w);
        sout0->data1_i(e_data_w);
        sout0->data0_i(w_data_w);

        wout0->clk_i(clk_i);
        wout0->rst_i(rst_i);
        wout0->data_o(w_out_data_o);
        wout0->val_o(w_out_val_o);
        wout0->ack_i(w_out_ack_i);
        wout0->req_i(req_w_w_o);
        wout0->gnt_o(gnt_w_w_o);
        wout0->rok_i(rok_w_w_o);
        wout0->rd_o(w_rd_w);
        wout0->data3_i(l_data_w);
        wout0->data2_i(n_data_w);
        wout0->data1_i(e_data_w);
        wout0->data0_i(s_data_w);

        crossbar0->l_req_n_i(l_req_n_xin_w);
        crossbar0->l_req_e_i(l_req_e_xin_w);
        crossbar0->l_req_s_i(l_req_s_xin_w);
        crossbar0->l_req_w_i(l_req_w_xin_w);
        crossbar0->l_req_n_o(l_req_n_xout_w);
        crossbar0->l_req_e_o(l_req_e_xout_w);
        crossbar0->l_req_s_o(l_req_s_xout_w);
        crossbar0->l_req_w_o(l_req_w_xout_w);
        crossbar0->n_req_l_i(n_req_l_xin_w);
        crossbar0->n_req_e_i(n_req_e_xin_w);
        crossbar0->n_req_s_i(n_req_s_xin_w);
        crossbar0->n_req_w_i(n_req_w_xin_w);
        crossbar0->n_req_l_o(n_req_l_xout_w);
        crossbar0->n_req_e_o(n_req_e_xout_w);
        crossbar0->n_req_s_o(n_req_s_xout_w);
        crossbar0->n_req_w_o(n_req_w_xout_w);
        crossbar0->e_req_l_i(e_req_l_xin_w);
        crossbar0->e_req_n_i(e_req_n_xin_w);
        crossbar0->e_req_s_i(e_req_s_xin_w);
        crossbar0->e_req_w_i(e_req_w_xin_w);
        crossbar0->e_req_l_o(e_req_l_xout_w);
        crossbar0->e_req_n_o(e_req_n_xout_w);
        crossbar0->e_req_s_o(e_req_s_xout_w);
        crossbar0->e_req_w_o(e_req_w_xout_w);
        crossbar0->s_req_l_i(s_req_l_xin_w);
        crossbar0->s_req_n_i(s_req_n_xin_w);
        crossbar0->s_req_e_i(s_req_e_xin_w);
        crossbar0->s_req_w_i(s_req_w_xin_w);
        crossbar0->s_req_l_o(s_req_l_xout_w);
        crossbar0->s_req_n_o(s_req_n_xout_w);
        crossbar0->s_req_e_o(s_req_e_xout_w);
        crossbar0->s_req_w_o(s_req_w_xout_w);
        crossbar0->w_req_l_i(w_req_l_xin_w);
        crossbar0->w_req_n_i(w_req_n_xin_w);
        crossbar0->w_req_e_i(w_req_e_xin_w);
        crossbar0->w_req_s_i(w_req_s_xin_w);
        crossbar0->w_req_l_o(w_req_l_xout_w);
        crossbar0->w_req_n_o(w_req_n_xout_w);
        crossbar0->w_req_e_o(w_req_e_xout_w);
        crossbar0->w_req_s_o(w_req_s_xout_w);
        crossbar0->l_gnt_n_i(l_gnt_n_xout_w);
        crossbar0->l_gnt_e_i(l_gnt_e_xout_w);
        crossbar0->l_gnt_s_i(l_gnt_s_xout_w);
        crossbar0->l_gnt_w_i(l_gnt_w_xout_w);
        crossbar0->l_gnt_n_o(l_gnt_n_xin_w);
        crossbar0->l_gnt_e_o(l_gnt_e_xin_w);
        crossbar0->l_gnt_s_o(l_gnt_s_xin_w);
        crossbar0->l_gnt_w_o(l_gnt_w_xin_w);
        crossbar0->n_gnt_l_i(n_gnt_l_xout_w);
        crossbar0->n_gnt_e_i(n_gnt_e_xout_w);
        crossbar0->n_gnt_s_i(n_gnt_s_xout_w);
        crossbar0->n_gnt_w_i(n_gnt_w_xout_w);
        crossbar0->n_gnt_l_o(n_gnt_l_xin_w);
        crossbar0->n_gnt_e_o(n_gnt_e_xin_w);
        crossbar0->n_gnt_s_o(n_gnt_s_xin_w);
        crossbar0->n_gnt_w_o(n_gnt_w_xin_w);
        crossbar0->e_gnt_l_i(e_gnt_l_xout_w);
        crossbar0->e_gnt_n_i(e_gnt_n_xout_w);
        crossbar0->e_gnt_s_i(e_gnt_s_xout_w);
        crossbar0->e_gnt_w_i(e_gnt_w_xout_w);
        crossbar0->e_gnt_l_o(e_gnt_l_xin_w);
        crossbar0->e_gnt_n_o(e_gnt_n_xin_w);
        crossbar0->e_gnt_s_o(e_gnt_s_xin_w);
        crossbar0->e_gnt_w_o(e_gnt_w_xin_w);
        crossbar0->s_gnt_l_i(s_gnt_l_xout_w);
        crossbar0->s_gnt_n_i(s_gnt_n_xout_w);
        crossbar0->s_gnt_e_i(s_gnt_e_xout_w);
        crossbar0->s_gnt_w_i(s_gnt_w_xout_w);
        crossbar0->s_gnt_l_o(s_gnt_l_xin_w);
        crossbar0->s_gnt_n_o(s_gnt_n_xin_w);
        crossbar0->s_gnt_e_o(s_gnt_e_xin_w);
        crossbar0->s_gnt_w_o(s_gnt_w_xin_w);
        crossbar0->w_gnt_l_i(w_gnt_l_xout_w);
        crossbar0->w_gnt_n_i(w_gnt_n_xout_w);
        crossbar0->w_gnt_e_i(w_gnt_e_xout_w);
        crossbar0->w_gnt_s_i(w_gnt_s_xout_w);
        crossbar0->w_gnt_l_o(w_gnt_l_xin_w);
        crossbar0->w_gnt_n_o(w_gnt_n_xin_w);
        crossbar0->w_gnt_e_o(w_gnt_e_xin_w);
        crossbar0->w_gnt_s_o(w_gnt_s_xin_w);
    }


};
