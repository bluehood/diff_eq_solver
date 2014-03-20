#!/usr/bin/gnuplot -p

set term wxt 0
splot './rk2_spyral_cool.txt' u 2:3:4 w l, './euler_spyral_cool.txt' u 2:3:4 w l
set term wxt 1
splot './rk2_spyral_failed.txt' u 2:3:4 w l, './euler_spyral_failed.txt' u 2:3:4 w l
set term wxt 2
splot './rk2_spyral.txt' u 2:3:4 w l
