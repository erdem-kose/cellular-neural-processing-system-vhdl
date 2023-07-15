library ieee;
	use ieee.std_logic_1164.all;
	use ieee.std_logic_unsigned.all;
	use ieee.std_logic_textio.all;
	use ieee.numeric_std.all;
library std;
	use std.textio.all;
library cnn_library;
	use cnn_library.cnn_package.all;


entity cnn_constants is
	port (
		clk : in std_logic;
		
		control_data_in : in std_logic_vector(busWidth-1 downto 0):=(others=>'0');
		control_address : in std_logic_vector(busWidth-1 downto 0):=(others=>'0');

		cnn_rst: out std_logic:='1';
		
		iter_cnt: out std_logic_vector(busWidth-1 downto 0) := std_logic_vector(to_unsigned(2,busWidth));
		template_no : out std_logic_vector(busWidth-1 downto 0) := std_logic_vector(to_unsigned(0,busWidth));
		Ts : out std_logic_vector(busWidth-1 downto 0) := std_logic_vector(to_unsigned(205,busWidth));
	
		imageWidth: out std_logic_vector(busWidth-1 downto 0) := std_logic_vector(to_unsigned(128,busWidth));
		imageHeight: out std_logic_vector(busWidth-1 downto 0) := std_logic_vector(to_unsigned(128,busWidth));
		
		interface_bram_we : out std_logic_vector(0 downto 0);
		template_we : out std_logic_vector(0 downto 0);
		
		error_sum_slc : out std_logic:='0'
	);
end cnn_constants;

architecture Behavioral of cnn_constants is

begin
	process (clk)
	begin
		if (rising_edge(clk)) then
			case (to_integer(unsigned(control_address))) is
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
					
			end case;
		end if;
	end process;

end Behavioral;
