/*
 * =====================================================================================
 *
 *       Filename:  Projection_handler.h
 *
 *    Description:  as above 
 *
 *        Version:  1.0
 *        Created:  2011/03/29 12時16分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef         PROJECTION_HANDLER_H	
	#define 	PROJECTION_HANDLER_H	
#include	<cstdio>
#include	<iostream>
#include 	"vertex.h"	
#include 	"edge.h"	
#include 	"obstacle.h"	
#include 	"level_info.h"	
#include	<fstream>
#include	<vector>
#include	<cstdlib>
#include	<map>

using namespace std ; 
class Projection_handler{

	private:
	public:
	Projection_handler(){
			
	}

	void generate_project_vertex( vector<level_info> &every_level_info , vector<edge> &vertical_edge ) ; 
	void generate_project_vertex_for_overlap( vector<level_info> &every_level_info , vector<edge> &vertical_edge ) ; 


	/*-----------------------------------------------------------------------------
	 *   這裡 要再多寫個 出 gnu 的檔  , 我要畫出  ， 所有的 vertex  obstacle_corner  , 及 projection_vertex
	 *   跟 vertical_edge 就可以了  , 我記得 我有寫過 ， 但我忘記放那裡了  ,  下面是要做出 gnu 的func 
	 *-----------------------------------------------------------------------------*/
	void generate_all_vertex_and_vertical_edge_gnu( vector<level_info> &every_level_info , vector<edge> &vertical_edge ) ; 
	void generate_all_vertex_and_vertical_edge_for_overlap_gnu( vector<level_info> &every_level_info , vector<edge> &vertical_edge ) ; 

};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  projection_detail
 *  Description:   這要用來處理 projection  , 所產生的點  ， 因為projection 有分 source 跟
 *  		   target  , 有了這個 func 我只要把 頭尾掉換一下 就好了  ， 不用說什麼
 *  		   寫一個由下到上 ， 由上到下的東西 
 *  		   下面的第二個引數 ， array_vertex_of_every_level[i][j] , i 代表是那一層 
 * =====================================================================================
 */
	void
