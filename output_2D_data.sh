#/bin/bash

# 只要打  sh output_data.sh 就可以了  不用加什麼東西  
# 好了 ， 這樣 確實可以拿到我要的 ， 那些行  
# 好吧  ， 看來是把 non_overlap 的2 ， 3D  都給寫在這個檔好了  
# 0 = non_overlap , 1 = overlap 
# 丟入 expect 檔的參數 順序分別是 ,  $width $height $k_obstacles $m_pin $levels $mode
#  我打算就用這麼一個檔  去跑所有的case ， 即便是non_overlap , 及 overlap 
# 我overlap  還有個問題哎  ， 就是有的case  跑不到尾巴  ， 不管了 ， 總之
# 先把 所有要傳入的參數  都給它 自動化好 

width=100
height=100
k_obstacles=32
m_pin=10
levels=1
mode=0

# 下面的 -ne 是網路上查的 為了可以 讓echo 認識換行字符
echo   "----------Here is running 2D non_overlap example---------\n" > twoD_data.txt
# 我想寫成 傳四個參數 ， 順序分別是 width height obsatcles m_pins 
# 太久沒看  都忘了 有的東西 都忘了是要幹麻的了  ， 下面有個指令 egrep '(Complete|created)' , 這行
# 是指 原程式 跑出來的所有東西  ， 我只要抓  有這個字的 行  放到 twoD_data.txt  這個檔中  
echo   "Case ind1 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode| egrep '(Complete|created)' >> twoD_data.txt
echo    "Case ind1 done \n"

# cmd1 && cmd2 ， 這是cmd1 正確執行才 run cmd2  , 但， 會把前面的data 餵過來嗎
m_pin=10
k_obstacles=43
echo    "\nCase ind2 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case ind2 done \n"

m_pin=10
k_obstacles=50
echo    "\nCase ind3 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case ind3 done \n"

m_pin=25
k_obstacles=79
echo    "\nCase ind4 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case ind4 done \n"

m_pin=33
k_obstacles=71
echo    "\nCase ind5 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case ind5 done \n"

m_pin=10
k_obstacles=10
echo    "\nCase rc1 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc1 done \n"

m_pin=30
k_obstacles=10
echo    "\nCase rc2 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc2 done \n"

m_pin=50
k_obstacles=10
echo    "\nCase rc3 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc3 done \n"

m_pin=70
k_obstacles=10
echo    "\nCase rc4 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc4 done \n"

m_pin=100
k_obstacles=10
echo    "\nCase rc5 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc5 done \n"

m_pin=100
k_obstacles=500
echo    "\nCase rc6 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc6 done \n"

# 這個 我 不小心把plane的大小 弄成  200 X 200 怎麼run的時間  變的差很多 
# 原本是 3.4 sec 變成  24 sec 怎麼會差這麼多  ， 
m_pin=200
k_obstacles=500
echo    "\nCase rc7 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc7 done \n"

# 超過大概600 ， plane 大小好像就不夠用了 , ok 這手動跑的出來
# 那  ， 這個的plane大小要弄成多少 ， 我有點忘了 200 x 200 
# 下面的這個case 沒有跑出 時間哎 ， 我想是obstacle太多 ， 做不出來
sp_width=130
sp_height=130
m_pin=200
k_obstacles=800
echo    "\nCase rc8 \n" >> twoD_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc8 done \n"

#, ok 這手動跑的出來
sp_width=140
sp_height=140
m_pin=200
k_obstacles=1000
echo    "\nCase rc9 \n" >> twoD_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc9 done \n"

m_pin=500
k_obstacles=100
echo    "\nCase rc10 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc10 done \n"

#, ok 這手動跑的出來
m_pin=1000
k_obstacles=100
echo    "\nCase rc11 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc11 done \n"

# 裡面就這個case 最怕了  ， 因為obsatcle 有10000 , 我怕我設定的plane 大小會塞不下去
# 下面這個超大case  , 只要500x500 就可以裝下去了 密度大概是 82吧 , 這就不之道跑不跑的出來了
# 這個 case 的話 ， 我記得很大  我有放在我的mail裡 ， 可以去找看看  
m_pin=1000
k_obstacles=10000
ssp_width=450
ssp_height=450
echo    "\nCase rc12 \n" >> twoD_data.txt
expect spawn_all_examps.sh $ssp_width $ssp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rc12 done \n"

m_pin=10
k_obstacles=500
echo    "\nCase rt1 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rt1 done \n"

m_pin=50
k_obstacles=500
echo    "\nCase rt2 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rt2 done \n"

m_pin=100
k_obstacles=500
echo    "\nCase rt3 \n" >> twoD_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rt3 done \n"

#, ok 這手動跑的出來 , 
sp_width=140
sp_height=140
m_pin=100
k_obstacles=1000
echo    "\nCase rt4 \n" >> twoD_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rt4 done \n"

#, ok 這手動跑的出來 ,
sp_width=140
sp_height=140
m_pin=200
k_obstacles=2000
echo    "\nCase rt5 \n" >> twoD_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_data.txt
echo    "Case rt5 done \n"

# 告夭  下面的 >> 不小心寫成 >  害的我東西 ， 都被它藝住了 ， 按  
echo   "----------End of 2D non_overlap example---------\n" >> twoD_data.txt





