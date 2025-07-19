// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2023.2 (lin64) Build 4029153 Fri Oct 13 20:13:54 MDT 2023
// Date        : Wed Jun 18 21:19:42 2025
// Host        : fpga06 running 64-bit Ubuntu 22.04.5 LTS
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_22c0_gapping_demand_toggle_0_sim_netlist.v
// Design      : bd_22c0_gapping_demand_toggle_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xcu55c-fsvh2892-2L-e
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "bd_22c0_gapping_demand_toggle_0,c_counter_binary_v12_0_17,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "c_counter_binary_v12_0_17,Vivado 2023.2" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (CLK,
    CE,
    Q);
  (* x_interface_info = "xilinx.com:signal:clock:1.0 clk_intf CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME clk_intf, ASSOCIATED_BUSIF q_intf:thresh0_intf:l_intf:load_intf:up_intf:sinit_intf:sset_intf, ASSOCIATED_RESET SCLR, ASSOCIATED_CLKEN CE, FREQ_HZ 50000000, FREQ_TOLERANCE_HZ 0, PHASE 0, CLK_DOMAIN cd_ctrl_00, INSERT_VIP 0" *) input CLK;
  (* x_interface_info = "xilinx.com:signal:clockenable:1.0 ce_intf CE" *) (* x_interface_parameter = "XIL_INTERFACENAME ce_intf, POLARITY ACTIVE_HIGH" *) input CE;
  (* x_interface_info = "xilinx.com:signal:data:1.0 q_intf DATA" *) (* x_interface_parameter = "XIL_INTERFACENAME q_intf, LAYERED_METADATA xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value data} bitwidth {attribs {resolve_type generated dependency bitwidth format long minimum {} maximum {}} value 1} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} DATA_WIDTH 1}" *) output [0:0]Q;

  wire CE;
  wire CLK;
  wire [0:0]Q;
  wire NLW_U0_THRESH0_UNCONNECTED;

  (* C_AINIT_VAL = "0" *) 
  (* C_CE_OVERRIDES_SYNC = "0" *) 
  (* C_COUNT_BY = "1" *) 
  (* C_COUNT_MODE = "0" *) 
  (* C_COUNT_TO = "1" *) 
  (* C_FB_LATENCY = "0" *) 
  (* C_HAS_CE = "1" *) 
  (* C_HAS_LOAD = "0" *) 
  (* C_HAS_SCLR = "0" *) 
  (* C_HAS_SINIT = "0" *) 
  (* C_HAS_SSET = "0" *) 
  (* C_HAS_THRESH0 = "0" *) 
  (* C_IMPLEMENTATION = "0" *) 
  (* C_LATENCY = "1" *) 
  (* C_LOAD_LOW = "0" *) 
  (* C_RESTRICT_COUNT = "0" *) 
  (* C_SCLR_OVERRIDES_SSET = "1" *) 
  (* C_SINIT_VAL = "0" *) 
  (* C_THRESH0_VALUE = "1" *) 
  (* C_VERBOSITY = "0" *) 
  (* C_WIDTH = "1" *) 
  (* C_XDEVICEFAMILY = "virtexuplusHBM" *) 
  (* downgradeipidentifiedwarnings = "yes" *) 
  (* is_du_within_envelope = "true" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_c_counter_binary_v12_0_17 U0
       (.CE(CE),
        .CLK(CLK),
        .L(1'b0),
        .LOAD(1'b0),
        .Q(Q),
        .SCLR(1'b0),
        .SINIT(1'b0),
        .SSET(1'b0),
        .THRESH0(NLW_U0_THRESH0_UNCONNECTED),
        .UP(1'b1));
endmodule
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2023.2"
`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
eFbqyWxvTxgrA/YtdaoI20/0Oxv6heWR3Rkp9/xOWnvLDdGDhtGJBQqdO4v1RO/kikveHE3JyVBx
OMXM/QBYbcn/QmEMFud4drsy8IbaUwVstP+Mzovw04CY0e6ucHcNG8bkdAhiixaw1DGilwl8tfXo
1/LD/FGivkVY+qD5JIE=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
qZVOEd9Suj8PFYlAHZ5eNfv9g67bFY/Iau3fGJHFAIz/4EbdSAUDaGh/Aj5F/sayLnlRNhD6w+39
N7ODCROvgCW/DEQMBCPz7kcEchwyjzrqkhJexEv0Dz7kFQn1ftmdbnZ6SxsSg0bAUSqDETfwIrDN
VELNGURpq3DjO751fQLkz152JThZlONrPm6SqH+2yq0k/imlDMyhznvq+Up4EXiczfO25/APInqH
9ImfZSrqCiz3p7BNa9t1DtJtjx4nO4g/3qItwAhtZOzSyNgUZUJkS0OgYwLaNbOAMte1lEZ3aCj/
PtYFcVrRv6BV9zObKm5JRWmYYw/qLDjrN9AsCA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
PTqB7iZsvJeVQbxSYRkkEOB7dur2/Y+zWd7rSI4QgTOZZuY7cx4mymLcNTtY69vWs3+Ir6xtLuRI
kV9wRh0KJKuphJal6eQJChHGu6rp+AHyp8AyhIwGgID1vxyyu7xhU5nl4qM40fYe+ov2uBp5DVP0
GoOHS6Gilji9DRkCQuI=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
nl92noyushAx6EnMgw3oSlb0lEtv202gnVMSNN83+NLaV5DJ/HimKQF470dvcnALDIl0xa3e3Dx2
/s2hBMgu9+fSioH4xbMFQTaBWMjBfDKLVgBkEfT5zBbn1LpjuMEnd/TVHxe/dqXJ8Ev1EIyVB5r3
7KAUvfDL8CretmawtvJtixs8bH8vAxLO4BUzVNbXDL44NeL/PffK31PA74odtZbSUGIq+Gf0nEXP
yEajhFawSXpK6M+iRpsuDwKHS/YxQldY5i8FGvVQrcrDBe3XAh+jjvxUqPhZBRChKpDSo0q7V9L4
JAZoQiGn28UrFoWwrxxP1gsv7sPdry3YTRu8DA==

`pragma protect key_keyowner="Real Intent", key_keyname="RI-RSA-KEY-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
KbhPmoTx5e7VcsW2JDC/J/bcPlnz66gRyRCtg0E0qHv9wqViijoH+owrxv0IWMQoDBsXBaeQm2xz
nHRZDc5s+B+JlzwwZQGB8pQM3sXMmxGcH+jeVqy6X8gKOEQFgnIK2FJlAjHpfO0xmJkl3wxWImNr
ADPNoWEMdruR5ksSgKexng6J3lkv4vPYoEvCF+Jq91pp71EIJgPtwlY833cs1Exi28xe2Qo/nzU7
oEFG5gySNEidQa25q1QrCDnSmj7j5wDJg5xzjXYmwWk9873dPWyEXdpFMqjxovIcyph/uXidS1PQ
XxFetrAMjtseoYWmz4Lm4f+rpe89PGRhWXsiZw==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2022_10", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
u7x25sjZWe79yzU52TdAK1EHhzoxhg0OOXYOwTp455Bu78gqkmKRv90VNHoa9foKyQc+Ui9ovV+f
Mu2Crcme1IbP51J6eQdKys/57qJrcFCxGtJs1Tw7KJ7NffFwkytoqR7pgvmtIH6+qncA8b3aZTLq
uwD9bGF9UFZVZ2XBc83+LRU+GZnNMHOa4eegWtueYHh1zUhGju1xbiGWuhliZ58pcNp6gCDiDv+p
GdiwFDT5RDj1bjrkOecRL2fvOdGLrhdqiTh7mvJeDStjjXiovaCdny21gVHf+dzrpyPE2xGgBinA
czj0D2pyt8plttBhpmMBtLm7Yegb1rHiA1UC4Q==

`pragma protect key_keyowner="Metrics Technologies Inc.", key_keyname="DSim", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
ax8Nn3y8qIY97tYqs/w6/65Cp2CG+WRyi20ND2JmdKrfZukanonYYzlNsDdtoOeMZdG6gzVR279V
Et2Qz8YBLQmhBZMJ13mNxEOwpSUbc5lUbLJ9CQ+4u6jvStTDzX+odxkCCqHG8GJhSSFPGX+Z3VZc
TdU/OWddzxwk6JO1tiPn+qt2Q8nMj3Ulh7gqAxPMp0gosh6z+Kx5ZXSuVE/EPNyUDXjRSXjnWPwN
NnM94gbzG23dPqFIOG6f2m5ugBmUUghvI75DFpM7vJkXsEWAfZeOV351MLISR26yMaWxONCdGWTQ
DW1hvUkse+kVt5BxF8ft6CnT7VchA/flFKvfPg==

`pragma protect key_keyowner="Atrenta", key_keyname="ATR-SG-RSA-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=384)
`pragma protect key_block
e/Ry1l/vqJFJqrtAPFKKjxxp0MWhiORDa7WOuKdNY7LaztvYcFdzfNmZDmSuTrj2jPF7WDL6uDC8
FMboEovqCHZ9s014f2WS+jYxvraqlkgyGQ1Q7gAZ6yaBsdSi2RgWdbpy++ECpfVjq8/sYcJl+CZl
ZIsOc7C4BTFh3wysjt9r3cLq/k/dOO8xw2ZAarjqRzjr0h9T3TEzowI/jWVIqkEICEpCsA6k6h2U
oRuVQxQmdtSXmU/Zm/g8I4dj6axYw/zYpYJe2v9s4sy3NCBf/p+z3JFoReqcGYtminQ5ba3zzR5v
mVNiXw+YtQVCe0IsGLqjaEBWcXrs5SNFVYKce0xzBupVSQ0hP5cTBMtcToem7n2nM+9LdhZqlPBl
KimcvX+KrlUbox8H49P1OzL0A8+Eterrfdy0jE2DF+YuIuSAnudKM11WtjqK5yq8zJk5JOHQUQZl
qO7dmmZT95FjGPG/jqS+uYlHdZCNNyQ2l2SbVZw7NPkIJGq5bAKeGsSg

`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="CDS_RSA_KEY_VER_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
OqTG2QUdaSjYwH6PeUiMov3bCDPXiUvE4U1Z/Vd7xbPVo7tNNwQeTcXKi/ZUR5G1tkEs1OoxIqaH
ytImL/6Ro6liEE5oT3otxdQb1Yz3ukWdnhQpgw3O2DRb5K1R4L7p5QCVIgq+/7f4oNn8VSJ1hT7o
jVMeT5IvgkJXZsqX/2c86iOEUQ/Xha6SVw6W50dS1u8Q/FzR54WBSo0MFMxZWh5Pyf5qnBJKna0d
nVdDHDBFDajINOk8j7Oonu6ynHrhNkyo1IEnNv/ZQPbHo2aynI/MIaO/5etmCQO2m+53okz5H7pu
RWYkE0zXoE1v6jl/QhZyc23SZppXzWGhd1zsRw==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 2336)
`pragma protect data_block
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
`pragma protect end_protected
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2023.2"
`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
eFbqyWxvTxgrA/YtdaoI20/0Oxv6heWR3Rkp9/xOWnvLDdGDhtGJBQqdO4v1RO/kikveHE3JyVBx
OMXM/QBYbcn/QmEMFud4drsy8IbaUwVstP+Mzovw04CY0e6ucHcNG8bkdAhiixaw1DGilwl8tfXo
1/LD/FGivkVY+qD5JIE=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
qZVOEd9Suj8PFYlAHZ5eNfv9g67bFY/Iau3fGJHFAIz/4EbdSAUDaGh/Aj5F/sayLnlRNhD6w+39
N7ODCROvgCW/DEQMBCPz7kcEchwyjzrqkhJexEv0Dz7kFQn1ftmdbnZ6SxsSg0bAUSqDETfwIrDN
VELNGURpq3DjO751fQLkz152JThZlONrPm6SqH+2yq0k/imlDMyhznvq+Up4EXiczfO25/APInqH
9ImfZSrqCiz3p7BNa9t1DtJtjx4nO4g/3qItwAhtZOzSyNgUZUJkS0OgYwLaNbOAMte1lEZ3aCj/
PtYFcVrRv6BV9zObKm5JRWmYYw/qLDjrN9AsCA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
PTqB7iZsvJeVQbxSYRkkEOB7dur2/Y+zWd7rSI4QgTOZZuY7cx4mymLcNTtY69vWs3+Ir6xtLuRI
kV9wRh0KJKuphJal6eQJChHGu6rp+AHyp8AyhIwGgID1vxyyu7xhU5nl4qM40fYe+ov2uBp5DVP0
GoOHS6Gilji9DRkCQuI=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
nl92noyushAx6EnMgw3oSlb0lEtv202gnVMSNN83+NLaV5DJ/HimKQF470dvcnALDIl0xa3e3Dx2
/s2hBMgu9+fSioH4xbMFQTaBWMjBfDKLVgBkEfT5zBbn1LpjuMEnd/TVHxe/dqXJ8Ev1EIyVB5r3
7KAUvfDL8CretmawtvJtixs8bH8vAxLO4BUzVNbXDL44NeL/PffK31PA74odtZbSUGIq+Gf0nEXP
yEajhFawSXpK6M+iRpsuDwKHS/YxQldY5i8FGvVQrcrDBe3XAh+jjvxUqPhZBRChKpDSo0q7V9L4
JAZoQiGn28UrFoWwrxxP1gsv7sPdry3YTRu8DA==

`pragma protect key_keyowner="Real Intent", key_keyname="RI-RSA-KEY-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
KbhPmoTx5e7VcsW2JDC/J/bcPlnz66gRyRCtg0E0qHv9wqViijoH+owrxv0IWMQoDBsXBaeQm2xz
nHRZDc5s+B+JlzwwZQGB8pQM3sXMmxGcH+jeVqy6X8gKOEQFgnIK2FJlAjHpfO0xmJkl3wxWImNr
ADPNoWEMdruR5ksSgKexng6J3lkv4vPYoEvCF+Jq91pp71EIJgPtwlY833cs1Exi28xe2Qo/nzU7
oEFG5gySNEidQa25q1QrCDnSmj7j5wDJg5xzjXYmwWk9873dPWyEXdpFMqjxovIcyph/uXidS1PQ
XxFetrAMjtseoYWmz4Lm4f+rpe89PGRhWXsiZw==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2022_10", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
u7x25sjZWe79yzU52TdAK1EHhzoxhg0OOXYOwTp455Bu78gqkmKRv90VNHoa9foKyQc+Ui9ovV+f
Mu2Crcme1IbP51J6eQdKys/57qJrcFCxGtJs1Tw7KJ7NffFwkytoqR7pgvmtIH6+qncA8b3aZTLq
uwD9bGF9UFZVZ2XBc83+LRU+GZnNMHOa4eegWtueYHh1zUhGju1xbiGWuhliZ58pcNp6gCDiDv+p
GdiwFDT5RDj1bjrkOecRL2fvOdGLrhdqiTh7mvJeDStjjXiovaCdny21gVHf+dzrpyPE2xGgBinA
czj0D2pyt8plttBhpmMBtLm7Yegb1rHiA1UC4Q==

`pragma protect key_keyowner="Metrics Technologies Inc.", key_keyname="DSim", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
ax8Nn3y8qIY97tYqs/w6/65Cp2CG+WRyi20ND2JmdKrfZukanonYYzlNsDdtoOeMZdG6gzVR279V
Et2Qz8YBLQmhBZMJ13mNxEOwpSUbc5lUbLJ9CQ+4u6jvStTDzX+odxkCCqHG8GJhSSFPGX+Z3VZc
TdU/OWddzxwk6JO1tiPn+qt2Q8nMj3Ulh7gqAxPMp0gosh6z+Kx5ZXSuVE/EPNyUDXjRSXjnWPwN
NnM94gbzG23dPqFIOG6f2m5ugBmUUghvI75DFpM7vJkXsEWAfZeOV351MLISR26yMaWxONCdGWTQ
DW1hvUkse+kVt5BxF8ft6CnT7VchA/flFKvfPg==

`pragma protect key_keyowner="Atrenta", key_keyname="ATR-SG-RSA-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=384)
`pragma protect key_block
e/Ry1l/vqJFJqrtAPFKKjxxp0MWhiORDa7WOuKdNY7LaztvYcFdzfNmZDmSuTrj2jPF7WDL6uDC8
FMboEovqCHZ9s014f2WS+jYxvraqlkgyGQ1Q7gAZ6yaBsdSi2RgWdbpy++ECpfVjq8/sYcJl+CZl
ZIsOc7C4BTFh3wysjt9r3cLq/k/dOO8xw2ZAarjqRzjr0h9T3TEzowI/jWVIqkEICEpCsA6k6h2U
oRuVQxQmdtSXmU/Zm/g8I4dj6axYw/zYpYJe2v9s4sy3NCBf/p+z3JFoReqcGYtminQ5ba3zzR5v
mVNiXw+YtQVCe0IsGLqjaEBWcXrs5SNFVYKce0xzBupVSQ0hP5cTBMtcToem7n2nM+9LdhZqlPBl
KimcvX+KrlUbox8H49P1OzL0A8+Eterrfdy0jE2DF+YuIuSAnudKM11WtjqK5yq8zJk5JOHQUQZl
qO7dmmZT95FjGPG/jqS+uYlHdZCNNyQ2l2SbVZw7NPkIJGq5bAKeGsSg

`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="CDS_RSA_KEY_VER_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
OqTG2QUdaSjYwH6PeUiMov3bCDPXiUvE4U1Z/Vd7xbPVo7tNNwQeTcXKi/ZUR5G1tkEs1OoxIqaH
ytImL/6Ro6liEE5oT3otxdQb1Yz3ukWdnhQpgw3O2DRb5K1R4L7p5QCVIgq+/7f4oNn8VSJ1hT7o
jVMeT5IvgkJXZsqX/2c86iOEUQ/Xha6SVw6W50dS1u8Q/FzR54WBSo0MFMxZWh5Pyf5qnBJKna0d
nVdDHDBFDajINOk8j7Oonu6ynHrhNkyo1IEnNv/ZQPbHo2aynI/MIaO/5etmCQO2m+53okz5H7pu
RWYkE0zXoE1v6jl/QhZyc23SZppXzWGhd1zsRw==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-PREC-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
Dj8Vyr3CViQ6cQWAQ6d5ZGN9wdgZqukn6FbW8laXbrzD2IONpOFe/MwqTYgytwsJvnZYVI4b722b
PTWF9NfE2vhzbeXnuSdPKkyhBHD/1QfLVPzl2vKDTHX88tH24a37W11MLIor0KG56r/rHTAi8/Bm
LAL+Dn/i9UHetzgIUGIUKau6rif1XNVLNbqeIksSpjgokp9WERsfJsOhzgVpvHHjGI5akrldHRW7
41B/LB60CcCrxoJRZFFdsO660L0zcvrxeCwlBtIykjrAunySzEzmenCGGK7qCxUtNI2wC0cn6/03
S2h8yFh3NDAF+SWKhBmZyqjmd7BZXKrUJcxohw==

`pragma protect key_keyowner="Synplicity", key_keyname="SYNP15_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
He+cC8ES+2aIJhzTYmgzLhZ+PUF7ihAE/O4XmaTnuIrFejQ7NZd971uG5aJ//MO5MnSu67I02d9a
rCx5tDKeClPJkXUKGZs66tnUyrX/Tgpsv3avcnXotnTK7zcwwaYGe7f6zswuBqw7hPUvfJK0wfHv
pOboRCVSV74RilaBClT3vWMG0RwJhxfMU+uGoluZ9IRH/dtolOgl+cym3CGlmUX+gGKsECuXNUb8
8kgJFNIktkMRBtxb8bQbgarYdXcQhmdybepvEyIQd2gSvu8AVk86fwRSfsyDoO7qaPeM3PhC+/A6
h7zWq9t2lcc89W2akCF6a0FWiPmxrHfpGEflfQ==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 2832)
`pragma protect data_block
Xu0CYeyGiuSc/r2mQtergSGJ+z/rjEaM3ixcnPGAbyEOhlvmjPsavtoii3y/m/SrFITfvbzoUqyh
mO/eOl1NCOfJZazlzbmydmjX/hYO2yoHn6DhWed8Kh+pY6LvNATm/O7gXdZnxRp1v57i7NDN2860
6iuQkXCIFf+GtxzT+Eao751hfDQmGcNOe7mT3TCkrHoTxDeL5fZIcNcHllNdPZjLRIkuKDzjb0g7
fZMRgL4bRPcmLXtwEGXQhbd4qzfKIZd2i6J7eBusLAEjq4VOVBHGROTn5NBbylIBFqiw4TMXz916
3Myv4CEaDRzUvSieMKAHLEAJttbwYxcznBLkg1p2fdBF/Xb51NKDvJdhPW0AGzddfRodk1mHpC/J
faYCU9Zds1PrRqaxP6Q9RR7RseN5nnNjlNGTZ0btP4g5ywkRwkrwqtXtyOhcE8Ja2IQvqJwtHXo/
EL+L1OGBDyp3whP8kueogzoBbsjik9Ms7cSNoRrR+etXBwVJ1pJHdEl50ZHhl4THBtgklj5413gZ
bz/siY24xm9A2LuZQP56jUnOFXMtU0tq/ZdMz2bOGqZ2oJVrG7x/KBwxPtNI6IuA18nlyMgeynYo
+wYvx+nfkIM8+SDXzNx9RtX1OuL+zOFNDSImHaMaxn+/T0LveSz20aeN2jGCm2Q3NSj4/D7/eIh0
7k26jX7LIqbWTi4FlJZmmT0BL3XJKcF/IkdnfoZ1kXyMa7SeJ2IJiur2fpYRlH+R0zavqBDLB3LI
gFeO3onVdUmww8ZKb/FnyPn1DSTQ5n6nLxsUANSTIJsALv1Sp5P4kfxxGp+31z+O2w/1HcfipAFw
DTiZK+2k6wyrPvCICBDTYL7h17ptbDTjsad7a4wmrHajXjUlmuhYuPKO5MHmZLDMGt2SuvjS2WBD
h6raR94RyRq2jk93UwS1jzM6Hdjn5w3fSo+Qoieq+mqAipTJlr2rU0A8ww83Pq/1QwWLwbJ6xSfr
74gkqeNERfUTgMSzpOmBnipvU6sl0ZEW6HfK8ca5Hr08aGNFI23y7CeaouD+T1yJAUsFD98IcJA6
u2nBeX2+WDIJ7mj1Lsmcc9eH25PgtZRpNPbgzNiJeTG4ncE+VLV1VjGDJeeHEhoLclkzE6gBAGVL
tkUEJV7vpTyJPlYTJMICH3usxaVCu/b3NkiXU5Ih22Eh87FP71H/39EBHB4OXQLpGXS1SXIqB+C2
vwNNOcdMBPywoytVazufGXnik3qWbz62Shq9S2z6NXsaFYOs49r2JYnyZmpsueNd4AIPA2oiXsZ9
lMuABFmqCZ/T4NYv1eFksQ01x5EMtEfGN+fDakbl2l6OvOwRDasGATUQVeWW157voZe8/Aer7TJG
b4qEDc39AYRAj34YjSm8E/d1ZLprtbFwU5ciWhqKnUoKoBkSiSqDRWdm+no7N30W83hEHbu9Cf4f
tqTucgYwozcumLbiAO9OYLP0BDIaq+ppPnyJsVRiRKF01S9dL5iGKA2nHtmKxOUi4ggPKoG2SWtI
vP+Inj1DWRmeZVhMm1odYm+jehtla2dWNMiThS5xG7OGivmceDzrDNr/H/OnSgqhlHlYg8PtAOkH
OKQ3OcWsOubcyFlmB/ekcyNjYGErULJhaF5gr+MeB/kOavNfaWbsMUEY0H7jXwaFukwLIsoYK1Ai
GCQ4ZqMu8hVMuLCr8y/EvdcRGn81te4r3Te1ZTD6otPse+tVrl64GqAo/tQHmklJxZslbGkWVdHY
qZDhLtbH4C3dOtLViZ510NvrqxD8Ud1Ed/mJ3oARmHIaga0XfKj9bgR9qtyspHFMtt2mVFkLk1ko
Y8wHYxIk9qkqGv8dMXArFmCGhbfzBNLigYggtS0sKxGlwAzL6oi++1+3QSp0zvNag5O1ykaWKT6g
mFe4sUlsXoYs9M9XnfVZ3Or48vqFTx5YGEmWZdHdWpZlt3Am2jrXjtB9r0+02zqJ3VCdfeyhAZB0
0JEHU16REzaTteyIlmb6Mca7DsmhpNaqLUZrpiArYPzWeA3/RIel7azv34E7qfM0yi5Z+nNAfMfS
kkOATsgXfaASWq5Y/CODTa38G0Ir7+1hGU+DgcpcAOcy60+xsZqkToExSuPKSTAsXkaIgdlv/iN1
XtC5Dt56BjMKTWj2Fl/6+8WiJFk6Y4tW80Hf3c+NKw8ErcIWeq7Npf+wCD9/G93hkUtfCG7xMDkF
KY0UqclS4hv8UwITrlVqCqKFDyzXdti7YjlqGTfttcFtn/vBtWFj10N+OInYODzRN9OBWyNiwBqg
h2KNf1ZYbXQ+Bry2autQtojjeY09BrrfbgARZhSLqiVm6a3z+EofdsmyshBYyUqFDhKv73pdYypt
WcpkRimTl/c57h9qwXhDUcI+ewyrnqWxxHqgC0K/TCmg34K/xoC8VsGCznH2MnUQl4v78HpBcSqc
Uy/GabzHRr0ZrO3U00bhLKWEA9YeRC47IVOIoFz7EW++gdCmfp0cWEXBWoqFQcHc78MycnxNTYrt
KHWFnsQ5QD6u+goFPrSXmKpKtFW3V3vMvrFznmbHAtbZUEvG38DzTRpGyBR9HchK1LQL6cWuFFvL
P43DViM8J4fQy+SyFrfP1ySmDaB3sNL8pGeYXA+9+kYhg5qB2SjfLxL5x/5U76OrwQXmeE46ZBk3
gaBHkrwvFObivxb4HmozNOOZlgEFw0AgfzLCbM/7XUnTrmiLqtdiVaLKGk9R62Jpc0rk/OPWOVPG
/lpvU9FJZKvygmpBgOzaGfaGcWHNJaWMtFFvvUZZgEeE73OHdp+oMhnBZNKgZexf+7MKuXCHoHJl
sNsQT/TU1dmzOJqHJ2PzkjpDYEgpPy2ROGQBvtRi3I3DxSPOYhRfsrpaJT14VTU01i+Fwvf1s6Qf
iCn9VrEmuJu4qtfvoCUtCcrxxwXX0OMVuYYwSfTNDDdWQEZqPPkEhjoRq7N81/dWRP7/KMigiVWE
Xh8Cf5cnm9Q2NXDTjYtHqoLoiYx6Qk73iiDrxaYYUPUDApRoMQmMH1BJ8WUiL4A3VyTSiqDxLIB7
cmOynekSaQ6Wh1qg0YYsaZBAl4EFawM8mHMlmRjsmq7kGw6cXWvMWT3ziJ/2eDuCFweLm8FT6cAo
P2MnS0KV1qho2Jztbr9QnDT6lk/jR6s/EyypQ2088tFbKGg7P/ejDyY8MUy8beOHw3O+iLd0fx+q
SQCib2Hi1xGSmnNq3AUyT9DMR+7QGNxO7ocjLZ6y/cn9+WSox/BH99hf2x8iUTdzrI7lRgogK0vE
68gDrRL90EDKSv21p4PJ4ilX4+hEnWetxnjtmbAwPialxghKmesU/ptPnm6J4LqjFz2RpX43Qp8b
nTC3an2+Hu/6FuMNDkK9Rc7WGYPbtnw5uGPPfIOBR9/kcbJhr+NMEH6kjKY36CD0J3F4QCt17I7j
AWU7kdqA1XFIBQkgOlehlYitq+4L8WE04V4/LIgFkMwKCzzsRGq8Hd0wEa5vban9pW03gagr8/Q9
QajmFlwQYhkXmjofuWPUpuIRI4n8RnXoqLAzvvd3BRee+3VqFsBVBTP/0xufEHc/hMY1YY4lh3z1
+Noa0muD+MGX654/BlqM2t8MYQLIZjQQZ4hu0NXFMOAoVmwd7eE1EnInfykJ6Q0YO4edQUkpXJVl
wt8vctCY3Ogg26agWKiSYFkhVMXgVFw6fVt3jrcvBLPbvnL8oCCl+ygMc16/bKjepttfU1z6042t
5FAwVobi3OmrkJcUi1s0Y73j6JV/SXe9IXbVhpYOw5WkUST4yeqT
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
