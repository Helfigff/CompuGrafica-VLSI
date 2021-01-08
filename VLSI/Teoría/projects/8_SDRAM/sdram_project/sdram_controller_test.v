//En caso de no funcionar el top de VHDL, poner como entidad top este controlador en verilog 
module sdram_controller_test(

	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	output		    [12:0]		DRAM_ADDR,
	output		     [1:0]		DRAM_BA,
	output		          		DRAM_CAS_N,
	output		          		DRAM_CKE,
	output		          		DRAM_CLK,
	output		          		DRAM_CS_N,
	inout 		    [15:0]		DRAM_DQ,
	output		          		DRAM_LDQM,
	output		          		DRAM_RAS_N,
	output		          		DRAM_UDQM,
	output		          		DRAM_WE_N,

	input 		     [1:0]		KEY,

	output		     [9:0]		LEDR,
 
	input 		     [9:0]		SW
);



reg   [127:0]   data;
reg     [4:0]   state           = 5'b00001;
reg     [4:0]   next_state      = 5'b00010;

wire   [21:0]   address         = 22'b1;
wire            reset           = 1'b0;

wire            write_command;
wire            read_command;
wire            write_finished;
wire            read_finished;
wire  [127:0]   write_data;
wire  [127:0]   read_data;

reg             write_request;
reg             read_request;

assign  write_data      = {112'b0, SW};
assign  LEDR            = data[9:0];

assign  write_command   = ~KEY[0];
assign  read_command    = ~KEY[1];

always @(posedge MAX10_CLK1_50)
begin
    state <= #1 next_state;
end


always @(state or write_command or read_command or write_finished or read_finished)
begin
    case(state)
        5'b00001:
            if(write_command)
                next_state  <= 5'b00010;
            else if(read_command)
                next_state  <= 5'b01000;
            else
                next_state  <= 5'b00001;
                
        5'b00010:
            if(write_finished)
                next_state  <= 5'b00100;
            else
                next_state  <= 5'b00010;
        5'b00100:
            next_state      <= 5'b00001;
            
        5'b01000:
            if(read_finished)
                next_state  <= 5'b10000;
            else
                next_state  <= 5'b01000;
        5'b10000:
            next_state      <= 5'b00001;
    endcase
end

always @(state)
begin
    case(state)
        5'b00001:
        begin
            write_request   <= #1 1'b0;
            read_request    <= #1 1'b0;
        end
        
        5'b00010:
        begin
            write_request   <= #1 1'b1;
            read_request    <= #1 1'b0;
        end
        5'b00100:
        begin
            write_request   <= #1 1'b0;
            read_request    <= #1 1'b0;
        end
        
        5'b01000:
        begin
            write_request   <= #1 1'b0;
            read_request    <= #1 1'b1;
        end
        5'b10000:
        begin
            write_request   <= #1 1'b0;
            read_request    <= #1 1'b0;
            data            <= #1 read_data;
        end
    endcase
end

sdram_controller sdram_controller(
	.iclk(MAX10_CLK1_50),
    .ireset(reset),
    
    .iwrite_req(write_request),
    .iwrite_address(address),
    .iwrite_data(write_data),
    .owrite_ack(write_finished),
    
    .iread_req(read_request),
    .iread_address(address),
    .oread_data(read_data),
    .oread_ack(read_finished),
    
	.DRAM_ADDR(DRAM_ADDR),
    .DRAM_BA(DRAM_BA),
    .DRAM_CAS_N(DRAM_CAS_N),
    .DRAM_CKE(DRAM_CKE),
    .DRAM_CLK(DRAM_CLK),
    .DRAM_CS_N(DRAM_CS_N),
    .DRAM_DQ(DRAM_DQ),
    .DRAM_LDQM(DRAM_LDQM),
    .DRAM_RAS_N(DRAM_RAS_N),
    .DRAM_UDQM(DRAM_UDQM),
    .DRAM_WE_N(DRAM_WE_N)
);

endmodule
