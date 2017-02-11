/*
 * =====================================================================================
 *
 *       Filename:  my_overlap_OASG.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/07/10 16時58分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef         MY_OVERLAP_OASG_H
	#define 	MY_OVERLAP_OASG_H
#include	<iostream>
#include	<fstream>
#include	"edge.h"
#include	"obstacle.h"
#include	"vertex.h"
#include	"level_info.h"
#include	<vector>
#include        <cstdio>
#include	<algorithm>
#include	<map>
#include	<cmath>
using namespace std ; 

 class my_overlap_OASG{

 private:
 	
	 /*-----------------------------------------------------------------------------
	  *  
 	 那我自已弄一個指標 
	bool (*op_x)();
	bool (*op_y)();
	  *-----------------------------------------------------------------------------*/
 public:
 	my_overlap_OASG(){
		
		/*-----------------------------------------------------------------------------
		 *  
		op_x = &my_overlap_OASG::sort_x_position_in_OASG ; 
		op_y = &my_overlap_OASG::sort_y_position_in_OASG ; 
		幹 ， 完全不行
		 *-----------------------------------------------------------------------------*/
	}
 	// 這裡我想先宣告 ， 下面再放實作 
	//bool (*sort_x_position) (vertex , vertex ) , bool (*sort_y_position)  (vertex , vertex )
	//
 	void build_OASG( vector<edge> &OASG , level_info &my_level_info  ) ; 
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
	  *   看來  我是打算把 ， 各層的 OASG 處理好後 都丟入 vector  OASG 內 ， 
	  *   所以我傳入的  ， 是單層的資訊  
	  *   前二個參數 是要用來畫 edge 的  ， 第三個是要畫 點及 obstacle 
	  *-----------------------------------------------------------------------------*/
	 void build_OASG_can_handle_projection_vertex(  level_info &my_level_info ) ; 
	 void print_3D_OASG_gnu(  vector<level_info> &every_level_info , vector<edge> &vertical_edge );

};

/*-----------------------------------------------------------------------------
 *  sort_y_position, 用來sort vertex 內的y軸 , 要用降密的方式
 *  看來  正如  剛神說的  ， 如果 前面沒有指名是那個類別的func 
 *  這就如同是  一個副程式而已
 *-----------------------------------------------------------------------------*/
bool sort_y_position_in_overlap_OASG(vertex v1 , vertex v2){
	/*-----------------------------------------------------------------------------
	 這裡呢  ， 我想要多加點東西 ， 當有二個 vertex 的y 相同時， 
	 就是一個 vertex 壘在一個 obstace的corner2  , 而  排的方式又是
	 先corner2 再 vertex  , 那會造成  第三個 vertex 如果在 這個obstacle中間
	 則這二個 vertex 應該變成一個 obstacle segment 的 ， 卻變成 non 
	 所以要把 corner2 排在下面 
	 *-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------
	 *  			content code 
		return ( v1.print_y_position() > v2.print_y_position() );	
	 *-----------------------------------------------------------------------------*/
		return ( v1.print_y_position() > v2.print_y_position() );	
}

 /*-----------------------------------------------------------------------------
 *  sort_x_position, 用來sort vertex 內的x軸 , 要用升密的方式
 *-----------------------------------------------------------------------------*/
bool sort_x_position_in_overlap_OASG(vertex v1 , vertex v2){
	return ( v1.print_x_position() < v2.print_x_position() );	
}


/*-----------------------------------------------------------------------------
 *  
不之道 func 傳的參數  可不可以拿func 當參數去傳
 找到一個大概的解決方法了  叫function pointer 
bool (*sort_x_position) (vertex , vertex )  , bool (*sort_y_position)  (vertex , vertex)
 *-----------------------------------------------------------------------------*/
