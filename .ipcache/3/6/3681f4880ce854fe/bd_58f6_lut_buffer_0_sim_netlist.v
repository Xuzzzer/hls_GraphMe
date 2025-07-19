// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2023.2 (lin64) Build 4029153 Fri Oct 13 20:13:54 MDT 2023
// Date        : Wed Jun 18 21:34:40 2025
// Host        : fpga06 running 64-bit Ubuntu 22.04.5 LTS
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_58f6_lut_buffer_0_sim_netlist.v
// Design      : bd_58f6_lut_buffer_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xcu55c-fsvh2892-2L-e
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "bd_58f6_lut_buffer_0,lut_buffer_v2_0_0_lut_buffer,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* X_CORE_INFO = "lut_buffer_v2_0_0_lut_buffer,Vivado 2023.2" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (tdi_i,
    tms_i,
    tck_i,
    drck_i,
    sel_i,
    shift_i,
    update_i,
    capture_i,
    runtest_i,
    reset_i,
    bscanid_en_i,
    tdo_o,
    tdi_o,
    tms_o,
    tck_o,
    drck_o,
    sel_o,
    shift_o,
    update_o,
    capture_o,
    runtest_o,
    reset_o,
    bscanid_en_o,
    tdo_i);
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan TDI" *) input tdi_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan TMS" *) input tms_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan TCK" *) input tck_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan DRCK" *) input drck_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan SEL" *) input sel_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan SHIFT" *) input shift_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan UPDATE" *) input update_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan CAPTURE" *) input capture_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan RUNTEST" *) input runtest_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan RESET" *) input reset_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan BSCANID_EN" *) input bscanid_en_i;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 s_bscan TDO" *) output tdo_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan TDI" *) output tdi_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan TMS" *) output tms_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan TCK" *) output tck_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan DRCK" *) output drck_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan SEL" *) output sel_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan SHIFT" *) output shift_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan UPDATE" *) output update_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan CAPTURE" *) output capture_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan RUNTEST" *) output runtest_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan RESET" *) output reset_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan BSCANID_EN" *) output bscanid_en_o;
  (* X_INTERFACE_INFO = "xilinx.com:interface:bscan:1.0 m_bscan TDO" *) input tdo_i;

  wire bscanid_en_i;
  wire bscanid_en_o;
  wire capture_i;
  wire capture_o;
  wire drck_i;
  wire drck_o;
  wire reset_i;
  wire reset_o;
  wire runtest_i;
  wire runtest_o;
  wire sel_i;
  wire sel_o;
  wire shift_i;
  wire shift_o;
  wire tck_i;
  wire tck_o;
  wire tdi_i;
  wire tdi_o;
  wire tdo_i;
  wire tdo_o;
  wire tms_i;
  wire tms_o;
  wire update_i;
  wire update_o;
  wire [31:0]NLW_inst_bscanid_o_UNCONNECTED;

  (* C_EN_BSCANID_VEC = "0" *) 
  (* DONT_TOUCH *) 
  (* KEEP_HIERARCHY = "soft" *) 
  (* is_du_within_envelope = "true" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_lut_buffer_v2_0_0_lut_buffer inst
       (.bscanid_en_i(bscanid_en_i),
        .bscanid_en_o(bscanid_en_o),
        .bscanid_i({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .bscanid_o(NLW_inst_bscanid_o_UNCONNECTED[31:0]),
        .capture_i(capture_i),
        .capture_o(capture_o),
        .drck_i(drck_i),
        .drck_o(drck_o),
        .reset_i(reset_i),
        .reset_o(reset_o),
        .runtest_i(runtest_i),
        .runtest_o(runtest_o),
        .sel_i(sel_i),
        .sel_o(sel_o),
        .shift_i(shift_i),
        .shift_o(shift_o),
        .tck_i(tck_i),
        .tck_o(tck_o),
        .tdi_i(tdi_i),
        .tdi_o(tdi_o),
        .tdo_i(tdo_i),
        .tdo_o(tdo_o),
        .tms_i(tms_i),
        .tms_o(tms_o),
        .update_i(update_i),
        .update_o(update_o));
endmodule
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2023.2"
`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
Y0xh9134RsBng5etaGXwBVUEA8J2bWMgUTQDTIAitarHeeOiZsCqvqnqSMBLJzSAT8Clo2xSEdDb
f2T0bmuallNuAd4vpVjfpqZxOdLmUV+3X8aXUTr4KkIVjWQBC+K45w+OfnfdElcaUW1WQbp7AvaT
05bjWBH/BdJKBk0Kz8k=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
PE7RpcxN5xLwFXFfGeLCerEjPkCaTxfWD0b+oprDoEGqctY+F7Wc4+NqdSUAG4JbLWR/Pc8Mvten
+K5WbuDTljggJrkieJAK3rBOK8BdubtGJNC4uQ5v2trZYixfg4cWld5Z1MOB9aGfI0nF082l9Fc1
oNJFrkGcenyozvDKcCrtwvJaYRweulCV8/ynKznNpn+AvYhnoluR06IxzZXoj14b7IZt4g/2m455
clbUPyf1qLHbJGSK/Rvl0/W7cB5xxs3pM9/5p3UZ1MPFflZOAeCDwlOgzpXGAzfCkl9cSVqnIFFm
Q3cDkAfbRmP4jbDuwO2EbXgpiNcvc0br8gVcbA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
V73a/OJRlITsAnnyOJ7wtYI9yf/7gLrmQAWi1efFadTpN18wAOW5wcGb51JuXb/TOad9XvQaZxoY
I1ZZckK9R3kp0xHb3eRHqTEs38gIdB9DieJsPfcgrAgAh7N3AeXDhRXODyfLCVtmoF6cv0lmGI3F
6gKImN8nvzJnJHdYS0w=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
DCcPCQlu9uE6EBAoASplmWq6o/1vaTK24YYikkUnuM7wUc+K6wrTEbkFivj5OqxF/zGGynv+kjob
I28B+W/69av/irvgfSaOkl5CUwgmAnYrJQd5zO7pvvK7YBJ/f93xJ/FmpQTogAblevs7NdJLp3g2
OjMs/8iwMyXJYb2YgHoEFYKN8iqqLfoE1FTy3G1JWKcwGAYvCl6xaaPp+oYT4c7L99IYhk6R8LBP
5s2r5TkwtZsEUda1DuYu5UkWe5K0DyTVUxajXsz/s/xuKDYMlzV72Q6wQBSnBiknt/jnVyDo1tW+
PGq16LOUpjH7iA8esxtuBNSsdeu0hMvHXuCk0Q==

`pragma protect key_keyowner="Real Intent", key_keyname="RI-RSA-KEY-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
nwGINoKaVugQKTFyoLZc9O4TFbtk1/YjUn+a0zrC86S6J/PE/JSOh+4Yfyc8eQ/WKAw+4uNuqCfu
xMoblhyJX/PlyEcylGam6sg3YG3KUGmi+YfHMkiy5TIq7RfDEHBwm0OHajhuCJo+X/6WxDGrk+PS
JQwDW5IPxNMsoAYvcfcnzoAzaBMZ2IOHG/dpyyZ9tggqfcMqg7t2BLQujqkegYCW4gY5rCPGeljI
4AGn2WPvX+9JN5GQNIRdoRh8Onlhi39C7f6rkpR/zl9AY5kpcq9JW6q9bZNUXWBSnGm4Qz7GqfRv
VWcK4i0ng1C6xuiSkLkN+3y6/j1T30YlwsMfwA==

`pragma protect key_keyowner="Metrics Technologies Inc.", key_keyname="DSim", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
hEsll6mGIaBLE7AxJMVNtolpxRIRkhMsR1xHRjohvcUv4jFzKCjUO8d3wAnCq+Pz8f91PWDGP2f8
vGvDzErKDDVww33sDh+czaUMpdCCZkXt02jx1NfASHgkhqZt48L0UuNktllZas8HQy1w28ioRL/7
3KzyZN4uQ0f8w82zbdxC6U1l1meuVs6Ymk3Nsfmr36ARxpZj/9mbYwpjCUYyUzvUJRzuQfrAg0Ug
NoZDuxYRFYh7nfwlzgujXfMnemYGWwvjSq+iGvBWkCedSDGyNW+1BepcrFfzMd0eKQwmcj8h07J6
R97hYRxcHh/xlYrOs7brn6ldE9gjFrrjzoo8iw==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2022_10", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
LCn+A4p440uf9LYQwYUsNjUDkTOYDGJVSfBa72VZuxFlEtdCBsGMjyJlD71i9wfT+zo8h+uKo5qg
vdv6mNq8TlFLiiopLnEQiAavSCyjdKaqzw8udtBKGsJVh0jvWBiBGYR3s//q93WXtDm9YvhHTdgy
QyzPYSyta3qQBDVoFvr9QDfszU/AgD7tMB34LAvkpr+FTkjoCCJrveOtK2B2WHLDkSZUKkPVQ4z2
NkNE0C0TKTL07EoIHcBpTszfP8fVP255K0UDLBoKbNkya3Q/UqjG6bR9pNXI4n94ocrJxFUkJyc7
WjwNxUjgC0HL2CND2aA9LS0fnSdpZ/JZhvib0Q==

`pragma protect key_keyowner="Atrenta", key_keyname="ATR-SG-RSA-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=384)
`pragma protect key_block
eQmhkruRac3U1ERXu1Se13kEfZn5KP07/7J3pWhxX9QDO3A6aHkXHeiaH+qtRq5D2WFtbDCyVd5o
yPb7cQ9Sf2K6uFN+DTniB0oRADGePTdy2g3FHV68hvgIVlFrc9uf2rfs2yWR6pds+LDyYHhnSlEJ
hayggxgMxA41kth5hR2kGSybXpDjpQylauMvP+MetSb/27syf7QWVbXFhfLQE64opyObtme8TiWj
5MsRktemiPbC/x2RmB+ZklSRAvWu35tDO9u3XZ/Kk6WG2Pqj607cIfA/TsqoxZ6522ktimOnveJA
pSE6WUmDIQaOZ6pXjVIv4GEXZX73ZA3wLhRrmL7QxfXqCAeqFjouROHvM8Zv9kOfFGuDEo44JQSg
frNFO+XzslBTB/aplI0UWXL5ak9uQ1BUPtBO7xedGF/B7KKNsOoTml/48MYOaM6/3vjqSeiVUbvK
VVOQg7nBLowDBuFhcmn2RlKHia3ldFgRHkvFeuIJs5w1Ca1q0zuPaDzZ

`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="CDS_RSA_KEY_VER_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
gmsDEPC6u2G9YmnQc+V5rl/+mw4IztZGOLCaixhGfvI1t16GeLgoWqRktnjvSlEQbNzx7qnrDu3r
je4CfK2ZF54ZUMDX1QZd4bL54eK/AB3GiU3wZKUDtd9ZB5j4Oq5zWBU/nuHisg8FDEXkIndNgak5
cWycB82LoueWPC88cLKbbFasGsKFV3+Cn/sQ88RAmKtwlL2bwHvI+udevI4dY8w7//0nMejfXbdJ
QRGDs0h2SxkGs298aaM464WfmgQ63xcJn9AB5LuagH+a7BdhySu3RNfRzgSw5k3AYXJd3Q8Mht1s
ztXL0X+/yquUTzhcMBTfyicvWkiYRw6CxzjnDA==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 2960)
`pragma protect data_block
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
`pragma protect end_protected
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;
    parameter GRES_WIDTH = 10000;
    parameter GRES_START = 10000;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    wire GRESTORE;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;
    reg GRESTORE_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;
    assign (strong1, weak0) GRESTORE = GRESTORE_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

    initial begin 
	GRESTORE_int = 1'b0;
	#(GRES_START);
	GRESTORE_int = 1'b1;
	#(GRES_WIDTH);
	GRESTORE_int = 1'b0;
    end

endmodule
`endif
