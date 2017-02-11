/*
 * =====================================================================================
 *
 *       Filename:  O_2D_OASG.h
 *
 *    Description:  just like what i wirte above  
 *
 *        Version:  1.0
 *        Created:  2011/03/01 12時44分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */


#ifndef        O_2D_OASG_H 
	#define O_2D_OASG_H 	

#include	<iostream>
#include	<fstream>
#include	"edge.h"
#include	"obstacle.h"
#include	"vertex.h"
#include	"level_info.h"
// 載入下面這個 header file  是因為 
// 我可能會要用到 sort_x_position  , sort_y_position 
// 這二個副程式  ， 因為這 二個算是副程式  
// 所以我要載入下面這個  不然會 redefine 
// #include	"my_non_overlap_OASG.h"
#include	<vector>
#include        <cstdio>
#include	<algorithm>
#include	<map>
#include	<cmath>
using namespace std ; 

 class O_2D_OASG{

 private:
 	
	 /*-----------------------------------------------------------------------------
	  *  
 	 那我自已弄一個指標 
	bool (*op_x)();
	bool (*op_y)();
	  *-----------------------------------------------------------------------------*/
 public:
 	O_2D_OASG(){
		
		/*-----------------------------------------------------------------------------
		 *  
		op_x = &O_2D_OASG::sort_x_position_in_OASG ; 
		op_y = &O_2D_OASG::sort_y_position_in_OASG ; 
		幹 ， 完全不行
		 *-----------------------------------------------------------------------------*/
	}
 	// 這裡我想先宣告 ， 下面再放實作 
	//bool (*sort_x_position) (vertex , vertex ) , bool (*sort_y_position)  (vertex , vertex )
	//
 	void build_OASG( vector<edge> &OASG , level_info &my_level_info  ) ; 
 	double build_OASG_fast_version( vector<edge> &OASG , level_info &my_level_info  ) ; 
	// 奇怪了  ， 照著 錯誤 message  去做  ， 弄成static 就可以過了 真不之道為什麼	
	// 是不是物件還沒做出來的關係
	
	/*-----------------------------------------------------------------------------
	 *  
	bool sort_x_position_in_OASG(vertex v1 , vertex v2) ; 
	bool sort_y_position_in_OASG(vertex v1 , vertex v2) ; 
	水 拉 ，  下面 經我這麼一改  ， 大幅減少了  我要傳的參數 很方便哎 
	 *-----------------------------------------------------------------------------*/
	 void print_OASG_gnu( vector<edge> &OASG , level_info &my_level_info );

	
	 /*-----------------------------------------------------------------------------
	  *  下面我要用來可以 處理3D 的  , 先讓它可以處理3D資訊 ， 而且可以處理 projection_vertex
	  *-----------------------------------------------------------------------------*/
 	 double build_3D_OASG_and_projection_vertex( level_info &my_level_info  ) ; 
	 void print_3D_OASG_gnu(  vector<level_info> &every_level_info , vector<edge> &vertical_edge );
};

// 下面這二個新的副程式 ， 也是用來 排序的 內容跟我之前用的是一樣的
// 只是名字不一樣而已  ， 不然會發  ， func redefine 的問題 
bool sort_x_position_in_O_2D_OASG(vertex v1 , vertex v2){
	return ( v1.print_x_position() < v2.print_x_position() );	
}

bool sort_y_position_in_O_2D_OASG(vertex v1 , vertex v2){
	return ( v1.print_y_position() < v2.print_y_position() );	
}

// 要個降幕的排序 
bool sort_y_position_decent_in_O_2D_OASG(vertex v1 , vertex v2){
	return ( v1.print_y_position() > v2.print_y_position() );	
}