void my_overlap_OASG::build_OASG( vector<edge> &OASG , level_info &my_level_info ){	

		// 這裡  ， 我要多個 結構 ， 用來存  ， 所有會用到的點 ， 要注意的  就是 
		// 因為是在overlap下 ， 所以  要多判斷 ， 點是否alvie   , 哎  
		// 好像也不用  ， 因為 edge咟本不用會到它 
		// vector<vertex> all_vertex ; 

		int debug = 0 ; 
		if( debug == 1 ){cout << "Line 117  , enter build_overlap_OASG  " << endl ;}
		// 下面幾行只是做一些初始化的動作而已  
		vector<vertex> x_position[my_level_info.plane_width] ;
		vector<vertex> y_position[my_level_info.plane_height] ;
		bool **head_tail_state = new bool *[my_level_info.plane_width] ; 
		for( int i = 0 ; i < my_level_info.plane_width ; i++){
			head_tail_state[i] = new bool[2];
		}
	 
		// 初始化, head_tail_state  . [0] 用來記head  , [1]用來記tail 
		// true 代表那個位置是vertex 
		for( int i = 0 ; i < my_level_info.plane_width ; i++){
			for( int j = 0 ; j < 2 ; j++){
				head_tail_state[i][j] = false ; 
			}
		}


		/*-----------------------------------------------------------------------------
		 *  應該就是在這裡 ， 把我額外抓的 ， overlap_vertex  ,放進去   , 抓出 overlap_vertex
		 *  以及 obstacle_corner alive_or_not 的修改地方 ， 是在 every_level_info裡面  
		 *  這裡的overlap_vertex 指的是  ， 二個obstacle 相交 ， 中間十字的那個點  
		 *-----------------------------------------------------------------------------*/
		 for(unsigned int i = 0 ; i < my_level_info.overlap_vertex.size() ; i++ ){
		 	x_position[ my_level_info.overlap_vertex[i].print_x_position() ].push_back( my_level_info.overlap_vertex[i] ); 
			// all_vertex.push_back( my_level_info.overlap_vertex[i] ) ; 
		 }// scan all overlap_vertex  to add into every x_position vector 

		//my_level_info.array_vertex 內的東西　，　放到相對應的x_position內, my_level_info.total  是 指my_level_info.array_vertex 內的元素數目
		//此時的array_vertex 內 有所有的pin  , 及 obsatcle_corner  
		for( int j = 0 ; j < my_level_info.total ; j++){
			// all_vertex.push_back( my_level_info.array_vertex[j] ) ; 	
			int which_x = my_level_info.array_vertex[j].print_x_position() ;
			// 把 相應的x_position 的vertex 放進去  ， 要對 被obstacle 擋到的 vertex
			// 設定其 core == false 
			x_position[which_x ].push_back( my_level_info.array_vertex[j] );

			if( my_level_info.array_vertex[j].print_y_position() == my_level_info.plane_height - 1  ){
				head_tail_state[which_x][0] = true ;  
			}
			if(my_level_info.array_vertex[j].print_y_position() == 0 ){
				head_tail_state[which_x][1] = true ;  
			}
		}// end of for j

		// 將東西放到對應的y_position內	 , 我忘為什麼要加這個 區塊了 
		for( int i = 0 ; i < my_level_info.plane_height ; i++){
			for( int j = 0 ; j < my_level_info.total ; j++){
				if( my_level_info.array_vertex[j].print_y_position() == i){
					y_position[i].push_back(my_level_info.array_vertex[j]);
				}
			}// end of for j
		}//end of for i 

		for( int i = 0 ; i < my_level_info.plane_height ; i++){
			 for(unsigned int j = 0 ; j < my_level_info.overlap_vertex.size() ; j++ ){
				y_position[ i ].push_back( my_level_info.overlap_vertex[j] ); 
			 }// scan all overlap_vertex  to add into every x_position vector 
		}//end of for i 

		if( debug == 1  ){cout << "Line 234 " << endl ;}

		/*-----------------------------------------------------------------------------
		 *  			CHECK vertex 的值 , 跟head_tail_state 
		 *
		 *		for(int i = 0 ; i < my_level_info.total ; i++){
				cout << "x  = "<< my_level_info.array_vertex[i].print_x_position() << " ( " <<  
				my_level_info.array_vertex[i].print_x_position() << " , " << my_level_info.array_vertex[i].print_y_position() << " ) " 
				<< endl << endl ; 
			}	
			
			for( int i = 0 ; i < my_level_info.plane_width ; i++){
					cout << "x = " << i <<  " ,  ( " << head_tail_state[i][0] << " , " << head_tail_state[i][1] << "  ) " << endl;  
			}
		 *
		 *
		 *-----------------------------------------------------------------------------*/
		
		/*-----------------------------------------------------------------------------
		 *驗證東西是否存入了， 
		int sizeofx_position = 0 ;
		for(int i = 0 ; i < my_level_info.plane_width ; i++ ){
			sizeofx_position = sizeofx_position + x_position[i].size() ; 
		}
		
		cout << "my_level_info.total = " << my_level_info.total << endl ;
		cout << "x_position = " << sizeofx_position << endl ; 

		 *-----------------------------------------------------------------------------*/
		



		/*-----------------------------------------------------------------------------
		 * 接下來要排序各個x_position , 內的y軸　，　以降蟇的方式排序，　
		 * 我是用內建的排序去做的
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < my_level_info.plane_width ; i++){
			sort( x_position[i].begin() , x_position[i].end() , sort_y_position_in_overlap_OASG  );
			// 按  自已寫個sort 好了 
			// 哇操  這裡面存的是vertex  , 我還要寫個 assignement operator
		}			

		// 這裡為什麼 要sort y 我有點忘了   , 我有點記起來了 ， 如果 有點茖在 obstacle 的上面 ， 
		// 也就是 corner 2 ,3 間 ， 或 1,4 間 ， 要微調其 left , right bound 
		for(int i = 0 ; i < my_level_info.plane_height; i++){
			 sort( y_position[i].begin() , y_position[i].end() , sort_x_position_in_overlap_OASG );
		}			
		
		
		if( debug == 1  ){cout << "Line 284 , sort " << endl ;}

		/*-----------------------------------------------------------------------------
			印出各個x_position 的內容 , 看有否真的排序
			裡面沒東西的 就印出This is empty 

		 *  for(int i = 0 ; i < my_level_info.plane_width ; i++){
			cout << "x_position is " << i  << endl ; 
			if(x_position[i].size() == 0){
				cout << "This is empty " << endl ; 
			}
			for(unsigned int j = 0 ; j < x_position[i].size() ; j++){
				cout  << x_position[i][j].print_y_position() << "   ";
			
			}		
			cout << endl << endl ; 
		}
		
			// 這裡是印出　y_position 的內容　看有否排序
			for(int i = 0 ; i < my_level_info.plane_height; i++){
			cout << "y_position is " << i  << endl ; 
			if(y_position[i].size() == 0){
				cout << "This is empty " << endl ; 
			}
			for(unsigned int j = 0 ; j < y_position[i].size() ; j++){
				cout  << y_position[i][j].print_x_position() << "   ";
			
			}		
			cout << endl << endl ; 
		}
		 *-----------------------------------------------------------------------------*/
			



		
		/*-----------------------------------------------------------------------------
		 * 用一個vector  存edge  這種data 名為OASG 
		 * 在vertex.h 內　建個bounding box interval , 存有l_boundary , r_boundary  , 
		 * 以及其中心的core , 
		 * 再建一個func , 用來判斷core 是否還存活著　，　因為core活著　才能更新這個
		 * 		interval 
		 *    another func  , 接收seg information 以及是 obstacle seg or not , 
		 *     		      不同性質的seg ，　是做不同的事
		 *-----------------------------------------------------------------------------*/
		// vector<edge> OASG ;  原本是放這的  ， 因為要切割成獨立 ， 所以 
		// 要放到最上面去 
		vector<int>  obstacle_cross[my_level_info.plane_width] ;
	 
		map<int,obstacle>::iterator it ;  

		/*-----------------------------------------------------------------------------
		 這個loop , 是要做map 的動作 ， 1 個key值對應到一個 obsatcle 
		for(unsigned int i = 0 ; i < my_level_info.complete_obstacle_information.size() ; i++  ){
			obstacle_map[ my_level_info.complete_obstacle_information[i].print_obstacle_id() ] =  my_level_info.complete_obstacle_information[i] ; 
		}
		 *-----------------------------------------------------------------------------*/
	 
	 	// 各個 以 x_position 分類的vector  , 分別存著 ， 有那些obsacle 有擋到這個區域   ,  存的是其obstacle  id , 
		// 也就是用來   obstacle_map 裡去抓東西的
		for(int i = 0 ; i < my_level_info.plane_width ; i++  ){
			if( x_position[i].size() > 0  ){
				for(unsigned int j = 0 ; j < my_level_info.complete_obstacle_information.size() ; j++ ){
					// 如果obsacle 有擋到這個x位置　，　便把這個obstacle_id 記下來
					if( i >= my_level_info.complete_obstacle_information[j].print_ld_corner_x() &&
					    i <= my_level_info.complete_obstacle_information[j].print_ru_corner_x() ){
						obstacle_cross[i].push_back( my_level_info.complete_obstacle_information[j].print_obstacle_id() ); 
					}
				}// inner loop j 
			}
		}// 把各個x_position中　，　被那些obstacle 擋到　，　記下其obstacle_id 

		/*-----------------------------------------------------------------------------
				// 把值抓出來看看吧
				cout << "output map info :" << endl ; 
				for(it = obstacle_map.begin() ; it != obstacle_map.end() ; it++ ){
					cout << (*it).first ; 
					(*it).second.print_out_for_check() ;
				}
		 *-----------------------------------------------------------------------------*/
		
		if( debug == 1  ){cout << "Line 364, before vertical_edge " << endl ;}

		/*-----------------------------------------------------------------------------
		 *  				建　OASG 中的　直線
		 *-----------------------------------------------------------------------------*/
		// 接下來是我　新方法的核心　，　要先建　OASG 中的　直線　 , 在overlap  下 只要多加一個 條件即可 ， 
		// 其它就用原本的方式去判斷 ， 就是要連線的二點 ， core_state 都要為true 也就是活著的 ， 因為當下
		// 死的只有  被擋在 obstacle body 內的 
		for(int i = 0 ; i < my_level_info.plane_width ; i++ ){
			// 因為這裡是要建直線　，　所以我只要考慮　，　x_position中　，　vertex 數目　>= 2 的即可 
			if( x_position[i].size() >= 2 && obstacle_cross[i].size() > 0 ){
				// 我要scan obstacle_cross , 抓出它的key , 再對到obstacle info 來用　 
				// 中間如果被obstacle的肚子擋到　，　就不能連線
				for(unsigned int j = 0 ; j < x_position[i].size() - 1 ; j++ ){

					bool bad_case = false ; 
					for(unsigned int k = 0 ; k < obstacle_cross[i].size() ; k++  ){
						int key = 0 ; 
						key = obstacle_cross[i][k] ;
						it = my_level_info.obstacle_map.find(key) ; 
						// 先判斷 y seg 有沒有相交　，　再判斷是交在那裡　 交在頭尾才連線　，　其它就不理它　
						// 只看肚子
						if( i != (*it).second.print_ld_corner_x() && i != (*it).second.print_ru_corner_x() ){
							// 肚子如果有相交就不連　,  下面二個判斷式　，　不能加上　＝　號　，　
							// 　會出事　，　二個相貼的obstacle 會出錯　
							if(  x_position[i][j].print_y_position()   >  (*it).second.print_ld_corner_y() && 
							     x_position[i][j+1].print_y_position() <  (*it).second.print_ru_corner_y() ){
								bad_case = true ;  
								break ; 
							}
						 
						}// 只看肚子 ,不看頭尾
						
					}// loop k
					// 看來是 要加在這裡 ,  下面那個是false 代表 通過了  我之前建連線的條件 
					// 我只要再多上一 overlap 要的條件就可以了 ， 就是建線的二點, 其core 都要是活的才行
					if( bad_case == false && x_position[i][j].core_state() == true && x_position[i][j + 1].core_state() == true ){
						OASG.push_back( edge( x_position[i][j].print_x_position() ,
								      x_position[i][j].print_y_position() , 					
								      x_position[i][j + 1].print_x_position() ,
								      x_position[i][j + 1].print_y_position() )  );		 
					}
				}// loop j
				
			}
			else if( x_position[i].size() >= 2 && obstacle_cross[i].size() ==  0 ){

				// 都沒有 obsatcle 擋到  ， 所以 也不會有 overlap_vertex 的出現 ， 
				// 這塊應該可以 ， 用之前的寫法 ， 不用改 
				for(unsigned int j = 0 ; j < x_position[i].size() - 1  ; j++ ){

					if( x_position[i][j].core_state() == true && x_position[i][j + 1].core_state() == true ){
						OASG.push_back( edge( x_position[i][j].print_x_position() ,
								      x_position[i][j].print_y_position() , 					
								      x_position[i][j + 1].print_x_position() ,
								      x_position[i][j + 1].print_y_position() )  ); 
					 
					}// end of loop if
				}// end of loop for
				 
			}// end of else if
		}// end of 建OASG 的直線　

		
		if( debug == 1  ){cout << "Line 424, after vertical_edge " << endl ;}


		/*-----------------------------------------------------------------------------
		 *  				建interval
		 *-----------------------------------------------------------------------------*/
		/*-----------------------------------------------------------------------------
		 好了　，　接下來　，　要用新方法　，　重建interval   了　，　如果　成功　便可以把下面那一大塊
		 給整個刪了　
		 先由　，　vertex 間的關係　，　及自已的屬性　，　先建個大概的 interval 
		 大概的意思 ， 應該是 l , r bound 只取上下的鄰近點 ， 而沒有 考慮到obstacle 
		 *-----------------------------------------------------------------------------*/
		if( debug == 1  ){cout << "Line 434, before build step1 interval  " << endl ;}
		for(int i = 0 ; i < my_level_info.plane_width ; i++  ){
			// 只有一個點的情況　，　一定是　pin 點　 , 
			// 且overlap_vertex 的case 也不會在這裡出現 
			if( x_position[i].size() == 1 ){
				x_position[i][0].set_l_interval( my_level_info.plane_height - 1 ) ;	 
				x_position[i][0].set_r_interval( 0 ) ;	 
			}
			else if( x_position[i].size() > 1 ){
				for(unsigned int j = 0 ; j < x_position[i].size()  ; j++  ){
					// core_state == false  , 沒必要建interval 
					// head 
					if( j == 0 && x_position[i][j].core_state() == true  ){
						x_position[i][j].set_l_interval( my_level_info.plane_height - 1 ) ;	 
						x_position[i][j].set_r_interval( x_position[i][j + 1].print_y_position() + 1  ) ;	 
					}
					// tail 
					else if( j == x_position[i].size() - 1 && x_position[i][j].core_state() == true ){
						x_position[i][j].set_l_interval( x_position[i][j - 1].print_y_position() - 1 ) ;	 
						x_position[i][j].set_r_interval( 0 ) ;	 
						break ; 	 
					}
					// normal case 
					else {
						// 這裡還要針對　，　corner 1,2 做調整　，　週其r , l 
						// corner 1,2 的其中一個邊界會被， obstacle 擋到  所以要多加設定
						// 而corner 3,4 則不用
						if( x_position[i][j].print_corner_id() == 1 && x_position[i][j].core_state() == true ){
							x_position[i][j].set_l_interval( x_position[i][j].print_y_position() );
							x_position[i][j].set_r_interval( x_position[i][j + 1].print_y_position() + 1  ) ;	 
						 
						}
						else if( x_position[i][j].print_corner_id() == 2 && x_position[i][j].core_state() == true ){
							x_position[i][j].set_l_interval( x_position[i][j - 1].print_y_position() - 1 ) ;	 
							x_position[i][j].set_r_interval( x_position[i][j].print_y_position() );
						 
						}
						else if( x_position[i][j].core_state() == true) {
							x_position[i][j].set_l_interval( x_position[i][j - 1].print_y_position() - 1 ) ;	 
							x_position[i][j].set_r_interval( x_position[i][j + 1].print_y_position() + 1  ) ;	 
						}

					}// end of else block 
				}// loop j 
			}// end of else if
		}// end of loop i 

			
		if( debug == 1  ){cout << "Line 482, after build step1 interval  " << endl ;}

		/*-----------------------------------------------------------------------------
		 我想把　，　在obstacle左邊界內的vertex 先給設成false  , 可以哎
		 這也不用改 ， overlap_vertex 也算是 會在 obstacle 的l bound 
		 不用向後連線 ， 所以可留著

		 喔 ， 我想到了  ， 這裡是看  ， corner2 , 遇到 corner1 代表是在 obstacle 的左邊界內 
		 *-----------------------------------------------------------------------------*/
		if( debug == 1  ){cout << "Line 489  " << endl ;}
		for(int i = 0 ; i < my_level_info.plane_width ; i++ ){
			for(unsigned int j = 0  ; j < x_position[i].size() ; j++  ){

				int which_y = x_position[i][j].print_y_position() ; 
				// 這裡 我怕corner 2 一設定完 ， 就把自已設成false  ,  所以後面多個true 的條件
				// overlap_vertex 也算在 pin 同屬性的丫   , 所以為true  ,應該有改到 overlap_vertex 才對丫 
				if(  my_level_info.only_left_bound[i][which_y] == true ){
					// 下面這裡 ， 把它設成 false  , 我想呢  ， 是不是直接把他給刪了 ， 就好了  
					x_position[i][j].set_core_state(false );	 
				}

			}// end of loop j 
		}// end of loop i 


		if( debug == 1  ){cout << "Line 515" << endl ;}


		// 用obstacle的肚子　，　做微調了　， 下面要有判斷式　，　記得　抓為true 的vertex 　
		for(int i = 0 ; i < my_level_info.plane_width ; i++  ){
			int l = 0 , r = 0 ; 
			for(unsigned int j = 0 ; j < x_position[i].size()  ; j++  ){
				// 把我之前設定的interval  , 先給拿出來設定初值
				l = x_position[i][j].print_l_interval() ;
				r = x_position[i][j].print_r_interval() ;

				// 對interval 做微調的這裡 ， 有要因為overlap 做更動的嗎
				if( x_position[i][j].core_state() == true ){
						 
					for(unsigned int k = 0 ; k < obstacle_cross[i].size() ; k++  ){
						int key = 0 ; 
						key = obstacle_cross[i][k] ;
						it = my_level_info.obstacle_map.find(key) ; 
						// 先判斷 y seg 有沒有相交　，　再判斷是交在那裡　 交在頭尾才連線　，　其它就不理它　
						// 只看肚子
						if( i != (*it).second.print_ld_corner_x() && i != (*it).second.print_ru_corner_x() ){
							// 抓肚子　，　更新出　離此點　最近的　l , r  
							// 先抓左　，　在core 的左區塊
							if( (*it).second.print_ld_corner_y() >= x_position[i][j].print_y_position() && 
							    (*it).second.print_ld_corner_y() <  l 			   	    ){
								l = (*it).second.print_ld_corner_y() ; 
							}
							// 再抓　，　core 的右區塊　
							if( (*it).second.print_ru_corner_y() <= x_position[i][j].print_y_position() && 
							    (*it).second.print_ru_corner_y() >  r 			   	    ){
								r = (*it).second.print_ru_corner_y() ; 
							}
						}// 只看肚子 ,不看頭尾 , 因為 頭尾是在 大概的設定 ， 就做好了 , 這裡要focus 的 ， 當然是肚子
						
					}// loop k
					
					// 抓出l , r 後　，　要分其是pin  corner ,1,2,3,4 的不同　去分別設定	
					if( x_position[i][j].Is_pin_or_not() == true ){
						x_position[i][j].set_l_interval( l ) ;	 
						x_position[i][j].set_r_interval( r ) ;	 
					}
					else if( x_position[i][j].print_corner_id() == 1  ){
						  x_position[i][j].set_r_interval( r ) ;	 
						  // 下面這行  ， 是  因為 obstacle_overlap 下 ， corner 2 會跟obstacle 內的點連線
						  // 所以 interval 要再設定的嚴一點
						  x_position[i][j].set_l_interval( x_position[i][j].print_y_position() ) ;	 
					}
					else if( x_position[i][j].print_corner_id() == 2  ){
						  x_position[i][j].set_l_interval( l ) ;	 
						  x_position[i][j].set_r_interval( x_position[i][j].print_y_position() ) ;	 
					}
					else if(   x_position[i][j].print_corner_id() == 4 ){
						  x_position[i][j].set_r_interval( r ) ;	 
					}
					else if(   x_position[i][j].print_corner_id() == 3 ){
						  x_position[i][j].set_l_interval( l ) ;	 
					}
					else if( x_position[i][j].Is_pin_or_not() == false && x_position[i][j].print_corner_id() == 0 ){
						// 因為 overlap_vertex 的屬性跟pin 是比較像的  ， 所以 設定也要跟他一樣
						// 我想呢 ， 只有茖在 obstacle 右邊界的 點 ， 才會落在這區塊去做設定
						// 左邊界 就不用了吧 , 會被直接擋到就算是死了 
						x_position[i][j].set_l_interval( l ) ;	 
						x_position[i][j].set_r_interval( r ) ;	 
					}

				}// 為true  才有更新interval 的必要
			}// loop j 
		}// end of loop i 

		if( debug == 1  ){cout << "Line 574" << endl ;}

		/*-----------------------------------------------------------------------------
		 *  			Check Code , May 9
		 * 			CHECK FOR INTERVAL 	
		// 弄個檢查interval 的東西吧　　，　print 出　interval 
		//這裡要檢查　，　看每個x位置的edge 是不是　頭尾相接 , 如果頭尾相接　那就沒有判斷到中間的
		//obstacle  了　　，　那只能印圖來看　有沒有畫錯　, 下面　這段code 只能　抓點跟點間的edge 
		//不能判斷中間有沒有obstacle擋到　
		
		for( int i = 0 ; i < my_level_info.plane_width ; i++ ){
			cout << "X position is  "  << i  << endl ; 
			for(unsigned int j = 0 ; j < x_position[i].size() ; j++  ){
				x_position[i][j].print_interval() ; 
			}
		}	
		 *-----------------------------------------------------------------------------*/
		 if( debug == 1 ){
			for( int i = 0 ; i < my_level_info.plane_width ; i++ ){
				cout << "X position is  "  << i  << endl ; 
				for(unsigned int j = 0 ; j < x_position[i].size() ; j++  ){
					x_position[i][j].print_interval() ; 
				}
			}	
		 }// end of debug == 1

		
		
		
		/*-----------------------------------------------------------------------------
		 *  我想先把 segment都給 建起來  
		 *  我測試到　60 * 60 ，　OASG  都不會出錯　，　以後　如果要修改
		 *  那就是改　Segment 跟　，　橫向的連線　，　這二塊　　，
		 *  尤其是後者　，　那塊有點亂，　暫且先維持這樣好了　
		 *-----------------------------------------------------------------------------*/
		if( debug == 1  ){cout << "Line 606 ,before  build Segment " << endl ;}
		vector<edge> Segment[my_level_info.plane_width] ; 

		/*-----------------------------------------------------------------------------
		 *  			check code , May 9
		cout << endl << endl << "Output Segment " << endl; 
		 *-----------------------------------------------------------------------------*/
		if( debug == 1  ){cout << endl << endl << "Output Segment " << endl;}
		for( int i = 0 ; i < my_level_info.plane_width ; i++ ){

			for(unsigned int j = 0 ; j < x_position[i].size()  ; j++ ){
				// 只有一個點　，　一定是pin 點　，　所以上下不會產生obstacle segment 
				// 在 有overlap 的影響下 ， 建obsatcle Seg 是一樣的 ， 要在 obsatcle 的
				// 左邊界內 才是 。
				if( x_position[i].size() == 1 ){
					// 一個點， 所以有二個Segment  , 上半塊Seg  
					Segment[i].push_back( edge( i ,my_level_info.plane_height - 1 , i ,x_position[i][j].print_y_position(), false)  );
					// 下半塊Seg 
					Segment[i].push_back( edge ( i ,x_position[i][j].print_y_position() , i , 0 , false ) );
				} 
				//bool meet_corner2 = false ; 	
				//int obstacle_id = 0 ; 
				if(  x_position[i].size() > 1 ){
					//  這區塊前面的 二個大 ix 都是為了 準確的判斷二點間是不是obstacle_Seg , 在多加了overalp_vertex後
					//  有什麼obsatcle Seg case 是我會沒加入的
					bool seg_state = false ; 
					// 這裡要改一下　，　要二個vertex , 都是obstacle corner 第一個的corner id 是２　，　第二個是１　，　才給true 
					// 這裡會怕　，　j指向最後一個　，　　而j+1 指不到東西 , 因為是要抓 obsatcle Seg 
					//  index 要指在合法的區域內   , 想想看 ， 什麼case下面這個if 不能進去
					//  二個都是pin 的時侯不能進去
					if( j <= x_position[i].size() - 2 && ( x_position[i][j].Is_pin_or_not() != true || 
						x_position[i][j+1].Is_pin_or_not() != true )  ){
						
						 // 直接給true 擋太大了　，　因為中間有的區塊　不是obstacle 
						 // 那用反面來思考　，　二個obstacle的id 要一樣　才給true 
						 // 用個loop 來看　如果二個要當obstacle_seg 的點中　，　存在一個為false 的pixel ，　那便不能給true 
						 // 這是避免我擋太大　，　有的obstacle 的橫軸線　，　會沒連起來　
						int false_pixel_coount = 0 ; 
						for( int k = x_position[i][j].print_y_position() ; k >= x_position[i][j+1].print_y_position() + 1 ; k-- ){
							if( my_level_info.obstacle_plane[i][k] == false ){
								false_pixel_coount++ ; 
							}
						}

						/*-----------------------------------------------------------------------------
						  等於０　，　代表這可以當成　obstacle_seg , 因為中間都是true pixel 
						  ... , 偏偏又有錯了　，　如果上下的二個vertex 中　，　緊鄰一個obstacle , 
						  那這不能算是個　obstacle seg 丫　　
						  我可以用完整的obsacle資訊來處理這個問題　，
						  這個segment  在obstacle的肚子　，　兩x端的內部　　，　且　y 軸間有相交　
						  j 跟j+ 1  的這二點 ， 連線 ， 跟obstacle 的肚子有相交
						 *-----------------------------------------------------------------------------*/
						bool close_case = false ; 
						for(unsigned int k = 0 ; k < my_level_info.complete_obstacle_information.size() ; k++  ){
							//先判斷x , 看有沒有在肚子　	 
							if( i < my_level_info.complete_obstacle_information[k].print_ru_corner_x() && 
							    i > my_level_info.complete_obstacle_information[k].print_ld_corner_x() &&  
							    x_position[i][j].print_y_position() > 
							    my_level_info.complete_obstacle_information[k].print_ld_corner_y() && 
							    x_position[i][j+1].print_y_position() < 
							    my_level_info.complete_obstacle_information[k].print_ru_corner_y() ){
								close_case = true ; 	 
								//有抓到就不用一直比下去了
								break ; 
							}
						}

						if( false_pixel_coount == 0 && close_case == false ){
							seg_state = true ; 
						}
					}// 用來 處理 normal 及 有一個 例外的case 
					// 二個點 ， 有分成pin 及obstacle_corner , 總共四個case 下面只抓一種 ， 上面抓了三種
					else if(j <= x_position[i].size() - 2 && seg_state == false && x_position[i][j].Is_pin_or_not() == true && 
						x_position[i][j+1].Is_pin_or_not() == true ){
						

						/*-----------------------------------------------------------------------------
						  等於０　，　代表這可以當成　obstacle_seg , 因為中間都是true pixel 
						  ... , 偏偏又有錯了　，　如果上下的二個vertex 中　，　緊鄰一個obstacle , 
						  那這不能算是個　obstacle seg 丫　　
						  我可以用完整的obsacle資訊來處理這個問題　，
						  這個segment  在obstacle的肚子　，　兩x端的內部　　，　且　y 軸間有相交　

						  兩端都是pin的case , 只要注意有沒有在obsatcle的left bound  , 
						 *-----------------------------------------------------------------------------*/
						bool anothor_case = false ; 
						for(unsigned int k = 0 ; k < my_level_info.complete_obstacle_information.size() ; k++  ){
							//先判斷x , 看有沒有在肚子　	 
							if( i == my_level_info.complete_obstacle_information[k].print_ld_corner_x() &&  
							    x_position[i][j].print_y_position() <=  
							    my_level_info.complete_obstacle_information[k].print_ru_corner_y() && 
							    x_position[i][j+1].print_y_position() >= 
							    my_level_info.complete_obstacle_information[k].print_ld_corner_y() ){
								anothor_case = true ; 	 
								//有抓到就不用一直比下去了
								break ; 
							}
						}

						if(  anothor_case == true ){
							seg_state = true ; 
						}
					}// 用來 處理 normal 及 有一個 例外的case 

					//head segment
					if( j == 0 ){
						// 第一個seg 不一定是fasle   , 丫　，　如果　obstalce vertex 放在最上面　
						Segment[i].push_back(edge(  i ,my_level_info.plane_height - 1 , i ,x_position[i][j].print_y_position(), false ) );
						Segment[i].push_back( edge ( i ,x_position[i][j].print_y_position() , 
									     i ,x_position[i][j+ 1].print_y_position(), seg_state )  );
					}
					//tail segment 
					else if( j == x_position[i].size() - 1 ){
						Segment[i].push_back( edge ( i , x_position[i][j].print_y_position() ,
									     i , 0 ,false ) );
					}
					// normal case 
					else{ 
						Segment[i].push_back( edge ( i ,x_position[i][j].print_y_position() , 
									     i ,x_position[i][j+ 1].print_y_position(), seg_state )  );
					}
				}
			}// second loop for
		}// first loop for

		
		if( debug == 1  ){cout << "Line 606 ,after build Segment " << endl ;}
		/*-----------------------------------------------------------------------------
		 * 			CHECK Code  ,  May 9 
		 *
		 *  			CHECK FOR SEGMENT 
		 *
		for(int i = 0 ; i < my_level_info.plane_width ; i++){
			cout << "X position is " << i << endl ; 
			cout << "size in Segment " << i << "  = " << Segment[i].size() << endl ; 
			for(unsigned int j = 0  ; j < Segment[i].size() ; j++ ){
				// output and see 
				Segment[i][j].print_seg() ; 	
			}
		}
		 *-----------------------------------------------------------------------------*/
		if( debug == 1  ){
			for(int i = 0 ; i < my_level_info.plane_width ; i++){
				cout << "X position is " << i << endl ; 
				cout << "size in Segment " << i << "  = " << Segment[i].size() << endl ; 
				for(unsigned int j = 0  ; j < Segment[i].size() ; j++ ){
					// output and see 
					Segment[i][j].print_seg() ; 	
				}
			}
		}// end of debug == 1 


		
		// 要做OASG的線了 , 先給定初值  
		unsigned int interval_line  = 0  , segment_line = 0  , alive_interval = 0 ; 
		// h ,k 分別指向 interval_line 及 segment_line 的內部 
		unsigned int h = 0  , k  = 0  ; 	
		bool how_to_move =  false ; 
		//要先抓出一個非零 的interval_line 免的下面  在抓interval的core_state會出錯
		while( x_position[interval_line].size() == 0  ){
			interval_line++ ; 
		}
		//都忘了 segment_line 也要非零的line
		segment_line = interval_line + 1 ; 
		while( Segment[segment_line].size() == 0  ){
			segment_line++ ; 
		}

		// 我要怎麼抓出  最後二行有東西的line 當 interval_line 及 segment_line 的終點 
		unsigned int deadline_of_interval_line = 0  , deadline_of_segment_line = 0  , count = 0 , location = my_level_info.plane_width - 1 ; 
		while( count < 2 ){
			if( x_position[location].size() > 0 && count == 0 ){
				deadline_of_segment_line = location ; 
				count++ ;
				
			}	
			else if(x_position[location].size() > 0 && count == 1 ){
				deadline_of_interval_line = location ; 
				count++ ;
			}
			location-- ;
		}	
		

		/*-----------------------------------------------------------------------------
		 * 				Check Code , May 9  
		cout << " deadline_of_interval_line  = " << deadline_of_interval_line << 
		"     deadline_of_segment_line  " << deadline_of_segment_line << endl ; 
		 *-----------------------------------------------------------------------------*/

		if( debug == 1  ){cout << "Line 796 ,before build edge of OASG  " << endl ;}
		int h_count = 1 ; 	
		do{

			// 這裡應該是 ， 抓current line 中 ， 還有多少個 alive_interval , 
			// 名字取不好  應該叫  alive_intervals 
			unsigned int x = 0 ; 
			alive_interval = 0 ; 
			while( x < x_position[interval_line].size()  ){
				if( x_position[interval_line][x].core_state() == true ){
					alive_interval++ ; 
				}
				x++ ;
			}
			
			if( debug == 1  ){
				cout << " where is interval_line = " << interval_line << "   where is segment_line  = " << segment_line << endl ; 
				cout << " alive_interval  =  " << alive_interval << endl ; 	
				cout << " h = " << h << "  k  = " <<  k << endl ;   
				h_count++ ; 
			}
			
			/*-----------------------------------------------------------------------------
			 *  // 指向一個還活著的interval , 如果都是false 怎麼半
			while( alive_interval > 0 && h < x_position[interval_line].size() &&  x_position[interval_line][h].core_state() == false ){
				h++ ;
			}
			
			 *-----------------------------------------------------------------------------*/
		
			
					
			// 有個error的情況 , 那就是  ,h 跟 k 都不動  , 就會一直loop  
			if( alive_interval > 0  &&  x_position[interval_line][h].core_state() == true ){
			//這裡要解決 , 傳入的seg , 含頭尾的情況 , 頭尾是有vertex 
				     
			how_to_move = x_position[interval_line][h].modify_interval_and_add_edgs_to_OASG( 
											   Segment[segment_line][k].print_seg_state() , 
										           segment_line ,
											   Segment[segment_line][k].print_left_of_seg(), 
											   Segment[segment_line][k].print_right_of_seg(),
											   OASG   ,my_level_info.plane_height - 1 , 
											   head_tail_state[segment_line][0], 
											   head_tail_state[segment_line][1] , 0 ); 	
				//下面這個是要印plane頭尾　，　是否有vertex 
				//cout << "x = " << segment_line << " , (" << head_tail_state[segment_line][0] 
				//<< " , " << head_tail_state[segment_line][1] << " ) "
				//<< endl ;     
					 
				if( how_to_move == true ){
					h++ ;	   
					while( h < x_position[interval_line].size() && x_position[interval_line][h].core_state() == false ){
						h++  ; 
					}
				}
				else {
					k++ ; 
				}
				// 不能讓h , k 跑超過  它們應有的大小
				if( h >= x_position[interval_line].size() ){
					h = x_position[interval_line].size()  ; 
				}
				if( k >= Segment[segment_line].size() ){
					k = Segment[segment_line].size()  ; 
				}
			}// alive_interval > 0  &&  x_position[interval_line][h].core_state() == true

			
			//cout << " h = " << h << "  k  = " <<  k << endl  << endl ; 

			// 向新的interval_line 做更改 , 那給好初值, 如果有 interval , 一直不被蓋住 , 那就要	
			// 我segment_line 跟 interval_line的終止點 設錯了 , 因為 我只抓 有東西的那些line , 所以 不會跑到最後一行
			if( alive_interval == 0  || ( segment_line == deadline_of_segment_line  && h == x_position[interval_line].size()   ) ){
				
				// 下面二個 while 是要指向  , 元素大於0 的那個line 	
				interval_line ++ ; 
				while( x_position[interval_line].size() == 0 && interval_line <= deadline_of_interval_line  ){
					interval_line++ ; 
				}
	 
				segment_line = interval_line + 1 ;
				while( Segment[segment_line].size() == 0 && segment_line <=  deadline_of_segment_line  ){
					segment_line++ ; 
				}
				// 初值設定
				h = 0 , k = 0 ; 
				while( h < x_position[interval_line].size() && x_position[interval_line][h].core_state() == false ){
						h++  ; 
				}
			}
	  
			// h 跑到底 還要看 是否有interval 存活  有活著的interval_line  就不能往前推進
			// 好像不用看k 只要 h 有跑到底就可以了,   因為能把h 推到底 , 下面就沒有其它東西了
			//   所以不用再看k 
			if( alive_interval > 0 && (  h == x_position[interval_line].size() || k == Segment[segment_line].size()  )    ){
				h = 0 , k = 0 ;  
				segment_line++ ;
				while( Segment[segment_line].size() == 0 && segment_line <=  deadline_of_segment_line  ){
					segment_line++ ; 
				}
				while( h < x_position[interval_line].size() && x_position[interval_line][h].core_state() == false ){
						h++  ; 
				}
			}
			
			if( interval_line > deadline_of_interval_line ){
				break ; 
			}	
					
		
		}while( interval_line <=  deadline_of_segment_line ) ; 

		if( debug == 1  ){cout << "Line 903 ,after build edge of OASG  " << endl ;}

		// 因為  我check 時 發現， 我這會出現， 有自已 指向自已的 edge 發生 ， 
		// 所以 我這裡 要多個 check 把這個error 拿掉 
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){
			if( OASG[i].print_node1_x() == OASG[i].print_node2_x() &&
			    OASG[i].print_node1_y() == OASG[i].print_node2_y() ){
				OASG.erase( OASG.begin() + i); 
				i--;
			}
		}// end of loop i 

		// 想不到  我的OASG 裡面 還有 edge 有相同的case 跑出來 
		for( unsigned int i = 0 ; i < OASG.size() ; i++ ){
			bool already_have = false ; 
			for( unsigned int j = i + 1 ; j < OASG.size() ; j++ ){
			 
				if( OASG[i].print_node1_x() == OASG[j].print_node1_x() &&
				    OASG[i].print_node1_y() == OASG[j].print_node1_y() && 
				    OASG[i].print_node2_x() == OASG[j].print_node2_x() && 
				    OASG[i].print_node2_y() == OASG[j].print_node2_y() ){
					already_have = true ;  
				}
				else if( OASG[i].print_node2_x() == OASG[j].print_node1_x() &&
					 OASG[i].print_node2_y() == OASG[j].print_node1_y() && 
					 OASG[i].print_node1_x() == OASG[j].print_node2_x() && 
					 OASG[i].print_node1_y() == OASG[j].print_node2_y() ){
						already_have = true ;  
				}
				if( already_have == true  ){
					// 把這個相同的給刪掉 ， 再給個false 往後看 
					OASG.erase( OASG.begin() + j ); 
					j-- ; 
					already_have = false ; 
				}
			}// end of loop j 

		}// end of loop i 

		/*-----------------------------------------------------------------------------
		 *  				check code , May 9
		cout << "edges in OASG is " << OASG.size() << endl ; //　確實有抓edge 進來　，　但是　有錯誤
		這裡  因為全部的edge都出來了 ， 所以  我要分群， 得知那些pin點是在同一群的  
		分群完後  ， 並不用 之道 那一群的id  是多少  ， 我只要之道 ， 從id取出來的值 
		相同就是同個group ， 不同就是  不同個group  , 我在這個func 內的最最上面  用個新的結構
		叫all_vertex 去放  ， 所有的vertex  , 以便我之後 回復 plane 分群的  初值  
		奇怪了  ， 怎麼卡在 922 這裡 
		 *-----------------------------------------------------------------------------*/
		if( debug == 1  ){cout << "Line 922 ,start to seperate group  " << endl ;}

		vector<edge> seperate_group[ OASG.size() ] ; 
		int count_two = 0 ; 
		 for(unsigned int i = 0 ; i < OASG.size() ; i++ ){
		 	 
			if( debug == 1  ){ cout << " i =   " << i  ;}
			int node1_x = 	OASG[ i ].print_node1_x() ; 
			int node1_y = 	OASG[ i ].print_node1_y() ; 

			int node2_x = 	OASG[ i ].print_node2_x() ; 
			int node2_y = 	OASG[ i ].print_node2_y() ; 
			// 先判斷那二個點 ， 有沒有其它的subtree   , 沒有的話  ， 就依 count_two 值 放到 
			// 指定的箱子裡面 
			if( my_level_info.which_forest[node1_x][node1_y] == -1 &&  my_level_info.which_forest[node2_x][node2_y] == -1 ){
				if( debug == 1  ){ cout << " Line 938   "  << endl ;}
				// 給值 ， 再放到  ， vector 中   
				my_level_info.which_forest[node1_x][node1_y] = count_two ; 
				my_level_info.which_forest[node2_x][node2_y] = count_two ; 
				// 之後是要利用 ， 下面二個點 ， 去改上面的id 
				seperate_group[count_two].push_back( OASG[i] ) ; 
				count_two++ ; 
				//cout << "line   "<< 1771  << endl ; 
			}
			else if( my_level_info.which_forest[node1_x][node1_y] >= 0 &&  my_level_info.which_forest[node2_x][node2_y] == -1 ){
				if( debug == 1  ){ cout << " Line 948   "  << endl ;}
				// 給值 ， 再放到  ， vector 中   
				my_level_info.which_forest[node2_x][node2_y] = my_level_info.which_forest[node1_x][node1_y] ; 
				// 下面這  好像不是放在 count_two  所在的箱子 ， 而是放在  >= 0 的那個箱子才對 
				seperate_group[ my_level_info.which_forest[node1_x][node1_y] ].push_back( OASG[i] ) ; 
				// cout << "line   "<< 1778  << endl ; 
			}
			else if( my_level_info.which_forest[node1_x][node1_y] == -1 &&  my_level_info.which_forest[node2_x][node2_y] >= 0 ){
				if( debug == 1  ){ cout << " Line 956 "  << endl ;}
				// 給值 ， 再放到  ， vector 中   
				my_level_info.which_forest[node1_x][node1_y] = my_level_info.which_forest[node2_x][node2_y]; 
				seperate_group[  my_level_info.which_forest[node2_x][node2_y] ].push_back(  OASG[i] ) ; 
				// cout << "line   "<< 1784 << endl ; 
			}
			else if( my_level_info.which_forest[node1_x][node1_y] >= 0 &&  my_level_info.which_forest[node2_x][node2_y] >= 0 && 
				 my_level_info.which_forest[node1_x][node1_y] != my_level_info.which_forest[node2_x][node2_y] ){

				if( debug == 1  ){ cout << " Line 964 "  << endl ;}
				// 為什麼進來這個  會出錯呢  
				// cout << "line   "<< 1788 << endl ; 
				
				// 下面二個loop 是依大小 ， 來看 ， 是誰要合併誰  
				// 由下面一行可以之道 ， node2 的 被蓋掉  ， 也就是說  ， node1所在的 subtree  , 量比較大  
				int which_group = my_level_info.which_forest[node2_x][node2_y] ; 
				int des_group   = my_level_info.which_forest[node1_x][node1_y] ; 

				my_level_info.which_forest[node2_x][node2_y] = my_level_info.which_forest[node1_x][node1_y] ; 
				seperate_group[  des_group ].push_back(  OASG[i] ) ; 

				// cout <<  "seperate_group[ which_group ].size() = " << seperate_group[ which_group ].size() << endl ; 
				for(unsigned int k = 0 ; k < seperate_group[ which_group ].size() ; k++ ){
					// cout << " j  =" << j << endl ; 
					// 班過去 node1 那個箱子 ， 順便改  ， 在 which_forest  上的group id 	 
					int tmp_x = -1 , tmp_y = -1 ; 
					tmp_x = seperate_group[ which_group ][k].print_node1_x() ; 
					tmp_y = seperate_group[ which_group ][k].print_node1_y() ; 
					my_level_info.which_forest[tmp_x][tmp_y] =  des_group ; 

					tmp_x = seperate_group[ which_group ][k].print_node2_x() ; 
					tmp_y = seperate_group[ which_group ][k].print_node2_y() ; 
					my_level_info.which_forest[tmp_x][tmp_y] =  des_group ; 
					// 要把東西都給班過去   , 為什麼下面這行  會讓我的code 當了呢   , 告夭 ， 我之道為什麼了拉  
					// 我把  node1箱子的index 給了  node2  , 又 從它那取箱子的index , 等於我都在處理同個箱子 按  
					seperate_group[ des_group ].push_back( seperate_group[which_group][k] ) ; 
				}// end of loop j 

				// 班完後 ， 把另一邊的東西給刪了 
				seperate_group[  which_group  ].clear()  ;

			}// 都在不同的subtree上 
		 }// scan all OASG edges 

		if( debug == 1  ){cout << "Line 991 ,End of seperate group  " << endl ;}

		// 把 which_forest 中 ， 分出來的  ， pin點分群的id ， 給放到 ， vertex 中的  which_group  , 這個值中  
		for( unsigned int i = 0 ; i < my_level_info.only_pin.size() ; i++ ){
			int x  = my_level_info.only_pin[i].print_x_position() ; 
			int y  = my_level_info.only_pin[i].print_y_position() ; 
			my_level_info.only_pin[i].which_group = my_level_info.which_forest[x][y] ; 
		}// scan all vertex 
		if( debug == 1  ){cout << "Line 999 " << endl ;}


	}// end of build_OASG


