library ieee;
use ieee.std_logic_1164.all;

entity bascul_d is 
	PORT(
		clk , data : in std_logic;
		Q , Qb : out std_logic	
	);

end bascul_d;

architecture BEHV of bascul_d is
	begin
	process(clk)
	begin
		if rising_edge(clk) then
			Q <= data;
			Qb <= not data;
		end if;
	end process;


end BEHV;