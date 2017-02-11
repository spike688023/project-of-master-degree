#/bin/bash

# expect spawn_all_examps.sh  >> twoD_data.txt
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
mode=1

# 下面的 -ne 是網路上查的 為了可以 讓echo 認識換行字符
echo -ne "----------Here is running 2D non_overlap example---------\n" > twoD_overlap_data.txt
# 我想寫成 傳四個參數 ， 順序分別是 width height obsatcles m_pins 
# 太久沒看  都忘了 有的東西 都忘了是要幹麻的了  ， 下面有個指令 egrep '(Complete|created)' , 這行
# 是指 原程式 跑出來的所有東西  ， 我只要抓  有這個字的 行  放到 twoD_overlap_data.txt  這個檔中  
echo -ne "Case ind1 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case ind1 done \n"

# cmd1 && cmd2 ， 這是cmd1 正確執行才 run cmd2  , 但， 會把前面的data 餵過來嗎
m_pin=10
k_obstacles=43
echo -ne "\nCase ind2 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case ind2 done \n"

m_pin=10
k_obstacles=50
echo -ne "\nCase ind3 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case ind3 done \n"

m_pin=25
k_obstacles=79
echo -ne "\nCase ind4 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case ind4 done \n"

m_pin=33
k_obstacles=71
echo -ne "\nCase ind5 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case ind5 done \n"

m_pin=10
k_obstacles=10
echo -ne "\nCase rc1 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc1 done \n"

m_pin=30
k_obstacles=10
echo -ne "\nCase rc2 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc2 done \n"

m_pin=50
k_obstacles=10
echo -ne "\nCase rc3 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc3 done \n"

m_pin=70
k_obstacles=10
echo -ne "\nCase rc4 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc4 done \n"

m_pin=100
k_obstacles=10
echo -ne "\nCase rc5 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc5 done \n"

# 下面的這個case 沒有跑出 時間哎 ， 我想是obstacle太多 ， 做不出來
sp_width=200
sp_height=200
m_pin=100
k_obstacles=500
echo -ne "\nCase rc6 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc6 done \n"

m_pin=200
k_obstacles=500
echo -ne "\nCase rc7 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc7 done \n"

# 超過大概600 ， plane 大小好像就不夠用了 , ok 這手動跑的出來
m_pin=200
k_obstacles=800
echo -ne "\nCase rc8 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc8 done \n"

#, ok 這手動跑的出來
m_pin=200
k_obstacles=1000
echo -ne "\nCase rc9 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc9 done \n"

m_pin=500
k_obstacles=100
echo -ne "\nCase rc10 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc10 done \n"

#, ok 這手動跑的出來
m_pin=1000
k_obstacles=100
echo -ne "\nCase rc11 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc11 done \n"

# 裡面就這個case 最怕了  ， 因為obsatcle 有10000 , 我怕我設定的plane 大小會塞不下去
# 下面這個超大case  , 只要500x500 就可以裝下去了 密度大概是 82吧 , 這就不之道跑不跑的出來了
# 這個 case 的話 ， 我記得很大  我有放在我的mail裡 ， 可以去找看看  
m_pin=1000
k_obstacles=10000
ssp_width=500
ssp_height=500
echo -ne "\nCase rc12 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $ssp_width $ssp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rc12 done \n"

m_pin=10
k_obstacles=500
echo -ne "\nCase rt1 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rt1 done \n"

m_pin=50
k_obstacles=500
echo -ne "\nCase rt2 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rt2 done \n"

m_pin=100
k_obstacles=500
echo -ne "\nCase rt3 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $width $height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rt3 done \n"

#, ok 這手動跑的出來 , 34 secs , 69 secs
m_pin=100
k_obstacles=1000
echo -ne "\nCase rt4 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rt4 done \n"

#, ok 這手動跑的出來 , My = 32 sec  , Author = 174 sec
m_pin=200
k_obstacles=2000
echo -ne "\nCase rt5 \n" >> twoD_overlap_data.txt
expect spawn_all_examps.sh $sp_width $sp_height $k_obstacles $m_pin $levels $mode | egrep '(Complete|created)' >> twoD_overlap_data.txt
echo -ne "Case rt5 done \n"

echo -ne "----------End of 2D non_overlap example---------\n" > twoD_overlap_data.txt

# 這裡 要run 3D的case  , 而3D的數據  只有十組 ， 所以還好   , 只是要再另外再寫一個  ， 用來跑3D auto input 的
# expect 檔 

echo -ne "----------Here is running 3D non_overlap example---------\n" > ThreeD_overlap_data.txt
m_pin=50
k_obstacles=6
levels=5
echo -ne "Case ind1 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case ind1 done \n"

m_pin=200
k_obstacles=85
levels=6
echo -ne "Case ind2 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case ind2 done \n"


m_pin=250
k_obstacles=13
levels=10
echo -ne "Case ind3 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case ind3 done \n"

m_pin=500
k_obstacles=100
levels=5
echo -ne "Case ind4 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case ind4 done \n"

m_pin=1000
k_obstacles=20
levels=5
echo -ne "Case ind5 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case ind5 done \n"

m_pin=25
k_obstacles=10
levels=10
echo -ne "Case rt1 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case rt1 done \n"

m_pin=100
k_obstacles=20
levels=10
echo -ne "Case rt2 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case rt2 done \n"

m_pin=250
k_obstacles=50
levels=10
echo -ne "Case rt3 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case rt3 done \n"

m_pin=500
k_obstacles=50
levels=10
echo -ne "Case rt4 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case rt4 done \n"

m_pin=1000
k_obstacles=100
levels=5
echo -ne "Case rt5 \n" >> ThreeD_overlap_data.txt
expect spawn_all_examples.sh $width $height $k_obstacles $m_pin $levels | egrep '(Complete|created)' >> ThreeD_overlap_data.txt
echo -ne "Case rt5 done \n"

echo -ne "----------End of 3D non_overlap example---------\n" > ThreeD_overlap_data.txt
