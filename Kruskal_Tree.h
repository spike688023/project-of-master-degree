/*
 * =====================================================================================
 *
 *       Filename:  kruskal_tree.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/02/09 11時37分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef        KRUSKAL_TREE_H 
	#define  KRUSKAL_TREE_H  	
#include	<iostream>
#include	<fstream>
#include	"edge.h"
#include	"vertex.h"
#include	"level_info.h"
#include	<vector>
#include        <cstdio>
using namespace std ; 

 class kruskal_tree{

 private:

 public:

	 /*-----------------------------------------------------------------------------
	  *  雖然名字一樣 ， 但是  , 第二個引數放的東西不一樣 
	  *  應該是不會呼叫錯才對丫 , 那我建kruskal_tree 是錯在那裡呢 
	  *-----------------------------------------------------------------------------*/
	void build_2D_kruskal_tree( vector<edge> &SG_edge , level_info &my_level_info );

	// 下面這個  是有在 3D 下才 要用的
	void build_2D_kruskal_tree_overlap_version( vector<edge> &SG_edge , level_info &my_level_info );

	// 這個的話 ， 則是  要分群  
	void build_2D_overlap_kruskal_tree(  vector<edge> &SG_edge , level_info &my_level_info , vector<vertex> &list_SG_vertex ) ; 
   
	void print_2D_kruskal_tree_gun( vector<edge> &SG_edge ,  level_info &my_level_info ) ; 

	void print_2D_overlap_kruskal_tree_gun( vector<edge> &SG_edge , level_info &my_level_info  ); 

	// 我想  用來處理 3D 的func ， 藉由第二個引數的不同 ， 來決定是處理2D 還是 3D 
	void build_3D_kruskal_tree( vector<edge> &SG_edge , vector<level_info> &every_level_info );
   
	void print_3D_kruskal_tree_gun( vector<edge> &SG_edge ,  vector<level_info> &every_level_info ) ; 

	// 有overlap 的3D 看來還是要再改一下
	void build_3D_overlap_kruskal_tree( vector<edge> &SG_edge , vector<level_info> &every_level_info );
};

bool sort_weight( edge e1 , edge e2 ){
	return ( e1.print_weight() < e2.print_weight() );
}


