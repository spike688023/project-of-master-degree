/*
 * =====================================================================================
 *
 *       Filename:  rectilinear.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/02/09 16時50分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef       RECTILINEAR_H 
	#define  RECTILINEAR_H 
#include	<iostream>
#include	<fstream>
#include	<cmath>
#include	<map>
#include	<stack>
#include	"edge.h"
#include	"vertex.h"
#include 	"obstacle.h"	
#include 	"level_info.h"	
#include	<vector>
#include        <cstdio>
using namespace std ; 

 class rectilinear{

 private:

 public:

	 /*-----------------------------------------------------------------------------
	  *  vector<edge> &SG_edge , vertex *array_vertex , vector<edge> &obstacle_avoding_Kruskal , int const total , 
					      vector<edge> &OASG , vector<obstacle> &complete_obstacle_information
	  *-----------------------------------------------------------------------------*/
	void change_to_rectilinear( vector<edge> &obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal  ) ; 
	void change_to_rectilinear_by_me_mix_author( vector<edge> &obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal  ) ; 
	// 下面二個func  , 第一個參數  我用call by value  , 因為裡面分別會有一些排序的動作  ， 我不想影響到其它的 ， 所以就用 value 
	void change_to_rectilinear_by_author( vector<edge> obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal  ) ; 
	void change_to_rectilinear_by_teacher( vector<edge> obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal  ) ; 
					      
	void obstacle_avoding_rectilinear_Kruskal_gnu(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , level_info &my_level_info );
	void overlap_obstacle_avoding_rectilinear_Kruskal_gnu(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , level_info &my_level_info );
	void overlap_obstacle_avoding_rectilinear_Kruskal_gnu_trans_eps(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , level_info &my_level_info );

	/*-----------------------------------------------------------------------------
	 *  這裡 ， 要再寫一個 用來 印出 3D 的 gnu 檔
	 *-----------------------------------------------------------------------------*/
	void ThreeD_obstacle_avoding_rectilinear_Kruskal_gnu(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , vector<level_info> &every_level_info );

};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  two_edges_overlap_or_not
 *  Description:  為什麼我下面那個func ， 的輸入參數 ， 不能用const 
 *  		  const edge &edg1  , 為什麼會錯 我在 class edge 中 的copy constructor也
 *  		  是這麼定的丫 
 * =====================================================================================
 */
bool	
two_edges_overlap_or_not ( edge &edg1 , edge &edg2 )
{

	// 這裡 為什麼要個neighbor 的變數呢   , 因為  box 要有overlap , 要二個有相鄰  或 相交才行 
	// 一邊相鄰  ， 一邊相交 ， 或  二邊都相交  才算是 有overlap , 不然會發生  ， 一邊相交  另一邊沒相鄰也沒相交  
	// 這種不會產生  overlap 
	bool overlap = false ; 
	int neighbor = 0 ; 
	if( edg1.print_max_y() >  edg2.print_min_y() &&
	    edg1.print_min_y() <  edg2.print_max_y() ){
		// overlap成立
		overlap = true ; 
		neighbor++;
	}
	else if( edg1.print_min_y() ==  edg2.print_max_y() ||  
		 edg1.print_max_y() ==  edg2.print_min_y() ){
		neighbor++; 
	}
	// x 軸的判斷了 
	if( edg1.print_max_x() >  edg2.print_min_x() &&
	    edg1.print_min_x() <  edg2.print_max_x() ){
		// overlap成立
		overlap = true ; 
		neighbor++;
	}
	else if( edg1.print_min_x() ==  edg2.print_max_x() ||  
		 edg1.print_max_x() ==  edg2.print_min_x() ){
		neighbor++; 
	}


	if( overlap == true && neighbor == 2 ){
		return true ; 
	}
	else{
		return false ; 
	}

}		/* -----  end of function two_edges_overlap_or_not  ----- */


bool	
two_edges_overlap_or_not_by_new_method ( edge &edg1 , edge &edg2 )
{

	// 先抓出有沒有共用點 ， 
	bool find_common_vertex = false ; 
	if( edg1.print_node1_x() == edg2.print_node1_x() &&
	    edg1.print_node1_y() == edg2.print_node1_y() ){
		find_common_vertex = true ;  
	}
	else if( edg1.print_node1_x() == edg2.print_node2_x() &&
	         edg1.print_node1_y() == edg2.print_node2_y() ){
		find_common_vertex = true ;  
	}

	if( edg1.print_node2_x() == edg2.print_node1_x() &&
	    edg1.print_node2_y() == edg2.print_node1_y() ){
		find_common_vertex = true ;  
	}
	else if( edg1.print_node2_x() == edg2.print_node2_x() &&
	         edg1.print_node2_y() == edg2.print_node2_y() ){
		find_common_vertex = true ;  
	}

	// 這裡要抓的是  看有沒有overlap  , 而不用去注意  ，是x or y 的overlap 
	bool overlap = false ; 
	if( edg1.print_max_y() >  edg2.print_min_y() &&
	    edg1.print_min_y() <  edg2.print_max_y() ){
		// overlap成立
		overlap = true ; 
	}
	else if( edg1.print_max_x() >  edg2.print_min_x() &&
	    	 edg1.print_min_x() <  edg2.print_max_x() ){
		// overlap成立
		overlap = true ; 
	}

	// 最後輸出 
	if( find_common_vertex == true && overlap == true  ){
		return true ;  
	}
	else {
		return false ; 
	}

}		/* -----  end of function two_edges_overlap_or_not  ----- */


int cal_overlap_value( edge &e1 , edge &e2 ){

	// 因為前提 已經算好了  ， 是否有overlap 
	// 所以這裡只要看  ， x, y 軸相交的分別是多少
	// 把值加一加就可以了 
	int x_value = 0 , y_value = 0 ; 
	// 下面這塊 是 y 相交的 case 
	if( e1.print_max_y() >  e2.print_min_y() &&
	    e1.print_min_y() <  e2.print_max_y() ){

		// 這裡是 判斷  y 是否有相交  ， 有的話  就要算 overlap 的值  ，
		// 很簡單  ， 有相交  就取其 box 在y軸上  ， 邊長最短的那個  , 多個等號好了 
		// 免得它們一樣大小
		if( ( e1.print_max_y() - e1.print_min_y() ) >= ( e2.print_max_y() - e2.print_min_y() ) ){
			y_value = y_value +  ( e2.print_max_y() - e2.print_min_y() ) ; 
		}
		else if( ( e1.print_max_y() - e1.print_min_y() ) <= ( e2.print_max_y() - e2.print_min_y() ) ){
			y_value = y_value +  ( e1.print_max_y() - e1.print_min_y() ) ; 
		}
		
	}//  先算 y 軸相交的值是多少  
	
	// 下面這塊 是 x 相交的 case 
	if( e1.print_max_x() >  e2.print_min_x() &&
	    e1.print_min_x() <  e2.print_max_x() ){

		// 這裡是 判斷  y 是否有相交  ， 有的話  就要算 overlap 的值  ，
		// 很簡單  ， 有相交  就取其 box 在y軸上  ， 邊長最短的那個  , 多個等號好了 
		// 免得它們一樣大小
		if( ( e1.print_max_x() - e1.print_min_x() ) >= ( e2.print_max_x() - e2.print_min_x() ) ){
			x_value = x_value +  ( e2.print_max_x() - e2.print_min_x() ) ; 
		}
		else if( ( e1.print_max_x() - e1.print_min_x() ) <= ( e2.print_max_x() - e2.print_min_x() ) ){
			x_value = x_value +  ( e1.print_max_x() - e1.print_min_x() ) ; 
		}
		
	}//  先算 y 軸相交的值是多少  

	return ( x_value + y_value ) ; 

}// 計算 這個二個 edge overlap 的值是多少 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  two_edge_at_same_floor
 *  Description:  
 * =====================================================================================
 */
bool	
two_edge_at_same_floor ( edge &e1 , edge &e2 )
{
	
	bool e1_vertical_or_not = true , e2_vertical_or_not = true ; 

	if( e1.print_node1_z() ==  e1.print_node2_z() ){
		e1_vertical_or_not = false ;  
	}

	if( e2.print_node1_z() ==  e2.print_node2_z() ){
		e2_vertical_or_not = false ;  
	}

	// 都為 ， false  代表他們不是垂直的edge  , 而是在plane上的 
	if( e1_vertical_or_not == false && e2_vertical_or_not == false  &&  e1.print_node1_z() == e2.print_node1_z() ){
		return true ; 
	}
	else {
		return false ; 	
	}

}		/* -----  end of function two_edge_at_same_floor  ----- */


void rectilinearize_near_case(edge &e1 , edge &e2 , vector<edge>& vec ){
	bool debug = false ; 

	if( debug == true  ){
		cout << "I get in rectilinearize_near_case "  << endl ; 
	}

	vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
	vector<vertex> all_vertex ; 
	all_vertex.push_back( vertex( e1.print_node1_x() , e1.print_node1_y() , e1.print_node1_z() ) );
	all_vertex.push_back( vertex( e1.print_node2_x() , e1.print_node2_y() , e1.print_node2_z() ) );
	all_vertex.push_back( vertex( e2.print_node1_x() , e2.print_node1_y() , e2.print_node1_z() ) );
	all_vertex.push_back( vertex( e2.print_node2_x() , e2.print_node2_y() , e2.print_node2_z() ) );
	// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
	// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
	unsigned int index_i = 0 , index_j = 0  ; 
	bool find_common_vertex = false ; 
	for(unsigned int i = 0 ; i < all_vertex.size() - 2 ; i++ ){

		for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

			if( all_vertex[i].print_x_position() == all_vertex[j].print_x_position() &&
			    all_vertex[i].print_y_position() == all_vertex[j].print_y_position() ){

				index_i = i ;  
				index_j = j ;  
				common_vertex.set_position( all_vertex[i].print_x_position() , all_vertex[i].print_y_position() );
				common_vertex.set_z_position( all_vertex[i].print_z_position() );
				find_common_vertex = true ; 
				break ; 

			}// end of ix
		 
		}// loop j 
	 
	}// loop i 
	// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
	// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置 
	if( find_common_vertex == true ){
	 
		all_vertex.erase( all_vertex.begin() + index_j ) ; 
		all_vertex.erase( all_vertex.begin() + index_i ) ; 
		// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
		near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
		near.set_z_position( all_vertex[0].print_z_position() );
		far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
		far.set_z_position( all_vertex[1].print_z_position() );

	}

	if( debug == true  ){
		cout << "I get in rectilinearize_near_case , and process the data done "  << endl ; 
	}

	bool x_overlap = false , y_overlap = false ; 

	if( e1.print_max_x() >  e2.print_min_x() &&
	    e1.print_min_x() <  e2.print_max_x() ){
	    x_overlap = true ; 
	}

	if( e1.print_max_y() >  e2.print_min_y() &&
	    e1.print_min_y() <  e2.print_max_y() ){
	    y_overlap = true ;
	}

	// 以 y 軸為主要的分割線 , 我在看 輸出的圖時 ， 會有edge 連到  最下面  那也就是 x 的位置為0的地方  ， 
	// 所以是  ， 值有錯 ， 或是有人的值沒有給改到  
	if(  y_overlap == true && find_common_vertex == true ){

		// 我打算把  near 拿來存  ， 跟common_vertex , 最近的那個  , near 存的是近點
		// 這裡是把 近點  都調到 near ， 下面的code 就會比較好寫 
		if( fabs( near.print_y_position()  - common_vertex.print_y_position() ) >
		    fabs( far.print_y_position() - common_vertex.print_y_position() ) ){
		    vertex temp( near  ) ; 
		    near.set_position( far.print_x_position() , far.print_y_position() ) ; 
		    far.set_position( temp.print_x_position() , temp.print_y_position() ) ; 
		}
		int z = common_vertex.print_z_position() ; 
		// 先建立 主軸上的線 ，  也就是 ， 依common的 x 配合 ， 另外二點的 y , 建出二個小段的直線
		// 哇  ， 我還要看 ， 這二個點 ， 的y 距離 ， 誰離  common_vertex 比較近跟遠 
		//  哇 ， 其中的 一條線 有可能是 直線的話  ， 要再另外 考慮 
		if(  near.print_y_position() == far.print_y_position()  ){
			//  這個 case 下  只會有一個  steiner 
			vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
					     common_vertex.print_x_position() , near.print_y_position() 	 , z )  ) ;	
			// 那二條橫線 , 基本上 z 放誰的 都一樣  ， 沒差  
			vec.push_back( edge( near.print_x_position() , near.print_y_position() , z ,
					     common_vertex.print_x_position() , near.print_y_position()  , z )  ) ;	
			vec.push_back( edge( far.print_x_position() , far.print_y_position() , z ,
					     common_vertex.print_x_position() , near.print_y_position() , z )  ) ;	

		 
		}// 這個 case 是說 ， 只會有一個 steiner point 
		else {
		    	// 先處理近點 ， 且判斷是否直線  , 近點是直線
		    	if( near.print_x_position() == common_vertex.print_x_position() ){
				// 直線的 edge 
				vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
						     near.print_x_position()          , near.print_y_position() 	 , z )  ) ;	
				// 切成 L 型的 二個edge 
				vec.push_back( edge( common_vertex.print_x_position() , far.print_y_position() , z ,
						     near.print_x_position()          , near.print_y_position()  , z )  ) ;	
				vec.push_back( edge( common_vertex.print_x_position() , far.print_y_position() , z ,
						     far.print_x_position()         , far.print_y_position() 	 , z )  ) ;	
			}// near 跟common_vertex 之間是條直線 ， 那這條 不用改 
		 	// 遠點是直線  
		    	else if( far.print_x_position() == common_vertex.print_x_position() ){
				// 切成 L 型的 二個edge  , 這裡是要先建 near 的
				vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
						     common_vertex.print_x_position() , near.print_y_position() 	 , z )  ) ;	
				vec.push_back( edge( near.print_x_position() , near.print_y_position() , z ,
						     common_vertex.print_x_position() , near.print_y_position()	 , z )  ) ;	
				// 直線的 edge 
				vec.push_back( edge( far.print_x_position() , far.print_y_position() , z ,
						     common_vertex.print_x_position() , near.print_y_position()  , z )  ) ;	
			}// near 跟common_vertex 之間是條直線 ， 那這條 不用改 
			else {
				// near 的 L 型  
				vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
						     common_vertex.print_x_position() , near.print_y_position() 	 , z )  ) ;	
				vec.push_back( edge( common_vertex.print_x_position() , near.print_y_position() , z ,
						     near.print_x_position() , near.print_y_position() 	 , z )  ) ;	
				// far 的 L 型  
				vec.push_back( edge( common_vertex.print_x_position() , near.print_y_position() , z ,
						     common_vertex.print_x_position() , far.print_y_position() 	 , z )  ) ;	
				vec.push_back( edge( far.print_x_position() , far.print_y_position() , z ,
						     common_vertex.print_x_position() , far.print_y_position() 	 , z )  ) ;	
			
			} // 二個edge 都是斜線的 

		} // normal case , 
	 
	}// end fo  y_overlap 
	
	// 以 x 軸為主要的分割線
	if(  x_overlap == true && find_common_vertex == true ){
	 
		// 我打算把  near 拿來存  ， 跟common_vertex , 最近的那個  , near 存的是近點
		// 這裡是把 近點  都調到 near ， 下面的code 就會比較好寫 
		if( fabs( near.print_x_position()  - common_vertex.print_x_position() ) >
		    fabs( far.print_x_position() - common_vertex.print_x_position() ) ){
		    vertex temp( near  ) ; 
		    near.set_position( far.print_x_position() , far.print_y_position() ) ; 
		    far.set_position( temp.print_x_position() , temp.print_y_position() ) ; 

		}
		int z = common_vertex.print_z_position() ; 
		// 先建立 主軸上的線 ，  也就是 ， 依common的 x 配合 ， 另外二點的 y , 建出二個小段的直線
		// 哇  ， 我還要看 ， 這二個點 ， 的y 距離 ， 誰離  common_vertex 比較近跟遠 
		//  哇 ， 其中的 一條線 有可能是 直線的話  ， 要再另外 考慮 
		if(  near.print_x_position() == far.print_x_position()  ){
			//  這個 case 下  只會有一個  steiner 
			vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
					     near.print_x_position() , common_vertex.print_y_position() 	 , z )  ) ;	
			// 那二條橫線 , 基本上 z 放誰的 都一樣  ， 沒差  
			vec.push_back( edge( near.print_x_position() , near.print_y_position() , z ,
					     near.print_x_position() , common_vertex.print_y_position()  , z )  ) ;	
			vec.push_back( edge( far.print_x_position() , far.print_y_position() , z ,
					     near.print_x_position() , common_vertex.print_y_position() , z )  ) ;	

		 
		}// 這個 case 是說 ， 只會有一個 steiner point 
		else {
		    	// 先處理近點 ， 且判斷是否直線  , 近點是直線
		    	if( near.print_y_position() == common_vertex.print_y_position() ){
				// 直線的 edge 
				vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
						     near.print_x_position()          , near.print_y_position() 	 , z )  ) ;	
				// 切成 L 型的 二個edge 
				vec.push_back( edge( far.print_x_position() , common_vertex.print_y_position() , z ,
						     near.print_x_position()          , near.print_y_position()  , z )  ) ;	
				vec.push_back( edge( far.print_x_position() , common_vertex.print_y_position() , z ,
						     far.print_x_position()         , far.print_y_position() 	 , z )  ) ;	
			}// near 跟common_vertex 之間是條直線 ， 那這條 不用改 
		 	// 遠點是直線  
		    	else if( far.print_y_position() == common_vertex.print_y_position() ){
				// 切成 L 型的 二個edge  , 這裡是要先建 near 的
				vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
						     near.print_x_position() , common_vertex.print_y_position() 	 , z )  ) ;	
				vec.push_back( edge( near.print_x_position() , near.print_y_position() , z ,
						     near.print_x_position() , common_vertex.print_y_position()	 , z )  ) ;	
				// 直線的 edge 
				vec.push_back( edge( far.print_x_position() , far.print_y_position() , z ,
						     near.print_x_position() , common_vertex.print_y_position()  , z )  ) ;	
			}// near 跟common_vertex 之間是條直線 ， 那這條 不用改 
			else {
				// near 的 L 型  
				vec.push_back( edge( common_vertex.print_x_position() , common_vertex.print_y_position() , z ,
						     near.print_x_position() , common_vertex.print_y_position() 	 , z )  ) ;	
				vec.push_back( edge( near.print_x_position() , near.print_y_position() , z ,
						     near.print_x_position() , common_vertex.print_y_position()	 , z )  ) ;	
				// far 的 L 型  
				vec.push_back( edge( far.print_x_position() , common_vertex.print_y_position() , z ,
						     near.print_x_position()          , common_vertex.print_y_position()  , z )  ) ;	
				vec.push_back( edge( far.print_x_position() , common_vertex.print_y_position() , z ,
						     far.print_x_position()         , far.print_y_position() 	 , z )  ) ;	
			
			} // 二個edge 都是斜線的 

		} // normal case , 
	 
	}// end of x_overlap
	
}  // end of rectilinearize_near_case