bool sort_x_position_for_obstacle(obstacle o1 , obstacle o2){
	return ( o1.print_ld_corner_x() < o2.print_ld_corner_x() );	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  RU_Region_handler
 *  Description:  
 * =====================================================================================
 */
	void
RU_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex , 
		    vector<vertex> &all_pin , vector<obstacle> *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary 
		    , vector<vertex> *pin_on_right_position )
{

	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for(int i = my_level_info.plane_width - 1  ; i >= main_vertex.print_x_position() ; i-- ){

		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_ru_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_ru_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些
	
	/*-----------------------------------------------------------------------------
	 *  
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position 
	// 這裡要 由 右邊界 掃到 main_vertex , 
	for(int i = my_level_info.plane_width - 1  ; i >= main_vertex.print_x_position() ; i-- ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_r_boundary[i][j] ); 
		}//  把碰到 r_boundary 的 obstacle 放進來  

		/*-----------------------------------------------------------------------------
		// 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		// 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		// 如同指向下一個 
		// 我想把下面這塊給改寫  ，  不要用二個loop 只用一個  
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_l_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}// 只會遇到一個相同的 所以不用再掃下去了 
				
			}// scan current_state 內的element 
		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *  
		 *-----------------------------------------------------------------------------*/
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			// 再刪掉之前  ， 先把它給放進來  
			if( i  == current_state[k].print_lu_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&  
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			// if( i  == current_state[k].print_lu_corner_x() ){  , 我想看看 ， 
			// 碰到右邊界 ， 而沒買上把 obstacle拿起來  會不會出錯 
			// 哇   ， 不行 
			if( i  == current_state[k].print_lu_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}// 只會遇到一個相同的 所以不用再掃下去了 
			
		}// scan current_state 內的element 
		
		// 下面這一小塊  ， 我要換方法 去做  ， 我要抓出 二個obstacle ， 放到 vector 然後  用原本的方法 去判斷  
		// 跟main_vertex.y 最近二個上下obstacle  , 如此一來  ， 最內部的那個loop  , 其實只有一二個elements  , 
		// 並不會花上太多的時間 , 下面二個資料  ， A_min_y 是指 >= main_vertex.y 的cases 內   ， 最小的ld.y
		// B_min_y 則是< main_vertex.y 的cases 內 ， 最小的lu.y 
		vector<obstacle> bound_obstacle ; 
		int A_min_y = my_level_info.plane_height , A_index = -1 ; 
		int B_min_y = my_level_info.plane_height , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_ld_corner_y() >= main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() < A_min_y  			   ){
			   	A_min_y = current_state[k].print_ld_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() < B_min_y  			   ){
			   	B_min_y = current_state[k].print_lu_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 

		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){


			if(  i > main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() >= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < pin_on_right_position[i][j].print_y_position()  &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			else if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= pin_on_right_position[i][j].print_y_position()  &&
					    current_state[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 

			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 

		/*-----------------------------------------------------------------------------
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 ix 是 左下角的corner 
			 *-----------------------------------------------------------------------------*/
			if( i > main_vertex.print_x_position() &&  obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ld = false ; 

				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_l_boundary[i][j].print_ld_corner_y() >= main_vertex.print_y_position() ){
				 

					/*-----------------------------------------------------------------------------
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
						    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_ld = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_ld = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_l_boundary[i][j].print_ld_corner_y() < main_vertex.print_y_position() ){
					overlap_or_not_for_ld = true ; 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}
				
				if( overlap_or_not_for_ld == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 
				// 這裡 ， 之所以 要多一個判斷  ， 是  因為 左右邊界剛好落在 main_vertex.print_x_position()  的位置上

				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    current_state[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *  
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}

			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_lu_corner_y() >  main_vertex.print_y_position() && 
									obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
							     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
			}

		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 

		
		// 我少寫一塊 ， r_boundary 的判斷了  
		// 這裡的for loop 是在掃 在這個 x_position 上的 r_boundary 有誰
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 *-----------------------------------------------------------------------------*/
			// 這是右下角點 
			if(  i > main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_rd = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_rd_corner_y() >= main_vertex.print_y_position() ){
				
					/*-----------------------------------------------------------------------------
				 	 下面這塊  ， 好像也能簡化哎 ， current_state 記的是 ， 目前還能擋人的obstacle有那些
					 我只要記著  這些obstacle中  ， obstacle y 軸位置 最小的是誰  ， 就可以了 ， 
					 只要你的位置高於它  ， 那就一定會被擋到 
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
						    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_rd = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_rd = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_r_boundary[i][j].print_rd_corner_y() < main_vertex.print_y_position() ){
					overlap_or_not_for_rd = true ; 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}
				
				if( overlap_or_not_for_rd == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    current_state[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}

			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() >  main_vertex.print_y_position() && 
									obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
							     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  

			}

		}// end of for loop variable j 
			
	}// 掃過所有的x , for loop i 
	
	
	// 再將侯選點 ， 做 塞選 
	// 塞選完後 ， 建立 OASG 要的連線 
	// 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	// 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	// 也就沒有 2 的index 可以放東西  
	// 下面的這個vector 多加個1  即便 點是在邊界了  也沒什麼差  
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	
	// 下面的 i < my_level_info.plane_width 不能有等號 ， 因為 plane_width = 100 , index = 0 ~ 99 
	for(int i = main_vertex.print_x_position() ; i < my_level_info.plane_width  ; i++ ){

		if( candidate_filter[i].size() > 1  ){
			// 這裡確實是要用升序的 
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	// 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	// 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	// 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	// 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	int limits_position = my_level_info.plane_height ; 
	vector<vertex> real_candiate ; 
	for( int i = main_vertex.print_x_position()  ; i < my_level_info.plane_width ; i++ ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 我有依 y 排序過了 ， 所以 y 最小的有過 就會擋後面的
			// 即便沒過  後面的也輪不到 
			if(  candidate_filter[i][0].print_y_position() <= limits_position ){
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() - 1 ; 	
			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 
		

		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() - 1 ;  
		}

		/*-----------------------------------------------------------------------------
		// 下面這個 loop 是要用 obstacle 來限制住 limits_position  , 有點忘了， 下面這塊  是要幹麻的了
		//  哇 ， 不能拿掉   , 我打算拿那個bound_obstacle 來  這塊用  , 為什麼會有下面的情況要擋掉呢
		//  我想降低這塊 
		 for( unsigned int j = 0 ; j < obstacle_cross_main_vertex_y[i].size() ; j++ ){

			if( obstacle_cross_main_vertex_y[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_cross_main_vertex_y[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_cross_main_vertex_y[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() - 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				 *   // 看來沒有這種case會發生  ， 那只要把 上面個東西給 簡化就好了 
				 *      也就是抓出  ， 橫跨 main_vertex.y 的obstacle就可以了
				else if( obstacle_cross_main_y[i][j].print_ld_corner_y() > main_vertex.print_y_position() &&
					 obstacle_cross_main_y[i][j].print_ld_corner_y() <= limits_position ){
					limits_position =  obstacle_cross_main_y[i][j].print_ld_corner_y() - 1 ;  
				}// 為什麼有這個if 呢  ， 忘記是為了什麼  而放進來的了  , 這塊也拿掉看看吧 
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/

	} // 由 main_vertex 往 右邊界掃過去 


	/*-----------------------------------------------------------------------------
	 *    下面是為了 避免有edge 被重複的加入 ， 所加入的check code 
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		

		OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
				      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 

	}// scan all vertex i filtered 


}		/* -----  end of function RU_Region_handler  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LU_Region_handler
 *  Description:  
 * =====================================================================================
 */
	void
LU_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex , 
		    vector<vertex> &all_pin , vector<obstacle>  *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary 
		    , vector<vertex> *pin_on_right_position )
{
	
	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for( int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){

		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_lu_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_lu_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些
	
	/*-----------------------------------------------------------------------------
	 *  
	cout << "In line 528 , prepare for line_sweep " << endl; 
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position  , i 代表掃到那個 x_position 
	for(int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		// 把 obstacle  的 l_boundary 在這 x_position 上的 obstacle 
		// 給抓進來  , 因為是先左往右掃  ， 所以 l_boundary  是加的 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_l_boundary[i][j] ); 
		}//  把碰到 l_boundary 的 obstacle 放進來  

		/*-----------------------------------------------------------------------------
		// 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		// 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		// 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_r_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}
				
			}// scan current_state 內的element 

		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			// 再刪掉之前  ， 先把它給放進來  
			if( i  == current_state[k].print_ru_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_ru_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}// 只會遇到一個相同的 所以不用再掃下去了 
			
		}// scan current_state 內的element 

		vector<obstacle> bound_obstacle ; 
		int A_min_y = my_level_info.plane_height , A_index = -1 ; 
		int B_min_y = my_level_info.plane_height , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_ld_corner_y() >= main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() < A_min_y  			   ){
			   	A_min_y = current_state[k].print_ld_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() < B_min_y  			   ){
			   	B_min_y = current_state[k].print_lu_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 

		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){

			// 因為我 限定了 i < main_vertex.print_x_position() , 所以 不會有 自已跟自已比的情 況 
			if(  i < main_vertex.print_x_position()  && pin_on_right_position[i][j].print_y_position() >= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 
				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < pin_on_right_position[i][j].print_y_position()  &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			// pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() 這行不能有等號 ， 不然 會跟自已比到
			if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 
				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= pin_on_right_position[i][j].print_y_position()  &&
					    current_state[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 
				 
			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 

		/*-----------------------------------------------------------------------------
		cout << "In line 610 " << endl; 
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  哇 ， 這個人的想法 ， 有很多地方有錯 ， 我一開 以為 它不用看 l_boundary 
			 *  的 lu corner 是對的  ， 一般情是沒錯  ， 但 如果 corner_2 是在 平行 main_vertex.y
			 *  的 左邊那個區塊呢 ， 那這 卻實 是要連線的  , 但它 pseudo code 的描述 卻不是這樣 
			 *-----------------------------------------------------------------------------*/


			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 下面這個 ix 進來的單位是 obstacle  即便我第一個 ix 是想處理 corner_ld , 
			 但 corner_lu 也是會進來的 
			 *-----------------------------------------------------------------------------*/
			if( i < main_vertex.print_x_position() &&  obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ld = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				// 下面這個 if 也是要判斷的 , 因為 obstacle 可能會被 main_vertex.x 給切到body 的
				// 中間  , 所以 ld 不會被 rd 給擋到  
				if( obstacle_l_boundary[i][j].print_ld_corner_y() >= main_vertex.print_y_position()  ){
				 
					/*-----------------------------------------------------------------------------
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
						    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_ld = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_ld = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}// ld 在 Region 內
				else{
					overlap_or_not_for_ld = true ; 		
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}// 這當然是 ld 跑到 main_vertex.y 的下面 
				
				if( overlap_or_not_for_ld == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}// 塞入侯選點內 


			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ;  
				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    current_state[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  

				}


			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() && 
									obstacle_l_boundary[i][j].print_lu_corner_y() >  main_vertex.print_y_position() ){
			 
				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
							     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  

			}// end of else if

		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 
		//cout << "In line 681 " << endl; 

		
		// 我少寫一塊 ， r_boundary 的判斷了  
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 *-----------------------------------------------------------------------------*/

			// 這是右下角點 
			if( i < main_vertex.print_x_position() &&  obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_rd = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_rd_corner_y() >= main_vertex.print_y_position() ){
				 
					/*-----------------------------------------------------------------------------
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
						    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_rd = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_rd = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 
				}// 在 Region 內 
				else if( obstacle_r_boundary[i][j].print_rd_corner_y() < main_vertex.print_y_position() ){
					overlap_or_not_for_rd = true ; 	
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}// 在Region 的下方
				
				if( overlap_or_not_for_rd == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ;  
				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    current_state[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}
			 

			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() >  main_vertex.print_y_position() && 
									obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
							     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
			}

		}// end of for loop variable j 
		//cout << "In line 741 " << endl; 
			
	}// 掃過所有的x , for loop i 
	
	//cout << "In line 743 , already select all candidate " << endl ; 	
	// 再將侯選點 ， 做 塞選 
	// 塞選完後 ， 建立 OASG 要的連線 
	// 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	// 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	// 也就沒有 2 的index 可以放東西  
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	
	for(int i = 0 ; i <= main_vertex.print_x_position()   ; i++ ){

		// 這裡要注意 ， 位在上方區塊的 ， 要用升序去排
		// 下方的則用降序去排 
		if( candidate_filter[i].size() > 1  ){
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	// 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	// 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	// 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	// 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	int limits_position = my_level_info.plane_height ; 
	vector<vertex> real_candiate ; 
	for( int i = main_vertex.print_x_position()  ; i >= 0 ; i-- ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 我有依 y 排序過了 ， 所以 y 最小的有過 就會擋後面的
			// 即便沒過  後面的也輪不到 
			if( candidate_filter[i][0].print_y_position() <= limits_position ){
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() - 1 ; 	
			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 

		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() - 1 ;  
		}

		/*-----------------------------------------------------------------------------
		 for( unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			if( obstacle_r_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_r_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_r_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() - 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_r_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() &&
					 obstacle_r_boundary[i][j].print_ld_corner_y() <= limits_position ){
					limits_position =  obstacle_r_boundary[i][j].print_ld_corner_y() - 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/

	} // 由 main_vertex 往 l_boundary 掃過去 
	
	/*-----------------------------------------------------------------------------
	 * 	Check code , repeat edge  
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 
	
	//cout << "End of LU_Region_handler " << endl ; 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
				      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 

	}// scan all vertex i filtered 

}		/* -----  end of function LU_Region_handler  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  RD_Region_handler
 *  Description:  
 * =====================================================================================
 */
	void
RD_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex , 
		    vector<vertex> &all_pin , vector<obstacle>  *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary 
		    , vector<vertex> *pin_on_right_position )
{

	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for(  int i = my_level_info.plane_width - 1  ; i >=  main_vertex.print_x_position() ; i-- ){
		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_rd_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_rd_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些
	

	/*-----------------------------------------------------------------------------
	 *  
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 上面 應該是做好前置的處理 ， 會要微調的地方 ， 應該是下面這塊 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position 
	// 由 最右邊掃到 main_vertex ， 抓好 candidate 再由main_vertex 掃回去 
	for(int i = my_level_info.plane_width - 1  ; i >=  main_vertex.print_x_position() ; i-- ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_r_boundary[i][j] ); 
		}//  把碰到 l_boundary 的 obstacle 放進來  

		/*-----------------------------------------------------------------------------
		// 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		// 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		// 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_l_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  , 哎  想一想  好像又不用減1哎
					// begin 指向第一個元素 , k = 0 , 
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}
				
			}// scan current_state 內的element 
		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			if( i  == current_state[k].print_lu_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_lu_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}// 只會遇到一個相同的 所以不用再掃下去了 
			
		}// scan current_state 內的element 

		vector<obstacle> bound_obstacle ; 
		int A_max_y = 0 , A_index = -1 ; 
		int B_max_y = 0 , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() > A_max_y  			   ){
			   	A_max_y = current_state[k].print_lu_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_lu_corner_y() > main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() > B_max_y  			   ){
			   	B_max_y = current_state[k].print_ld_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 
		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){

			if( i > main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() <= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position()  		      &&
					    current_state[k].print_ru_corner_y() > pin_on_right_position[i][j].print_y_position()     ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position()  		      &&
					    bound_obstacle[k].print_ru_corner_y() > pin_on_right_position[i][j].print_y_position()     ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			else if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position()  		      &&
					    current_state[k].print_rd_corner_y() >= pin_on_right_position[i][j].print_y_position()     ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position()  		      &&
					    bound_obstacle[k].print_rd_corner_y() >= pin_on_right_position[i][j].print_y_position()     ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 

			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 
		/*-----------------------------------------------------------------------------
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 *-----------------------------------------------------------------------------*/
			if( i > main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_lu = false ; 


				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				 如果 lu 在 Region 內 又沒被擋 代表 可以連 
				if( obstacle_l_boundary[i][j].print_lu_corner_y() <= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    current_state[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
							overlap_or_not_for_lu = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
				}// 在Region 內 ， 要看看 有沒有被其它obstacle 擋到 
				 *-----------------------------------------------------------------------------*/
				if( obstacle_l_boundary[i][j].print_lu_corner_y() <= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    bound_obstacle[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
							overlap_or_not_for_lu = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}// 在Region 內 ， 要看看 有沒有被其它obstacle 擋到 
				else if( obstacle_l_boundary[i][j].print_lu_corner_y() > main_vertex.print_y_position() ){
					overlap_or_not_for_lu = true ;  
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}// 其它 ， 便是 lu 在 main_vertex.y 的上面 , 便直接給 true  , 
				
				if( overlap_or_not_for_lu == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <  main_vertex.print_y_position() &&
									obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
							     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_lu_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    current_state[k].print_rd_corner_y() >= obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    bound_obstacle[k].print_rd_corner_y() >= obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}

			 

			}

		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 


		// 我少寫一塊 ， r_boundary 的判斷了  
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 *-----------------------------------------------------------------------------*/

			if( i > main_vertex.print_x_position() &&  obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ru = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_ru_corner_y() <= main_vertex.print_y_position() ){
				 

					/*-----------------------------------------------------------------------------
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() <  main_vertex.print_y_position() 		     &&
						    current_state[k].print_ru_corner_y() >  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
							overlap_or_not_for_ru = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() <  main_vertex.print_y_position() 		     &&
						    bound_obstacle[k].print_ru_corner_y() >  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
							overlap_or_not_for_ru = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 
					
				}
				else if( obstacle_r_boundary[i][j].print_ru_corner_y() > main_vertex.print_y_position() ){
					overlap_or_not_for_ru = true ;  
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				 
				}
				
				if( overlap_or_not_for_ru == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() <  main_vertex.print_y_position() &&
									obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
							     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  

			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <=  main_vertex.print_y_position() 		     &&
					    current_state[k].print_rd_corner_y() >=  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <=  main_vertex.print_y_position() 		     &&
					    bound_obstacle[k].print_rd_corner_y() >=  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}


			}


		}// 因為是 r_boundary  所以在這個 Region , 二個corner 都要看 

	}// 掃過所有的x , for loop i 
	
	
	// 再將侯選點 ， 做 塞選 
	// 塞選完後 ， 建立 OASG 要的連線 
	// 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	// 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	// 也就沒有 2 的index 可以放東西  
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	// 哇  ， 該不會是錯這吧  ， plane_width = 100 , 所以 我只能跑到 99 
	for(int i = main_vertex.print_x_position() ; i <  my_level_info.plane_width    ; i++ ){
		if( candidate_filter[i].size() > 1  ){
			// 因為是 LD 所以 要用降的排 
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_decent_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	// 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	// 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	// 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	// 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	// 因為是 LD_REgion 所以 main_vertex  本身 就是最高的位置了  
	int limits_position = 0 ; 
	vector<vertex> real_candiate ; 
	// 從 main_vertex 往左掃 ， 抓出 真正要連的 ， 因該是不用 main_vertex.print_x_position + 1 的吧 
	// 這是  RD_Region_handler 
	// 由右往左掃 ， 還是要 記下 目前有那些 obstacle 擋到  , 因為不想再 弄一個新的 vector 出來
	// 所以直接用 把之前的 vector  給 clear 就好了 
	for( int i = main_vertex.print_x_position()  ; i < my_level_info.plane_width ; i++ ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 main_vertex 在這個 Region  是最大的y ， 所以 
			// 這裡的 candidate_filter  的y 被降排了  ， 了 所以  第一個y 會最這行裡 最大的 
			if(  candidate_filter[i][0].print_y_position() >= limits_position ){
				// 第一塊先連線 
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() + 1 ; 	


			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 

		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() + 1 ;  
		}

		/*-----------------------------------------------------------------------------
		 *   
		 如果不用 老師的方法 ， 而是照我自已想的方法
		 也就是  多觀察一個東西 ， 就是 掃回去 遇到的boundary
		 用來 調整 limits_position , 那 是要先連線呢 還是 先更新 
		 boundary  應是先連線 ， 再更新 limits_position 
		 因為這是 RD_Region_handler , 回頭是 左往右掃  ， 所以只要看 l_boundary
		 來更新 limits_position  , 我只要注意obstacle 的最小 y 跟最大 y 即可 
		 如果 最大y 在 Region 內 ， 且 比 limits_position 大 就更新 
		 另一case  如果 最大y  剛好在 main_vertex 的 y  , 代表 後面的連線 跟這個
		 main_vertex 沒有關係了 ， 所以 ， limits_position = main_vertex.y + 1 , 
		 使得跟後面的點 都連不到 
		 for( unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			if( obstacle_l_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_l_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_l_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() + 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_l_boundary[i][j].print_lu_corner_y() < main_vertex.print_y_position() &&
					 obstacle_l_boundary[i][j].print_lu_corner_y() >= limits_position ){
					limits_position =  obstacle_l_boundary[i][j].print_lu_corner_y() + 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/


	} // 由 main_vertex 往 l_boundary 掃過去 

	/*-----------------------------------------------------------------------------
	 *  		Check code 
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
				      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 

	}// scan all vertex i filtered 

}		/* -----  end of function RD_Region_handler  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LD_Region_handler
 *  Description:  基本上呢  ， level_info 裡面 就已經有 所有的 obstacle 及pin 
 *  		  跟各種我要的資訊了 
 * =====================================================================================
 */
	void
LD_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex , 
		    vector<vertex> &all_pin , vector<obstacle> *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary 
		    , vector<vertex> *pin_on_right_position )
{

	
	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for(  int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){
		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_ld_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_ld_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些

	/*-----------------------------------------------------------------------------
	 *  
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 上面 應該是做好前置的處理 ， 會要微調的地方 ， 應該是下面這塊 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position 
	for(int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_l_boundary[i][j] ); 
		}//  把碰到 l_boundary 的 obstacle 放進來  

		/*-----------------------------------------------------------------------------
		// 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		// 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		// 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){
			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_r_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  , 哎  想一想  好像又不用減1哎
					// begin 指向第一個元素 , k = 0 , 
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}
				
			}// scan current_state 內的element 
		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			if( i  == current_state[k].print_ru_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_ru_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}// 只會遇到一個相同的 所以不用再掃下去了 
			
		}// scan current_state 內的element 

		vector<obstacle> bound_obstacle ; 
		int A_max_y = 0 , A_index = -1 ; 
		int B_max_y = 0 , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() > A_max_y  			   ){
			   	A_max_y = current_state[k].print_lu_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_lu_corner_y() > main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() > B_max_y  			   ){
			   	B_max_y = current_state[k].print_ld_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 
		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){


			if( i < main_vertex.print_x_position() &&  pin_on_right_position[i][j].print_y_position() <= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position()  			&&
					    current_state[k].print_ru_corner_y() > pin_on_right_position[i][j].print_y_position()       ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position()  			&&
					    bound_obstacle[k].print_ru_corner_y() > pin_on_right_position[i][j].print_y_position()       ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			// pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() 這行不能有等號 ， 不然 會跟自已比到
			if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				 有哎 ， 我有送他一個break 還那麼慢  
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position()  			&&
					    current_state[k].print_rd_corner_y() >= pin_on_right_position[i][j].print_y_position()       ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position()  			&&
					    bound_obstacle[k].print_rd_corner_y() >= pin_on_right_position[i][j].print_y_position()       ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  )  ) ;  
				}// 塞入侯選點內 
				 
			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 
		/*-----------------------------------------------------------------------------
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 *-----------------------------------------------------------------------------*/
			if(  i < main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_lu = false ; 

				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_l_boundary[i][j].print_lu_corner_y() <= main_vertex.print_y_position() ){
				 
					/*-----------------------------------------------------------------------------
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    current_state[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
							overlap_or_not_for_lu = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    bound_obstacle[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
							overlap_or_not_for_lu = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_l_boundary[i][j].print_lu_corner_y() > main_vertex.print_y_position() ){
					overlap_or_not_for_lu = true ; 	
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}
				
				if( overlap_or_not_for_lu == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <  main_vertex.print_y_position() && 
									obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
							     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  

			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_lu_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    current_state[k].print_rd_corner_y() >= obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    bound_obstacle[k].print_rd_corner_y() >= obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}

			}


		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 

		
		// 我少寫一塊 ， r_boundary 的判斷了  
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 下面的第一個 if 是 權限最寬的 ， rd <= main_vertex.y 代表 ru 可以在 main_vertex.y 的上面
			 也可以在下面  
			 *-----------------------------------------------------------------------------*/

			if( i < main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ru = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_ru_corner_y() <= main_vertex.print_y_position() ){
				 
					/*-----------------------------------------------------------------------------
					for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
						if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    current_state[k].print_ru_corner_y() > obstacle_r_boundary[i][j].print_ru_corner_y()        ){
							overlap_or_not_for_ru = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all current_state's obstacle 
					 *-----------------------------------------------------------------------------*/
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    bound_obstacle[k].print_ru_corner_y() > obstacle_r_boundary[i][j].print_ru_corner_y()        ){
							overlap_or_not_for_ru = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 
				}
				else if( obstacle_r_boundary[i][j].print_ru_corner_y() > main_vertex.print_y_position() ){

					overlap_or_not_for_ru = true ; 	
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}
				if( overlap_or_not_for_ru == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() <  main_vertex.print_y_position() &&
									obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
							     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  

			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    current_state[k].print_rd_corner_y() >= obstacle_r_boundary[i][j].print_ru_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    bound_obstacle[k].print_rd_corner_y() >= obstacle_r_boundary[i][j].print_ru_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false  ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}

			}


		}// 因為是 r_boundary  所以在這個 Region , 二個corner 都要看 

	}// 掃過所有的x , for loop i 
	
	
	// 再將侯選點 ， 做 塞選 
	// 塞選完後 ， 建立 OASG 要的連線 
	// 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	// 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	// 也就沒有 2 的index 可以放東西  
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	
	for(int i = 0 ; i < main_vertex.print_x_position() + 1  ; i++ ){

		if( candidate_filter[i].size() > 1  ){
			// 因為是 LD 所以 要用降的排 
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_decent_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	// 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	// 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	// 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	// 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	// 因為是 LD_REgion 所以 main_vertex  本身 就是最高的位置了  
	int limits_position = 0 ; 
	vector<vertex> real_candiate ; 
	// 從 main_vertex 往左掃 ， 抓出 真正要連的 ， 因該是不用 main_vertex.print_x_position + 1 的吧 
	for( int i = main_vertex.print_x_position()  ; i >= 0 ; i-- ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 main_vertex 在這個 Region  是最大的y ， 所以 
			// 這裡的 candidate_filter  的y 被降排了  ， 了 所以  第一個y 會最這行裡 最大的 
			if( candidate_filter[i][0].print_y_position() >= limits_position ){
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() + 1 ; 	
			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 
		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() + 1 ;  
		}

		/*-----------------------------------------------------------------------------
		 for( unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			if( obstacle_r_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_r_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_r_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() + 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_r_boundary[i][j].print_ld_corner_y() < main_vertex.print_y_position() &&
					 obstacle_r_boundary[i][j].print_ld_corner_y() >= limits_position ){
					limits_position =  obstacle_r_boundary[i][j].print_ld_corner_y() + 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/

	} // 由 main_vertex 往 l_boundary 掃過去 
	
	/*-----------------------------------------------------------------------------
	 *     Check Code 
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , 
				      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() )  ); 	 

	}// scan all vertex i filtered 

}		/* -----  end of function LD_Region_handler  ----- */




/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  O_2D_OASG::build_OASG
 *  Description:  
 * =====================================================================================
 */
//不之道 func 傳的參數  可不可以拿func 當參數去傳
// 找到一個大概的解決方法了  叫function pointer 
//bool (*sort_x_position) (vertex , vertex )  , bool (*sort_y_position)  (vertex , vertex)
void O_2D_OASG::build_OASG( vector<edge> &OASG , level_info &my_level_info ){	

	/*-----------------------------------------------------------------------------
	 先準備好要的資料 ,  先由 array_vertex 中 ， 把 pin 抽出來
	 再 用個新的 vector 把 complete_obstacle_information 存過來 
	 轉過來的這些新資料 ， 記得 對 x_position 做排序 
	 pin 不用重新給id 但 obstacle 要 
	  好在 我都有寫 copy constructor 
	  下面的一些排序 使得 我要在這個class 內 
	  加入二個sort 用的副程式 ， 我想到了一個 好的解決方法 
	  就是 vertex 的sort 就交給 vertex  我只要把 vector 傳過去就可以了
	  這樣  二個額外的比較副程式 ， 我只要在vertex 那裡寫就好了  其它地方便不用再加了  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> all_pin ; 
	for(int i = 0 ; i < my_level_info.total ; i++ ){

		if( my_level_info.array_vertex[i].Is_pin_or_not() == true  ){
			all_pin.push_back(  my_level_info.array_vertex[i]  );	 
		}

	}// 把裡面的 pin 點 拉出來 
	
	vector<obstacle> relocate_id_complete_obstacle_info ; 
	// 這個 obstacle的東西 ， 可能是必要的 ， 因為它是要對 其 x 做排序 
	for(unsigned int i = 0 ; i < my_level_info.complete_obstacle_information.size() ; i++ ){
		relocate_id_complete_obstacle_info.push_back(  my_level_info.complete_obstacle_information[i]  ); 
	}// 把 obstacle 給轉移 過來
	
	// 好 上面把東西都給轉好了  ， 接下來 要做排序的動作  ， all_pin 要依 x_position 由小排到大
	// obstacle  也是 且要給 新的 id 
	sort( all_pin.begin() , all_pin.end() , sort_x_position_in_O_2D_OASG );	
	sort( relocate_id_complete_obstacle_info.begin() , relocate_id_complete_obstacle_info.end() , sort_x_position_for_obstacle );

	
	for(unsigned int i = 0 ; i < relocate_id_complete_obstacle_info.size() ; i++ ){
		// 我打算從1 開始算 ， 所以  i + 1 
		relocate_id_complete_obstacle_info[i].set_obstacle_id( i + 1 ) ; 	 
	}// 依x_position 的大小 ， 排序  且  依 id 來看 

	// 我想到要怎麼寫這塊核心了  ， 我把 抓出來的pin 點 ， 依plane_width , 放到各自對應的x 
	// 這樣便方便多了  ， 對 obstacle 來說的話 ， 用三個 obstacle vector  也是以 x_position 
	// 來區分 ， 但我分 三種 l_boundary , r_boundary , current_state , 這三個來存obstacle 
	// 我想呢 大多只要看其 obstacle_id 便可以大概之道了  , line_sweep 我只要用個for loop 
	// 來控制 ， 就可以之道 目前掃到那一行了 
	vector<vertex> pin_on_right_position[my_level_info.plane_width] ;	
	for( unsigned int i = 0 ; i < all_pin.size() ; i++ ){

		pin_on_right_position[ all_pin[i].print_x_position() ].push_back(  all_pin[i]   );	

	}// 把所有的pin 點 ， 依照 其所在的x 位置 ， 分別 放到相對應的vector中
	

	/*-----------------------------------------------------------------------------
	 看來  我要把那四個付責處理的程式 ， 寫成副程式 ， 放在上面
	 這個當主程式 來呼叫   先把 架構給弄好  
	 這裡的架構  主要是  分辦  這個 vertex 是pin 及 obstacle_corner 的那一個
	 因為  不同的  有些處理器  它是不用被丟進去的 
	 像 corner 1 就不用處理 Region 1 的連線 

	 第三個參數 是主要畫分 Region 的 vertex 
	 *-----------------------------------------------------------------------------*/
	// 我之前 要掃 過 array_vertex 是用什麼去掃的丫 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( my_level_info.array_vertex[i].Is_pin_or_not() == true ){

	
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			 *-----------------------------------------------------------------------------*/

		}
		// 下面的4 個 corner  , 分別只要處理 3 個 Region  ， 因為有一個  跟本不用去理會的 
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 1 ){
			
			/*-----------------------------------------------------------------------------
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			 *-----------------------------------------------------------------------------*/

		}// end of corner 1 , 左下角點 
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 2 ){
			
			/*-----------------------------------------------------------------------------
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			 *  
			 *-----------------------------------------------------------------------------*/

		}// end of corner 2 , 左上角點
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 3 ){
			 
			/*-----------------------------------------------------------------------------
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			 *  
			 *-----------------------------------------------------------------------------*/

		}// end of corner 3  , 右上角點
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 4 ){
			 
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   relocate_id_complete_obstacle_info ,  pin_on_right_position);						 
			 *-----------------------------------------------------------------------------*/

		}// end of corner 4  , obstacle 的右下角點

	}// end of for i 


	/*-----------------------------------------------------------------------------
	 *     如果  我把 edge 比對的code 放在這裡 ，  那我routing 所花的時間  ， 會變多嗎
	 *     因為 ， 我現在是把  edge重複的比對  ， 放在那四個Region 的最後面那裡  
	 *     要休改一下
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i = 0 ; i <  OASG.size() ; i++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int j = i + 1 ; j < OASG.size() ; j++ ){

			if( OASG[i].print_node1_x() == OASG[j].print_node1_x() &&
			    OASG[i].print_node1_y() == OASG[j].print_node1_y() && 
			    OASG[i].print_node2_x() == OASG[j].print_node2_x() && 
			    OASG[i].print_node2_y() == OASG[j].print_node2_y() ){
				already_have = true ;  
			}
			else if( OASG[i].print_node1_x() == OASG[j].print_node2_x() &&
			         OASG[i].print_node1_y() == OASG[j].print_node2_y() && 
			         OASG[i].print_node2_x() == OASG[j].print_node1_x() && 
			         OASG[i].print_node2_y() == OASG[j].print_node1_y() ){
					already_have = true ;  
					// 應該就是錯這裡了 ， 我這裡放了個 break ,  所以才沒有跑下面的
					// 刪edge的動作 ， 難怪 ， edge的數目會是  正確的2倍 
			}

			if( already_have == true ){
				// 這裡要 ， 把重復的刪掉 ， 有寫過的東西， 要再寫就很簡單
				// 想不到後面edge 的重複 ， 不只有一個 ,  可能還會對到二，三個
				OASG.erase( OASG.begin() + j ); 
				j-- ; 
				already_have = false ; 
			}// 這樣才把 edge 給加進去丫 

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  


	}// scan all vertex i filtered 

}// end of build_OASG

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  O_2D_OASG::build_OASG_fast_version
 *  Description:   這個  是要重寫 原本的那個版本 ， 基本上  的架構是不變的
 *  		   也是分成四個區塊去做  ， 只是我要加速它  ， 它有點慢  ， 
 *  		   把各個區塊 在裡面  以及會碰到 像限線的  把它給都區分開來
 *  		   也就是 把會用到的obstacle pin ， 分在一堆  ， 放到會用的那個
 *  		   處理器裡  , 這就不用 每次都掃過所有的pin obstacle 
 * =====================================================================================
 */
double O_2D_OASG::build_OASG_fast_version( vector<edge> &OASG , level_info &my_level_info ){	

	/*-----------------------------------------------------------------------------
	 先準備好要的資料 ,  先由 array_vertex 中 ， 把 pin 抽出來
	 再 用個新的 vector 把 complete_obstacle_information 存過來 
	 轉過來的這些新資料 ， 記得 對 x_position 做排序 
	 pin 不用重新給id 但 obstacle 要 
	  好在 我都有寫 copy constructor 
	  下面的一些排序 使得 我要在這個class 內 
	  加入二個sort 用的副程式 ， 我想到了一個 好的解決方法 
	  就是 vertex 的sort 就交給 vertex  我只要把 vector 傳過去就可以了
	  這樣  二個額外的比較副程式 ， 我只要在vertex 那裡寫就好了  其它地方便不用再加了  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> all_pin ; 
	for(int i = 0 ; i < my_level_info.total ; i++ ){

		if( my_level_info.array_vertex[i].Is_pin_or_not() == true  ){
			all_pin.push_back(  my_level_info.array_vertex[i]  );	 
		}

	}// 把裡面的 pin 點 拉出來 
	
	vector<obstacle> relocate_id_complete_obstacle_info ; 
	// 這個 obstacle的東西 ， 可能是必要的 ， 因為它是要對 其 x 做排序 
	for(unsigned int i = 0 ; i < my_level_info.complete_obstacle_information.size() ; i++ ){
		relocate_id_complete_obstacle_info.push_back(  my_level_info.complete_obstacle_information[i]  ); 
	}// 把 obstacle 給轉移 過來
	
	// 好 上面把東西都給轉好了  ， 接下來 要做排序的動作  ， all_pin 要依 x_position 由小排到大
	// obstacle  也是 且要給 新的 id   , 丕排也沒有關係丫  
	// sort( all_pin.begin() , all_pin.end() , sort_x_position_in_O_2D_OASG );	
	// sort( relocate_id_complete_obstacle_info.begin() , relocate_id_complete_obstacle_info.end() , sort_x_position_for_obstacle );

	
	for(unsigned int i = 0 ; i < relocate_id_complete_obstacle_info.size() ; i++ ){
		// 我打算從1 開始算 ， 所以  i + 1 
		relocate_id_complete_obstacle_info[i].set_obstacle_id( i + 1 ) ; 	 
	}// 依x_position 的大小 ， 排序  且  依 id 來看 

	// 我想到要怎麼寫這塊核心了  ， 我把 抓出來的pin 點 ， 依plane_width , 放到各自對應的x 
	// 這樣便方便多了  ， 對 obstacle 來說的話 ， 用三個 obstacle vector  也是以 x_position 
	// 來區分 ， 但我分 三種 l_boundary , r_boundary , current_state , 這三個來存obstacle 
	// 我想呢 大多只要看其 obstacle_id 便可以大概之道了  , line_sweep 我只要用個for loop 
	// 來控制 ， 就可以之道 目前掃到那一行了 
	vector<vertex> pin_on_right_position[my_level_info.plane_width] ;	
	for( unsigned int i = 0 ; i < all_pin.size() ; i++ ){

		pin_on_right_position[ all_pin[i].print_x_position() ].push_back(  all_pin[i]   );	

	}// 把所有的pin 點 ， 依照 其所在的x 位置 ， 分別 放到相對應的vector中
	

	// 我打算  ， 再外面分   ， 這些obstacle  不必說  ， 每次  跑一個點 ， 就要在外面  ， 
	// 掃過所有的obstacle 
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	for( unsigned int i = 0 ; i < relocate_id_complete_obstacle_info.size() ; i++ ){
		 
		 obstacle_l_boundary[ relocate_id_complete_obstacle_info[i].print_ld_corner_x() ].push_back(  relocate_id_complete_obstacle_info[i]  );
		 obstacle_r_boundary[ relocate_id_complete_obstacle_info[i].print_ru_corner_x() ].push_back(  relocate_id_complete_obstacle_info[i]  );

	}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

	/*-----------------------------------------------------------------------------
	 看來  我要把那四個付責處理的程式 ， 寫成副程式 ， 放在上面
	 這個當主程式 來呼叫   先把 架構給弄好  
	 這裡的架構  主要是  分辦  這個 vertex 是pin 及 obstacle_corner 的那一個
	 因為  不同的  有些處理器  它是不用被丟進去的 
	 像 corner 1 就不用處理 Region 1 的連線 

	 第三個參數 是主要畫分 Region 的 vertex 
	 *-----------------------------------------------------------------------------*/
	// 我之前 要掃 過 array_vertex 是用什麼去掃的丫 
	for( int i = 0 ; i < my_level_info.total ; i++ ){



		if( my_level_info.array_vertex[i].Is_pin_or_not() == true ){

			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 

		}
		// 下面的4 個 corner  , 分別只要處理 3 個 Region  ， 因為有一個  跟本不用去理會的 
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 1 ){
			
			/*-----------------------------------------------------------------------------
			 *  
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 


		}// end of corner 1 , 左下角點 
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 2 ){
			
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 

		}// end of corner 2 , 左上角點
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 3 ){
			 
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/

			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 

		}// end of corner 3  , 右上角點
		else if( my_level_info.array_vertex[i].Is_pin_or_not() == false && my_level_info.array_vertex[i].print_corner_id() == 4 ){
			 
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/
			RU_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			RD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 
			LD_Region_handler( OASG , my_level_info , my_level_info.array_vertex[i] , all_pin , 
					   obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position);						 

		}// end of corner 4  , obstacle 的右下角點

	}// end of for i 


	/*-----------------------------------------------------------------------------
	 *     如果  我把 edge 比對的code 放在這裡 ，  那我routing 所花的時間  ， 會變多嗎
	 *     因為 ， 我現在是把  edge重複的比對  ， 放在那四個Region 的最後面那裡  
	 *     要休改一下 , 會不會是下面這塊  花掉太多時間呢 
	 *-----------------------------------------------------------------------------*/
	clock_t start , end ; 
	double dif   ; 
	start = clock() ; 
	for(unsigned int i = 0 ; i <  OASG.size() ; i++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int j = i + 1 ; j < OASG.size() ; j++ ){

			if( OASG[i].print_node1_x() == OASG[j].print_node1_x() &&
			    OASG[i].print_node1_y() == OASG[j].print_node1_y() && 
			    OASG[i].print_node2_x() == OASG[j].print_node2_x() && 
			    OASG[i].print_node2_y() == OASG[j].print_node2_y() ){
				already_have = true ;  
			}
			else if( OASG[i].print_node1_x() == OASG[j].print_node2_x() &&
			         OASG[i].print_node1_y() == OASG[j].print_node2_y() && 
			         OASG[i].print_node2_x() == OASG[j].print_node1_x() && 
			         OASG[i].print_node2_y() == OASG[j].print_node1_y() ){
					already_have = true ;  
					// 應該就是錯這裡了 ， 我這裡放了個 break ,  所以才沒有跑下面的
					// 刪edge的動作 ， 難怪 ， edge的數目會是  正確的2倍 
			}

			if( already_have == true ){
				// 這裡要 ， 把重復的刪掉 ， 有寫過的東西， 要再寫就很簡單
				// 想不到後面edge 的重複 ， 不只有一個 ,  可能還會對到二，三個
				OASG.erase( OASG.begin() + j ); 
				j-- ; 
				already_have = false ; 
			}// 這樣才把 edge 給加進去丫 

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

	}// scan all vertex i filtered 
	end = clock() ; 
	dif = (double) ( end - start ) ; 
	return dif ; 	
	// 下面的輸出說明了  ， 刪edge真是花了不少的時間  
	//cout << "Delete the same edge , time spend is = " << (double) ( dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;

}// end of build_OASG_fast_version

// 告夭 ， 原來 我傳錯參數過來 ， 所以  我一開始是傳 vector<level_info> &my_level_info 所以
// 裡面一直抓不到我要的東西
void O_2D_OASG::print_OASG_gnu(  vector<edge> &OASG , level_info &my_level_info ){
	
	// 這裡先寫印出OASG 的腳本好了　
	ofstream oasgcout("O_2D_OASG.gnu");
	oasgcout << " set title \"O_2D_OASG\" " << endl ; 
	oasgcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	oasgcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	oasgcout << "set xlabel \"X-axis\"" << endl ; 
	oasgcout << "set ylabel \"Y-axis\"" << endl ; 
	oasgcout << "set output " << endl ; 
	oasgcout << "set terminal X11 " << endl ; 
	//oasgcout << "set terminal postscript color" << endl ; 
	//oasgcout << "set output \"O_2D_OASG.ps\"" << endl ; 
	oasgcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 
	// 這裡要讀obstacle 出來　　，　寫在PlaneData.gnu 裡面
	oasgcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( !( my_level_info.array_vertex[i].Is_pin_or_not() )    )
		{

			if( my_level_info.array_vertex[i].print_corner_id() == 1){
				oasgcout << "set object "<< my_level_info.array_vertex[i].print_obstacle_id() <<" rect from " ; 
				my_level_info.array_vertex[i].print_obstacle_for_script(oasgcout )  ;
				oasgcout << "to " ;	
			}
			else if( my_level_info.array_vertex[i].print_corner_id() == 3){
				my_level_info.array_vertex[i].print_obstacle_for_script(oasgcout ) ;
				oasgcout << endl ; 
			}

		}
	}
	//那這裡要印出　OASG 內的所有line 
	oasgcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < OASG.size() ; i++){
		OASG[i].print_edge_information_for_gnu( oasgcout , i + 1 );
	}
	// PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　　
	oasgcout << "plot \"PlaneData.txt\" w lp pt 7 lc rgb \"black\" " << endl ; 
	oasgcout << "pause -1 " << endl ;
	oasgcout.close() ;

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ThreeD_RU_Region_handler
 *  Description:  already modify 
 * =====================================================================================
 */
	void
ThreeD_RU_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex , 
			   vector<vertex> &all_pin , vector<obstacle> *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary
			   , vector<vertex> *pin_on_right_position )
{

	
	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for(int i = my_level_info.plane_width - 1  ; i >= main_vertex.print_x_position() ; i-- ){

		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_ru_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_ru_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些



	/*-----------------------------------------------------------------------------
	 *  
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position 
	// 這裡要 由 右邊界 掃到 main_vertex , 
	for(int i = my_level_info.plane_width - 1  ; i >= main_vertex.print_x_position() ; i-- ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的r_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_r_boundary[i][j] ); 
		}//  把碰到 r_boundary 的 obstacle 放進來  

		/*-----------------------------------------------------------------------------
		// 接下來  ， 便是 遇到 l_boundary 時 ， 要做 刪的動作 
		// 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		// 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_l_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}// 只會遇到一個相同的 所以不用再掃下去了 
				
			}// scan current_state 內的element 
		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *  
		 *-----------------------------------------------------------------------------*/
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			// 再刪掉之前  ， 先把它給放進來  
			if( i  == current_state[k].print_lu_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_lu_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}// 只會遇到一個相同的 所以不用再掃下去了 
			
		}// scan current_state 內的element 

		vector<obstacle> bound_obstacle ; 
		int A_min_y = my_level_info.plane_height , A_index = -1 ; 
		int B_min_y = my_level_info.plane_height , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_ld_corner_y() >= main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() < A_min_y  			   ){
			   	A_min_y = current_state[k].print_ld_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() < B_min_y  			   ){
			   	B_min_y = current_state[k].print_lu_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 

		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){


			if(  i > main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() >= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     my_level_info.which_level 			     )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			else if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     my_level_info.which_level 			     )  ) ;  
				}// 塞入侯選點內 

			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 

		/*-----------------------------------------------------------------------------
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 ix 是 左下角的corner 
			 *-----------------------------------------------------------------------------*/
			if( i > main_vertex.print_x_position() &&  obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ld = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_l_boundary[i][j].print_ld_corner_y() >= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_ld = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_l_boundary[i][j].print_ld_corner_y() < main_vertex.print_y_position() ){
					overlap_or_not_for_ld = true ; 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}
				
				if( overlap_or_not_for_ld == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() ,
								     my_level_info.which_level 	                   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}

				 *-----------------------------------------------------------------------------*/
			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_lu_corner_y() >  main_vertex.print_y_position() && 
									obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
							     obstacle_l_boundary[i][j].print_lu_corner_y() ,
							     my_level_info.which_level 			   )  ) ;  
				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_lu_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
				
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}

		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 

		
		// 我少寫一塊 ， r_boundary 的判斷了  
		// 這裡的for loop 是在掃 在這個 x_position 上的 r_boundary 有誰
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 *-----------------------------------------------------------------------------*/
			// 這是右下角點 
			if(  i > main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_rd = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_rd_corner_y() >= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_rd = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_r_boundary[i][j].print_rd_corner_y() < main_vertex.print_y_position() ){
					overlap_or_not_for_rd = true ; 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}
				
				if( overlap_or_not_for_rd == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 
				
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() >  main_vertex.print_y_position() && 
									obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
							     obstacle_r_boundary[i][j].print_ru_corner_y() ,
							     my_level_info.which_level 			   )  ) ;  
				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_ru_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			 

			}

		}// end of for loop variable j 
			
	}// 掃過所有的x , for loop i 
	
	
	/*-----------------------------------------------------------------------------
	 再將侯選點 ， 做 塞選 
	 塞選完後 ， 建立 OASG 要的連線 
	 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	 也就沒有 2 的index 可以放東西  
	 下面的這個vector 多加個1  即便 點是在邊界了  也沒什麼差  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	
	// 下面的 i < my_level_info.plane_width 不能有等號 ， 因為 plane_width = 100 , index = 0 ~ 99 
	for(int i = main_vertex.print_x_position() ; i < my_level_info.plane_width  ; i++ ){

		if( candidate_filter[i].size() > 1  ){
			// 這裡確實是要用升序的 
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	/*-----------------------------------------------------------------------------
	 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	 *-----------------------------------------------------------------------------*/
	int limits_position = my_level_info.plane_height ; 
	vector<vertex> real_candiate ; 
	for( int i = main_vertex.print_x_position()  ; i < my_level_info.plane_width ; i++ ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 我有依 y 排序過了 ， 所以 y 最小的有過 就會擋後面的
			// 即便沒過  後面的也輪不到 
			if(  candidate_filter[i][0].print_y_position() <= limits_position ){
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() - 1 ; 	
			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 
		
		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() - 1 ;  
		}

		/*-----------------------------------------------------------------------------
		// 下面這個 loop 是要用 obstacle 來限制住 limits_position 
		 for( unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			if( obstacle_l_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_l_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_l_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() - 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_l_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() &&
					 obstacle_l_boundary[i][j].print_ld_corner_y() <= limits_position ){
					limits_position =  obstacle_l_boundary[i][j].print_ld_corner_y() - 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/

	} // 由 main_vertex 往 右邊界掃過去 

	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    main_vertex.print_z_position() == OASG[i].print_node1_z() &&
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() &&
			    main_vertex.print_z_position()      == OASG[i].print_node2_z() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         main_vertex.print_z_position() == OASG[i].print_node2_z() &&	
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() &&
				 main_vertex.print_z_position()      == OASG[i].print_node1_z() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		//  我在想 ， 為什麼會有 edge 建到別層 ， 我想呢 是 ， projection_vertex , 在建給別層的時侯  ， z 軸屬性 ， 是原先那層的
		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , my_level_info.which_level  ,
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() , my_level_info.which_level  )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 

}		/* -----  end of function RU_Region_handler  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ThreeD_LU_Region_handler
 *  Description:  
 * =====================================================================================
 */
	void
ThreeD_LU_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex , 
			   vector<vertex> &all_pin  , vector<obstacle> *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary 
			   , vector<vertex> *pin_on_right_position )
{
	
	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for( int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){

		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_lu_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_lu_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些
	

	/*-----------------------------------------------------------------------------
	cout << "In line 528 , prepare for line_sweep " << endl; 
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position  , i 代表掃到那個 x_position 
	for(int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		// 把 obstacle  的 l_boundary 在這 x_position 上的 obstacle 
		// 給抓進來  , 因為是先左往右掃  ， 所以 l_boundary  是加的 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_l_boundary[i][j] ); 
		}//  把碰到 l_boundary 的 obstacle 放進來  


		/*-----------------------------------------------------------------------------
		 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_r_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}
				
			}// scan current_state 內的element 

		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			// 再刪掉之前  ， 先把它給放進來  
			if( i  == current_state[k].print_ru_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_ru_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}
			
		}// scan current_state 內的element 
		vector<obstacle> bound_obstacle ; 
		int A_min_y = my_level_info.plane_height , A_index = -1 ; 
		int B_min_y = my_level_info.plane_height , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_ld_corner_y() >= main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() < A_min_y  			   ){
			   	A_min_y = current_state[k].print_ld_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() < B_min_y  			   ){
			   	B_min_y = current_state[k].print_lu_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 

		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){

			// 因為我 限定了 i < main_vertex.print_x_position() , 所以 不會有 自已跟自已比的情 況 
			if(  i < main_vertex.print_x_position()  && pin_on_right_position[i][j].print_y_position() >= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     my_level_info.which_level 			     )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			// pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() 這行不能有等號 ， 不然 會跟自已比到
			if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= pin_on_right_position[i][j].print_y_position()  &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     my_level_info.which_level 			     )  ) ;  
				}// 塞入侯選點內 
				 
			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 

		/*-----------------------------------------------------------------------------
		cout << "In line 610 " << endl; 
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  哇 ， 這個人的想法 ， 有很多地方有錯 ， 我一開 以為 它不用看 l_boundary 
			 *  的 lu corner 是對的  ， 一般情是沒錯  ， 但 如果 corner_2 是在 平行 main_vertex.y
			 *  的 左邊那個區塊呢 ， 那這 卻實 是要連線的  , 但它 pseudo code 的描述 卻不是這樣 
			 *-----------------------------------------------------------------------------*/


			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 下面這個 ix 進來的單位是 obstacle  即便我第一個 ix 是想處理 corner_ld , 
			 但 corner_lu 也是會進來的 
			 *-----------------------------------------------------------------------------*/
			if( i < main_vertex.print_x_position() &&  obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ld = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				// 下面這個 if 也是要判斷的 , 因為 obstacle 可能會被 main_vertex.x 給切到body 的
				// 中間  , 所以 ld 不會被 rd 給擋到  
				if( obstacle_l_boundary[i][j].print_ld_corner_y() >= main_vertex.print_y_position()  ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_ld = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}// ld 在 Region 內
				else{
					overlap_or_not_for_ld = true ; 		
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() ,
								     my_level_info.which_level  		   )  ) ;  
				}// 這當然是 ld 跑到 main_vertex.y 的下面 
				
				if( overlap_or_not_for_ld == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() ,
								     my_level_info.which_level  		   )  ) ;  
				}// 塞入侯選點內 


			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ;  

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() ,
								     my_level_info.which_level  		   )  ) ;  

				}

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ;  
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_ld_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  

				}
				 *-----------------------------------------------------------------------------*/

			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() && 
									obstacle_l_boundary[i][j].print_lu_corner_y() >  main_vertex.print_y_position() ){
			 
				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
							     obstacle_l_boundary[i][j].print_lu_corner_y() ,
							     my_level_info.which_level  		   )  ) ;  
				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ;  
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_l_boundary[i][j].print_lu_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  

				}
				 *-----------------------------------------------------------------------------*/

			}// end of else if

		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 
		//cout << "In line 681 " << endl; 

		
		// 我少寫一塊 ， r_boundary 的判斷了  
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 *-----------------------------------------------------------------------------*/

			// 這是右下角點 
			if( i < main_vertex.print_x_position() &&  obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){
				bool overlap_or_not_for_rd = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_rd_corner_y() >= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
						    bound_obstacle[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
							overlap_or_not_for_rd = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}// 在 Region 內 
				else if( obstacle_r_boundary[i][j].print_rd_corner_y() < main_vertex.print_y_position() ){
					overlap_or_not_for_rd = true ; 	
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() ,
								     my_level_info.which_level  		   )  ) ;  
				}// 在Region 的下方
				
				if( overlap_or_not_for_rd == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() ,
								     my_level_info.which_level  		   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() > main_vertex.print_y_position() ){
				bool overlap_or_not = false ;  

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    bound_obstacle[k].print_rd_corner_y() >= main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() ,
								     my_level_info.which_level  		   )  ) ;  
				}
			 
				/*-----------------------------------------------------------------------------
				 *   
				bool overlap_or_not = false ;  
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_rd_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/

			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() >  main_vertex.print_y_position() && 
									obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
							     obstacle_r_boundary[i][j].print_ru_corner_y() ,
							     my_level_info.which_level  		   )  ) ;  
				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ;  
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < obstacle_r_boundary[i][j].print_ru_corner_y() &&
					    current_state[k].print_ru_corner_y() > main_vertex.print_y_position()                ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
				 
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}

		}// end of for loop variable j 
		//cout << "In line 741 " << endl; 
			
	}// 掃過所有的x , for loop i 
	

	/*-----------------------------------------------------------------------------
	cout << "In line 743 , already select all candidate " << endl ; 	
	 再將侯選點 ， 做 塞選 
	 塞選完後 ， 建立 OASG 要的連線 
	 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	 也就沒有 2 的index 可以放東西  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	
	for(int i = 0 ; i <= main_vertex.print_x_position()   ; i++ ){

		// 這裡要注意 ， 位在上方區塊的 ， 要用升序去排
		// 下方的則用降序去排 
		if( candidate_filter[i].size() > 1  ){
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	/*-----------------------------------------------------------------------------
	 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	 *-----------------------------------------------------------------------------*/
	int limits_position = my_level_info.plane_height ; 
	vector<vertex> real_candiate ; 
	for( int i = main_vertex.print_x_position()  ; i >= 0 ; i-- ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 我有依 y 排序過了 ， 所以 y 最小的有過 就會擋後面的
			// 即便沒過  後面的也輪不到 
			if( candidate_filter[i][0].print_y_position() <= limits_position ){
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() - 1 ; 	
			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 

		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() - 1 ;  
		}

		/*-----------------------------------------------------------------------------
		 *  
		 for( unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			if( obstacle_r_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_r_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_r_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() - 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_r_boundary[i][j].print_ld_corner_y() > main_vertex.print_y_position() &&
					 obstacle_r_boundary[i][j].print_ld_corner_y() <= limits_position 		){
					limits_position =  obstacle_r_boundary[i][j].print_ld_corner_y() - 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/

	} // 由 main_vertex 往 l_boundary 掃過去 
	
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    main_vertex.print_z_position() == OASG[i].print_node1_z() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() &&
			    main_vertex.print_z_position()      == OASG[i].print_node2_z() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         main_vertex.print_z_position() == OASG[i].print_node2_z() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() && 
			         main_vertex.print_z_position()      == OASG[i].print_node1_z() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , my_level_info.which_level ,
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() ,  my_level_info.which_level )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 
	

}		/* -----  end of function LU_Region_handler  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ThreeD_RD_Region_handler
 *  Description:  
 * =====================================================================================
 */
	void
ThreeD_RD_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex  , 
			   vector<vertex> &all_pin  , vector<obstacle> *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary
			   , vector<vertex> *pin_on_right_position )
{

	
	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for(  int i = my_level_info.plane_width - 1  ; i >=  main_vertex.print_x_position() ; i-- ){

		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_rd_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_rd_corner_x() >= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些

	/*-----------------------------------------------------------------------------
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 上面 應該是做好前置的處理 ， 會要微調的地方 ， 應該是下面這塊 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position 
	// 由 最右邊掃到 main_vertex ， 抓好 candidate 再由main_vertex 掃回去 
	for(int i = my_level_info.plane_width - 1  ; i >=  main_vertex.print_x_position() ; i-- ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_r_boundary[i][j] ); 
		}//  把碰到 l_boundary 的 obstacle 放進來  


		/*-----------------------------------------------------------------------------
		 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_l_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){
					// 這裡要記得多減個1 
					// 以 current_state.begin() + 6 來說 是指
					// 刪第七個元素  , 哎  想一想  好像又不用減1哎
					// begin 指向第一個元素 , k = 0 , 
					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}
				
			}// scan current_state 內的element 
		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			// 再刪掉之前  ， 先把它給放進來  
			if( i  == current_state[k].print_lu_corner_x() && bool_obstacle_cross_main_vertex_y[i] == false &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_lu_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}// 只會遇到一個相同的 所以不用再掃下去了 
			
		}// scan current_state 內的element 
		vector<obstacle> bound_obstacle ; 
		int A_max_y = 0 , A_index = -1 ; 
		int B_max_y = 0 , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() > A_max_y  			   ){
			   	A_max_y = current_state[k].print_lu_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_lu_corner_y() > main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() > B_max_y  			   ){
			   	B_max_y = current_state[k].print_ld_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 
		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){

			if( i > main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() <= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position()  		      &&
					    bound_obstacle[k].print_ru_corner_y() > pin_on_right_position[i][j].print_y_position()     ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     pin_on_right_position[i][j].print_z_position()  )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			else if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position()  		      &&
					    bound_obstacle[k].print_rd_corner_y() >= pin_on_right_position[i][j].print_y_position()     ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     pin_on_right_position[i][j].print_z_position()  )  ) ;  
				}// 塞入侯選點內 

			}
			
		}// 考慮那些pin 可以放入 ， 當侯選點 
		/*-----------------------------------------------------------------------------
		 *  
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 *-----------------------------------------------------------------------------*/
			if( i > main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_lu = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				// 如果 lu 在 Region 內 又沒被擋 代表 可以連 
				if( obstacle_l_boundary[i][j].print_lu_corner_y() <= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    bound_obstacle[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
							overlap_or_not_for_lu = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}// 在Region 內 ， 要看看 有沒有被其它obstacle 擋到 
				else if( obstacle_l_boundary[i][j].print_lu_corner_y() > main_vertex.print_y_position() ){
					overlap_or_not_for_lu = true ;  
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}// 其它 ， 便是 lu 在 main_vertex.y 的上面 , 便直接給 true  , 
				
				if( overlap_or_not_for_lu == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <  main_vertex.print_y_position() &&
									obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
							     obstacle_l_boundary[i][j].print_ld_corner_y() ,
							     my_level_info.which_level 			   )  ) ;  
				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
					    current_state[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_ld_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_lu_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    bound_obstacle[k].print_rd_corner_y() >= obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
					    current_state[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false  ){
				 
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			 

			}

		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 


		// 我少寫一塊 ， r_boundary 的判斷了  
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 *-----------------------------------------------------------------------------*/

			if( i > main_vertex.print_x_position() &&  obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ru = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				if( obstacle_r_boundary[i][j].print_ru_corner_y() <= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() <  main_vertex.print_y_position() 		     &&
						    bound_obstacle[k].print_ru_corner_y() >  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
							overlap_or_not_for_ru = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 
					
				}
				else if( obstacle_r_boundary[i][j].print_ru_corner_y() > main_vertex.print_y_position() ){
					overlap_or_not_for_ru = true ;  
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				 
				}
				
				if( overlap_or_not_for_ru == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() <  main_vertex.print_y_position() &&
									obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
							     obstacle_r_boundary[i][j].print_rd_corner_y() ,
							     my_level_info.which_level 			   )  ) ;  

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() <  main_vertex.print_y_position() 		     &&
					    current_state[k].print_ru_corner_y() >  obstacle_r_boundary[i][j].print_rd_corner_y()    ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <=  main_vertex.print_y_position() 		     &&
					    bound_obstacle[k].print_rd_corner_y() >=  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}


				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() <  main_vertex.print_y_position() 		     &&
					    current_state[k].print_ru_corner_y() >  obstacle_r_boundary[i][j].print_ru_corner_y()    ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}


		}// 因為是 r_boundary  所以在這個 Region , 二個corner 都要看 

	}// 掃過所有的x , for loop i 
	
	
	/*-----------------------------------------------------------------------------
	 再將侯選點 ， 做 塞選 
	 塞選完後 ， 建立 OASG 要的連線 
	 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	 也就沒有 2 的index 可以放東西  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內

	// 哇  ， 該不會是錯這吧  ， plane_width = 100 , 所以 我只能跑到 99 
	for(int i = main_vertex.print_x_position() ; i <  my_level_info.plane_width    ; i++ ){
		if( candidate_filter[i].size() > 1  ){
			// 因為是 LD 所以 要用降的排 
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_decent_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 


	/*-----------------------------------------------------------------------------
	 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	 因為是 LD_REgion 所以 main_vertex  本身 就是最高的位置了  
	 *-----------------------------------------------------------------------------*/
	int limits_position = 0 ; 
	vector<vertex> real_candiate ; 

	/*-----------------------------------------------------------------------------
	 從 main_vertex 往左掃 ， 抓出 真正要連的 ， 因該是不用 main_vertex.print_x_position + 1 的吧 
	 這是  RD_Region_handler 
	 由右往左掃 ， 還是要 記下 目前有那些 obstacle 擋到  , 因為不想再 弄一個新的 vector 出來
	 所以直接用 把之前的 vector  給 clear 就好了 
	 *-----------------------------------------------------------------------------*/
	for( int i = main_vertex.print_x_position()  ; i < my_level_info.plane_width ; i++ ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 main_vertex 在這個 Region  是最大的y ， 所以 
			// 這裡的 candidate_filter  的y 被降排了  ， 了 所以  第一個y 會最這行裡 最大的 
			if(  candidate_filter[i][0].print_y_position() >= limits_position ){
				// 第一塊先連線 
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() + 1 ; 	


			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 

		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() + 1 ;  
		}

		/*-----------------------------------------------------------------------------
		 *   
		 如果不用 老師的方法 ， 而是照我自已想的方法
		 也就是  多觀察一個東西 ， 就是 掃回去 遇到的boundary
		 用來 調整 limits_position , 那 是要先連線呢 還是 先更新 
		 boundary  應是先連線 ， 再更新 limits_position 
		 因為這是 RD_Region_handler , 回頭是 左往右掃  ， 所以只要看 l_boundary
		 來更新 limits_position  , 我只要注意obstacle 的最小 y 跟最大 y 即可 
		 如果 最大y 在 Region 內 ， 且 比 limits_position 大 就更新 
		 另一case  如果 最大y  剛好在 main_vertex 的 y  , 代表 後面的連線 跟這個
		 main_vertex 沒有關係了 ， 所以 ， limits_position = main_vertex.y + 1 , 
		 使得跟後面的點 都連不到 
		 for( unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			if( obstacle_l_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_l_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_l_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() + 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_l_boundary[i][j].print_lu_corner_y() < main_vertex.print_y_position() &&
					 obstacle_l_boundary[i][j].print_lu_corner_y() >= limits_position ){
					limits_position =  obstacle_l_boundary[i][j].print_lu_corner_y() + 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/


	} // 由 main_vertex 往 l_boundary 掃過去 

	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    main_vertex.print_z_position() == OASG[i].print_node1_z() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    main_vertex.print_z_position()      == OASG[i].print_node2_z() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         main_vertex.print_z_position() == OASG[i].print_node2_z() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         main_vertex.print_z_position()      == OASG[i].print_node1_z() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , my_level_info.which_level ,
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() , my_level_info.which_level )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 

}		/* -----  end of function RD_Region_handler  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ThreeD_LD_Region_handler
 *  Description:  基本上呢  ， level_info 裡面 就已經有 所有的 obstacle 及pin 
 *  		  跟各種我要的資訊了 
 * =====================================================================================
 */
	void
ThreeD_LD_Region_handler ( vector<edge> &OASG , level_info &my_level_info , vertex main_vertex  , 
			   vector<vertex> &all_pin  , vector<obstacle> *obstacle_ll_boundary , vector<obstacle> *obstacle_rr_boundary 
			   , vector<vertex> *pin_on_right_position )
{

	
	// 好吧  ， 我裡面  再做一次的分類  ，  , 把 在區域內要用的分出來 就好  
	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	bool bool_obstacle_cross_main_vertex_y[my_level_info.plane_width] ; 

	for(  int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){

		// 下面這一行 ， 是為了初始化  obstacle_cross_main_vertex_y 
		bool_obstacle_cross_main_vertex_y[i] = false ; 

		for( unsigned int j = 0 ; j < obstacle_ll_boundary[i].size() ; j++ ){

				if( obstacle_ll_boundary[i][j].print_ld_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_ll_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
					obstacle_l_boundary[i].push_back( obstacle_ll_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

		for( unsigned int j = 0 ; j < obstacle_rr_boundary[i].size() ; j++ ){
			 
				if( obstacle_rr_boundary[i][j].print_ld_corner_x() <= main_vertex.print_x_position() &&
				    obstacle_rr_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
					obstacle_r_boundary[i].push_back( obstacle_rr_boundary[i][j] ) ;  
				}

		}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 
	
	}// 只處理會用到的那些

	/*-----------------------------------------------------------------------------
	 做line_sweep 時 ， 要判斷是否為 這個區域 要的東西
	 記得是要 >   <   不能有等於  
	 因為 line_sweep 中  ， 會有掃到 自已的點 
	 我想呢  它的line_sweep  是 ， 左上角的Region 它是由左邊界掃到 這個 main_vertex 
	 而右上角的Region 則是由 右邊界 掃到 main_vertex ， 由此可見 一個vertex 要抓其OASG 裡的edge 
	 它的這個方法 ， 會要掃平面 二次 ， 而且還要對 那些抓出來的侯選點 ， 再做最後的 逃汰 
	 上面 應該是做好前置的處理 ， 會要微調的地方 ， 應該是下面這塊 
	 *-----------------------------------------------------------------------------*/
	vector<obstacle> current_state ; 
	vector<vertex> candidate ;
	// 先用個for 來跑整個plane 的x_position 
	for(int i = 0 ; i <= main_vertex.print_x_position() ; i++ ){
		// 先藉由 ， l_boundary 及 r_boundary 更新 current_state 	 
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){
			// 把在這個x_position 上的l_boundary 都放進來 
			// i 代表 x 位置 ， j 代表 在這個位置上的 obstacle 數目 
			current_state.push_back( obstacle_l_boundary[i][j] ); 
		}//  把碰到 l_boundary 的 obstacle 放進來  


		/*-----------------------------------------------------------------------------
		 接下來  ， 便是 遇到 r_boundary 時 ， 要做 刪的動作 
		 每 erase 一個element 便 重頭 再看  , 又或者是 把index 減1 
		 如同指向下一個 
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){
			// 每個 j 都會 指向一個 obstacle  我只要利用 obstacle_id 
			// 跟 current_state 中的 element 做比對 ， 比對了 就break 出去
			// 因為一個 obstacle_id 只會對到一個人  
			for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

				if( obstacle_r_boundary[i][j].print_obstacle_id() == current_state[k].print_obstacle_id() ){

					current_state.erase( current_state.begin() + k  );	 
					break ; 
				}
				
			}// scan current_state 內的element 
		}// 做 current_state 中 ， 對 obstacle 刪的動作 
		 *-----------------------------------------------------------------------------*/

		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){

			// 再刪掉之前  ， 先把它給放進來  
			if( i  == current_state[k].print_ru_corner_x() &&
			    main_vertex.print_y_position() < current_state[k].print_lu_corner_y() &&
			    main_vertex.print_y_position() > current_state[k].print_ld_corner_y() ){
				bool_obstacle_cross_main_vertex_y[i] = true  ;		
			}// 只會遇到一個相同的 所以不用再掃下去了 

			if( i  == current_state[k].print_ru_corner_x() ){
				// 這裡要記得多減個1 
				// 以 current_state.begin() + 6 來說 是指
				// 刪第七個元素  
				current_state.erase( current_state.begin() + k  );	 
				k-- ; 
			}
			
		}// scan current_state 內的element 

		vector<obstacle> bound_obstacle ; 
		int A_max_y = 0 , A_index = -1 ; 
		int B_max_y = 0 , B_index = -1 ; 
		for(unsigned int k = 0 ; k < current_state.size() ; k++  ){
			
			if( current_state[k].print_lu_corner_y() <= main_vertex.print_y_position() && 
			    current_state[k].print_lu_corner_y() > A_max_y  			   ){
			   	A_max_y = current_state[k].print_lu_corner_y() ;   
				A_index = k ;
			}			

			if( current_state[k].print_lu_corner_y() > main_vertex.print_y_position() && 
			    current_state[k].print_ld_corner_y() > B_max_y  			   ){
			   	B_max_y = current_state[k].print_ld_corner_y() ;   
				B_index = k ;
			}			

		}// scan current_state 內的element 
		if( A_index >= 0  ){
			bound_obstacle.push_back( current_state[ A_index ] ) ; 	 
		}
		if( B_index >= 0  ){
			bound_obstacle.push_back( current_state[ B_index ] ) ; 	 
		}

		// 哇 ， 我忘了 ， 還有 pin 點的 要加入   , 來判斷 這個pin 能否當 侯選點  ,  因為 pin_on_right_position 
		// 已經依 x_position 對 x 分類了 ， 所以  這裡只要看其y 有沒有在 Region 內 即可 
		for(unsigned int j = 0 ; j < pin_on_right_position[i].size() ; j++ ){


			if( i < main_vertex.print_x_position() &&  pin_on_right_position[i][j].print_y_position() <= main_vertex.print_y_position()  ){

				bool overlap_or_not = false ; 
				// 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				// 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				// pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				// 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position()  			&&
					    bound_obstacle[k].print_ru_corner_y() > pin_on_right_position[i][j].print_y_position()       ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     my_level_info.which_level 			     )  ) ;  
				}// 塞入侯選點內 

			}// 在我們要的範內了 
			// pin_on_right_position[i][j].print_y_position() > main_vertex.print_y_position() 這行不能有等號 ， 不然 會跟自已比到
			if( i == main_vertex.print_x_position() && pin_on_right_position[i][j].print_y_position() < main_vertex.print_y_position() ){

				bool overlap_or_not = false ; 
				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position()  			&&
					    bound_obstacle[k].print_rd_corner_y() >= pin_on_right_position[i][j].print_y_position()       ){
						overlap_or_not = true ;  
						break ; 
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 
				
				if( overlap_or_not == false ){
					candidate.push_back( vertex( pin_on_right_position[i][j].print_x_position()  , 
					                             pin_on_right_position[i][j].print_y_position()  ,
								     my_level_info.which_level 			     )  ) ;  
				}// 塞入侯選點內 
				 
			}

			
		}// 考慮那些pin 可以放入 ， 當侯選點 

		/*-----------------------------------------------------------------------------
		 抓出所有的侯選點 , 真是要跟 在這個x_position 上的所有 vertex 來看看
		 能否放入侯選點內了   ,  因為這是  LU_Region_handler 所以 ， l_boundary 只要看
		 corner_1 , r_boundary 則要看 corner_3 and corner_4  還要記得 上面是以 x_position 
		 為單位 ， 這裡還要多判斷  vertex 是否在  Region 內 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < obstacle_l_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 *-----------------------------------------------------------------------------*/
			if(  i < main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_lu = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				 *-----------------------------------------------------------------------------*/
				if( obstacle_l_boundary[i][j].print_lu_corner_y() <= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    bound_obstacle[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
							overlap_or_not_for_lu = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_l_boundary[i][j].print_lu_corner_y() > main_vertex.print_y_position() ){
					overlap_or_not_for_lu = true ; 	
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
					                             obstacle_l_boundary[i][j].print_ld_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}
				
				if( overlap_or_not_for_lu == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
					                             obstacle_l_boundary[i][j].print_lu_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_ld_corner_y() <  main_vertex.print_y_position() && 
									obstacle_l_boundary[i][j].print_lu_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
							     obstacle_l_boundary[i][j].print_ld_corner_y() ,
							     my_level_info.which_level 			   )  ) ;  

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
					    current_state[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_ld_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_ld_corner_x() , 
								     obstacle_l_boundary[i][j].print_ld_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}
			else if( i == main_vertex.print_x_position() && obstacle_l_boundary[i][j].print_lu_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    bound_obstacle[k].print_rd_corner_y() >= obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
					    current_state[k].print_ru_corner_y() > obstacle_l_boundary[i][j].print_lu_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false ){
					candidate.push_back( vertex( obstacle_l_boundary[i][j].print_lu_corner_x() , 
								     obstacle_l_boundary[i][j].print_lu_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}


		}// 因為是 l_boundary  所以在這個 Region 只要 只要看 corner_1 且 要確定是否在 Region 內  , variable j 

		
		// 我少寫一塊 ， r_boundary 的判斷了  
		for(unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			/*-----------------------------------------------------------------------------
			 *  
			 因為 最外層的 loop  已經 限制了 x ， 所以這裡 
			 只要限制好y 就 是在 Region 內 ， 但有個問題 
			 我抓的 侯選點 會不會跟main_vertex 是同個點
			 還有就是 我抓的 OASG edge 會不會 另一頭的點先掃
			 s.t 這個 edge 已經在裡面了 
			 第一個 if 是 , 右上角點
			 下面的第一個 if 是 權限最寬的 ， rd <= main_vertex.y 代表 ru 可以在 main_vertex.y 的上面
			 也可以在下面  
			 *-----------------------------------------------------------------------------*/

			if( i < main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() <= main_vertex.print_y_position() ){
				bool overlap_or_not_for_ru = false ; 

				/*-----------------------------------------------------------------------------
				 由 pseudo 內知 ， 我把此點到 main_vertex 的這段當成一個 interval 
				 來跟 current_state 內的 比對  ， 只要有相交 這個點 就不能納入侯選點
				 pseudo code 內 沒有要跟 vertex 比  所以便只跟current_state 比就好了
				 只要有 obstacle 的 min_y 在這段範圍內 出現 便是 被擋到 
				 *-----------------------------------------------------------------------------*/
				if( obstacle_r_boundary[i][j].print_ru_corner_y() <= main_vertex.print_y_position() ){
				 
					for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
						if( bound_obstacle[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
						    bound_obstacle[k].print_ru_corner_y() > obstacle_r_boundary[i][j].print_ru_corner_y()        ){
							overlap_or_not_for_ru = true ;  
						}// 判斷是否相交   
					}// loop k  , scan all bound_obstacle's obstacle 

				}
				else if( obstacle_r_boundary[i][j].print_ru_corner_y() > main_vertex.print_y_position() ){

					overlap_or_not_for_ru = true ; 	
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
					                             obstacle_r_boundary[i][j].print_rd_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}
				if( overlap_or_not_for_ru == false ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
					                             obstacle_r_boundary[i][j].print_ru_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}// 塞入侯選點內 

			}// 這樣便確定是在 Region 內了  
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_rd_corner_y() <  main_vertex.print_y_position() &&
									obstacle_r_boundary[i][j].print_ru_corner_y() >= main_vertex.print_y_position() ){

				candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
							     obstacle_r_boundary[i][j].print_rd_corner_y() ,
							     my_level_info.which_level 			   )  ) ;  

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
					    current_state[k].print_ru_corner_y() > obstacle_r_boundary[i][j].print_rd_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false  ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_rd_corner_x() , 
								     obstacle_r_boundary[i][j].print_rd_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}
			else if( i == main_vertex.print_x_position() && obstacle_r_boundary[i][j].print_ru_corner_y() < main_vertex.print_y_position() ){
				bool overlap_or_not = false ; 

				for(unsigned int k = 0 ; k < bound_obstacle.size() ; k++ ){
					if( bound_obstacle[k].print_lu_corner_y() <= main_vertex.print_y_position() 			&&
					    bound_obstacle[k].print_rd_corner_y() >= obstacle_r_boundary[i][j].print_ru_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all bound_obstacle's obstacle 

				if( overlap_or_not == false  ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() ,
								     my_level_info.which_level 			   )  ) ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				bool overlap_or_not = false ; 
				for(unsigned int k = 0 ; k < current_state.size() ; k++ ){
					if( current_state[k].print_ld_corner_y() < main_vertex.print_y_position() 			&&
					    current_state[k].print_ru_corner_y() > obstacle_r_boundary[i][j].print_ru_corner_y()        ){
						overlap_or_not = true ;  
					}// 判斷是否相交   
				}// loop k  , scan all current_state's obstacle 
				if( overlap_or_not == false  ){
					candidate.push_back( vertex( obstacle_r_boundary[i][j].print_ru_corner_x() , 
								     obstacle_r_boundary[i][j].print_ru_corner_y() )  ) ;  
				}
				 *-----------------------------------------------------------------------------*/
			}


		}// 因為是 r_boundary  所以在這個 Region , 二個corner 都要看 

	}// 掃過所有的x , for loop i 
	
	

	/*-----------------------------------------------------------------------------
	 再將侯選點 ， 做 塞選 
	 塞選完後 ， 建立 OASG 要的連線 
	 侯選點 ， 依 x 分發 位置 ， 再依 y 排序 由小到大  	
	 下面要多加個1  是因為 ， 如果  main_vertex的 位置是2 那index 0 ~ 1 , 
	 也就沒有 2 的index 可以放東西  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> candidate_filter[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < candidate.size() ; i++ ){
		// 依 x 位置 放到相對應的 vector 中 		
		candidate_filter[ candidate[i].print_x_position() ].push_back( candidate[i] ); 
	}// 放到對應的籃子內
	
	for(int i = 0 ; i < main_vertex.print_x_position() + 1  ; i++ ){

		if( candidate_filter[i].size() > 1  ){
			// 因為是 LD 所以 要用降的排 
			sort( candidate_filter[i].begin() , candidate_filter[i].end() , sort_y_position_decent_in_O_2D_OASG );	 
		}// 要大於1個 element 才有排序的必要 

	}// sort 



	/*-----------------------------------------------------------------------------
	 排好了  ， 接下來便是  ， 跟 main_vertex 比  看那些是重要的  可以來 連線 放入 OASG 內 
	 做最後的filter , 因為是 LU_Region_handler 所以  應該是由 自已所在的位置 ， 往 l_boundary 
	 掃過去 , limits_position 用來 紀路  目前還能接受的 點的高度 ， 含自已也算
	 如有點在 range 內 要記得更新  且 減1 ( 在這個 Region 內是減1 )
	 因為是 LD_REgion 所以 main_vertex  本身 就是最高的位置了  
	 *-----------------------------------------------------------------------------*/
	int limits_position = 0 ; 
	vector<vertex> real_candiate ; 
	// 從 main_vertex 往左掃 ， 抓出 真正要連的 ， 因該是不用 main_vertex.print_x_position + 1 的吧 
	for( int i = main_vertex.print_x_position()  ; i >= 0 ; i-- ){

		if( candidate_filter[i].size() > 0  ){

			// 因為 main_vertex 在這個 Region  是最大的y ， 所以 
			// 這裡的 candidate_filter  的y 被降排了  ， 了 所以  第一個y 會最這行裡 最大的 
			if( candidate_filter[i][0].print_y_position() >= limits_position ){
				real_candiate.push_back( candidate_filter[i][0] ) ; 	 
				limits_position = candidate_filter[i][0].print_y_position() + 1 ; 	
			}// 放入 real_candiate ， 因為我還要 check  看  有沒有沖到  OASG 內的edge  ,  所以不能直接放

		}// 裡面有東西  才有進來看的必要 

		if( bool_obstacle_cross_main_vertex_y[i] == true  ){
				limits_position = main_vertex.print_y_position() + 1 ;  
		}

		/*-----------------------------------------------------------------------------
		 for( unsigned int j = 0 ; j < obstacle_r_boundary[i].size() ; j++ ){

			if( obstacle_r_boundary[i].size() > 0 ){

				if( main_vertex.print_y_position() < obstacle_r_boundary[i][j].print_lu_corner_y() &&
				    main_vertex.print_y_position() > obstacle_r_boundary[i][j].print_ld_corner_y() ){
					limits_position = main_vertex.print_y_position() + 1 ;  
				}// 被 obstacle 擋住了 ， 後面的連線 ， 都跟你無關 ， 
				else if( obstacle_r_boundary[i][j].print_ld_corner_y() < main_vertex.print_y_position() &&
					 obstacle_r_boundary[i][j].print_ld_corner_y() >= limits_position ){
					limits_position =  obstacle_r_boundary[i][j].print_ld_corner_y() + 1 ;  
				}
			 
			}// 確定有 obstacle  才能做更新丫 

		 }// 看在這個  x 位置上 ， 有那些 obstacle 的 l_boundary 在上面  
		 *-----------------------------------------------------------------------------*/

	} // 由 main_vertex 往 l_boundary 掃過去 
	
	for(unsigned int j = 0 ; j < real_candiate.size() ; j++ ){
		
		bool already_have = false ; 
		// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){

			if( main_vertex.print_x_position() == OASG[i].print_node1_x() &&
			    main_vertex.print_y_position() == OASG[i].print_node1_y() && 
			    main_vertex.print_z_position() == OASG[i].print_node1_z() && 
			    real_candiate[j].print_x_position() == OASG[i].print_node2_x() && 
			    main_vertex.print_z_position()      == OASG[i].print_node2_z() && 
			    real_candiate[j].print_y_position() == OASG[i].print_node2_y() ){
				already_have = true ;  
				break ; 
			}
			else if( main_vertex.print_x_position() == OASG[i].print_node2_x() &&
			         main_vertex.print_y_position() == OASG[i].print_node2_y() && 
			         main_vertex.print_z_position() == OASG[i].print_node2_z() && 
			         real_candiate[j].print_x_position() == OASG[i].print_node1_x() && 
			         main_vertex.print_z_position()      == OASG[i].print_node1_z() && 
			         real_candiate[j].print_y_position() == OASG[i].print_node1_y() ){
					already_have = true ;  
					break ; 
			}

		}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  

		if( already_have == false  ){
			OASG.push_back( edge( main_vertex.print_x_position() , main_vertex.print_y_position() , my_level_info.which_level ,
			   		      real_candiate[j].print_x_position() , real_candiate[j].print_y_position() , my_level_info.which_level )  ); 	 
		}// 這樣才把 edge 給加進去丫 

	}// scan all vertex i filtered 

}		/* -----  end of function LD_Region_handler  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  O_2D_OASG::build_3D_OASG_and_projection_vertex
 *  Description:  
 * =====================================================================================
 */