projection_detail ( level_info &source_level_info , level_info &target_level_info  , vector<edge> &vertical_edge )
{

	bool debug = false ; 
	// 如果 total 的數目是9 那代表index是從 0 到 8 
	// 所以我要放東西進去 是要先放再++ , 左邊這個東西 ， 好像只能放一個 , 下面那個平行化加下去
	// 好像有快一點    ,  有時會有個warning   是講  下面平行化的那行 ， 我想是 
	// makefile內  ， 沒有加入那個  -fopenmp  這個選項  的關係 
	int size = source_level_info.array_vertex_vector.size() ; 
	#pragma omp parallel for    
	for(int j = 0 ; j < size ; j++ ){

		// 先把本層要看的點 給抓出來 
		int x = source_level_info.array_vertex_vector[j].print_x_position() ; 
		int y = source_level_info.array_vertex_vector[j].print_y_position() ; 
		// 進去下面二個主要的 body 之前 ， 我先 判斷 它打到 target , 
		// 是打中 obstacle body or vertex ( pin or obstacle_corner ) 
		// 這裡的打中 ， 是要打中  obstacle body 
		vector<vertex> four_seperate_vertex ; 
		bool hit_obstacle_body = false ; 

		if( debug == true  ){
			 
			if( target_level_info.pin_plane[x][y] == true   ){
				cout << target_level_info.obstacle_id[x][y] ; 	
			}
		}

		// 好像  不用這麼麻煩 ， 只要點 是落在  pin_plane 內 為true  好像就可以了  
		// 所以  就拿他的 obstacle_id  , 為true   是有打中   ， false 就沒有 
		// 有打中再改就好了 ， 沒打中就不用   , 看來是當在下面這一塊  , 丫  ， 我忘了  
		// pin_plane 有個問題   ， 就是 不能run 厚度為1的obstacle 
		// 那換個方法 ， 只要有打到  ， obstacle_plane  , 就拿出那個obstacle_id  來比對看看 就
		// 可以了 
		if( target_level_info.obstacle_plane[x][y] == true   ){

			int which_obstacle = target_level_info.obstacle_id[x][y] ; 
			if( x >  target_level_info.complete_obstacle_information[which_obstacle].print_ld_corner_x() &&
			    x <  target_level_info.complete_obstacle_information[which_obstacle].print_ru_corner_x() &&	
			    y >  target_level_info.complete_obstacle_information[which_obstacle].print_ld_corner_y() &&	
			    y <  target_level_info.complete_obstacle_information[which_obstacle].print_ru_corner_y() ){
			 
				// 打中後， 應該是先在 source_level_info 那裡 ， 先展開出四個點才對
				four_seperate_vertex.push_back( vertex( x ,
									target_level_info.complete_obstacle_information[which_obstacle].print_ru_corner_y() ,
									source_level_info.which_level ) ) ; 		

				four_seperate_vertex.push_back( vertex( x ,
									target_level_info.complete_obstacle_information[which_obstacle].print_ld_corner_y(),
									source_level_info.which_level ) ) ; 		

				four_seperate_vertex.push_back( vertex( target_level_info.complete_obstacle_information[which_obstacle].print_ru_corner_x(),
									y ,
									source_level_info.which_level ) ) ; 		

				four_seperate_vertex.push_back( vertex( target_level_info.complete_obstacle_information[which_obstacle].print_ld_corner_x(),
									y ,
									source_level_info.which_level ) ) ; 		

				hit_obstacle_body = true ; 
			}// 判斷有沒有打到 

		}// 有打中 obstacle 
		
		/*-----------------------------------------------------------------------------
		// 下面這塊可以改  ，  不想要跟 那麼多的obstacle 做比對 ， 
		// 這個non_overlap的  ， 似乎可以這麼做  ， 但  overlap 好像不行  
		map<int,obstacle>::iterator it ;  
		int key = 0 ; 
		for(unsigned int k = 0 ; k < target_level_info.obstacle_cross[x].size() ; k++ ){

			 key = target_level_info.obstacle_cross[x][k] ;
			 it =  target_level_info.obstacle_map.find( key ) ; 
			// 這裡 代表落在裡面 
			if( x > (*it).second.print_ld_corner_x() && 
			    x < (*it).second.print_ru_corner_x() &&
			    y > (*it).second.print_ld_corner_y() && 
			    y < (*it).second.print_ru_corner_y() ){
                                 
				// 打中後， 應該是先在 source_level_info 那裡 ， 先展開出四個點才對
				four_seperate_vertex.push_back( vertex( x ,
									(*it).second.print_ru_corner_y(),
									source_level_info.which_level ) ) ; 		

				four_seperate_vertex.push_back( vertex( x ,
									(*it).second.print_ld_corner_y(),
									source_level_info.which_level ) ) ; 		

				four_seperate_vertex.push_back( vertex( (*it).second.print_ru_corner_x(),
									y ,
									source_level_info.which_level ) ) ; 		

				four_seperate_vertex.push_back( vertex( (*it).second.print_ld_corner_x(),
									y ,
									source_level_info.which_level ) ) ; 		

				hit_obstacle_body = true ; 
				break ; 	
			}// inside obstacle body or not   

		}// scan all obstacle   , end of loop k 
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  這裡呢， 展開的四個點的位置 確實是都之道了 ， 我要再由 source_level_info 
		 *  去過濾 ， 過完後 ， 留下的點 ， 我要再藉由 ， 各層的 array_vertex 來比看看
		 *  要不要建新的點出來  , 這二個位置都不用建新點的話 ， 就直接連了  
		 *-----------------------------------------------------------------------------*/
		vector<vertex> four_seperate_vertex_filter_by_source_level ; 
		for(unsigned int l = 0 ; l < four_seperate_vertex.size() ; l++ ){

			int tmp_x = four_seperate_vertex[l].print_x_position() ; 
			int tmp_y = four_seperate_vertex[l].print_y_position() ; 
			// 看有沒有打中 source_level 的obstacle body 
			bool hit_or_not = false ; 

			if( source_level_info.obstacle_plane[tmp_x][tmp_y] == true ){

				int which_obstacle = source_level_info.obstacle_id[tmp_x][tmp_y] ; 

				if( tmp_x >  source_level_info.complete_obstacle_information[which_obstacle].print_ld_corner_x() &&
				    tmp_x <  source_level_info.complete_obstacle_information[which_obstacle].print_ru_corner_x() &&	
				    tmp_y >  source_level_info.complete_obstacle_information[which_obstacle].print_ld_corner_y() &&	
				    tmp_y <  source_level_info.complete_obstacle_information[which_obstacle].print_ru_corner_y() ){
					hit_or_not = true ;  
				}
			}// 判斷有沒有打到裡面  

			/*-----------------------------------------------------------------------------
			map<int,obstacle>::iterator it ;  
			int key = 0 ; 
			for(unsigned int k = 0 ; k < source_level_info.obstacle_cross[tmp_x].size() ; k++ ){

				key = source_level_info.obstacle_cross[tmp_x][k] ;
				it =  source_level_info.obstacle_map.find( key ) ; 
				// 這裡 代表落在裡面 
				if( tmp_x > (*it).second.print_ld_corner_x() && 
				    tmp_x < (*it).second.print_ru_corner_x() &&
				    tmp_y > (*it).second.print_ld_corner_y() && 
				    tmp_y < (*it).second.print_ru_corner_y() ){
					hit_or_not = true ;  
					break ; 	
				}// inside obstacle body or not   
			}// scan all obstacle   , end of loop k 
			 *-----------------------------------------------------------------------------*/

			// filter 內的東西 ， 我只要其 x , y  
			if( hit_or_not == false  ){
				four_seperate_vertex_filter_by_source_level.push_back( four_seperate_vertex[l] ) ;  
			}// 如果 沒打到  就加入到 four_seperate_vertex_filter_by_source_level 中 
		 
		}// 那四個展開點 


		/*-----------------------------------------------------------------------------
		 *  
		 bool hit_vertex_or_not = false ;  , 這行應該不用
		 只要沒有打中body , target_level  的那個位置 又是 true pixel 
		 代表不是打中 pin 就是 corner , 等一下 這好像不行 ， 因為 obstacle_corner 
		 在pin 中 好像沒被 設定成 true pixel 
		 *-----------------------------------------------------------------------------*/
		bool hit_vertex_or_not_on_target_level = false ;
		if( hit_obstacle_body == false  ){
		 
		 	if( target_level_info.only_vertex[x][y] == true  ){
					hit_vertex_or_not_on_target_level = true ;  
			}
			/*-----------------------------------------------------------------------------
		 	// 下面這個 loop 比對的 東西 太多了  , 看來要再最上面做個分類  
			for(unsigned int k = 0 ; k <  target_level_info.array_vertex_on_position[x].size() ; k++ ){
				if(     y == target_level_info.array_vertex_on_position[x][k].print_y_position() ){
					hit_vertex_or_not_on_target_level = true ;  
					break ; 
				}
			}// loop k , check hit pin or obstacle_corner
			 *-----------------------------------------------------------------------------*/

		}// 沒有打到  obstacle body 才有必要來看是否打到 vertex  


		/*-----------------------------------------------------------------------------
		 *  前面都是前置處理
		 *
		 打中 obstacle body or obstacle side 
		 打中 body 要展開是很正常拉 ， side的話麻 ， 
		 我先不做 ， 打中邊界的處理 ， 只先處理打中
		 body inside 
		 *-----------------------------------------------------------------------------*/ 
		 if( hit_obstacle_body == true && four_seperate_vertex_filter_by_source_level.size() > 0 ){


			/*-----------------------------------------------------------------------------
			 * 這裡要做的事 ， 是看 filter 內的點 ， 在source 跟 target 二層內 ， 有沒有要
			 * 加入 projection_vertex , 如果沒有 就直接建立直線
			 *-----------------------------------------------------------------------------*/
			bool filter_vertex_hit_on_source_level = false ; 
			bool filter_vertex_hit_on_target_level = false ; 

			for( unsigned int k = 0 ; k < four_seperate_vertex_filter_by_source_level.size() ; k++ ){

				int tmp_x = four_seperate_vertex_filter_by_source_level[k].print_x_position() ; 
				int tmp_y = four_seperate_vertex_filter_by_source_level[k].print_y_position() ; 


				if( source_level_info.only_vertex[tmp_x][tmp_y] == true ){
						filter_vertex_hit_on_source_level = true ; 
				}

				if( target_level_info.only_vertex[tmp_x][tmp_y] == true ){
						filter_vertex_hit_on_target_level = true ; 
				}
				/*-----------------------------------------------------------------------------
				// 先跟 source_level 比  , 因為下面這個 array_vertex_vector , 中 我有放 projection_vertex 
				// 所以 ， 不用額外再設一個 用來觀察 projection_vertex 的了 
				for( unsigned int l = 0 ; l < source_level_info.array_vertex_on_position[tmp_x].size() ; l++ ){
				 
					if(     tmp_y == source_level_info.array_vertex_on_position[tmp_x][l].print_y_position() ){
						filter_vertex_hit_on_source_level = true ; 
						break ; 
					}

				}// scan all vertex in source_level 

				// 先跟 target_level 比 
				for( unsigned int l = 0 ; l < target_level_info.array_vertex_on_position[tmp_x].size() ; l++ ){
				 
					if(     tmp_y == target_level_info.array_vertex_on_position[tmp_x][l].print_y_position() ){
						filter_vertex_hit_on_target_level = true ; 
						break ; 
					}

				}// scan all vertex in target_level 
				 *-----------------------------------------------------------------------------*/

				// 看source 層 
				if( filter_vertex_hit_on_source_level == true && filter_vertex_hit_on_target_level == true  ){
					// 加線 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				}
				// 下面的三種cases , 也都有線要加 ， 差別只是在 上下層有沒有要增加的投影點 
				else if( filter_vertex_hit_on_source_level == false && filter_vertex_hit_on_target_level == true ){

					// 要多存個 array_vertex_on_position  , 因為上面都是用這個 結構去比東西的 
					source_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					// source_level_info.array_vertex_on_position[tmp_x].push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )); 
					source_level_info.only_vertex[tmp_x][tmp_y] = true ; 

					source_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				 
				}
				else if( filter_vertex_hit_on_source_level == true && filter_vertex_hit_on_target_level == false ){

					target_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					// target_level_info.array_vertex_on_position[tmp_x].push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )); 
					target_level_info.only_vertex[tmp_x][tmp_y] = true ; 

					target_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				 
				}
				else if( filter_vertex_hit_on_source_level == false && filter_vertex_hit_on_target_level == false ){

					// source_level_info.array_vertex_on_position[tmp_x].push_back( 
					// 							vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					source_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					source_level_info.only_vertex[tmp_x][tmp_y]  = true ; 

					target_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					// target_level_info.array_vertex_on_position[tmp_x].push_back( 
					// 							vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					target_level_info.only_vertex[tmp_x][tmp_y] = true ; 

					source_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					target_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				 
				}


			}// scan all vertex in filter 
		 
		}// 最塊是這麻煩的了 
		else if( hit_obstacle_body == false && hit_vertex_or_not_on_target_level == true ){

			vertical_edge.push_back( edge( x , y , source_level_info.which_level , 
						       x , y , target_level_info.which_level )  ) ; 
			
			/*-----------------------------------------------------------------------------
			 *  這是  沒有打中 obstacle body 但有打中 vertex  那就直接連線就好了  , 這裡打中的
			 *  vertex  我有把 projection vertex 算進去 
			 *  
			if( hit_corner_or_not == true ){
				every_level_info[i + 1].pin_plane[x][y] = true ; 
			}//  是打到 obstacle_corner  , 所以不用建 projection_vertex , 但可以建直線
			else{
				// 看來我把投影點 弄到別層 很簡單 ， 因為我點的預設就是 corner_id = 0 , pin = false  	
				every_level_info[i + 1].projection_vertex.push_back( vertex( x , y , every_level_info[i + 1].which_level  ) ) ; 	
				every_level_info[i + 1].pin_plane[x][y] = true ; 
			}
			 *-----------------------------------------------------------------------------*/

		}// 打中 pin or obstacle , 這二點在2D routing 上 本來就有了 ， 所以只要建垂直線就好了 
		else if( hit_obstacle_body == false && hit_vertex_or_not_on_target_level == false ){
		 
			target_level_info.array_vertex_vector.push_back( vertex( x , y , target_level_info.which_level )  ) ; 
			// target_level_info.array_vertex_on_position[x].push_back( vertex( x , y , target_level_info.which_level )  ) ; 
			target_level_info.only_vertex[x][y] = true ; 

			target_level_info.projection_vertex.push_back( vertex( x , y , target_level_info.which_level )  ) ; 
			vertical_edge.push_back( edge( x , y , source_level_info.which_level , 
						       x , y , target_level_info.which_level )  ) ; 

		}// 這塊的話  ， 要建點也要建線 

	}// end of loop j 

}		/* -----  end of function projection_detail  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  projection_detail_fox_overlap
 *  Description:   這要用來處理 projection  , 所產生的點  ， 因為projection 有分 source 跟
 *  		   target  , 有了這個 func 我只要把 頭尾掉換一下 就好了  ， 不用說什麼
 *  		   寫一個由下到上 ， 由上到下的東西 
 *  		   下面的第二個引數 ， array_vertex_of_every_level[i][j] , i 代表是那一層 
 * =====================================================================================
 */
	void
projection_detail_for_overlap ( level_info &source_level_info , level_info &target_level_info  , vector<edge> &vertical_edge )
{


	int debug = 0 ; 
	if( debug == 1 ){
		cout << " Get in detail " << endl ;  
	}
	/*-----------------------------------------------------------------------------
	 如果 total 的數目是9 那代表index是從 0 到 8 
	 所以我要放東西進去 是要先放再++ , 這樣 ， 我每層都有 pin , obstacle_corners , overlap_vertex 
	 原來我分成了  ， 打過去 是打到obstacle body or 打到點 
	 我的主軸就是下面這個j loop 丫
	 我想要用新方法去想  ， 
	 不能用 pin_plane , 因為打 obstacle 也分不出來丫 
	int size = source_level_info.array_vertex_vector.size() ; 
	#pragma omp parallel for ordered 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int j = 0 ; j < source_level_info.array_vertex_vector.size(); j++ ){

		if( debug == 1 ){
			cout << " Get in big loop  " << j << endl ;  
		}
		// 先把本層要看的點 給抓出來 
		int x = source_level_info.array_vertex_vector[j].print_x_position() ; 
		int y = source_level_info.array_vertex_vector[j].print_y_position() ; 


		// 進去下面二個主要的 body 之前 ， 我先 判斷 它打到 target , 
		// 是打中 obstacle body or vertex ( pin or obstacle_corner ) 
		// obstacle 再怎麼重壘 ， 十字展開 所抓的點 ， 也同樣是四個 
		int min_x = -1 , max_x = -1 , min_y = -1 , max_y = -1 ; 
		vector<vertex> four_seperate_vertex ; 
		bool hit_obstacle_body = false ; 

		map<int,obstacle>::iterator body_it ;  
		int body_key = 0 ; 
		if( debug == 1 ){
			cout << " Get in line 323  " << endl ;  
		}
		// 我預設是 false 沒有打到  ， 下面是要抓 有打到的
		// 只有跑到323 沒有 跑到下面的 391 , 想不到這麼前面就有錯了 
		if( target_level_info.obstacle_body_cross[x].size() > 0 ){

			
			/*-----------------------------------------------------------------------------
			 *  第一塊 ， 把  有相交的obstacle key都先抓出來放到key裡面去 
			 *-----------------------------------------------------------------------------*/
			if( debug == 1 ){
				cout << " Get in line 334  " << endl ;  
				// cout << "target_level_info.obstacle_body_cross[x].size() = " << target_level_info.obstacle_body_cross[x].size() << endl;  
			}
			vector<int> hit_body_obstacle ; 	
			for(unsigned int k = 0 ; k < target_level_info.obstacle_body_cross[x].size() ; k++ ){

				body_key =  target_level_info.obstacle_body_cross[x][k] ; 
				body_it = target_level_info.obstacle_map.find( body_key ) ; 
				
				// 這裡 代表落在裡面 
				if( y >= (*body_it).second.print_ld_corner_y() && 
				    y <= (*body_it).second.print_ru_corner_y() ){
					hit_body_obstacle.push_back( body_key ) ; 	
				}// inside obstacle body or not   

			}// scan all obstacle   , end of loop k 

			/*-----------------------------------------------------------------------------
			 *  第二塊 ， scan all 相交的obstacle ， 抓出最大小的x,y 
			 *-----------------------------------------------------------------------------*/
			if( debug == 1 ){
				cout << " Get in line 354  " << endl ;  
			}

			if( hit_body_obstacle.size() > 0 ){

				hit_obstacle_body = true ;  
				body_key =  target_level_info.obstacle_body_cross[x][0] ; 
				body_it = target_level_info.obstacle_map.find( body_key ) ; 
				// 那下面這個loop 我要用 ， 來抓  所有相交的obstacle , 抓出 最大小的x,y位置
				min_x = (*body_it).second.print_ld_corner_x() ; 
				min_y = (*body_it).second.print_ld_corner_y() ; 

				max_x = (*body_it).second.print_ru_corner_x() ; 
				max_y = (*body_it).second.print_ru_corner_y() ; 

				for(unsigned int k = 0 ; k < hit_body_obstacle.size() ; k++ ){
					body_key =  target_level_info.obstacle_body_cross[x][k] ; 
					body_it = target_level_info.obstacle_map.find( body_key ) ; 
					// 一旦有相交 ， 我要比對那四個變數 看有沒有要更新的 , 先比好min 
					if( (*body_it).second.print_ld_corner_x() < min_x ){
						min_x = (*body_it).second.print_ld_corner_x() ;  
					}
					if( (*body_it).second.print_ld_corner_y() < min_y ){
						min_y = (*body_it).second.print_ld_corner_y() ;  
					}
					// 比  max 
					if( (*body_it).second.print_ru_corner_x() > max_x ){
						max_x = (*body_it).second.print_ru_corner_y() ;  
					}
					if( (*body_it).second.print_ru_corner_y() > max_y ){
						max_y = (*body_it).second.print_ru_corner_y() ;  
					}
				 
				}// for loop k 

				// 把點先加入 ， 再 看能不能用下面的方法去 過慮
				if( target_level_info.pin_plane[x][min_y] == false ){
					four_seperate_vertex.push_back( vertex( x , min_y , target_level_info.which_level ) ) ; 
				}

				if( target_level_info.pin_plane[x][max_y] == false ){
					four_seperate_vertex.push_back( vertex( x , max_y , target_level_info.which_level ) ) ; 
				}

				if( target_level_info.pin_plane[min_x][y] == false ){
					four_seperate_vertex.push_back( vertex( min_x , y , target_level_info.which_level ) ) ; 
				}

				if( target_level_info.pin_plane[max_x][y] == false ){
					four_seperate_vertex.push_back( vertex( max_x , y , target_level_info.which_level ) ) ; 
				}

			}// 要大於0 才進來 

			
		}// target_level_info.obstacle_body_cross[x][0] > 0


		if( debug == 1 ){
			cout << " Get in line 391  " << endl ;  
		}
		/*-----------------------------------------------------------------------------
		 *  這裡呢， 展開的四個點的位置 確實是都之道了 ， 我要再由 source_level_info 
		 *  去過濾 ， 過完後 ， 留下的點 ， 我要再藉由 ， 各層的 array_vertex 來比看看
		 *  要不要建新的點出來  , 這二個位置都不用建新點的話 ， 就直接連了  
		 *  我只focus 
		 *-----------------------------------------------------------------------------*/
		vector<vertex> four_seperate_vertex_filter_by_source_level ; 
		for(unsigned int l = 0 ; l < four_seperate_vertex.size() ; l++ ){

			int tmp_x = four_seperate_vertex[l].print_x_position() ; 
			int tmp_y = four_seperate_vertex[l].print_y_position() ; 
			// 看有沒有打中 source_level 的obstacle body 
			bool hit_or_not = false ; 
			for(unsigned int k = 0 ; k < source_level_info.obstacle_body_cross[tmp_x].size() ; k++ ){
				body_key =  source_level_info.obstacle_body_cross[tmp_x][k] ; 
				body_it  = source_level_info.obstacle_map.find( body_key ) ; 
				// 這裡 代表落在裡面 
				if( tmp_y > (*body_it).second.print_ld_corner_y() && 
				    tmp_y < (*body_it).second.print_ru_corner_y() ){
					hit_or_not = true ;  
					break ; 	
				}// inside obstacle body or not   
			}// scan all obstacle   , end of loop k 

			// filter 內的東西 ， 我只要其 x , y  
			if( hit_or_not == false   ){
				four_seperate_vertex_filter_by_source_level.push_back( four_seperate_vertex[l] ) ;  
			}// 如果 沒打到  就加入到 four_seperate_vertex_filter_by_source_level 中 
		 
		}// 那四個展開點 


		/*-----------------------------------------------------------------------------
		 *  
		 bool hit_vertex_or_not = false ;  , 這行應該不用
		 只要沒有打中body , target_level  的那個位置 又是 true pixel 
		 代表不是打中 pin 就是 corner , 等一下 這好像不行 ， 因為 obstacle_corner 
		 在pin 中 好像沒被 設定成 true pixel 
		 *-----------------------------------------------------------------------------*/
		if( debug == 1 ){
			cout << " Get in line 433  " << endl ;  
		}
		bool hit_vertex_or_not_on_target_level = false ;
		if( hit_obstacle_body == false  ){
		 
		 	if( target_level_info.only_vertex[x][y]  == true){
					hit_vertex_or_not_on_target_level = true ;  
			}

			/*-----------------------------------------------------------------------------
			for(unsigned int k = 0 ; k <  target_level_info.array_vertex_on_position[x].size() ; k++ ){
				if(     y == target_level_info.array_vertex_on_position[x][k].print_y_position() ){
					hit_vertex_or_not_on_target_level = true ;  
					break ; 
				}
			}// loop k , check hit pin or obstacle_corner
			 *-----------------------------------------------------------------------------*/


		}// 沒有打到  obstacle body 才有必要來看是否打到 vertex  


		/*-----------------------------------------------------------------------------
		 *  前面都是前置處理
		 *
		 打中 obstacle body or obstacle side 
		 打中 body 要展開是很正常拉 ， side的話麻 ， 
		 我先不做 ， 打中邊界的處理 ， 只先處理打中
		 body inside 
		 *-----------------------------------------------------------------------------*/ 
		if( debug == 1 ){
			cout << " Get in line 461  " << endl ;  
		}

		 if( hit_obstacle_body == true && four_seperate_vertex_filter_by_source_level.size() > 0 ){


			/*-----------------------------------------------------------------------------
			 * 這裡要做的事 ， 是看 filter 內的點 ， 在source 跟 target 二層內 ， 有沒有要
			 * 加入 projection_vertex , 如果沒有 就直接建立直線
			 *-----------------------------------------------------------------------------*/
			bool filter_vertex_hit_on_source_level = false ; 
			bool filter_vertex_hit_on_target_level = false ; 

			for( unsigned int k = 0 ; k < four_seperate_vertex_filter_by_source_level.size() ; k++ ){

				int tmp_x = four_seperate_vertex_filter_by_source_level[k].print_x_position() ; 
				int tmp_y = four_seperate_vertex_filter_by_source_level[k].print_y_position() ; 

				if( source_level_info.only_vertex[tmp_x][tmp_y]  == true ){
						filter_vertex_hit_on_source_level = true ; 
				}
				
				if( target_level_info.only_vertex[tmp_x][tmp_y]  == true ){
						filter_vertex_hit_on_target_level = true ; 
				}

				/*-----------------------------------------------------------------------------
				// 先跟 source_level 比  , 因為下面這個 array_vertex_vector , 中 我有放 projection_vertex 
				// 所以 ， 不用額外再設一個 用來觀察 projection_vertex 的了 
				for( unsigned int l = 0 ; l < source_level_info.array_vertex_on_position[tmp_x].size() ; l++ ){
				 
					if(     tmp_y == source_level_info.array_vertex_on_position[tmp_x][l].print_y_position() ){
						filter_vertex_hit_on_source_level = true ; 
						break ; 
					}

				}// scan all vertex in source_level 

				// 先跟 target_level 比 
				for( unsigned int l = 0 ; l < target_level_info.array_vertex_on_position[tmp_x].size() ; l++ ){
				 
					if(     tmp_y == target_level_info.array_vertex_on_position[tmp_x][l].print_y_position() ){
						filter_vertex_hit_on_target_level = true ; 
						break ; 
					}

				}// scan all vertex in target_level 
				 *-----------------------------------------------------------------------------*/

				// 看source 層 
				if( filter_vertex_hit_on_source_level == true && filter_vertex_hit_on_target_level == true  ){
					// 加線 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				}
				// 下面的三種cases , 也都有線要加 ， 差別只是在 上下層有沒有要增加的投影點 
				else if( filter_vertex_hit_on_source_level == false && filter_vertex_hit_on_target_level == true ){

					source_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					// source_level_info.array_vertex_on_position[tmp_x].push_back( 
					// 						vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					source_level_info.only_vertex[tmp_x][tmp_y] = true ; 

					source_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				 
				}
				else if( filter_vertex_hit_on_source_level == true && filter_vertex_hit_on_target_level == false ){

					target_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					// target_level_info.array_vertex_on_position[tmp_x].push_back( 
					// 							vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					target_level_info.only_vertex[tmp_x][tmp_y] = true ; 

					target_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				 
				}
				else if( filter_vertex_hit_on_source_level == false && filter_vertex_hit_on_target_level == false ){

					source_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					// source_level_info.array_vertex_on_position[tmp_x].push_back( 
					// 							vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					source_level_info.only_vertex[tmp_x][tmp_y] = true ; 
					
					target_level_info.array_vertex_vector.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					// target_level_info.array_vertex_on_position[tmp_x].push_back( 
					// 							vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					target_level_info.only_vertex[tmp_x][tmp_y] = true ; 

					source_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , source_level_info.which_level )  ) ; 
					target_level_info.projection_vertex.push_back( vertex( tmp_x , tmp_y , target_level_info.which_level )  ) ; 
					vertical_edge.push_back( edge( tmp_x , tmp_y , source_level_info.which_level , 
								       tmp_x , tmp_y , target_level_info.which_level )  ) ; 
				 
				}


			}// scan all vertex in filter 
		 
		}// 最塊是這麻煩的了 
		else if( hit_obstacle_body == false && hit_vertex_or_not_on_target_level == true ){

			vertical_edge.push_back( edge( x , y , source_level_info.which_level , 
						       x , y , target_level_info.which_level )  ) ; 
			
			/*-----------------------------------------------------------------------------
			 *  這是  沒有打中 obstacle body 但有打中 vertex  那就直接連線就好了  , 這裡打中的
			 *  vertex  我有把 projection vertex 算進去 
			 *  
			if( hit_corner_or_not == true ){
				every_level_info[i + 1].pin_plane[x][y] = true ; 
			}//  是打到 obstacle_corner  , 所以不用建 projection_vertex , 但可以建直線
			else{
				// 看來我把投影點 弄到別層 很簡單 ， 因為我點的預設就是 corner_id = 0 , pin = false  	
				every_level_info[i + 1].projection_vertex.push_back( vertex( x , y , every_level_info[i + 1].which_level  ) ) ; 	
				every_level_info[i + 1].pin_plane[x][y] = true ; 
			}
			 *-----------------------------------------------------------------------------*/

		}// 打中 pin or obstacle , 這二點在2D routing 上 本來就有了 ， 所以只要建垂直線就好了 
		else if( hit_obstacle_body == false && hit_vertex_or_not_on_target_level == false ){
		 
			target_level_info.array_vertex_vector.push_back( vertex( x , y , target_level_info.which_level )  ) ; 
			// target_level_info.array_vertex_on_position[x].push_back( vertex( x , y , target_level_info.which_level )  ) ; 
			target_level_info.only_vertex[x][y] = true ; 

			target_level_info.projection_vertex.push_back( vertex( x , y , target_level_info.which_level )  ) ; 
			vertical_edge.push_back( edge( x , y , source_level_info.which_level , 
						       x , y , target_level_info.which_level )  ) ; 

		}// 這塊的話  ， 要建點也要建線 

	}// end of loop j 

}		/* -----  end of function projection_detail  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_project_vertex
 *  Description:   這樣一來  ， 這個只要寫個大概的樣子出來 就可以了 主軸 就交給 projection_detail
 * =====================================================================================
 */
	void
