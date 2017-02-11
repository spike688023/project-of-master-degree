 set title "Obstacle_Avoding_Kruskal_Tree" 
 set xrange[0:199]
 set yrange[0:199]
set xlabel "X-axis"
set ylabel "Y-axis"
set output 
set terminal X11 
set style rect fc lt -1 fs solid 0.15 noborder  
# 這裡要印出　obstacle 的資訊　
set object 1 rect from 25,51  to 40,60  
set object 2 rect from 28,173  to 34,195  
set object 3 rect from 155,45  to 163,64  
set object 4 rect from 133,186  to 143,190  
set object 5 rect from 4,100  to 8,186  
set object 6 rect from 66,6  to 171,7  
set object 7 rect from 1,67  to 78,83  
set object 8 rect from 176,115  to 190,161  
set object 9 rect from 70,13  to 155,16  
set object 10 rect from 58,21  to 113,27  
set object 11 rect from 57,87  to 134,97  
set object 12 rect from 170,9  to 178,51  
set object 13 rect from 149,119  to 163,126  
set object 14 rect from 102,49  to 124,84  
set object 15 rect from 85,152  to 144,172  
set object 16 rect from 92,137  to 127,149  
set object 17 rect from 164,90  to 168,141  
set object 18 rect from 41,111  to 122,116  
set object 19 rect from 51,122  to 71,182  
set object 20 rect from 117,186  to 120,193  
set object 21 rect from 182,61  to 185,86  
set object 22 rect from 108,9  to 143,11  
set object 23 rect from 9,83  to 31,109  
set object 24 rect from 23,147  to 45,167  
set object 25 rect from 10,10  to 16,65  
set object 26 rect from 137,127  to 144,137  
set object 27 rect from 3,16  to 5,31  
set object 28 rect from 82,118  to 96,129  
set object 29 rect from 73,42  to 108,43  
set object 30 rect from 192,32  to 198,171  
set object 31 rect from 135,28  to 139,102  
set object 32 rect from 20,29  to 29,41  
# 這裡要印出　edge   
set arrow 1 from 122 , 116 to 41 , 116  nohead lt 1 lc rgb "black" lw 1 
set arrow 2 from 157 , 3 to 86 , 1  nohead lt 1 lc rgb "black" lw 1 
set arrow 3 from 66 , 7 to 16 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 4 from 41 , 116 to 29 , 145  nohead lt 1 lc rgb "black" lw 1 
set arrow 5 from 144 , 152 to 176 , 161  nohead lt 1 lc rgb "black" lw 1 
set arrow 6 from 20 , 189 to 9 , 164  nohead lt 1 lc rgb "black" lw 1 
set arrow 7 from 23 , 147 to 9 , 164  nohead lt 1 lc rgb "black" lw 1 
set arrow 8 from 112 , 132 to 122 , 116  nohead lt 1 lc rgb "black" lw 1 
set arrow 9 from 3 , 31 to 3 , 57  nohead lt 1 lc rgb "black" lw 1 
set arrow 10 from 86 , 1 to 66 , 6  nohead lt 1 lc rgb "black" lw 1 
set arrow 11 from 190 , 161 to 191 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 12 from 127 , 149 to 144 , 152  nohead lt 1 lc rgb "black" lw 1 
set arrow 13 from 112 , 132 to 127 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 14 from 1 , 83 to 1 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 15 from 3 , 16 to 3 , 31  nohead lt 1 lc rgb "black" lw 1 
set arrow 16 from 176 , 161 to 190 , 161  nohead lt 1 lc rgb "black" lw 1 
set arrow 17 from 127 , 137 to 127 , 149  nohead lt 1 lc rgb "black" lw 1 
set arrow 18 from 1 , 67 to 3 , 57  nohead lt 1 lc rgb "black" lw 1 
set arrow 19 from 10 , 10 to 2 , 14  nohead lt 1 lc rgb "black" lw 1 
set arrow 20 from 8 , 100 to 9 , 109  nohead lt 1 lc rgb "black" lw 1 
set arrow 21 from 1 , 93 to 4 , 100  nohead lt 1 lc rgb "black" lw 1 
set arrow 22 from 1 , 93 to 1 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 23 from 29 , 145 to 23 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 24 from 16 , 10 to 10 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 25 from 4 , 100 to 8 , 100  nohead lt 1 lc rgb "black" lw 1 
set arrow 26 from 2 , 14 to 3 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 27 from 66 , 6 to 66 , 7  nohead lt 1 lc rgb "black" lw 1 
set arrow 28 from 9 , 109 to 23 , 147  nohead lt 1 lc rgb "black" lw 1 
plot "PlaneData.txt" w lp pt 7 lc rgb "black" 
pause -1 