bool sort_3D_weight( edge e1 , edge e2 ){
	return ( e1.print_3D_weight() < e2.print_3D_weight() );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  build_2D_kruskal_tree_detail
 *  Description:   這個東西 ， 我好像把它改成可以弄2,3D , 但之後發現， 改成3D ， 
 *  		  回去弄2D , 好像有出錯 
 * =====================================================================================
 */
	void
build_2D_kruskal_tree_detail (   vector<edge> &Complete , vector<edge> &SG_edge , unsigned int total_pin_nu )
{
	
	int subtree_count = 0 ;
	if( Complete.size() > 0 ){
		SG_edge.push_back( Complete[0] );
		subtree_count++ ; 
		// 我實在是 有點忘了 ， 這裡應該是要把 每個 edge 都給個 id 
		// 但 index 怎麼都會是放 0 呢  , 喔 我想起來了 
		// Complete 是全連通的圖 ， 我要從裡面 慢慢的選 ， 選出一些
		// 是kruskal_tree 要的edge 放到 SG_edge 中 ， 而index 之所以
		// 放 0 是因為其 weight 最小 一定會被放進來的 
		SG_edge[0].set_subtree_id( subtree_count );
	}

	for(unsigned int i = 1  ; i < Complete.size() ; i++ ){
		//下面的使用 , 如同 土哥講的一樣  , 指標跟 [] 是等價的 , 
		//cout << SG_vertex[i].size() << endl;     , 如果要傳二維的東西過來  這裡的SG_vertex 要寫 vector<vertex> *SG_vertex  
		int node1_subtree_id = 0  , node2_subtree_id = 0   ; 
		// 這裡有新的想法 , 我把subtree_id 放在edge上 , 先放一個edge到SG_edge 且subtree_id = 1 , 
		// 之後抓的每個edge 內的二個點 , 都跟 SG_vertex 內的點比對 , 如果有點相等 , 
		// 就把subtree_id 給其  node1 , node2 , 兩個id都是0 就直接加進去 ,且把subtree_count+1 , 
		// 所以有三種case要考慮 , 二個id都是0 , 一個0一個非零的正整數 , 二個都為非零的正整數 

		//抓出 這二個node跟SG_edge內 , 有點相等的點 , 沒有的話 , 0 就代表沒有
		for( unsigned int j = 0  ; j < SG_edge.size() ; j++ ){
			// 先抓出node1是歸屬那個subtree 
 			if( Complete[i].print_node1_x() == SG_edge[j].print_node1_x() &&
			    Complete[i].print_node1_y() == SG_edge[j].print_node1_y() ){
 				node1_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
			else if( Complete[i].print_node1_x() == SG_edge[j].print_node2_x() &&
				 Complete[i].print_node1_y() == SG_edge[j].print_node2_y() ){
 				node1_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
			// node2 
			if( Complete[i].print_node2_x() == SG_edge[j].print_node1_x() &&
			    Complete[i].print_node2_y() == SG_edge[j].print_node1_y() ){
 				node2_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
			else if( Complete[i].print_node2_x() == SG_edge[j].print_node2_x() && 
				 Complete[i].print_node2_y() == SG_edge[j].print_node2_y() ){
 				node2_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
		}
	  	
		// 下面這塊 應該 沒什麼地方要改吧 
		// 上面的for 抓出了 , edge 二端點的subtree 歸屬 , 接下來便是 , 針對不同的歸屬 , 做出不同的動作
		// 剛突然想說  可以用反面的思考來寫 , 就不用想的那麼細了 , 但是 沒能記到一些 我要的資訊  來判斷東西 , 所以不行 
		//  Case 1 :  兩個值皆為 0  , 代表此edge 可以直接納入 , 記得要對edge_count++ 
		if( node1_subtree_id == 0 && node2_subtree_id == 0 ){
			//  因為我丟的是edge 的物件　，　所以我要設定一個edge copy constructor , 
 			SG_edge.push_back( Complete[i] );
			subtree_count++ ; 
			SG_edge[ SG_edge.size() - 1 ].set_subtree_id( subtree_count );
		}
		// Case 2 : 只有一個是 0 , 另一個是非零
		if( (node1_subtree_id == 0 && node2_subtree_id != 0) || (node1_subtree_id != 0 && node2_subtree_id == 0) ){
			if( node2_subtree_id != 0 ){
 				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node2_subtree_id );
			} 
			else if( node1_subtree_id != 0 ){
 				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node1_subtree_id );
			}
		}
		// Case 3 : 二個都是非零 , 這是最麻煩的case  , 我要把二顆樹的id合併 , 用最小的那個取代 
		if(node1_subtree_id != 0 && node2_subtree_id != 0 && node1_subtree_id != node2_subtree_id ){
			// 比大小是因為  , 我要用小的來當作整顆樹的id 
			if( node1_subtree_id > node2_subtree_id ){
				// 先把edge加入 , 再用個for , 把所有id大的edge , 都變成id小的那個   
 				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node2_subtree_id );
				for( unsigned int j = 0  ; j < SG_edge.size() ; j++){
 					if( SG_edge[j].return_subtree_id() == node1_subtree_id ){
 						SG_edge[j].set_subtree_id( node2_subtree_id );
					}
				}
			} 
			else if( node1_subtree_id < node2_subtree_id ){
				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node1_subtree_id );
				for( unsigned int j = 0  ; j < SG_edge.size() ; j++){
 					if( SG_edge[j].return_subtree_id() == node2_subtree_id ){
 						SG_edge[j].set_subtree_id( node1_subtree_id );
					}
				} 
			}
		}
		//抓到足夠的edge , 後面不用再看了
		if( SG_edge.size() == total_pin_nu - 1 ){
 			break ; 
		}
	}// scan all edge in Complete , increasing 

}		/* -----  end of function build_kruskal_tree_detail  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  build_3D_kruskal_tree_detail
 *  Description:   這個東西 ， 我好像把它改成可以弄2,3D , 但之後發現， 改成3D ， 
 *  		  回去弄2D , 好像有出錯 
 * =====================================================================================
 */
	void
build_3D_kruskal_tree_detail (   vector<edge> &Complete , vector<edge> &SG_edge , unsigned int total_pin_nu )
{
	
	int subtree_count = 0 ;
	if( Complete.size() > 0 ){
		SG_edge.push_back( Complete[0] );
		subtree_count++ ; 
		// 我實在是 有點忘了 ， 這裡應該是要把 每個 edge 都給個 id 
		// 但 index 怎麼都會是放 0 呢  , 喔 我想起來了 
		// Complete 是全連通的圖 ， 我要從裡面 慢慢的選 ， 選出一些
		// 是kruskal_tree 要的edge 放到 SG_edge 中 ， 而index 之所以
		// 放 0 是因為其 weight 最小 一定會被放進來的 
		SG_edge[0].set_subtree_id( subtree_count );
	}

	for(unsigned int i = 1  ; i < Complete.size() ; i++ ){
		//下面的使用 , 如同 土哥講的一樣  , 指標跟 [] 是等價的 , 
		//cout << SG_vertex[i].size() << endl;     , 如果要傳二維的東西過來  這裡的SG_vertex 要寫 vector<vertex> *SG_vertex  
		int node1_subtree_id = 0  , node2_subtree_id = 0   ; 
		// 這裡有新的想法 , 我把subtree_id 放在edge上 , 先放一個edge到SG_edge 且subtree_id = 1 , 
		// 之後抓的每個edge 內的二個點 , 都跟 SG_vertex 內的點比對 , 如果有點相等 , 
		// 就把subtree_id 給其  node1 , node2 , 兩個id都是0 就直接加進去 ,且把subtree_count+1 , 
		// 所以有三種case要考慮 , 二個id都是0 , 一個0一個非零的正整數 , 二個都為非零的正整數 

		//抓出 這二個node跟SG_edge內 , 有點相等的點 , 沒有的話 , 0 就代表沒有
		for( unsigned int j = 0  ; j < SG_edge.size() ; j++ ){
			// 先抓出node1是歸屬那個subtree 
 			if( Complete[i].print_node1_x() == SG_edge[j].print_node1_x() &&
			    Complete[i].print_node1_y() == SG_edge[j].print_node1_y() && 
			    Complete[i].print_node1_z() == SG_edge[j].print_node1_z() ){
 				node1_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
			else if( Complete[i].print_node1_x() == SG_edge[j].print_node2_x() &&
				 Complete[i].print_node1_y() == SG_edge[j].print_node2_y() &&
				 Complete[i].print_node1_z() == SG_edge[j].print_node2_z() ){
 				node1_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
			// node2 
			if( Complete[i].print_node2_x() == SG_edge[j].print_node1_x() &&
			    Complete[i].print_node2_y() == SG_edge[j].print_node1_y() &&
			    Complete[i].print_node2_z() == SG_edge[j].print_node1_z() ){
 				node2_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
			else if( Complete[i].print_node2_x() == SG_edge[j].print_node2_x() && 
				 Complete[i].print_node2_y() == SG_edge[j].print_node2_y() &&
				 Complete[i].print_node2_z() == SG_edge[j].print_node2_z() ){
 				node2_subtree_id = SG_edge[j].return_subtree_id() ; 
			}
		}
	  	
		// 下面這塊 應該 沒什麼地方要改吧 
		// 上面的for 抓出了 , edge 二端點的subtree 歸屬 , 接下來便是 , 針對不同的歸屬 , 做出不同的動作
		// 剛突然想說  可以用反面的思考來寫 , 就不用想的那麼細了 , 但是 沒能記到一些 我要的資訊  來判斷東西 , 所以不行 
		//  Case 1 :  兩個值皆為 0  , 代表此edge 可以直接納入 , 記得要對edge_count++ 
		if( node1_subtree_id == 0 && node2_subtree_id == 0 ){
			//  因為我丟的是edge 的物件　，　所以我要設定一個edge copy constructor , 
 			SG_edge.push_back( Complete[i] );
			subtree_count++ ; 
			SG_edge[ SG_edge.size() - 1 ].set_subtree_id( subtree_count );
		}
		// Case 2 : 只有一個是 0 , 另一個是非零
		if( (node1_subtree_id == 0 && node2_subtree_id != 0) || (node1_subtree_id != 0 && node2_subtree_id == 0) ){
			if( node2_subtree_id != 0 ){
 				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node2_subtree_id );
			} 
			else if( node1_subtree_id != 0 ){
 				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node1_subtree_id );
			}
		}
		// Case 3 : 二個都是非零 , 這是最麻煩的case  , 我要把二顆樹的id合併 , 用最小的那個取代 
		if(node1_subtree_id != 0 && node2_subtree_id != 0 && node1_subtree_id != node2_subtree_id ){
			// 比大小是因為  , 我要用小的來當作整顆樹的id 
			if( node1_subtree_id > node2_subtree_id ){
				// 先把edge加入 , 再用個for , 把所有id大的edge , 都變成id小的那個   
 				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node2_subtree_id );
				for( unsigned int j = 0  ; j < SG_edge.size() ; j++){
 					if( SG_edge[j].return_subtree_id() == node1_subtree_id ){
 						SG_edge[j].set_subtree_id( node2_subtree_id );
					}
				}
			} 
			else if( node1_subtree_id < node2_subtree_id ){
				SG_edge.push_back( Complete[i] );
				SG_edge[ SG_edge.size() - 1 ].set_subtree_id( node1_subtree_id );
				for( unsigned int j = 0  ; j < SG_edge.size() ; j++){
 					if( SG_edge[j].return_subtree_id() == node2_subtree_id ){
 						SG_edge[j].set_subtree_id( node1_subtree_id );
					}
				} 
			}
		}
		//抓到足夠的edge , 後面不用再看了
		if( SG_edge.size() == total_pin_nu - 1 ){
 			break ; 
		}
	}// scan all edge in Complete , increasing 

}		/* -----  end of function build_kruskal_tree_detail  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: kruskal_tree::build_kruskal_tree  
 *  Description:   這是用來吃 2D 的
 * =====================================================================================
 */

void kruskal_tree::build_2D_kruskal_tree(  vector<edge> &SG_edge , level_info &my_level_info ){
	
	// 這裡我可以這樣宣告 , 但要怎麼初始化 , 還是說這樣就好了
	// 我記得土哥說  動態陣列  , 有一維可以不用給值  , 二維 , 原來前面不用加 * 
	vector<vertex> list_SG_vertex ; 
	// 接下來要先把 , 所有pin_vertex 都抓出來 , 先建個spanning graph 
	// my_level_info.array_vertex  是用來存 不pin or obstacle corner 的地方
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if(my_level_info.array_vertex[i].Is_pin_or_not() == true ){
			list_SG_vertex.push_back(my_level_info.array_vertex[i]);
		}
	}
	
	/*-----------------------------------------------------------------------------
	 *      			簡單的測pin 有沒有被納入 
	 *      			只輸出一個點來看而己
	 *for( int i = 0 ; i < plane_width ; i++ ){
 		if( SG_vertex[i].size() > 0 ){
 			cout << "x = " << i << "  ,  size = " << SG_vertex[i].size() << endl ;   
			cout << SG_vertex[i][0].print_x_position() << " , " << SG_vertex[i][0].print_y_position() << endl ; 
		}
	}
	 *-----------------------------------------------------------------------------*/
	
	// 接下來 , 我要 從list_SG_vertex , 一個個往後掃 , 建立Complete_graph 
	// 抓出所有edge  放進Complete_graph__edge 中 , 而且還要更改 class edge 因為這裡是要做
	// 我要多一個值叫 weight ，來排序  	
	// vector<edge> SG_edge ;  原本是放這的
	// 
	vector<edge> Complete_graph_edge ; 
	for(unsigned int i = 0  ; i < list_SG_vertex.size() ; i++){
		for(unsigned int j = i + 1 ; j < list_SG_vertex.size() ; j++ ){
			Complete_graph_edge.push_back( edge( list_SG_vertex[i].print_x_position() , list_SG_vertex[i].print_y_position() ,
					   	 	     list_SG_vertex[j].print_x_position() , list_SG_vertex[j].print_y_position()     ) );
		}
	}	
	// 做排序
	sort(Complete_graph_edge.begin() , Complete_graph_edge.end() , sort_weight );
	
	// 由edge weight 小的開始看 , 能否加入 SG_edge , 我打算在class edge 中 , 加入 一個func , 用來判斷edge能不能加 , 能加 , 就直接加入 , 
	// 所以傳入的引數有 , SG_vertex[plane_width] 判斷點歸屬那個subtree , SG_edge 要直接把點加入 ,  因為是由為個Complete_graph__edge呼叫的
	// 所以這個不用傳入 , 不斷的加edge 後 , 我只要判斷 , SG_edge的數目 , 是否等於  my_level_info.my_level_info.pin_nu -  1 , 到了便可以退出 :

	for(unsigned int i = 0  ; i < Complete_graph_edge.size() ; i ++ ){
 		if( SG_edge.size() <= list_SG_vertex.size() - 1 ){
		}
	}	
	// 傳 SG_vertex[0] , 如同傳這個vector 的第一個位置 , 那為什麼不能只傳 SG_vertex , 應是 要像第二三個參數一樣 , 要一維的  	
	// SG_vertex[0] 是傳 x = 0  , 這個位置上 , 有的vertex 
	build_2D_kruskal_tree_detail(  Complete_graph_edge , SG_edge , my_level_info.pin_nu ); // SG_vertex[plane_width] , 是要判斷 每個edge 的歸屬 , 第二個引數是要看
												//要不要加入第三個引數內  , 要傳二維的過去
												//第一個引要要寫成 &SG_vertex[0] 
	/*-----------------------------------------------------------------------------
	 *  cout << "SG_edge  size = " << SG_edge.size() << endl ;
	for(unsigned int i = 0  ; i < SG_edge.size() ; i++ ){
 		cout << SG_edge[i].print_weight() << endl  ; 

	}
	 *-----------------------------------------------------------------------------*/

}// end of kruskal_tree::build_kruskal_tree


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: kruskal_tree::build_overlap_kruskal_tree  
 *  Description:   這是用來吃 2D 的
 * =====================================================================================
 */