Projection_handler::generate_project_vertex ( vector<level_info> &every_level_info , vector<edge> &vertical_edge )
{
	bool debug  = false ; 
	//  我想先用一個vector 把各層中 ， array_vertex 內的東西 都塞入 這個新的vector 內
	// vector<vertex> mirror_of_array_vertex[ every_level_info.size() ] ; 
	for(unsigned  int i = 0 ; i < every_level_info.size()  ; i++ ){

		for(int j = 0 ; j < every_level_info[i].total ; j++ ){

			int x = every_level_info[i].array_vertex[j].print_x_position() ;	
			int y = every_level_info[i].array_vertex[j].print_y_position() ;	
			// mirror_of_array_vertex[i].push_back( every_level_info[i].array_vertex[j] ); 
			every_level_info[i].array_vertex_vector.push_back( every_level_info[i].array_vertex[j] ) ; 
			every_level_info[i].array_vertex_on_position[ every_level_info[i].array_vertex[j].print_x_position() ].push_back(
									every_level_info[i].array_vertex[j] )  ; 
			every_level_info[i].only_vertex[x][y] = true ; 
		}// copy 各個vertex 

		for( unsigned int j = 0 ; j < every_level_info[i].complete_obstacle_information.size() ; j++ ){

				int temp_x_cord_1 =  every_level_info[i].complete_obstacle_information[j].print_ld_corner_x() ; 
				int temp_x_cord_2 =  every_level_info[i].complete_obstacle_information[j].print_ru_corner_x() ; 

				int temp_y_cord_1 =  every_level_info[i].complete_obstacle_information[j].print_ld_corner_y() ; 
				int temp_y_cord_2 =  every_level_info[i].complete_obstacle_information[j].print_ru_corner_y() ; 

 				for(int k = temp_x_cord_1  ;  k <= temp_x_cord_2  ;  k++   ){
					for(int h = temp_y_cord_1  ;  h <= temp_y_cord_2 ;  h++   ){
						/*-----------------------------------------------------------------------------
						// 這裡  ， 要把 obstacle的 id 灌進去   ,  id 就是在  complete_obstacle_information 內的 index 
						every_level_info[i].obstacle_id[k][h] =  
										every_level_info[i].complete_obstacle_information[j].print_obstacle_id() ; 
						 *-----------------------------------------------------------------------------*/
						every_level_info[i].obstacle_id[k][h] = j ; 
					}
				}// end of 把obstacle 的資料加入　，　二個plane中　

		}// loop j 

	}// scan 各層
	
	if( debug == true  ){
		cout << "Line 756 "  << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 *  
	 第二個引數 是在 之後建3D_OASG 時 ， 要合進去的vector 
	 由 every_level_info.size()  可以知道  總共有幾層 	
	 先從最底層往上 projection , 先打pin 再打 obstacle 
	 打過去有三種 1。 打到 pin  2. 是打到 obstacle corner 
	 3. 打到空白 ， 除了一班的空白外 obstacle corner 間的邊界
	    也算是空白  
	    打上去 是在前二個case 則不用加點進去 ， 只有第三種case 要加
	    加的話  就加在 vertex_list 中 , 因為會從 這一層往上看一層
	    所以 index 要注意 不能指到底 ,  只能指到前一層 
	    我直接用pin_plane 來看即可  ，  因為 它讓你可以用obstacle的週圍 
	    而且  裡面還有pin 點的資訊 ， 只要對上去 是false 的 就可以用了 
	    那等於是打到 obstacle body 的 是最麻煩的 
	 *-----------------------------------------------------------------------------*/

	for(unsigned  int i = 0 ; i < every_level_info.size() - 1 ; i++ ){

		// 第三個參數 ， 因為 是二維的vector 所以不能直接傳 要用 指標, 直接傳過去的這個東西 ， 應該就是一維的第一個位置 
		projection_detail( every_level_info[i] , every_level_info[ i + 1]  , vertical_edge ) ; 	
	
	}// 由底往上掃過去
	

	if( debug == true  ){
		cout << "Line 784 "  << endl ; 
	}
	 for( unsigned int i = 0 ; i < vertical_edge.size() ; i++ ){
	 	int index_of_same_element = -1 ; 
		bool have_same_elemnt = false ; 
	 	// 不之道能不能用 ==  來重載這個運算元 ， 用來比對	 
		for(unsigned int j = i + 1 ; j < vertical_edge.size() ; j++ ){

			if( vertical_edge[i].print_node1_x() == vertical_edge[j].print_node1_x() &&
			    vertical_edge[i].print_node1_y() == vertical_edge[j].print_node1_y() &&
			    vertical_edge[i].print_node1_z() == vertical_edge[j].print_node1_z() && 
			    vertical_edge[i].print_node2_x() == vertical_edge[j].print_node2_x() &&
			    vertical_edge[i].print_node2_y() == vertical_edge[j].print_node2_y() &&
			    vertical_edge[i].print_node2_z() == vertical_edge[j].print_node2_z() ){
			    	have_same_elemnt  = true ; 
				index_of_same_element = j ;  
				break ; 
			}
			else if( vertical_edge[i].print_node1_x() == vertical_edge[j].print_node2_x() &&
			         vertical_edge[i].print_node1_y() == vertical_edge[j].print_node2_y() &&
			         vertical_edge[i].print_node1_z() == vertical_edge[j].print_node2_z() && 
			         vertical_edge[i].print_node2_x() == vertical_edge[j].print_node1_x() &&
			         vertical_edge[i].print_node2_y() == vertical_edge[j].print_node1_y() &&
			         vertical_edge[i].print_node2_z() == vertical_edge[j].print_node1_z() ){
			    	have_same_elemnt  = true ; 
				index_of_same_element = j ;  
				break ; 
			}

		}// end of loop j 

		if( have_same_elemnt == true  ){
			// cout << " Is it happen " << endl; 
			vertical_edge.erase( vertical_edge.begin() + index_of_same_element ); 	 
			i = i - 1 ; 
		}

	 }// check vertical_edge , do not have same element 

	for( int i =  every_level_info.size() - 1 ; i >= 1  ; i-- ){
	
		projection_detail( every_level_info[i] , every_level_info[ i - 1 ]  , vertical_edge ) ; 	

	}// 由上往下掃 

	if( debug == true  ){
		cout << "Line 790 "  << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 *  最後這裡 我想把 vertical_edge 內的edge 抓來看看  ， 
	 *  把有相同的edge 弄掉  
	 *-----------------------------------------------------------------------------*/
	 for( unsigned int i = 0 ; i < vertical_edge.size() ; i++ ){
	 	int index_of_same_element = -1 ; 
		bool have_same_elemnt = false ; 
	 	// 不之道能不能用 ==  來重載這個運算元 ， 用來比對	 
		for(unsigned int j = i + 1 ; j < vertical_edge.size() ; j++ ){

			if( vertical_edge[i].print_node1_x() == vertical_edge[j].print_node1_x() &&
			    vertical_edge[i].print_node1_y() == vertical_edge[j].print_node1_y() &&
			    vertical_edge[i].print_node1_z() == vertical_edge[j].print_node1_z() && 
			    vertical_edge[i].print_node2_x() == vertical_edge[j].print_node2_x() &&
			    vertical_edge[i].print_node2_y() == vertical_edge[j].print_node2_y() &&
			    vertical_edge[i].print_node2_z() == vertical_edge[j].print_node2_z() ){
			    	have_same_elemnt  = true ; 
				index_of_same_element = j ;  
				break ; 
			}
			else if( vertical_edge[i].print_node1_x() == vertical_edge[j].print_node2_x() &&
			         vertical_edge[i].print_node1_y() == vertical_edge[j].print_node2_y() &&
			         vertical_edge[i].print_node1_z() == vertical_edge[j].print_node2_z() && 
			         vertical_edge[i].print_node2_x() == vertical_edge[j].print_node1_x() &&
			         vertical_edge[i].print_node2_y() == vertical_edge[j].print_node1_y() &&
			         vertical_edge[i].print_node2_z() == vertical_edge[j].print_node1_z() ){
			    	have_same_elemnt  = true ; 
				index_of_same_element = j ;  
				break ; 
			}

		}// end of loop j 

		if( have_same_elemnt == true  ){
			// cout << " Is it happen " << endl; 
			vertical_edge.erase( vertical_edge.begin() + index_of_same_element ); 	 
			i = i - 1 ; 
		}

	 }// check vertical_edge , do not have same element 


	 /*-----------------------------------------------------------------------------
	  *  這裡要建 projection_vertex 的 txt 檔  
	  *  檔有建起來 ， 不過裡面是空的丫 ， 很奇怪 ， 是不是 projection_vertex 內 我跟
	  *  本沒有放東西進去
	  *-----------------------------------------------------------------------------*/
	ofstream fcout("projection_vertex.txt");

	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		// 這裡的projection_vertex ， 沒有沒被我含到 Overlap_vertex 
		for( unsigned int j = 0 ; j < every_level_info[i].projection_vertex.size() ; j++  ){
			// 下一行 ， 我很明顯的 ， 我的vertex 的 z_cord 有點錯 
			 every_level_info[i].projection_vertex[j].print_3D_position_gnu(fcout) ;	
			// 奇怪了 z 軸的值 ， 我是用  level_info的which_level 放進去的哎
			// every_level_info[i].projection_vertex[j].print_3D_position_gnu(fcout , i ) ;	
		}// 每層內的 projection_vertex

	}// 一層層的看 

	fcout.close();	

	
}		/* -----  end of function generate_project_vertex  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_project_vertex_for_overlap
 *  Description:   這樣一來  ， 這個只要寫個大概的樣子出來 就可以了 主軸 就交給 projection_detail
 * =====================================================================================
 */
	void
Projection_handler::generate_project_vertex_for_overlap ( vector<level_info> &every_level_info , vector<edge> &vertical_edge )
{
	int debug = 0 ; 
	if( debug == 1  ){
		cout << "I get in  generate_project_vertex_for_overlap " << endl ; 
	}

	//  我想先用一個vector 把各層中 ， array_vertex 內的東西 都塞入 這個新的vector 內
	// vector<vertex> mirror_of_array_vertex[ every_level_info.size() ] ; 
	for(unsigned  int i = 0 ; i < every_level_info.size()  ; i++ ){

		for(int j = 0 ; j < every_level_info[i].total ; j++ ){
			int x = every_level_info[i].array_vertex[j].print_x_position()  ; 
			int y = every_level_info[i].array_vertex[j].print_y_position()  ; 
			/*-----------------------------------------------------------------------------
			 我想想看丫  ， array_vertex 內 ， 應該沒有把 overlap_vertex 放進去  
			 那這樣的話  ， 下面要再多個loop 放它進去 , 因果 我確實沒放進去
			 mirror_of_array_vertex[i].push_back( every_level_info[i].array_vertex[j] ); 
			 多下面這個  是為了 以後  ， 放便處理 ， 用vector 來呼叫 方便多了 
			every_level_info[i].array_vertex_on_position[ every_level_info[i].array_vertex[j].print_x_position() ].push_back(
									every_level_info[i].array_vertex[j] )  ; 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].array_vertex_vector.push_back( every_level_info[i].array_vertex[j] ) ; 
			every_level_info[i].only_vertex[x][y] = true ; 
		}// copy 各個vertex 

		// overlap_vertex 的點也要加進去 ， 因為它也要算是要投點的點  , 這裡的overlap_vertex , 應是指  obstacle 相交的
		// 那個點 
		for(unsigned int j = 0 ; j < every_level_info[i].overlap_vertex.size() ; j++ ){
			int x = every_level_info[i].overlap_vertex[j].print_x_position()  ; 
			int y = every_level_info[i].overlap_vertex[j].print_y_position()  ; 
			/*-----------------------------------------------------------------------------
			 mirror_of_array_vertex[i].push_back( every_level_info[i].overlap_vertex[j] ); 
			 多下面這個  是為了 以後  ， 放便處理 ， 用vector 來呼叫 方便多了 
			every_level_info[i].array_vertex_on_position[ every_level_info[i].array_vertex[j].print_x_position() ].push_back(
									every_level_info[i].array_vertex[j] )  ; 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].array_vertex_vector.push_back( every_level_info[i].overlap_vertex[j] ) ; 
			every_level_info[i].only_vertex[x][y] = true ; 
		}// scan all overlap_vertex 

	}// scan 各層


	/*-----------------------------------------------------------------------------
	 *  
	 第二個引數 是在 之後建3D_OASG 時 ， 要合進去的vector 
	 由 every_level_info.size()  可以知道  總共有幾層 	
	 先從最底層往上 projection , 先打pin 再打 obstacle 
	 打過去有三種 1。 打到 pin  2. 是打到 obstacle corner 
	 3. 打到空白 ， 除了一班的空白外 obstacle corner 間的邊界
	    也算是空白  
	    打上去 是在前二個case 則不用加點進去 ， 只有第三種case 要加
	    加的話  就加在 vertex_list 中 , 因為會從 這一層往上看一層
	    所以 index 要注意 不能指到底 ,  只能指到前一層 
	    我直接用pin_plane 來看即可  ，  因為 它讓你可以用obstacle的週圍 
	    而且  裡面還有pin 點的資訊 ， 只要對上去 是false 的 就可以用了 
	    那等於是打到 obstacle body 的 是最麻煩的 
	 *-----------------------------------------------------------------------------*/

	if( debug == 1  ){
		cout << "Do projection  " << endl ; 
	}

	for(unsigned  int i = 0 ; i < every_level_info.size() - 1 ; i++ ){

		// 第三個參數 ， 因為 是二維的vector 所以不能直接傳 要用 指標, 直接傳過去的這個東西 ， 應該就是一維的第一個位置 
		// 我各層中 ， 所抓的點 ， 都沒有含第三維的維度 ， 原來我是在建edge 時 ， 才會加上去
		projection_detail_for_overlap( every_level_info[i] , every_level_info[ i + 1]  , vertical_edge ) ; 	
	
	}// 由底往上掃過去
	

	for( int i =  every_level_info.size() - 1 ; i >= 1  ; i-- ){
	
		projection_detail_for_overlap( every_level_info[i] , every_level_info[ i - 1 ]  , vertical_edge ) ; 	

	}// 由上往下掃 


	/*-----------------------------------------------------------------------------
	 *  最後這裡 我想把 vertical_edge 內的edge 抓來看看  ， 
	 *  把有相同的edge 弄掉  
	 *-----------------------------------------------------------------------------*/
	if( debug == 1  ){
		cout << "check edge already have   " << endl ; 
	}

	 for( unsigned int i = 0 ; i < vertical_edge.size() ; i++ ){
	 	int index_of_same_element = -1 ; 
		bool have_same_elemnt = false ; 
	 	// 不之道能不能用 ==  來重載這個運算元 ， 用來比對	 
		for(unsigned int j = i + 1 ; j < vertical_edge.size() ; j++ ){

			if( vertical_edge[i].print_node1_x() == vertical_edge[j].print_node1_x() &&
			    vertical_edge[i].print_node1_y() == vertical_edge[j].print_node1_y() &&
			    vertical_edge[i].print_node1_z() == vertical_edge[j].print_node1_z() && 
			    vertical_edge[i].print_node2_x() == vertical_edge[j].print_node2_x() &&
			    vertical_edge[i].print_node2_y() == vertical_edge[j].print_node2_y() &&
			    vertical_edge[i].print_node2_z() == vertical_edge[j].print_node2_z() ){
			    	have_same_elemnt  = true ; 
				index_of_same_element = j ;  
				break ; 
			}
			else if( vertical_edge[i].print_node1_x() == vertical_edge[j].print_node2_x() &&
			         vertical_edge[i].print_node1_y() == vertical_edge[j].print_node2_y() &&
			         vertical_edge[i].print_node1_z() == vertical_edge[j].print_node2_z() && 
			         vertical_edge[i].print_node2_x() == vertical_edge[j].print_node1_x() &&
			         vertical_edge[i].print_node2_y() == vertical_edge[j].print_node1_y() &&
			         vertical_edge[i].print_node2_z() == vertical_edge[j].print_node1_z() ){
			    	have_same_elemnt  = true ; 
				index_of_same_element = j ;  
				break ; 
			}

		}// end of loop j 

		if( have_same_elemnt == true  ){
			vertical_edge.erase( vertical_edge.begin() + index_of_same_element ); 	 
			i = i - 1 ; 
		}

	 }// check vertical_edge , do not have same element 


	 /*-----------------------------------------------------------------------------
	  *  這裡要建 projection_vertex 的 txt 檔  
	  *  檔有建起來 ， 不過裡面是空的丫 ， 很奇怪 ， 是不是 projection_vertex 內 我跟
	  *  本沒有放東西進去
	  *-----------------------------------------------------------------------------*/
	if( debug == 1  ){
		cout << " Do projection_vertex.txt  " << endl ; 
	}
	ofstream fcout("projection_vertex.txt");

	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		// 這裡的projection_vertex ， 沒有沒被我含到 Overlap_vertex 
		for( unsigned int j = 0 ; j < every_level_info[i].projection_vertex.size() ; j++  ){
			// 下一行 ， 我很明顯的 ， 我的vertex 的 z_cord 有點錯 
			 every_level_info[i].projection_vertex[j].print_3D_position_gnu(fcout) ;	
			// 奇怪了 z 軸的值 ， 我是用  level_info的which_level 放進去的哎
			// every_level_info[i].projection_vertex[j].print_3D_position_gnu(fcout , i ) ;	
		}// 每層內的 projection_vertex

	}// 一層層的看 

	fcout.close();	

	/*-----------------------------------------------------------------------------
	 *  應該弄個 ， 3D_overlap_vertex , 來印出來就好了 ， projection vertex 實在是太多了
	 *  而且在這裡 ， Projection_vertex 有含到 overlap_vertex  , 我只想把overlap_vertex 
	 *  給納進來就好弓 
	 *-----------------------------------------------------------------------------*/
	if( debug == 1  ){
		cout << " Do  Overlap_Vertex_3D.txt " << endl ; 
	}
	ofstream ffcout("Overlap_Vertex_3D.txt");

	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		// 這裡的projection_vertex ， 沒有沒被我含到 Overlap_vertex 
		for( unsigned int j = 0 ; j < every_level_info[i].overlap_vertex.size() ; j++  ){
			// 下一行 ， 我很明顯的 ， 我的vertex 的 z_cord 有點錯   , 回去把建overlap的地方
			// 改一下 ， 讓它有建
			 every_level_info[i].overlap_vertex[j].print_3D_position_gnu(ffcout) ;	
			// 奇怪了 z 軸的值 ， 我是用  level_info的which_level 放進去的哎
			// every_level_info[i].projection_vertex[j].print_3D_position_gnu(fcout , i ) ;	
		}// 每層內的 projection_vertex

	}// 一層層的看 

	ffcout.close();	

	
}		/* -----  end of function generate_project_vertex  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_all_vertex_and_vertical_edge
 *  Description:  
 * =====================================================================================
 */
	void
Projection_handler::generate_all_vertex_and_vertical_edge_gnu( vector<level_info> &every_level_info , vector<edge> &vertical_edge )
{

	ofstream SGcout("Projection_vertex.gnu");
	SGcout << " set title \"Projection_vertex\" " << endl ; 
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
			// 來畫    
			
			every_level_info[i].complete_obstacle_information[j].print_3D_obstacle(SGcout ,  every_level_info[i].which_level , arrow_count ) ; 	

		}// end of print out all obstacle  variable j 

	}// end of get every_level_info to print out  variable i 


	//那這裡要印出　OASG 內的所有line  , 我還要多加上 vertical_edge 的line 
	//這裡 我有新發現 ， edge 的印出  ， 要給number  , 如果  edge number 給
	//重復 ， 似乎只會印後面的 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	// 下面這個是 vertical_edge 的 
	for( unsigned  int i = 0  ; i < vertical_edge.size() ; i++ ){
		arrow_count++ ; 
		vertical_edge[i].print_3D_edge_information_for_gnu( SGcout ,  arrow_count  ) ;  
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
	 *-----------------------------------------------------------------------------*/

	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;

}		/* -----  end of function generate_all_vertex_and_vertical_edge  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_all_vertex_and_vertical_edge_for_overlap
 *  Description:  
 * =====================================================================================
 */
	void
