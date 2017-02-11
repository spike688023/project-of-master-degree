 set title "OASG" 
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
set arrow 1 from 1 , 93 to 1 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 2 from 1 , 83 to 1 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 3 from 3 , 57 to 3 , 31  nohead lt 1 lc rgb "black" lw 1 
set arrow 4 from 3 , 31 to 3 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 5 from 4 , 186 to 4 , 100  nohead lt 1 lc rgb "black" lw 1 
set arrow 6 from 5 , 31 to 5 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 7 from 8 , 186 to 8 , 100  nohead lt 1 lc rgb "black" lw 1 
set arrow 8 from 9 , 164 to 9 , 109  nohead lt 1 lc rgb "black" lw 1 
set arrow 9 from 9 , 109 to 9 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 10 from 10 , 65 to 10 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 11 from 16 , 65 to 16 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 12 from 20 , 41 to 20 , 29  nohead lt 1 lc rgb "black" lw 1 
set arrow 13 from 23 , 167 to 23 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 14 from 25 , 60 to 25 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 15 from 28 , 195 to 28 , 173  nohead lt 1 lc rgb "black" lw 1 
set arrow 16 from 29 , 41 to 29 , 29  nohead lt 1 lc rgb "black" lw 1 
set arrow 17 from 31 , 109 to 31 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 18 from 34 , 195 to 34 , 173  nohead lt 1 lc rgb "black" lw 1 
set arrow 19 from 40 , 60 to 40 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 20 from 41 , 116 to 41 , 111  nohead lt 1 lc rgb "black" lw 1 
set arrow 21 from 45 , 167 to 45 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 22 from 51 , 182 to 51 , 122  nohead lt 1 lc rgb "black" lw 1 
set arrow 23 from 57 , 97 to 57 , 87  nohead lt 1 lc rgb "black" lw 1 
set arrow 24 from 58 , 27 to 58 , 21  nohead lt 1 lc rgb "black" lw 1 
set arrow 25 from 66 , 7 to 66 , 6  nohead lt 1 lc rgb "black" lw 1 
set arrow 26 from 70 , 16 to 70 , 13  nohead lt 1 lc rgb "black" lw 1 
set arrow 27 from 71 , 182 to 71 , 122  nohead lt 1 lc rgb "black" lw 1 
set arrow 28 from 73 , 43 to 73 , 42  nohead lt 1 lc rgb "black" lw 1 
set arrow 29 from 78 , 83 to 78 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 30 from 82 , 129 to 82 , 118  nohead lt 1 lc rgb "black" lw 1 
set arrow 31 from 85 , 172 to 85 , 152  nohead lt 1 lc rgb "black" lw 1 
set arrow 32 from 92 , 149 to 92 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 33 from 96 , 129 to 96 , 118  nohead lt 1 lc rgb "black" lw 1 
set arrow 34 from 102 , 84 to 102 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 35 from 108 , 43 to 108 , 42  nohead lt 1 lc rgb "black" lw 1 
set arrow 36 from 108 , 11 to 108 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 37 from 113 , 27 to 113 , 21  nohead lt 1 lc rgb "black" lw 1 
set arrow 38 from 117 , 193 to 117 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 39 from 120 , 193 to 120 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 40 from 122 , 116 to 122 , 111  nohead lt 1 lc rgb "black" lw 1 
set arrow 41 from 124 , 84 to 124 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 42 from 127 , 149 to 127 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 43 from 133 , 190 to 133 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 44 from 134 , 97 to 134 , 87  nohead lt 1 lc rgb "black" lw 1 
set arrow 45 from 135 , 102 to 135 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 46 from 137 , 137 to 137 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 47 from 139 , 102 to 139 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 48 from 143 , 190 to 143 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 49 from 143 , 11 to 143 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 50 from 144 , 172 to 144 , 152  nohead lt 1 lc rgb "black" lw 1 
set arrow 51 from 144 , 152 to 144 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 52 from 144 , 137 to 144 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 53 from 149 , 126 to 149 , 119  nohead lt 1 lc rgb "black" lw 1 
set arrow 54 from 155 , 64 to 155 , 45  nohead lt 1 lc rgb "black" lw 1 
set arrow 55 from 155 , 45 to 155 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 56 from 155 , 16 to 155 , 13  nohead lt 1 lc rgb "black" lw 1 
set arrow 57 from 163 , 126 to 163 , 119  nohead lt 1 lc rgb "black" lw 1 
set arrow 58 from 163 , 119 to 163 , 64  nohead lt 1 lc rgb "black" lw 1 
set arrow 59 from 163 , 64 to 163 , 45  nohead lt 1 lc rgb "black" lw 1 
set arrow 60 from 164 , 141 to 164 , 90  nohead lt 1 lc rgb "black" lw 1 
set arrow 61 from 168 , 141 to 168 , 90  nohead lt 1 lc rgb "black" lw 1 
set arrow 62 from 170 , 51 to 170 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 63 from 171 , 7 to 171 , 6  nohead lt 1 lc rgb "black" lw 1 
set arrow 64 from 176 , 161 to 176 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 65 from 178 , 51 to 178 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 66 from 182 , 86 to 182 , 61  nohead lt 1 lc rgb "black" lw 1 
set arrow 67 from 185 , 86 to 185 , 61  nohead lt 1 lc rgb "black" lw 1 
set arrow 68 from 190 , 161 to 190 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 69 from 192 , 171 to 192 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 70 from 198 , 171 to 198 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 71 from 1 , 67 to 2 , 14  nohead lt 1 lc rgb "black" lw 1 
set arrow 72 from 1 , 67 to 3 , 57  nohead lt 1 lc rgb "black" lw 1 
set arrow 73 from 1 , 93 to 4 , 100  nohead lt 1 lc rgb "black" lw 1 
set arrow 74 from 1 , 83 to 9 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 75 from 1 , 67 to 10 , 65  nohead lt 1 lc rgb "black" lw 1 
set arrow 76 from 1 , 67 to 78 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 77 from 2 , 14 to 3 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 78 from 2 , 14 to 10 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 79 from 3 , 31 to 5 , 31  nohead lt 1 lc rgb "black" lw 1 
set arrow 80 from 3 , 16 to 5 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 81 from 3 , 57 to 10 , 65  nohead lt 1 lc rgb "black" lw 1 
set arrow 82 from 4 , 186 to 8 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 83 from 4 , 100 to 8 , 100  nohead lt 1 lc rgb "black" lw 1 
set arrow 84 from 5 , 31 to 10 , 65  nohead lt 1 lc rgb "black" lw 1 
set arrow 85 from 5 , 16 to 10 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 86 from 8 , 186 to 9 , 164  nohead lt 1 lc rgb "black" lw 1 
set arrow 87 from 8 , 100 to 9 , 109  nohead lt 1 lc rgb "black" lw 1 
set arrow 88 from 8 , 100 to 9 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 89 from 8 , 186 to 20 , 189  nohead lt 1 lc rgb "black" lw 1 
set arrow 90 from 8 , 186 to 23 , 167  nohead lt 1 lc rgb "black" lw 1 
set arrow 91 from 8 , 186 to 28 , 173  nohead lt 1 lc rgb "black" lw 1 
set arrow 92 from 9 , 164 to 20 , 189  nohead lt 1 lc rgb "black" lw 1 
set arrow 93 from 9 , 164 to 23 , 167  nohead lt 1 lc rgb "black" lw 1 
set arrow 94 from 9 , 164 to 23 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 95 from 9 , 109 to 23 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 96 from 9 , 109 to 29 , 145  nohead lt 1 lc rgb "black" lw 1 
set arrow 97 from 9 , 109 to 31 , 109  nohead lt 1 lc rgb "black" lw 1 
set arrow 98 from 9 , 83 to 31 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 99 from 10 , 65 to 16 , 65  nohead lt 1 lc rgb "black" lw 1 
set arrow 100 from 10 , 10 to 16 , 10  nohead lt 1 lc rgb "black" lw 1 
set arrow 101 from 16 , 65 to 20 , 41  nohead lt 1 lc rgb "black" lw 1 
set arrow 102 from 16 , 10 to 20 , 29  nohead lt 1 lc rgb "black" lw 1 
set arrow 103 from 16 , 65 to 25 , 60  nohead lt 1 lc rgb "black" lw 1 
set arrow 104 from 16 , 10 to 58 , 21  nohead lt 1 lc rgb "black" lw 1 
set arrow 105 from 16 , 10 to 66 , 7  nohead lt 1 lc rgb "black" lw 1 
set arrow 106 from 16 , 10 to 70 , 13  nohead lt 1 lc rgb "black" lw 1 
set arrow 107 from 16 , 65 to 78 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 108 from 16 , 10 to 108 , 11  nohead lt 1 lc rgb "black" lw 1 
set arrow 109 from 16 , 10 to 108 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 110 from 20 , 189 to 23 , 167  nohead lt 1 lc rgb "black" lw 1 
set arrow 111 from 20 , 41 to 25 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 112 from 20 , 189 to 28 , 195  nohead lt 1 lc rgb "black" lw 1 
set arrow 113 from 20 , 189 to 28 , 173  nohead lt 1 lc rgb "black" lw 1 
set arrow 114 from 20 , 41 to 29 , 41  nohead lt 1 lc rgb "black" lw 1 
set arrow 115 from 20 , 29 to 29 , 29  nohead lt 1 lc rgb "black" lw 1 
set arrow 116 from 23 , 167 to 28 , 173  nohead lt 1 lc rgb "black" lw 1 
set arrow 117 from 23 , 147 to 29 , 145  nohead lt 1 lc rgb "black" lw 1 
set arrow 118 from 23 , 167 to 45 , 167  nohead lt 1 lc rgb "black" lw 1 
set arrow 119 from 23 , 147 to 45 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 120 from 25 , 51 to 29 , 41  nohead lt 1 lc rgb "black" lw 1 
set arrow 121 from 25 , 60 to 40 , 60  nohead lt 1 lc rgb "black" lw 1 
set arrow 122 from 25 , 51 to 40 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 123 from 28 , 195 to 34 , 195  nohead lt 1 lc rgb "black" lw 1 
set arrow 124 from 28 , 173 to 34 , 173  nohead lt 1 lc rgb "black" lw 1 
set arrow 125 from 29 , 145 to 31 , 109  nohead lt 1 lc rgb "black" lw 1 
set arrow 126 from 29 , 41 to 40 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 127 from 29 , 145 to 41 , 116  nohead lt 1 lc rgb "black" lw 1 
set arrow 128 from 29 , 145 to 45 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 129 from 29 , 145 to 51 , 122  nohead lt 1 lc rgb "black" lw 1 
set arrow 130 from 29 , 29 to 58 , 27  nohead lt 1 lc rgb "black" lw 1 
set arrow 131 from 29 , 41 to 73 , 42  nohead lt 1 lc rgb "black" lw 1 
set arrow 132 from 29 , 29 to 135 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 133 from 31 , 109 to 41 , 111  nohead lt 1 lc rgb "black" lw 1 
set arrow 134 from 31 , 109 to 57 , 97  nohead lt 1 lc rgb "black" lw 1 
set arrow 135 from 31 , 83 to 57 , 87  nohead lt 1 lc rgb "black" lw 1 
set arrow 136 from 31 , 83 to 78 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 137 from 31 , 109 to 135 , 102  nohead lt 1 lc rgb "black" lw 1 
set arrow 138 from 34 , 173 to 45 , 167  nohead lt 1 lc rgb "black" lw 1 
set arrow 139 from 34 , 195 to 51 , 182  nohead lt 1 lc rgb "black" lw 1 
set arrow 140 from 34 , 173 to 51 , 182  nohead lt 1 lc rgb "black" lw 1 
set arrow 141 from 34 , 195 to 117 , 193  nohead lt 1 lc rgb "black" lw 1 
set arrow 142 from 40 , 51 to 58 , 27  nohead lt 1 lc rgb "black" lw 1 
set arrow 143 from 40 , 51 to 73 , 43  nohead lt 1 lc rgb "black" lw 1 
set arrow 144 from 40 , 60 to 78 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 145 from 40 , 51 to 102 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 146 from 41 , 116 to 45 , 147  nohead lt 1 lc rgb "black" lw 1 
set arrow 147 from 41 , 116 to 51 , 122  nohead lt 1 lc rgb "black" lw 1 
set arrow 148 from 41 , 111 to 57 , 97  nohead lt 1 lc rgb "black" lw 1 
set arrow 149 from 41 , 116 to 82 , 118  nohead lt 1 lc rgb "black" lw 1 
set arrow 150 from 41 , 116 to 122 , 116  nohead lt 1 lc rgb "black" lw 1 
set arrow 151 from 41 , 111 to 122 , 111  nohead lt 1 lc rgb "black" lw 1 
set arrow 152 from 45 , 167 to 51 , 182  nohead lt 1 lc rgb "black" lw 1 
set arrow 153 from 45 , 147 to 51 , 122  nohead lt 1 lc rgb "black" lw 1 
set arrow 154 from 51 , 182 to 71 , 182  nohead lt 1 lc rgb "black" lw 1 
set arrow 155 from 51 , 122 to 71 , 122  nohead lt 1 lc rgb "black" lw 1 
set arrow 156 from 57 , 87 to 78 , 83  nohead lt 1 lc rgb "black" lw 1 
set arrow 157 from 57 , 87 to 102 , 84  nohead lt 1 lc rgb "black" lw 1 
set arrow 158 from 57 , 97 to 122 , 111  nohead lt 1 lc rgb "black" lw 1 
set arrow 159 from 57 , 97 to 134 , 97  nohead lt 1 lc rgb "black" lw 1 
set arrow 160 from 57 , 87 to 134 , 87  nohead lt 1 lc rgb "black" lw 1 
set arrow 161 from 58 , 21 to 66 , 7  nohead lt 1 lc rgb "black" lw 1 
set arrow 162 from 58 , 21 to 70 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 163 from 58 , 27 to 73 , 42  nohead lt 1 lc rgb "black" lw 1 
set arrow 164 from 58 , 27 to 113 , 27  nohead lt 1 lc rgb "black" lw 1 
set arrow 165 from 58 , 21 to 113 , 21  nohead lt 1 lc rgb "black" lw 1 
set arrow 166 from 66 , 7 to 70 , 13  nohead lt 1 lc rgb "black" lw 1 
set arrow 167 from 66 , 6 to 86 , 1  nohead lt 1 lc rgb "black" lw 1 
set arrow 168 from 66 , 7 to 108 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 169 from 66 , 6 to 157 , 3  nohead lt 1 lc rgb "black" lw 1 
set arrow 170 from 66 , 7 to 171 , 7  nohead lt 1 lc rgb "black" lw 1 
set arrow 171 from 66 , 6 to 171 , 6  nohead lt 1 lc rgb "black" lw 1 
set arrow 172 from 70 , 13 to 108 , 11  nohead lt 1 lc rgb "black" lw 1 
set arrow 173 from 70 , 16 to 113 , 21  nohead lt 1 lc rgb "black" lw 1 
set arrow 174 from 70 , 16 to 155 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 175 from 70 , 13 to 155 , 13  nohead lt 1 lc rgb "black" lw 1 
set arrow 176 from 71 , 182 to 82 , 129  nohead lt 1 lc rgb "black" lw 1 
set arrow 177 from 71 , 122 to 82 , 129  nohead lt 1 lc rgb "black" lw 1 
set arrow 178 from 71 , 122 to 82 , 118  nohead lt 1 lc rgb "black" lw 1 
set arrow 179 from 71 , 182 to 85 , 172  nohead lt 1 lc rgb "black" lw 1 
set arrow 180 from 71 , 182 to 117 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 181 from 73 , 43 to 78 , 67  nohead lt 1 lc rgb "black" lw 1 
set arrow 182 from 73 , 43 to 102 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 183 from 73 , 43 to 108 , 43  nohead lt 1 lc rgb "black" lw 1 
set arrow 184 from 73 , 42 to 108 , 42  nohead lt 1 lc rgb "black" lw 1 
set arrow 185 from 78 , 83 to 102 , 84  nohead lt 1 lc rgb "black" lw 1 
set arrow 186 from 78 , 67 to 102 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 187 from 82 , 129 to 85 , 152  nohead lt 1 lc rgb "black" lw 1 
set arrow 188 from 82 , 129 to 92 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 189 from 82 , 129 to 96 , 129  nohead lt 1 lc rgb "black" lw 1 
set arrow 190 from 82 , 118 to 96 , 118  nohead lt 1 lc rgb "black" lw 1 
set arrow 191 from 85 , 152 to 92 , 149  nohead lt 1 lc rgb "black" lw 1 
set arrow 192 from 85 , 172 to 117 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 193 from 85 , 172 to 144 , 172  nohead lt 1 lc rgb "black" lw 1 
set arrow 194 from 85 , 152 to 144 , 152  nohead lt 1 lc rgb "black" lw 1 
set arrow 195 from 86 , 1 to 157 , 3  nohead lt 1 lc rgb "black" lw 1 
set arrow 196 from 92 , 137 to 96 , 129  nohead lt 1 lc rgb "black" lw 1 
set arrow 197 from 92 , 137 to 112 , 132  nohead lt 1 lc rgb "black" lw 1 
set arrow 198 from 92 , 149 to 127 , 149  nohead lt 1 lc rgb "black" lw 1 
set arrow 199 from 92 , 137 to 127 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 200 from 96 , 129 to 112 , 132  nohead lt 1 lc rgb "black" lw 1 
set arrow 201 from 96 , 118 to 122 , 116  nohead lt 1 lc rgb "black" lw 1 
set arrow 202 from 96 , 129 to 137 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 203 from 96 , 118 to 137 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 204 from 96 , 118 to 149 , 119  nohead lt 1 lc rgb "black" lw 1 
set arrow 205 from 102 , 49 to 108 , 43  nohead lt 1 lc rgb "black" lw 1 
set arrow 206 from 102 , 84 to 124 , 84  nohead lt 1 lc rgb "black" lw 1 
set arrow 207 from 102 , 49 to 124 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 208 from 108 , 42 to 113 , 27  nohead lt 1 lc rgb "black" lw 1 
set arrow 209 from 108 , 43 to 124 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 210 from 108 , 42 to 135 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 211 from 108 , 11 to 143 , 11  nohead lt 1 lc rgb "black" lw 1 
set arrow 212 from 108 , 9 to 143 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 213 from 112 , 132 to 122 , 116  nohead lt 1 lc rgb "black" lw 1 
set arrow 214 from 112 , 132 to 127 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 215 from 112 , 132 to 137 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 216 from 113 , 27 to 124 , 49  nohead lt 1 lc rgb "black" lw 1 
set arrow 217 from 113 , 27 to 135 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 218 from 113 , 21 to 155 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 219 from 117 , 193 to 120 , 193  nohead lt 1 lc rgb "black" lw 1 
set arrow 220 from 117 , 186 to 120 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 221 from 120 , 193 to 133 , 190  nohead lt 1 lc rgb "black" lw 1 
set arrow 222 from 120 , 186 to 133 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 223 from 122 , 116 to 127 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 224 from 122 , 111 to 134 , 97  nohead lt 1 lc rgb "black" lw 1 
set arrow 225 from 122 , 111 to 135 , 102  nohead lt 1 lc rgb "black" lw 1 
set arrow 226 from 122 , 116 to 137 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 227 from 122 , 116 to 149 , 119  nohead lt 1 lc rgb "black" lw 1 
set arrow 228 from 124 , 84 to 134 , 87  nohead lt 1 lc rgb "black" lw 1 
set arrow 229 from 124 , 49 to 135 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 230 from 127 , 137 to 134 , 97  nohead lt 1 lc rgb "black" lw 1 
set arrow 231 from 127 , 137 to 135 , 102  nohead lt 1 lc rgb "black" lw 1 
set arrow 232 from 127 , 137 to 137 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 233 from 127 , 149 to 144 , 152  nohead lt 1 lc rgb "black" lw 1 
set arrow 234 from 127 , 149 to 164 , 141  nohead lt 1 lc rgb "black" lw 1 
set arrow 235 from 133 , 190 to 143 , 190  nohead lt 1 lc rgb "black" lw 1 
set arrow 236 from 133 , 186 to 143 , 186  nohead lt 1 lc rgb "black" lw 1 
set arrow 237 from 134 , 97 to 135 , 102  nohead lt 1 lc rgb "black" lw 1 
set arrow 238 from 134 , 87 to 135 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 239 from 135 , 102 to 137 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 240 from 135 , 102 to 139 , 102  nohead lt 1 lc rgb "black" lw 1 
set arrow 241 from 135 , 28 to 139 , 28  nohead lt 1 lc rgb "black" lw 1 
set arrow 242 from 137 , 127 to 139 , 102  nohead lt 1 lc rgb "black" lw 1 
set arrow 243 from 137 , 137 to 144 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 244 from 137 , 127 to 144 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 245 from 139 , 102 to 144 , 127  nohead lt 1 lc rgb "black" lw 1 
set arrow 246 from 139 , 102 to 149 , 119  nohead lt 1 lc rgb "black" lw 1 
set arrow 247 from 139 , 102 to 155 , 64  nohead lt 1 lc rgb "black" lw 1 
set arrow 248 from 139 , 28 to 155 , 45  nohead lt 1 lc rgb "black" lw 1 
set arrow 249 from 139 , 28 to 155 , 16  nohead lt 1 lc rgb "black" lw 1 
set arrow 250 from 139 , 102 to 164 , 90  nohead lt 1 lc rgb "black" lw 1 
set arrow 251 from 143 , 186 to 144 , 172  nohead lt 1 lc rgb "black" lw 1 
set arrow 252 from 143 , 11 to 155 , 13  nohead lt 1 lc rgb "black" lw 1 
set arrow 253 from 143 , 9 to 170 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 254 from 144 , 127 to 149 , 126  nohead lt 1 lc rgb "black" lw 1 
set arrow 255 from 144 , 152 to 164 , 141  nohead lt 1 lc rgb "black" lw 1 
set arrow 256 from 144 , 137 to 164 , 141  nohead lt 1 lc rgb "black" lw 1 
set arrow 257 from 144 , 172 to 176 , 161  nohead lt 1 lc rgb "black" lw 1 
set arrow 258 from 144 , 152 to 176 , 161  nohead lt 1 lc rgb "black" lw 1 
set arrow 259 from 144 , 172 to 192 , 171  nohead lt 1 lc rgb "black" lw 1 
set arrow 260 from 149 , 119 to 155 , 64  nohead lt 1 lc rgb "black" lw 1 
set arrow 261 from 149 , 126 to 163 , 126  nohead lt 1 lc rgb "black" lw 1 
set arrow 262 from 149 , 119 to 163 , 119  nohead lt 1 lc rgb "black" lw 1 
set arrow 263 from 155 , 64 to 163 , 64  nohead lt 1 lc rgb "black" lw 1 
set arrow 264 from 155 , 45 to 163 , 45  nohead lt 1 lc rgb "black" lw 1 
set arrow 265 from 155 , 13 to 170 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 266 from 157 , 3 to 171 , 6  nohead lt 1 lc rgb "black" lw 1 
set arrow 267 from 163 , 126 to 164 , 141  nohead lt 1 lc rgb "black" lw 1 
set arrow 268 from 163 , 119 to 164 , 90  nohead lt 1 lc rgb "black" lw 1 
set arrow 269 from 163 , 64 to 164 , 90  nohead lt 1 lc rgb "black" lw 1 
set arrow 270 from 163 , 64 to 170 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 271 from 163 , 45 to 170 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 272 from 163 , 45 to 170 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 273 from 163 , 64 to 182 , 86  nohead lt 1 lc rgb "black" lw 1 
set arrow 274 from 163 , 64 to 182 , 61  nohead lt 1 lc rgb "black" lw 1 
set arrow 275 from 164 , 141 to 168 , 141  nohead lt 1 lc rgb "black" lw 1 
set arrow 276 from 164 , 90 to 168 , 90  nohead lt 1 lc rgb "black" lw 1 
set arrow 277 from 168 , 90 to 170 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 278 from 168 , 141 to 176 , 161  nohead lt 1 lc rgb "black" lw 1 
set arrow 279 from 168 , 141 to 176 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 280 from 168 , 90 to 176 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 281 from 168 , 90 to 182 , 86  nohead lt 1 lc rgb "black" lw 1 
set arrow 282 from 170 , 9 to 171 , 7  nohead lt 1 lc rgb "black" lw 1 
set arrow 283 from 170 , 51 to 176 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 284 from 170 , 51 to 178 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 285 from 170 , 9 to 178 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 286 from 171 , 7 to 178 , 9  nohead lt 1 lc rgb "black" lw 1 
set arrow 287 from 176 , 115 to 178 , 51  nohead lt 1 lc rgb "black" lw 1 
set arrow 288 from 176 , 115 to 182 , 86  nohead lt 1 lc rgb "black" lw 1 
set arrow 289 from 176 , 161 to 190 , 161  nohead lt 1 lc rgb "black" lw 1 
set arrow 290 from 176 , 115 to 190 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 291 from 178 , 51 to 182 , 61  nohead lt 1 lc rgb "black" lw 1 
set arrow 292 from 178 , 51 to 192 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 293 from 178 , 9 to 192 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 294 from 182 , 86 to 185 , 86  nohead lt 1 lc rgb "black" lw 1 
set arrow 295 from 182 , 61 to 185 , 61  nohead lt 1 lc rgb "black" lw 1 
set arrow 296 from 185 , 86 to 190 , 115  nohead lt 1 lc rgb "black" lw 1 
set arrow 297 from 185 , 61 to 192 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 298 from 190 , 161 to 191 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 299 from 190 , 115 to 191 , 137  nohead lt 1 lc rgb "black" lw 1 
set arrow 300 from 190 , 161 to 192 , 171  nohead lt 1 lc rgb "black" lw 1 
set arrow 301 from 190 , 115 to 192 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 302 from 191 , 137 to 192 , 171  nohead lt 1 lc rgb "black" lw 1 
set arrow 303 from 191 , 137 to 192 , 32  nohead lt 1 lc rgb "black" lw 1 
set arrow 304 from 192 , 171 to 198 , 171  nohead lt 1 lc rgb "black" lw 1 
set arrow 305 from 192 , 32 to 198 , 32  nohead lt 1 lc rgb "black" lw 1 
plot "PlaneData.txt" w lp pt 7 lc rgb "black" 
pause -1 
