`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: PSU CMPEN 331
// Engineer: David Rohweder
// 
// Create Date: 03/21/2022 04:25:25 PM
// Design Name: 
// Module Name: testbench
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module testbench();
    reg clock;
    wire [31:0]pc;
    wire [31:0]dinstOut;
    wire ewreg;
    wire em2reg;
    wire ewmem;
    wire [3:0]ealuc;
    wire ealuimm;
    wire [4:0]ern;
    wire [31:0]eqa;
    wire [31:0]eqb;
    wire [31:0]eimm32;
    wire mwreg;
    wire mm2reg; 
    wire mwmem; 
    wire [4:0]mrn;
    wire [31:0]mr;
    wire [31:0]mqb;
    wire wwreg;
    wire [4:0]wrn;    
    wire wm2reg;
    wire [31:0]wr;
    wire [31:0]wdo;
    wire [31:0]r; // project
    wire [31:0]mdo; // project 

    initial begin
        clock = 0;
    end

    datapath dataP(clock, pc, dinstOut, ewreg, em2reg, ewmem, ealuc, ealuimm, ern, eqa, eqb, eimm32, mwreg, mm2reg, mwmem, mrn, mr, mqb, wwreg, wrn, wm2reg, wr, wdo, r, mdo);
    always begin
        #5
        clock = ~clock;
    end
    
endmodule