Projection_handler::generate_all_vertex_and_vertical_edge_for_overlap_gnu( vector<level_info> &every_level_info , vector<edge> &vertical_edge )
{

	ofstream SGcout("Projection_vertex.gnu");
	SGcout << " set title \"Projection_vertex\" " << endl ; 
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
			// 來畫    
			
			every_level_info[i].complete_obstacle_information[j].print_3D_obstacle(SGcout ,  every_level_info[i].which_level , arrow_count ) ; 	

		}// end of print out all obstacle  variable j 

	}// end of get every_level_info to print out  variable i 


	//那這裡要印出　OASG 內的所有line  , 我還要多加上 vertical_edge 的line 
	//這裡 我有新發現 ， edge 的印出  ， 要給number  , 如果  edge number 給
	//重復 ， 似乎只會印後面的 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	// 下面這個是 vertical_edge 的 
	for( unsigned  int i = 0  ; i < vertical_edge.size() ; i++ ){
		arrow_count++ ; 
		vertical_edge[i].print_3D_edge_information_for_gnu( SGcout ,  arrow_count  ) ;  
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
	 *-----------------------------------------------------------------------------*/

	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" , \"Overlap_Vertex_3D.txt\" w points  pt 7  lc rgb  \"green\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;

}		/* -----  end of function generate_all_vertex_and_vertical_edge  ----- */

#endif 