void kruskal_tree::build_2D_overlap_kruskal_tree(  vector<edge> &SG_edge , level_info &my_level_info , vector<vertex> &list_SG_vertex ){
	
	
	/*-----------------------------------------------------------------------------
	 *      			簡單的測pin 有沒有被納入 
	 *      			只輸出一個點來看而己
	 *for( int i = 0 ; i < plane_width ; i++ ){
 		if( SG_vertex[i].size() > 0 ){
 			cout << "x = " << i << "  ,  size = " << SG_vertex[i].size() << endl ;   
			cout << SG_vertex[i][0].print_x_position() << " , " << SG_vertex[i][0].print_y_position() << endl ; 
		}
	}
	 *-----------------------------------------------------------------------------*/
	
	// 接下來 , 我要 從list_SG_vertex , 一個個往後掃 , 建立Complete_graph 
	// 抓出所有edge  放進Complete_graph__edge 中 , 而且還要更改 class edge 因為這裡是要做
	// 我要多一個值叫 weight ，來排序  	
	// vector<edge> SG_edge ;  原本是放這的
	// 
	vector<edge> Complete_graph_edge ; 
	for(unsigned int i = 0  ; i < list_SG_vertex.size() ; i++){
		for(unsigned int j = i + 1 ; j < list_SG_vertex.size() ; j++ ){
			Complete_graph_edge.push_back( edge( list_SG_vertex[i].print_x_position() , list_SG_vertex[i].print_y_position() ,
					   	 	     list_SG_vertex[j].print_x_position() , list_SG_vertex[j].print_y_position()     ) );
		}
	}	
	// 做排序
	sort(Complete_graph_edge.begin() , Complete_graph_edge.end() , sort_weight );
	
	// 由edge weight 小的開始看 , 能否加入 SG_edge , 我打算在class edge 中 , 加入 一個func , 用來判斷edge能不能加 , 能加 , 就直接加入 , 
	// 所以傳入的引數有 , SG_vertex[plane_width] 判斷點歸屬那個subtree , SG_edge 要直接把點加入 ,  因為是由為個Complete_graph__edge呼叫的
	// 所以這個不用傳入 , 不斷的加edge 後 , 我只要判斷 , SG_edge的數目 , 是否等於  my_level_info.my_level_info.pin_nu -  1 , 到了便可以退出 :

	for(unsigned int i = 0  ; i < Complete_graph_edge.size() ; i ++ ){
 		if( SG_edge.size() <= list_SG_vertex.size() - 1 ){
		}
	}	
	// 傳 SG_vertex[0] , 如同傳這個vector 的第一個位置 , 那為什麼不能只傳 SG_vertex , 應是 要像第二三個參數一樣 , 要一維的  	
	// SG_vertex[0] 是傳 x = 0  , 這個位置上 , 有的vertex 
	build_2D_kruskal_tree_detail(  Complete_graph_edge , SG_edge , my_level_info.pin_nu ); // SG_vertex[plane_width] , 是要判斷 每個edge 的歸屬 , 第二個引數是要看
												//要不要加入第三個引數內  , 要傳二維的過去
												//第一個引要要寫成 &SG_vertex[0] 
	/*-----------------------------------------------------------------------------
	 *  cout << "SG_edge  size = " << SG_edge.size() << endl ;
	for(unsigned int i = 0  ; i < SG_edge.size() ; i++ ){
 		cout << SG_edge[i].print_weight() << endl  ; 

	}
	 *-----------------------------------------------------------------------------*/

}// end of kruskal_tree::build_kruskal_tree

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: kruskal_tree::build_kruskal_tree_overlap_version
 *  Description:   這是用來吃 2D 的
 * =====================================================================================
 */

