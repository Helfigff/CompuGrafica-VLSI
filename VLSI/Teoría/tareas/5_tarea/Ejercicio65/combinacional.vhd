library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_UNSIGNED.all;

entity combinacional is 
port(
E,Q0,Q1: in std_logic;
D0,D1,S: out std_logic

);

end combinacional;

architecture arqcomb of combinacional is
begin
D0<= Q0 xor E;
D1<= (Q0 AND E) OR (Q1 AND (NOT E));
S<= (Q0 AND E) OR (Q1 AND (NOT E));

end arqcomb;

