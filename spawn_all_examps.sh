#/bin/bash
#/usr/bin/expect 

# file=$1
#這裡要寫一個 自動化跑數劇的程式
#可以讓我不用一個個key 要輸入的數值 , 下面這是用來 ， 接
# 呼叫此.sh 時的傳入參數 , 我看看能不能自已定參數
# 哇 太久沒弄這個  突然看不懂 ， 下面的四個  參數  是
# 我用expect  去執行這個檔時 ， 事先畏給它的 資料 
# 以便在後 用  
set plane_width  [lindex $argv 0]
set plane_height [lindex $argv 1]
set obstacles    [lindex $argv 2]
set pins         [lindex $argv 3]
set levels       [lindex $argv 4]
set mode 	 [lindex $argv 5]

# 要一開始就輸入 expect 才能進入 expect的環境 , 告夭又忘了，
# 這個sh  是要打 expect spawn_all_examps.sh 來執行的
spawn ./generate_input

#下面則是 ， 看到什麼樣的選項就餵給他什麼資料去run
# 先跑2D的  , 0  是 Non_Overlap_Mode , 1 是 Overlap_Mode 
# 下面的 問句  好像只要有一個字母錯 就不行
expect "Choice the Mode you want :" { send "$mode\r" }
expect "Input plane_level :" { send "$levels\r" }
expect "Input plane_width :" { send "$plane_width\r" }
expect "Input plane_height:" { send "$plane_height\r" }
expect "How many obstacle you want :" { send "$obstacles\r" }
expect "How many pins you want :"  { send "$pins\r" } 

# 下面這三句 ， 碰不是問句 ，  也就是需要使用者輸入東西的選項 ， 
# 如果 我有加入會發生什麼 ，  , 這樣才會等到全部執行完 ， 不然 
# 下面的東西 不會run完就結束了

# expect " Time spend of of OAST = "  { send "\r" }
#expect "Edges of my_OASG = "  { send "\r" }
expect "Time of O_OASG spend  = "  { send "\r" }
expect "Finish = "  { send "1\r" }

# 我想喔， expect 指令應該是 有其等待的時間， 如果server 太長沒有回應  ， 就會關閉
#expect "Complete routing time by my_OASG = "    ， 還有一個東西 就是連續二個expect 指令應該是
# 它把後者當成是 一個參數了吧  我覺得

#expect "Complete routing time by Author_OASG = "   { send "\r" }

# > twoD_data.txt 怎麼讓接下來的東西都轉到另一個檔內呢 
# echo "abc" >> twoD_data.txt


# 先把一個會用到的東西， 寫在這裡 ， 叫轉向附加
# 就是不會刪掉 ， 指定檔案內的內容 ， 而會加上去
# 符號就是 >> , 而一般的轉向 > 是會清空的 


# expect eof  , 不之道這是幹麻的