void rectilinearize_cross_case(edge &e1 , edge &e2 , vector<edge>& vec ){

	bool debug = false ; 

	if( debug == true  ){
		cout << "I get in rectilinearize_cross_case "  << endl ; 
	}

	vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
	vector<vertex> all_vertex ; 
	all_vertex.push_back( vertex( e1.print_node1_x() , e1.print_node1_y() , e1.print_node1_z() ) );
	all_vertex.push_back( vertex( e1.print_node2_x() , e1.print_node2_y() , e1.print_node2_z() ) );
	all_vertex.push_back( vertex( e2.print_node1_x() , e2.print_node1_y() , e2.print_node1_z() ) );
	all_vertex.push_back( vertex( e2.print_node2_x() , e2.print_node2_y() , e2.print_node2_z() ) );
	// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
	// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
	unsigned int index_i = 0 , index_j = 0  ; 
	bool find_common_vertex = false ; 
	for(unsigned int i = 0 ; i < all_vertex.size() - 2 ; i++ ){

		for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

			if( all_vertex[i].print_x_position() == all_vertex[j].print_x_position() &&
			    all_vertex[i].print_y_position() == all_vertex[j].print_y_position() ){

				index_i = i ;  
				index_j = j ;  
				common_vertex.set_position( all_vertex[i].print_x_position() , all_vertex[i].print_y_position() );
				common_vertex.set_z_position( all_vertex[i].print_z_position() );
				find_common_vertex = true ; 
				break ; 

			}// end of ix
		 
		}// loop j 
	 
	}// loop i 

	// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
	// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置  , 奇怪了 ， 那我為什麼 ， 
	// 不先刪第二個  ， 刪了也不會 effect 到第一個的位置丫 
	if( find_common_vertex == true ){
	 
		all_vertex.erase( all_vertex.begin() + index_j ) ; 
		all_vertex.erase( all_vertex.begin() + index_i ) ; 
		// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
		near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
		near.set_z_position( all_vertex[0].print_z_position() );
		far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
		far.set_z_position( all_vertex[1].print_z_position() );

	}


	// 看來不用分像限 ， 建的方式都一樣丫 
	int steiner_x = 0 , steiner_y = 0 , z = common_vertex.print_z_position() ;  
	if( fabs( (  near.print_y_position() - common_vertex.print_y_position() ) ) < 
	    fabs( (  far.print_y_position()  - common_vertex.print_y_position() ) ) ){
		// 這樣代表 ， near 的 y  比較近  
		steiner_y  = near.print_y_position() ;  
		steiner_x  = far.print_x_position() ;  
	}
	else if( fabs( (  near.print_y_position() - common_vertex.print_y_position() ) ) > 
		 fabs( (  far.print_y_position()  - common_vertex.print_y_position() ) ) ){
		 //  這樣則是 far 的y 比較近 
		steiner_y  = far.print_y_position() ;  
		steiner_x  = near.print_x_position() ;  
	}

	if( find_common_vertex == true  ){
	 
		// 先建3，4的  比較直觀  , 先把這個x,y跟  near ,far 連線 ， 要建二條 
		vec.push_back( edge( steiner_x , steiner_y  , z ,
				     near.print_x_position() , near.print_y_position()  , z )  ) ;	
		vec.push_back( edge( steiner_x , steiner_y  , z ,
				     far.print_x_position() , far.print_y_position() 	 , z )  ) ;	
		// 這裡的話 ， 就建 ， steiner 跟common_vertex 間的 二個直線了  
		vec.push_back( edge( steiner_x , steiner_y  , z ,
				     common_vertex.print_x_position() , steiner_y  , z )  ) ;	
		vec.push_back( edge( common_vertex.print_x_position() , steiner_y  , z ,
				     common_vertex.print_x_position() , common_vertex.print_y_position() 	 , z )  ) ;	

	}

}  // end of rectilinearize_cross_case 

bool two_box_near_or_not(edge &e1 , edge &e2){

	bool x_overlap = false , y_overlap = false ; 

	if( e1.print_max_x() >  e2.print_min_x() &&
	    e1.print_min_x() <  e2.print_max_x() ){
	    x_overlap = true ; 
	}

	if( e1.print_max_y() >  e2.print_min_y() &&
	    e1.print_min_y() <  e2.print_max_y() ){
	    y_overlap = true ;
	}

	if( x_overlap == true && y_overlap == false ){
		return true ;  
	}
	else if( x_overlap == false && y_overlap == true ){
		return true ;  
	}
	else {
		return false ;  
	}

} // end of two_box_near_or_not

bool two_box_cross_or_not(edge &e1 , edge &e2){

	bool x_overlap = false , y_overlap = false ; 

	if( e1.print_max_x() >  e2.print_min_x() &&
	    e1.print_min_x() <  e2.print_max_x() ){
	    x_overlap = true ; 
	}

	if( e1.print_max_y() >  e2.print_min_y() &&
	    e1.print_min_y() <  e2.print_max_y() ){
	    y_overlap = true ;
	}

	if( x_overlap == true && y_overlap == true ){
		return true ;  
	}
	else{
		return false ; 
	}

} // end of two_box_corss_or_not

/*-----------------------------------------------------------------------------
 *   sort overlap_count ,   降密， 
 *-----------------------------------------------------------------------------*/
 bool sort_overlap_count(edge e1 , edge e2){
	return ( e1.print_overlap_count() > e2.print_overlap_count() );	 
 }

// 下面這個 ， 是 增加的 
 bool sort_overlap_edge_weight(edge e1 , edge e2){
	return ( e1.print_weight() > e2.print_weight() );	 
 }

 bool sort_overlap_edge_weight_decreasing(edge e1 , edge e2){
	return ( e1.print_weight() < e2.print_weight() );	 
 }

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  one_edge_rectilinear
 *  Description:  
 * =====================================================================================
 */
	void
one_edge_rectilinear ( edge& edg , vector<edge>& vec  )
{
		
	// 奇怪了 ， 是錯在下面這個if , 就沒有進去了哎 
	if( edg.Is_vertical_or_horizontal_line() == false ){
		// 我想預設是右下角的三角型 
		int left_down_corner_min_x = edg.print_min_x() ;
		int left_down_corner_min_y = edg.print_min_y() ;
		int right_down_corner_max_x = edg.print_max_x() ;
		int right_down_corner_min_y = edg.print_min_y() ;
		// 下面這二種加edge的方式 ， 只適用於左下右上的edge , 所以 另一種edge的調整 ， 要另外寫
		// 如何判斷 edge 是左下右上 or 左上右下 ， 藉由二個點的 x 跟x 相減 ， y 跟y 相減
		// 左下右上的case  , 其相減二個都是正 要麻是負 
		if( edg.Is_left_down_right_up_case() == true ){
			vec.push_back( edge(right_down_corner_max_x , right_down_corner_min_y  , edg.print_node1_z() , 
					    edg.print_node1_x()     , edg.print_node1_y()      , edg.print_node1_z() ) );
			vec.push_back( edge(right_down_corner_max_x , right_down_corner_min_y  , edg.print_node1_z() , 
					    edg.print_node2_x()     , edg.print_node2_y()      , edg.print_node1_z() ) );
		}
		else if( edg.Is_left_down_right_up_case() == false ){
			vec.push_back( edge(left_down_corner_min_x , left_down_corner_min_y , edg.print_node1_z() , 
					    edg.print_node1_x()    , edg.print_node1_y()    , edg.print_node1_z() ) );
			vec.push_back( edge(left_down_corner_min_x , left_down_corner_min_y , edg.print_node1_z() , 
					    edg.print_node2_x()    , edg.print_node2_y()    , edg.print_node1_z() ) );
		}
	 
	}// 非水平垂直的線 ， 才要修改 
	else if( edg.Is_vertical_or_horizontal_line() == true ){
		// 直接把此水平 垂直的edge 放入 rectilinear_kruskal
		vec.push_back( edg );	
	}


}		/* -----  end of function one_edge_rectilinear  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  abstract_case_one  
 *  Description:  這裡 是要處理 ， 開頭的第一個元素的overlap_count == 1 的case 
 *  		  這表示 ， 後面也都為1  ， 則 要抓出的解 便是唯一 , 我還是把
 *  		  這二種case 給分開來， 寫好了  
 *  		  雖然 抽像化 成這樣 ， 但能處理的case , 好像只有 二個edges 間的overlap 
 *  		  , 3個edges 的overlap  , 一定會有一個 是 overlap_count > 1  的
 * =====================================================================================
 */
	void