void kruskal_tree::build_2D_kruskal_tree_overlap_version(  vector<edge> &SG_edge , level_info &my_level_info ){
	
	
	// 這裡我可以這樣宣告 , 但要怎麼初始化 , 還是說這樣就好了
	// 我記得土哥說  動態陣列  , 有一維可以不用給值  , 二維 , 原來前面不用加 * 
	vector<vertex> list_SG_vertex ; 
	// 接下來要先把 , 所有pin_vertex 都抓出來 , 先建個spanning graph 
	// my_level_info.array_vertex  是用來存 不pin or obstacle corner 的地方
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if(my_level_info.array_vertex[i].Is_pin_or_not() == true ){
			list_SG_vertex.push_back(my_level_info.array_vertex[i]);
		}
	}
	
	// 這裡  ， 要記得  ， 把 my_level_info 中的connected_graph 的值， 都給清成 －1 
	// 先掃過 ， 所有的pin 點 ， 看  ， 總共分成了 幾群
	int how_many_group = 0 ; 
	vector<int> ids ; 
	ids.push_back( my_level_info.connected_graph[ list_SG_vertex[0].print_x_position() ][ list_SG_vertex[0].print_y_position() ] ) ; 
	how_many_group++ ;
	for( unsigned int i = 1 ; i < list_SG_vertex.size() ; i++  ){

		int  x = list_SG_vertex[i].print_x_position() ; 
		int  y = list_SG_vertex[i].print_y_position() ; 
		bool match_or_not = false ; 
		// 有比到相 ， 代表在同個 group 所以不用再多一個箱子了  
		for( unsigned int j = 0 ; j < ids.size() ; j++ ){
			if( my_level_info.connected_graph[x][y] == ids[j] ){
				match_or_not = true ;  
				break ; 
			}
		}// scan all ele from ids 

		if( match_or_not == false   ){
			how_many_group++ ; 
		}
		 
	}//  scan all pin vertex

	// 喔， 我想到了 ， 依這pin所在的group 跟 ids 比  ， 相同就 放到跟ids 相同index 的箱子  
	if( how_many_group == 1 ){
	 
		vector<edge> Complete_graph_edge ; 
		for(unsigned int i = 0  ; i < list_SG_vertex.size() ; i++){
			for(unsigned int j = i + 1 ; j < list_SG_vertex.size() ; j++ ){
				Complete_graph_edge.push_back( edge( list_SG_vertex[i].print_x_position() , list_SG_vertex[i].print_y_position() ,
								     list_SG_vertex[j].print_x_position() , list_SG_vertex[j].print_y_position()     ) );
			}
		}	
		// 做排序
		sort(Complete_graph_edge.begin() , Complete_graph_edge.end() , sort_weight );
		
		// 由edge weight 小的開始看 , 能否加入 SG_edge , 我打算在class edge 中 , 加入 一個func , 用來判斷edge能不能加 , 能加 , 就直接加入 , 
		// 所以傳入的引數有 , SG_vertex[plane_width] 判斷點歸屬那個subtree , SG_edge 要直接把點加入 ,  因為是由為個Complete_graph__edge呼叫的
		// 所以這個不用傳入 , 不斷的加edge 後 , 我只要判斷 , SG_edge的數目 , 是否等於  my_level_info.my_level_info.pin_nu -  1 , 到了便可以退出 :

		for(unsigned int i = 0  ; i < Complete_graph_edge.size() ; i ++ ){
			if( SG_edge.size() <= list_SG_vertex.size() - 1 ){
			}
		}	
		// 傳 SG_vertex[0] , 如同傳這個vector 的第一個位置 , 那為什麼不能只傳 SG_vertex , 應是 要像第二三個參數一樣 , 要一維的  	
		// SG_vertex[0] 是傳 x = 0  , 這個位置上 , 有的vertex 
		build_2D_kruskal_tree_detail(  Complete_graph_edge , SG_edge , my_level_info.pin_nu ); // SG_vertex[plane_width] , 是要判斷 每個edge 的歸屬 
													//	, 第二個引數是要看
													//要不要加入第三個引數內  , 要傳二維的過去
													//第一個引要要寫成 &SG_vertex[0] 
	}
	else if( how_many_group > 1 ){
		vector<vertex> seperate_group[ how_many_group ] ; 

		for( unsigned int i = 0 ; i < list_SG_vertex.size() ; i++  ){

			int  x = list_SG_vertex[i].print_x_position() ; 
			int  y = list_SG_vertex[i].print_y_position() ; 
			// 有比到相 ， 代表在同個 group 所以不用再多一個箱子了  
			for( unsigned int j = 0 ; j < ids.size() ; j++ ){
				if( my_level_info.connected_graph[x][y] == ids[j] ){
					seperate_group[j].push_back( list_SG_vertex[i] ) ; 	
					break ; 
				}
			}// scan all ele from ids 

			 
		}//  scan all pin vertex
	 
	}// end of how_many_group > 1

	/*-----------------------------------------------------------------------------
	 *      			簡單的測pin 有沒有被納入 
	 *      			只輸出一個點來看而己
	 *for( int i = 0 ; i < plane_width ; i++ ){
 		if( SG_vertex[i].size() > 0 ){
 			cout << "x = " << i << "  ,  size = " << SG_vertex[i].size() << endl ;   
			cout << SG_vertex[i][0].print_x_position() << " , " << SG_vertex[i][0].print_y_position() << endl ; 
		}
	}
	 *-----------------------------------------------------------------------------*/
	
	// 接下來 , 我要 從list_SG_vertex , 一個個往後掃 , 建立Complete_graph 
	// 抓出所有edge  放進Complete_graph__edge 中 , 而且還要更改 class edge 因為這裡是要做
	// 我要多一個值叫 weight ，來排序  	
	// vector<edge> SG_edge ;  原本是放這的
	// 
	/*-----------------------------------------------------------------------------
	 *  cout << "SG_edge  size = " << SG_edge.size() << endl ;
	for(unsigned int i = 0  ; i < SG_edge.size() ; i++ ){
 		cout << SG_edge[i].print_weight() << endl  ; 

	}
	 *-----------------------------------------------------------------------------*/

}// end of kruskal_tree::build_kruskal_tree

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:   kruskal_tree::print_kruskal_tree_gun
 *  Description:   這是 處理 2D 的
 * =====================================================================================
 */

