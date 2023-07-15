	/*
	signal gpo1 : std_logic_vector(2*busWidth-1 downto 0);--bram:bram_address,bram_data_in
	signal gpo2 : std_logic_vector(2*busWidth-1 downto 0);--template:template_address,template_data_in
	signal gpo3 : std_logic_vector(2*busWidth-1 downto 0);--control address/control value
	signal gpo4 : std_logic_vector(2*busWidth-1 downto 0);--unused
	signal gpi1 : std_logic_vector(2*busWidth-1 downto 0);--template_data_out,bram_data_out
	signal gpi2 : std_logic_vector(2*busWidth-1 downto 0);--ready
	signal gpi3 : std_logic_vector(2*busWidth-1 downto 0);--error/error_sum
	*/
	/*
				when 0 =>

				when 1 =>
					imageWidth<=control_data_in;
				when 2 =>
					imageHeight<=control_data_in;
				when 3 =>
					Ts<=control_data_in;
				when 4 =>
					iter_cnt<=control_data_in;
				when 5 =>
					template_no<=control_data_in;
				when 6 =>
					cnn_rst<=control_data_in(0);
				when 7 =>
					interface_bram_we<=control_data_in(0 downto 0);
				when 8 =>
					template_we<=control_data_in(0 downto 0);
				when 9 =>
					error_sum_slc<=control_data_in(0);
				when others =>

	*/
#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xiomodule.h"

#ifndef CNN_INIT_H_
#define CNN_INIT_H_

void init_uart();
void init_bram();
void init_templates();
void init_control();
void init_system();

#endif /* CNN_LIBRARY_H_ */