abstract_case_one ( vector<edge>& handle , vector<edge>& vec  )
{
	/*-----------------------------------------------------------------------------
	 *  
	 for test code cout << handle.size() << endl ;   
	 				step 1 . 
	 將原始 edge 的分身 都給弄出來 ， 且給上 rec_id , 由1 開始給 
	          要注意 水平跟垂直的線 沒有分身 ， 只要把自已給丟出去 ， 再記得給上
	          rec_id 即可
	          斜線的話 ， 會分出四條線 ， 二組rec_id  
	 *-----------------------------------------------------------------------------*/
	int rec_id = 1 ; 
	// 在 have_rec_id 中的 edges  , 都是水平跟垂直的了
	vector<edge> have_rec_id ; 
	for(unsigned int i = 0 ; i < handle.size() ; i++ ){

		// 這邊用來處理分身	
		if( handle[i].Is_vertical_or_horizontal_line() == true ){
			handle[i].set_rec_id( rec_id ) ;  
			have_rec_id.push_back( handle[i] );
		}
		else if( handle[i].Is_vertical_or_horizontal_line() == false ){
			// 這裡的話 因為是斜線  ， 所以 有分二種  ， 且
			// 二種 轉rec 分身的方式也不同  
			if( handle[i].Is_left_down_right_up_case() == true  ){
				// 這是左下右上的case 	 , 因為知道了edge的種類 ， 所以只要藉由 max min x y 
				// 就可以建出要的edge 了 
				// 下三角的水平線 , 我怕這裡的建構子會抓錯人  , 這裡 放10個參數  才不會建錯人
				have_rec_id.push_back( edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							     handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() , rec_id , 
							     false 		     , true 		       , false  			    )  );
				// 下三角的垂直線
				have_rec_id.push_back( edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,  
							     handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							     false 		     , true 		       , false  			    )  );
				rec_id++ ;
				// 上三角的水平線
				have_rec_id.push_back( edge( handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,  
							     handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							     false 		     , true 		       , false  			    )  );
				// 上三角的垂直線
				have_rec_id.push_back( edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							     handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							     false 		     , true 		       , false  			    )  );

			}
			else if( handle[i].Is_left_down_right_up_case() == false ){
				// 這裡是 左上右下的case 
				// 下三角的水平線
				have_rec_id.push_back( edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							     handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() , rec_id ,  
							     false 		     , true 		       , false  			    )  );
				// 下三角的垂直線
				have_rec_id.push_back( edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							     handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							     false 		     , true 		       , false  			    )  );
				rec_id++ ;
				// 上三角的水平線
				have_rec_id.push_back( edge( handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,
							     handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							     false 		     , true 		       , false  			    )  );
				// 上三角的垂直線
				have_rec_id.push_back( edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							     handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							     false 		     , true 		       , false  			    )  );
			}
		 
		}// end of if
		rec_id++ ; 
	}// end of for give rec_id


	
	/*-----------------------------------------------------------------------------
	 *  				step 2 
	 *  	上個step 已經轉好分身 ， 給好id 了 ， 這裡要用個vector  記下來
	 *  	所有  彼此 有overlap 的edge , 其 rec_id ， 這樣可以縮小 ，要考慮
	 *  	的 edge 數目 , 我記得沒錯的話 ， 兩edge 要有 overlap 要
	 *  	水平對 水平  垂直 對垂直  ， 對到 後還要看 是不是相同的 x 跟 y  ,
	 *  	上面二個條件對到後 ， 要
	 *  	min j < max i && max j > min i  成立 才ok  
	 *  	j i 指到那個edge  都可以  
	 *-----------------------------------------------------------------------------*/
	vector<int> overlap_rec_id ; 
	for(unsigned int i = 0 ; i <  have_rec_id.size() - 1 ; i++ ){
		// 利用bubble sort  兩兩比對 ， 先比出  id 不同
		// 再看 水平垂直  context 內才比  overlap 
		for(unsigned int j = i + 1 ; j < have_rec_id.size() ; j++ ){

			if( ( have_rec_id[i].print_rec_id() != have_rec_id[j].print_rec_id() )  && 
			    ( have_rec_id[i].Is_horizontal_line() == have_rec_id[j].Is_horizontal_line() )  ){
				// 要先看這裡是水平的overlap or vertical overlap 
				// 水平的話   ， 要看這二個的  y 是否相
				if(  have_rec_id[i].Is_horizontal_line() == true && 
				     ( have_rec_id[i].print_max_y() == have_rec_id[j].print_max_y() )  &&
				     ( have_rec_id[i].print_min_x() <  have_rec_id[j].print_max_x() )  && 
				     ( have_rec_id[i].print_max_x() >  have_rec_id[j].print_min_x() )  ){
				     // 上面的條件都通過 ， 代表這二個 rec_id 是可以放入的
				     // 但放入之前  ， 要先看看 ， 裡面是否已經有了 
					int first_rec_id = have_rec_id[i].print_rec_id() ; 	
					int second_rec_id = have_rec_id[j].print_rec_id() ; 	
					bool first_rec_id_in = false ; 
					bool second_rec_id_in = false ; 
					for(unsigned int k = 0 ; k < overlap_rec_id.size() ; k++  ){
						if( first_rec_id == overlap_rec_id[k]  ){
							first_rec_id_in = true ;  
						}
						else if( second_rec_id == overlap_rec_id[k]  ){
							second_rec_id_in = true ;  
						}
					}// end of for k 
					
					if( first_rec_id_in == false  ){
						overlap_rec_id.push_back( first_rec_id ) ; 	 
					}
					if( second_rec_id_in == false  ){
						overlap_rec_id.push_back( second_rec_id ) ; 	 
					}

				}
				else if( have_rec_id[i].Is_horizontal_line() == false &&  
				     ( have_rec_id[i].print_max_x() == have_rec_id[j].print_max_x() )  &&
				     ( have_rec_id[i].print_min_y() <  have_rec_id[j].print_max_y() )  && 
				     ( have_rec_id[i].print_max_y() >  have_rec_id[j].print_min_y() )  ){

					int first_rec_id = have_rec_id[i].print_rec_id() ; 	
					int second_rec_id = have_rec_id[j].print_rec_id() ; 	
					bool first_rec_id_in = false ; 
					bool second_rec_id_in = false ; 
					for(unsigned int k = 0 ; k < overlap_rec_id.size() ; k++  ){
						if( first_rec_id == overlap_rec_id[k]  ){
							first_rec_id_in = true ;  
						}
						else if( second_rec_id == overlap_rec_id[k]  ){
							second_rec_id_in = true ;  
						}
					}// end of for k 
					
					if( first_rec_id_in == false  ){
						overlap_rec_id.push_back( first_rec_id ) ; 	 
					}
					if( second_rec_id_in == false  ){
						overlap_rec_id.push_back( second_rec_id ) ; 	 
					}
				 
				}// end of 垂直的line 比較  有否 overlap 
			}// end of if 判斷id 要不同 ， 要都是水平 或 垂直 


		}// end of for j 
	}// end of for i   , and end of step 2 
		


	/*-----------------------------------------------------------------------------
	 *  				step 3 . 
	 *  	抓到了 所有重要的 rec_id ,  先用一個新的vecotr , 存 有這些id 的
	 *  	edge  , overlap_rec_id 存 有相互overlap 的那些id 
	 *  	have_rec_id 存 各個edge rectilinear 化後的edge  ,  且 給定了 rec_id 值了 
	 *-----------------------------------------------------------------------------*/
	vector<edge> prepare_for_filter ; 
	for(unsigned int i = 0 ; i < overlap_rec_id.size() ; i++ ){
		for(unsigned int j = 0  ; j < have_rec_id.size() ; j++  ){
			if( have_rec_id[j].print_rec_id() == overlap_rec_id[i]  ){
				prepare_for_filter.push_back( have_rec_id[j] ) ;  
			}
		}
	}// end of step 3 

	/*-----------------------------------------------------------------------------
	 *  				step 4 . 
	 *  	做最後的 filter  , 將最後重要的 edge  放入 vec 中
	 *  	也是要像bubble sort  一樣的結構 ， 兩兩做 比較 
	 *  	如果 往後 抓不到  跟自已有overlap 的 ， 便直接放入 
	 *  	vec 中  ， 如果 有的話 便比大小  ， 較大的放入 
	 *-----------------------------------------------------------------------------*/
	 for(unsigned int i = 0  ; i < prepare_for_filter.size() ; i++ ){

		 /*-----------------------------------------------------------------------------
		  *  
		   如果找不到  一個跟自已  overlap 的edge   , 那下面這個bool 
		   就會維持 false  , 就可以直接 把這個edge 放入 
		   哇 這裡又有個新問題了  ， 也就是 找到另一半了 
		   但是它是 index j 指向的 ， 我如果現在把它放入
		   那後面又會再放一次  ， 所以這裡 ， 只要 以 index i 的
		   為主軸 就可以了  ， 當i 指向的edge 比 j 的大 那就可以放
		   反之則不行  ， 抓到自已的另一半 又判斷完了  就可以break  出去了
		   不用把j  跑到底  
		  *-----------------------------------------------------------------------------*/
		bool find_another = false ; 
	 	for(unsigned int j = i + 1 ; j < prepare_for_filter.size() ; j++  ){
			// 下面要先看 find_another , 要不要調整
			if( ( prepare_for_filter[i].print_rec_id() != prepare_for_filter[j].print_rec_id() )  && 
			    ( prepare_for_filter[i].Is_horizontal_line() == prepare_for_filter[j].Is_horizontal_line() )  ){
				 
				if(  prepare_for_filter[i].Is_horizontal_line() == true && 
				     ( prepare_for_filter[i].print_max_y() == prepare_for_filter[j].print_max_y() )  &&
				     ( prepare_for_filter[i].print_min_x() <  prepare_for_filter[j].print_max_x() )  && 
				     ( prepare_for_filter[i].print_max_x() >  prepare_for_filter[j].print_min_x() )  ){
					find_another = true ; 
				}
				else if( prepare_for_filter[i].Is_horizontal_line() == false &&  
				     ( prepare_for_filter[i].print_max_x() == prepare_for_filter[j].print_max_x() )  &&
				     ( prepare_for_filter[i].print_min_y() <  prepare_for_filter[j].print_max_y() )  && 
				     ( prepare_for_filter[i].print_max_y() >  prepare_for_filter[j].print_min_y() )  ){
					find_another = true ; 
				}// end of 垂直的line 比較  有否 overlap 
			}// end of if

			if( find_another == true && 
			    ( prepare_for_filter[i].print_weight() >= prepare_for_filter[j].print_weight() )  ){
				vec.push_back( prepare_for_filter[i] ) ; 	 
				break ;
			}
		 
		}// end of for j 

		if( find_another == false ){
			vec.push_back( prepare_for_filter[i] ) ; 	 
		}

	 }// end of step4 

}		/* -----  end of function abstract_case_one  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  abstract_case_two
 *  Description:  這個 func 傳入二個參數 ， 第一個是 要被 處理的那那edges , 
 *  		  第二個是處理好後 ， 要放入的vector 
 * =====================================================================================
 */
	void
