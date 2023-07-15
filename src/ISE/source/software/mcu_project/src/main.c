	/*
	signal gpo1 : std_logic_vector(2*busWidth-1 downto 0);--bram:bram_address,bram_data_in
	signal gpo2 : std_logic_vector(2*busWidth-1 downto 0);--image dim:imageWidth,imageHeight
	signal gpo3 : std_logic_vector(2*busWidth-1 downto 0);--template:template_address,template_data_in
	signal gpo4 : std_logic_vector(2*busWidth-1 downto 0);--control register: 31:16:Ts,15:9:iter_cnt,8:3:template_no,2:cnn_rst,1:bram_we,0:template_we
	signal gpi1 : std_logic_vector(2*busWidth-1 downto 0);--template_data_out,bram_temp_data_out
	signal gpi2 : std_logic_vector(2*busWidth-1 downto 0);--ready
	*/

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xiomodule.h"

int main()
{
	init_platform();

	XIOModule uart_io; u8 data_uart=0; u8 ack_uart=0;
	ack_uart = XIOModule_Initialize(&uart_io, XPAR_IOMODULE_0_DEVICE_ID);
	ack_uart = XIOModule_Start(&uart_io);
	ack_uart = XIOModule_CfgInitialize(&uart_io, NULL, 1);

	XIOModule bram_add_datain_o; u32 bram_add_datain=0;
	bram_add_datain = XIOModule_Initialize(&bram_add_datain_o, XPAR_IOMODULE_0_DEVICE_ID);
	bram_add_datain = XIOModule_Start(&bram_add_datain_o);

	XIOModule image_width_height_o; u32 image_width_height=0;
	image_width_height = XIOModule_Initialize(&image_width_height_o, XPAR_IOMODULE_0_DEVICE_ID);
	image_width_height = XIOModule_Start(&image_width_height_o);

	XIOModule control_o; u32 control=0;
	control = XIOModule_Initialize(&control_o, XPAR_IOMODULE_0_DEVICE_ID);
	control = XIOModule_Start(&control_o);

	XIOModule bram_temp_data_out_i; u32 bram_temp_data_out=0;
	bram_temp_data_out = XIOModule_Initialize(&bram_temp_data_out_i, XPAR_IOMODULE_0_DEVICE_ID);
	bram_temp_data_out = XIOModule_Start(&bram_temp_data_out_i);

	XIOModule ready_i; u32 ready=0;
	ready = XIOModule_Initialize(&ready_i, XPAR_IOMODULE_0_DEVICE_ID);
	ready = XIOModule_Start(&ready_i);

	int i,j;
	int imageWidth=128;
	int imageHeight=128;
	int Ts=205;
	int iter_cnt=3;
	int template_no=0;

	control=(1<<2);
	XIOModule_DiscreteWrite(&control_o, 1, control);

	while (1)
	{
		while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
		switch(data_uart)
		{
			case 0://Header kismi
				while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
				imageWidth = data_uart;
				while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
				imageHeight = data_uart;
				while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
				Ts = (data_uart<<8);
				while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
				Ts |= (data_uart);
				while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
				iter_cnt = data_uart-1;
				while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
				template_no = data_uart;
				break;
			case 1://Durum goruntusunu alma
				i=0; j=imageWidth*imageHeight;
				while(i<j)
				{
					bram_add_datain = (i<<16);

					while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
					bram_add_datain |= (data_uart<<8);

					while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
					bram_add_datain |= (data_uart);

					XIOModule_DiscreteWrite(&bram_add_datain_o, 1, bram_add_datain);

					control=(1<<1);
					XIOModule_DiscreteWrite(&control_o, 4, control);

					control=(0<<1);
					XIOModule_DiscreteWrite(&control_o, 4, control);
					i=i+1;
				}
				break;
			case 2://Giris goruntusunu alma
				i=imageWidth*imageHeight; j=2*imageWidth*imageHeight;
				while(i<j)
				{
					bram_add_datain = (i<<16);

					while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
					bram_add_datain |= (data_uart<<8);

					while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);
					bram_add_datain |= (data_uart);

					XIOModule_DiscreteWrite(&bram_add_datain_o, 1, bram_add_datain);

					control=(1<<1);
					XIOModule_DiscreteWrite(&control_o, 4, control);

					control=(0<<1);
					XIOModule_DiscreteWrite(&control_o, 4, control);
					i=i+1;
				}
				break;
			case 3://Hesaplama-Algoritma burada olacak
				data_uart=3;
				while ((ack_uart = XIOModule_Send(&uart_io,&data_uart,1)) == 0);

				image_width_height=((imageWidth<<16) | imageHeight);
				XIOModule_DiscreteWrite(&image_width_height_o, 2, image_width_height);
				control=((Ts<<16) | (iter_cnt<<9) | (template_no<<3) | (1<<2)) | (0<<1) | 0;
				XIOModule_DiscreteWrite(&control_o, 4, control);
				control=((Ts<<16) | (iter_cnt<<9) | (template_no<<3) | (0<<2)) | (0<<1) | 0;
				XIOModule_DiscreteWrite(&control_o, 4, control);

				while ((ready = XIOModule_DiscreteRead(&ready_i, 1)&0xFFFFFFFE) == 0);
				while ((ack_uart = XIOModule_Send(&uart_io,&data_uart,1)) == 0);
				break;
			case 4://Goruntu yollama
				i=0; j=imageWidth*imageHeight;
				while(i<j)
				{
					bram_add_datain=(i<<16);
					XIOModule_DiscreteWrite(&bram_add_datain_o, 1, bram_add_datain);
					bram_temp_data_out = XIOModule_DiscreteRead(&bram_temp_data_out_i, 1);

					data_uart = (bram_temp_data_out&0x0000FF00)>>8;
					while ((ack_uart = XIOModule_Send(&uart_io,&data_uart,1) == 0));
					//ack_uart = XIOModule_Send(&uart_io,&data_uart,1);
					//while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);

					data_uart = (bram_temp_data_out&0x000000FF);
					while ((ack_uart = XIOModule_Send(&uart_io,&data_uart,1)) == 0);
					//ack_uart = XIOModule_Send(&uart_io,&data_uart,1);
					//while ((ack_uart = XIOModule_Recv(&uart_io, &data_uart, 1)) == 0);

					i=i+1;
				}
				data_uart=0;
				break;
			default:
				break;
		}
	}

	cleanup_platform();
	return 0;


}