void kruskal_tree::print_2D_kruskal_tree_gun( vector<edge> &SG_edge , level_info &my_level_info  ) {
	
	// 先寫出 ,SG的腳本好了  , 這裡有個很奇的現像 , 那就是  我先畫 OASG 再畫 Spanning_graph , 它會把OASG掛上去哎	
	// 所以執行gnuplot 的順序是  PlaneData   Spanning_graph  OASG   很奇怪 , 我上面的其實是complete graph ,
	// 雖然建出來的東西 應該是一樣的 , 但會花較多的時間  
	ofstream SGcout("Kruskal_Tree.gnu");
	SGcout << " set title \"Kruskal_Tree\" " << endl ; 
	SGcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	SGcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	SGcout << "set xlabel \"X-axis\"" << endl ; 
	SGcout << "set ylabel \"Y-axis\"" << endl ; 
	SGcout << "set output " << endl ; 
	SGcout << "set terminal X11 " << endl ; 
	/*-----------------------------------------------------------------------------
	SGcout << "set terminal postscript color" << endl ; 
	SGcout << "set output \"Kruskal_Tree.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/
	SGcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 

	SGcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( !( my_level_info.array_vertex[i].Is_pin_or_not() )    )
		{
			// obstacle 的資訊 , 是 要給二個點 才能畫  分別是1跟3 
			if( my_level_info.array_vertex[i].print_corner_id() == 1){
				SGcout << "set object "<< my_level_info.array_vertex[i].print_obstacle_id() <<" rect from " ; 
				my_level_info.array_vertex[i].print_obstacle_for_script(SGcout )  ; // 這是要輸出點1的資訊的
				SGcout << "to " ;	
			}
			else if( my_level_info.array_vertex[i].print_corner_id() == 3){
				my_level_info.array_vertex[i].print_obstacle_for_script(SGcout ) ;// 這是點3 
				SGcout << endl ; 
			}

		}
	}

	
	//那這裡要印出　OASG 內的所有line 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < SG_edge.size() ; i++){
		SG_edge[i].print_edge_information_for_gnu( SGcout , i + 1 );// i + 1 只是obstacle 的編號而己 
	}
	
	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	SGcout << "plot \"PlaneData.txt\" w lp pt 7 lc rgb \"black\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	SGcout.close() ;

}// end of kruskal_tree::print_kruskal_tree_gun

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:   kruskal_tree::print_2D_Overlap_kruskal_tree_gun
 *  Description:   這是 處理 2D overlap的
 * =====================================================================================
 */

void kruskal_tree::print_2D_overlap_kruskal_tree_gun( vector<edge> &SG_edge , level_info &my_level_info  ) {
	
	// 先寫出 ,SG的腳本好了  , 這裡有個很奇的現像 , 那就是  我先畫 OASG 再畫 Spanning_graph , 它會把OASG掛上去哎	
	// 所以執行gnuplot 的順序是  PlaneData   Spanning_graph  OASG   很奇怪 , 我上面的其實是complete graph ,
	// 雖然建出來的東西 應該是一樣的 , 但會花較多的時間   , 奇了 ， 這裡 建個kruskal tree ， 幹麻要額外寫一個
	ofstream SGcout("Kruskal_Tree.gnu");
	SGcout << " set title \"Kruskal_Tree\" " << endl ; 
	SGcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	SGcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	SGcout << "set xlabel \"X-axis\"" << endl ; 
	SGcout << "set ylabel \"Y-axis\"" << endl ; 
	SGcout << "set output " << endl ; 
	SGcout << "set terminal X11 " << endl ; 
	/*-----------------------------------------------------------------------------
	SGcout << "set terminal postscript color" << endl ; 
	SGcout << "set output \"Kruskal_Tree.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/
	SGcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 

	SGcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	for( int i = 0 ; i < my_level_info.total ; i++ ){
		if( !( my_level_info.array_vertex[i].Is_pin_or_not() )    )
		{
			// obstacle 的資訊 , 是 要給二個點 才能畫  分別是1跟3 
			if( my_level_info.array_vertex[i].print_corner_id() == 1){
				SGcout << "set object "<< my_level_info.array_vertex[i].print_obstacle_id() <<" rect from " ; 
				my_level_info.array_vertex[i].print_obstacle_for_script(SGcout )  ; // 這是要輸出點1的資訊的
				SGcout << "to " ;	
			}
			else if( my_level_info.array_vertex[i].print_corner_id() == 3){
				my_level_info.array_vertex[i].print_obstacle_for_script(SGcout ) ;// 這是點3 
				SGcout << endl ; 
			}

		}
	}

	
	//那這裡要印出　OASG 內的所有line 
	SGcout << "# 這裡要印出　edge   " << endl ; 
	for(unsigned int i  = 0 ; i < SG_edge.size() ; i++){
		SG_edge[i].print_edge_information_for_gnu( SGcout , i + 1 );// i + 1 只是obstacle 的編號而己 
	}
	
	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	SGcout << "plot \"Overlap_PlaneData.txt\" w lp pt 7 lc rgb \"black\" ,  \"Overlap_Vertex.txt\" w lp pt 7 lc rgb \"green\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	SGcout.close() ;

}// end of kruskal_tree::print_kruskal_tree_gun

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  build_3D_kruskal_tree
 *  Description:    這是可以吃3D  ,  先依照 2D 的東西 改出來
 * =====================================================================================
 */
	void
kruskal_tree::build_3D_kruskal_tree ( vector<edge> &SG_edge , vector<level_info> &every_level_info )
{
	
	// 這裡我可以這樣宣告 , 但要怎麼初始化 , 還是說這樣就好了
	// 我記得土哥說  動態陣列  , 有一維可以不用給值  , 二維 , 原來前面不用加 * 
	vector<vertex> list_SG_vertex ; 
	int total_pin_3D = 0 ; 
	// 接下來要先把 , 所有pin_vertex 都抓出來 , 先建個spanning graph 
	// my_level_info.array_vertex  是用來存 不pin or obstacle corner 的地方
	for( int i = 0 ; i < every_level_info[0].plane_level ; i++ ){

		for( int j = 0 ; j < every_level_info[i].total ; j++ ){
			if( every_level_info[i].array_vertex[j].Is_pin_or_not() == true ){
				list_SG_vertex.push_back( every_level_info[i].array_vertex[j] );
			}
		}// end of loop j 

		total_pin_3D = total_pin_3D + every_level_info[i].print_pin_nu() ; 
	 
	}// end of loop i 
	
	/*-----------------------------------------------------------------------------
	 *      			簡單的測pin 有沒有被納入 
	 *      			只輸出一個點來看而己
	 *for( int i = 0 ; i < plane_width ; i++ ){
 		if( SG_vertex[i].size() > 0 ){
 			cout << "x = " << i << "  ,  size = " << SG_vertex[i].size() << endl ;   
			cout << SG_vertex[i][0].print_x_position() << " , " << SG_vertex[i][0].print_y_position() << endl ; 
		}
	}
	 *-----------------------------------------------------------------------------*/
	
	// 接下來 , 我要 從list_SG_vertex , 一個個往後掃 , 建立Complete_graph 
	// 抓出所有edge  放進Complete_graph__edge 中 , 而且還要更改 class edge 因為這裡是要做
	// 我要多一個值叫 weight ，來排序  	
	// vector<edge> SG_edge ;  原本是放這的
	vector<edge> Complete_graph_edge ; 
	for(unsigned int i = 0  ; i < list_SG_vertex.size() ; i++){
		for(unsigned int j = i + 1 ; j < list_SG_vertex.size() ; j++ ){
			// 下面這個建構式 ， 是我 新寫的 
			Complete_graph_edge.push_back( edge( list_SG_vertex[i].print_x_position() , list_SG_vertex[i].print_y_position() , 
						             list_SG_vertex[i].print_z_position() ,		
					   	 	     list_SG_vertex[j].print_x_position() , list_SG_vertex[j].print_y_position() ,
							     list_SG_vertex[j].print_z_position()				         	) );
		}
	}	

	// 做排序 ,    這裡呢 我要改的 應該是 
	sort( Complete_graph_edge.begin() , Complete_graph_edge.end() , sort_3D_weight );
	
	// 由edge weight 小的開始看 , 能否加入 SG_edge , 我打算在class edge 中 , 加入 一個func , 用來判斷edge能不能加 , 能加 , 就直接加入 , 
	// 所以傳入的引數有 , SG_vertex[plane_width] 判斷點歸屬那個subtree , SG_edge 要直接把點加入 ,  因為是由為個Complete_graph__edge呼叫的
	// 所以這個不用傳入 , 不斷的加edge 後 , 我只要判斷 , SG_edge的數目 , 是否等於  my_level_info.my_level_info.pin_nu -  1 , 到了便可以退出 :


	// 奇怪了 下面這一塊  ， 裡面沒有東西丫 
	//  真是沒有東西  ， 但是 ， 我拿掉怎麼 ， 就會 當了呢 
	for(unsigned int i = 0  ; i < Complete_graph_edge.size() ; i ++ ){
 		if( SG_edge.size() <= list_SG_vertex.size() - 1 ){
		}
	}	

	// 傳 SG_vertex[0] , 如同傳這個vector 的第一個位置 , 那為什麼不能只傳 SG_vertex , 應是 要像第二三個參數一樣 , 要一維的  	
	// SG_vertex[0] 是傳 x = 0  , 這個位置上 , 有的vertex 
	// 這裡呢 ， 因為 第三個引數 是要傳 這層 有多少個 pin 點 ， 那是在2D 上的 
	// 拿到3D 來看 也就是要 傳入 整個 3D 下的pin 點有多少個 
	build_3D_kruskal_tree_detail(  Complete_graph_edge , SG_edge , total_pin_3D ); // SG_vertex[plane_width] , 是要判斷 每個edge 的歸屬 , 第二個引數是要看
												//要不要加入第三個引數內  , 要傳二維的過去
												//第一個引要要寫成 &SG_vertex[0] 
	/*-----------------------------------------------------------------------------
	 *  cout << "SG_edge  size = " << SG_edge.size() << endl ;
	for(unsigned int i = 0  ; i < SG_edge.size() ; i++ ){
 		cout << SG_edge[i].print_weight() << endl  ; 

	}
	 *-----------------------------------------------------------------------------*/

}		/* -----  end of function build_3D_kruskal_tree  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  build_3D_overlap_kruskal_tree
 *  Description:    這是可以吃3D  ,  先依照 2D 的東西 改出來
 * =====================================================================================
 */
	void
kruskal_tree::build_3D_overlap_kruskal_tree ( vector<edge> &SG_edge , vector<level_info> &every_level_info )
{
	
	// 這裡我可以這樣宣告 , 但要怎麼初始化 , 還是說這樣就好了
	// 我記得土哥說  動態陣列  , 有一維可以不用給值  , 二維 , 原來前面不用加 * 
	vector<vertex> list_SG_vertex ; 
	int total_pin_3D = 0 ; 
	// 接下來要先把 , 所有pin_vertex 都抓出來 , 先建個spanning graph 
	// my_level_info.array_vertex  是用來存 不pin or obstacle corner 的地方
	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		for(unsigned  int j = 0 ; j < every_level_info[i].array_vertex_vector.size() ; j++ ){
			if( every_level_info[i].array_vertex_vector[j].Is_pin_or_not() == true ){
				list_SG_vertex.push_back( every_level_info[i].array_vertex_vector[j] );
			}
		}// end of loop j 

		total_pin_3D = total_pin_3D + every_level_info[i].print_pin_nu() ; 
	 
	}// end of loop i 
	
	/*-----------------------------------------------------------------------------
	 *      			簡單的測pin 有沒有被納入 
	 *      			只輸出一個點來看而己
	 *for( int i = 0 ; i < plane_width ; i++ ){
 		if( SG_vertex[i].size() > 0 ){
 			cout << "x = " << i << "  ,  size = " << SG_vertex[i].size() << endl ;   
			cout << SG_vertex[i][0].print_x_position() << " , " << SG_vertex[i][0].print_y_position() << endl ; 
		}
	}
	 *-----------------------------------------------------------------------------*/
	
	// 接下來 , 我要 從list_SG_vertex , 一個個往後掃 , 建立Complete_graph 
	// 抓出所有edge  放進Complete_graph__edge 中 , 而且還要更改 class edge 因為這裡是要做
	// 我要多一個值叫 weight ，來排序  	
	// vector<edge> SG_edge ;  原本是放這的
	vector<edge> Complete_graph_edge ; 
	for(unsigned int i = 0  ; i < list_SG_vertex.size() ; i++){
		for(unsigned int j = i + 1 ; j < list_SG_vertex.size() ; j++ ){
			// 下面這個建構式 ， 是我 新寫的 
			Complete_graph_edge.push_back( edge( list_SG_vertex[i].print_x_position() , list_SG_vertex[i].print_y_position() , 
						             list_SG_vertex[i].print_z_position() ,		
					   	 	     list_SG_vertex[j].print_x_position() , list_SG_vertex[j].print_y_position() ,
							     list_SG_vertex[j].print_z_position()				         	) );
		}
	}	

	// 做排序 ,    這裡呢 我要改的 應該是 
	sort( Complete_graph_edge.begin() , Complete_graph_edge.end() , sort_3D_weight );
	
	// 由edge weight 小的開始看 , 能否加入 SG_edge , 我打算在class edge 中 , 加入 一個func , 用來判斷edge能不能加 , 能加 , 就直接加入 , 
	// 所以傳入的引數有 , SG_vertex[plane_width] 判斷點歸屬那個subtree , SG_edge 要直接把點加入 ,  因為是由為個Complete_graph__edge呼叫的
	// 所以這個不用傳入 , 不斷的加edge 後 , 我只要判斷 , SG_edge的數目 , 是否等於  my_level_info.my_level_info.pin_nu -  1 , 到了便可以退出 :


	// 奇怪了 下面這一塊  ， 裡面沒有東西丫 
	//  真是沒有東西  ， 但是 ， 我拿掉怎麼 ， 就會 當了呢 
	for(unsigned int i = 0  ; i < Complete_graph_edge.size() ; i ++ ){
 		if( SG_edge.size() <= list_SG_vertex.size() - 1 ){
		}
	}	

	// 傳 SG_vertex[0] , 如同傳這個vector 的第一個位置 , 那為什麼不能只傳 SG_vertex , 應是 要像第二三個參數一樣 , 要一維的  	
	// SG_vertex[0] 是傳 x = 0  , 這個位置上 , 有的vertex 
	// 這裡呢 ， 因為 第三個引數 是要傳 這層 有多少個 pin 點 ， 那是在2D 上的 
	// 拿到3D 來看 也就是要 傳入 整個 3D 下的pin 點有多少個 
	build_3D_kruskal_tree_detail(  Complete_graph_edge , SG_edge , total_pin_3D ); // SG_vertex[plane_width] , 是要判斷 每個edge 的歸屬 , 第二個引數是要看
												//要不要加入第三個引數內  , 要傳二維的過去
												//第一個引要要寫成 &SG_vertex[0] 
	/*-----------------------------------------------------------------------------
	 *  cout << "SG_edge  size = " << SG_edge.size() << endl ;
	for(unsigned int i = 0  ; i < SG_edge.size() ; i++ ){
 		cout << SG_edge[i].print_weight() << endl  ; 
	}
	 *-----------------------------------------------------------------------------*/

}		/* -----  end of function build_3D_kruskal_tree  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:   kruskal_tree::print_3D_kruskal_tree_gun
 *  Description:   這是 處理 3D 的
 * =====================================================================================
 */

void kruskal_tree::print_3D_kruskal_tree_gun( vector<edge> &SG_edge ,  vector<level_info> &every_level_info ) {
	
	// 先寫出 ,SG的腳本好了  , 這裡有個很奇的現像 , 那就是  我先畫 OASG 再畫 Spanning_graph , 它會把OASG掛上去哎	
	// 所以執行gnuplot 的順序是  PlaneData   Spanning_graph  OASG   很奇怪 , 我上面的其實是complete graph ,
	// 雖然建出來的東西 應該是一樣的 , 但會花較多的時間  
	ofstream SGcout("3D_Kruskal_Tree.gnu");
	SGcout << " set title \"3D_Kruskal_Tree\" " << endl ; 
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
	for(unsigned int i  = 0 ; i < SG_edge.size() ; i++){
		// 我想這裡 也要改了  ， 下面 這個 func 原是用來處理 2D 的 
		// edge.h 已經改過了 ， 所以裡面是 3D 的 position 
		SG_edge[i].print_3D_edge_information_for_gnu( SGcout , arrow_count + i + 1 );// i + 1 只是obstacle 的編號而己 
	}
	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;

}// end of kruskal_tree::print_kruskal_tree_gun

#endif