abstract_case_two ( vector<edge>& handle , vector<edge>& vec )
{
		
	/*-----------------------------------------------------------------------------
	 *  
	 *    很好  ， 我的想法是沒錯的 ， 但我從 data 中觀察到 ， overlap_count > 1 
	 *    好 像只有一個 ， 沒有出現二個以上的  , 是我的case 太簡單了嗎 
	 *    但 ， 我只想先實作 簡單的 也就是 ， 看來不行  ， 我能抓出
	 *    有二個 > 2 的edges 的case 而且  怎麼看都是會發生的 ， 所以
	 *    安份點 慢慢做吧 
	 *
	 *    				step 1 .
	 *    	先看這群edges 中 ， 有多少個 edge 的overlap 是 > 1 的 
	 *    	抓好後 ， 建個int array[ overlap_count_greater_one ] , 初值都是0 
	 *      之後要用來 做排列組合要用的 。 
	 *-----------------------------------------------------------------------------*/
	 int overlap_count_greater_one = 0 ; 
	 for(unsigned int i = 0 ; i < handle.size() ; i++ ){
	 	if( handle[i].print_overlap_count() > 1 ){
			overlap_count_greater_one++ ;  
		}
	 }// end of for i 
	
	// 好奇怪喔 ， 它就是沒有 pow(int , int ) 的版本
	int cases =  pow( 2.0 , overlap_count_greater_one )  ; 
	// 算了 我想 還是用 vector 來紀各種 排列組合的cases 吧
	vector<int> combination[ cases ] ; 
	// 好了 ， 接下來 ， 各種case 要塞好它相對的排列組合 ， 以方便後面要用
	// 先給 初值 ， 我打算index 為 0 的 當低位元
	for(int i = 0 ; i < cases ; i++ ){
		// overlap_count_greater_one 的值多少 ， 就給幾個 0
		for(int j = 0 ; j < overlap_count_greater_one ; j++ ){
		 combination[i].push_back( 0 );
		}
	}// initialization all give 0
	
	// 要把各種10 , 01 , 00 , 11 等 各種排列組合 放進去
	// i 由 1 開始 ， 因為不用變 
	for( int i = 1 ; i < cases ; i++ ){
		vector<int> temp_combition ;  
		// 把上個排列組合的值 ， 抓下來 
		for( unsigned int j = 0 ; j < combination[ i - 1 ].size() ; j++ ){
			temp_combition.push_back( combination[ i - 1 ][j] ) ;  
		}
		// 接下來 ， 要先加1 ， 再判斷 那些地方會有進位
		temp_combition[0]++;	
		// 要用個for 判斷各個位元， 要不要做進位的動作
		for(unsigned int j = 0 ; j < temp_combition.size() ; j++ ){
			if( temp_combition[j] == 2 ){
				temp_combition[ j + 1 ]++ ;  
				temp_combition[j] = 0  ; 
			}
		}

		// 上面抓好要的值了 現在放進去
		for( unsigned int j = 0 ; j < combination[i].size() ; j++ ){
		 	combination[i][j] = temp_combition[j] ; 
		}
	}// 給定 排列組合的值 


	/*-----------------------------------------------------------------------------
	 *  				check code , May 9
	// 要不要先印出來看看  , 當然要丫 , 目前看來是可以
	for( int i = 0 ; i < cases ; i++ ){
		for( unsigned int j = 0 ; j < combination[i].size() ; j++ ){
		 	cout << combination[i][j]   ; 
		}
		cout << endl ; 
	}
	 *
	 *-----------------------------------------------------------------------------*/
	// 這二個變數 是放來比較用的 
	int min_weight = 0  ;
	vector<edge> min_weight_vec ; 

	// 因為 ， 要抓 weight 最小的那個 case , 所以要用個 while 確定每種case都有跑
	int where_reach = 0 ; 
	while( where_reach < cases ){

		/*-----------------------------------------------------------------------------
		 *  
		 *  				step 1 . 給定rec_id 
		   用where_reach 來抓 ， 現在要用那種 combination ， 以及 
		   是否所有case 都跑過了  , 那這裡 ， 要先把rec_id 給抓出來 
		   因為一組combination 內的 overlap_count > 1  的元素數目 ， 
		   跟傳入的handle 內的 數目是相同的 ， 所以只要 overlap_count > 1 的
		   就到 combination 內 ， 看是要抓 上 or 下 三角  
		   0 抓上三角 ， 1抓下
		 *-----------------------------------------------------------------------------*/
		 vector<edge> have_rec_id ; 
		 int rec_id = 1 ; 
		 for(unsigned int i = 0  ; i < handle.size() ; i++ ){
			  if( handle[i].print_overlap_count() > 1 ){
			  	// 這裡就要看 ， 在combination內的值是多少 ， 才能決定是要上or 下三角	 
				if( combination[where_reach][i] == 0 ){
							 
					if( handle[i].Is_vertical_or_horizontal_line() == true ){
						handle[i].set_rec_id( rec_id ) ;  
						have_rec_id.push_back( handle[i] );
					}
					else if( handle[i].Is_vertical_or_horizontal_line() == false ){
						// 這裡的話 因為是斜線  ， 所以 有分二種  ， 且
						// 二種 轉rec 分身的方式也不同  
						if( handle[i].Is_left_down_right_up_case() == true  ){
							// 這是左下右上的case 	 , 因為知道了edge的種類 ， 所以只要藉由 max min x y 
							// 就可以建出要的edge 了 
							// 上三角的水平線
							have_rec_id.push_back( 
								edge( handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,
								      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,rec_id ,
								      false  		      , true 			, false 			    )  );
							// 上三角的垂直線
							have_rec_id.push_back( 
								edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
								      handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
								      false  		      , true 			, false 			     )  );

						}
						else if( handle[i].Is_left_down_right_up_case() == false ){
							// 這裡是 左上右下的case 
							// 上三角的水平線
							have_rec_id.push_back( 
								edge( handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,
								      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id , 
								      false  		      , true 			, false 			     )  );
							// 上三角的垂直線
							have_rec_id.push_back( 
								edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
								      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
								      false  		      , true 			, false 			     )  );
						}
					 
					}// end of if 看 edge 的型態 
				}// 0 取上三角 
				else if( combination[where_reach][i] == 1 ){
				 
					if( handle[i].Is_vertical_or_horizontal_line() == true ){
						handle[i].set_rec_id( rec_id ) ;  
						have_rec_id.push_back( handle[i] );
					}
					else if( handle[i].Is_vertical_or_horizontal_line() == false ){
						// 這裡的話 因為是斜線  ， 所以 有分二種  ， 且
						// 二種 轉rec 分身的方式也不同  
						if( handle[i].Is_left_down_right_up_case() == true  ){
							// 這是左下右上的case 	 , 因為知道了edge的種類 ， 所以只要藉由 max min x y 
							// 就可以建出要的edge 了 
							// 下三角的水平線
							have_rec_id.push_back( 
								edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
								      handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() , rec_id ,
								      false  		      , true 			, false 			     )  );
							// 下三角的垂直線
							have_rec_id.push_back( 
								edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
								      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id , 
								      false  		      , true 			, false 			     )  );

						}
						else if( handle[i].Is_left_down_right_up_case() == false ){
							// 這裡是 左上右下的case 
							// 下三角的水平線
							have_rec_id.push_back( 
								edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
								      handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() , rec_id ,
								      false  		      , true 			, false 			     )  );
							// 下三角的垂直線
							have_rec_id.push_back( 
								edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
								      handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
								      false  		      , true 			, false 			     )  );
						}
					 
					}// end of else if 看 edge 的型態 
				}// end of 值為1 取下三角 
				// 因為這裡加的edge 都只有一組  所以上面的內部 ， 不用 
				// 自已去加rec_id 
				rec_id++ ; 
			  }// end of overlap_count > 1
			  else if( handle[i].print_overlap_count() == 1 ){
			   
				if( handle[i].Is_vertical_or_horizontal_line() == true ){
					handle[i].set_rec_id( rec_id ) ;  
					have_rec_id.push_back( handle[i] );
				}
				else if( handle[i].Is_vertical_or_horizontal_line() == false ){
					// 這裡的話 因為是斜線  ， 所以 有分二種  ， 且
					// 二種 轉rec 分身的方式也不同  
					if( handle[i].Is_left_down_right_up_case() == true  ){
						// 這是左下右上的case 	 , 因為知道了edge的種類 ， 所以只要藉由 max min x y 
						// 就可以建出要的edge 了 
						// 下三角的水平線
						have_rec_id.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
						// 下三角的垂直線
						have_rec_id.push_back( 
							edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
						rec_id++ ;
						// 上三角的水平線
						have_rec_id.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
						// 上三角的垂直線
						have_rec_id.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );

					}
					else if( handle[i].Is_left_down_right_up_case() == false ){
						// 這裡是 左上右下的case 
						// 下三角的水平線
						have_rec_id.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
						// 下三角的垂直線
						have_rec_id.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
						rec_id++ ;
						// 上三角的水平線
						have_rec_id.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
						// 上三角的垂直線
						have_rec_id.push_back( 
							edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() , rec_id ,
							      false  		      , true 			, false 			     )  );
					}
				 
				}// end of if 給定分身
			  }// overlap_count == 1 
		 }//  給定 rec_id  end of for i 



		/*-----------------------------------------------------------------------------
		 *  				step 2 
		 *  	上個step 已經轉好分身 ， 給好id 了 ， 這裡要用個vector  記下來
		 *  	所有  彼此 有overlap 的edge , 其 rec_id ， 這樣可以縮小 ，要考慮
		 *  	的 edge 數目 , 我記得沒錯的話 ， 兩edge 要有 overlap 要
		 *  	水平對 水平  垂直 對垂直  ， 對到 後還要看 是不是相同的 x 跟 y  ,
		 *  	上面二個條件對到後 ， 要
		 *  	min j < max i && max j > min i  成立 才ok  
		 *  	j i 指到那個edge  都可以  
		 *-----------------------------------------------------------------------------*/
		vector<int> overlap_rec_id ; 
		for(unsigned int i = 0 ; i <  have_rec_id.size() - 1 ; i++ ){
			// 利用bubble sort  兩兩比對 ， 先比出  id 不同
			// 再看 水平垂直  context 內才比  overlap 
			for(unsigned int j = i + 1 ; j < have_rec_id.size() ; j++ ){

				if( ( have_rec_id[i].print_rec_id() != have_rec_id[j].print_rec_id() )  && 
				    ( have_rec_id[i].Is_horizontal_line() == have_rec_id[j].Is_horizontal_line() )  ){
					// 要先看這裡是水平的overlap or vertical overlap 
					// 水平的話   ， 要看這二個的  y 是否相
					if(  have_rec_id[i].Is_horizontal_line() == true && 
					     ( have_rec_id[i].print_max_y() == have_rec_id[j].print_max_y() )  &&
					     ( have_rec_id[i].print_min_x() <  have_rec_id[j].print_max_x() )  && 
					     ( have_rec_id[i].print_max_x() >  have_rec_id[j].print_min_x() )  ){
					     // 上面的條件都通過 ， 代表這二個 rec_id 是可以放入的
					     // 但放入之前  ， 要先看看 ， 裡面是否已經有了 
						int first_rec_id = have_rec_id[i].print_rec_id() ; 	
						int second_rec_id = have_rec_id[j].print_rec_id() ; 	
						bool first_rec_id_in = false ; 
						bool second_rec_id_in = false ; 
						for(unsigned int k = 0 ; k < overlap_rec_id.size() ; k++  ){
							if( first_rec_id == overlap_rec_id[k]  ){
								first_rec_id_in = true ;  
							}
							else if( second_rec_id == overlap_rec_id[k]  ){
								second_rec_id_in = true ;  
							}
						}// end of for k 
						
						if( first_rec_id_in == false  ){
							overlap_rec_id.push_back( first_rec_id ) ; 	 
						}
						if( second_rec_id_in == false  ){
							overlap_rec_id.push_back( second_rec_id ) ; 	 
						}

					}
					else if( have_rec_id[i].Is_horizontal_line() == false &&  
					     ( have_rec_id[i].print_max_x() == have_rec_id[j].print_max_x() )  &&
					     ( have_rec_id[i].print_min_y() <  have_rec_id[j].print_max_y() )  && 
					     ( have_rec_id[i].print_max_y() >  have_rec_id[j].print_min_y() )  ){

						int first_rec_id = have_rec_id[i].print_rec_id() ; 	
						int second_rec_id = have_rec_id[j].print_rec_id() ; 	
						bool first_rec_id_in = false ; 
						bool second_rec_id_in = false ; 
						for(unsigned int k = 0 ; k < overlap_rec_id.size() ; k++  ){
							if( first_rec_id == overlap_rec_id[k]  ){
								first_rec_id_in = true ;  
							}
							else if( second_rec_id == overlap_rec_id[k]  ){
								second_rec_id_in = true ;  
							}
						}// end of for k 
						
						if( first_rec_id_in == false  ){
							overlap_rec_id.push_back( first_rec_id ) ; 	 
						}
						if( second_rec_id_in == false  ){
							overlap_rec_id.push_back( second_rec_id ) ; 	 
						}
					 
					}// end of 垂直的line 比較  有否 overlap 
				}// end of if 判斷id 要不同 ， 要都是水平 或 垂直 


			}// end of for j 
		}// end of for i   , and end of step 2 


		/*-----------------------------------------------------------------------------
		 *  				step 3 . 
		 *  	抓到了 所有重要的 rec_id ,  先用一個新的vecotr , 存 有這些id 的
		 *  	edge  , overlap_rec_id 存 有相互overlap 的那些id 
		 *  	have_rec_id 存 各個edge rectilinear 化後的edge  ,  且 給定了 rec_id 值了 
		 *-----------------------------------------------------------------------------*/
		vector<edge> prepare_for_filter ; 
		for(unsigned int i = 0 ; i < overlap_rec_id.size() ; i++ ){
			for(unsigned int j = 0  ; j < have_rec_id.size() ; j++  ){
				if( have_rec_id[j].print_rec_id() == overlap_rec_id[i]  ){
					prepare_for_filter.push_back( have_rec_id[j] ) ;  
				}
			}
		}// end of step 3 

		/*-----------------------------------------------------------------------------
		 *  				step 4 .
		 *  	哇， 我的想法有點錯 ， 藉由排列組合 ， 雖能抓出 總體weight最小的
		 *  	但是 ， 一個edge如果 overlap_count 到3 那代表 ， 有一個三角
		 *  	是二面都有相鄰 ， 如果 選那一面  那會造成 ， 有一個edge 的二個分身
		 *  	會完全不被選到 , 但我想到一個  稍微能解決的方法 ， 由edge 的二端點 
		 *      來看 ， 一個edge 做rectilinear後 ， 的二個分身都沒被選到 ， 代表這個
		 *      edge 的二端點 ， 其中一個  跟 temp_min_weight_vec 中的所有點比  ， 會都
		 *      對不到  ， 這便是我少加入的edge  . 
		 *      看來  這要塞到別的地方 ， 因為 我上面的方法 ， 會有一點錯 
		 *      overlap的二個rectilinear edge 我會取大的 ， 這會導致 小的edge 的某個點
		 *      在我上面的方法  ， 會找不到  , 我要把這一塊 塞到 ， edge 間
		 *      還沒有 overlap 的edge 比大小 取大的那裡  
		 *-----------------------------------------------------------------------------*/
		 for( unsigned int i = 0  ; i < handle.size() ; i++ ){
			bool first_node = false , second_node = false ; 
			for( unsigned int j = 0 ; j < prepare_for_filter.size() ; j++ ){
				//  first_node 先比 
				if( first_node == false && 
				    handle[i].print_node1_x() == prepare_for_filter[j].print_node1_x() && 
				    handle[i].print_node1_y() == prepare_for_filter[j].print_node1_y() ){
					first_node = true ;  
				}
				else if( first_node == false && 
					 handle[i].print_node1_x() == prepare_for_filter[j].print_node2_x() && 
					 handle[i].print_node1_y() == prepare_for_filter[j].print_node2_y() ){
					first_node = true ;  
				}

				//  second_node 先比 
				if( second_node == false && 
				    handle[i].print_node2_x() == prepare_for_filter[j].print_node1_x() && 
				    handle[i].print_node2_y() == prepare_for_filter[j].print_node1_y() ){
					second_node = true ;  
				}
				else if( second_node == false && 
					 handle[i].print_node2_x() == prepare_for_filter[j].print_node2_x() && 
					 handle[i].print_node2_y() == prepare_for_filter[j].print_node2_y() ){
					second_node = true ;  
				}

				if(  first_node == true && second_node == true){
					break ;  
				}
			}// end of for j 
			// 只要這個edge 有分身在 那這二個bool 變數 ， 一定都會是true 

			if(  first_node == false || second_node == false ){
				// 只要有一個是false 代表這個edge  沒有一個分身給放入的 
				// 我們就要幫它把分身放下去, 我想預設都放下三角好了  
				if( handle[i].Is_vertical_or_horizontal_line() == true ){
					prepare_for_filter.push_back( handle[i] );
				}
				else if( handle[i].Is_vertical_or_horizontal_line() == false ){
					// 這裡的話 因為是斜線  ， 所以 有分二種  ， 且
					// 二種 轉rec 分身的方式也不同  
					if( handle[i].Is_left_down_right_up_case() == true  ){
						// 這是左下右上的case 	 , 因為知道了edge的種類 ， 所以只要藉由 max min x y 
						// 就可以建出要的edge 了 
						// 下三角的水平線
						prepare_for_filter.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() )  );
						// 下三角的垂直線
						prepare_for_filter.push_back( 
							edge( handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_max_y() , handle[i].print_node1_z() )  );

					}
					else if( handle[i].Is_left_down_right_up_case() == false ){
						// 這裡是 左上右下的case 
						// 下三角的水平線
						prepare_for_filter.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() ,
							      handle[i].print_max_x() , handle[i].print_min_y() , handle[i].print_node1_z() )  );
						// 下三角的垂直線
						prepare_for_filter.push_back( 
							edge( handle[i].print_min_x() , handle[i].print_min_y() , handle[i].print_node1_z() , 
							      handle[i].print_min_x() , handle[i].print_max_y() , handle[i].print_node1_z() )  );
					}
				 
				}// end of else if 看 edge 的型態 
			}// end of   first_node == false || second_node == false
		 }// end of for i 


		/*-----------------------------------------------------------------------------
		 *  				step 5 . 
		 *  	做最後的 filter  , 將最後重要的 edge  放入 temp_vec 中
		 *  	也是要像bubble sort  一樣的結構 ， 兩兩做 比較 
		 *  	如果 往後 抓不到  跟自已有overlap 的 ， 便直接放入 
		 *  	temp_vec 中  ， 如果 有的話 便比大小  ， 較大的放入 
		 *-----------------------------------------------------------------------------*/
		 vector<edge> temp_vec ; 
		 for(unsigned int i = 0  ; i < prepare_for_filter.size() ; i++ ){

			 /*-----------------------------------------------------------------------------
			  *  
			   如果找不到  一個跟自已  overlap 的edge   , 那下面這個bool 
			   就會維持 false  , 就可以直接 把這個edge 放入 
			   哇 這裡又有個新問題了  ， 也就是 找到另一半了 
			   但是它是 index j 指向的 ， 我如果現在把它放入
			   那後面又會再放一次  ， 所以這裡 ， 只要 以 index i 的
			   為主軸 就可以了  ， 當i 指向的edge 比 j 的大 那就可以放
			   反之則不行  ， 抓到自已的另一半 又判斷完了  就可以break  出去了
			   不用把j  跑到底  
			  *-----------------------------------------------------------------------------*/
			bool find_another = false ; 
			for(unsigned int j = i + 1 ; j < prepare_for_filter.size() ; j++  ){
				// 下面要先看 find_another , 要不要調整
				if( ( prepare_for_filter[i].print_rec_id() != prepare_for_filter[j].print_rec_id() )  && 
				    ( prepare_for_filter[i].Is_horizontal_line() == prepare_for_filter[j].Is_horizontal_line() )  ){
					 
					if(  prepare_for_filter[i].Is_horizontal_line() == true && 
					     ( prepare_for_filter[i].print_max_y() == prepare_for_filter[j].print_max_y() )  &&
					     ( prepare_for_filter[i].print_min_x() <  prepare_for_filter[j].print_max_x() )  && 
					     ( prepare_for_filter[i].print_max_x() >  prepare_for_filter[j].print_min_x() )  ){
						find_another = true ; 
					}
					else if( prepare_for_filter[i].Is_horizontal_line() == false &&  
					     ( prepare_for_filter[i].print_max_x() == prepare_for_filter[j].print_max_x() )  &&
					     ( prepare_for_filter[i].print_min_y() <  prepare_for_filter[j].print_max_y() )  && 
					     ( prepare_for_filter[i].print_max_y() >  prepare_for_filter[j].print_min_y() )  ){
						find_another = true ; 
					}// end of 垂直的line 比較  有否 overlap 
				}// end of if

				if( find_another == true && 
				    ( prepare_for_filter[i].print_weight() >= prepare_for_filter[j].print_weight() )  ){
					temp_vec.push_back( prepare_for_filter[i] ) ; 	 
					break ;
				}
			 
			}// end of for j 

			if( find_another == false ){
				temp_vec.push_back( prepare_for_filter[i] ) ; 	 
			}

		 }// end of step4 

		
		 /*-----------------------------------------------------------------------------
		  *  				Step 5 . 
		  *  	因為這是比較麻煩的case , 所以 這裡 只要做比較 記下 總weight 最小的那個 
		  *  	要設個新變數 temp_min_weight_vec , 用來做比較的 ， 
		  *  	比到最後一次 再把這個裡面的值 放回vec 中即可
		  *-----------------------------------------------------------------------------*/
		  int  temp_vec_weight = 0 ; 
		  for(unsigned int i = 0 ; i < temp_vec.size() ; i++ ){
		   	temp_vec_weight = temp_vec_weight + temp_vec[i].print_weight() ; 
		  }
		
		  
		  /*-----------------------------------------------------------------------------
		   *  
				// 這二個變數 是放來比較用的 
				int min_weight = 0  ;
				vector<edge> min_weight_vec ; 
		   *-----------------------------------------------------------------------------*/
		  if( where_reach == 0  ){
		  	//  要先弄個初值給 min_weight_vec 	 
			min_weight = temp_vec_weight ; 
			for( unsigned int i = 0 ; i < temp_vec.size() ; i++ ){
				min_weight_vec.push_back( temp_vec[i] ) ; 
			}
		  }
		  else {
		  	if( temp_vec_weight <  min_weight ){
				// 有新的case ,  比我記的還小 ， 就要把它換過去  
				min_weight = temp_vec_weight ; 
				min_weight_vec.clear() ; 
				for( unsigned int i = 0 ; i < temp_vec.size() ; i++ ){
					min_weight_vec.push_back( temp_vec[i] ) ; 
				}
			}// end of 這個case 比我 記的還要小 
		  }// end of else  block 
		where_reach++ ; 
	}//   用來抓所有的case 的最小weight



	/*-----------------------------------------------------------------------------
	 *  				step 6 . 
	 *  	上面的 那一大塊 while 已經幫我把  ， weight 最小的case  ， 
	 *  	以及 那些edges 給抓出來了  ， 那接下來 只要把 min_weight_vec 
	 *  	內的edge 都放到 vec  中 就可以了 
	 *-----------------------------------------------------------------------------*/

	 for( unsigned int i = 0 ; i < min_weight_vec.size() ; i++ ){
	  	vec.push_back( min_weight_vec[i] ) ; 
	 }



	
}		/* -----  end of function abstract_case_two  ----- */



