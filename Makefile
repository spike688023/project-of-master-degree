CFlags = -O2 -Wall  
generate_input: generate_input.cpp my_non_overlap_OASG.o my_overlap_OASG.o O_2D_OASG.o kruskal_tree.o obstacle_avoding_kruskal.o  rectilinear.o  Projection_handler.o
#generate_input.o my_non_overlap_OASG.o O_2D_OASG.o kruskal_tree.o obstacle_avoding_kruskal.o  rectilinear.o level_info.o Projection_handler.o	
# 下面的command 是當 上面的depency 的目的標檔 ， 比後面相依的檔 就  ， 執行下面的指令  ， 而下面的指令是shell script 所以前面要 空個tap , 
# 即便你是註解行  ， 前面有放tap ， 也一樣會show在screen 上  
# 這行是我沒用makefile時下的指令 g++ -fopenmp $(CCFlags) -o generate_input  generate_input.cpp 
# make後 為什麼會有一些奇怪的warning 跑出來  ， 是不是我的header 沒有 把 定義給切開的關係 
	g++  -O2 -o generate_input generate_input.cpp  my_non_overlap_OASG.o my_overlap_OASG.o O_2D_OASG.o kruskal_tree.o obstacle_avoding_kruskal.o  rectilinear.o  

#generate_input.o: generate_input.cpp my_non_overlap_OASG.h O_2D_OASG.h kruskal_tree.h obstacle_avoding_kruskal.h  rectilinear.h  Projection_handler.h
#	g++ -fopenmp $(CCFlags)   -o generate_input.o generate_input.cpp

# g++  $(CCFlags)  -c my_non_overlap_OASG.h -o my_non_overlap_OASG.o  我原先是下這個 好像沒什麼差別哎   , -c 這個參數是只做編譯 不做連結  
my_non_overlap_OASG.o:   my_non_overlap_OASG.h vertex.h edge.h obstacle.h level_info.h 
	g++  $(CCFlags)   -o my_non_overlap_OASG.o my_non_overlap_OASG.h

my_overlap_OASG.o:   my_overlap_OASG.h vertex.h edge.h obstacle.h level_info.h 
	g++  $(CCFlags)   -o my_overlap_OASG.o my_overlap_OASG.h

O_2D_OASG.o:  O_2D_OASG.h vertex.h edge.h obstacle.h level_info.h 
	g++ $(CCFlags)  -fopenmp -o O_2D_OASG.o  O_2D_OASG.h

rectilinear.o:  rectilinear.h vertex.h edge.h obstacle.h level_info.h 
	g++ $(CCFlags)   -o rectilinear.o rectilinear.h

Projection_handler.o: Projection_handler.h vertex.h edge.h obstacle.h level_info.h 
	g++ -fopenmp $(CCFlags)   -o Projection_handler.o Projection_handler.h

kruskal_tree.o: kruskal_tree.h vertex.h edge.h level_info.h 
	g++ $(CCFlags)   -o kruskal_tree.o  kruskal_tree.h
  
obstacle_avoding_kruskal.o: obstacle_avoding_kruskal.h vertex.h edge.h obstacle.h level_info.h path_node.h
	g++ -fopenmp $(CCFlags) -o obstacle_avoding_kruskal.o obstacle_avoding_kruskal.h

# 為什麼 下面這個  就沒事呢   , 我又知道新東西了 就是 -o <filename> <inputfile> 後面是要放 需要的檔  ， 讓我試看看 , 好像跟我原本
# 沒什麼大差別哎 
level_info.o: level_info.h vertex.h obstacle.h  
	g++ $(CCFlags)   -o level_info.o level_info.h

clean:
	rm -f *.o generate_input