double O_2D_OASG::build_3D_OASG_and_projection_vertex( level_info &my_level_info  ){



	/*-----------------------------------------------------------------------------
	 先準備好要的資料 ,  先由 array_vertex 中 ， 把 pin 抽出來
	 再 用個新的 vector 把 complete_obstacle_information 存過來 
	 轉過來的這些新資料 ， 記得 對 x_position 做排序 
	 pin 不用重新給id 但 obstacle 要 
	  好在 我都有寫 copy constructor 
	  下面的一些排序 使得 我要在這個class 內 
	  加入二個sort 用的副程式 ， 我想到了一個 好的解決方法 
	  就是 vertex 的sort 就交給 vertex  我只要把 vector 傳過去就可以了
	  這樣  二個額外的比較副程式 ， 我只要在vertex 那裡寫就好了  其它地方便不用再加了  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> all_pin ; 
	for(unsigned int i = 0 ; i < my_level_info.array_vertex_vector.size() ; i++ ){

		if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == true  ){
			all_pin.push_back(  my_level_info.array_vertex_vector[i]  );	 
		}
		// 這個是 projection_vertex , 因為跟 pin 的屬性 相同 ， 所以我也要把它給放進來
		else if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == false && my_level_info.array_vertex_vector[i].print_corner_id() == 0 ){
			all_pin.push_back(  my_level_info.array_vertex_vector[i]  );	 
		}

	}// 把裡面的 pin 點 拉出來 
	
	vector<obstacle> relocate_id_complete_obstacle_info ; 
	for(unsigned int i = 0 ; i < my_level_info.complete_obstacle_information.size() ; i++ ){
		relocate_id_complete_obstacle_info.push_back(  my_level_info.complete_obstacle_information[i]  ); 
	}// 把 obstacle 給轉移 過來
	
	// 好 上面把東西都給轉好了  ， 接下來 要做排序的動作  ， all_pin 要依 x_position 由小排到大
	// obstacle  也是 且要給 新的 id 
	// sort( all_pin.begin() , all_pin.end() , sort_x_position_in_O_2D_OASG );	
	// sort( relocate_id_complete_obstacle_info.begin() , relocate_id_complete_obstacle_info.end() , sort_x_position_for_obstacle );

	
	for(unsigned int i = 0 ; i < relocate_id_complete_obstacle_info.size() ; i++ ){
		// 我打算從1 開始算 ， 所以  i + 1 
		relocate_id_complete_obstacle_info[i].set_obstacle_id( i + 1 ) ; 	 
	}// 依x_position 的大小 ， 排序  且  依 id 來看 


	/*-----------------------------------------------------------------------------
	 我想到要怎麼寫這塊核心了  ， 我把 抓出來的pin 點 ， 依plane_width , 放到各自對應的x 
	 這樣便方便多了  ， 對 obstacle 來說的話 ， 用三個 obstacle vector  也是以 x_position 
	 來區分 ， 但我分 三種 l_boundary , r_boundary , current_state , 這三個來存obstacle 
	 我想呢 大多只要看其 obstacle_id 便可以大概之道了  , line_sweep 我只要用個for loop 
	 來控制 ， 就可以之道 目前掃到那一行了 
	 *-----------------------------------------------------------------------------*/
	vector<vertex> pin_on_right_position[my_level_info.plane_width] ;	
	for( unsigned int i = 0 ; i < all_pin.size() ; i++ ){

		pin_on_right_position[ all_pin[i].print_x_position() ].push_back(  all_pin[i]   );	

	}// 把所有的pin 點 ， 依照 其所在的x 位置 ， 分別 放到相對應的vector中


	vector<obstacle> obstacle_l_boundary[my_level_info.plane_width] ; 
	vector<obstacle> obstacle_r_boundary[my_level_info.plane_width] ; 
	for( unsigned int i = 0 ; i < relocate_id_complete_obstacle_info.size() ; i++ ){
		 
		 obstacle_l_boundary[ relocate_id_complete_obstacle_info[i].print_ld_corner_x() ].push_back(  relocate_id_complete_obstacle_info[i]  );
		 obstacle_r_boundary[ relocate_id_complete_obstacle_info[i].print_ru_corner_x() ].push_back(  relocate_id_complete_obstacle_info[i]  );

	}// 如上一般 ， 把 obstacle  依 x_position 放到適當的位置 

	/*-----------------------------------------------------------------------------
	 看來  我要把那四個付責處理的程式 ， 寫成副程式 ， 放在上面
	 這個當主程式 來呼叫   先把 架構給弄好  
	 這裡的架構  主要是  分辦  這個 vertex 是pin 及 obstacle_corner 的那一個
	 因為  不同的  有些處理器  它是不用被丟進去的 
	 像 corner 1 就不用處理 Region 1 的連線 
	 我之前 要掃 過 array_vertex 是用什麼去掃的丫 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i = 0 ; i < my_level_info.array_vertex_vector.size() ; i++ ){

		/*-----------------------------------------------------------------------------
		 *  
		vector<obstacle> give_to_RU  ; 
		vector<obstacle> give_to_RD  ; 
		vector<obstacle> give_to_LU  ; 
		vector<obstacle> give_to_LD  ; 

		for(unsigned int j = 0 ; j < relocate_id_complete_obstacle_info.size() ; j++ ){

			// LU 只要 obstacle 的左上角點 ， 在 目前點的 左上那區內  ， 就可以把這個obstacle 放進來 
			if( relocate_id_complete_obstacle_info[j].print_lu_corner_x() <= my_level_info.array_vertex[i].print_x_position() &&
			    relocate_id_complete_obstacle_info[j].print_lu_corner_y() >= my_level_info.array_vertex[i].print_y_position() ){
				give_to_LU.push_back( relocate_id_complete_obstacle_info[j] ) ;  
			}// LU
			if( relocate_id_complete_obstacle_info[j].print_ld_corner_x() <= my_level_info.array_vertex[i].print_x_position() &&
			    relocate_id_complete_obstacle_info[j].print_ld_corner_y() <= my_level_info.array_vertex[i].print_y_position() ){
				give_to_LD.push_back( relocate_id_complete_obstacle_info[j] ) ;  
			}// LD

			if( relocate_id_complete_obstacle_info[j].print_ru_corner_x() >= my_level_info.array_vertex[i].print_x_position() &&
			    relocate_id_complete_obstacle_info[j].print_ru_corner_y() >= my_level_info.array_vertex[i].print_y_position() ){
				give_to_RU.push_back( relocate_id_complete_obstacle_info[j] ) ;  
			}// RU
			if( relocate_id_complete_obstacle_info[j].print_rd_corner_x() >= my_level_info.array_vertex[i].print_x_position() &&
			    relocate_id_complete_obstacle_info[j].print_rd_corner_y() <= my_level_info.array_vertex[i].print_y_position() ){
				give_to_RD.push_back( relocate_id_complete_obstacle_info[j] ) ;  
			}// RD
		 
		}// scan all obstacles 
		 *-----------------------------------------------------------------------------*/

		if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == true ){

			/*-----------------------------------------------------------------------------
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/
			ThreeD_RU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position     		  );
			ThreeD_LU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
					     	  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_RD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_LD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );

		}
		// 下面的4 個 corner  , 分別只要處理 3 個 Region  ， 因為有一個  跟本不用去理會的 
		else if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == false && my_level_info.array_vertex_vector[i].print_corner_id() == 1 ){
			
			/*-----------------------------------------------------------------------------
			 *  
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/

			ThreeD_LU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
					     	  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_RD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_LD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );


		}// end of corner 1 , 左下角點 
		else if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == false && my_level_info.array_vertex_vector[i].print_corner_id() == 2 ){
			
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/

			ThreeD_RU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position     		  );
			ThreeD_LU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
					     	  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_LD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );


		}// end of corner 2 , 左上角點
		else if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == false && my_level_info.array_vertex_vector[i].print_corner_id() == 3 ){
			 
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/

			ThreeD_RU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position     		  );
			ThreeD_LU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
					     	  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_RD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	

		}// end of corner 3  , 右上角點
		else if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == false && my_level_info.array_vertex_vector[i].print_corner_id() == 4 ){
			 
			/*-----------------------------------------------------------------------------
			 *  
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			 *-----------------------------------------------------------------------------*/

			ThreeD_RU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position     		  );
			ThreeD_RD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_LD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );


		}// end of corner 4  , obstacle 的右下角點
		else if( my_level_info.array_vertex_vector[i].Is_pin_or_not() == false && my_level_info.array_vertex_vector[i].print_corner_id() == 0 ){
			 
			/*-----------------------------------------------------------------------------
			RU_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			RD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			LD_Region_handler( OASG , my_level_info ,my_level_info.array_vertex[i] );						 
			為什麼  這個都沒有進來呢  ， 告夭 ， 我 projection_vertex 我設的是 0 ， 我怎麼會記成 5 呢
			 *-----------------------------------------------------------------------------*/
			ThreeD_RU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position     		  );
			ThreeD_LU_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  , 
					     	  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_RD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );	
			ThreeD_LD_Region_handler( my_level_info.author_level_OASG , my_level_info , my_level_info.array_vertex_vector[i]  ,
						  all_pin , obstacle_l_boundary , obstacle_r_boundary ,  pin_on_right_position 		  );


		}// end of projection_vertex  ,  跟 pin 有點像 的 屬性  , 所以跟pin 點一樣 ， 要處理4個 Region

	}// end of for i 

	clock_t start , end ; 
	double dif   ; 
	start = clock() ; 
	// 想不到  我的my_level_info.my_level_OASG 裡面 還有 edge 有相同的case 跑出來 
	for( unsigned int i = 0 ; i < my_level_info.author_level_OASG.size() ; i++ ){
		bool already_have = false ; 
		for( unsigned int j = i + 1 ; j < my_level_info.author_level_OASG.size() ; j++ ){
		 
			if( my_level_info.author_level_OASG[i].print_node1_x() == my_level_info.author_level_OASG[j].print_node1_x() &&
			    my_level_info.author_level_OASG[i].print_node1_y() == my_level_info.author_level_OASG[j].print_node1_y() && 
			    my_level_info.author_level_OASG[i].print_node1_z() == my_level_info.author_level_OASG[j].print_node1_z() && 
			    my_level_info.author_level_OASG[i].print_node2_x() == my_level_info.author_level_OASG[j].print_node2_x() && 
			    my_level_info.author_level_OASG[i].print_node2_z() == my_level_info.author_level_OASG[j].print_node2_z() && 
			    my_level_info.author_level_OASG[i].print_node2_y() == my_level_info.author_level_OASG[j].print_node2_y() ){
				already_have = true ;  
			}
			else if( my_level_info.author_level_OASG[i].print_node2_x() == my_level_info.author_level_OASG[j].print_node1_x() &&
				 my_level_info.author_level_OASG[i].print_node2_y() == my_level_info.author_level_OASG[j].print_node1_y() && 
				 my_level_info.author_level_OASG[i].print_node2_z() == my_level_info.author_level_OASG[j].print_node1_z() && 
				 my_level_info.author_level_OASG[i].print_node1_x() == my_level_info.author_level_OASG[j].print_node2_x() && 
				 my_level_info.author_level_OASG[i].print_node1_y() == my_level_info.author_level_OASG[j].print_node2_y() && 
				 my_level_info.author_level_OASG[i].print_node1_z() == my_level_info.author_level_OASG[j].print_node2_z() ){
					already_have = true ;  
			}
			if( already_have == true  ){
				// 把這個相同的給刪掉 ， 再給個false 往後看 
				my_level_info.author_level_OASG.erase( my_level_info.author_level_OASG.begin() + j ); 
				j-- ; 
				already_have = false ; 
			}
		}// end of loop j 

	}// end of loop i 
	end = clock() ; 
	dif = (double) ( end - start ) ; 
	return dif ; 	
	// cout << "Delete the same edge , time spend is = " << (double) ( dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;

}// end of ThreeD_LD_Region_handler   


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  O_2D_OASG::print_3D_OASG_gnu
 *  Description:   這裡是要印出3D 的 
 * =====================================================================================
 */