void rectilinear::change_to_rectilinear( vector<edge> &obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal ){
	
	// 先做分群
	int group_count = 0 ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++  ){
		
		/*-----------------------------------------------------------------------------
		 *   這裡我要用二個變數， 來判斷相鄰這個東西
		 *   bool overlap = false ; 
		 *   int neighbor = 0 ; 
		 *   因為是以 ， x , y 的角度來看 ， 所以 其中一邊要overlap 有成立才行
		 *   只要是相交於一個點 ， 或是 有 overlap  , 其neighbor 這個參數都要＋＋
		 *   才行 ， ＝＞ 也就是說  要 overlap 為 true ,  且 neighbor == 2 
		 *   才是分在同一群的   
		 *-----------------------------------------------------------------------------*/
		 if( obstacle_avoding_Kruskal[i].print_group_id() == 0 ){
		 	group_count++ ;  
			// 自已要先給 group_id 
			obstacle_avoding_Kruskal[i].set_group_id( group_count );
		 }// 沒被分到群 ， 才有被要來看

		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
			// i 指的是 ，主要比對的 對像 ， j 比的是次要比對的對像
			// 這裡面 便是我判斷分群的核心地方了  
			// 先以 y 軸為角度來看 
			// 先寫overlap 的判斷吧
			// 給適當的group_id 
			if( two_edges_overlap_or_not( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] )  ){
				// 這裡面要先多個 判斷 ， i 指向的主要edge  , == 0 or != 0 , 後面id的指定 情況也不同 
				// 我下面if 的判斷式中  ， 變數不應該用i 丫 ， i  指向的一定有個group_id的值 

				/*-----------------------------------------------------------------------------
				 *  
				if( obstacle_avoding_Kruskal[i].print_group_id() == 0 ){
					obstacle_avoding_Kruskal[j].set_group_id( group_count );
					cout << "do i get in " << endl ; 
				}
				 *-----------------------------------------------------------------------------*/
				obstacle_avoding_Kruskal[j].set_group_id( obstacle_avoding_Kruskal[i].print_group_id() );
			}// 給group_id 

		}// end of for j

	 
	}// end of for i


	// 把各自放到 ， 相對應的 ，vector array , 然後各自 做rectilinear 
	// group_count 可以知道 ， 我的 vector array ， 要多大
	// 問題原來是下面 這個 for_rectilinear的size給的不夠 ， 因為  只給 group_count ， 那index 
	// 只會從 0 到 group_count - 1 , 但下面 我在給edge時 ， 如果給的index 超出我原本的設定 ， 那之後將無法取用這個edge
	vector<edge> for_rectilinear[group_count + 1 ];
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++  ){
		for_rectilinear[ obstacle_avoding_Kruskal[i].print_group_id() ].push_back( obstacle_avoding_Kruskal[i] );
	}


	// 下面這塊 ， 我要計算 ， 每個for_rectilinear[index] 的overlap_count 的次數 
	//for(int i = 0 ; i <= group_count && for_rectilinear[i].size() > 1 ; i++ )   為什麼左式的判斷會錯了
	// 會進不去哎 
	for(int i = 0 ; i <= group_count  ; i++ ){
		// 下面二個for 主要是 內部的兩兩比對
		for(unsigned int j = 0  ; j < for_rectilinear[i].size() ; j++ ){
			for(unsigned int k = j + 1 ; k < for_rectilinear[i].size() ; k++   ){
				// 我要想把上面那個 用來判斷 overlap 的那段 code 
				// 改成一個 func ， 然後回傳 true or false 
				
				if( two_edges_overlap_or_not( for_rectilinear[i][j] , for_rectilinear[i][k] )  ){
					for_rectilinear[i][j].overlap_count_add_add();	
					for_rectilinear[i][k].overlap_count_add_add();	
				}

			}// end of for k
		}// end of for j 

		
		/*-----------------------------------------------------------------------------
		 *  
		 *  下面這塊 ， 已經不用做這種事了 
		// 這裡我要把 ， size == 2 的 抓出來 ， 就些有規律的安排 ， 像是 斜線 ， 要放在index 0 的位置上
		// , 如果index 0  的 是 水平 或垂直線 ， 就把index 0 , 1 的做swap
		if( for_rectilinear[i].size() == 2 && for_rectilinear[i][0].Is_vertical_or_horizontal_line() == true ){
			// 看來這裡 ， 要寫個 overload assignement operator   
			edge temp ; 
			temp = for_rectilinear[i][1] ; 
			for_rectilinear[i][1] = for_rectilinear[i][0] ; 
			for_rectilinear[i][0] = temp ; 
		}
		 *-----------------------------------------------------------------------------*/
	}// end of for i 


	// 抓好overlap_count 的次數後 ， 便是 做排序  ， 依 overlap_count 的次數 ， 以降密排 
	// 排是有排好拉 ， 但是 四個edge為一組的 ， 怎麼  overlap_count 是 3,1,1,1
	// 對拉 ， 這也是可以
	for(int i = 0 ; i <= group_count  ; i++ ){
		if( for_rectilinear[i].size() > 1 ){
			sort( for_rectilinear[i].begin() , for_rectilinear[i].end() , sort_overlap_count );	 
		}
	}// end of for i 


	
	/*-----------------------------------------------------------------------------
	 *  要不要寫個 ， debug code 來看看， 東西有沒有 照我的要求排好序 
	 *
	 *  我想到方法 ， 處理size == 3 的case  了 ， 
	 *  只要把edge size 不同的case , 用func 呼叫 來處理 ， 
	 *  這樣 ， 即便我size == 3 裡面有些獨立的case  , 也就 沒關係了 
	 *-----------------------------------------------------------------------------*/

	// 下面這個vector 是我最後一步主要的輸出  
	//vector<edge> obstacle_avoding_rectilinear_Kruskal ;
	for(int i = 0 ; i <= group_count ; i++ ){
			

		/*-----------------------------------------------------------------------------
		 這裡便是retilinear 的核心 ，  看我是能 ， 全部抽像化的去做 rectilinear , 
		 還是 還是要依size 去做rectilinear  , 我記得有 ， 判斷 edge  是水平 or 垂直的func 
		 好了  ， 這裡的rectilinear  , 我要重寫 ， 
		 用一個 我在昨天想到的 ， 新的抽像化方法 ， 只差在
		 我是用 overlap_count 去做分類 ， 因為我有依 overlap_count 去sort 所
		 只要第一元素的overlap_count ==  1  那代表後面也都是 
		 也就是 這種case 的解都是唯一的 ， 
		 剩下的其它case 便要排列組合 ， 抓出其 weight 最小的 來印出來 
		 *-----------------------------------------------------------------------------*/
		if( for_rectilinear[i].size() == 1 ){
			// 怎麼會有edge ， 做了 rectilinear  又把自已給放了進去呢 	
			//  又是gunplot 檔的執行先後的問題 ， 我不太之道是為什麼
			one_edge_rectilinear( for_rectilinear[i][0] , obstacle_avoding_rectilinear_Kruskal );

		}// end of size == 1 的case 
		else if( for_rectilinear[i].size() > 1 ){

			/*-----------------------------------------------------------------------------
			 * 				Check Code . May 9 
			cout << "for_rectilinear[i].size() = " << for_rectilinear[i].size() << endl ; 
			// 奇怪了 怎麼overlap_count 都是 0 
			for(unsigned int j = 0 ; j < for_rectilinear[i].size() ; j++ ){
				cout << "for_rectilinear[i][" << j << "].print_overlap_count() = " 
				<< for_rectilinear[i][j].print_overlap_count() << endl ; 
			}
			 *-----------------------------------------------------------------------------*/
			// 這裡要用個if 來區分 我上面講的二種主要的case 
			if( for_rectilinear[i][0].print_overlap_count() == 1 ){
				// 這二種主要的case  , 我要用 func 來處理 
				// 而傳入的引數 ， 有二個 ， for_rectilinear[i] , 以及 obstacle_avoding_rectilinear_Kruskal , 
				// 這二個應該就夠了 ， 但名字 要取什麼 , 叫 abstract_case_one 
				abstract_case_one( for_rectilinear[i] , obstacle_avoding_rectilinear_Kruskal );
			}
			else if( for_rectilinear[i][0].print_overlap_count() > 1 ){
				abstract_case_two( for_rectilinear[i] , obstacle_avoding_rectilinear_Kruskal );
			}
		 
		}// for_rectilinear[i].size() > 1


			
	}// end of for i 

	 // cout << " obstacle_avoding_rectilinear_Kruskal : " << endl ; 
	 // cout << " obstacle_avoding_rectilinear_Kruskal'size =  " << obstacle_avoding_rectilinear_Kruskal.size() << endl ;
	/*-----------------------------------------------------------------------------
	 *   
	cout << "size of every group " << endl ; 
	int cal_count = 0 ; for(int i = 0 ; i <= group_count ; i++ ){ 
	cal_count = cal_count + for_rectilinear[i].size() ; 
		cout << for_rectilinear[i].size() << " "  ; 
		if( i == group_count ){
			cout << endl << "for_rectilinear[group_count].size() is " << for_rectilinear[group_count].size() << endl ;  
			// 奇怪了 ， 明明size 是1 ， 但我確不能存取來使用， 是怎麼回事
			for_rectilinear[group_count][0].print_kruskal_edge_information() ; 	
		}
	}
	// 奇怪了 為什麼會少了一個edge 沒抓到呢 , 因為我上面對group_count的比對沒有掛上等號
	cout << endl << "size in for_rectilinear is " << cal_count << endl ; 
	 *-----------------------------------------------------------------------------*/

}// end of change_to_rectilinear


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  method by author 
 *  Description:  用以處理 ， edge 間有相交的case , 只需處理 case 2 , 3  
 * =====================================================================================
 */
	void
