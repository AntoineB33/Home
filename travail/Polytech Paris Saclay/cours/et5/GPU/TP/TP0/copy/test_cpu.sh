#!/bin/bash

./filter inputs/siesta.png o_siesta_cpu_5.png 5 cpu
if result="$(diff o_siesta_cpu_5.png output_examples/siesta_5.png)"
then
    echo -e "\e[92m-- Passed first test\e[39m" 
    echo
else
    echo -e "\e[91m-- Failed first test\e[39m" 
    echo
fi

./filter inputs/ducks.png o_ducks_cpu_3.png 3 cpu
if result="$(diff o_ducks_cpu_3.png output_examples/ducks_3.png)"
then
    echo -e "\e[92m-- Passed second test\e[39m" 
    echo
else
    echo -e "\e[91m-- Failed second test\e[39m" 
    echo
fi

./filter inputs/waves.png o_waves_cpu_2.png 2 cpu
if result="$(diff o_waves_cpu_2.png output_examples/waves_2.png)"
then
    echo -e "\e[92m-- Passed third test\e[39m" 
    echo
else
    echo -e "\e[91m-- Failed third test\e[39m" 
    echo
fi