void O_2D_OASG::print_3D_OASG_gnu( vector<level_info> &every_level_info , vector<edge> &vertical_edge  ){

	ofstream SGcout("Author_3D_OASG.gnu");
	SGcout << " set title \"Author_3D_author_OASG\" " << endl ; 
	SGcout << " set xrange[0:" << every_level_info[0].plane_width - 1 <<  "]" << endl ; 
	SGcout << " set yrange[0:" << every_level_info[0].plane_height - 1 <<  "]" << endl ; 
	SGcout << " set zrange[0:" << every_level_info.size() <<  "]" << endl ; 
	SGcout << "set xlabel \"X-axis\"" << endl ; 
	SGcout << "set ylabel \"Y-axis\"" << endl ; 
	SGcout << "set zlabel \"Z-axis\"" << endl ; 
	SGcout << "set output " << endl ; 
	SGcout << "set terminal X11 " << endl ; 

	SGcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	int arrow_count = 1 ; 
	for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
			 
		/*-----------------------------------------------------------------------------
		   用下面這個範本來改好了  
		  SGcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 
		 這裡要讀obstacle 出來　　，　寫在PlaneData.gnu 裡面
		 為什麼下面的 obstacle 第一層的數目  不太對呢   , 奇怪了  沒錯丫 
		 不之道為什麼 又好了哎 
		 *-----------------------------------------------------------------------------*/
		for(unsigned int j = 0 ; j < every_level_info[i].complete_obstacle_information.size() ; j++ ){

			SGcout << "# This is obstacle number " << j << endl; 
			/*-----------------------------------------------------------------------------
			 中間這塊 ， 要藉由各層的 vertex 判斷其 是否為 obstacle_corner 
			 來畫出 obstacle  , 那是之前的方法 ， 我現在要用complete_obstacle_information  
			 來畫     , func 內 會付責 arrow_count++ 的動作 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].complete_obstacle_information[j].print_3D_obstacle(SGcout ,  every_level_info[i].which_level , arrow_count ) ; 	

		}// end of print out all obstacle  variable j 

	}// end of get every_level_info to print out  variable i 


	//那這裡要印出　OASG 內的所有line  , 我還要多加上 vertical_edge 的line 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
	 
		for(unsigned int j  = 0 ; j < every_level_info[i].author_level_OASG.size() ; j++){
			/*-----------------------------------------------------------------------------
			 我想這裡 也要改了  ， 下面 這個 func 原是用來處理 2D 的 
			 edge.h 已經改過了 ， 所以裡面是 3D 的 position 
			 OASG[i].print_3D_edge_information_for_gnu( SGcout , arrow_count + i + 1 );// i + 1 只是obstacle 的編號而己 
			 *-----------------------------------------------------------------------------*/
			arrow_count++;
			every_level_info[i].author_level_OASG[j].print_3D_edge_information_for_gnu( SGcout , arrow_count ) ; 
		}

	}

	// 下面這個是 vertical_edge 的 
	for( unsigned  int i = 0  ; i < vertical_edge.size() ; i++ ){
		arrow_count++;	
		vertical_edge[i].print_3D_edge_information_for_gnu( SGcout , arrow_count ) ;  
	}


	/*-----------------------------------------------------------------------------
	PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　

	 下面這裡的話 ，  我能不能用二個放點的 txt 檔 ，應該可以的樣子
	 下面就 再多放一個 splot 不就好了麻 , 我來試看看先 
	 水拉  測試成功 我接下來要做的就是  把 projection_vertex.txt 的檔給弄出來
	 有點忘了  我是在那裡建 PlaneData_3D.txt  這個檔的了 

	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" " << endl ; 
	還沒寫出來前 ， 先不要改 code 
	 *-----------------------------------------------------------------------------*/

	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;
}  

#endif