method_by_author( vector<edge>& handle , vector<edge>& vec  )
{
	bool debug = false ; 
	if(  debug == true ){
		cout << " Get in method_by_author " << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 *  首先呢 ， 傳進來的 handle 是要 處理成 rectilinear 的 ， 而  我已經對 ， weight 做排序了 
	 *  要二個變數 ， 之道index 指向那二個 ， 以及  二個bool 去看說  ， 現在這二個的情況是 ， 那種case
	 *  也用一個ok變數 ， 直到 handle 內的東西都沒了  都跑出while 
	 *-----------------------------------------------------------------------------*/
	 unsigned int index_a = 0 , index_b = 0 ; 
	 bool  ok = false ; 

	 //  用 個等大小的vector 去記  ， 那些edge  已經做了  rectilinear 
	 vector<bool> already_rectilinear  ; 
	 for(unsigned int i = 0 ; i < handle.size() ; i++ ){
	 	already_rectilinear.push_back( false );	 
	 }// scan all elements on handle 

	if(  debug == true ){
		cout << " before get in line 1507 " << endl ; 
	}
	// 這裡 要怎麼寫比較好呢  ， 才不會出錯  ， 因為是要配對  ， 找個sharing edge 最大的一對  ， 
	// 但有可能找不到  ， 
	 while( ok == false ){

		bool find_index_a = false , find_index_b = false ; 
		// 先抓 ， sharing edge 最大的那一對  , 上限弄成 減1  是怕  loop j 會抓不到東西
		// 因為每次抓進來的數目都不同 ， 有奇有偶 ， 會不會有人配不對呢 
		for(unsigned int i = 0 ; i < handle.size()  ; i++ ){

			// 直接 指向第一個  
			if( already_rectilinear[i] == false ){
				find_index_a = true ; 
				index_a =  i ; 
				break ; 
			}// end of already_rectilinear[i] == false

		}// scan all elements on handle , what we need is false element , not do rectilinear 

		// 這裡是要找出  ， index_b 是誰   , index_a 最遠指到  ，倒數第二個   ,  如果物件只有二個  ,  這裡會不會出錯呢  
		// 現在改另一種寫法 ， 就是 index_a 不是指向最後一個元素  
		if( find_index_a == true && index_a < handle.size() - 1 ){

			int temp_overlap_value = 0 ; 
			int max_overlap_value = 0 ; 
			// 有可能發生  index_a 沒指到人丫
			for(unsigned int j = index_a + 1 ; j < handle.size() ; j++  ){
			 
				// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
				// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
				// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
				if( already_rectilinear[j] == false  && two_edges_overlap_or_not_by_new_method( handle[index_a] , handle[j] ) == true  ){
					// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		
					temp_overlap_value = cal_overlap_value( handle[index_a] , handle[j] );

					if( temp_overlap_value > max_overlap_value ){
						max_overlap_value = temp_overlap_value ; 	 
						index_b = j ;
						find_index_b = true ; 
					}

				}// end of already_rectilinear[i] == false

			}// end of loop j 

		}// end of if index_a < handle.size() - 1

		if( find_index_a == true && find_index_b == true  ){
		 
			// 再分它的case   ， 有東西 要考慮 ， 就是  可能會找不到  另一個 sharing edge 最大的下手
			// 所以這個 的主軸還要改一下
			bool two_box_near = false , two_box_cross = false ;  
			two_box_near  = two_box_near_or_not(  handle[index_a] , handle[index_b] );
			two_box_cross = two_box_cross_or_not( handle[index_a] , handle[index_b] );

			if( two_box_near == true ){
				rectilinearize_near_case(handle[index_a] , handle[index_b] ,  vec ) ; 
			}
			else if( two_box_cross == true ){
				rectilinearize_cross_case(handle[index_a] , handle[index_b] ,  vec ) ; 
			}// 因為有可能  有  沒配到對的  所以不能放外面
		 
			already_rectilinear[index_a] = true ; 
			already_rectilinear[index_b] = true ; 

		}// 兩個  ， 有找到配對的  
		else if( find_index_a == true  && find_index_b == false ){
			already_rectilinear[index_a] = true ; 	 
			// 這個就當成是  一個edge 的rectilinear 
			one_edge_rectilinear ( handle[index_a] ,  vec  ) ; 
		}// 對不到人 ， 只有自已一個 
		else if(  find_index_a == false ){
			// index_a 找不到  ， 那代表  ， 就可以出去了
			ok  = true  ;  
		}// index_a  就找不到人了 

	 }// end of while  , handle內的edge 都處理完了  才出來 

	if(  debug == true ){
		cout << " out of while loop  , in line 1586 " << endl ; 
	}

}		/* -----  end of function method_by_author ----- */

void rectilinear::change_to_rectilinear_by_me_mix_author( vector<edge> &obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal ){
	
	// 這個  是我有分群的那個方法  
	bool debug = false ; 	
	if(  debug == true ){
		cout << " We enter change_to_rectilinear_by_author " << endl ; 
	}

	// 先做分群
	int group_count = 0 ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++  ){
		
		/*-----------------------------------------------------------------------------
		 *   這裡我要用二個變數， 來判斷相鄰這個東西
		 *   bool overlap = false ; 
		 *   int neighbor = 0 ; 
		 *   因為是以 ， x , y 的角度來看 ， 所以 其中一邊要overlap 有成立才行
		 *   只要是相交於一個點 ， 或是 有 overlap  , 其neighbor 這個參數都要＋＋
		 *   才行 ， ＝＞ 也就是說  要 overlap 為 true ,  且 neighbor == 2 
		 *   才是分在同一群的   
		 *-----------------------------------------------------------------------------*/
		 if( obstacle_avoding_Kruskal[i].print_group_id() == 0 ){
		 	group_count++ ;  
			// 自已要先給 group_id 
			obstacle_avoding_Kruskal[i].set_group_id( group_count );
		 }// 沒被分到群 ， 才有被要來看

		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
			// i 指的是 ，主要比對的 對像 ， j 比的是次要比對的對像
			// 這裡面 便是我判斷分群的核心地方了  
			// 先以 y 軸為角度來看 
			// 先寫overlap 的判斷吧
			// 給適當的group_id 
			if( two_edges_overlap_or_not( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] )  ){
				// 這裡面要先多個 判斷 ， i 指向的主要edge  , == 0 or != 0 , 後面id的指定 情況也不同 
				// 我下面if 的判斷式中  ， 變數不應該用i 丫 ， i  指向的一定有個group_id的值 

				/*-----------------------------------------------------------------------------
				 *  
				if( obstacle_avoding_Kruskal[i].print_group_id() == 0 ){
					obstacle_avoding_Kruskal[j].set_group_id( group_count );
					cout << "do i get in " << endl ; 
				}
				 *-----------------------------------------------------------------------------*/
				obstacle_avoding_Kruskal[j].set_group_id( obstacle_avoding_Kruskal[i].print_group_id() );
			}// 給group_id 

		}// end of for j

	 
	}// end of for i


	// 把各自放到 ， 相對應的 ，vector array , 然後各自 做rectilinear 
	// group_count 可以知道 ， 我的 vector array ， 要多大
	// 問題原來是下面 這個 for_rectilinear的size給的不夠 ， 因為  只給 group_count ， 那index 
	// 只會從 0 到 group_count - 1 , 但下面 我在給edge時 ， 如果給的index 超出我原本的設定 ， 那之後將無法取用這個edge
	vector<edge> for_rectilinear[group_count + 1 ];
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++  ){
		for_rectilinear[ obstacle_avoding_Kruskal[i].print_group_id() ].push_back( obstacle_avoding_Kruskal[i] );
	}




	/*-----------------------------------------------------------------------------
	 *  這裡我要改成 是sort edge weight  ,  因為要處理  成 rectilinear , 我要從 edge weight 最大的下手 ， 
	 *  往下面找 sharing edge  最大的那個  來做  rectilinear 
	 *-----------------------------------------------------------------------------*/
	for(int i = 0 ; i <= group_count  ; i++ ){
		if( for_rectilinear[i].size() > 1 ){
			sort( for_rectilinear[i].begin() , for_rectilinear[i].end() , sort_overlap_edge_weight);	 
		}
	}// end of for i 


	if(  debug == true ){
		cout << " In line 1657 " << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 *  要不要寫個 ， debug code 來看看， 東西有沒有 照我的要求排好序 
	 *
	 *  我想到方法 ， 處理size == 3 的case  了 ， 
	 *  只要把edge size 不同的case , 用func 呼叫 來處理 ， 
	 *  這樣 ， 即便我size == 3 裡面有些獨立的case  , 也就 沒關係了 
	 *-----------------------------------------------------------------------------*/

	// 下面這個vector 是我最後一步主要的輸出  
	//vector<edge> obstacle_avoding_rectilinear_Kruskal ;
	for(int i = 0 ; i <= group_count ; i++ ){
			

		/*-----------------------------------------------------------------------------
		 這裡便是retilinear 的核心 ，  看我是能 ， 全部抽像化的去做 rectilinear , 
		 還是 還是要依size 去做rectilinear  , 我記得有 ， 判斷 edge  是水平 or 垂直的func 
		 好了  ， 這裡的rectilinear  , 我要重寫 ， 
		 用一個 我在昨天想到的 ， 新的抽像化方法 ， 只差在
		 我是用 overlap_count 去做分類 ， 因為我有依 overlap_count 去sort 所
		 只要第一元素的overlap_count ==  1  那代表後面也都是 
		 也就是 這種case 的解都是唯一的 ， 
		 剩下的其它case 便要排列組合 ， 抓出其 weight 最小的 來印出來 
		 *-----------------------------------------------------------------------------*/
		if( for_rectilinear[i].size() == 1 ){
			// 怎麼會有edge ， 做了 rectilinear  又把自已給放了進去呢 	
			//  又是gunplot 檔的執行先後的問題 ， 我不太之道是為什麼
			one_edge_rectilinear( for_rectilinear[i][0] , obstacle_avoding_rectilinear_Kruskal );

		}// end of size == 1 的case 
		else if( for_rectilinear[i].size() > 1 ){
			method_by_author( for_rectilinear[i] , obstacle_avoding_rectilinear_Kruskal );
		}// for_rectilinear[i].size() > 1


			
	}// end of for i 

	if(  debug == true ){
		cout << " Done all rectilinear  " << endl ; 
	}

}// end of change_to_rectilinear_by_author


void rectilinear::change_to_rectilinear_by_author( vector<edge> handle , vector<edge> &vec ){

	// 這裡 我要完全 照作者的方法  去寫  , 先排序 ， 之後會照  我method_by_author 這個去改 應該就可以了 
	sort( handle.begin() , handle.end() , sort_overlap_edge_weight);	 


	/*-----------------------------------------------------------------------------
	 *  首先呢 ， 傳進來的 handle 是要 處理成 rectilinear 的 ， 而  我已經對 ， weight 做排序了 
	 *  要二個變數 ， 之道index 指向那二個 ， 以及  二個bool 去看說  ， 現在這二個的情況是 ， 那種case
	 *  也用一個ok變數 ， 直到 handle 內的東西都沒了  都跑出while 
	 *-----------------------------------------------------------------------------*/
	 unsigned int index_a = 0 , index_b = 0 ; 
	 bool  ok = false ; 

	 //  用 個等大小的vector 去記  ， 那些edge  已經做了  rectilinear 
	 vector<bool> already_rectilinear  ; 
	 for(unsigned int i = 0 ; i < handle.size() ; i++ ){
	 	already_rectilinear.push_back( false );	 
	 }// scan all elements on handle 

	// 這裡 要怎麼寫比較好呢  ， 才不會出錯  ， 因為是要配對  ， 找個sharing edge 最大的一對  ， 
	// 但有可能找不到  ， 
	 while( ok == false ){

		bool find_index_a = false , find_index_b = false ; 
		// 先抓 ， sharing edge 最大的那一對  , 上限弄成 減1  是怕  loop j 會抓不到東西
		// 因為每次抓進來的數目都不同 ， 有奇有偶 ， 會不會有人配不對呢 
		for(unsigned int i = 0 ; i < handle.size()  ; i++ ){

			// 直接 指向第一個  
			if( already_rectilinear[i] == false ){
				find_index_a = true ; 
				index_a =  i ; 
				break ; 
			}// end of already_rectilinear[i] == false

		}// scan all elements on handle , what we need is false element , not do rectilinear 

		// 這裡是要找出  ， index_b 是誰   , index_a 最遠指到  ，倒數第二個   ,  如果物件只有二個  ,  這裡會不會出錯呢  
		// 現在改另一種寫法 ， 就是 index_a 不是指向最後一個元素  
		if( find_index_a == true && index_a < handle.size() - 1 ){

			int temp_overlap_value = 0 ; 
			int max_overlap_value = 0 ; 
			// 有可能發生  index_a 沒指到人丫
			for(unsigned int j = index_a + 1 ; j < handle.size() ; j++  ){
			 
				// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
				// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
				// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
				if( already_rectilinear[j] == false  && two_edges_overlap_or_not_by_new_method( handle[index_a] , handle[j] ) == true  ){
					// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		
					temp_overlap_value = cal_overlap_value( handle[index_a] , handle[j] );

					if( temp_overlap_value > max_overlap_value ){
						max_overlap_value = temp_overlap_value ; 	 
						index_b = j ;
						find_index_b = true ; 
					}

				}// end of already_rectilinear[i] == false

			}// end of loop j 

		}// end of if index_a < handle.size() - 1

		if( find_index_a == true && find_index_b == true  ){
		 
			// 再分它的case   ， 有東西 要考慮 ， 就是  可能會找不到  另一個 sharing edge 最大的下手
			// 所以這個 的主軸還要改一下
			bool two_box_near = false , two_box_cross = false ;  
			two_box_near  = two_box_near_or_not(  handle[index_a] , handle[index_b] );
			two_box_cross = two_box_cross_or_not( handle[index_a] , handle[index_b] );

			if( two_box_near == true ){
				rectilinearize_near_case(handle[index_a] , handle[index_b] ,  vec ) ; 
			}
			else if( two_box_cross == true ){
				rectilinearize_cross_case(handle[index_a] , handle[index_b] ,  vec ) ; 
			}// 因為有可能  有  沒配到對的  所以不能放外面
		 
			already_rectilinear[index_a] = true ; 
			already_rectilinear[index_b] = true ; 

		}// 兩個  ， 有找到配對的  
		else if( find_index_a == true  && find_index_b == false ){
			already_rectilinear[index_a] = true ; 	 
			// 這個就當成是  一個edge 的rectilinear 
			one_edge_rectilinear ( handle[index_a] ,  vec  ) ; 
		}// 對不到人 ， 只有自已一個 
		else if(  find_index_a == false ){
			// index_a 找不到  ， 那代表  ， 就可以出去了
			ok  = true  ;  
		}// index_a  就找不到人了 

	 }// end of while  , handle內的edge 都處理完了  才出來 


}// end of change_to_rectilinear_by_author