/*-----------------------------------------------------------------------------
 *   這裡要多寫個  用來 多處理 projection vertex  的 OASG   , 只是要多處理這個點而已
 *   整體上來講 ， 也還是個 2D 的東西 
 *   要改的地方大概有 ， 1.  吃的vertex 要含有projection  , 然後是 
 *   跟 obstacle corner 同類 且 ， corner id = 0  ;    我想呢 要加的東西不多
 *-----------------------------------------------------------------------------*/
void my_overlap_OASG::build_OASG_can_handle_projection_vertex( level_info &my_level_info ){	

		int debug = 0  ; 
		if( debug == 1 ){
			cout << "Line 922" << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 *  這下二個 x y position 是用來存 各個位置上 有什麼vertex   
		 *  而且還要存下 一個x位置下 ， 最高跟低點 有沒有被用走
		 *  這裡 因為是要處理 overlap  , 所以 建interval 跟segment 都要修改
		 *-----------------------------------------------------------------------------*/
		vector<vertex> x_position[my_level_info.plane_width] ;
		vector<vertex> y_position[my_level_info.plane_height] ;
		bool **head_tail_state = new bool *[my_level_info.plane_width] ; 
		for( int i = 0 ; i < my_level_info.plane_width ; i++){
			head_tail_state[i] = new bool[2];
		}
	 
		// 初始化, head_tail_state  . [0] 用來記head  , [1]用來記tail 
		// true 代表那個位置是vertex 
		for( int i = 0 ; i < my_level_info.plane_width ; i++){
			for( int j = 0 ; j < 2 ; j++){
				head_tail_state[i][j] = false ; 
			}
		}


		if( debug == 1 ){
			cout << "Line 946" << endl ;  
		}
		//my_level_info.array_vertex 內的東西　，　放到相對應的x_position內, my_level_info.total  是 指my_level_info.array_vertex 內的元素數目
		//這裡是要多處理  projection vertex , 所以 要看的不是 array_vertex 而是用 vector存的 array_vertex_vector
		//我已經在 Projection_handler 中 ， 把 Overlap_Vertex 給塞進去 array_vertex_vector 了  ， 所以不用在多一個loop 塞進去
		// 新抓的 projection_vertex  也有被放進來 
		for( int i = 0 ; i < my_level_info.plane_width ; i++){
			for(unsigned int j = 0 ; j < my_level_info.array_vertex_vector.size() ; j++){
				if( my_level_info.array_vertex_vector[j].print_x_position() == i){
					x_position[i].push_back( my_level_info.array_vertex_vector[j] );

					if( my_level_info.array_vertex_vector[j].print_y_position() == my_level_info.plane_height - 1  ){
						head_tail_state[i][0] = true ;  
					}
					if(my_level_info.array_vertex_vector[j].print_y_position() == 0 ){
						head_tail_state[i][1] = true ;  
					}
				}
			}// end of for j
		}//end of for i 

		if( debug == 1  ){
			// 這裡要看看 每個 x_position 中 ，不在限定範圍的 ， 把它給印出來
			for(int  i = 0 ; i < my_level_info.plane_width ; i++ ){
				if( x_position[i].size() > 0 ){
					for( unsigned int j = 0 ; j < x_position[i].size() ; j++ ){
						// 因為是看x_position 所以 只要check y 的值就好了 
						if(  x_position[i][j].print_y_position() < 0 || 
						     x_position[i][j].print_y_position() > my_level_info.plane_height  ){
							x_position[i][j].print_out_for_check() ; 	 
						}
					}
				}// end of ix 
			}
		}// scan all x_position 

		// 將東西放到對應的y_position內	
		for( int i = 0 ; i < my_level_info.plane_height ; i++){
			for(unsigned int j = 0 ; j < my_level_info.array_vertex_vector.size() ; j++){
				if( my_level_info.array_vertex_vector[j].print_y_position() == i){
					y_position[i].push_back( my_level_info.array_vertex_vector[j] );
				}
			}// end of for j
		}//end of for i 

		/*-----------------------------------------------------------------------------
		 *  			CHECK vertex 的值 , 跟head_tail_state 
		 *
		 *		for(int i = 0 ; i < my_level_info.total ; i++){
				cout << "x  = "<< my_level_info.array_vertex[i].print_x_position() << " ( " <<  
				my_level_info.array_vertex[i].print_x_position() << " , " 
				<< my_level_info.array_vertex[i].print_y_position() << " ) " << endl << endl ; 
			}	
			
			for( int i = 0 ; i < my_level_info.plane_width ; i++){
					cout << "x = " << i <<  " ,  ( " << head_tail_state[i][0] << " , " << head_tail_state[i][1] << "  ) " << endl;  
			}
		 *
		 *
		 *-----------------------------------------------------------------------------*/
		
		/*-----------------------------------------------------------------------------
		 *驗證東西是否存入了， 
		int sizeofx_position = 0 ;
		for(int i = 0 ; i < my_level_info.plane_width ; i++ ){
			sizeofx_position = sizeofx_position + x_position[i].size() ; 
		}
		
		cout << "my_level_info.total = " << my_level_info.total << endl ;
		cout << "x_position = " << sizeofx_position << endl ; 

		 *-----------------------------------------------------------------------------*/
		



		/*-----------------------------------------------------------------------------
		 * 接下來要排序各個x_position , 內的y軸　，　以降蟇的方式排序，　
		 * 方便我之後抓　segment  這裡要用Bubble Sort     
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < my_level_info.plane_width ; i++){
			sort( x_position[i].begin() , x_position[i].end() , sort_y_position_in_overlap_OASG  );
			// 按  自已寫個sort 好了 
			// 哇操  這裡面存的是vertex  , 我還要寫個 assignement operator 好在不用寫了 
		}			

		for(int i = 0 ; i < my_level_info.plane_height; i++){
			 sort( y_position[i].begin() , y_position[i].end() , sort_x_position_in_overlap_OASG );
		}			
		
		

		/*-----------------------------------------------------------------------------
			印出各個x_position 的內容 , 看有否真的排序
			裡面沒東西的 就印出This is empty 

		 *  for(int i = 0 ; i < my_level_info.plane_width ; i++){
			cout << "x_position is " << i  << endl ; 
			if(x_position[i].size() == 0){
				cout << "This is empty " << endl ; 
			}
			for(unsigned int j = 0 ; j < x_position[i].size() ; j++){
				cout  << x_position[i][j].print_y_position() << "   ";
			
			}		
			cout << endl << endl ; 
		}
		
			// 這裡是印出　y_position 的內容　看有否排序
			for(int i = 0 ; i < my_level_info.plane_height; i++){
			cout << "y_position is " << i  << endl ; 
			if(y_position[i].size() == 0){
				cout << "This is empty " << endl ; 
			}
			for(unsigned int j = 0 ; j < y_position[i].size() ; j++){
				cout  << y_position[i][j].print_x_position() << "   ";
			
			}		
			cout << endl << endl ; 
		}
		 *-----------------------------------------------------------------------------*/
			



		
		/*-----------------------------------------------------------------------------
		 * 用一個vector  存edge  這種data 名為my_level_info.my_level_OASG 
		 * 在vertex.h 內　建個bounding box interval , 存有l_boundary , r_boundary  , 
		 * 以及其中心的core , 
		 * 再建一個func , 用來判斷core 是否還存活著　，　因為core活著　才能更新這個
		 * 		interval 
		 *    another func  , 接收seg information 以及是 obstacle seg or not , 
		 *     		      不同性質的seg ，　是做不同的事
		 *-----------------------------------------------------------------------------*/
		// vector<edge> my_level_info.my_level_OASG ;  原本是放這的  ， 因為要切割成獨立 ， 所以 
		// 要放到最上面去  , 下面這個 vector 是存  這個 x 位置下 ， 有那些 obstacle 有跨到它 
		vector<int>  obstacle_cross[my_level_info.plane_width] ;
	 
		map<int,obstacle>::iterator it ;  

		// 喔 ， obstacle_map 看起來是 是要存  一個 mapping  用 obstacle_id  來對到 obstacle 的一個東西 
		/*-----------------------------------------------------------------------------
		 *   我每層皆有設 置obstacle_map 了 ， 所以 ， 要找對應還要找到 指定的是那一層 ， 
		 *   在 level_info 中有設置了 ， 所以這裡不用 再 做一次
		for(unsigned int i = 0 ; i < my_level_info.complete_obstacle_information.size() ; i++  ){
			obstacle_map[ my_level_info.complete_obstacle_information[i].print_obstacle_id() ] =  my_level_info.complete_obstacle_information[i] ; 
		}
		 *-----------------------------------------------------------------------------*/
	 
		for(int i = 0 ; i < my_level_info.plane_width ; i++  ){
			if( x_position[i].size() > 0  ){
				for(unsigned int j = 0 ; j < my_level_info.complete_obstacle_information.size() ; j++ ){
					// 如果obsacle 有擋到這個x位置　，　便把這個obstacle_id 記下來 , 這裡應有包含 obstacle 的頭跟尾
					// 也就是 obstacle 有在這範圍內出現
					if( i >= my_level_info.complete_obstacle_information[j].print_ld_corner_x() &&
					    i <= my_level_info.complete_obstacle_information[j].print_ru_corner_x() ){
						obstacle_cross[i].push_back( my_level_info.complete_obstacle_information[j].print_obstacle_id() ); 
					}
				}// inner loop j 
			}
		}// 把各個x_position中　，　被那些obstacle 擋到　，　記下其obstacle_id 

		/*-----------------------------------------------------------------------------
				// 把值抓出來看看吧
				cout << "output map info :" << endl ; 
				for(it = obstacle_map.begin() ; it != obstacle_map.end() ; it++ ){
					cout << (*it).first ; 
					(*it).second.print_out_for_check() ;
				}
		 *-----------------------------------------------------------------------------*/
		

		if( debug == 1 ){
			cout << "Line 1102 " << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 接下來是我　新方法的核心　，　要先建　my_level_info.my_level_OASG 中的　直線　
		 這裡的建直線 是指 my_level_info.my_level_OASG 內 會看的到的直線 ， 而不是指 segment interval 
		 下面這塊 應該不用改  ， 因為只是建直線 ， 而沒有再判斷 是pin 還是 corner 的地方  
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < my_level_info.plane_width ; i++ ){
			// 因為這裡是要建直線　，　所以我只要考慮　，　x_position中　，　vertex 數目　>= 2 的即可 
			if( x_position[i].size() >= 2 && obstacle_cross[i].size() > 0 ){
				/*-----------------------------------------------------------------------------
				 我要scan obstacle_cross , 抓出它的key , 再對到obstacle info 來用　 
				 中間如果被obstacle的肚子擋到　，　就不能連線 , 我想起來 下面的 index 
				 為什麼 只跑到 size - 1 , 了 因為 我會兩兩這樣看 ， 這個會往下一個看 
				 *-----------------------------------------------------------------------------*/
				for(unsigned int j = 0 ; j < x_position[i].size() - 1 ; j++ ){

					bool bad_case = false ; 
					for(unsigned int k = 0 ; k < obstacle_cross[i].size() ; k++  ){
						// 這裡的key 裡應是抓到 obstacle_id , 然後把 obstacle 的資訊給抓出來 
						int key = 0 ; 
						key = obstacle_cross[i][k] ;
						it = my_level_info.obstacle_map.find(key) ; 
						// 先判斷 y seg 有沒有相交　，　再判斷是交在那裡　 交在頭尾才連線　，　其它就不理它　
						// 只看肚子 , 頭尾沒有差 還是可以直接連  , 只要不是被肚子擋到的就可以連
						if( i != (*it).second.print_ld_corner_x() && i != (*it).second.print_ru_corner_x() ){
							// 肚子如果有相交就不連　,  下面二個判斷式　，　不能加上　＝　號　，　
							// 　會出事　，　二個相貼的obstacle 會出錯　
							if(  x_position[i][j].print_y_position()   >  (*it).second.print_ld_corner_y() && 
							     x_position[i][j+1].print_y_position() <  (*it).second.print_ru_corner_y() ){
								bad_case = true ;  
								break ; 
							}
						 
						}// 只看肚子 ,不看頭尾
						
					}// loop k , scan all obstacle , 在這範圍的obstacle , 這樣就不用全部都看了 

					if( bad_case == false && x_position[i][j].core_state() == true && x_position[i][j + 1].core_state() == true ){
						my_level_info.my_level_OASG.push_back( edge(  x_position[i][j].print_x_position() ,
											      x_position[i][j].print_y_position() , 
											      my_level_info.which_level           , 
											      x_position[i][j + 1].print_x_position() ,
											      x_position[i][j + 1].print_y_position() , 
											      my_level_info.which_level               )  );		 
					} 

				}// loop j
				
			}
			else if( x_position[i].size() >= 2 && obstacle_cross[i].size() ==  0 ){
				for(unsigned int j = 0 ; j < x_position[i].size() - 1  ; j++ ){

					if( x_position[i][j].core_state() == true && x_position[i][j + 1].core_state() == true ){
						my_level_info.my_level_OASG.push_back( edge(  x_position[i][j].print_x_position() ,
											      x_position[i][j].print_y_position() , 
											      my_level_info.which_level           , 
											      x_position[i][j + 1].print_x_position() ,
											      x_position[i][j + 1].print_y_position() ,
											      my_level_info.which_level               )  ); 
					}// end of loop if

				}// end of loop for 
				 
			}// end of else if

		}// end of 建my_level_info.my_level_OASG 的直線　

		


		if( debug == 1 ){
			cout << "Line 1174 " << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 好了　，　接下來　，　要用新方法　，　重建interval   了　，　如果　成功　便可以把下面那一大塊
		 給整個刪了　先由　，　vertex 間的關係　，　及自已的屬性　，　先建個大概的 interval  , 只有大概丫 ， 難怪我覺得怪怪的 
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < my_level_info.plane_width ; i++  ){
			if( debug == 1 && i == 87 ){
				cout << "my_level_info.plane_width = " << i << endl ; 
				cout << " x_position[i].size() = " <<  x_position[i].size() << endl ; 
			}
			// 只有一個點的情況　，　一定是　pin 點, 哇 ， 在3D 下就 不是這樣了  ， 因為會有 別層 project 過來的projection vertex 
			if( x_position[i].size() == 1 ){
				x_position[i][0].set_l_interval( my_level_info.plane_height - 1 ) ;	 
				x_position[i][0].set_r_interval( 0 ) ;	 
			}
			else if( x_position[i].size() > 1 ){
				// 為什麼都是當在87的位置呢  , ??
				for(unsigned int j = 0 ; j < x_position[i].size()  ; j++  ){

					/*-----------------------------------------------------------------------------
					if( debug == 1 && j == 0 ){
						cout << "j = " << j << endl ; 
						// cout << "x_position[i][j].core_state() = " << x_position[i][j].core_state() << endl ; 	
					}
					 *-----------------------------------------------------------------------------*/
					// head 
					if( j == 0 && x_position[i][j].core_state() == true ){
						x_position[i][j].set_l_interval( my_level_info.plane_height - 1 ) ;	 
						x_position[i][j].set_r_interval( x_position[i][j + 1].print_y_position() + 1  ) ;	 
					}
					// tail 
					else if( j == x_position[i].size() - 1 && x_position[i][j].core_state() == true ){
						x_position[i][j].set_l_interval( x_position[i][j - 1].print_y_position() - 1 ) ;	 
						x_position[i][j].set_r_interval( 0 ) ;	 
						break ; 	 
					}
					// normal case 
					else {
						// 這裡還要針對　，　corner 1,2 做調整　，　週其r , l  , 因為 corner 1,2 會被到彼此
						if( x_position[i][j].print_corner_id() == 1 && x_position[i][j].core_state() == true ){
							x_position[i][j].set_l_interval( x_position[i][j].print_y_position() );
							x_position[i][j].set_r_interval( x_position[i][j + 1].print_y_position() + 1  ) ;	 
						 
						}
						else if( x_position[i][j].print_corner_id() == 2 && x_position[i][j].core_state() == true ){
							x_position[i][j].set_l_interval( x_position[i][j - 1].print_y_position() - 1 ) ;	 
							x_position[i][j].set_r_interval( x_position[i][j].print_y_position() );
						 
						}
						else if( x_position[i][j].core_state() == true){
							// 喔 ， 我抓到了 ， 因為第一個vertex 是 false , 所以會掉到這裡 
							x_position[i][j].set_l_interval( x_position[i][j - 1].print_y_position() - 1 ) ;	 
							x_position[i][j].set_r_interval( x_position[i][j + 1].print_y_position() + 1  ) ;	 
						}
					}// end of else block 
				}// loop j 
			}// end of else if

		}// end of loop i 

			

		if( debug == 1 ){
			cout << "Line 1226 " << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 我想把　，　在obstacle左邊界內的vertex 先給設成false 
		 我之道  我這裡在想什麼了  ， 之所以要先找 corner 2 , 是因為 我是 
		 由上往下掃 ， 所以先看到 corner 2 ,  在未看到 corner 1 之前 ， 
		 之間的 vertex 就先直接設定成 false  了  ， 免的 它會往後面連 
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < my_level_info.plane_width ; i++ ){

			bool meet_corner_2 = false ; 
			int obstacle_id = 0 ; 
			for(unsigned int j = 0  ; j < x_position[i].size() ; j++  ){
				if( x_position[i][j].Is_pin_or_not() == false && x_position[i][j].print_corner_id() == 2 ){
					// 撞到 corner2 且 要抓出他的 id  
					meet_corner_2 = true ;  
					obstacle_id = x_position[i][j].print_obstacle_id() ;
				}
				// 這裡 我怕corner 2 一設定完 ， 就把自已設成false 
				if( meet_corner_2 == true && x_position[i][j].Is_pin_or_not() == true ){
					x_position[i][j].set_core_state(false );	 
				}
				if( meet_corner_2 == true && x_position[i][j].Is_pin_or_not() == false && x_position[i][j].print_obstacle_id() != obstacle_id ){
					x_position[i][j].set_core_state(false );	 
				}
				// 下面這個 if 是新加的 ， 要能有抓到 projection_vertex , 因為 projection_vertex 的設定就是  false 及 0 
				if( meet_corner_2 == true && x_position[i][j].Is_pin_or_not() == false && x_position[i][j].print_corner_id() == 0 ){
					x_position[i][j].set_core_state(false );	 
				}

				if( x_position[i][j].Is_pin_or_not() == false && obstacle_id ==  x_position[i][j].print_obstacle_id()
				    && x_position[i][j].print_corner_id() == 1 ){
					meet_corner_2 = false ;  
				}	 
			}// end of loop j 

		}// end of loop i 




		if( debug == 1 ){
			cout << "Line 1268 " << endl ;  
		}
		// 用obstacle的肚子　，　做微調微調微調微調了　， 下面要有判斷式　，　記得　抓為true 的vertex 　
		for(int i = 0 ; i < my_level_info.plane_width ; i++  ){
			int l = 0 , r = 0 ; 
			for(unsigned int j = 0 ; j < x_position[i].size()  ; j++  ){

				l = x_position[i][j].print_l_interval() ;
				r = x_position[i][j].print_r_interval() ;

				if( x_position[i][j].core_state() == true ){
						 
					for(unsigned int k = 0 ; k < obstacle_cross[i].size() ; k++  ){
						int key = 0 ; 
						key = obstacle_cross[i][k] ;
						it = my_level_info.obstacle_map.find(key) ; 
						// 先判斷 y seg 有沒有相交　，　再判斷是交在那裡　 交在頭尾才連線　，　其它就不理它　
						// 只看肚子
						if( i != (*it).second.print_ld_corner_x() && i != (*it).second.print_ru_corner_x() ){

							// 抓肚子　，　更新出　離此點　最近的　l , r  
							// 先抓左　，　在core 的左區塊
							if( (*it).second.print_ld_corner_y() >= x_position[i][j].print_y_position() && 
							    (*it).second.print_ld_corner_y() <  l 			   	    ){
								l = (*it).second.print_ld_corner_y() ; 
							}
							// 再抓　，　core 的右區塊　
							if( (*it).second.print_ru_corner_y() <= x_position[i][j].print_y_position() && 
							    (*it).second.print_ru_corner_y() >  r 			   	    ){
								r = (*it).second.print_ru_corner_y() ; 
							}

						}// 只看肚子 ,不看頭尾
						
					}// loop k
					
					// 抓出l , r 後　，　要分其是pin  corner ,1,2,3,4 的不同　去分別設定	
					if( x_position[i][j].Is_pin_or_not() == true ){
						x_position[i][j].set_l_interval( l ) ;	 
						x_position[i][j].set_r_interval( r ) ;	 
					}
					else if(  x_position[i][j].print_corner_id() == 1  ){
						  x_position[i][j].set_r_interval( r ) ;	 
						  x_position[i][j].set_l_interval( x_position[i][j].print_y_position() ) ;	 
					}
					else if(  x_position[i][j].print_corner_id() == 2  ){
						  x_position[i][j].set_l_interval( l ) ;	 
						  x_position[i][j].set_r_interval( x_position[i][j].print_y_position() ) ;	 
					}
					else if(   x_position[i][j].print_corner_id() == 4 ){
						  x_position[i][j].set_r_interval( r ) ;	 
					}
					else if(   x_position[i][j].print_corner_id() == 3 ){
						  x_position[i][j].set_l_interval( l ) ;	 
					}
					// 下面這也是新加的 ， 要能看到 projection_vertex
					// 在 粗略的 interval 設定下  ， 我這個 projection_vertex 有否 放進去
					// 因為我 l r boundary 都是設成 －1  , 應該是有  我上面精略的設定 還蠻 normal 的  
					else if( x_position[i][j].Is_pin_or_not() == false && x_position[i][j].print_corner_id() == 0  ){
						  x_position[i][j].set_l_interval( l ) ;	 
						  x_position[i][j].set_r_interval( r ) ;	 
					}// this is projection_vertex

				}// 為true  才有更新interval 的必要
			}// loop j 
		}// end of loop i 


		/*-----------------------------------------------------------------------------
		 * 				Check Code , May 9 
		// 弄個檢查interval 的東西吧　　，　print 出　interval 
		for( int i = 0 ; i < my_level_info.plane_width ; i++ ){
			cout << "X position is  "  << i  << endl ; 
			for(unsigned int j = 0 ; j < x_position[i].size() ; j++  ){
				x_position[i][j].print_interval() ; 
			}
		}	
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 * 				CHECK FOR INTERVAL 	
		 * 		//這裡要檢查　，　看每個x位置的edge 是不是　頭尾相接 , 如果頭尾相接　那就沒有判斷到中間的
		 *
		//obstacle  了　　，　那只能印圖來看　有沒有畫錯　, 下面　這段code 只能　抓點跟點間的edge 
		//不能判斷中間有沒有obstacle擋到　
		for(unsigned int j = 0 ; j < my_level_info.my_level_OASG.size() ; j++  ){
			my_level_info.my_level_OASG[j].print_edge_information_for_gnu() ; 
		} 
		 *-----------------------------------------------------------------------------*/
		
		
		
		/*-----------------------------------------------------------------------------
		 *  我想先把 segment都給 建起來  
		 *  我測試到　60 * 60 ，　my_level_info.my_level_OASG  都不會出錯　，　以後　如果要修改
		 *  那就是改　Segment 跟　，　橫向的連線　，　這二塊　　，
		 *  尤其是後者　，　那塊有點亂，　暫且先維持這樣好了　
		 *  Segment 是用來存什麼的 我有點忘了 
		 *   這裡 好像也是由上往下掃  建立 每個點的 Segment 
		 *  我以segment 為單位 ， 來做最後的連線比對 ， 如果這個segment 中間 有 
		 *  obstacle 
		 *   
		 *   下面這一塊要做的好像就只是 粗略的區分 ， 這Segment 是不是 obstacle_seg 
		 *-----------------------------------------------------------------------------*/
		vector<edge> Segment[my_level_info.plane_width] ; 

		if( debug == 1 ){
			cout << "Line 1375 " << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 * 				Check Code , May 9 
		cout << endl << endl << "Output Segment " << endl; 
		 *-----------------------------------------------------------------------------*/
		for( int i = 0 ; i < my_level_info.plane_width ; i++ ){
			for(unsigned int j = 0 ; j < x_position[i].size()  ; j++ ){
				if( debug == 1 ){
					cout << "Line 1393 " << endl ;  
				}
				// 只有一個點　，　一定是pin 點　，　所以上下不會產生obstacle segment 
				if( x_position[i].size() == 1 ){

					/*-----------------------------------------------------------------------------
					 *  
					 有點想起來了  ， 我比較的單位是Segment 也就是 一個vertex interval 的左區塊跟右區塊 
					 我要建好整體的 Segment , 才能做最後比對的動作  , 奇怪了 即便是只有一個點 ， 我segment 上下 
					 一定會打到頂嗎  , 第五個參數 的 true false  , 是代表  這個 Segment 是 obstacle_seg or not 
					 喔  我之道了 ， obstacle_seg 是指  這二點間 要建立的seg , 如果這個區間 都是true pixel  
					 則代表是 obstacle_seg  ， 被肚子擋到似乎不算  ,  奇怪了 ，第三個參數的i 是什麼東西 , 
					 喔 ， 前四個參數 代表 ， 二個點 ， 一個上方旳  一個下方的點 , 這裡 不放 z 的參數 也沒關係丫
					 我是分開處理的哎 
					 *-----------------------------------------------------------------------------*/
					Segment[i].push_back( edge( i ,my_level_info.plane_height - 1     , my_level_info.which_level , 
								    i ,x_position[i][j].print_y_position(), my_level_info.which_level ,  false)  );
					Segment[i].push_back( edge ( i ,x_position[i][j].print_y_position() , my_level_info.which_level ,
								     i , 0 				    , my_level_info.which_level , false ) );
				} 
				//bool meet_corner2 = false ; 	
				//int obstacle_id = 0 ; 
				if( debug == 1 ){
					cout << "Line 1416 " << endl ;  
				}
				if(  x_position[i].size() > 1 ){
					// 這裡的 seg_state = fasle 應該是 還沒死的意思 
					bool seg_state = false ; 

					/*-----------------------------------------------------------------------------
					 *  
					 這裡要改一下　，　要二個vertex , 都是obstacle corner 第一個的corner id 是２　，　第二個是１　，　才給true 
					 這裡會怕　，　j指向最後一個　，　　而j+1 指不到東西
					 這裡好像要改 ， 因為 ， projection 是個有 pin 點性質 ， 但我卻把它放在 corner那 
					 下面這個if 的判段  是  只要 頭尾這二個vertex   只要一個是 obstacle corner 就可以了 
					 我也不之道為什麼要減3 ， 只不過 減2 ， 會有錯  
					 *-----------------------------------------------------------------------------*/
					if( j <= x_position[i].size() - 2 && ( x_position[i][j].Is_pin_or_not() != true || 
						x_position[i][j+1].Is_pin_or_not() != true ) ){
						
						if( debug == 1 ){
							cout << "Line 1436 " << endl ;  
						}
						/*-----------------------------------------------------------------------------
						 *  
						  直接給true 擋太大了　，　因為中間有的區塊　不是obstacle 
						  那用反面來思考　，　二個obstacle的id 要一樣　才給true 
						  用個loop 來看　如果二個要當obstacle_seg 的點中　，　存在一個為false 的pixel ，　那便不能給true 
						  這是避免我擋太大　，　有的obstacle 的橫軸線　，　會沒連起來　
						  false_pixel_coount 我想呢 應是 要抓說  這二點同時在 obstacle 的左右邊界上 ， 
						  原來這裡 我是利用 一個個pixel 往下掃來看 
						  下面這個fox 的無窮loop 的情 是發生在 ， j+1 指向的y  , 跟 j 指向的y 相同 ， 好像就會當了 
						  j+1 有指向一個 負值是 -805306368 , 怎麼回事丫 
						 *-----------------------------------------------------------------------------*/
						int false_pixel_coount = 0 ; 
						int count = 0 ; 
						for( int k = x_position[i][j].print_y_position() ; k >= x_position[i][j+1].print_y_position() + 1 ; k-- ){
							if( debug == 1 && count < 3 ){
								cout << endl  << "x_position[i].size() = " << x_position[i].size() << endl ; 
								cout << " j  = " << j  << endl ; 
								cout << endl << " x_position[i][j].print_y_position() = " 
								<< x_position[i][j].print_y_position() << endl ;  

								cout << " x_position[i][j+1].print_y_position() = " 
								<< x_position[i][j+1].print_y_position() << endl ;  

								cout << " x_position[i][j+1].Is_pin_or_not() = " 
								<< x_position[i][j+1].Is_pin_or_not() << endl ;  

								cout << " x_position[i][j+1].print_corner_id() = " 
								<< x_position[i][j+1].print_corner_id() << endl ;  

								cout << " x_position[i][j+1].print_obstacle_id() = " 
								<< x_position[i][j+1].print_obstacle_id() << endl ;  
								count++ ; 
							}
							if( my_level_info.obstacle_plane[i][k] == false ){
								false_pixel_coount++ ; 
							}
						}
						if( debug == 1 ){
							cout << "Line 1454 " << endl ;  
						}

						/*-----------------------------------------------------------------------------
						 *  
						  等於０　，　代表這可以當成　obstacle_seg , 因為中間都是true pixel 
						  ... , 偏偏又有錯了　，　如果上下的二個vertex 中　，　緊鄰一個obstacle , 
						  那這不能算是個　obstacle seg 丫　　
						  我可以用完整的obsacle資訊來處理這個問題　，
						  這個segment  在obstacle的肚子　，　兩x端的內部　　，　且　y 軸間有相交　
						 *-----------------------------------------------------------------------------*/
						if( debug == 1 ){
							cout << "Line 1466 " << endl ;  
						}
						bool close_case = false  ; 
						for(unsigned int k = 0 ; k < my_level_info.complete_obstacle_information.size() ; k++  ){
							//先判斷x , 看有沒有在肚子 , 再看 ， 這二個相鄰的點 是否一個在 肚上面方 一個在下方 
							// segment 的想法 ， 理應是 相鄰二個 上點的下segment 會是 下點的上segment 
							if( i < my_level_info.complete_obstacle_information[k].print_ru_corner_x() && 
							    i > my_level_info.complete_obstacle_information[k].print_ld_corner_x() &&  
							    x_position[i][j].print_y_position() > 
							    my_level_info.complete_obstacle_information[k].print_ld_corner_y() && 
							    x_position[i][j+1].print_y_position() <
							    my_level_info.complete_obstacle_information[k].print_ru_corner_y() ){
								close_case = true ; 	 
								break ; 
							}

						}

						// false_pixel_coount == 0 , 代表中間 都是為true 的pixel 
						if( false_pixel_coount == 0 && close_case == false ){
							seg_state = true ; 
						}

					}//  這個 if 只是為了抓一個 seg_state 要不要設成 true 而已  
					else if(j <= x_position[i].size() - 2 && seg_state == false && x_position[i][j].Is_pin_or_not() == true && 
						x_position[i][j+1].Is_pin_or_not() == true ){
						

						if( debug == 1 ){
							cout << "Line 1489 " << endl ;  
						}
						/*-----------------------------------------------------------------------------
						  等於０　，　代表這可以當成　obstacle_seg , 因為中間都是true pixel 
						  ... , 偏偏又有錯了　，　如果上下的二個vertex 中　，　緊鄰一個obstacle , 
						  那這不能算是個　obstacle seg 丫　　
						  我可以用完整的obsacle資訊來處理這個問題　，
						  這個segment  在obstacle的肚子　，　兩x端的內部　　，　且　y 軸間有相交　
						 *-----------------------------------------------------------------------------*/
						bool anothor_case = false ; 
						for(unsigned int k = 0 ; k < my_level_info.complete_obstacle_information.size() ; k++  ){
							//先判斷x , 看有沒有在肚子　	 
							if( i == my_level_info.complete_obstacle_information[k].print_ld_corner_x() &&  
							    x_position[i][j].print_y_position() <=  
							    my_level_info.complete_obstacle_information[k].print_ru_corner_y() && 
							    x_position[i][j+1].print_y_position() >= 
							    my_level_info.complete_obstacle_information[k].print_ld_corner_y() ){
								anothor_case = true ; 	 
								//有抓到就不用一直比下去了
								break ; 
							}
						}

						if(  anothor_case == true ){
							seg_state = true ; 
						}
					}// 用來 處理 normal 及 有一個 例外的case 

					
					/*-----------------------------------------------------------------------------
					 *  下面這塊 才是我要設定 segment 範圍以及 其segment 存與否的地方 ,  大多 我要設定的
					 *  是這個點的下個半segment  , 只有 head segment 是要 二邊都設的 
					 *-----------------------------------------------------------------------------*/

					if( debug == 1 ){
						cout << "Line 1515 " << endl ;  
					}
					//head segment
					if( j == 0 ){
						// 第一個seg 不一定是fasle   , 丫　，　如果　obstalce vertex 放在最上面　
						Segment[i].push_back( edge(  i ,my_level_info.plane_height - 1      , my_level_info.which_level , 
									     i ,x_position[i][j].print_y_position() , my_level_info.which_level , false )  );
						Segment[i].push_back( edge ( i ,x_position[i][j].print_y_position() , my_level_info.which_level ,
									     i ,x_position[i][j+ 1].print_y_position() , my_level_info.which_level , seg_state ));
					}
					//tail segment 
					else if( j == x_position[i].size() - 1 ){
						Segment[i].push_back( edge ( i , x_position[i][j].print_y_position() , my_level_info.which_level , 
									     i , 0 				     , my_level_info.which_level ,false ) );
					}
					// normal case 
					else{ 
						Segment[i].push_back( edge ( i ,x_position[i][j].print_y_position()    , my_level_info.which_level , 
									     i ,x_position[i][j+ 1].print_y_position() , my_level_info.which_level , seg_state));
					}
				}// size > 1 
			}// second loop for
		}// first loop for

		
		/*-----------------------------------------------------------------------------
		 *  喔 ， 所以 我上面 做的只是  ， 假設這個  x 位置有 二個點 ， 就會產生 三個seg
		 *  我要做的 只是抓出這些seg , 且判断是不是 obstacle_seg
		 *-----------------------------------------------------------------------------*/



		/*-----------------------------------------------------------------------------
		 * 				Check Code , May 9 
		for(int i = 0 ; i < my_level_info.plane_width ; i++){
			cout << "X position is " << i << endl ; 
			cout << "size in Segment " << i << "  = " << Segment[i].size() << endl ; 
			for(unsigned int j = 0  ; j < Segment[i].size() ; j++ ){
				// output and see 
				Segment[i][j].print_seg() ; 	
			}
		}
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  			CHECK FOR SEGMENT 
			 *               for(int i = 0 ; i < my_level_info.plane_width ; i++){
				cout << "X position is " << i << endl ; 
				cout << "size in Segment " << i << "  = " << Segment[i].size() << endl ; 
				for(unsigned int j = 0  ; j < Segment[i].size() ; j++ ){
					// output and see 
					Segment[i][j].print_seg() ; 	
				}
			}

			我有點想起來了 ，  因為 我是二個 x 位置上的東西 相互比對 ， 左邊的抓出 interval_line 
			右邊則是 segment_line , 主軸是 interval_line ,  主要是藉由 segment_line 來 減少
			interval_line 內的 alive_interval  數目  ， 只要  segment_line 內的全掃過後 ， 
			就往後 一行 再看 
		 *-----------------------------------------------------------------------------*/

		
		if( debug == 1 ){
			cout << "Line 1560 " << endl ;  
		}
		// 要做my_level_info.my_level_OASG的線了 , 先給定初值  
		unsigned int interval_line  = 0  , segment_line = 0  , alive_interval = 0 ; 
		// h ,k 分別指向 interval_line 及 segment_line 的內部 
		unsigned int h = 0  , k  = 0  ; 	
		bool how_to_move =  false ; 
		//要先抓出一個非零 的interval_line 免的下面  在抓interval的core_state會出錯
		//將 interval_line 停在 第一個 有點的 x 位置上 
		while( x_position[interval_line].size() == 0  ){
			interval_line++ ; 
		}
		//都忘了 segment_line 也要非零的line , 這裡也是一樣的意思 
		segment_line = interval_line + 1 ; 
		while( Segment[segment_line].size() == 0  ){
			segment_line++ ; 
		}

		// 我要怎麼抓出  最後二行有東西的line 當 interval_line 及 segment_line 的終點  ,  只是要抓它最遠跑到那裡而已 
		unsigned int deadline_of_interval_line = 0  , deadline_of_segment_line = 0  , count = 0 , location = my_level_info.plane_width - 1 ; 
		while( count < 2 ){
			if( x_position[location].size() > 0 && count == 0 ){
				deadline_of_segment_line = location ; 
				count++ ;
				
			}	
			else if(x_position[location].size() > 0 && count == 1 ){
				deadline_of_interval_line = location ; 
				count++ ;
			}
			location-- ;
		}	
		
		/*-----------------------------------------------------------------------------
		 * 				Check Code , May 9 
		cout << " deadline_of_interval_line  = " << deadline_of_interval_line << "     deadline_of_segment_line  " << deadline_of_segment_line << endl ; 
		 *-----------------------------------------------------------------------------*/
		if( debug == 1 ){
			cout << "Line 1598 " << endl ;  
		}

		do{

			
			/*-----------------------------------------------------------------------------
			 *  這塊呢 ， 我想是因為 同一個interval line 會跟 後面多個seg line 相比較 
			 *  而每次的比較 上面存活的interval 數目會不同 ， 所以  一開始這裡都要 重計一次 
			 *-----------------------------------------------------------------------------*/
			unsigned int x = 0 ; 
			alive_interval = 0 ; 
			while( x < x_position[interval_line].size()  ){
				if( x_position[interval_line][x].core_state() == true ){
					alive_interval++ ; 
				}
				x++ ;
			}
			
			
			/*-----------------------------------------------------------------------------
			 *  // 指向一個還活著的interval , 如果都是false 怎麼半
			while( alive_interval > 0 && h < x_position[interval_line].size() &&  x_position[interval_line][h].core_state() == false ){
				h++ ;
			}
			
			cout << " where is interval_line = " << interval_line << "   where is segment_line  = " << segment_line << endl ; 
			cout << " alive_interval  =  " << alive_interval << endl ; 	
			cout << " h = " << h << "  k  = " <<  k << endl ;   
			 *-----------------------------------------------------------------------------*/
		
			
					
			// 有個error的情況 , 那就是  ,h 跟 k 都不動  , 就會一直loop  
			// 這塊的核心 ， 主要是下面的四個 if block  , 要看一 下 分別是幹什麼的
			if( alive_interval > 0  &&  x_position[interval_line][h].core_state() == true ){
			//這裡要解決 , 傳入的seg , 含頭尾的情況 , 頭尾是有vertex 
			// modify_interval_and_add_edgs_to_OASG 這個func 是在 vertex 內的 
				     
			how_to_move = x_position[interval_line][h].modify_interval_and_add_edgs_to_OASG( Segment[segment_line][k].print_seg_state() , 
											   segment_line ,
											   Segment[segment_line][k].print_left_of_seg(), 
											   Segment[segment_line][k].print_right_of_seg(),
											   my_level_info.my_level_OASG   ,
											   my_level_info.plane_height - 1 , head_tail_state[segment_line][0], 
											   head_tail_state[segment_line][1]                                 ,
											   my_level_info.which_level 					); 	
				//下面這個是要印plane頭尾　，　是否有vertex 
				//cout << "x = " << segment_line << " , (" << head_tail_state[segment_line][0] << 
				//" , " << head_tail_state[segment_line][1] << " ) "
				//<< endl ;     
					 
				if( how_to_move == true ){
					h++ ;	   
					while( h < x_position[interval_line].size() && x_position[interval_line][h].core_state() == false ){
						h++  ; 
					}
				}
				else {
					k++ ; 
				}

				// 不能讓h , k 跑超過  它們應有的大小 , 分別指向 interval_line 及 segment_line 的內部 
				if( h >= x_position[interval_line].size() ){
					h = x_position[interval_line].size()  ; 
				}
				if( k >= Segment[segment_line].size() ){
					k = Segment[segment_line].size()  ; 
				}
			}

			
			/*-----------------------------------------------------------------------------
			 * 下面三塊的處理 層面好像跟 vertex 的屬性沒什麼關係 ， 所以  ， 我想應該不用動 
			 * 我先往建line 之間 比對建線的那裡來看看好了 
			 *  
			cout << " h = " << h << "  k  = " <<  k << endl  << endl ; 
			 向新的interval_line 做更改 , 那給好初值, 如果有 interval , 一直不被蓋住 , 那就要	
			 我segment_line 跟 interval_line的終止點 設錯了 , 因為 我只抓 有東西的那些line , 所以 不會跑到最後一行
			 *-----------------------------------------------------------------------------*/
			if( alive_interval == 0  || ( segment_line == deadline_of_segment_line  && h == x_position[interval_line].size()   ) ){
				
				// 下面二個 while 是要指向  , 元素大於0 的那個line 	
				interval_line ++ ; 
				while( x_position[interval_line].size() == 0 && interval_line <= deadline_of_interval_line  ){
					interval_line++ ; 
				}
	 
				segment_line = interval_line + 1 ;
				while( Segment[segment_line].size() == 0 && segment_line <=  deadline_of_segment_line  ){
					segment_line++ ; 
				}
				// 初值設定
				h = 0 , k = 0 ; 
				while( h < x_position[interval_line].size() && x_position[interval_line][h].core_state() == false ){
						h++  ; 
				}
			}
	  
			// h 跑到底 還要看 是否有interval 存活  有活著的interval_line  就不能往前推進
			// 好像不用看k 只要 h 有跑到底就可以了,   因為能把h 推到底 , 下面就沒有其它東西了
			//   所以不用再看k 
			if( alive_interval > 0 && (  h == x_position[interval_line].size() || k == Segment[segment_line].size()  )    ){
				h = 0 , k = 0 ;  
				segment_line++ ;
				while( Segment[segment_line].size() == 0 && segment_line <=  deadline_of_segment_line  ){
					segment_line++ ; 
				}
				while( h < x_position[interval_line].size() && x_position[interval_line][h].core_state() == false ){
						h++  ; 
				}
			}
			
			if( interval_line > deadline_of_interval_line ){
				break ; 
			}	
					
		
		}while( interval_line <=  deadline_of_segment_line ) ; 
		


		if( debug == 1 ){
			cout << "Line 1721 " << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 * 下面這塊 是要把多出來的edge 拿掉而已  跟本體的 my_level_info.my_level_OASG 核心沒有什麼關係 
		 * my_level_info.my_level_OASG 看上面那塊就好了 
		 *
		 因為  我check 時 發現， 我這會出現， 有自已 指向自已的 edge 發生 ， 
		 所以 我這裡 要多個 check 把這個error 拿掉 
		 *-----------------------------------------------------------------------------*/
		for( unsigned int i = 0 ; i < my_level_info.my_level_OASG.size() ; i++ ){
			if( my_level_info.my_level_OASG[i].print_node1_x() == my_level_info.my_level_OASG[i].print_node2_x() &&
			    my_level_info.my_level_OASG[i].print_node1_y() == my_level_info.my_level_OASG[i].print_node2_y() ){
				my_level_info.my_level_OASG.erase( my_level_info.my_level_OASG.begin() + i); 
				i--;
			}
		}// end of loop i 

		// 想不到  我的my_level_info.my_level_OASG 裡面 還有 edge 有相同的case 跑出來 
		for( unsigned int i = 0 ; i < my_level_info.my_level_OASG.size() ; i++ ){
			bool already_have = false ; 
			for( unsigned int j = i + 1 ; j < my_level_info.my_level_OASG.size() ; j++ ){
			 
				if( my_level_info.my_level_OASG[i].print_node1_x() == my_level_info.my_level_OASG[j].print_node1_x() &&
				    my_level_info.my_level_OASG[i].print_node1_y() == my_level_info.my_level_OASG[j].print_node1_y() && 
				    my_level_info.my_level_OASG[i].print_node1_z() == my_level_info.my_level_OASG[j].print_node1_z() && 
				    my_level_info.my_level_OASG[i].print_node2_x() == my_level_info.my_level_OASG[j].print_node2_x() && 
				    my_level_info.my_level_OASG[i].print_node2_z() == my_level_info.my_level_OASG[j].print_node2_z() && 
				    my_level_info.my_level_OASG[i].print_node2_y() == my_level_info.my_level_OASG[j].print_node2_y() ){
					already_have = true ;  
				}
				else if( my_level_info.my_level_OASG[i].print_node2_x() == my_level_info.my_level_OASG[j].print_node1_x() &&
					 my_level_info.my_level_OASG[i].print_node2_y() == my_level_info.my_level_OASG[j].print_node1_y() && 
					 my_level_info.my_level_OASG[i].print_node2_z() == my_level_info.my_level_OASG[j].print_node1_z() && 
					 my_level_info.my_level_OASG[i].print_node1_x() == my_level_info.my_level_OASG[j].print_node2_x() && 
					 my_level_info.my_level_OASG[i].print_node1_z() == my_level_info.my_level_OASG[j].print_node2_z() && 
					 my_level_info.my_level_OASG[i].print_node1_y() == my_level_info.my_level_OASG[j].print_node2_y() ){
						already_have = true ;  
				}
				if( already_have == true  ){
					// 把這個相同的給刪掉 ， 再給個false 往後看 
					my_level_info.my_level_OASG.erase( my_level_info.my_level_OASG.begin() + j ); 
					j-- ; 
					already_have = false ; 
				}
			}// end of loop j 

		}// end of loop i 
		// cout << "edges in OASG is " << my_level_info.my_level_OASG.size() << endl ; //　確實有抓edge 進來　，　但是　有錯誤

	}// end of build_OASG_can_handle_projection_vertex

/*-----------------------------------------------------------------------------
 *  
 告夭 ， 原來 我傳錯參數過來 ， 所以  我一開始是傳 vector<level_info> &my_level_info 所以
 裡面一直抓不到我要的東西
 *-----------------------------------------------------------------------------*/
void my_overlap_OASG::print_OASG_gnu( vector<edge> &OASG , level_info &my_level_info ){
	
	// 這裡先寫印出OASG 的腳本好了　
	ofstream oasgcout("OASG.gnu");
	oasgcout << " set title \"OASG\" " << endl ; 
	oasgcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	oasgcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	oasgcout << "set xlabel \"X-axis\"" << endl ; 
	oasgcout << "set ylabel \"Y-axis\"" << endl ; 
	// oasgcout << "set terminal postscript color" << endl ; 
	// oasgcout << "set output \"OASG.ps\"" << endl ; 
	oasgcout << "set output " << endl ; 
	oasgcout << "set terminal X11 " << endl ; 
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
	oasgcout << "plot \"Overlap_PlaneData.txt\" w lp pt 7 lc rgb \"black\" , \"Overlap_Vertex.txt\" w points  pt 7  lc rgb  \"green\" " << endl ; 
	oasgcout << "pause -1 " << endl ;
	oasgcout.close() ;
}// end of print_overlap_OASG_gnu


/*-----------------------------------------------------------------------------
 *   這裡要印出整個  3D 的 OASG 
 *-----------------------------------------------------------------------------*/
void my_overlap_OASG::print_3D_OASG_gnu( vector<level_info> &every_level_info , vector<edge> &vertical_edge ){

	ofstream SGcout("My_3D_OASG.gnu");
	SGcout << " set title \"My_3D_OASG\" " << endl ; 
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
			 
		//   用下面這個範本來改好了  
		//SGcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 
		// 這裡要讀obstacle 出來　　，　寫在PlaneData.gnu 裡面
		// 為什麼下面的 obstacle 第一層的數目  不太對呢   , 奇怪了  沒錯丫 
		// 不之道為什麼 又好了哎 
		for(unsigned int j = 0 ; j < every_level_info[i].complete_obstacle_information.size() ; j++ ){

			SGcout << "# This is obstacle number " << j << endl; 
			// 中間這塊 ， 要藉由各層的 vertex 判斷其 是否為 obstacle_corner 
			// 來畫出 obstacle  , 那是之前的方法 ， 我現在要用complete_obstacle_information  
			// 來畫     , func 內 會付責 arrow_count++ 的動作 
			every_level_info[i].complete_obstacle_information[j].print_3D_obstacle(SGcout ,  every_level_info[i].which_level , arrow_count ) ; 	

		}// end of print out all obstacle  variable j 

	}// end of get every_level_info to print out  variable i 


	//那這裡要印出　OASG 內的所有line  , 我還要多加上 vertical_edge 的line 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
	 
		for(unsigned int j  = 0 ; j < every_level_info[i].my_level_OASG.size() ; j++){
			// 我想這裡 也要改了  ， 下面 這個 func 原是用來處理 2D 的 
			// edge.h 已經改過了 ， 所以裡面是 3D 的 position 
			//OASG[i].print_3D_edge_information_for_gnu( SGcout , arrow_count + i + 1 );// i + 1 只是obstacle 的編號而己 
			arrow_count++;
			every_level_info[i].my_level_OASG[j].print_3D_edge_information_for_gnu( SGcout , arrow_count ) ; 
		}

	}

	// 下面這個是 vertical_edge 的 
	for( unsigned  int i = 0  ; i < vertical_edge.size() ; i++ ){
		arrow_count++;	
		vertical_edge[i].print_3D_edge_information_for_gnu( SGcout , arrow_count ) ;  
	}


	/*-----------------------------------------------------------------------------
	 *  
	PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　

	 下面這裡的話 ，  我能不能用二個放點的 txt 檔 ，應該可以的樣子
	 下面就 再多放一個 splot 不就好了麻 , 我來試看看先 
	 水拉  測試成功 我接下來要做的就是  把 projection_vertex.txt 的檔給弄出來
	 有點忘了  我是在那裡建 PlaneData_3D.txt  這個檔的了 

	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" " << endl ; 
	還沒寫出來前 ， 先不要改 code 
	SGcout << "splot \"Overlap_PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	 *-----------------------------------------------------------------------------*/

	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"Overlap_Vertex_3D.txt\" w points  pt 7  lc rgb  \"green\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;

}

#endif
