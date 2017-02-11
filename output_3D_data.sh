#/bin/bash

# 這裡都是 要跑3D的東西 ， 先弄個non_overlap  , 之後再用 overlap的

width=100
height=100
k_obstacles=32
m_pin=10
levels=1
mode=0

# 這裡 要run 3D的case  , 而3D的數據  只有十組 ， 所以還好   , 只是要再另外再寫一個  ， 用來跑3D auto input 的
# expect 檔  , 差點忘了 ， 這個是多層的  ， 我的輸入 是一層一次 ， 所以  下面用的那個spawn .sh 要改一下的樣子  

echo   "----------Here is running 3D non_overlap example---------\n" > ThreeD_data.txt
m_pin=50
k_obstacles=6
levels=5
echo   "Case ind1 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case ind1 done \n"

m_pin=200
k_obstacles=85
levels=6
echo   "Case ind2 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case ind2 done \n"


m_pin=250
k_obstacles=13
levels=10
echo   "Case ind3 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case ind3 done \n"

m_pin=500
k_obstacles=100
levels=5
echo   "Case ind4 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case ind4 done \n"

m_pin=1000
k_obstacles=20
levels=5
echo   "Case ind5 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case ind5 done \n"

m_pin=25
k_obstacles=10
levels=10
echo   "Case rt1 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case rt1 done \n"

m_pin=100
k_obstacles=20
levels=10
echo   "Case rt2 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case rt2 done \n"

m_pin=250
k_obstacles=50
levels=10
echo   "Case rt3 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case rt3 done \n"

m_pin=500
k_obstacles=50
levels=10
echo   "Case rt4 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case rt4 done \n"

m_pin=1000
k_obstacles=100
levels=5
echo   "Case rt5 \n" >> ThreeD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created|Time)' >> ThreeD_data.txt
echo   "Case rt5 done \n"

echo   "----------End of 3D non_overlap example---------\n" >> ThreeD_data.txt