void rectilinear::change_to_rectilinear_by_teacher( vector<edge> obstacle_avoding_Kruskal , vector<edge> &obstacle_avoding_rectilinear_Kruskal ){
	
	bool debug = false ; 
	if( debug == true  ){
		cout << "Here get in change_to_rectilinear_by_teacher "  << endl ; 
	}
	// 這裡要用老師講的方法  ， 我要 用個DFS的改良去  弄這個東西 ， 跟DFS很像拉 

	// 第一步 ， 應該是 ， 把這些edge會用到的點  ， 都給抓出來 ， 然後 編號  ， 再建個 相鄰矩陣
	// 感覺前置處理 ， 就會花了我很多的時闁 了  
	
	// 這樣一來 ， 就把所有的用到的點 ， 都給放進來了 
	
	/*-----------------------------------------------------------------------------
	 *  
	 *這塊的東西 ， 在這個方法裡面 ， 好像用不到 
	vector<vertex> all_vertex ;
	int z = obstacle_avoding_Kruskal[0].print_node1_z()  ; 
	for( unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){
		all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node1_x()  , obstacle_avoding_Kruskal[i].print_node1_y() , z ) ); 
		all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node2_x()  , obstacle_avoding_Kruskal[i].print_node2_y() , z ) ); 
	}// scan all edge 抓出裡面有用到的點 


	// 接下來呢  ， 麻煩的是 ， 點會有重複 ， 所以要刪掉 ， 每次都要做這些前置 ， 處理 ， 時間真的會比較快嗎 
	for( unsigned int i = 0 ; i < all_vertex.size() ; i++){

		for( unsigned int j = i + 1 ; all_vertex.size() ; j++ ){

			if( all_vertex[i].print_x_position() == all_vertex[j].print_x_position() &&
			    all_vertex[i].print_y_position() == all_vertex[j].print_y_position() ){
				all_vertex.erase( all_vertex.begin() + j ) ; 
				// 之所以要減1 ， 是因為會有個元素往前跑  ， 所以 不能忘了要檢查它 
				j-- ;
			}

		}// loop j 

	}// 要把  相同的點 ， 給刪掉  ，  loop i 
	 *-----------------------------------------------------------------------------*/

	if( debug == true  ){
		cout << "Here get in change_to_rectilinear_by_teacher , Line 1823"  << endl ; 
	}

	//  哇 ， 一排序 時間就輸人家了  
	sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight );	 
	// 這裡的話  ， 我要以edge為單位來寫 我想會簡單一點  , 我打算 排序 ， 由大的edge 先做 
	// 我覺得 我的之所以錯 是因為  ， 短的先做了 ， 要不就是  ， 相鄰的其它edge都給rect了
	// 那自已就要跟已 rectilinear 好的edge 去比 
	vector<int> adj_list[ obstacle_avoding_Kruskal.size()  ] ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){
		 
		 for( unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++ ){

			bool neighbor_or_not = false ; 
			// 這裡抓的相鄰  ， 是指  ， 有點相鄰且  有overlap 也就是 near 及 cross case 
			// 我忘了， 應該有配不到對的人 , 換一個method來看看  
			neighbor_or_not = two_edges_overlap_or_not( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) ; 
			if( neighbor_or_not == true  ){
				// 我好像只放了一邊 ， 忘了放另一邊的 , 
				// 奇怪  ， 改了  錯也不是在這丫 
				adj_list[i].push_back( j ) ;  
				adj_list[j].push_back( i ) ;  
			}

		 }// scan all vertex


	}// 把 adj_lsit 給補全了 

	if( debug == true  ){
		cout << "Here get in change_to_rectilinear_by_teacher , Line 1823"  << endl ; 
	}

	vector<bool> visit ; 
	for( unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++  ){
		visit.push_back( false ) ; 
	}// 初始化 


	/*-----------------------------------------------------------------------------
	 *    好像用不到這 map 哎 
	// 建好map , index 跟 edge 的map
	map<int,edge> edge_map ; 
	for( unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){
		edge_map[i] = obstacle_avoding_Kruskal[i] ; 
	}// 

	map<int,edge>::iterator edge_it_one ;  
	map<int,edge>::iterator edge_it_two ;  
	 *-----------------------------------------------------------------------------*/


	if( debug == true  ){
		cout << "Here before while loop  , Line 1880 "  << endl ; 
		// cout << "my_stack.size() =  " << my_stack.size()  << endl ; 
	}

	// 來看看  只有孤家寡人一個的  ， 沒有跟其它有overlap的edge 會不會也出錯 
	// 好像也不必用到while  , 直接用loop 也是可以的樣子 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){
	 
		if( visit[i] == false && adj_list[ i ].size() == 0 ){
			// 這個是  ， 有共用點 ， 但卻沒有落在  case 2 , 3 所以 只直把它給rectilinear 化就好了 	 
			// 這個沒給true  好像也沒什麼關係 ， 因為 其它人也不會跟它有相鄰丫 
			one_edge_rectilinear( obstacle_avoding_Kruskal[ i ] , obstacle_avoding_rectilinear_Kruskal ) ; 
			visit[i] = true ; 
		}
		else if( visit[i] == false && adj_list[ i ].size() > 0 )
		{
				
			// 由stack top 出來的值  去看其相鄰的edge有誰  ， 抓出  誰是  overlap 最大的 
			// 再做 rectilinear  
			// edge_it_one = edge_map.find( i ) ;  , 其怪了 ， 我覺得不用  用map丫
			// 我只要從adj抓出編號  ， 再到 obstacle_avoding_Kruskal 抓edge 不就好了  
			int map_key_two = 0 , max_overlap_index = 0 , max_overlap_value = -1 ; 
			bool find_another = false ; 
			for( unsigned int j = 0 ; j < adj_list[ i ].size() ; j++ ){

				// 要注意一個地方 ， 就是 edge_it_two 只是個iterator ， 要多加個pointer 讓它指下去才行
				// 還有個  是你iterator指的東西 是個  裝了二個data的東西 ， 所以 後面要加 .second 或 first 看是去抓
				// 前面的index 還是後面的 那個物件 
				map_key_two = adj_list[ i ][j] ;  

				// 告夭 ， 好像是 錯 ， 這裡 ， 比的那個要  visit == false   , 差點忘了  ， 只記得放進去的是要 visit == false , 
				// 都忘了  抓的那個也要是false 
				if(  visit[ map_key_two ] == false &&
				cal_overlap_value( obstacle_avoding_Kruskal[ i ] , obstacle_avoding_Kruskal[ map_key_two ] )  > 
								max_overlap_value  							){
					max_overlap_value =  cal_overlap_value( obstacle_avoding_Kruskal[ i ] , 
										obstacle_avoding_Kruskal[ map_key_two ] ) ; 
					//   max_overlap_index = j ;  原本是給左邊的這個樣子  ， 好像是這裡給錯了  
					//   應該是要給下面的這個樣子才是對的 
					max_overlap_index = map_key_two  ; 
					find_another = true ; 
				}

			}// scan 所有   , 所有的相鄰的edge 抓出一個overlap最大的那個  

			// 上面抓完了   ，跟其相鄰的edge 中  ， 有最大overlap 的那個edge  , 以及他的index  ， 所以  我下面要處理的東西 ， 便是 
			// top 所指向的edge 以及  map_key_two  所指向的 edge  , 裡應是錯這 ， 因為  有的edge給  ， rectilinear到 原點的0 , 0 , 0 
			// 但那種情況只有  ， 找不到 共用點才會的吧  , 喔  ， 除了 明顯的 ， 它都沒有相鄰 則是  是one_edge_rectilinear , 
			// 但有另一種情況 就 它的相鄰的那個給別人用了  , 所以它自已還是 one_edge_rectilinear
			if( find_another == true  ){
			 

				// 看來是卡在這一個喔 
				if( two_box_cross_or_not(  obstacle_avoding_Kruskal[ i ] , obstacle_avoding_Kruskal[ max_overlap_index ] ) == true ){
					rectilinearize_cross_case( obstacle_avoding_Kruskal[ i ] , obstacle_avoding_Kruskal[ max_overlap_index ] 
								 , obstacle_avoding_rectilinear_Kruskal ) ;  
				}
				else if( two_box_near_or_not( obstacle_avoding_Kruskal[ i ] , obstacle_avoding_Kruskal[ max_overlap_index ] ) == true ){
					rectilinearize_near_case(  obstacle_avoding_Kruskal[ i ] , obstacle_avoding_Kruskal[ max_overlap_index ] 
								 , obstacle_avoding_rectilinear_Kruskal ) ;  
				}
				// 做完  ， 要從visit 內把這二個給做了  ， true  代表來 過了 	
				visit[ max_overlap_index ] = true ; 

			}// 有找到另一半
			else{
				one_edge_rectilinear( obstacle_avoding_Kruskal[ i ] , obstacle_avoding_rectilinear_Kruskal ) ; 
			}// 只有自已一個 ， 沒找到另一半

			visit[ i ] = true ; 


		}// end of 相鄰而且有配對到 

	}// scan all obstacle_avoding_Kruskal.edge

	/*-----------------------------------------------------------------------------
	 *   , 先把這塊 while拿掉  看是那裡錯了 
	// 告夭  ， 我條件設成  == true  也就是為 空才能進去  ， 真白痴 
	// 先建好stack  , 給 0 就是從 ， edge編號為0 的開始trace 
	stack<int> my_stack ; 
	my_stack.push( 0 ) ; 
	while( my_stack.empty() == false ){

		if( debug == true  ){
			cout << "Here get in while loop   , Line 1890 "  << endl ; 
		}
		
		int map_key_one = my_stack.top() ; 
		if( adj_list[ map_key_one ].size() == 0 ){
			// 這個是  ， 有共用點 ， 但卻沒有落在  case 2 , 3 所以 只直把它給rectilinear 化就好了 	 
			one_edge_rectilinear( obstacle_avoding_Kruskal[ map_key_one ] , obstacle_avoding_rectilinear_Kruskal ) ; 
			visit[ map_key_one ] = true ; 
		}
		else if( adj_list[ map_key_one ].size() > 0)
		{
				
			// 由stack top 出來的值  去看其相鄰的edge有誰  ， 抓出  誰是  overlap 最大的 
			// 再做 rectilinear  
			// edge_it_one = edge_map.find( map_key_one ) ;  , 其怪了 ， 我覺得不用  用map丫
			// 我只要從adj抓出編號  ， 再到 obstacle_avoding_Kruskal 抓edge 不就好了  
			int map_key_two = 0 , max_overlap_index = 0 , max_overlap_value = -1 ; 

			for( unsigned int i = 0 ; i < adj_list[ map_key_one ].size() ; i++ ){

				// 要注意一個地方 ， 就是 edge_it_two 只是個iterator ， 要多加個pointer 讓它指下去才行
				// 還有個  是你iterator指的東西 是個  裝了二個data的東西 ， 所以 後面要加 .second 或 first 看是去抓
				// 前面的index 還是後面的 那個物件 
				map_key_two = adj_list[ map_key_one ][i] ;  

				// 告夭 ， 好像是 錯 ， 這裡 ， 比的那個要  visit == false   , 差點忘了  ， 只記得放進去的是要 visit == false , 
				// 都忘了  抓的那個也要是false 
				if(  visit[ map_key_two ] == false &&
				cal_overlap_value( obstacle_avoding_Kruskal[ map_key_one ] , obstacle_avoding_Kruskal[ map_key_two ] )  > 
								max_overlap_value  							){
					max_overlap_value =  cal_overlap_value( obstacle_avoding_Kruskal[ map_key_one ] , 
										obstacle_avoding_Kruskal[ map_key_two ] ) ; 
					max_overlap_index = i ; 
				}

			}// scan 所有   , 所有的相鄰的edge 抓出一個overlap最大的那個  

			// 上面抓完了   ，跟其相鄰的edge 中  ， 有最大overlap 的那個edge  , 以及他的index  ， 所以  我下面要處理的東西 ， 便是 
			// top 所指向的edge 以及  map_key_two  所指向的 edge  , 裡應是錯這 ， 因為  有的edge給  ， rectilinear到 原點的0 , 0 , 0 
			// 但那種情況只有  ， 找不到 共用點才會的吧 
			if( two_box_near_or_not( obstacle_avoding_Kruskal[ map_key_one ] , obstacle_avoding_Kruskal[ max_overlap_index ] ) == true ){
				rectilinearize_near_case(  obstacle_avoding_Kruskal[ map_key_one ] , obstacle_avoding_Kruskal[ max_overlap_index ] 
							 , obstacle_avoding_rectilinear_Kruskal ) ;  
			}
			else if( two_box_cross_or_not(  obstacle_avoding_Kruskal[ map_key_one ] , obstacle_avoding_Kruskal[ max_overlap_index ] ) == true ){
				rectilinearize_cross_case( obstacle_avoding_Kruskal[ map_key_one ] , obstacle_avoding_Kruskal[ max_overlap_index ] 
							 , obstacle_avoding_rectilinear_Kruskal ) ;  
			}

			// 做完  ， 要從visit 內把這二個給做了  ， true  代表來 過了 	
			visit[ map_key_one ] = true ; 
			visit[ max_overlap_index ] = true ; 


		}// end of 相鄰而且有配對到 
	
		// 接下來  要把 ， 那my_stack 內的那個 拿出來 ， 再從  ， visit 內 丟一個 ， 為 false 的edge 編號 放下去 
		my_stack.pop() ; 
		for( unsigned int i = 0 ; i < visit.size() ; i++ ){
			if( visit[i] == false  ){
				// 因 為 visit 的 index  位置  ， 就是它的編號  ， 所以把它給放進去就了  
				// 要給個break  , 免的一直比下去  ， 比到第一個就可以了
				my_stack.push( i ) ;  
				break ; 
			}
		}// 看vsit 內  有誰 是false 的 ， 把它的編號放進去

	}// 直到 stack 內為空  才出去這個loop 
	 *-----------------------------------------------------------------------------*/


	/*-----------------------------------------------------------------------------
	 *  
	 *  這裡原本， 以點為單位的寫法  
	// 接下來  ， 就是建相鄰矩陣了    , 用map  的方式去 建立  ， 我想會比較快   , 之後再改成 map好了  ，先把東西給寫出來 
	// adj_list 用來存  抓 vertex 的key 好了  , 從這裡 開始改吧  
	vector<vertex> adj_list[ all_vertex.size() ] ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){
		 
		 bool node1_find = false , node2_find = false ; 
		 int  node1_index = 0 , node2_index = 0 ; 
		 for( unsigned int j = 0 ; j < all_vertex.size() ; j++ ){
		 	// node1 比對到了  ， 那就把 它對面的node2 放進來 到  adj 內 
			if( obstacle_avoding_Kruskal[i].print_node1_x() == all_vertex[j].print_x_position() &&
			    obstacle_avoding_Kruskal[i].print_node1_y() == all_vertex[j].print_y_position() ){
			    node1_find = true ; 
			    node1_index = j ; 
			}
		 	// node1 比對到了  ， 那就把 它對面的node2 放進來 到  adj 內 
			if( obstacle_avoding_Kruskal[i].print_node2_x() == all_vertex[j].print_x_position() &&
			    obstacle_avoding_Kruskal[i].print_node2_y() == all_vertex[j].print_y_position() ){
			    node2_find = true ; 
			    node2_index = j ; 
			}
		 }// scan all vertex

		if( node1_find == true && node2_find == true ){
			adj_list[node1_index].push_back( node2_index )   ; 
			adj_list[node2_index].push_back( node1_index )   ; 
		}// 
	}// 把 adj_lsit 給補全了 
		
	// 接下來 ， 我要建每個點的相鄰 矩陣  ， 那點的編號 就依照 ， 它在all_vertex 中的 ， index 
	bool visit[all_vertex.size()] ; 
	for( unsigned int i = 0 ; i < all_vertex.size() ; i++  ){
		visit[i] = false ; 
	}// 初始化 

	// 建個 map 好了  , 
	map<int,vertex> vertex_map ; 
	for( unsigned int i = 0 ; i < all_vertex.size() ; i++ ){
		vertex_map[i] = all_vertex[i] ; 
	}// 

	//bound_it = obstacle_map.find( bound_key ) ;  ,  給個列子  以免忘了 
	map<int,vertex>::iterator vertex_it ;  

	// 建好  adj_list了  ， 接下來 ， 要用到 stack 及 DFS 去實作東西了   , 由 all_vertex  把點給放進去 
	// stack 內  ， 由它開始  ，  先把 第一個 東西給放進去  ， 之後用一個while 來看 stack 是否為空 ， 要不要
	// 跳出去  , 我在想  要不要把  all_vertex 建個 map  , 這樣我也才  好取用 
	stack<int> my_stack ; 
	// 放0 是要從編號0的 ， vertex 開始trace 
	my_stack.push_back( 0 ) ; 
	while( my_stack.empty() == true ){
	
		// 先不用 ， 我第一步 ， 應該是先要抓edge  , 每個點 ， 應該都至少有一個 相鄰的點才對 
		// 我抓第一個點 ， 來當初始的edge 來往下做 

	}// 直到 stack 內為空  才出去這個loop 
	 *-----------------------------------------------------------------------------*/

}// end of change_to_rectilinear_by_author

