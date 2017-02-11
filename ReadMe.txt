FILES
----------

	Kruskal_Tree.h              : 做Kruskal_Tree 的algorithm
	O_2D_OASG.h  		    : 原作者的algorithm
	edge.h                      : 把edge 分出來當成一個class 來處理
	generate_input.cpp          : 主檔 , 整體的流程跟架構
	level_info.h                : 紀録plane的資訊
	my_non_overlap_OASG.h       : 用我的方法來建non overlap OASG
	my_overlap_OASG.h           : 改成可以處理 overlapping obstacles
	obstacle.h                  : obstacle 的class
	obstacle_avoding_kruskal.h  : 由Kruskal_Tree 在OASG 抓edge 避開obstacle
	path_node.h                 : Routing table 內會用到的class
	rectilinear.h               : 做rectilinearize
	vertex.h                    : vertex class 
	Projection_handler.h        : 用來處理3D下的Projection
	*.sh 			    : 用來跑例子的 
        Makefile                    : 用來編譯我所有的程式

HOW TO USE
----------


1. Compile the Program 

        $ ./Make 

2. Run the Program

        $ ./generate_input 

3. Run gnuplot to get pictures 

        $ gnuplot obstacle_avoding_rectilinear_Kruskal.gnu  