void rectilinear::obstacle_avoding_rectilinear_Kruskal_gnu(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , level_info &my_level_info ) {
	
	ofstream OARKcout("obstacle_avoding_rectilinear_Kruskal.gnu");
	OARKcout << " set title \"Obstacle_Avoding_Rectilinear_Kruskal\" " << endl ; 
	OARKcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	OARKcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	OARKcout << "set xlabel \"X-axis\"" << endl ; 
	OARKcout << "set ylabel \"Y-axis\"" << endl ; 
	/*-----------------------------------------------------------------------------
	OARKcout << "set output " << endl ; 
	OARKcout << "set terminal X11 " << endl ; 
	OARKcout << "set terminal postscript color" << endl ; 
	OARKcout << "set output \"Obstacle_Avoding_Rectilinear_Kruskal.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/
	OARKcout << "set output " << endl ; 
	OARKcout << "set terminal X11 " << endl ; 
	OARKcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 

	OARKcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( !( my_level_info.array_vertex[i].Is_pin_or_not() )    )
		{
			// obstacle 的資訊 , 是 要給二個點 才能畫  分別是1跟3 
			if( my_level_info.array_vertex[i].print_corner_id() == 1){
				OARKcout << "set object "<< my_level_info.array_vertex[i].print_obstacle_id() <<" rect from " ; 
				my_level_info.array_vertex[i].print_obstacle_for_script( OARKcout )  ; // 這是要輸出點1的資訊的
				OARKcout << "to " ;	
			}
			else if( my_level_info.array_vertex[i].print_corner_id() == 3){
				my_level_info.array_vertex[i].print_obstacle_for_script( OARKcout ) ;// 這是點3 
				OARKcout << endl ; 
			}

		}
	}
	//那這裡要印出　OASG 內的所有line 
	OARKcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < obstacle_avoding_rectilinear_Kruskal.size() ; i++){
		// i + 1 只是obstacle 的編號而己 
		obstacle_avoding_rectilinear_Kruskal[i].print_edge_information_for_gnu( OARKcout , i + 1 );	}
	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	OARKcout << "plot \"PlaneData.txt\" w lp pt 7 lc rgb \"black\" " << endl ; 
	OARKcout << "pause -1 " << endl ;
	OARKcout.close() ;

}// end of obstacle_avoding_rectilinear_Kruskal_gnu

void rectilinear::overlap_obstacle_avoding_rectilinear_Kruskal_gnu(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , level_info &my_level_info ) {
	
	ofstream OARKcout("obstacle_avoding_rectilinear_Kruskal.gnu");
	OARKcout << " set title \"Obstacle_Avoding_Rectilinear_Kruskal\" " << endl ; 
	OARKcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	OARKcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	OARKcout << "set xlabel \"X-axis\"" << endl ; 
	OARKcout << "set ylabel \"Y-axis\"" << endl ; 
	/*-----------------------------------------------------------------------------
	OARKcout << "set output " << endl ; 
	OARKcout << "set terminal X11 " << endl ; 
	OARKcout << "set terminal postscript color" << endl ; 
	OARKcout << "set output \"Obstacle_Avoding_Rectilinear_Kruskal.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/
	OARKcout << "set output " << endl ; 
	OARKcout << "set terminal X11 " << endl ; 
	OARKcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 

	OARKcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( !( my_level_info.array_vertex[i].Is_pin_or_not() )    )
		{
			// obstacle 的資訊 , 是 要給二個點 才能畫  分別是1跟3 
			if( my_level_info.array_vertex[i].print_corner_id() == 1){
				OARKcout << "set object "<< my_level_info.array_vertex[i].print_obstacle_id() <<" rect from " ; 
				my_level_info.array_vertex[i].print_obstacle_for_script( OARKcout )  ; // 這是要輸出點1的資訊的
				OARKcout << "to " ;	
			}
			else if( my_level_info.array_vertex[i].print_corner_id() == 3){
				my_level_info.array_vertex[i].print_obstacle_for_script( OARKcout ) ;// 這是點3 
				OARKcout << endl ; 
			}

		}
	}
	//那這裡要印出　OASG 內的所有line 
	OARKcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < obstacle_avoding_rectilinear_Kruskal.size() ; i++){
		// i + 1 只是obstacle 的編號而己 
		obstacle_avoding_rectilinear_Kruskal[i].print_edge_information_for_gnu( OARKcout , i + 1 );	}
	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	OARKcout << "plot \"Overlap_PlaneData.txt\" w lp pt 7 lc rgb \"black\" , \"Overlap_Vertex.txt\" w points  pt 7  lc rgb  \"green\" " << endl ; 
	OARKcout << "pause -1 " << endl ;
	OARKcout.close() ;

}// end of obstacle_avoding_rectilinear_Kruskal_gnu


void rectilinear::overlap_obstacle_avoding_rectilinear_Kruskal_gnu_trans_eps(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , level_info &my_level_info ) {
	
	ofstream OARKcout("obstacle_avoding_rectilinear_Kruskal_eps.gnu");
	OARKcout << " set title \"Revised Obstacle_Avoding Rectilinear Spanning Forest with rectilinear obstacles\" " << endl ; 
	OARKcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	OARKcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	OARKcout << "set xlabel \"X-axis\"" << endl ; 
	OARKcout << "set ylabel \"Y-axis\"" << endl ; 
	/*-----------------------------------------------------------------------------
	OARKcout << "set output " << endl ; 
	OARKcout << "set terminal X11 " << endl ; 
	OARKcout << "set terminal postscript color" << endl ; 
	OARKcout << "set output \"Obstacle_Avoding_Rectilinear_Kruskal.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/
	OARKcout << "set terminal postscript color" << endl ; 
	OARKcout << "set output \"Revised Obstacle_Avoding Rectilinear Spanning Forest with rectilinear obstacles.eps\"" << endl ; 
	OARKcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 

	OARKcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( !( my_level_info.array_vertex[i].Is_pin_or_not() )    )
		{
			// obstacle 的資訊 , 是 要給二個點 才能畫  分別是1跟3 
			if( my_level_info.array_vertex[i].print_corner_id() == 1){
				OARKcout << "set object "<< my_level_info.array_vertex[i].print_obstacle_id() <<" rect from " ; 
				my_level_info.array_vertex[i].print_obstacle_for_script( OARKcout )  ; // 這是要輸出點1的資訊的
				OARKcout << "to " ;	
			}
			else if( my_level_info.array_vertex[i].print_corner_id() == 3){
				my_level_info.array_vertex[i].print_obstacle_for_script( OARKcout ) ;// 這是點3 
				OARKcout << endl ; 
			}

		}
	}
	//那這裡要印出　OASG 內的所有line 
	OARKcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < obstacle_avoding_rectilinear_Kruskal.size() ; i++){
		// i + 1 只是obstacle 的編號而己 
		obstacle_avoding_rectilinear_Kruskal[i].print_edge_information_for_gnu( OARKcout , i + 1 );	}
	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	OARKcout << "plot \"Overlap_PlaneData.txt\" w lp pt 7 lc rgb \"black\" " << endl ; 
	// OARKcout << "pause -1 " << endl ;
	OARKcout.close() ;

}// end of obstacle_avoding_rectilinear_Kruskal_gnu


/*-----------------------------------------------------------------------------
 *  3D gnu 檔
 *-----------------------------------------------------------------------------*/
void rectilinear::ThreeD_obstacle_avoding_rectilinear_Kruskal_gnu(  vector<edge> &obstacle_avoding_rectilinear_Kruskal , vector<level_info> &every_level_info ){

	ofstream SGcout("3D_obstacle_avoding_rectilinear_kruskal_tree.gnu");
	SGcout << " set title \"3D_obstacle_avoding_rectilinear_kruskal_tree\" " << endl ; 
	SGcout << " set xrange[0:" << every_level_info[0].plane_width - 1 <<  "]" << endl ; 
	SGcout << " set yrange[0:" << every_level_info[0].plane_height - 1 <<  "]" << endl ; 
	SGcout << " set zrange[0:" << every_level_info.size() <<  "]" << endl ; 
	SGcout << "set xlabel \"X-axis\"" << endl ; 
	SGcout << "set ylabel \"Y-axis\"" << endl ; 
	SGcout << "set zlabel \"Z-axis\"" << endl ; 
	SGcout << "set output " << endl ; 
	SGcout << "set terminal X11 " << endl ; 
	/*-----------------------------------------------------------------------------
	SGcout << "set terminal postscript color" << endl ; 
	SGcout << "set output \"3D_Kruskal_Tree.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/

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
			
			every_level_info[i].complete_obstacle_information[j].print_3D_obstacle(SGcout ,  every_level_info[i].which_level ,arrow_count) ; 	

		}// end of print out all obstacle  variable j 

	}// end of get every_level_info to print out  variable i 

	
	//那這裡要印出　OASG 內的所有line 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < obstacle_avoding_rectilinear_Kruskal.size() ; i++){
		// 我想這裡 也要改了  ， 下面 這個 func 原是用來處理 2D 的 
		// edge.h 已經改過了 ， 所以裡面是 3D 的 position 
		obstacle_avoding_rectilinear_Kruskal[i].print_3D_edge_information_for_gnu( SGcout , arrow_count + i + 1 );// i + 1 只是obstacle 的編號而己 
	}


	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	// SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\"  " << endl ; 
	//SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;
}// end of ThreeD_obstacle_avoding_rectilinear_Kruskal_gnu

#endif
