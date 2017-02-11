/*
 * =====================================================================================
 *
 *       Filename:  obstacle_avoding_kruskal.h 
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/02/09 12時59分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef        OBSTACLE_AVODING_KRUSKAL_H 
	#define  OBSTACLE_AVODING_KRUSKAL_H 
#include	<iostream>
#include	<fstream>
#include	"edge.h"
#include	"vertex.h"
#include 	"path_node.h"	
#include 	"obstacle.h"	
#include 	"level_info.h"	
#include 	"rectilinear.h"	
#include	<vector>
#include	<omp.h>
#include        <cstdio>
// 哇操 ， 這是在網頁中查到的東西 ， 就是 ，  hash_map 不是內建的  ， 是放在別的地方
// 所以要在下面前面多加個目路才行  hash_map的实现代码（其实现为sgi stl的版本），其在include/ext目录下，该目录还包含了hash_set，rope等的实现。 
// #include        <ext/hash_map>

using namespace std ; 
// using namespace __gnu_cxx;

 class obstacle_avoding_kruskal{

 private:

 public:

	//void (int const &total , vertex *array_vertex , int const &pin_nu , vector<edge> &SG_edge );
  	// int const &plane_width , int const &plane_height ,vector<edge> &SG_edge , int const &total , vertex *array_vertex
	void build_obstacle_avoding_kruskal(  vector<edge> &SG_edge  , vector<edge> &obstacle_avoding_Kruskal  ,  
					      vector<edge> &OASG , level_info &my_level_info  ) ; 

	void build_obstacle_avoding_kruskal_imp(  vector<edge> &SG_edge  , vector<edge> &obstacle_avoding_Kruskal  ,  
					      vector<edge> &OASG , level_info &my_level_info  ) ; 

	void build_overlap_obstacle_avoding_kruskal_imp(  vector<edge> &SG_edge  , vector<edge> &obstacle_avoding_Kruskal  ,  
					      vector<edge> &OASG , level_info &my_level_info  ) ; 

	void build_obstacle_avoding_kruskal_with_projection_vertex(  vector<edge> &SG_edge  , vector<edge> &ThreeD_obstacle_avoding_Kruskal   , 
								     vector<edge> &vertical_edge , vector<level_info> &every_level_info       ); 
					      
	
	void build_obstacle_avoding_kruskal_with_projection_vertex_imp(  vector<edge> &SG_edge  , vector<edge> &ThreeD_obstacle_avoding_Kruskal   , 
								     vector<edge> &vertical_edge , vector<level_info> &every_level_info       ); 

	/*-----------------------------------------------------------------------------
	 *  奇怪了  ， 第一個參數 ， 怎麼會是 SG_edge  呢 ， 這是用來存 2D Kruskal_Tree 的
	 *  ， 我沒有 理由 ， 在這裡 又在畫一次 ， 這個  kruskal 丫   ， 這裡 理應是畫
	 *  obstacle_avoding_kruskal 才對  ， 那應該是傳 obstacle_avoding_Kruskal 
	 *-----------------------------------------------------------------------------*/
	void print_obstacle_avoding_kruskal_gun( vector<edge> &SG_edge , level_info &my_level_info );

	// 因為 ， overlap obstacle 的 輸出不太一樣  ， 所以  ， 要額外寫一個 
	void print_overlap_obstacle_avoding_kruskal_gun( vector<edge> &SG_edge , level_info &my_level_info );


	/*-----------------------------------------------------------------------------
	 *  這裡 是要印出  3D 的 obstacle_avoding_kruskal , 所以  ， 只要傳入
	 *  ThreeD_obstacle_avoding_Kruskal 來印出edge 應該就可以了 
	 *-----------------------------------------------------------------------------*/
	void print_threeD_obstacle_avoding_kruskal_gun( vector<edge> &ThreeD_obstacle_avoding_Kruskal  , vector<level_info> &every_level_info );
};

//1 define the hash function
struct hash_vertex{
        size_t operator()(const class vertex &a1 )const{
        //  return  hash<int>(classA.getvalue());
                         return a1.get_id();
        }
};

//2 define the equal function , 好奇怪的規格  ， 它用的func 要弄成 const 
//我記得沒錯的話  ， 是func裡的東西 ， 都不能改   , 連我裡面呼叫的func 都要弄成 const 
struct equal_vertex{
        bool operator()(const class vertex &a1, const class vertex &a2 )const{
        	return  ( a1.print_x_position() == a2.print_x_position() && a1.print_y_position() == a2.print_y_position() 
									 && a1.print_z_position() == a2.print_z_position() );
        }
};


void obstacle_avoding_kruskal::build_obstacle_avoding_kruskal( vector<edge> &SG_edge  , vector<edge> &obstacle_avoding_Kruskal  ,
							       vector<edge> &OASG , level_info &my_level_info ){

	// 09 27 我打算改一樣東西  ， 就是 ， edge 在 跟 obstacle 有相鄰時  ， 不改動它  ， 等到 rectilinear 再去弄 
	
	 // cout << "Coming obstacle_avoding_Kruskal func " << endl ; 
	/*-----------------------------------------------------------------------------
	 *  				step 1 ,2 
	 *  				check edge ok or not 
	 *  				理應是obstacle的數目比較少　，　所以
	 *  				裡面的loop 是用　my_level_info.complete_obstacle_information
	 *  				這裡  光是每個edge 都要跟obstacle 做比對 ， 怎麼受的了 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i  = 0 ; i < SG_edge.size() ; i++){
		
		/*-----------------------------------------------------------------------------
		 *  cout << "Here is edge information " << endl ;  
		SG_edge[i].print_kruskal_edge_information() ; 
		cout << "min_x = " ;
		cout << SG_edge[i].print_min_x() << endl ; 
		cout << "max_x = " ;
		cout << SG_edge[i].print_max_x() << endl ;
		cout << "min_y = " ;
		cout << SG_edge[i].print_min_y() << endl ; 
		cout << "max_y = " ;
		cout << SG_edge[i].print_max_y() << endl ;
		cout << "kruskal_edge_ok_or_not = "  <<  SG_edge[i].print_kruskal_edge_ok_not() << endl ;

		下面呢  我要重 寫  ， 用 obstacle_map 及 obstacle_bound_cross , 這二個參數  ， 來 減少下面
		的時間 
		 *-----------------------------------------------------------------------------*/
		for(int j = SG_edge[i].print_min_x() ; j <= SG_edge[i].print_max_x() ; j++ ){

			map<int,obstacle>::iterator it ;  
			int key = 0 ; 
			for(unsigned int k = 0 ; k < my_level_info.obstacle_cross[j].size() ; k++ ){
			 
				 key = my_level_info.obstacle_cross[j][k] ;
				 it = my_level_info.obstacle_map.find( key ) ; 
				 bool first_case = false , Second_case = false ; 

				 if( SG_edge[i].print_max_x() >= (*it).second.print_ld_corner_x() && 
				     SG_edge[i].print_min_x() <= (*it).second.print_ru_corner_x() ){
					first_case = true ;  
				 }
				 if( SG_edge[i].print_max_y() >= (*it).second.print_ld_corner_y() && 
				     SG_edge[i].print_min_y() <= (*it).second.print_ru_corner_y() ){
					Second_case = true ;  
				 }

				 /*-----------------------------------------------------------------------------
				  *  cout << "first_case = " << first_case << endl; 
				 cout << "Second_case = " << Second_case << endl ;
				 if( first_case == true || Second_case == true ){
					my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
				 }
				 cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
				  *-----------------------------------------------------------------------------*/
				  

				 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
				 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
				 //  就不用再往下比了  , 所以我多了個 break ; 
				 if( first_case == true && Second_case == true ){
					SG_edge[i].set_kruskal_edge_ok_not(false);
					SG_edge[i].add_which_obstacle_cross( (*it).second.print_obstacle_id() );
					break ; 
				 }
			}
		 
		}// 只 scan SG 有踫到的x 範圍  

		/*-----------------------------------------------------------------------------
		 *  
		for(unsigned int j  = 0 ; j < my_level_info.complete_obstacle_information.size()  ; j++){
		
		//	   我想到個好方法了　，　藉由　在Kruskal edge 中　，　抓出其最大的x,y
		//	 跟obstacle中　，　的左下角點比　，　即可　只要max_x 大於等於node1_x 
		//	 　就有機會相交了　，　還要再心對max_y >= node1_y ，如果二個都成立
		//	 那就一定有overlap , 但如果x 那關都過不了　就一定沒有overlap
		//	, 我還少弄一些判斷進去　那就是min x,y , 要小於左下的角點　，　不然edge
		//	　如果在右上角　　也會被算進去
		//		max_y >= ld_corner_y
		//		min_y <= ru_corner_y
		//		反之　x 的也是如此 
		//	 bool first_case = false , Second_case = false ; 

			 if( SG_edge[i].print_max_x() >= my_level_info.complete_obstacle_information[j].print_ld_corner_x() && 
			     SG_edge[i].print_min_x() <= my_level_info.complete_obstacle_information[j].print_ru_corner_x() ){
			 	first_case = true ;  
			 }
			 if( SG_edge[i].print_max_y() >= my_level_info.complete_obstacle_information[j].print_ld_corner_y() && 
			     SG_edge[i].print_min_y() <= my_level_info.complete_obstacle_information[j].print_ru_corner_y() ){
			 	Second_case = true ;  
			 }

			// cout << "first_case = " << first_case << endl; 
			// cout << "Second_case = " << Second_case << endl ;
			// if( first_case == true || Second_case == true ){
			//	my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
			// }
			// cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
			  

			 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
			 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
			 //  就不用再往下比了  , 所以我多了個 break ; 
			 if( first_case == true && Second_case == true ){
			 	SG_edge[i].set_kruskal_edge_ok_not(false);
				SG_edge[i].add_which_obstacle_cross( my_level_info.complete_obstacle_information[j].print_obstacle_id() );
				break ; 
			 }
			//cout << "kruskal_edge_ok_or_not = "  <<  SG_edge[i].print_kruskal_edge_ok_not() << endl ;
		}// end of for　j
		 *-----------------------------------------------------------------------------*/

	}// end of 抓出edge  是否要更動　，　以及overlap 的obstacle 有那些  
	

	

	/*-----------------------------------------------------------------------------
	 * 				Check
	 * 		every Kruskal edge ok or not 
	 * 		and which obstacles it cross  .    
	 *        cout << endl << endl << "Check Kruskal_Tree edge " << endl ; 
		for(unsigned int i = 0 ; i < SG_edge.size() ; i++  ){
		
		cout << "This is edge number " << i+1 << endl ; 
		// 奇怪了　，　只要是設定false 那就一定有值會出來才對丫  
		SG_edge[i].print_kruskal_edge_information() ; 
	}

	 *-----------------------------------------------------------------------------*/
	 
	/*-----------------------------------------------------------------------------
	 *  				Step 3,4
	 *  		因為我已經抓出　有cross 的obstacle了　，　要抓其四個端點
	 *  		很簡單　，　接下來呢，　就建個新的vector 用來存
	 *  		obstacle_avoding_by_kruskal_Tree , 
	 *  		為true 的edge 就直接放入　，　false 的便修改後　，再放入
	 *
	 *-----------------------------------------------------------------------------*/
	// 下面這二個  好像後面還會用到的樣子
	// 沒有 只有  第一個 要記得放外面就好了
	// vector<edge> obstacle_avoding_Kruskal ; 	
	// 下面建了個 vertex_map , 是用來  ， 給vertex抓出它的index 
	vector<vertex> vertex_need ; 

	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 因為我已經寫好了 vertex 的copy constructor 所以vertex 可以直接給了  	
			// 這裡是存  ， 在plane 內 ， 所有的pin 及 obstalce corner 所組成的點 
			vertex_need.push_back( my_level_info.array_vertex[j] );	
			vertex_need[j].set_id( j ) ; 
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = j ; 

	}// end of for
	
	/*-----------------------------------------------------------------------------
	 *  
	// vector<vertex> vertex_seperate[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < vertex_need.size() ; i++ ){
		vertex_need[i].set_id( i ) ; 
		int x = vertex_need[i].print_x_position() ; 
		int y = vertex_need[i].print_y_position() ; 
		my_level_info.vertex_id[x][y] = i ; 

		// vertex_seperate[ vertex_need[i].print_x_position() ].push_back( vertex_need[i] ) ; 	 
	}
	 *-----------------------------------------------------------------------------*/

	 // 這裡把表的初值給建好了
	 // 應該把　path_node 換成　table_node 比較好　
	 // 因為要建相鄰 串列  ， 所以要有個 一維的點集合 ， 後面串 接它相鄰的人有誰  
	 vector<path_node> table[ vertex_need.size() ];			 

	 /*-----------------------------------------------------------------------------
	 // 先把表的樣子給先建起來　
	 // 這個表table  我想建起 ， 這個 vertex 跟 那些vertex 是相鄰的 就好了  
	 // 我只要比對 這是那一個row 的 ， 就可以到 vertex_need 抓到這個點的資訊了 
	 for(unsigned int j = 0  ; j <  vertex_need.size() ; j++ ){
		for(unsigned int k = 0  ; k <  vertex_need.size() ; k++ ){
			table[j].push_back( path_node() ); 
		} 
	 }
	  *-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
	 *  				下面這塊　是建好OAST 的核心
	 *  				要用的edge　我都抓好了　，　
	 *  				接下來便是要抓shortest path 
	 *  	先建出一張表來，　各個vertex 到別的點的weight , 要建幾乘幾的表，
	 *  	可由vertex_need 去看　，　再由edge_need 去一個個掃　，　來建立
	 *  	weight , 那　無法到達的 無限要怎麼代表它呢　
	 *  	, 我建了二個data structure , node_need 跟　edge_need 
	 *  	,node_need中的第０跟１　，　理應是　我要的source跟desitination 
	 *  	表的index 順序，　我想就跟node_need 一樣　，　
	 *  	而表的內容　，　就掃過edge_need , 來一個個建　，　一個edge_need
	 *  	要建　二個位置的值　像(3,1) (1,3)　這樣　
	 *-----------------------------------------------------------------------------*/
	 // 接下來　　，　把值弄進去　，　每個edge 都跟vertex_need 掃過一遍
	 // 抓出edge  二端點的node index ,  這裡建相鄰串列的地方 也是花太多時間了

	for(unsigned int j = 0  ; j < OASG.size() ; j++  ){

		// 內層的for  , 要抓出兩端點的index 
		int index_one = -1 , index_two = -1  ; 

		int x = OASG[j].print_node1_x() , y = OASG[j].print_node1_y()  ; 
		index_one = my_level_info.vertex_id[x][y]  ; 

		int xx = OASG[j].print_node2_x() , yy = OASG[j].print_node2_y()  ; 
		index_two = my_level_info.vertex_id[xx][yy]  ; 
		/*-----------------------------------------------------------------------------
		// 下面這個loop  太花時間了 ， 我打把它寫成map 去抓  , 水拉   ， 換成下面的方法 ， 有變快了
		// 應該是因為  外層是 OASG  太大的關係 
		for(unsigned int k = 0 ; k < vertex_seperate[ OASG[j].print_node1_x() ].size() ; k++ ){

			if( OASG[j].print_node1_y() == vertex_seperate[ OASG[j].print_node1_x() ][k].print_y_position() ){
				// 為什麼 ， 不能在這裡加break ; 
				index_one = vertex_seperate[ OASG[j].print_node1_x() ][k].get_id() ;  
			}

		}// end of for

		for(unsigned int k = 0 ; k < vertex_seperate[ OASG[j].print_node2_x() ].size() ; k++ ){

			if( OASG[j].print_node2_y() == vertex_seperate[ OASG[j].print_node2_x() ][k].print_y_position() ){
				index_two =  vertex_seperate[ OASG[j].print_node2_x() ][k].get_id() ;  
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  
		for(unsigned int k = 0  ;  k < vertex_need.size() ; k++  ){
			// 有的vertex 會對不到
			if(OASG[j].print_node1_x() == vertex_need[k].print_x_position() &&
			   OASG[j].print_node1_y() == vertex_need[k].print_y_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_one = k ;  
			}
			if(OASG[j].print_node2_x() == vertex_need[k].print_x_position() &&
			   OASG[j].print_node2_y() == vertex_need[k].print_y_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_two = k ;  
			}
		}// 　抓好二個端點的index 了
		 *-----------------------------------------------------------------------------*/

		// 要把值給送進table 
		if( index_one != -1 && index_two != -1 ){
			/*-----------------------------------------------------------------------------
			 *  
			 這步做完 ， 應該  就把各個相鄰的東西 ， 給抓出來了吧  
			 之前 我是藉由 ， row 跟 column 來判斷 ， 這個path_node 是
			 那紀路那兩個 vertex 的關係  ， 但現在不能用column 來抓
			 了  ， 所以要在path_node 中 ， 多加一項資訊 ， 叫 
			 desitination_vertex_id  ,  奇怪了 ， 這個老是不能班過去  
			 *-----------------------------------------------------------------------------*/
			table[index_one].push_back( path_node( index_one ,  OASG[j].print_weight() , index_two ) ) ; 
			table[index_two].push_back( path_node( index_two ,  OASG[j].print_weight() , index_one ) ) ; 

			/*-----------------------------------------------------------------------------
			table[index_one][index_two].set_infinite(false) ;
			table[index_one][index_two].set_path_weight( OASG[j].print_weight() );
			table[index_two][index_one].set_infinite(false) ;
			table[index_two][index_one].set_path_weight( OASG[j].print_weight() );	 
			 *-----------------------------------------------------------------------------*/

		}// 建起表內  各個格子的內容 
		
		
	}// end of 抓二端點的　index , 且埴滿table內的值　


	/*-----------------------------------------------------------------------------
	 * 			check code , May 9 
	cout << endl << "here become single source all desitination : " << endl ;  
	// 在這之後的東西 就沒有跑出來了 ， 所以 我想 理應是某個func 被我改了 
	// 但 effect 到這裡  
	// 改善最多的  應該是因為 我下面放的這個吧   ,  只要放對地方  ， 改善很多哎 
	 *-----------------------------------------------------------------------------*/
	int SG_edge_change = SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	for(int i = 0  ; i < SG_edge_change ; i++  ){

		/*-----------------------------------------------------------------------------
		 * 			check code , May 9 
		cout << SG_edge[i].print_kruskal_edge_ok_not() << endl ; 
		 *-----------------------------------------------------------------------------*/
		if( SG_edge[i].print_kruskal_edge_ok_not() == true  ){
			obstacle_avoding_Kruskal.push_back( SG_edge[i] ) ; 
		}
		else{
			
			/*-----------------------------------------------------------------------------
			 *  我要把edge_need 放到上面去做 ， 這裡變成 ， 抓edge兩端點的index出來即可
			 *  這裡抓的index 是指在 vertex_need 裡的index 
			 *  跟老師討論過 ，  那 safe_range 好像不太必要哎 , safe_range = 3
			 *  下面這塊真的是很麻煩 ， 每次都要比到完 
			 *-----------------------------------------------------------------------------*/
			int head_index = -1 , tail_index = -1  ; 	

			int  x = SG_edge[i].print_node1_x() , y = SG_edge[i].print_node1_y()  ; 
			head_index =  my_level_info.vertex_id[x][y] ;  

			int xx = SG_edge[i].print_node2_x() , yy = SG_edge[i].print_node2_y()  ; 
			tail_index =  my_level_info.vertex_id[xx][yy] ;  
			/*-----------------------------------------------------------------------------
			for(unsigned int j = 0 ; j < vertex_seperate[ SG_edge[i].print_node1_x() ].size() ; j++ ){

				if( SG_edge[i].print_node1_y() == vertex_seperate[ SG_edge[i].print_node1_x() ][j].print_y_position() ){
					head_index =  vertex_seperate[ SG_edge[i].print_node1_x() ][j].get_id() ;  
					// 為什麼 ， 不能在這裡加break ; 
				}

			}// end of for

			for(unsigned int j = 0 ; j < vertex_seperate[ SG_edge[i].print_node2_x() ].size() ; j++ ){

				if( SG_edge[i].print_node2_y() == vertex_seperate[ SG_edge[i].print_node2_x() ][j].print_y_position() ){
					tail_index = vertex_seperate[ SG_edge[i].print_node2_x() ][j].get_id() ;  
				}

			}// end of for
			 *-----------------------------------------------------------------------------*/

			//cout << "my_level_info.total = " << my_level_info.total << ",  vertex_need.size " << vertex_need.size() << endl ;   

			// 這裡要寫些code來看資訊 ， 用來抓bug 的 ， 要印出 ， 此時的i 是多少
			// 以及這個要被改的edge  資訊 ， 
			//cout << " i = " << i << endl ; 
			//SG_edge[i].print_kruskal_edge_information() ; 
			
			/*-----------------------------------------------------------------------------
			 *  
			// 這裡 印出個花費相鄰矩陣來看看 好了  
			cout << "Here print out weight table : " << i << endl ; 
			for(unsigned int j = 0 ; j < vertex_need.size() ; j++ ){
			 	for(unsigned int k = 0 ; k < vertex_need.size() ; k++ ){

					if( j == k ){
						table[j][j].set_infinite(false) ;
						table[j][j].set_path_weight( 0 );	 
					}

					if(table[j][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[j][k].print_path_weight() << " " ;	
					}

				}// inner loop j 
				cout << endl << endl ;; 
			}// end of 印出矩陣來看看
			 *-----------------------------------------------------------------------------*/
	
			/*-----------------------------------------------------------------------------
			 *  表建好後　，　接下來便是執行　one source all destination 的algorithm 
			先抓source 行中　weight  最小的　，　及　此edge node2是誰
			把weight加到　node2那行　再跟 source 行比對，　　如果值有變小　，　或
			原本true 的　變false ，　也可把值納入　
			抓出shortest path 中間要的轉接點後　，　可以用　之前的方法　，　從edge_need 中
			抓出　其edge_need 含這些點集的edge 即可
			還是說　用個　bool 紀路這個點　是否當過轉接點,  轉接點 只會有一個 ， 初值我設成 －1 
			只要是 source  能直接到達的 ， 都設成 －1 ， 
			原先的表是拿來查的 ， 不能直接休改 ， 
			 *-----------------------------------------------------------------------------*/
			 
			 /*-----------------------------------------------------------------------------
			  *  
			  因為table的直是用來查的 不是用來更新的 ， 不然  會出點錯誤 ，
			  所以這裡我要建個一行path_node 來 ， 存原始行的data , 
			  等路抓出來以後 ， 再用這個還原回去 
			  *-----------------------------------------------------------------------------*/

			 // 這是 要被更新的那一行
			 vector<path_node> for_update;			 
			// 我只要把上面的for_update變成 for_update[2] , 就可以一個從頭處理， 
			// 一個從尾巴了  , 好  這裡應該不用動它  , for_update 是紀著
			// 要被 更新  的那一row 

			for(unsigned int j = 0  ; j <  table[head_index].size() ; j++ ){
				for_update.push_back( table[head_index][j] ); 
			}// end of 抓出 我要更新的那個row 
			// cout << "for_update.size() = " << for_update.size() << endl ; 


			/*-----------------------------------------------------------------------------
			  如果抓不到一個可以轉接的點　，　便跳出去了　
			  這個 do while 把整個表給更新好了　，　接下來便是看我怎麼抓中間的edge了
			  感覺整個最花時閶就是 下面這塊了  ， 因為 有二個do while 的loop 
			  tail_index_in_for_update , 因為一開始 裡面沒有 我們要到的點的index 
			  所以最後有把要的點放進來的時侯 ， 要記住它的index  
			 *-----------------------------------------------------------------------------*/
			 bool ok = true ; 
			 int tail_index_in_for_update = -1 ; 
			 // int safe_count = 0 ; 
			 // cout << endl << "Ready coming update routing table on SG_edge : " << i << endl ; 
			 do{
			 	
				 /*-----------------------------------------------------------------------------
				  *  下面這一小塊  ， 是要把 之間有路  ， 且 還沒當過 轉接點的 vertex 給抓出來 
				  *  因為每個row的長度還是會變 ， 所以 還是 紀路  其  desitination_vertex_id 
				  *  這應該在我新方法中 ， 不能用 ， 因為我中間會加新的點進去 
				  *-----------------------------------------------------------------------------*/
				vector<int> non_infinite_used ; 
				for(unsigned int j = 0  ; j < for_update.size() ; j++  ){

					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					// 這邊有個新的問題 ， 因為我剛初是把全部的東西都放進來
					// 所以  會有 無限的這個情況  那我有改過了  ， 不會有這種
					// 情況了 ， 要不要拿掉呢 
					if( for_update[j].print_used() == false  ){
						non_infinite_used.push_back( j ) ; 
					}// end of 沒當過轉接的點 

					// 基本上 ， 只要 這個目地點給放入 ， 它在for_update 中的 index 都不會變了 
					// 我想呢  下面這個  ， 代表 ， 它跟目地點 本來就是相連的了  ， 很近  
					if( for_update[j].print_desitination_vertex_index() == tail_index ){
						// safe_count++ ;  
						tail_index_in_for_update = j ; 
					}

				}// end of  把  這個  row 中  ， 還能用來 轉接的點 給抓出來  

				// non_infinite_used.size() > 0 && tail_index_in_for_update == - 1
				// 這二塊 地方  ， 加入後的時間差很多  ， 但是時間變少的 ， 它的圖不好丫 
				if( non_infinite_used.size() > 0 ){

					for(unsigned int j = 0 ; j < non_infinite_used.size() ; j++ ){

						 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
						int update_row_index = 0 ; 
						int min_weight = 0 ; 
						
						min_weight = for_update[ non_infinite_used[j] ].print_path_weight() ;  
						update_row_index = for_update[ non_infinite_used[j] ].print_desitination_vertex_index()  ;	

						/*-----------------------------------------------------------------------------
						 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						 這裡呢 我想平行化  ， 要把下面的 ， for  index 弄成 signed  

						long int size_of_row_index = (long int)table[update_row_index].size()  ;  
						#pragma omp parallel for ordered 
						for(long int j = 0  ; j < size_of_row_index  ; j++  )

						case_one 是指 有找到 相同 目的點的vertex , 可能可以 簡化
						case_two 則是 都沒有找到  所以 push_back 

						用case_match 來代替好了 ， false 相當於上面的 case_two 
									   true  則是 case_one
						 *-----------------------------------------------------------------------------*/
						for(unsigned int k = 0  ; k < table[update_row_index].size()  ; k++  ){
							
							 bool case_match = false ; 
							 int  match_index_in_for_update = -1 ; 
							/*-----------------------------------------------------------------------------
							 *  這裡面 ， 應該還要一個for , 用來看 ， 這個轉接點 ， 能否 簡短
							 *  main_vertex 到其它點的距離(能簡短的 ， 當然是 有相同 目對點的) ，
							 *  如都遇不到相同目的點的 ， 代表 ， 這是個新的路 ， 可以放進來 
							 *  也就是說  有二種case 
							 *-----------------------------------------------------------------------------*/
							 for(unsigned int l = 0 ; l < for_update.size() ; l++ ){
							 	if( table[update_row_index][k].print_desitination_vertex_index() ==
								    for_update[l].print_desitination_vertex_index()              ){
									case_match = true ;  
									match_index_in_for_update = l ; 
									break ;
								}// 抓出  有相同 目地點的vertex位置 
							 }// end of for_update  

							if( case_match == true  ){
								// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
								// 值的比較不能含等號
								if( table[update_row_index][k].print_path_weight() + min_weight < 
								    for_update[ match_index_in_for_update ].print_path_weight()          	 ){
								    // 把轉接點放入　　，　更新weight ,
									 for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 

									 for_update[match_index_in_for_update].set_path_weight( 
									 	table[update_row_index][k].print_path_weight() + min_weight 
									 ) ; 

									 for_update[match_index_in_for_update].set_used( false ) ; 	
								}

								/*-----------------------------------------------------------------------------
								 *  這個case 在我的新方法中 ， 是不會出現的 
								// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
								else if( table[update_row_index][k].print_infinite() == false 			      && 
									 for_update[match_index_in_for_update].print_infinite() == true 	      )
								{
								    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
								    for_update[match_index_in_for_update].set_infinite( false ); 
								    for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 
								    for_update[match_index_in_for_update].set_path_weight( 
								    	table[update_row_index][k].print_path_weight() + min_weight 
									) ; 
								}
								 *  
								 *-----------------------------------------------------------------------------*/
							 
							}// end of case_match == true
							else if( case_match == false ){
								// 除了 ， 把這個 path_node 塞進去外 ， 還要多設定 ， 
								// 它是由 誰 轉接過來的  , 因為原本是到不了它的 
								for_update.push_back( table[update_row_index][k] ) ; 
								for_update[ for_update.size() - 1 ].set_trans_node( update_row_index )  ; 
							    	for_update[ for_update.size() - 1 ].set_path_weight( 
									table[update_row_index][k].print_path_weight() + min_weight 
								) ; 
							 
							}// end of case_match == false

						}// end of loop k

						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[ non_infinite_used[j] ].set_used(true);

					 
					}// end of scan all non_infinite_used.size() 

				}// end of non_infinite_used.size() > 0

				// 一旦 找到目的點 ， 但不能確定 它就是最短的 ， 所以  
				// 基本上呢 ， 由開始點 ， 往外search 的routing 如同是一個
				// 廣度search  , 等同一個同心圓向外擴張 ， 當safe_count == 1
				// 代表 第一次碰到目的點 ， 但不能確定是最短的 所以 
				// 我讓它有容錯的空間  ， 同心圓再擴張一點
				// 一個edge 的最長距離便是 ， 二個斜邊的相加 ， 
				// 最大的更新次數 ， 便是 >= safe_range 
				//tail_index_in_for_update != - 1  && safe_count >= safe_range
				if( tail_index_in_for_update != - 1  ){
					ok = false ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				// 沒有點可以拿來　當轉接　，　輸入的點跟obstacle數目　是３０，３０　４０，４０　時
				// 會有 中間沒路的情況　
				// 要連線的[0][0] , [0][1] , 分別是頭跟尾丫　, 第一個到達的點 ， 一定是最短的嗎 
				// if( tail_index_in_for_update != -1 && for_update[tail_index_in_for_update].print_infinite() == true  ){
					/*-----------------------------------------------------------------------------
					 * 				Check Code , May 9  
					cout << endl ; 
					cout << " No path can reach " << endl ;  
					// 用來看是第幾個edge出這樣的錯 
					cout << "variable i is : " << i << endl ; 
					cout << "how many vertex in vertex_need : " << vertex_need.size() << endl ; 
					//cout << "size in edge_need : " << edge_need.size() << endl ; 
					cout << "size in obstacle_crossed :  " << SG_edge[i].which_obstacle_cross_size() << endl ; 
					SG_edge[i].print_kruskal_edge_information() ; 
					 *-----------------------------------------------------------------------------*/

					/*-----------------------------------------------------------------------------
					 *  奇怪了 ， 為什麼 ， 轉接點全是－1 ， 它跟本沒有在做 東西的轉接丫 ， 難怪會找不到
					 *  路 
					cout << "here is debug info : " << endl ; 
					for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
						if(table[0][k].print_infinite() == true ){
							cout << " xx " ;   
						}
						else{
							cout << " " << table[0][k].print_path_weight() << " " ;	
						}	
					}
					cout << endl ; 

					cout << "here is trans_node info : " << endl ; 
					for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
						cout << " " << table[0][j].print_trans_node() << " " ; 
					}
					cout << endl ; 
					 *-----------------------------------------------------------------------------*/
					/*-----------------------------------------------------------------------------
					 *  
						cout << " edge_need 's information :" ;
						for(unsigned int j = 0  ; j < edge_need.size() ; j++  ){
							edge_need[j].print_kruskal_edge_information() ;
						}
					 *-----------------------------------------------------------------------------*/

				// }// end of for_update[tail_index].print_infinite() == true



			 // ok 的意思是說　，　找不到點可以　再拿來做轉接　，　所以　source 能到的點　，　應該都找完了
			 }while( ok == true );
			 // cout << "Updating sucessful !!" << endl ; 
 
			
			 /*-----------------------------------------------------------------------------
			  *  
			 source line 的內容　更新好了　，　接下來要抓出　，　中間轉接了那些點
			 再　把兩兩相鄰的vertex ，　其直線給抓出來　放入　Obstacle_Avoding_Kruskal_Tree 
			 source 的index 是　０　，　destination 的index 是　１
			  所以我判斷　有沒有路過去　，　便是看　index 1 的那個位置　0 能否過的去　

			  下面這塊  有點忘了 ， 先看看 再改吧  
			  這裡是要抓出  從起始點到 終點 ， 中間經過那些點 ， 
			  是一串 vertex_id 的串列 
			  這邊的 index 是要到 vertex_need 中用的
			  *-----------------------------------------------------------------------------*/

			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index_in_for_update ; 
			 // cout << "Find the chain of vertex " << endl ; 
			 do{
			 	// cout << "Do i get in 593 " << endl ; 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				// 丫 ， 我之道了 ， 有可能是 start_index 這裡面沒有東西
				// 所以就當了  , 我 tail_index_in_for_update 一定能抓到東西嗎
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}
			 	// 看來 這裡面 ， 要用個loop  , 來抓出 desitination_vertex_id , 是我們要的 那個path_node 
				// 再一個個 trace 回去 
			 	// 要確定裡面真是有東西　才行
				else {
					trans_order.push_back( for_update[start_index].print_trans_node() );	

					for(unsigned int j = 0 ; j < for_update.size() ; j++ ){
						if( for_update[j].print_desitination_vertex_index() == for_update[start_index].print_trans_node() ){
							start_index = j  ; 
							break ; 
						}
					}// end of loop j 
				} 

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 

			 /*-----------------------------------------------------------------------------
			  *  
			 cout << " how many size in edge_need : " << edge_need.size() << endl ;  
			 
			 cout << "here is trans_order info : " <<  endl  ; 
			 for(unsigned int i = 0 ; i < trans_order.size() ; i++  ){
			  	cout << " " << trans_order[i] << " " ; 
			 }
			cout << endl << endl ;  
			  *-----------------------------------------------------------------------------*/


			/*-----------------------------------------------------------------------------
				 好了， trans_order 內的東西， 我已經抓其了 ， 現在要把 edge 放入
				 Obstacle_Avoding_Kruskal_Tree 內 , 我想把兩兩的index 對的vertex 抓出來放入即可 
			 *  看來是要在這裡加了 ， 把要加入的edge 跟 obstacle_avoding_Kruskal 內的edge 
			 *  做比對， 只要裡面已經有這個edge 便不加入
			 *  下面的for 的j 上限是 －1 ， 因為 內容會用到 j + 1 
			 *-----------------------------------------------------------------------------*/
			int trans_order_signed = trans_order.size() - 1 ; 	
			for(int j = 0 ; j < trans_order_signed ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
				 }
			}// end of 加入已避開obstacle的edge 
			
			

		}// end of else , 這個區塊　我一次只改一個edge  , end of if
		       
	}// scan 過Kruskal_Tree 中所有的edge ，　並對false的做修改, a for loop  use variable i , so else_block cant use variable i 
	
	 // cout << endl << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 按  ， 我不懂哎 ， 我少了個loop 直接去取那個值  ， 怎麼還比較慢呢  
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = -1 ; 

	}// end of for

}// end of func build_obstacle_avoding_kruskal 


void obstacle_avoding_kruskal::build_obstacle_avoding_kruskal_imp( vector<edge> &SG_edge  , vector<edge> &obstacle_avoding_Kruskal  ,
							       vector<edge> &OASG , level_info &my_level_info ){

	// 09 27 我打算改一樣東西  ， 就是 ， edge 在 跟 obstacle 有相鄰時  ， 不改動它  ， 等到 rectilinear 再去弄 
	
	 // cout << "Coming obstacle_avoding_Kruskal func " << endl ; 
	/*-----------------------------------------------------------------------------
	 *  				step 1 ,2 
	 *  				check edge ok or not 
	 *  				理應是obstacle的數目比較少　，　所以
	 *  				裡面的loop 是用　my_level_info.complete_obstacle_information
	 *  				這裡  光是每個edge 都要跟obstacle 做比對 ， 怎麼受的了 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i  = 0 ; i < SG_edge.size() ; i++){
		
		/*-----------------------------------------------------------------------------
		 *  cout << "Here is edge information " << endl ;  
		SG_edge[i].print_kruskal_edge_information() ; 
		cout << "min_x = " ;
		cout << SG_edge[i].print_min_x() << endl ; 
		cout << "max_x = " ;
		cout << SG_edge[i].print_max_x() << endl ;
		cout << "min_y = " ;
		cout << SG_edge[i].print_min_y() << endl ; 
		cout << "max_y = " ;
		cout << SG_edge[i].print_max_y() << endl ;
		cout << "kruskal_edge_ok_or_not = "  <<  SG_edge[i].print_kruskal_edge_ok_not() << endl ;

		下面呢  我要重 寫  ， 用 obstacle_map 及 obstacle_bound_cross , 這二個參數  ， 來 減少下面
		的時間 
		 *-----------------------------------------------------------------------------*/
		for(int j = SG_edge[i].print_min_x() ; j <= SG_edge[i].print_max_x() ; j++ ){

			map<int,obstacle>::iterator it ;  
			int key = 0 ; 
			for(unsigned int k = 0 ; k < my_level_info.obstacle_cross[j].size() ; k++ ){
			 
				 key = my_level_info.obstacle_cross[j][k] ;
				 it = my_level_info.obstacle_map.find( key ) ; 
				 bool first_case = false , Second_case = false ; 

				 if( SG_edge[i].print_max_x() >= (*it).second.print_ld_corner_x() && 
				     SG_edge[i].print_min_x() <= (*it).second.print_ru_corner_x() ){
					first_case = true ;  
				 }
				 if( SG_edge[i].print_max_y() >= (*it).second.print_ld_corner_y() && 
				     SG_edge[i].print_min_y() <= (*it).second.print_ru_corner_y() ){
					Second_case = true ;  
				 }

				 /*-----------------------------------------------------------------------------
				  *  cout << "first_case = " << first_case << endl; 
				 cout << "Second_case = " << Second_case << endl ;
				 if( first_case == true || Second_case == true ){
					my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
				 }
				 cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
				  *-----------------------------------------------------------------------------*/
				  

				 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
				 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
				 //  就不用再往下比了  , 所以我多了個 break ; 
				 if( first_case == true && Second_case == true ){
					SG_edge[i].set_kruskal_edge_ok_not(false);
					SG_edge[i].add_which_obstacle_cross( (*it).second.print_obstacle_id() );
					break ; 
				 }
			}
		 
		}// 只 scan SG 有踫到的x 範圍  

		/*-----------------------------------------------------------------------------
		 *  
		for(unsigned int j  = 0 ; j < my_level_info.complete_obstacle_information.size()  ; j++){
		
		//	   我想到個好方法了　，　藉由　在Kruskal edge 中　，　抓出其最大的x,y
		//	 跟obstacle中　，　的左下角點比　，　即可　只要max_x 大於等於node1_x 
		//	 　就有機會相交了　，　還要再心對max_y >= node1_y ，如果二個都成立
		//	 那就一定有overlap , 但如果x 那關都過不了　就一定沒有overlap
		//	, 我還少弄一些判斷進去　那就是min x,y , 要小於左下的角點　，　不然edge
		//	　如果在右上角　　也會被算進去
		//		max_y >= ld_corner_y
		//		min_y <= ru_corner_y
		//		反之　x 的也是如此 
		//	 bool first_case = false , Second_case = false ; 

			 if( SG_edge[i].print_max_x() >= my_level_info.complete_obstacle_information[j].print_ld_corner_x() && 
			     SG_edge[i].print_min_x() <= my_level_info.complete_obstacle_information[j].print_ru_corner_x() ){
			 	first_case = true ;  
			 }
			 if( SG_edge[i].print_max_y() >= my_level_info.complete_obstacle_information[j].print_ld_corner_y() && 
			     SG_edge[i].print_min_y() <= my_level_info.complete_obstacle_information[j].print_ru_corner_y() ){
			 	Second_case = true ;  
			 }

			// cout << "first_case = " << first_case << endl; 
			// cout << "Second_case = " << Second_case << endl ;
			// if( first_case == true || Second_case == true ){
			//	my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
			// }
			// cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
			  

			 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
			 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
			 //  就不用再往下比了  , 所以我多了個 break ; 
			 if( first_case == true && Second_case == true ){
			 	SG_edge[i].set_kruskal_edge_ok_not(false);
				SG_edge[i].add_which_obstacle_cross( my_level_info.complete_obstacle_information[j].print_obstacle_id() );
				break ; 
			 }
			//cout << "kruskal_edge_ok_or_not = "  <<  SG_edge[i].print_kruskal_edge_ok_not() << endl ;
		}// end of for　j
		 *-----------------------------------------------------------------------------*/

	}// end of 抓出edge  是否要更動　，　以及overlap 的obstacle 有那些  
	

	

	/*-----------------------------------------------------------------------------
	 * 				Check
	 * 		every Kruskal edge ok or not 
	 * 		and which obstacles it cross  .    
	 *        cout << endl << endl << "Check Kruskal_Tree edge " << endl ; 
		for(unsigned int i = 0 ; i < SG_edge.size() ; i++  ){
		
		cout << "This is edge number " << i+1 << endl ; 
		// 奇怪了　，　只要是設定false 那就一定有值會出來才對丫  
		SG_edge[i].print_kruskal_edge_information() ; 
	}

	 *-----------------------------------------------------------------------------*/
	 
	/*-----------------------------------------------------------------------------
	 *  				Step 3,4
	 *  		因為我已經抓出　有cross 的obstacle了　，　要抓其四個端點
	 *  		很簡單　，　接下來呢，　就建個新的vector 用來存
	 *  		obstacle_avoding_by_kruskal_Tree , 
	 *  		為true 的edge 就直接放入　，　false 的便修改後　，再放入
	 *
	 *-----------------------------------------------------------------------------*/
	// 下面這二個  好像後面還會用到的樣子
	// 沒有 只有  第一個 要記得放外面就好了
	// vector<edge> obstacle_avoding_Kruskal ; 	
	// 下面建了個 vertex_map , 是用來  ， 給vertex抓出它的index 
	vector<vertex> vertex_need ; 
	vector<vertex> only_pin    ; 

	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 因為我已經寫好了 vertex 的copy constructor 所以vertex 可以直接給了  	
			// 這裡是存  ， 在plane 內 ， 所有的pin 及 obstalce corner 所組成的點 
			vertex_need.push_back( my_level_info.array_vertex[j] );	
			vertex_need[j].set_id( j ) ; 
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = j ; 
			if( my_level_info.array_vertex[j].Is_pin_or_not() == true  ){
				// 只要connect_graph中 ， 所以 pin 所在的subtree id 都一樣  那代表 是
				// 在同一個connect_graph 了  
				only_pin.push_back( my_level_info.array_vertex[j] ) ;  
			}

	}// end of for
	
	/*-----------------------------------------------------------------------------
	 *  
	// vector<vertex> vertex_seperate[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < vertex_need.size() ; i++ ){
		vertex_need[i].set_id( i ) ; 
		int x = vertex_need[i].print_x_position() ; 
		int y = vertex_need[i].print_y_position() ; 
		my_level_info.vertex_id[x][y] = i ; 

		// vertex_seperate[ vertex_need[i].print_x_position() ].push_back( vertex_need[i] ) ; 	 
	}
	 *-----------------------------------------------------------------------------*/

	 // 這裡把表的初值給建好了
	 // 應該把　path_node 換成　table_node 比較好　
	 // 因為要建相鄰 串列  ， 所以要有個 一維的點集合 ， 後面串 接它相鄰的人有誰  
	 vector<path_node> table[ vertex_need.size() ];			 

	 /*-----------------------------------------------------------------------------
	 // 先把表的樣子給先建起來　
	 // 這個表table  我想建起 ， 這個 vertex 跟 那些vertex 是相鄰的 就好了  
	 // 我只要比對 這是那一個row 的 ， 就可以到 vertex_need 抓到這個點的資訊了 
	 for(unsigned int j = 0  ; j <  vertex_need.size() ; j++ ){
		for(unsigned int k = 0  ; k <  vertex_need.size() ; k++ ){
			table[j].push_back( path_node() ); 
		} 
	 }
	  *-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
	 *  				下面這塊　是建好OAST 的核心
	 *  				要用的edge　我都抓好了　，　
	 *  				接下來便是要抓shortest path 
	 *  	先建出一張表來，　各個vertex 到別的點的weight , 要建幾乘幾的表，
	 *  	可由vertex_need 去看　，　再由edge_need 去一個個掃　，　來建立
	 *  	weight , 那　無法到達的 無限要怎麼代表它呢　
	 *  	, 我建了二個data structure , node_need 跟　edge_need 
	 *  	,node_need中的第０跟１　，　理應是　我要的source跟desitination 
	 *  	表的index 順序，　我想就跟node_need 一樣　，　
	 *  	而表的內容　，　就掃過edge_need , 來一個個建　，　一個edge_need
	 *  	要建　二個位置的值　像(3,1) (1,3)　這樣　
	 *-----------------------------------------------------------------------------*/
	 // 接下來　　，　把值弄進去　，　每個edge 都跟vertex_need 掃過一遍
	 // 抓出edge  二端點的node index ,  這裡建相鄰串列的地方 也是花太多時間了

	for(unsigned int j = 0  ; j < OASG.size() ; j++  ){

		// 內層的for  , 要抓出兩端點的index 
		int index_one = -1 , index_two = -1  ; 

		int x = OASG[j].print_node1_x() , y = OASG[j].print_node1_y()  ; 
		index_one = my_level_info.vertex_id[x][y]  ; 

		int xx = OASG[j].print_node2_x() , yy = OASG[j].print_node2_y()  ; 
		index_two = my_level_info.vertex_id[xx][yy]  ; 
		/*-----------------------------------------------------------------------------
		// 下面這個loop  太花時間了 ， 我打把它寫成map 去抓  , 水拉   ， 換成下面的方法 ， 有變快了
		// 應該是因為  外層是 OASG  太大的關係 
		for(unsigned int k = 0 ; k < vertex_seperate[ OASG[j].print_node1_x() ].size() ; k++ ){

			if( OASG[j].print_node1_y() == vertex_seperate[ OASG[j].print_node1_x() ][k].print_y_position() ){
				// 為什麼 ， 不能在這裡加break ; 
				index_one = vertex_seperate[ OASG[j].print_node1_x() ][k].get_id() ;  
			}

		}// end of for

		for(unsigned int k = 0 ; k < vertex_seperate[ OASG[j].print_node2_x() ].size() ; k++ ){

			if( OASG[j].print_node2_y() == vertex_seperate[ OASG[j].print_node2_x() ][k].print_y_position() ){
				index_two =  vertex_seperate[ OASG[j].print_node2_x() ][k].get_id() ;  
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  
		for(unsigned int k = 0  ;  k < vertex_need.size() ; k++  ){
			// 有的vertex 會對不到
			if(OASG[j].print_node1_x() == vertex_need[k].print_x_position() &&
			   OASG[j].print_node1_y() == vertex_need[k].print_y_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_one = k ;  
			}
			if(OASG[j].print_node2_x() == vertex_need[k].print_x_position() &&
			   OASG[j].print_node2_y() == vertex_need[k].print_y_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_two = k ;  
			}
		}// 　抓好二個端點的index 了
		 *-----------------------------------------------------------------------------*/

		// 要把值給送進table 
		if( index_one != -1 && index_two != -1 ){
			/*-----------------------------------------------------------------------------
			 *  
			 這步做完 ， 應該  就把各個相鄰的東西 ， 給抓出來了吧  
			 之前 我是藉由 ， row 跟 column 來判斷 ， 這個path_node 是
			 那紀路那兩個 vertex 的關係  ， 但現在不能用column 來抓
			 了  ， 所以要在path_node 中 ， 多加一項資訊 ， 叫 
			 desitination_vertex_id  ,  奇怪了 ， 這個老是不能班過去  
			 *-----------------------------------------------------------------------------*/
			table[index_one].push_back( path_node( index_one ,  OASG[j].print_weight() , index_two ) ) ; 
			table[index_two].push_back( path_node( index_two ,  OASG[j].print_weight() , index_one ) ) ; 

			/*-----------------------------------------------------------------------------
			table[index_one][index_two].set_infinite(false) ;
			table[index_one][index_two].set_path_weight( OASG[j].print_weight() );
			table[index_two][index_one].set_infinite(false) ;
			table[index_two][index_one].set_path_weight( OASG[j].print_weight() );	 
			 *-----------------------------------------------------------------------------*/

		}// 建起表內  各個格子的內容 
		
		
	}// end of 抓二端點的　index , 且埴滿table內的值　


	/*-----------------------------------------------------------------------------
	 * 			check code , May 9 
	cout << endl << "here become single source all desitination : " << endl ;  
	// 在這之後的東西 就沒有跑出來了 ， 所以 我想 理應是某個func 被我改了 
	// 但 effect 到這裡  
	// 改善最多的  應該是因為 我下面放的這個吧   ,  只要放對地方  ， 改善很多哎 
	 *-----------------------------------------------------------------------------*/
	int SG_edge_change = SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	for(int i = 0  ; i < SG_edge_change ; i++  ){

		/*-----------------------------------------------------------------------------
		 * 			check code , May 9 
		cout << SG_edge[i].print_kruskal_edge_ok_not() << endl ; 
		 *-----------------------------------------------------------------------------*/
		if( SG_edge[i].print_kruskal_edge_ok_not() == true  ){
			obstacle_avoding_Kruskal.push_back( SG_edge[i] ) ; 
		}
		else{
			
			/*-----------------------------------------------------------------------------
			 *  我要把edge_need 放到上面去做 ， 這裡變成 ， 抓edge兩端點的index出來即可
			 *  這裡抓的index 是指在 vertex_need 裡的index 
			 *  跟老師討論過 ，  那 safe_range 好像不太必要哎 , safe_range = 3
			 *  下面這塊真的是很麻煩 ， 每次都要比到完 
			 *-----------------------------------------------------------------------------*/
			int head_index = -1 , tail_index = -1  ; 	

			int  x = SG_edge[i].print_node1_x() , y  = SG_edge[i].print_node1_y()  ; 
			head_index = my_level_info.vertex_id[x][y] ; 
			int xx = SG_edge[i].print_node2_x() , yy = SG_edge[i].print_node2_y()  ; 
			tail_index = my_level_info.vertex_id[xx][yy] ; 
			/*-----------------------------------------------------------------------------
			 *  
			// 看來要從 ， 這裡著手 ， 會比較正確了   , 這裡是起始點 ， 所以 目的點如果 已經到過了  
			// 那就要  ， 從已加入的edge中  ， 找到  ， 離自已最近的點 ， 去做 routing 了 
			if( my_level_info.already_in_OAST[x][y] == true ){
				// 找一個離起始點最近的 ， 來當 新的目的點 		 
				int temp_min_weight_value  = 0 ; 
				int min_weight_value  = abs( obstacle_avoding_Kruskal[0].print_node1_x() - xx) + 
							abs( obstacle_avoding_Kruskal[0].print_node1_y() - yy) ; 
				for(unsigned int j = 0 ; j < obstacle_avoding_Kruskal.size() ; j++  ){

					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node1_x() - xx) + 
								 abs( obstacle_avoding_Kruskal[j].print_node1_y() - yy) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						head_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node1_x()]
										    [obstacle_avoding_Kruskal[j].print_node1_y()] ; 
					}
					
					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node2_x() - xx) + 
								 abs( obstacle_avoding_Kruskal[j].print_node2_y() - yy) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						head_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node2_x()]
										    [obstacle_avoding_Kruskal[j].print_node2_y()] ; 
					}

				}// loop j 
			}
			else if( my_level_info.already_in_OAST[x][y] == false ){
				head_index = my_level_info.vertex_id[x][y] ; 
			}



			// 看來要從 ， 這裡著手 ， 會比較正確了   , 這裡是起始點 ， 所以 目的點如果 已經到過了  
			// 那就要  ， 從已加入的edge中  ， 找到  ， 離自已最近的點 ， 去做 routing 了 
			if( my_level_info.already_in_OAST[xx][yy] == true ){
				// 找一個離起始點最近的 ， 來當 新的目的點 		 
				int temp_min_weight_value  = 0 ; 
				int min_weight_value  = abs( obstacle_avoding_Kruskal[0].print_node1_x() - x) + 
							abs( obstacle_avoding_Kruskal[0].print_node1_y() - y) ; 
				for(unsigned int j = 0 ; j < obstacle_avoding_Kruskal.size() ; j++  ){

					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node1_x() - x) + 
								 abs( obstacle_avoding_Kruskal[j].print_node1_y() - y) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						tail_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node1_x()]
										    [obstacle_avoding_Kruskal[j].print_node1_y()] ; 
					}
					
					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node2_x() - x) + 
								 abs( obstacle_avoding_Kruskal[j].print_node2_y() - y) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						tail_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node2_x()]
										    [obstacle_avoding_Kruskal[j].print_node2_y()] ; 
					}

				}// loop j 
			}
			else if( my_level_info.already_in_OAST[xx][yy] == false ){
				tail_index = my_level_info.vertex_id[xx][yy] ; 
			}
			 *-----------------------------------------------------------------------------*/

			/*-----------------------------------------------------------------------------
			for(unsigned int j = 0 ; j < vertex_seperate[ SG_edge[i].print_node1_x() ].size() ; j++ ){

				if( SG_edge[i].print_node1_y() == vertex_seperate[ SG_edge[i].print_node1_x() ][j].print_y_position() ){
					head_index =  vertex_seperate[ SG_edge[i].print_node1_x() ][j].get_id() ;  
					// 為什麼 ， 不能在這裡加break ; 
				}

			}// end of for

			for(unsigned int j = 0 ; j < vertex_seperate[ SG_edge[i].print_node2_x() ].size() ; j++ ){

				if( SG_edge[i].print_node2_y() == vertex_seperate[ SG_edge[i].print_node2_x() ][j].print_y_position() ){
					tail_index = vertex_seperate[ SG_edge[i].print_node2_x() ][j].get_id() ;  
				}

			}// end of for
			 *-----------------------------------------------------------------------------*/

			//cout << "my_level_info.total = " << my_level_info.total << ",  vertex_need.size " << vertex_need.size() << endl ;   

			// 這裡要寫些code來看資訊 ， 用來抓bug 的 ， 要印出 ， 此時的i 是多少
			// 以及這個要被改的edge  資訊 ， 
			//cout << " i = " << i << endl ; 
			//SG_edge[i].print_kruskal_edge_information() ; 
			
			/*-----------------------------------------------------------------------------
			 *  
			// 這裡 印出個花費相鄰矩陣來看看 好了  
			cout << "Here print out weight table : " << i << endl ; 
			for(unsigned int j = 0 ; j < vertex_need.size() ; j++ ){
			 	for(unsigned int k = 0 ; k < vertex_need.size() ; k++ ){

					if( j == k ){
						table[j][j].set_infinite(false) ;
						table[j][j].set_path_weight( 0 );	 
					}

					if(table[j][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[j][k].print_path_weight() << " " ;	
					}

				}// inner loop j 
				cout << endl << endl ;; 
			}// end of 印出矩陣來看看
			 *-----------------------------------------------------------------------------*/
	
			/*-----------------------------------------------------------------------------
			 *  表建好後　，　接下來便是執行　one source all destination 的algorithm 
			先抓source 行中　weight  最小的　，　及　此edge node2是誰
			把weight加到　node2那行　再跟 source 行比對，　　如果值有變小　，　或
			原本true 的　變false ，　也可把值納入　
			抓出shortest path 中間要的轉接點後　，　可以用　之前的方法　，　從edge_need 中
			抓出　其edge_need 含這些點集的edge 即可
			還是說　用個　bool 紀路這個點　是否當過轉接點,  轉接點 只會有一個 ， 初值我設成 －1 
			只要是 source  能直接到達的 ， 都設成 －1 ， 
			原先的表是拿來查的 ， 不能直接休改 ， 
			 *-----------------------------------------------------------------------------*/
			 
			 /*-----------------------------------------------------------------------------
			  *  
			  因為table的直是用來查的 不是用來更新的 ， 不然  會出點錯誤 ，
			  所以這裡我要建個一行path_node 來 ， 存原始行的data , 
			  等路抓出來以後 ， 再用這個還原回去 
			  *-----------------------------------------------------------------------------*/

			 // 這是 要被更新的那一行
			 vector<path_node> for_update;			 
			// 我只要把上面的for_update變成 for_update[2] , 就可以一個從頭處理， 
			// 一個從尾巴了  , 好  這裡應該不用動它  , for_update 是紀著
			// 要被 更新  的那一row 

			for(unsigned int j = 0  ; j <  table[head_index].size() ; j++ ){
				for_update.push_back( table[head_index][j] ); 
			}// end of 抓出 我要更新的那個row 
			// cout << "for_update.size() = " << for_update.size() << endl ; 


			/*-----------------------------------------------------------------------------
			  如果抓不到一個可以轉接的點　，　便跳出去了　
			  這個 do while 把整個表給更新好了　，　接下來便是看我怎麼抓中間的edge了
			  感覺整個最花時閶就是 下面這塊了  ， 因為 有二個do while 的loop 
			  tail_index_in_for_update , 因為一開始 裡面沒有 我們要到的點的index 
			  所以最後有把要的點放進來的時侯 ， 要記住它的index  
			 *-----------------------------------------------------------------------------*/
			 bool ok = true ; 
			 int tail_index_in_for_update = -1 ; 
			 // int safe_count = 0 ; 
			 // cout << endl << "Ready coming update routing table on SG_edge : " << i << endl ; 
			 do{
			 	
				 /*-----------------------------------------------------------------------------
				  *  下面這一小塊  ， 是要把 之間有路  ， 且 還沒當過 轉接點的 vertex 給抓出來 
				  *  因為每個row的長度還是會變 ， 所以 還是 紀路  其  desitination_vertex_id 
				  *  這應該在我新方法中 ， 不能用 ， 因為我中間會加新的點進去 
				  *-----------------------------------------------------------------------------*/
				vector<int> non_infinite_used ; 
				for(unsigned int j = 0  ; j < for_update.size() ; j++  ){

					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					// 這邊有個新的問題 ， 因為我剛初是把全部的東西都放進來
					// 所以  會有 無限的這個情況  那我有改過了  ， 不會有這種
					// 情況了 ， 要不要拿掉呢 
					if( for_update[j].print_used() == false  ){
						non_infinite_used.push_back( j ) ; 
					}// end of 沒當過轉接的點 

					// 基本上 ， 只要 這個目地點給放入 ， 它在for_update 中的 index 都不會變了 
					// 我想呢  下面這個  ， 代表 ， 它跟目地點 本來就是相連的了  ， 很近  
					if( for_update[j].print_desitination_vertex_index() == tail_index ){
						// safe_count++ ;  
						tail_index_in_for_update = j ; 
					}

				}// end of  把  這個  row 中  ， 還能用來 轉接的點 給抓出來  

				// non_infinite_used.size() > 0 && tail_index_in_for_update == - 1
				// 這二塊 地方  ， 加入後的時間差很多  ， 但是時間變少的 ， 它的圖不好丫 
				if( non_infinite_used.size() > 0 ){

					for(unsigned int j = 0 ; j < non_infinite_used.size() ; j++ ){

						 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
						int update_row_index = 0 ; 
						int min_weight = 0 ; 
						
						min_weight = for_update[ non_infinite_used[j] ].print_path_weight() ;  
						update_row_index = for_update[ non_infinite_used[j] ].print_desitination_vertex_index()  ;	

						/*-----------------------------------------------------------------------------
						 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						 這裡呢 我想平行化  ， 要把下面的 ， for  index 弄成 signed  

						long int size_of_row_index = (long int)table[update_row_index].size()  ;  
						#pragma omp parallel for ordered 
						for(long int j = 0  ; j < size_of_row_index  ; j++  )

						case_one 是指 有找到 相同 目的點的vertex , 可能可以 簡化
						case_two 則是 都沒有找到  所以 push_back 

						用case_match 來代替好了 ， false 相當於上面的 case_two 
									   true  則是 case_one
						 *-----------------------------------------------------------------------------*/
						for(unsigned int k = 0  ; k < table[update_row_index].size()  ; k++  ){
							
							 bool case_match = false ; 
							 int  match_index_in_for_update = -1 ; 
							/*-----------------------------------------------------------------------------
							 *  這裡面 ， 應該還要一個for , 用來看 ， 這個轉接點 ， 能否 簡短
							 *  main_vertex 到其它點的距離(能簡短的 ， 當然是 有相同 目對點的) ，
							 *  如都遇不到相同目的點的 ， 代表 ， 這是個新的路 ， 可以放進來 
							 *  也就是說  有二種case 
							 *-----------------------------------------------------------------------------*/
							 for(unsigned int l = 0 ; l < for_update.size() ; l++ ){
							 	if( table[update_row_index][k].print_desitination_vertex_index() ==
								    for_update[l].print_desitination_vertex_index()              ){
									case_match = true ;  
									match_index_in_for_update = l ; 
									break ;
								}// 抓出  有相同 目地點的vertex位置 
							 }// end of for_update  

							if( case_match == true  ){
								// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
								// 值的比較不能含等號
								if( table[update_row_index][k].print_path_weight() + min_weight < 
								    for_update[ match_index_in_for_update ].print_path_weight()          	 ){
								    // 把轉接點放入　　，　更新weight ,
									 for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 

									 for_update[match_index_in_for_update].set_path_weight( 
									 	table[update_row_index][k].print_path_weight() + min_weight 
									 ) ; 

									 for_update[match_index_in_for_update].set_used( false ) ; 	
								}

								/*-----------------------------------------------------------------------------
								 *  這個case 在我的新方法中 ， 是不會出現的 
								// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
								else if( table[update_row_index][k].print_infinite() == false 			      && 
									 for_update[match_index_in_for_update].print_infinite() == true 	      )
								{
								    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
								    for_update[match_index_in_for_update].set_infinite( false ); 
								    for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 
								    for_update[match_index_in_for_update].set_path_weight( 
								    	table[update_row_index][k].print_path_weight() + min_weight 
									) ; 
								}
								 *  
								 *-----------------------------------------------------------------------------*/
							 
							}// end of case_match == true
							else if( case_match == false ){
								// 除了 ， 把這個 path_node 塞進去外 ， 還要多設定 ， 
								// 它是由 誰 轉接過來的  , 因為原本是到不了它的 
								for_update.push_back( table[update_row_index][k] ) ; 
								for_update[ for_update.size() - 1 ].set_trans_node( update_row_index )  ; 
							    	for_update[ for_update.size() - 1 ].set_path_weight( 
									table[update_row_index][k].print_path_weight() + min_weight 
								) ; 
							 
							}// end of case_match == false

						}// end of loop k

						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[ non_infinite_used[j] ].set_used(true);

					 
					}// end of scan all non_infinite_used.size() 

				}// end of non_infinite_used.size() > 0

				// 一旦 找到目的點 ， 但不能確定 它就是最短的 ， 所以  
				// 基本上呢 ， 由開始點 ， 往外search 的routing 如同是一個
				// 廣度search  , 等同一個同心圓向外擴張 ， 當safe_count == 1
				// 代表 第一次碰到目的點 ， 但不能確定是最短的 所以 
				// 我讓它有容錯的空間  ， 同心圓再擴張一點
				// 一個edge 的最長距離便是 ， 二個斜邊的相加 ， 
				// 最大的更新次數 ， 便是 >= safe_range 
				//tail_index_in_for_update != - 1  && safe_count >= safe_range
				if( tail_index_in_for_update != - 1  ){
					ok = false ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				// 沒有點可以拿來　當轉接　，　輸入的點跟obstacle數目　是３０，３０　４０，４０　時
				// 會有 中間沒路的情況　
				// 要連線的[0][0] , [0][1] , 分別是頭跟尾丫　, 第一個到達的點 ， 一定是最短的嗎 
				// if( tail_index_in_for_update != -1 && for_update[tail_index_in_for_update].print_infinite() == true  ){
					/*-----------------------------------------------------------------------------
					 * 				Check Code , May 9  
					cout << endl ; 
					cout << " No path can reach " << endl ;  
					// 用來看是第幾個edge出這樣的錯 
					cout << "variable i is : " << i << endl ; 
					cout << "how many vertex in vertex_need : " << vertex_need.size() << endl ; 
					//cout << "size in edge_need : " << edge_need.size() << endl ; 
					cout << "size in obstacle_crossed :  " << SG_edge[i].which_obstacle_cross_size() << endl ; 
					SG_edge[i].print_kruskal_edge_information() ; 
					 *-----------------------------------------------------------------------------*/

					/*-----------------------------------------------------------------------------
					 *  奇怪了 ， 為什麼 ， 轉接點全是－1 ， 它跟本沒有在做 東西的轉接丫 ， 難怪會找不到
					 *  路 
					cout << "here is debug info : " << endl ; 
					for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
						if(table[0][k].print_infinite() == true ){
							cout << " xx " ;   
						}
						else{
							cout << " " << table[0][k].print_path_weight() << " " ;	
						}	
					}
					cout << endl ; 

					cout << "here is trans_node info : " << endl ; 
					for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
						cout << " " << table[0][j].print_trans_node() << " " ; 
					}
					cout << endl ; 
					 *-----------------------------------------------------------------------------*/
					/*-----------------------------------------------------------------------------
					 *  
						cout << " edge_need 's information :" ;
						for(unsigned int j = 0  ; j < edge_need.size() ; j++  ){
							edge_need[j].print_kruskal_edge_information() ;
						}
					 *-----------------------------------------------------------------------------*/

				// }// end of for_update[tail_index].print_infinite() == true



			 // ok 的意思是說　，　找不到點可以　再拿來做轉接　，　所以　source 能到的點　，　應該都找完了
			 }while( ok == true );
			 // cout << "Updating sucessful !!" << endl ; 
 
			
			 /*-----------------------------------------------------------------------------
			  *  
			 source line 的內容　更新好了　，　接下來要抓出　，　中間轉接了那些點
			 再　把兩兩相鄰的vertex ，　其直線給抓出來　放入　Obstacle_Avoding_Kruskal_Tree 
			 source 的index 是　０　，　destination 的index 是　１
			  所以我判斷　有沒有路過去　，　便是看　index 1 的那個位置　0 能否過的去　

			  下面這塊  有點忘了 ， 先看看 再改吧  
			  這裡是要抓出  從起始點到 終點 ， 中間經過那些點 ， 
			  是一串 vertex_id 的串列 
			  這邊的 index 是要到 vertex_need 中用的
			  *-----------------------------------------------------------------------------*/

			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index_in_for_update ; 
			 // cout << "Find the chain of vertex " << endl ; 
			 do{
			 	// cout << "Do i get in 593 " << endl ; 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				// 丫 ， 我之道了 ， 有可能是 start_index 這裡面沒有東西
				// 所以就當了  , 我 tail_index_in_for_update 一定能抓到東西嗎
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}
			 	// 看來 這裡面 ， 要用個loop  , 來抓出 desitination_vertex_id , 是我們要的 那個path_node 
				// 再一個個 trace 回去 
			 	// 要確定裡面真是有東西　才行
				else {
					trans_order.push_back( for_update[start_index].print_trans_node() );	

					for(unsigned int j = 0 ; j < for_update.size() ; j++ ){
						if( for_update[j].print_desitination_vertex_index() == for_update[start_index].print_trans_node() ){
							start_index = j  ; 
							break ; 
						}
					}// end of loop j 
				} 

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 

			 /*-----------------------------------------------------------------------------
			  *  cout << " how many size in edge_need : " << edge_need.size() << endl ;  
			 
			 cout << "here is trans_order info : " <<  endl  ; 
			 for(unsigned int i = 0 ; i < trans_order.size() ; i++  ){
			  	cout << " " << trans_order[i] << " " ; 
			 }
			cout << endl << endl ;  
			bool start_position = false ; 
			for(unsigned int j = 0 ; j < trans_order.size() - 1 ; j++ ){

				// bool insert_ok_not = false ; 
				// 當遇到第一個 這樣的case ,  那個edge 要加 ， 之後的 就不要了  
				int node1_x = 	vertex_need[ trans_order[j] ].print_x_position() ; 
				int node1_y = 	vertex_need[ trans_order[j] ].print_y_position() ; 

				int node2_x = 	vertex_need[ trans_order[j + 1] ].print_x_position() ; 
				int node2_y = 	vertex_need[ trans_order[j + 1] ].print_y_position() ; 

				while( start_position == false && j < trans_order.size() - 1 ){
					int temp1_x = 	vertex_need[ trans_order[j] ].print_x_position() ; 
					int temp1_y = 	vertex_need[ trans_order[j] ].print_y_position() ; 

					int temp2_x = 	vertex_need[ trans_order[j + 1] ].print_x_position() ; 
					int temp2_y = 	vertex_need[ trans_order[j + 1] ].print_y_position() ; 

					if( 	 my_level_info.already_in_OAST[temp1_x][ temp1_y]  == true   && 
					    	 my_level_info.already_in_OAST[temp2_x][ temp2_y]  == false  ){
						start_position = true ; 	 
					}
					else {
						obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
											  vertex_need[ trans_order[j] ].print_y_position() ,
											  vertex_need[ trans_order[j + 1] ].print_x_position() ,
											  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
						my_level_info.already_in_OAST[temp1_x][ temp1_y] =  true ; 
						my_level_info.already_in_OAST[temp2_x][ temp2_y] =  true ; 
						j++ ;	
					}


				}// end of while 

				// true 就可以從這裡  ， 開始加 edge 了 
				if( start_position == true  ){
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 


					my_level_info.already_in_OAST[node1_x][ node1_y] =  true ; 
					my_level_info.already_in_OAST[node2_x][ node2_y] =  true ; 
				}// end of  start_position == true
			}// end of loop j 
			  *-----------------------------------------------------------------------------*/


			int trans_order_signed = trans_order.size() - 1 ; 	
			for(int j = 0 ; j < trans_order_signed ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
				 	// 如果  ， 前後二個pin都在同個subtree  , 那代表連通  ， 只要全部的pin點都連通 ， 
					// 那就不用 再加edge了    , 如果 全部的 pin點都 連通了   ， 就放個break 出去 
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
					/*-----------------------------------------------------------------------------
					my_level_info.already_in_OAST[ vertex_need[ trans_order[j] ].print_x_position() ]
								     [ vertex_need[ trans_order[j] ].print_y_position() ] =  true ; 
					my_level_info.already_in_OAST[vertex_need[ trans_order[j + 1] ].print_x_position()]
								     [vertex_need[ trans_order[j + 1] ].print_y_position()] =  true ; 
					 *-----------------------------------------------------------------------------*/
				 }
			}// end of 加入已避開obstacle的edge 

			/*-----------------------------------------------------------------------------
				 好了， trans_order 內的東西， 我已經抓其了 ， 現在要把 edge 放入
				 Obstacle_Avoding_Kruskal_Tree 內 , 我想把兩兩的index 對的vertex 抓出來放入即可 
			 *  看來是要在這裡加了 ， 把要加入的edge 跟 obstacle_avoding_Kruskal 內的edge 
			 *  做比對， 只要裡面已經有這個edge 便不加入
			 *  下面的for 的j 上限是 －1 ， 因為 內容會用到 j + 1  
			int trans_order_signed = trans_order.size() - 1 ; 	
			for(int j = 0 ; j < trans_order_signed ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
					my_level_info.already_in_OAST[ vertex_need[ trans_order[j] ].print_x_position() ]
								     [ vertex_need[ trans_order[j] ].print_y_position() ] =  true ; 
					my_level_info.already_in_OAST[vertex_need[ trans_order[j + 1] ].print_x_position()]
								     [vertex_need[ trans_order[j + 1] ].print_y_position()] =  true ; 
				 }
			}// end of 加入已避開obstacle的edge 
			 *-----------------------------------------------------------------------------*/


			
			

		}// end of else , 這個區塊　我一次只改一個edge  , end of if
		       
	}// scan 過Kruskal_Tree 中所有的edge ，　並對false的做修改, a for loop  use variable i , so else_block cant use variable i 
	

	// cout << "enter the solve cycle problem  "  << endl ;  少了 sort  會有差嗎  

	// sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight_decreasing );	 
	// 所在的index 便是它的編號  ,  group的編號  ， 這樣就之道要往那個箱子放了  
	// count  用來記  要放第幾個箱子   , count 指向 空箱 
	vector<edge> seperate_group[ obstacle_avoding_Kruskal.size() ] ; 
	int count = 0 ; 
	// int index_delete = -1 ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){

		bool delete_or_not = false  ;
		// cout << "i = "	<< i << endl ; 
		
		// cout << " Do i get in "  << endl ;  , 我這樣 ， 沒有把後面的給刪掉 
		
		 // bool connect_graph_complete = false ;  
		int node1_x = 	obstacle_avoding_Kruskal[ i ].print_node1_x() ; 
		int node1_y = 	obstacle_avoding_Kruskal[ i ].print_node1_y() ; 

		int node2_x = 	obstacle_avoding_Kruskal[ i ].print_node2_x() ; 
		int node2_y = 	obstacle_avoding_Kruskal[ i ].print_node2_y() ; 
		// 先判斷那二個點 ， 有沒有其它的subtree   , 沒有的話  ， 就依 count 值 放到 
		// 指定的箱子裡面 
		if( my_level_info.connected_graph[node1_x][node1_y] == -1 &&  my_level_info.connected_graph[node2_x][node2_y] == -1 ){
			// 給值 ， 再放到  ， vector 中   
			my_level_info.connected_graph[node1_x][node1_y] = count ; 
			my_level_info.connected_graph[node2_x][node2_y] = count ; 
			// 之後是要利用 ， 下面二個點 ， 去改上面的id 
			seperate_group[count].push_back( obstacle_avoding_Kruskal[i] ) ; 
			count++ ; 
			//cout << "line   "<< 1771  << endl ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] >= 0 &&  my_level_info.connected_graph[node2_x][node2_y] == -1 ){
			// 給值 ， 再放到  ， vector 中   
			my_level_info.connected_graph[node2_x][node2_y] = my_level_info.connected_graph[node1_x][node1_y] ; 
			// 下面這  好像不是放在 count  所在的箱子 ， 而是放在  >= 0 的那個箱子才對 
			seperate_group[ my_level_info.connected_graph[node1_x][node1_y] ].push_back( obstacle_avoding_Kruskal[i] ) ; 
			// cout << "line   "<< 1778  << endl ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] == -1 &&  my_level_info.connected_graph[node2_x][node2_y] >= 0 ){
			// 給值 ， 再放到  ， vector 中   
			my_level_info.connected_graph[node1_x][node1_y] = my_level_info.connected_graph[node2_x][node2_y]; 
			seperate_group[  my_level_info.connected_graph[node2_x][node2_y] ].push_back(  obstacle_avoding_Kruskal[i] ) ; 
			// cout << "line   "<< 1784 << endl ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] >= 0 &&  my_level_info.connected_graph[node2_x][node2_y] >= 0 && 
			 my_level_info.connected_graph[node1_x][node1_y]  == my_level_info.connected_graph[node2_x][node2_y]           ){
		 
			// cout << "line   "<< 1844  << endl ; 
		 	// 這裡  要刪掉  那個 弄出cycle的edge 
			delete_or_not = true ; 
			obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
			i-- ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] >= 0 &&  my_level_info.connected_graph[node2_x][node2_y] >= 0 && 
			 my_level_info.connected_graph[node1_x][node1_y]  != my_level_info.connected_graph[node2_x][node2_y] 	   ){

			 // 為什麼進來這個  會出錯呢  
			// cout << "line   "<< 1788 << endl ; 
			
			// 下面二個loop 是依大小 ， 來看 ， 是誰要合併誰  
			// 由下面一行可以之道 ， node2 的 被蓋掉  ， 也就是說  ， node1所在的 subtree  , 量比較大  
			int which_group = my_level_info.connected_graph[node2_x][node2_y] ; 
			int des_group   = my_level_info.connected_graph[node1_x][node1_y] ; 

			my_level_info.connected_graph[node2_x][node2_y] = my_level_info.connected_graph[node1_x][node1_y] ; 
			seperate_group[  des_group ].push_back(  obstacle_avoding_Kruskal[i] ) ; 

			// cout <<  "seperate_group[ which_group ].size() = " << seperate_group[ which_group ].size() << endl ; 
			for(unsigned int k = 0 ; k < seperate_group[ which_group ].size() ; k++ ){
				// cout << " j  =" << j << endl ; 
				// 班過去 node1 那個箱子 ， 順便改  ， 在 connected_graph  上的group id 	 
				int tmp_x = -1 , tmp_y = -1 ; 
				tmp_x = seperate_group[ which_group ][k].print_node1_x() ; 
				tmp_y = seperate_group[ which_group ][k].print_node1_y() ; 
				my_level_info.connected_graph[tmp_x][tmp_y] =  my_level_info.connected_graph[node1_x][node1_y] ; 

				tmp_x = seperate_group[ which_group ][k].print_node2_x() ; 
				tmp_y = seperate_group[ which_group ][k].print_node2_y() ; 
				my_level_info.connected_graph[tmp_x][tmp_y] =  my_level_info.connected_graph[node1_x][node1_y] ; 
				// 要把東西都給班過去   , 為什麼下面這行  會讓我的code 當了呢   , 告夭 ， 我之道為什麼了拉  
				// 我把  node1箱子的index 給了  node2  , 又 從它那取箱子的index , 等於我都在處理同個箱子 按  
				seperate_group[ des_group ].push_back( seperate_group[which_group][k] ) ; 
			}// end of loop j 

			// 班完後 ， 把另一邊的東西給刪了 
			seperate_group[  which_group  ].clear()  ;

		}// 都在不同的subtree上 

		/*-----------------------------------------------------------------------------
		// 要怎麼判斷 ， 所有的pin點 都已經連通了呢  , 有二個樹 要合併的時侯 ， 再去比  ， 
		// 合併時 ， 二個tree 的subtree id 都會 統一了  ， 此時 ， 只要用only_pin 去 connect_graph
		// 中 ， 看  是否每個pin的subtree id 都一樣  ， 就可以了  如果 有一個不一樣  ， 就跳出來 
		for( unsigned int j = 0 ; j < only_pin.size() - 1 ; j++ ){
			 
			int  x =  only_pin[j].print_x_position() ;  
			int  y =  only_pin[j].print_y_position() ;  

			int  xx =  only_pin[ j + 1].print_x_position() ;  
			int  yy =  only_pin[ j + 1].print_y_position() ;  

			if(  my_level_info.connected_graph[x][y] >= 0 && my_level_info.connected_graph[xx][yy] >= 0 ){
			 
				if( my_level_info.connected_graph[x][y] != my_level_info.connected_graph[xx][yy] ){
					connect_graph_complete = false ;  
					break ; 
				}
				else if( my_level_info.connected_graph[x][y] == my_level_info.connected_graph[xx][yy] ){
					connect_graph_complete = true ;  
				}

			}


		}// scan all pins 

		if( connect_graph_complete == true ){
			break ;  
		}
		 *-----------------------------------------------------------------------------*/
		 if( delete_or_not == false  ){
			my_level_info.degree_per_vertex[ node1_x ][ node1_y]++ ; 
			my_level_info.degree_per_vertex[ node2_x ][ node2_y]++ ; 
		 }

	}// scan all edges in obstacle_avoding_Kruskal 

	// 這裡的話 ， 要把 ， index_delete 後面  的edge 都給刪了  , index_delete 一直是－1 哎  , 怪了  那代表沒有改到丫  
	// cout << "index_delete = " << index_delete << endl ; 
	// obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_delete + 1 , obstacle_avoding_Kruskal.end() ) ; 


	bool all_ok = false ; 
	while( all_ok == false ){
		all_ok = true ; 	
		// 拿掉的edge , degree 記得要--
		for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){

			// cout << "i = "	<< i << endl ; 
			
			// cout << " Do i get in "  << endl ;  , 我這樣 ， 沒有把後面的給刪掉 
			
			 // bool connect_graph_complete = false ;  
			int node1_x = 	obstacle_avoding_Kruskal[ i ].print_node1_x() ; 
			int node1_y = 	obstacle_avoding_Kruskal[ i ].print_node1_y() ; 

			int node2_x = 	obstacle_avoding_Kruskal[ i ].print_node2_x() ; 
			int node2_y = 	obstacle_avoding_Kruskal[ i ].print_node2_y() ; 

			// 在 pin_plane 及 obstacle_plane 中  ， 都同為true的  就是 
			if( my_level_info.degree_per_vertex[ node1_x ][ node1_y] == 1      && 
			    my_level_info.pin_plane[ node1_x ][ node1_y] == false          ){
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
				i-- ; 
				all_ok = false ; 
				my_level_info.degree_per_vertex[ node1_x ][ node1_y]-- ; 
				my_level_info.degree_per_vertex[ node2_x ][ node2_y]-- ; 
			}
			else if( my_level_info.degree_per_vertex[ node2_x ][ node2_y] == 1      && 
				 my_level_info.pin_plane[ node2_x ][ node2_y] == false     	){
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
				i-- ; 
				all_ok = false ; 
				my_level_info.degree_per_vertex[ node1_x ][ node1_y]-- ; 
				my_level_info.degree_per_vertex[ node2_x ][ node2_y]-- ; 
			}

		}// end of scan all obstacle_avoding_Kruskal 

	}// end of while 

	sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight );	 
	 unsigned int  index_b = 0 ; 




	bool  find_index_b = false ; 
	// 下面這個 loop i 用來 指向 第一個 edge 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size()  ; i++ ){

		int temp_overlap_value = 0 ; 
		int max_overlap_value = 0 ; 

		// 有可能發生  index_a 沒指到人丫 , 
		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
		 
			// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
			// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
			// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
			if( two_edges_overlap_or_not_by_new_method( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) == true  ){
				// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		 , 一定要找到底 ， 
				temp_overlap_value = cal_overlap_value( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] );

				if( temp_overlap_value > max_overlap_value ){
					max_overlap_value = temp_overlap_value ; 	 
					index_b = j ;
					find_index_b = true ; 
				}

			}// end of already_rectilinear[i] == false

		}// end of loop j 

		// 有找到  才要做事 ， 因為這裡只要考慮 二個edge 的  , 只要有找到就可以了  ， 以乎不用去分它的case  是怎樣  
		if(  find_index_b == true  ){
		 
			vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
			vector<vertex> all_vertex ; 
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node1_x() 
							, obstacle_avoding_Kruskal[i].print_node1_y() , obstacle_avoding_Kruskal[i].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node2_x() 
							, obstacle_avoding_Kruskal[i].print_node2_y() , obstacle_avoding_Kruskal[i].print_node2_z() ) );

			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node1_x() 
						, obstacle_avoding_Kruskal[index_b].print_node1_y() , obstacle_avoding_Kruskal[index_b].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node2_x() 
						, obstacle_avoding_Kruskal[index_b].print_node2_y() , obstacle_avoding_Kruskal[index_b].print_node2_z() ) );
			// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
			// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
			unsigned int index_i = 0 , index_j = 0  ; 
			bool find_common_vertex = false ; 
			for(unsigned int h = 0 ; h < all_vertex.size() - 2 ; h++ ){

				for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

					if( all_vertex[h].print_x_position() == all_vertex[j].print_x_position() &&
					    all_vertex[h].print_y_position() == all_vertex[j].print_y_position() ){

						index_i = h ;  
						index_j = j ;  
						common_vertex.set_position( all_vertex[h].print_x_position() , all_vertex[h].print_y_position() );
						common_vertex.set_z_position( all_vertex[h].print_z_position() );
						find_common_vertex = true ; 
						break ; 

					}// end of ix
				 
				}// loop j 
			 
			}// loop i 
			// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
			// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置 
			if( find_common_vertex == true ){
			 
			 	// 這裡是刪掉那個相同的 vertex 
				all_vertex.erase( all_vertex.begin() + index_j ) ; 
				all_vertex.erase( all_vertex.begin() + index_i ) ; 
				// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
				near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
				near.set_z_position( all_vertex[0].print_z_position() );
				far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
				far.set_z_position( all_vertex[1].print_z_position() );

			}

			// 這裡就是要看 ， 第三個edge有沒有比較短  ， 而且  是否在 OASG 內   , 因為還有 vertex_id 的關係 ， 
			// 所以  ， 很容易之道  ， 那條edge 有沒有在裡面  , 那個 記 點跟那些點有連 edge的那個資結 ， 叫什
			// 麼名字丫   , 喔  ， 叫table  , 先抓出  ， 它們三個點 分別是誰吧   , 點抓好了  ， 那接下來  便是看 
			// near 跟far 這二點的長度 ， 有沒有比  其它二個 edge 短  ， 有的話  ， 才要 換過來  ,  拿最長的 來 換 
			int third_edge_weight = abs( near.print_x_position() - far.print_x_position() ) + 
						abs( near.print_y_position() - far.print_y_position() )   ; 

			// 下面二個  用來記  ， third_edge 的 ， 兩端點的vertex_id , 看是否存在著這個edge 
			int one = -1 , two = -1 ; 
			one = my_level_info.vertex_id[ near.print_x_position() ][ near.print_y_position() ] ; 
			two = my_level_info.vertex_id[ far.print_x_position()  ][  far.print_y_position() ] ; 
			bool exist_or_not = false ; 
			for(unsigned int j = 0 ; j < table[one].size() ; j++ ){
				if( table[one][j].print_desitination_vertex_index() == two ){
					exist_or_not = true ;  
					break ; 
				}
			}// scan all vertex on index one 

			// 等一下  有可能  ， 二個都比它大  ， 所以 要先比看看 那二個edge 誰比較大   , 等於至少要放在一邊  
			if( exist_or_not == true 									    && 
			    obstacle_avoding_Kruskal[i].print_weight() >=  obstacle_avoding_Kruskal[index_b].print_weight() && 
			    obstacle_avoding_Kruskal[i].print_weight() >   third_edge_weight   				    ){

				// cout <<  " Do i find some case can change " << endl  ; 
				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i ) ; 
				i-- ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// 確實有改到 ， 但  量不多  ， 像 800 x 200 也才改了 3 個而已 
				// cout << "Do i change somthing " << endl ; 
			 
			}
			else if( exist_or_not == true 										&& 
				 obstacle_avoding_Kruskal[index_b].print_weight() >  obstacle_avoding_Kruskal[i].print_weight()	&&
				 obstacle_avoding_Kruskal[index_b].print_weight() >  third_edge_weight 				){

				// cout <<  " Do i find some case can change " << endl  ; 
				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_b ) ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// cout << "Do i change somthing " << endl ; 
			}// end of else if

		}// 兩個  ， 有找到配對的  


	}// scan all elements on obstacle_avoding_Kruskal , what we need is false element , not do rectilinear 



	 // cout << endl << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 按  ， 我不懂哎 ， 我少了個loop 直接去取那個值  ， 怎麼還比較慢呢  
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = -1 ; 

	}// end of for
	/*-----------------------------------------------------------------------------
	 * 這裡的話  ， 在做完OAST 後   ， 其實路徑還不是最短的  ， 所以 要再加一些東西 ， 去保證
	 * 這個tree  是最短的  ， 而  從圖上  觀察到 ， 能這樣 換的 其實  它們是 rectilinear 化的
	 * case 2 , 3 所以  只要 有發生這些case  , 我就看  ， 另一個 沒加入的edge  , 加入後 如果有比較短
	 * 而且 是在 OASG 內的edge  , 那就可以了  , 喔 ， 下面這行  ， 我只要有載入 那個 header file , 那裡面的
	 * func 就可以直接用了  ，  哎  ， 現在就排好序的話 ， 那 我之後rectilinear 的那個 就不用再排了 
	 *  
	 *  首先呢 ， 傳進來的 obstacle_avoding_Kruskal 是要 處理成 rectilinear 的 ， 而  我已經對 ， weight 做排序了 
	 *  要二個變數 ， 之道index 指向那二個 ， 以及  二個bool 去看說  ， 現在這二個的情況是 ， 那種case
	 *  也用一個ok變數 ， 直到 obstacle_avoding_Kruskal 內的東西都沒了  都跑出while 
	 *
	 *  哇 ， 這塊 會讓我的東西有問題  ， 就是解決cycle的那塊  的  
	sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight );	 
	 unsigned int  index_b = 0 ; 




	bool  find_index_b = false ; 
	// 下面這個 loop i 用來 指向 第一個 edge 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size()  ; i++ ){

		int temp_overlap_value = 0 ; 
		int max_overlap_value = 0 ; 

		// 有可能發生  index_a 沒指到人丫 , 
		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
		 
			// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
			// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
			// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
			if( two_edges_overlap_or_not_by_new_method( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) == true  ){
				// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		 , 一定要找到底 ， 
				temp_overlap_value = cal_overlap_value( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] );

				if( temp_overlap_value > max_overlap_value ){
					max_overlap_value = temp_overlap_value ; 	 
					index_b = j ;
					find_index_b = true ; 
				}

			}// end of already_rectilinear[i] == false

		}// end of loop j 

		// 有找到  才要做事 ， 因為這裡只要考慮 二個edge 的  , 只要有找到就可以了  ， 以乎不用去分它的case  是怎樣  
		if(  find_index_b == true  ){
		 
			vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
			vector<vertex> all_vertex ; 
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node1_x() 
							, obstacle_avoding_Kruskal[i].print_node1_y() , obstacle_avoding_Kruskal[i].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node2_x() 
							, obstacle_avoding_Kruskal[i].print_node2_y() , obstacle_avoding_Kruskal[i].print_node2_z() ) );

			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node1_x() 
						, obstacle_avoding_Kruskal[index_b].print_node1_y() , obstacle_avoding_Kruskal[index_b].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node2_x() 
						, obstacle_avoding_Kruskal[index_b].print_node2_y() , obstacle_avoding_Kruskal[index_b].print_node2_z() ) );
			// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
			// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
			unsigned int index_i = 0 , index_j = 0  ; 
			bool find_common_vertex = false ; 
			for(unsigned int h = 0 ; h < all_vertex.size() - 2 ; h++ ){

				for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

					if( all_vertex[h].print_x_position() == all_vertex[j].print_x_position() &&
					    all_vertex[h].print_y_position() == all_vertex[j].print_y_position() ){

						index_i = h ;  
						index_j = j ;  
						common_vertex.set_position( all_vertex[h].print_x_position() , all_vertex[h].print_y_position() );
						common_vertex.set_z_position( all_vertex[h].print_z_position() );
						find_common_vertex = true ; 
						break ; 

					}// end of ix
				 
				}// loop j 
			 
			}// loop i 
			// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
			// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置 
			if( find_common_vertex == true ){
			 
			 	// 這裡是刪掉那個相同的 vertex 
				all_vertex.erase( all_vertex.begin() + index_j ) ; 
				all_vertex.erase( all_vertex.begin() + index_i ) ; 
				// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
				near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
				near.set_z_position( all_vertex[0].print_z_position() );
				far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
				far.set_z_position( all_vertex[1].print_z_position() );

			}

			// 這裡就是要看 ， 第三個edge有沒有比較短  ， 而且  是否在 OASG 內   , 因為還有 vertex_id 的關係 ， 
			// 所以  ， 很容易之道  ， 那條edge 有沒有在裡面  , 那個 記 點跟那些點有連 edge的那個資結 ， 叫什
			// 麼名字丫   , 喔  ， 叫table  , 先抓出  ， 它們三個點 分別是誰吧   , 點抓好了  ， 那接下來  便是看 
			// near 跟far 這二點的長度 ， 有沒有比  其它二個 edge 短  ， 有的話  ， 才要 換過來  ,  拿最長的 來 換 
			int third_edge_weight = abs( near.print_x_position() - far.print_x_position() ) + 
						abs( near.print_y_position() - far.print_y_position() )   ; 

			// 下面二個  用來記  ， third_edge 的 ， 兩端點的vertex_id , 看是否存在著這個edge 
			int one = -1 , two = -1 ; 
			one = my_level_info.vertex_id[ near.print_x_position() ][ near.print_y_position() ] ; 
			two = my_level_info.vertex_id[ far.print_x_position()  ][  far.print_y_position() ] ; 
			bool exist_or_not = false ; 
			for(unsigned int j = 0 ; j < table[one].size() ; j++ ){
				if( table[one][j].print_desitination_vertex_index() == two ){
					exist_or_not = true ;  
					break ; 
				}
			}// scan all vertex on index one 

			// 等一下  有可能  ， 二個都比它大  ， 所以 要先比看看 那二個edge 誰比較大   , 等於至少要放在一邊  
			if( exist_or_not == true 									    && 
			    obstacle_avoding_Kruskal[i].print_weight() >=  obstacle_avoding_Kruskal[index_b].print_weight() && 
			    obstacle_avoding_Kruskal[i].print_weight() >   third_edge_weight   				    ){

				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i ) ; 
				i-- ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// 確實有改到 ， 但  量不多  ， 像 800 x 200 也才改了 3 個而已 
				// cout << "Do i change somthing " << endl ; 
			 
			}
			else if( exist_or_not == true 										&& 
				 obstacle_avoding_Kruskal[index_b].print_weight() >  obstacle_avoding_Kruskal[i].print_weight()	&&
				 obstacle_avoding_Kruskal[index_b].print_weight() >  third_edge_weight 				){

				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_b ) ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// cout << "Do i change somthing " << endl ; 
			}// end of else if

		}// 兩個  ， 有找到配對的  


	}// scan all elements on obstacle_avoding_Kruskal , what we need is false element , not do rectilinear 



	 // cout << endl << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 按  ， 我不懂哎 ， 我少了個loop 直接去取那個值  ， 怎麼還比較慢呢  
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = -1 ; 

	}// end of for
	 *-----------------------------------------------------------------------------*/

}// end of func build_obstacle_avoding_kruskal 



void obstacle_avoding_kruskal::build_overlap_obstacle_avoding_kruskal_imp( vector<edge> &SG_edge  , vector<edge> &obstacle_avoding_Kruskal  ,
							       vector<edge> &OASG , level_info &my_level_info ){

	// 09 27 我打算改一樣東西  ， 就是 ， edge 在 跟 obstacle 有相鄰時  ， 不改動它  ， 等到 rectilinear 再去弄 
	bool overlap_debug = false ; 
	
	 // cout << "Coming obstacle_avoding_Kruskal func " << endl ; 
	/*-----------------------------------------------------------------------------
	 *  				step 1 ,2 
	 *  				check edge ok or not 
	 *  				理應是obstacle的數目比較少　，　所以
	 *  				裡面的loop 是用　my_level_info.complete_obstacle_information
	 *  				這裡  光是每個edge 都要跟obstacle 做比對 ， 怎麼受的了 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i  = 0 ; i < SG_edge.size() ; i++){
		
		/*-----------------------------------------------------------------------------
		 *  cout << "Here is edge information " << endl ;  
		SG_edge[i].print_kruskal_edge_information() ; 
		cout << "min_x = " ;
		cout << SG_edge[i].print_min_x() << endl ; 
		cout << "max_x = " ;
		cout << SG_edge[i].print_max_x() << endl ;
		cout << "min_y = " ;
		cout << SG_edge[i].print_min_y() << endl ; 
		cout << "max_y = " ;
		cout << SG_edge[i].print_max_y() << endl ;
		cout << "kruskal_edge_ok_or_not = "  <<  SG_edge[i].print_kruskal_edge_ok_not() << endl ;

		下面呢  我要重 寫  ， 用 obstacle_map 及 obstacle_bound_cross , 這二個參數  ， 來 減少下面
		的時間 
		 *-----------------------------------------------------------------------------*/
		for(int j = SG_edge[i].print_min_x() ; j <= SG_edge[i].print_max_x() ; j++ ){

			map<int,obstacle>::iterator it ;  
			int key = 0 ; 
			for(unsigned int k = 0 ; k < my_level_info.obstacle_cross[j].size() ; k++ ){
			 
				 key = my_level_info.obstacle_cross[j][k] ;
				 it = my_level_info.obstacle_map.find( key ) ; 
				 bool first_case = false , Second_case = false ; 

				 if( SG_edge[i].print_max_x() >= (*it).second.print_ld_corner_x() && 
				     SG_edge[i].print_min_x() <= (*it).second.print_ru_corner_x() ){
					first_case = true ;  
				 }
				 if( SG_edge[i].print_max_y() >= (*it).second.print_ld_corner_y() && 
				     SG_edge[i].print_min_y() <= (*it).second.print_ru_corner_y() ){
					Second_case = true ;  
				 }

				 /*-----------------------------------------------------------------------------
				  *  cout << "first_case = " << first_case << endl; 
				 cout << "Second_case = " << Second_case << endl ;
				 if( first_case == true || Second_case == true ){
					my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
				 }
				 cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
				  *-----------------------------------------------------------------------------*/
				  

				 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
				 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
				 //  就不用再往下比了  , 所以我多了個 break ; 
				 if( first_case == true && Second_case == true ){
					SG_edge[i].set_kruskal_edge_ok_not(false);
					SG_edge[i].add_which_obstacle_cross( (*it).second.print_obstacle_id() );
					break ; 
				 }
			}
		 
		}// 只 scan SG 有踫到的x 範圍  


		/*-----------------------------------------------------------------------------
		 *  
		for(unsigned int j  = 0 ; j < my_level_info.complete_obstacle_information.size()  ; j++){
		
		//	   我想到個好方法了　，　藉由　在Kruskal edge 中　，　抓出其最大的x,y
		//	 跟obstacle中　，　的左下角點比　，　即可　只要max_x 大於等於node1_x 
		//	 　就有機會相交了　，　還要再心對max_y >= node1_y ，如果二個都成立
		//	 那就一定有overlap , 但如果x 那關都過不了　就一定沒有overlap
		//	, 我還少弄一些判斷進去　那就是min x,y , 要小於左下的角點　，　不然edge
		//	　如果在右上角　　也會被算進去
		//		max_y >= ld_corner_y
		//		min_y <= ru_corner_y
		//		反之　x 的也是如此 
		//	 bool first_case = false , Second_case = false ; 

			 if( SG_edge[i].print_max_x() >= my_level_info.complete_obstacle_information[j].print_ld_corner_x() && 
			     SG_edge[i].print_min_x() <= my_level_info.complete_obstacle_information[j].print_ru_corner_x() ){
			 	first_case = true ;  
			 }
			 if( SG_edge[i].print_max_y() >= my_level_info.complete_obstacle_information[j].print_ld_corner_y() && 
			     SG_edge[i].print_min_y() <= my_level_info.complete_obstacle_information[j].print_ru_corner_y() ){
			 	Second_case = true ;  
			 }

			// cout << "first_case = " << first_case << endl; 
			// cout << "Second_case = " << Second_case << endl ;
			// if( first_case == true || Second_case == true ){
			//	my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
			// }
			// cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
			  

			 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
			 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
			 //  就不用再往下比了  , 所以我多了個 break ; 
			 if( first_case == true && Second_case == true ){
			 	SG_edge[i].set_kruskal_edge_ok_not(false);
				SG_edge[i].add_which_obstacle_cross( my_level_info.complete_obstacle_information[j].print_obstacle_id() );
				break ; 
			 }
			//cout << "kruskal_edge_ok_or_not = "  <<  SG_edge[i].print_kruskal_edge_ok_not() << endl ;
		}// end of for　j
		 *-----------------------------------------------------------------------------*/

	}// end of 抓出edge  是否要更動　，　以及overlap 的obstacle 有那些  
	

	
	if( overlap_debug == true  ){
		cout << "Line 2348 "  << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 * 				Check
	 * 		every Kruskal edge ok or not 
	 * 		and which obstacles it cross  .    
	 *        cout << endl << endl << "Check Kruskal_Tree edge " << endl ; 
		for(unsigned int i = 0 ; i < SG_edge.size() ; i++  ){
		
		cout << "This is edge number " << i+1 << endl ; 
		// 奇怪了　，　只要是設定false 那就一定有值會出來才對丫  
		SG_edge[i].print_kruskal_edge_information() ; 
	}

	 *-----------------------------------------------------------------------------*/
	 
	/*-----------------------------------------------------------------------------
	 *  				Step 3,4
	 *  		因為我已經抓出　有cross 的obstacle了　，　要抓其四個端點
	 *  		很簡單　，　接下來呢，　就建個新的vector 用來存
	 *  		obstacle_avoding_by_kruskal_Tree , 
	 *  		為true 的edge 就直接放入　，　false 的便修改後　，再放入
	 *
	 *-----------------------------------------------------------------------------*/
	// 下面這二個  好像後面還會用到的樣子
	// 沒有 只有  第一個 要記得放外面就好了
	// vector<edge> obstacle_avoding_Kruskal ; 	
	// 下面建了個 vertex_map , 是用來  ， 給vertex抓出它的index 
	vector<vertex> vertex_need ; 

	// 在考慮 overlap 下  ， total 是否有把 overlap_vertex 給包進去呢 
	for( int j = 0 ; j < my_level_info.total ; j++ ){

			 
			// 因為我已經寫好了 vertex 的copy constructor 所以vertex 可以直接給了  	
			// 這裡是存  ， 在plane 內 ， 所有的pin 及 obstalce corner 所組成的點 
			vertex_need.push_back( my_level_info.array_vertex[j] );	
			vertex_need[j].set_id( j ) ; 
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = j ; 


	}// end of for

	if( overlap_debug == true  ){
		cout << "Line 2398 "  << endl ; 
	}

	for( unsigned int i = 0  ; i <  my_level_info.overlap_vertex.size() ; i++ ){
			
			int count = my_level_info.total + i ; 
			vertex_need.push_back( my_level_info.overlap_vertex[i] );	
			vertex_need[count].set_id( count ) ; 
			int x = vertex_need[count].print_x_position() ; 
			int y = vertex_need[count].print_y_position() ; 
			my_level_info.vertex_id[x][y] = count ; 
		 
	}// scan all overlap_vertex in my_level_info 
	
	if( overlap_debug == true  ){
		cout << "Line 2409 "  << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 *  
	// vector<vertex> vertex_seperate[ my_level_info.plane_width ] ; 
	for(unsigned int i = 0 ; i < vertex_need.size() ; i++ ){
		vertex_need[i].set_id( i ) ; 
		int x = vertex_need[i].print_x_position() ; 
		int y = vertex_need[i].print_y_position() ; 
		my_level_info.vertex_id[x][y] = i ; 

		// vertex_seperate[ vertex_need[i].print_x_position() ].push_back( vertex_need[i] ) ; 	 
	}
	 *-----------------------------------------------------------------------------*/

	 // 這裡把表的初值給建好了
	 // 應該把　path_node 換成　table_node 比較好　
	 // 因為要建相鄰 串列  ， 所以要有個 一維的點集合 ， 後面串 接它相鄰的人有誰  
	 vector<path_node> table[ vertex_need.size() ];			 

	 /*-----------------------------------------------------------------------------
	 // 先把表的樣子給先建起來　
	 // 這個表table  我想建起 ， 這個 vertex 跟 那些vertex 是相鄰的 就好了  
	 // 我只要比對 這是那一個row 的 ， 就可以到 vertex_need 抓到這個點的資訊了 
	 for(unsigned int j = 0  ; j <  vertex_need.size() ; j++ ){
		for(unsigned int k = 0  ; k <  vertex_need.size() ; k++ ){
			table[j].push_back( path_node() ); 
		} 
	 }
	  *-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
	 *  				下面這塊　是建好OAST 的核心
	 *  				要用的edge　我都抓好了　，　
	 *  				接下來便是要抓shortest path 
	 *  	先建出一張表來，　各個vertex 到別的點的weight , 要建幾乘幾的表，
	 *  	可由vertex_need 去看　，　再由edge_need 去一個個掃　，　來建立
	 *  	weight , 那　無法到達的 無限要怎麼代表它呢　
	 *  	, 我建了二個data structure , node_need 跟　edge_need 
	 *  	,node_need中的第０跟１　，　理應是　我要的source跟desitination 
	 *  	表的index 順序，　我想就跟node_need 一樣　，　
	 *  	而表的內容　，　就掃過edge_need , 來一個個建　，　一個edge_need
	 *  	要建　二個位置的值　像(3,1) (1,3)　這樣　
	 *-----------------------------------------------------------------------------*/
	 // 接下來　　，　把值弄進去　，　每個edge 都跟vertex_need 掃過一遍
	 // 抓出edge  二端點的node index ,  這裡建相鄰串列的地方 也是花太多時間了

	for(unsigned int j = 0  ; j < OASG.size() ; j++  ){

		// 內層的for  , 要抓出兩端點的index 
		int index_one = -1 , index_two = -1  ; 

		int x = OASG[j].print_node1_x() , y = OASG[j].print_node1_y()  ; 
		index_one = my_level_info.vertex_id[x][y]  ; 

		int xx = OASG[j].print_node2_x() , yy = OASG[j].print_node2_y()  ; 
		index_two = my_level_info.vertex_id[xx][yy]  ; 
		/*-----------------------------------------------------------------------------
		// 下面這個loop  太花時間了 ， 我打把它寫成map 去抓  , 水拉   ， 換成下面的方法 ， 有變快了
		// 應該是因為  外層是 OASG  太大的關係 
		for(unsigned int k = 0 ; k < vertex_seperate[ OASG[j].print_node1_x() ].size() ; k++ ){

			if( OASG[j].print_node1_y() == vertex_seperate[ OASG[j].print_node1_x() ][k].print_y_position() ){
				// 為什麼 ， 不能在這裡加break ; 
				index_one = vertex_seperate[ OASG[j].print_node1_x() ][k].get_id() ;  
			}

		}// end of for

		for(unsigned int k = 0 ; k < vertex_seperate[ OASG[j].print_node2_x() ].size() ; k++ ){

			if( OASG[j].print_node2_y() == vertex_seperate[ OASG[j].print_node2_x() ][k].print_y_position() ){
				index_two =  vertex_seperate[ OASG[j].print_node2_x() ][k].get_id() ;  
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  
		for(unsigned int k = 0  ;  k < vertex_need.size() ; k++  ){
			// 有的vertex 會對不到
			if(OASG[j].print_node1_x() == vertex_need[k].print_x_position() &&
			   OASG[j].print_node1_y() == vertex_need[k].print_y_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_one = k ;  
			}
			if(OASG[j].print_node2_x() == vertex_need[k].print_x_position() &&
			   OASG[j].print_node2_y() == vertex_need[k].print_y_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_two = k ;  
			}
		}// 　抓好二個端點的index 了
		 *-----------------------------------------------------------------------------*/

		// 要把值給送進table 
		if( index_one != -1 && index_two != -1 ){
			/*-----------------------------------------------------------------------------
			 *  
			 這步做完 ， 應該  就把各個相鄰的東西 ， 給抓出來了吧  
			 之前 我是藉由 ， row 跟 column 來判斷 ， 這個path_node 是
			 那紀路那兩個 vertex 的關係  ， 但現在不能用column 來抓
			 了  ， 所以要在path_node 中 ， 多加一項資訊 ， 叫 
			 desitination_vertex_id  ,  奇怪了 ， 這個老是不能班過去  
			 *-----------------------------------------------------------------------------*/
			table[index_one].push_back( path_node( index_one ,  OASG[j].print_weight() , index_two ) ) ; 
			table[index_two].push_back( path_node( index_two ,  OASG[j].print_weight() , index_one ) ) ; 

			/*-----------------------------------------------------------------------------
			table[index_one][index_two].set_infinite(false) ;
			table[index_one][index_two].set_path_weight( OASG[j].print_weight() );
			table[index_two][index_one].set_infinite(false) ;
			table[index_two][index_one].set_path_weight( OASG[j].print_weight() );	 
			 *-----------------------------------------------------------------------------*/

		}// 建起表內  各個格子的內容 
		
		
	}// end of 抓二端點的　index , 且埴滿table內的值　

	if( overlap_debug == true  ){
		cout << "Line 2531 "  << endl ; 
	}

	/*-----------------------------------------------------------------------------
	 * 			check code , May 9 
	cout << endl << "here become single source all desitination : " << endl ;  
	// 在這之後的東西 就沒有跑出來了 ， 所以 我想 理應是某個func 被我改了 
	// 但 effect 到這裡  
	// 改善最多的  應該是因為 我下面放的這個吧   ,  只要放對地方  ， 改善很多哎 
	 *-----------------------------------------------------------------------------*/
	int SG_edge_change = SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	for(int i = 0  ; i < SG_edge_change ; i++  ){

		/*-----------------------------------------------------------------------------
		 * 			check code , May 9 
		cout << SG_edge[i].print_kruskal_edge_ok_not() << endl ; 
		 *-----------------------------------------------------------------------------*/
		if( SG_edge[i].print_kruskal_edge_ok_not() == true  ){
			obstacle_avoding_Kruskal.push_back( SG_edge[i] ) ; 
			// cout << "Edge number " << i << " is ok " << endl ; 
		}
		else{
			
			// cout << "Edge number " << i << " is not ok " << endl ; 
			/*-----------------------------------------------------------------------------
			 *  我要把edge_need 放到上面去做 ， 這裡變成 ， 抓edge兩端點的index出來即可
			 *  這裡抓的index 是指在 vertex_need 裡的index 
			 *  跟老師討論過 ，  那 safe_range 好像不太必要哎 , safe_range = 3
			 *  下面這塊真的是很麻煩 ， 每次都要比到完 
			 *-----------------------------------------------------------------------------*/
			int head_index = -1 , tail_index = -1  ; 	

			int  x = SG_edge[i].print_node1_x() , y  = SG_edge[i].print_node1_y()  ; 
			head_index = my_level_info.vertex_id[x][y] ; 
			int xx = SG_edge[i].print_node2_x() , yy = SG_edge[i].print_node2_y()  ; 
			tail_index = my_level_info.vertex_id[xx][yy] ; 
			/*-----------------------------------------------------------------------------
			 *  
			// 看來要從 ， 這裡著手 ， 會比較正確了   , 這裡是起始點 ， 所以 目的點如果 已經到過了  
			// 那就要  ， 從已加入的edge中  ， 找到  ， 離自已最近的點 ， 去做 routing 了 
			if( my_level_info.already_in_OAST[x][y] == true ){
				// 找一個離起始點最近的 ， 來當 新的目的點 		 
				int temp_min_weight_value  = 0 ; 
				int min_weight_value  = abs( obstacle_avoding_Kruskal[0].print_node1_x() - xx) + 
							abs( obstacle_avoding_Kruskal[0].print_node1_y() - yy) ; 
				for(unsigned int j = 0 ; j < obstacle_avoding_Kruskal.size() ; j++  ){

					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node1_x() - xx) + 
								 abs( obstacle_avoding_Kruskal[j].print_node1_y() - yy) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						head_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node1_x()]
										    [obstacle_avoding_Kruskal[j].print_node1_y()] ; 
					}
					
					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node2_x() - xx) + 
								 abs( obstacle_avoding_Kruskal[j].print_node2_y() - yy) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						head_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node2_x()]
										    [obstacle_avoding_Kruskal[j].print_node2_y()] ; 
					}

				}// loop j 
			}
			else if( my_level_info.already_in_OAST[x][y] == false ){
				head_index = my_level_info.vertex_id[x][y] ; 
			}



			// 看來要從 ， 這裡著手 ， 會比較正確了   , 這裡是起始點 ， 所以 目的點如果 已經到過了  
			// 那就要  ， 從已加入的edge中  ， 找到  ， 離自已最近的點 ， 去做 routing 了 
			if( my_level_info.already_in_OAST[xx][yy] == true ){
				// 找一個離起始點最近的 ， 來當 新的目的點 		 
				int temp_min_weight_value  = 0 ; 
				int min_weight_value  = abs( obstacle_avoding_Kruskal[0].print_node1_x() - x) + 
							abs( obstacle_avoding_Kruskal[0].print_node1_y() - y) ; 
				for(unsigned int j = 0 ; j < obstacle_avoding_Kruskal.size() ; j++  ){

					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node1_x() - x) + 
								 abs( obstacle_avoding_Kruskal[j].print_node1_y() - y) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						tail_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node1_x()]
										    [obstacle_avoding_Kruskal[j].print_node1_y()] ; 
					}
					
					temp_min_weight_value =  abs( obstacle_avoding_Kruskal[j].print_node2_x() - x) + 
								 abs( obstacle_avoding_Kruskal[j].print_node2_y() - y) ;
					if( temp_min_weight_value < min_weight_value ){
						min_weight_value = temp_min_weight_value ; 
						tail_index = my_level_info.vertex_id[obstacle_avoding_Kruskal[j].print_node2_x()]
										    [obstacle_avoding_Kruskal[j].print_node2_y()] ; 
					}

				}// loop j 
			}
			else if( my_level_info.already_in_OAST[xx][yy] == false ){
				tail_index = my_level_info.vertex_id[xx][yy] ; 
			}
			 *-----------------------------------------------------------------------------*/

			/*-----------------------------------------------------------------------------
			for(unsigned int j = 0 ; j < vertex_seperate[ SG_edge[i].print_node1_x() ].size() ; j++ ){

				if( SG_edge[i].print_node1_y() == vertex_seperate[ SG_edge[i].print_node1_x() ][j].print_y_position() ){
					head_index =  vertex_seperate[ SG_edge[i].print_node1_x() ][j].get_id() ;  
					// 為什麼 ， 不能在這裡加break ; 
				}

			}// end of for

			for(unsigned int j = 0 ; j < vertex_seperate[ SG_edge[i].print_node2_x() ].size() ; j++ ){

				if( SG_edge[i].print_node2_y() == vertex_seperate[ SG_edge[i].print_node2_x() ][j].print_y_position() ){
					tail_index = vertex_seperate[ SG_edge[i].print_node2_x() ][j].get_id() ;  
				}

			}// end of for
			 *-----------------------------------------------------------------------------*/

			//cout << "my_level_info.total = " << my_level_info.total << ",  vertex_need.size " << vertex_need.size() << endl ;   

			// 這裡要寫些code來看資訊 ， 用來抓bug 的 ， 要印出 ， 此時的i 是多少
			// 以及這個要被改的edge  資訊 ， 
			//cout << " i = " << i << endl ; 
			//SG_edge[i].print_kruskal_edge_information() ; 
			
			/*-----------------------------------------------------------------------------
			 *  
			// 這裡 印出個花費相鄰矩陣來看看 好了  
			cout << "Here print out weight table : " << i << endl ; 
			for(unsigned int j = 0 ; j < vertex_need.size() ; j++ ){
			 	for(unsigned int k = 0 ; k < vertex_need.size() ; k++ ){

					if( j == k ){
						table[j][j].set_infinite(false) ;
						table[j][j].set_path_weight( 0 );	 
					}

					if(table[j][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[j][k].print_path_weight() << " " ;	
					}

				}// inner loop j 
				cout << endl << endl ;; 
			}// end of 印出矩陣來看看
			 *-----------------------------------------------------------------------------*/
	
			/*-----------------------------------------------------------------------------
			 *  表建好後　，　接下來便是執行　one source all destination 的algorithm 
			先抓source 行中　weight  最小的　，　及　此edge node2是誰
			把weight加到　node2那行　再跟 source 行比對，　　如果值有變小　，　或
			原本true 的　變false ，　也可把值納入　
			抓出shortest path 中間要的轉接點後　，　可以用　之前的方法　，　從edge_need 中
			抓出　其edge_need 含這些點集的edge 即可
			還是說　用個　bool 紀路這個點　是否當過轉接點,  轉接點 只會有一個 ， 初值我設成 －1 
			只要是 source  能直接到達的 ， 都設成 －1 ， 
			原先的表是拿來查的 ， 不能直接休改 ， 
			 *-----------------------------------------------------------------------------*/
			 
			 /*-----------------------------------------------------------------------------
			  *  
			  因為table的直是用來查的 不是用來更新的 ， 不然  會出點錯誤 ，
			  所以這裡我要建個一行path_node 來 ， 存原始行的data , 
			  等路抓出來以後 ， 再用這個還原回去 
			  *-----------------------------------------------------------------------------*/

			 // 這是 要被更新的那一行
			 vector<path_node> for_update;			 
			// 我只要把上面的for_update變成 for_update[2] , 就可以一個從頭處理， 
			// 一個從尾巴了  , 好  這裡應該不用動它  , for_update 是紀著
			// 要被 更新  的那一row 

			for(unsigned int j = 0  ; j <  table[head_index].size() ; j++ ){
				for_update.push_back( table[head_index][j] ); 
			}// end of 抓出 我要更新的那個row 
			// cout << "for_update.size() = " << for_update.size() << endl ; 


			/*-----------------------------------------------------------------------------
			  如果抓不到一個可以轉接的點　，　便跳出去了　
			  這個 do while 把整個表給更新好了　，　接下來便是看我怎麼抓中間的edge了
			  感覺整個最花時閶就是 下面這塊了  ， 因為 有二個do while 的loop 
			  tail_index_in_for_update , 因為一開始 裡面沒有 我們要到的點的index 
			  所以最後有把要的點放進來的時侯 ， 要記住它的index  
			 *-----------------------------------------------------------------------------*/
			 bool ok = true ; 
			 int tail_index_in_for_update = -1 ; 
			 // int safe_count = 0 ; 
			 // cout << endl << "Ready coming update routing table on SG_edge : " << i << endl ; 
			 do{
			 	
				 /*-----------------------------------------------------------------------------
				  *  下面這一小塊  ， 是要把 之間有路  ， 且 還沒當過 轉接點的 vertex 給抓出來 
				  *  因為每個row的長度還是會變 ， 所以 還是 紀路  其  desitination_vertex_id 
				  *  這應該在我新方法中 ， 不能用 ， 因為我中間會加新的點進去 
				  *-----------------------------------------------------------------------------*/
				vector<int> non_infinite_used ; 
				for(unsigned int j = 0  ; j < for_update.size() ; j++  ){

					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					// 這邊有個新的問題 ， 因為我剛初是把全部的東西都放進來
					// 所以  會有 無限的這個情況  那我有改過了  ， 不會有這種
					// 情況了 ， 要不要拿掉呢 
					if( for_update[j].print_used() == false  ){
						non_infinite_used.push_back( j ) ; 
					}// end of 沒當過轉接的點 

					// 基本上 ， 只要 這個目地點給放入 ， 它在for_update 中的 index 都不會變了 
					// 我想呢  下面這個  ， 代表 ， 它跟目地點 本來就是相連的了  ， 很近  
					if( for_update[j].print_desitination_vertex_index() == tail_index ){
						// safe_count++ ;  
						tail_index_in_for_update = j ; 
					}

				}// end of  把  這個  row 中  ， 還能用來 轉接的點 給抓出來  

				// non_infinite_used.size() > 0 && tail_index_in_for_update == - 1
				// 這二塊 地方  ， 加入後的時間差很多  ， 但是時間變少的 ， 它的圖不好丫 
				if( non_infinite_used.size() > 0 ){

					for(unsigned int j = 0 ; j < non_infinite_used.size() ; j++ ){

						 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
						int update_row_index = 0 ; 
						int min_weight = 0 ; 
						
						min_weight = for_update[ non_infinite_used[j] ].print_path_weight() ;  
						update_row_index = for_update[ non_infinite_used[j] ].print_desitination_vertex_index()  ;	

						/*-----------------------------------------------------------------------------
						 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						 這裡呢 我想平行化  ， 要把下面的 ， for  index 弄成 signed  

						long int size_of_row_index = (long int)table[update_row_index].size()  ;  
						#pragma omp parallel for ordered 
						for(long int j = 0  ; j < size_of_row_index  ; j++  )

						case_one 是指 有找到 相同 目的點的vertex , 可能可以 簡化
						case_two 則是 都沒有找到  所以 push_back 

						用case_match 來代替好了 ， false 相當於上面的 case_two 
									   true  則是 case_one
						 *-----------------------------------------------------------------------------*/
						for(unsigned int k = 0  ; k < table[update_row_index].size()  ; k++  ){
							
							 bool case_match = false ; 
							 int  match_index_in_for_update = -1 ; 
							/*-----------------------------------------------------------------------------
							 *  這裡面 ， 應該還要一個for , 用來看 ， 這個轉接點 ， 能否 簡短
							 *  main_vertex 到其它點的距離(能簡短的 ， 當然是 有相同 目對點的) ，
							 *  如都遇不到相同目的點的 ， 代表 ， 這是個新的路 ， 可以放進來 
							 *  也就是說  有二種case 
							 *-----------------------------------------------------------------------------*/
							 for(unsigned int l = 0 ; l < for_update.size() ; l++ ){
							 	if( table[update_row_index][k].print_desitination_vertex_index() ==
								    for_update[l].print_desitination_vertex_index()              ){
									case_match = true ;  
									match_index_in_for_update = l ; 
									break ;
								}// 抓出  有相同 目地點的vertex位置 
							 }// end of for_update  

							if( case_match == true  ){
								// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
								// 值的比較不能含等號
								if( table[update_row_index][k].print_path_weight() + min_weight < 
								    for_update[ match_index_in_for_update ].print_path_weight()          	 ){
								    // 把轉接點放入　　，　更新weight ,
									 for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 

									 for_update[match_index_in_for_update].set_path_weight( 
									 	table[update_row_index][k].print_path_weight() + min_weight 
									 ) ; 

									 for_update[match_index_in_for_update].set_used( false ) ; 	
								}

								/*-----------------------------------------------------------------------------
								 *  這個case 在我的新方法中 ， 是不會出現的 
								// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
								else if( table[update_row_index][k].print_infinite() == false 			      && 
									 for_update[match_index_in_for_update].print_infinite() == true 	      )
								{
								    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
								    for_update[match_index_in_for_update].set_infinite( false ); 
								    for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 
								    for_update[match_index_in_for_update].set_path_weight( 
								    	table[update_row_index][k].print_path_weight() + min_weight 
									) ; 
								}
								 *  
								 *-----------------------------------------------------------------------------*/
							 
							}// end of case_match == true
							else if( case_match == false ){
								// 除了 ， 把這個 path_node 塞進去外 ， 還要多設定 ， 
								// 它是由 誰 轉接過來的  , 因為原本是到不了它的 
								for_update.push_back( table[update_row_index][k] ) ; 
								for_update[ for_update.size() - 1 ].set_trans_node( update_row_index )  ; 
							    	for_update[ for_update.size() - 1 ].set_path_weight( 
									table[update_row_index][k].print_path_weight() + min_weight 
								) ; 
							 
							}// end of case_match == false

						}// end of loop k

						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[ non_infinite_used[j] ].set_used(true);

					 
					}// end of scan all non_infinite_used.size() 

				}// end of non_infinite_used.size() > 0

				// 一旦 找到目的點 ， 但不能確定 它就是最短的 ， 所以  
				// 基本上呢 ， 由開始點 ， 往外search 的routing 如同是一個
				// 廣度search  , 等同一個同心圓向外擴張 ， 當safe_count == 1
				// 代表 第一次碰到目的點 ， 但不能確定是最短的 所以 
				// 我讓它有容錯的空間  ， 同心圓再擴張一點
				// 一個edge 的最長距離便是 ， 二個斜邊的相加 ， 
				// 最大的更新次數 ， 便是 >= safe_range 
				//tail_index_in_for_update != - 1  && safe_count >= safe_range
				if( tail_index_in_for_update != - 1  ){
					ok = false ;  
				}

				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				// 沒有點可以拿來　當轉接　，　輸入的點跟obstacle數目　是３０，３０　４０，４０　時
				// 會有 中間沒路的情況　
				// 要連線的[0][0] , [0][1] , 分別是頭跟尾丫　, 第一個到達的點 ， 一定是最短的嗎 
				// if( tail_index_in_for_update != -1 && for_update[tail_index_in_for_update].print_infinite() == true  ){
					/*-----------------------------------------------------------------------------
					 * 				Check Code , May 9  
					cout << endl ; 
					cout << " No path can reach " << endl ;  
					// 用來看是第幾個edge出這樣的錯 
					cout << "variable i is : " << i << endl ; 
					cout << "how many vertex in vertex_need : " << vertex_need.size() << endl ; 
					//cout << "size in edge_need : " << edge_need.size() << endl ; 
					cout << "size in obstacle_crossed :  " << SG_edge[i].which_obstacle_cross_size() << endl ; 
					SG_edge[i].print_kruskal_edge_information() ; 
					 *-----------------------------------------------------------------------------*/

					/*-----------------------------------------------------------------------------
					 *  奇怪了 ， 為什麼 ， 轉接點全是－1 ， 它跟本沒有在做 東西的轉接丫 ， 難怪會找不到
					 *  路 
					cout << "here is debug info : " << endl ; 
					for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
						if(table[0][k].print_infinite() == true ){
							cout << " xx " ;   
						}
						else{
							cout << " " << table[0][k].print_path_weight() << " " ;	
						}	
					}
					cout << endl ; 

					cout << "here is trans_node info : " << endl ; 
					for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
						cout << " " << table[0][j].print_trans_node() << " " ; 
					}
					cout << endl ; 
					 *-----------------------------------------------------------------------------*/
					/*-----------------------------------------------------------------------------
					 *  
						cout << " edge_need 's information :" ;
						for(unsigned int j = 0  ; j < edge_need.size() ; j++  ){
							edge_need[j].print_kruskal_edge_information() ;
						}
					 *-----------------------------------------------------------------------------*/

				// }// end of for_update[tail_index].print_infinite() == true



			 // ok 的意思是說　，　找不到點可以　再拿來做轉接　，　所以　source 能到的點　，　應該都找完了
			 }while( ok == true );
			 // cout << "Updating sucessful !!" << endl ; 
 
			
			 /*-----------------------------------------------------------------------------
			  *  
			 source line 的內容　更新好了　，　接下來要抓出　，　中間轉接了那些點
			 再　把兩兩相鄰的vertex ，　其直線給抓出來　放入　Obstacle_Avoding_Kruskal_Tree 
			 source 的index 是　０　，　destination 的index 是　１
			  所以我判斷　有沒有路過去　，　便是看　index 1 的那個位置　0 能否過的去　

			  下面這塊  有點忘了 ， 先看看 再改吧  
			  這裡是要抓出  從起始點到 終點 ， 中間經過那些點 ， 
			  是一串 vertex_id 的串列 
			  這邊的 index 是要到 vertex_need 中用的
			  *-----------------------------------------------------------------------------*/

			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index_in_for_update ; 
			 // cout << "Find the chain of vertex " << endl ; 
			 do{
			 	// cout << "Do i get in 593 " << endl ; 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				// 丫 ， 我之道了 ， 有可能是 start_index 這裡面沒有東西
				// 所以就當了  , 我 tail_index_in_for_update 一定能抓到東西嗎
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}
			 	// 看來 這裡面 ， 要用個loop  , 來抓出 desitination_vertex_id , 是我們要的 那個path_node 
				// 再一個個 trace 回去 
			 	// 要確定裡面真是有東西　才行
				else {
					trans_order.push_back( for_update[start_index].print_trans_node() );	

					for(unsigned int j = 0 ; j < for_update.size() ; j++ ){
						if( for_update[j].print_desitination_vertex_index() == for_update[start_index].print_trans_node() ){
							start_index = j  ; 
							break ; 
						}
					}// end of loop j 
				} 

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 

			 /*-----------------------------------------------------------------------------
			  *  cout << " how many size in edge_need : " << edge_need.size() << endl ;  
			 
			 cout << "here is trans_order info : " <<  endl  ; 
			 for(unsigned int i = 0 ; i < trans_order.size() ; i++  ){
			  	cout << " " << trans_order[i] << " " ; 
			 }
			cout << endl << endl ;  
			bool start_position = false ; 
			for(unsigned int j = 0 ; j < trans_order.size() - 1 ; j++ ){

				// bool insert_ok_not = false ; 
				// 當遇到第一個 這樣的case ,  那個edge 要加 ， 之後的 就不要了  
				int node1_x = 	vertex_need[ trans_order[j] ].print_x_position() ; 
				int node1_y = 	vertex_need[ trans_order[j] ].print_y_position() ; 

				int node2_x = 	vertex_need[ trans_order[j + 1] ].print_x_position() ; 
				int node2_y = 	vertex_need[ trans_order[j + 1] ].print_y_position() ; 

				while( start_position == false && j < trans_order.size() - 1 ){
					int temp1_x = 	vertex_need[ trans_order[j] ].print_x_position() ; 
					int temp1_y = 	vertex_need[ trans_order[j] ].print_y_position() ; 

					int temp2_x = 	vertex_need[ trans_order[j + 1] ].print_x_position() ; 
					int temp2_y = 	vertex_need[ trans_order[j + 1] ].print_y_position() ; 

					if( 	 my_level_info.already_in_OAST[temp1_x][ temp1_y]  == true   && 
					    	 my_level_info.already_in_OAST[temp2_x][ temp2_y]  == false  ){
						start_position = true ; 	 
					}
					else {
						obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
											  vertex_need[ trans_order[j] ].print_y_position() ,
											  vertex_need[ trans_order[j + 1] ].print_x_position() ,
											  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
						my_level_info.already_in_OAST[temp1_x][ temp1_y] =  true ; 
						my_level_info.already_in_OAST[temp2_x][ temp2_y] =  true ; 
						j++ ;	
					}


				}// end of while 

				// true 就可以從這裡  ， 開始加 edge 了 
				if( start_position == true  ){
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 


					my_level_info.already_in_OAST[node1_x][ node1_y] =  true ; 
					my_level_info.already_in_OAST[node2_x][ node2_y] =  true ; 
				}// end of  start_position == true
			}// end of loop j 
			  *-----------------------------------------------------------------------------*/


			int trans_order_signed = trans_order.size() - 1 ; 	
			for(int j = 0 ; j < trans_order_signed ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
				 	// 如果  ， 前後二個pin都在同個subtree  , 那代表連通  ， 只要全部的pin點都連通 ， 
					// 那就不用 再加edge了    , 如果 全部的 pin點都 連通了   ， 就放個break 出去 
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
					/*-----------------------------------------------------------------------------
					my_level_info.already_in_OAST[ vertex_need[ trans_order[j] ].print_x_position() ]
								     [ vertex_need[ trans_order[j] ].print_y_position() ] =  true ; 
					my_level_info.already_in_OAST[vertex_need[ trans_order[j + 1] ].print_x_position()]
								     [vertex_need[ trans_order[j + 1] ].print_y_position()] =  true ; 
					 *-----------------------------------------------------------------------------*/
				 }
			}// end of 加入已避開obstacle的edge 

			/*-----------------------------------------------------------------------------
				 好了， trans_order 內的東西， 我已經抓其了 ， 現在要把 edge 放入
				 Obstacle_Avoding_Kruskal_Tree 內 , 我想把兩兩的index 對的vertex 抓出來放入即可 
			 *  看來是要在這裡加了 ， 把要加入的edge 跟 obstacle_avoding_Kruskal 內的edge 
			 *  做比對， 只要裡面已經有這個edge 便不加入
			 *  下面的for 的j 上限是 －1 ， 因為 內容會用到 j + 1  
			int trans_order_signed = trans_order.size() - 1 ; 	
			for(int j = 0 ; j < trans_order_signed ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() )   ); 
					my_level_info.already_in_OAST[ vertex_need[ trans_order[j] ].print_x_position() ]
								     [ vertex_need[ trans_order[j] ].print_y_position() ] =  true ; 
					my_level_info.already_in_OAST[vertex_need[ trans_order[j + 1] ].print_x_position()]
								     [vertex_need[ trans_order[j + 1] ].print_y_position()] =  true ; 
				 }
			}// end of 加入已避開obstacle的edge 
			 *-----------------------------------------------------------------------------*/


			
			

		}// end of else , 這個區塊　我一次只改一個edge  , end of if
		       
	}// scan 過Kruskal_Tree 中所有的edge ，　並對false的做修改, a for loop  use variable i , so else_block cant use variable i 

	if( overlap_debug == true  ){
		cout << "Line 3151 "  << endl ; 
	}
	

	// cout << "enter the solve cycle problem  "  << endl ;  少了 sort  會有差嗎  

	// sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight_decreasing );	 
	// 所在的index 便是它的編號  ,  group的編號  ， 這樣就之道要往那個箱子放了  
	// count  用來記  要放第幾個箱子   , count 指向 空箱 
	vector<edge> seperate_group[ obstacle_avoding_Kruskal.size() ] ; 
	int count = 0 ; 
	// int index_delete = -1 ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){

		bool delete_or_not = false  ;
		// cout << "i = "	<< i << endl ; 
		
		// cout << " Do i get in "  << endl ;  , 我這樣 ， 沒有把後面的給刪掉 
		
		 // bool connect_graph_complete = false ;  
		int node1_x = 	obstacle_avoding_Kruskal[ i ].print_node1_x() ; 
		int node1_y = 	obstacle_avoding_Kruskal[ i ].print_node1_y() ; 

		int node2_x = 	obstacle_avoding_Kruskal[ i ].print_node2_x() ; 
		int node2_y = 	obstacle_avoding_Kruskal[ i ].print_node2_y() ; 
		// 先判斷那二個點 ， 有沒有其它的subtree   , 沒有的話  ， 就依 count 值 放到 
		// 指定的箱子裡面 
		if( my_level_info.connected_graph[node1_x][node1_y] == -1 &&  my_level_info.connected_graph[node2_x][node2_y] == -1 ){
			// 給值 ， 再放到  ， vector 中   
			my_level_info.connected_graph[node1_x][node1_y] = count ; 
			my_level_info.connected_graph[node2_x][node2_y] = count ; 
			// 之後是要利用 ， 下面二個點 ， 去改上面的id 
			seperate_group[count].push_back( obstacle_avoding_Kruskal[i] ) ; 
			count++ ; 
			//cout << "line   "<< 1771  << endl ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] >= 0 &&  my_level_info.connected_graph[node2_x][node2_y] == -1 ){
			// 給值 ， 再放到  ， vector 中   
			my_level_info.connected_graph[node2_x][node2_y] = my_level_info.connected_graph[node1_x][node1_y] ; 
			// 下面這  好像不是放在 count  所在的箱子 ， 而是放在  >= 0 的那個箱子才對 
			seperate_group[ my_level_info.connected_graph[node1_x][node1_y] ].push_back( obstacle_avoding_Kruskal[i] ) ; 
			// cout << "line   "<< 1778  << endl ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] == -1 &&  my_level_info.connected_graph[node2_x][node2_y] >= 0 ){
			// 給值 ， 再放到  ， vector 中   
			my_level_info.connected_graph[node1_x][node1_y] = my_level_info.connected_graph[node2_x][node2_y]; 
			seperate_group[  my_level_info.connected_graph[node2_x][node2_y] ].push_back(  obstacle_avoding_Kruskal[i] ) ; 
			// cout << "line   "<< 1784 << endl ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] >= 0 &&  my_level_info.connected_graph[node2_x][node2_y] >= 0 && 
			 my_level_info.connected_graph[node1_x][node1_y]  == my_level_info.connected_graph[node2_x][node2_y]           ){
		 
			// cout << "line   "<< 1844  << endl ; 
		 	// 這裡  要刪掉  那個 弄出cycle的edge 
			delete_or_not = true ; 
			obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
			i-- ; 
		}
		else if( my_level_info.connected_graph[node1_x][node1_y] >= 0 &&  my_level_info.connected_graph[node2_x][node2_y] >= 0 && 
			 my_level_info.connected_graph[node1_x][node1_y]  != my_level_info.connected_graph[node2_x][node2_y] 	   ){

			 // 為什麼進來這個  會出錯呢  
			// cout << "line   "<< 1788 << endl ; 
			
			// 下面二個loop 是依大小 ， 來看 ， 是誰要合併誰  
			// 由下面一行可以之道 ， node2 的 被蓋掉  ， 也就是說  ， node1所在的 subtree  , 量比較大  
			int which_group = my_level_info.connected_graph[node2_x][node2_y] ; 
			int des_group   = my_level_info.connected_graph[node1_x][node1_y] ; 

			my_level_info.connected_graph[node2_x][node2_y] = my_level_info.connected_graph[node1_x][node1_y] ; 
			seperate_group[  des_group ].push_back(  obstacle_avoding_Kruskal[i] ) ; 

			// cout <<  "seperate_group[ which_group ].size() = " << seperate_group[ which_group ].size() << endl ; 
			for(unsigned int k = 0 ; k < seperate_group[ which_group ].size() ; k++ ){
				// cout << " j  =" << j << endl ; 
				// 班過去 node1 那個箱子 ， 順便改  ， 在 connected_graph  上的group id 	 
				int tmp_x = -1 , tmp_y = -1 ; 
				tmp_x = seperate_group[ which_group ][k].print_node1_x() ; 
				tmp_y = seperate_group[ which_group ][k].print_node1_y() ; 
				my_level_info.connected_graph[tmp_x][tmp_y] =  my_level_info.connected_graph[node1_x][node1_y] ; 

				tmp_x = seperate_group[ which_group ][k].print_node2_x() ; 
				tmp_y = seperate_group[ which_group ][k].print_node2_y() ; 
				my_level_info.connected_graph[tmp_x][tmp_y] =  my_level_info.connected_graph[node1_x][node1_y] ; 
				// 要把東西都給班過去   , 為什麼下面這行  會讓我的code 當了呢   , 告夭 ， 我之道為什麼了拉  
				// 我把  node1箱子的index 給了  node2  , 又 從它那取箱子的index , 等於我都在處理同個箱子 按  
				seperate_group[ des_group ].push_back( seperate_group[which_group][k] ) ; 
			}// end of loop j 

			// 班完後 ， 把另一邊的東西給刪了 
			seperate_group[  which_group  ].clear()  ;

		}// 都在不同的subtree上 

		/*-----------------------------------------------------------------------------
		// 要怎麼判斷 ， 所有的pin點 都已經連通了呢  , 有二個樹 要合併的時侯 ， 再去比  ， 
		// 合併時 ， 二個tree 的subtree id 都會 統一了  ， 此時 ， 只要用my_level_info.only_pin 去 connect_graph
		// 中 ， 看  是否每個pin的subtree id 都一樣  ， 就可以了  如果 有一個不一樣  ， 就跳出來 
		for( unsigned int j = 0 ; j < my_level_info.only_pin.size() - 1 ; j++ ){
			 
			int  x =  my_level_info.only_pin[j].print_x_position() ;  
			int  y =  my_level_info.only_pin[j].print_y_position() ;  

			int  xx =  my_level_info.only_pin[ j + 1].print_x_position() ;  
			int  yy =  my_level_info.only_pin[ j + 1].print_y_position() ;  

			if(  my_level_info.connected_graph[x][y] >= 0 && my_level_info.connected_graph[xx][yy] >= 0 ){
			 
				if( my_level_info.connected_graph[x][y] != my_level_info.connected_graph[xx][yy] ){
					connect_graph_complete = false ;  
					break ; 
				}
				else if( my_level_info.connected_graph[x][y] == my_level_info.connected_graph[xx][yy] ){
					connect_graph_complete = true ;  
				}

			}


		}// scan all pins 

		if( connect_graph_complete == true ){
			break ;  
		}
		 *-----------------------------------------------------------------------------*/
		 if( delete_or_not == false  ){
			my_level_info.degree_per_vertex[ node1_x ][ node1_y]++ ; 
			my_level_info.degree_per_vertex[ node2_x ][ node2_y]++ ; 
		 }

	}// scan all edges in obstacle_avoding_Kruskal 

	// 這裡的話 ， 要把 ， index_delete 後面  的edge 都給刪了  , index_delete 一直是－1 哎  , 怪了  那代表沒有改到丫  
	// cout << "index_delete = " << index_delete << endl ; 
	// obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_delete + 1 , obstacle_avoding_Kruskal.end() ) ; 
	sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight );	 
	 unsigned int  index_b = 0 ; 




	bool  find_index_b = false ; 
	// 下面這個 loop i 用來 指向 第一個 edge 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size()  ; i++ ){

		int temp_overlap_value = 0 ; 
		int max_overlap_value = 0 ; 

		// 有可能發生  index_a 沒指到人丫 , 
		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
		 
			// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
			// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
			// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
			if( two_edges_overlap_or_not_by_new_method( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) == true  ){
				// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		 , 一定要找到底 ， 
				temp_overlap_value = cal_overlap_value( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] );

				if( temp_overlap_value > max_overlap_value ){
					max_overlap_value = temp_overlap_value ; 	 
					index_b = j ;
					find_index_b = true ; 
				}

			}// end of already_rectilinear[i] == false

		}// end of loop j 

		// 有找到  才要做事 ， 因為這裡只要考慮 二個edge 的  , 只要有找到就可以了  ， 以乎不用去分它的case  是怎樣  
		if(  find_index_b == true  ){
		 
			vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
			vector<vertex> all_vertex ; 
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node1_x() 
							, obstacle_avoding_Kruskal[i].print_node1_y() , obstacle_avoding_Kruskal[i].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node2_x() 
							, obstacle_avoding_Kruskal[i].print_node2_y() , obstacle_avoding_Kruskal[i].print_node2_z() ) );

			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node1_x() 
						, obstacle_avoding_Kruskal[index_b].print_node1_y() , obstacle_avoding_Kruskal[index_b].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node2_x() 
						, obstacle_avoding_Kruskal[index_b].print_node2_y() , obstacle_avoding_Kruskal[index_b].print_node2_z() ) );
			// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
			// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
			unsigned int index_i = 0 , index_j = 0  ; 
			bool find_common_vertex = false ; 
			for(unsigned int h = 0 ; h < all_vertex.size() - 2 ; h++ ){

				for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

					if( all_vertex[h].print_x_position() == all_vertex[j].print_x_position() &&
					    all_vertex[h].print_y_position() == all_vertex[j].print_y_position() ){

						index_i = h ;  
						index_j = j ;  
						common_vertex.set_position( all_vertex[h].print_x_position() , all_vertex[h].print_y_position() );
						common_vertex.set_z_position( all_vertex[h].print_z_position() );
						find_common_vertex = true ; 
						break ; 

					}// end of ix
				 
				}// loop j 
			 
			}// loop i 
			// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
			// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置 
			if( find_common_vertex == true ){
			 
			 	// 這裡是刪掉那個相同的 vertex 
				all_vertex.erase( all_vertex.begin() + index_j ) ; 
				all_vertex.erase( all_vertex.begin() + index_i ) ; 
				// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
				near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
				near.set_z_position( all_vertex[0].print_z_position() );
				far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
				far.set_z_position( all_vertex[1].print_z_position() );

			}

			// 這裡就是要看 ， 第三個edge有沒有比較短  ， 而且  是否在 OASG 內   , 因為還有 vertex_id 的關係 ， 
			// 所以  ， 很容易之道  ， 那條edge 有沒有在裡面  , 那個 記 點跟那些點有連 edge的那個資結 ， 叫什
			// 麼名字丫   , 喔  ， 叫table  , 先抓出  ， 它們三個點 分別是誰吧   , 點抓好了  ， 那接下來  便是看 
			// near 跟far 這二點的長度 ， 有沒有比  其它二個 edge 短  ， 有的話  ， 才要 換過來  ,  拿最長的 來 換 
			int third_edge_weight = abs( near.print_x_position() - far.print_x_position() ) + 
						abs( near.print_y_position() - far.print_y_position() )   ; 

			// 下面二個  用來記  ， third_edge 的 ， 兩端點的vertex_id , 看是否存在著這個edge 
			int one = -1 , two = -1 ; 
			one = my_level_info.vertex_id[ near.print_x_position() ][ near.print_y_position() ] ; 
			two = my_level_info.vertex_id[ far.print_x_position()  ][  far.print_y_position() ] ; 
			bool exist_or_not = false ; 
			for(unsigned int j = 0 ; j < table[one].size() ; j++ ){
				if( table[one][j].print_desitination_vertex_index() == two ){
					exist_or_not = true ;  
					break ; 
				}
			}// scan all vertex on index one 

			// 等一下  有可能  ， 二個都比它大  ， 所以 要先比看看 那二個edge 誰比較大   , 等於至少要放在一邊  
			if( exist_or_not == true 									    && 
			    obstacle_avoding_Kruskal[i].print_weight() >=  obstacle_avoding_Kruskal[index_b].print_weight() && 
			    obstacle_avoding_Kruskal[i].print_weight() >   third_edge_weight   				    ){

				// cout <<  " Do i find some case can change " << endl  ; 
				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				int x1 = obstacle_avoding_Kruskal[i].print_node1_x() ; 
				int y1 = obstacle_avoding_Kruskal[i].print_node1_y() ; 
				int x2 = obstacle_avoding_Kruskal[i].print_node2_x() ; 
				int y2 = obstacle_avoding_Kruskal[i].print_node2_y() ; 

				my_level_info.degree_per_vertex[ x1 ][ y1 ]-- ; 
				my_level_info.degree_per_vertex[ x2 ][ y2 ]-- ; 
				
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i ) ; 
				i-- ; 
				my_level_info.degree_per_vertex[ near.print_x_position() ][ near.print_y_position() ]++ ; 
				my_level_info.degree_per_vertex[ far.print_x_position() ][ far.print_y_position() ]++ ; 

				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// 確實有改到 ， 但  量不多  ， 像 800 x 200 也才改了 3 個而已 
				// cout << "Do i change somthing " << endl ; 
			 
			}
			else if( exist_or_not == true 										&& 
				 obstacle_avoding_Kruskal[index_b].print_weight() >  obstacle_avoding_Kruskal[i].print_weight()	&&
				 obstacle_avoding_Kruskal[index_b].print_weight() >  third_edge_weight 				){

				// cout <<  " Do i find some case can change " << endl  ; 
				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				int x1 = obstacle_avoding_Kruskal[index_b].print_node1_x() ; 
				int y1 = obstacle_avoding_Kruskal[index_b].print_node1_y() ; 
				int x2 = obstacle_avoding_Kruskal[index_b].print_node2_x() ; 
				int y2 = obstacle_avoding_Kruskal[index_b].print_node2_y() ; 
				my_level_info.degree_per_vertex[ x1 ][ y1 ]-- ; 
				my_level_info.degree_per_vertex[ x2 ][ y2 ]-- ; 

				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_b ) ; 

				my_level_info.degree_per_vertex[ near.print_x_position() ][ near.print_y_position() ]++ ; 
				my_level_info.degree_per_vertex[ far.print_x_position() ][ far.print_y_position() ]++ ; 

				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// cout << "Do i change somthing " << endl ; 
			}// end of else if

		}// 兩個  ， 有找到配對的  


	}// scan all elements on obstacle_avoding_Kruskal , what we need is false element , not do rectilinear 


	bool all_ok = false ; 
	while( all_ok == false ){
		all_ok = true ; 	
		// 拿掉的edge , degree 記得要--
		for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){

			// cout << "i = "	<< i << endl ; 
			
			// cout << " Do i get in "  << endl ;  , 我這樣 ， 沒有把後面的給刪掉 
			
			 // bool connect_graph_complete = false ;  
			int node1_x = 	obstacle_avoding_Kruskal[ i ].print_node1_x() ; 
			int node1_y = 	obstacle_avoding_Kruskal[ i ].print_node1_y() ; 

			int node2_x = 	obstacle_avoding_Kruskal[ i ].print_node2_x() ; 
			int node2_y = 	obstacle_avoding_Kruskal[ i ].print_node2_y() ; 

			// 在 pin_plane 及 obstacle_plane 中  ， 都同為true的  就是 
			if( my_level_info.degree_per_vertex[ node1_x ][ node1_y] == 1      && 
			    my_level_info.pin_plane[ node1_x ][ node1_y] == false          ){
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
				i-- ; 
				all_ok = false ; 
				my_level_info.degree_per_vertex[ node1_x ][ node1_y]-- ; 
				my_level_info.degree_per_vertex[ node2_x ][ node2_y]-- ; 
			}
			else if( my_level_info.degree_per_vertex[ node2_x ][ node2_y] == 1      && 
				 my_level_info.pin_plane[ node2_x ][ node2_y] == false     	){
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
				i-- ; 
				all_ok = false ; 
				my_level_info.degree_per_vertex[ node1_x ][ node1_y]-- ; 
				my_level_info.degree_per_vertex[ node2_x ][ node2_y]-- ; 
			}

		}// end of scan all obstacle_avoding_Kruskal 

	}// end of while 




	 // cout << endl << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 按  ， 我不懂哎 ， 我少了個loop 直接去取那個值  ， 怎麼還比較慢呢  
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = -1 ; 

	}// end of for

	/*-----------------------------------------------------------------------------
	 * 這裡的話  ， 在做完OAST 後   ， 其實路徑還不是最短的  ， 所以 要再加一些東西 ， 去保證
	 * 這個tree  是最短的  ， 而  從圖上  觀察到 ， 能這樣 換的 其實  它們是 rectilinear 化的
	 * case 2 , 3 所以  只要 有發生這些case  , 我就看  ， 另一個 沒加入的edge  , 加入後 如果有比較短
	 * 而且 是在 OASG 內的edge  , 那就可以了  , 喔 ， 下面這行  ， 我只要有載入 那個 header file , 那裡面的
	 * func 就可以直接用了  ，  哎  ， 現在就排好序的話 ， 那 我之後rectilinear 的那個 就不用再排了 
	 *  
	 *  首先呢 ， 傳進來的 obstacle_avoding_Kruskal 是要 處理成 rectilinear 的 ， 而  我已經對 ， weight 做排序了 
	 *  要二個變數 ， 之道index 指向那二個 ， 以及  二個bool 去看說  ， 現在這二個的情況是 ， 那種case
	 *  也用一個ok變數 ， 直到 obstacle_avoding_Kruskal 內的東西都沒了  都跑出while 
	 *
	 *  哇 ， 這塊 會讓我的東西有問題  ， 就是解決cycle的那塊  的  
	sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight );	 
	 unsigned int  index_b = 0 ; 




	bool  find_index_b = false ; 
	// 下面這個 loop i 用來 指向 第一個 edge 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size()  ; i++ ){

		int temp_overlap_value = 0 ; 
		int max_overlap_value = 0 ; 

		// 有可能發生  index_a 沒指到人丫 , 
		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
		 
			// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
			// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
			// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
			if( two_edges_overlap_or_not_by_new_method( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) == true  ){
				// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		 , 一定要找到底 ， 
				temp_overlap_value = cal_overlap_value( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] );

				if( temp_overlap_value > max_overlap_value ){
					max_overlap_value = temp_overlap_value ; 	 
					index_b = j ;
					find_index_b = true ; 
				}

			}// end of already_rectilinear[i] == false

		}// end of loop j 

		// 有找到  才要做事 ， 因為這裡只要考慮 二個edge 的  , 只要有找到就可以了  ， 以乎不用去分它的case  是怎樣  
		if(  find_index_b == true  ){
		 
			vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
			vector<vertex> all_vertex ; 
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node1_x() 
							, obstacle_avoding_Kruskal[i].print_node1_y() , obstacle_avoding_Kruskal[i].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node2_x() 
							, obstacle_avoding_Kruskal[i].print_node2_y() , obstacle_avoding_Kruskal[i].print_node2_z() ) );

			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node1_x() 
						, obstacle_avoding_Kruskal[index_b].print_node1_y() , obstacle_avoding_Kruskal[index_b].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node2_x() 
						, obstacle_avoding_Kruskal[index_b].print_node2_y() , obstacle_avoding_Kruskal[index_b].print_node2_z() ) );
			// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
			// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
			unsigned int index_i = 0 , index_j = 0  ; 
			bool find_common_vertex = false ; 
			for(unsigned int h = 0 ; h < all_vertex.size() - 2 ; h++ ){

				for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

					if( all_vertex[h].print_x_position() == all_vertex[j].print_x_position() &&
					    all_vertex[h].print_y_position() == all_vertex[j].print_y_position() ){

						index_i = h ;  
						index_j = j ;  
						common_vertex.set_position( all_vertex[h].print_x_position() , all_vertex[h].print_y_position() );
						common_vertex.set_z_position( all_vertex[h].print_z_position() );
						find_common_vertex = true ; 
						break ; 

					}// end of ix
				 
				}// loop j 
			 
			}// loop i 
			// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
			// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置 
			if( find_common_vertex == true ){
			 
			 	// 這裡是刪掉那個相同的 vertex 
				all_vertex.erase( all_vertex.begin() + index_j ) ; 
				all_vertex.erase( all_vertex.begin() + index_i ) ; 
				// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
				near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
				near.set_z_position( all_vertex[0].print_z_position() );
				far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
				far.set_z_position( all_vertex[1].print_z_position() );

			}

			// 這裡就是要看 ， 第三個edge有沒有比較短  ， 而且  是否在 OASG 內   , 因為還有 vertex_id 的關係 ， 
			// 所以  ， 很容易之道  ， 那條edge 有沒有在裡面  , 那個 記 點跟那些點有連 edge的那個資結 ， 叫什
			// 麼名字丫   , 喔  ， 叫table  , 先抓出  ， 它們三個點 分別是誰吧   , 點抓好了  ， 那接下來  便是看 
			// near 跟far 這二點的長度 ， 有沒有比  其它二個 edge 短  ， 有的話  ， 才要 換過來  ,  拿最長的 來 換 
			int third_edge_weight = abs( near.print_x_position() - far.print_x_position() ) + 
						abs( near.print_y_position() - far.print_y_position() )   ; 

			// 下面二個  用來記  ， third_edge 的 ， 兩端點的vertex_id , 看是否存在著這個edge 
			int one = -1 , two = -1 ; 
			one = my_level_info.vertex_id[ near.print_x_position() ][ near.print_y_position() ] ; 
			two = my_level_info.vertex_id[ far.print_x_position()  ][  far.print_y_position() ] ; 
			bool exist_or_not = false ; 
			for(unsigned int j = 0 ; j < table[one].size() ; j++ ){
				if( table[one][j].print_desitination_vertex_index() == two ){
					exist_or_not = true ;  
					break ; 
				}
			}// scan all vertex on index one 

			// 等一下  有可能  ， 二個都比它大  ， 所以 要先比看看 那二個edge 誰比較大   , 等於至少要放在一邊  
			if( exist_or_not == true 									    && 
			    obstacle_avoding_Kruskal[i].print_weight() >=  obstacle_avoding_Kruskal[index_b].print_weight() && 
			    obstacle_avoding_Kruskal[i].print_weight() >   third_edge_weight   				    ){

				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i ) ; 
				i-- ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// 確實有改到 ， 但  量不多  ， 像 800 x 200 也才改了 3 個而已 
				// cout << "Do i change somthing " << endl ; 
			 
			}
			else if( exist_or_not == true 										&& 
				 obstacle_avoding_Kruskal[index_b].print_weight() >  obstacle_avoding_Kruskal[i].print_weight()	&&
				 obstacle_avoding_Kruskal[index_b].print_weight() >  third_edge_weight 				){

				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_b ) ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// cout << "Do i change somthing " << endl ; 
			}// end of else if

		}// 兩個  ， 有找到配對的  


	}// scan all elements on obstacle_avoding_Kruskal , what we need is false element , not do rectilinear 



	 // cout << endl << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	for( int j = 0 ; j < my_level_info.total ; j++ ){
			// 按  ， 我不懂哎 ， 我少了個loop 直接去取那個值  ， 怎麼還比較慢呢  
			int x = vertex_need[j].print_x_position() ; 
			int y = vertex_need[j].print_y_position() ; 
			my_level_info.vertex_id[x][y] = -1 ; 

	}// end of for
	 *-----------------------------------------------------------------------------*/

}// end of func build_obstacle_avoding_kruskal 

/*-----------------------------------------------------------------------------
 *    我有點忘了 各個參數是存什麼東西了 ， 想一想  ， 第一個好像是  果然是 存Kruskal_Tree 的edge
 *    第二個理應是 ， 用來 存處理完的output , 而 OASG 是我建routing table 的藍圖 ， 不過在這裡
 *    我還要多把 vertical_edge 的東西也放進來 ，  如果routing table 能吃到 3D 的資訊 那我還有什麼東西
 *    是要多加的呢 
 *    第三個參數 的OASG 我打算存起各層的 2D OASG 的 edge 
 *-----------------------------------------------------------------------------*/
void obstacle_avoding_kruskal::build_obstacle_avoding_kruskal_with_projection_vertex( vector<edge> &ThreeD_SG_edge  , vector<edge> &obstacle_avoding_Kruskal   , 
										      vector<edge> &vertical_edge ,  vector<level_info> &every_level_info      ){
	
	/*-----------------------------------------------------------------------------
	 *  				step 1 ,2 
	 *  		check edge ok or not 
	 *  		理應是obstacle的數目比較少　，　所以
	 *  		裡面的loop 是用　my_level_info.complete_obstacle_information
	 *  		這塊 想必也是要改的， 我要把 在各層上的edge 抓出跟它 相 cross 的
	 *  		obstacle  , 跨層的edge 就不用了 ， 不過要把它的edge屬性 設成 false 
	 *  		用routing table 來找路 
	 *  		這裡呢 我想還有一個問題 ， 那就是 ， 不同層 它的obstacle_id 可能會一樣
	 *  		所以我要確定分的層 是正確的 
	 *
	 *  		每個edge 都要跟那一層的 obstacle 做比對 ， 感覺有點麻煩 花時間  
	 *  		又或 是 我依每個 obstacle 的 x 位置來做 分類  ， 只看那幾個位置的obstacle 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i  = 0 ; i < ThreeD_SG_edge.size() ; i++){
		
		/*-----------------------------------------------------------------------------
		 *  cout << "Here is edge information " << endl ;  
		ThreeD_SG_edge[i].print_kruskal_edge_information() ; 
		cout << "min_x = " ;
		cout << ThreeD_SG_edge[i].print_min_x() << endl ; 
		cout << "max_x = " ;
		cout << ThreeD_SG_edge[i].print_max_x() << endl ;
		cout << "min_y = " ;
		cout << ThreeD_SG_edge[i].print_min_y() << endl ; 
		cout << "max_y = " ;
		cout << ThreeD_SG_edge[i].print_max_y() << endl ;
		cout << "kruskal_edge_ok_or_not = "  <<  ThreeD_SG_edge[i].print_kruskal_edge_ok_not() << endl ;
		 *-----------------------------------------------------------------------------*/
		
		if( ThreeD_SG_edge[i].print_node1_z() != ThreeD_SG_edge[i].print_node2_z() ){

			/*-----------------------------------------------------------------------------
			 false 代表 這個 edge 要重新找路 ， 因為它 cross 到了 obstacle , 
			 那我現在紀路的這種edge  , 要不要 記 它跟那些obstacle cross呢  , 雖然 
			 我有記 edge 跟那些obstacle 相 cross 但是 ， 後面好像沒有用到這個資訊丫
			 *-----------------------------------------------------------------------------*/
			 ThreeD_SG_edge[i].set_kruskal_edge_ok_not(false);
			 ThreeD_SG_edge[i].set_cross_level_edge_y_or_n( true ) ; 
		}// 跨層的 Kruskal edge 
		else{

			int which_level = ThreeD_SG_edge[i].print_node1_z() ; 	
			for(int j = ThreeD_SG_edge[i].print_min_x() ; j <= ThreeD_SG_edge[i].print_max_x() ; j++ ){

				map<int,obstacle>::iterator it ;  
				int key = 0 ; 
				for(unsigned int k = 0 ; k < every_level_info[which_level].obstacle_cross[j].size() ; k++ ){
				 
					 key = every_level_info[which_level].obstacle_cross[j][k] ;
					 it = every_level_info[which_level].obstacle_map.find( key ) ; 
					 bool first_case = false , Second_case = false ; 

					 if( ThreeD_SG_edge[i].print_max_x() >= (*it).second.print_ld_corner_x() && 
					     ThreeD_SG_edge[i].print_min_x() <= (*it).second.print_ru_corner_x() ){
						first_case = true ;  
					 }
					 if( ThreeD_SG_edge[i].print_max_y() >= (*it).second.print_ld_corner_y() && 
					     ThreeD_SG_edge[i].print_min_y() <= (*it).second.print_ru_corner_y() ){
						Second_case = true ;  
					 }

					 /*-----------------------------------------------------------------------------
					  *  cout << "first_case = " << first_case << endl; 
					 cout << "Second_case = " << Second_case << endl ;
					 if( first_case == true || Second_case == true ){
						my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
					 }
					 cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
					  *-----------------------------------------------------------------------------*/
					  

					 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
					 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
					 //  就不用再往下比了  , 所以我多了個 break ; 
					 if( first_case == true && Second_case == true ){
						ThreeD_SG_edge[i].set_kruskal_edge_ok_not(false);
						ThreeD_SG_edge[i].add_which_obstacle_cross( (*it).second.print_obstacle_id() );
						break ; 
					 }// end of  first_case == true && Second_case == true
				}// end of loop k 
			 
			}// 只 scan SG 有踫到的x 範圍  

			/*-----------------------------------------------------------------------------
			int which_level = ThreeD_SG_edge[i].print_node1_z() ; 	
			for(unsigned int j  = 0 ; j < every_level_info[ which_level ].complete_obstacle_information.size()  ; j++){
			
				 bool first_case = false , Second_case = false ; 

				 if( ThreeD_SG_edge[i].print_max_x() >= every_level_info[ which_level ].complete_obstacle_information[j].print_ld_corner_x() && 
				     ThreeD_SG_edge[i].print_min_x() <= every_level_info[ which_level ].complete_obstacle_information[j].print_ru_corner_x() ){
					first_case = true ;  
				 }
				 if( ThreeD_SG_edge[i].print_max_y() >= every_level_info[ which_level ].complete_obstacle_information[j].print_ld_corner_y() && 
				     ThreeD_SG_edge[i].print_min_y() <= every_level_info[ which_level ].complete_obstacle_information[j].print_ru_corner_y() ){
					Second_case = true ;  
				 }

				 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false  ,  這裡好像只有設定  是否有 obstacle 擋到而已
				 //  並不用設說  誰擋到 
				 if( first_case == true && Second_case == true ){
				 	// 喔 ， 這裡是要看  各個kruskal_edge中 ， 那些是要換換徑的 ， 而這裡是 只要看某一層的
					ThreeD_SG_edge[i].set_kruskal_edge_ok_not(false);
					// 由 edge 的z 坐標 我就可以之道 它是在那一層了  ,  感覺下面沒有要用的意思 先弄掉
					//ThreeD_SG_edge[i].add_which_obstacle_cross( every_level_info[ which_level ].
					//complete_obstacle_information[j].print_obstacle_id() );
					 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
					 //  就不用再往下比了  , 所以我多了個 break ; 
					 break ; 
				 }
				//cout << "kruskal_edge_ok_or_not = "  <<  ThreeD_SG_edge[i].print_kruskal_edge_ok_not() << endl ;
			}// end of for　j
			 *-----------------------------------------------------------------------------*/

		}// 在同一層的edge 

	}// end of 抓出edge  是否要更動　，　以及overlap 的obstacle 有那些  ,  指這個kruskal edge 跟那些obstacle有碰到 ， 要改路徑的
	

	

	/*-----------------------------------------------------------------------------
	 * 				Check
	 * 		every Kruskal edge ok or not 
	 * 		and which obstacles it cross  .    
	 *        cout << endl << endl << "Check Kruskal_Tree edge " << endl ; 
		for(unsigned int i = 0 ; i < ThreeD_SG_edge.size() ; i++  ){
		
		cout << "This is edge number " << i+1 << endl ; 
		// 奇怪了　，　只要是設定false 那就一定有值會出來才對丫  
		ThreeD_SG_edge[i].print_kruskal_edge_information() ; 
	}

	 *-----------------------------------------------------------------------------*/
	 
	/*-----------------------------------------------------------------------------
	 *  				Step 3,4
	 *  		因為我已經抓出　有cross 的obstacle了　，　要抓其四個端點
	 *  		很簡單　，　接下來呢，　就建個新的vector 用來存
	 *  		obstacle_avoding_by_kruskal_Tree , 
	 *  		為true 的edge 就直接放入　，　false 的便修改後　，再放入
	 *
	  下面這二個  好像後面還會用到的樣子
	  沒有 只有  第一個 要記得放外面就好了
	  vector<edge> obstacle_avoding_Kruskal ; 	
	  我想呢 ， 我全部建成一張大的table 來查就好了 
	  哎  ， 下面的  ， vertex_seperate , 好像可以拿掉 ， 我在 projection_vertex 內  ， 
	  已經有分類了  ， 所以下面不用再做了   , 哎  不太一樣哎  ， 下面是把各層的  都放進來 
	  但我 projection_vertex 內的  ， 是 各層存自已的   , 哎 ， 分層去找 好像有比較好  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> vertex_need ; 
	// 下面這個結構 等於是把  ， 各層的東西 ， 都放到這層來了 
	// vector<vertex> vertex_seperate[ every_level_info[0].plane_width ] ; 
	// vector<vertex> vertex_seperate[ every_level_info.size() ][ every_level_info[0].plane_width ] ; 
	// vector<vertex> vertex_seperate[ every_level_info.size() ][ every_level_info[0].plane_width ] ; 
	// hash_map<vertex ,int, hash_vertex , equal_vertex > hmap;

	int count = -1 ; 
	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		for( unsigned int j = 0 ; j < every_level_info[i].array_vertex_vector.size() ; j++ ){
			count++ ;
			// 因為我已經寫好了 vertex 的copy constructor 所以vertex 可以直接給了  	
			vertex_need.push_back( every_level_info[i].array_vertex_vector[j] );	
			// 下面三行 ， 應該就把  ， plane上  ， 有點的地方  設定好他的id了  
			//
			int x = every_level_info[i].array_vertex_vector[j].print_x_position() ; 
			int y = every_level_info[i].array_vertex_vector[j].print_y_position() ; 
			every_level_info[i].vertex_id[x][y] = count ; 
		}// 把每一層的vertex 都給抓出來  

	}// end of for
	
	/*-----------------------------------------------------------------------------
	for(unsigned int i = 0 ; i < vertex_need.size() ; i++ ){
		vertex_need[i].set_id( i ) ; 
		vertex_seperate[ vertex_need[i].print_x_position() ].push_back( vertex_need[i] ) ; 	 
	}
	 *-----------------------------------------------------------------------------*/

	 // 這裡把表的初值給建好了
	 // 應該把　path_node 換成　table_node 比較好　
	 vector<path_node> table[vertex_need.size()];			 

	/*-----------------------------------------------------------------------------
	 *  				下面這塊　是建好OAST 的核心
	 *  				要用的edge　我都抓好了　，　
	 *  				接下來便是要抓shortest path 
	 *  	先建出一張表來，　各個vertex 到別的點的weight , 要建幾乘幾的表，
	 *  	可由vertex_need 去看　，　再由edge_need 去一個個掃　，　來建立
	 *  	weight , 那　無法到達的 無限要怎麼代表它呢　
	 *  	, 我建了二個data structure , node_need 跟　edge_need 
	 *  	,node_need中的第０跟１　，　理應是　我要的source跟desitination 
	 *  	表的index 順序，　我想就跟node_need 一樣　，　
	 *  	而表的內容　，　就掃過edge_need , 來一個個建　，　一個edge_need
	 *  	要建　二個位置的值　像(3,1) (1,3)　這樣　
	 *-----------------------------------------------------------------------------*/



	 /*-----------------------------------------------------------------------------
	  接下來　　，　把值弄進去　，　每個edge 都跟vertex_need 掃過一遍
	  抓出edge  二端點的node index ,  下面這個loop 的 OASG 也要改
	  因為是2D 上的 要用的話 必需要是3D 的才行,  我要把各層的OASG edge 跟 vertical_edge 
	  我打算用 threeD_OAThreeD_SG_edge 存起各層的OASG edges ,  以及 vertical_edge , 因為 
	  3D  就是能夠在 ， 不同層下 找到別的路 連到同曾的另外一個 點 
	  *-----------------------------------------------------------------------------*/
	 vector<edge> threeD_OASG_edge ; 
	 //  先把各層的edge 丟進去 , 以及 垂直的 edge 丟入 , 
	 //  這樣我才好建起table , 
	 for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		 for( unsigned int j = 0 ; j < every_level_info[i].my_level_OASG.size() ; j++  ){
			threeD_OASG_edge.push_back( every_level_info[i].my_level_OASG[j] ) ; 	 
		 }

	 }// 總共有幾層

	// vertical_edge 是外面傳進來的 ， 因為 不可能存在各層中 
	for( unsigned int i = 0 ; i < vertical_edge.size()  ; i++ ){
		threeD_OASG_edge.push_back( vertical_edge[i] ) ; 
	}// 垂直的edge 

	// 這裡只是藉由 ， 3D_OASG 的edge 來填滿table內要用的資訊而已 
	// 好像  一個func內 只有一個地方能加那個  平行化的東西  
	for(unsigned int j = 0  ; j < threeD_OASG_edge.size() ; j++  ){

		// 內層的for  , 要抓出兩端點的index  , 下面能加 break 嗎  
		int index_one = -1 , index_two = -1  ; 


		int x = threeD_OASG_edge[j].print_node1_x() , y = threeD_OASG_edge[j].print_node1_y() , z = threeD_OASG_edge[j].print_node1_z() ; 
		index_one = every_level_info[z].vertex_id[x][y]  ; 
		/*-----------------------------------------------------------------------------
		 *   , 怎麼我用hash map 比我原本的還要慢呢  
		vertex temp( threeD_OASG_edge[j].print_node1_x() , threeD_OASG_edge[j].print_node1_y()  , threeD_OASG_edge[j].print_node1_z() ) ; 
		index_one = hmap[temp] ; 

		index_one = hmap[ vertex( threeD_OASG_edge[j].print_node1_x() , threeD_OASG_edge[j].print_node1_y()  , threeD_OASG_edge[j].print_node1_z() ) ] ; 
		還是  我不同層  用不同的 hash_map 

		int x = threeD_OASG_edge[j].print_node1_x() , z = threeD_OASG_edge[j].print_node1_z() ; 
		for(unsigned int k = 0 ; k < vertex_seperate[ z ][ x ].size() ; k++ ){

			if( threeD_OASG_edge[j].print_node1_y() == vertex_seperate[z][ x ][k].print_y_position() &&
			    threeD_OASG_edge[j].print_node1_z() == vertex_seperate[z][ x ][k].print_z_position() ){
				index_one =  vertex_seperate[z][ x ][k].get_id() ;  
				break ; 
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/



		int xx = threeD_OASG_edge[j].print_node2_x() , yy = threeD_OASG_edge[j].print_node2_y() , zz = threeD_OASG_edge[j].print_node2_z() ; 
		index_two = every_level_info[zz].vertex_id[xx][yy]  ; 
		/*-----------------------------------------------------------------------------
		int xx = threeD_OASG_edge[j].print_node2_x() , zz = threeD_OASG_edge[j].print_node2_z() ; 
		for(unsigned int k = 0 ; k < vertex_seperate[zz][ xx ].size() ; k++ ){

			if( threeD_OASG_edge[j].print_node2_y() == vertex_seperate[zz][ xx ][k].print_y_position() &&
			    threeD_OASG_edge[j].print_node2_z() == vertex_seperate[zz][ xx ][k].print_z_position() ){
				index_two = vertex_seperate[zz][ xx ][k].get_id() ;  
				break; 
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/



		/*-----------------------------------------------------------------------------
		for(unsigned int k = 0  ;  k < vertex_need.size() ; k++  ){
			// 有的vertex 會對不到
			if(threeD_OASG_edge[j].print_node1_x() == vertex_need[k].print_x_position() &&
			   threeD_OASG_edge[j].print_node1_y() == vertex_need[k].print_y_position() &&
			   threeD_OASG_edge[j].print_node1_z() == vertex_need[k].print_z_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_one = k ;  
			}
			if(threeD_OASG_edge[j].print_node2_x() == vertex_need[k].print_x_position() &&
			   threeD_OASG_edge[j].print_node2_y() == vertex_need[k].print_y_position() &&
			   threeD_OASG_edge[j].print_node2_z() == vertex_need[k].print_z_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_two = k ;  
			}
		}// 　抓好二個端點的index 了
		 *-----------------------------------------------------------------------------*/
		// 要把值給送進table 
		if( index_one != -1 && index_two != -1 ){
			table[index_one].push_back( path_node( index_one ,  threeD_OASG_edge[j].print_weight() , index_two ) ) ; 
			table[index_two].push_back( path_node( index_two ,  threeD_OASG_edge[j].print_weight() , index_one ) ) ; 
			/*-----------------------------------------------------------------------------
			table[index_one][index_two].set_infinite(false) ;
			// 這邊的 set_path_weight 我想也要改 因為原本是只有 2D 的判斷 
			table[index_one][index_two].set_path_weight( threeD_OASG_edge[j].print_3D_weight() );
			table[index_two][index_one].set_infinite(false) ;
			table[index_two][index_one].set_path_weight( threeD_OASG_edge[j].print_3D_weight() );	 
			 *  
			 *-----------------------------------------------------------------------------*/
		}
		
	}// end of 抓二端點的　index , 且埴滿table內的值　

	/*-----------------------------------------------------------------------------
	 * 				Check Code , May 9 
	cout << endl << "here become single source all desitination : " << endl ;  
	 *-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
	 *  
	 在這之後的東西 就沒有跑出來了 ， 所以 我想 理應是某個func 被我改了 
	 但 effect 到這裡  
	 下面這塊是核心了    , 不之道有沒有東西要改的 , 這塊好像 沒用到 我先前記的
	 有 cross的 obstacle 丫 喔 ，  好像沒必要用到  , 忘記下面的order  是幹麻的了
	 總之是要依順序去 執行thread 但是  ， 我拿掉它  好像沒有差   , 本來想說會不會快一點的說  
	int SG_edge_change = ThreeD_SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	 *-----------------------------------------------------------------------------*/
	int SG_edge_change = ThreeD_SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	for(int i = 0  ; i < SG_edge_change ; i++  ){

		// cout << ThreeD_SG_edge[i].print_kruskal_edge_ok_not() << endl ; 
		// true 代表 ， 它不用找替換的路徑
		if( ThreeD_SG_edge[i].print_kruskal_edge_ok_not() == true  ){
			obstacle_avoding_Kruskal.push_back( ThreeD_SG_edge[i] ) ; 
		}
		// 只要是   , 進下面這區塊的 便是要找路替換的 
		else {
			
			/*-----------------------------------------------------------------------------
			 *  我要把edge_need 放到上面去做 ， 這裡變成 ， 抓edge兩端點的index出來即可
			for(unsigned  int j = 0 ; j < vertex_need.size() ; j++ ){
				// 有可能是這裡沒改到 ， 少加了 z 的參數  ， 可能會拿到別層的東西
				if( ThreeD_SG_edge[i].print_node1_x() == vertex_need[j].print_x_position() &&
				    ThreeD_SG_edge[i].print_node1_y() == vertex_need[j].print_y_position() &&
				    ThreeD_SG_edge[i].print_node1_z() == vertex_need[j].print_z_position() ){
					head_index = j ;  
				}
				else if( ThreeD_SG_edge[i].print_node2_x() == vertex_need[j].print_x_position() &&
				         ThreeD_SG_edge[i].print_node2_y() == vertex_need[j].print_y_position() &&
					 ThreeD_SG_edge[i].print_node2_z() == vertex_need[j].print_z_position() ){
					tail_index = j ;  
				}
			}// end of for
			 *-----------------------------------------------------------------------------*/


			int head_index = -1 , tail_index = -1 ; 	

			int  x = ThreeD_SG_edge[i].print_node1_x() , y = ThreeD_SG_edge[i].print_node1_y() ,  z =  ThreeD_SG_edge[i].print_node1_z() ; 
			head_index =  every_level_info[z].vertex_id[x][y] ;  

			int xx = ThreeD_SG_edge[i].print_node2_x() , yy = ThreeD_SG_edge[i].print_node2_y() ,  zz = ThreeD_SG_edge[i].print_node2_z() ; 
			tail_index =  every_level_info[zz].vertex_id[xx][yy] ;  
			/*-----------------------------------------------------------------------------
			int head_index = -1 , tail_index = -1 ; 	
			int  x = ThreeD_SG_edge[i].print_node1_x() ,  z =  ThreeD_SG_edge[i].print_node1_z() ; 
			for(unsigned int j = 0 ; j < vertex_seperate[z][ x ].size() ; j++ ){

				if( ThreeD_SG_edge[i].print_node1_y() == vertex_seperate[z][ x ][j].print_y_position() &&
				    ThreeD_SG_edge[i].print_node1_z() == vertex_seperate[z][ x ][j].print_z_position() ){
					head_index =  vertex_seperate[z][ x ][j].get_id() ;  
					break ; 
				}

			}// end of for

			int xx = ThreeD_SG_edge[i].print_node2_x() , zz = ThreeD_SG_edge[i].print_node2_z() ; 
			for(unsigned int j = 0 ; j < vertex_seperate[ zz ][ xx ].size() ; j++ ){

				if( ThreeD_SG_edge[i].print_node2_y() == vertex_seperate[ zz ][ xx ][j].print_y_position() &&
				    ThreeD_SG_edge[i].print_node2_z() == vertex_seperate[ zz ][ xx ][j].print_z_position() ){
					tail_index = vertex_seperate[ zz ][ xx ][j].get_id() ;  
					break ; 
				}

			}// end of for
			 *-----------------------------------------------------------------------------*/


			//cout << "my_level_info.total = " << my_level_info.total << ",  vertex_need.size " << vertex_need.size() << endl ;   

			// 這裡要寫些code來看資訊 ， 用來抓bug 的 ， 要印出 ， 此時的i 是多少
			// 以及這個要被改的edge  資訊 ， 
			//cout << " i = " << i << endl ; 
			//ThreeD_SG_edge[i].print_kruskal_edge_information() ; 
			
			/*-----------------------------------------------------------------------------
			 *  
			// 這裡 印出個花費相鄰矩陣來看看 好了  
			cout << "Here print out weight table : " << i << endl ; 
			for(unsigned int j = 0 ; j < vertex_need.size() ; j++ ){
			 	for(unsigned int k = 0 ; k < vertex_need.size() ; k++ ){

					if( j == k ){
						table[j][j].set_infinite(false) ;
						table[j][j].set_path_weight( 0 );	 
					}

					if(table[j][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[j][k].print_path_weight() << " " ;	
					}

				}// inner loop j 
				cout << endl << endl ;; 
			}// end of 印出矩陣來看看
			 *-----------------------------------------------------------------------------*/
	
			/*-----------------------------------------------------------------------------
			 *  表建好後　，　接下來便是執行　one source all destination 的algorithm 
			先抓source 行中　weight  最小的　，　及　此edge node2是誰
			把weight加到　node2那行　再跟 source 行比對，　　如果值有變小　，　或
			原本true 的　變false ，　也可把值納入　
			抓出shortest path 中間要的轉接點後　，　可以用　之前的方法　，　從edge_need 中
			抓出　其edge_need 含這些點集的edge 即可
			還是說　用個　bool 紀路這個點　是否當過轉接點,  轉接點 只會有一個 ， 初值我設成 －1 
			只要是 source  能直接到達的 ， 都設成 －1 ， 
			原先的表是拿來查的 ， 不能直接休改 ， 
			 *-----------------------------------------------------------------------------*/
			 
			 /*-----------------------------------------------------------------------------
			  *  
			  因為table的直是用來查的 不是用來更新的 ， 不然  會出點錯誤 ，
			  所以這裡我要建個一行path_node 來 ， 存原始行的data , 
			  等路抓出來以後 ， 再用這個還原回去 
			  *-----------------------------------------------------------------------------*/
			 // 這是 要被更新的那一行
			 vector<path_node> for_update;			 
			// 我只要把上面的for_update變成 for_update[2] , 就可以一個從頭處理， 
			// 一個從尾巴了 
			for(unsigned int j = 0  ; j <  table[head_index].size() ; j++ ){
				for_update.push_back( table[head_index][j] ); 
			}


			/*-----------------------------------------------------------------------------
			 *  
			  如果抓不到一個可以轉接的點　，　便跳出去了　
			  這個 do while 把整個表給更新好了　，　接下來便是看我怎麼抓中間的edge了
			 *-----------------------------------------------------------------------------*/
			 bool ok = true ; 
			 int tail_index_in_for_update = -1 ; 
			 // int safe_count = 0 ; 
			 do{
			 	
			 	vector<int> non_infinite_used  ; 
				for(unsigned int j = 0  ; j < for_update.size() ; j++  ){

					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					// 這邊有個新的問題 ， 因為我剛初是把全部的東西都放進來
					// 所以  會有 無限的這個情況  那我有改過了  ， 不會有這種
					// 情況了 ， 要不要拿掉呢 
					if( for_update[j].print_used() == false  ){
						non_infinite_used.push_back( j ) ; 
					}// end of 沒當過轉接的點 

					// 基本上 ， 只要 這個目地點給放入 ， 它在for_update 中的 index 都不會變了 
					if( for_update[j].print_desitination_vertex_index() == tail_index ){
						// safe_count++ ;  
						tail_index_in_for_update = j ; 
					}

					/*-----------------------------------------------------------------------------
					 *  
					 *  		origin code 
					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					if( for_update[j].print_infinite() == false && for_update[j].print_used() == false  ){
						non_infinite_used.push_back(j); 
					}
					 *-----------------------------------------------------------------------------*/
				} 


				 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
				if( non_infinite_used.size() > 0 ){

					for(unsigned int j = 0 ; j < non_infinite_used.size() ; j++ ){

						 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
						int update_row_index = 0 ; 
						int min_weight = 0 ; 
						
						min_weight = for_update[ non_infinite_used[j] ].print_path_weight() ;  
						update_row_index = for_update[ non_infinite_used[j] ].print_desitination_vertex_index()  ;	

						/*-----------------------------------------------------------------------------
						 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						 這裡呢 我想平行化  ， 要把下面的 ， for  index 弄成 signed  

						long int size_of_row_index = (long int)table[update_row_index].size()  ;  
						#pragma omp parallel for ordered 
						for(long int j = 0  ; j < size_of_row_index  ; j++  )

						case_one 是指 有找到 相同 目的點的vertex , 可能可以 簡化
						case_two 則是 都沒有找到  所以 push_back 

						用case_match 來代替好了 ， false 相當於上面的 case_two 
									   true  則是 case_one
						 *-----------------------------------------------------------------------------*/
						for(unsigned int k = 0  ; k < table[update_row_index].size()  ; k++  ){
							
							 bool case_match = false ; 
							 int  match_index_in_for_update = -1 ; 
							/*-----------------------------------------------------------------------------
							 *  這裡面 ， 應該還要一個for , 用來看 ， 這個轉接點 ， 能否 簡短
							 *  main_vertex 到其它點的距離(能簡短的 ， 當然是 有相同 目對點的) ，
							 *  如都遇不到相同目的點的 ， 代表 ， 這是個新的路 ， 可以放進來 
							 *  也就是說  有二種case 
							 *-----------------------------------------------------------------------------*/
							 for(unsigned int l = 0 ; l < for_update.size() ; l++ ){
							 	if( table[update_row_index][k].print_desitination_vertex_index() ==
								    for_update[l].print_desitination_vertex_index()              ){
									case_match = true ;  
									match_index_in_for_update = l ; 
									break ;
								}// 抓出  有相同 目地點的vertex位置 
							 }// end of for_update  

							if( case_match == true  ){
								// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
								// 值的比較不能含等號
								if( table[update_row_index][k].print_path_weight() + min_weight < 
								    for_update[ match_index_in_for_update ].print_path_weight()          	 ){
								    // 把轉接點放入　　，　更新weight ,
									 for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 

									 for_update[match_index_in_for_update].set_path_weight( 
									 	table[update_row_index][k].print_path_weight() + min_weight 
									 ) ; 

									 for_update[match_index_in_for_update].set_used( false ) ; 	
								}

								/*-----------------------------------------------------------------------------
								 *  這個case 在我的新方法中 ， 是不會出現的 
								// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
								else if( table[update_row_index][k].print_infinite() == false 			      && 
									 for_update[match_index_in_for_update].print_infinite() == true 	      )
								{
								    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
								    for_update[match_index_in_for_update].set_infinite( false ); 
								    for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 
								    for_update[match_index_in_for_update].set_path_weight( 
								    	table[update_row_index][k].print_path_weight() + min_weight 
									) ; 
								}
								 *  
								 *-----------------------------------------------------------------------------*/
							 
							}// end of case_match == true
							else if( case_match == false ){
								// 除了 ， 把這個 path_node 塞進去外 ， 還要多設定 ， 
								// 它是由 誰 轉接過來的  , 因為原本是到不了它的 
								for_update.push_back( table[update_row_index][k] ) ; 
								for_update[ for_update.size() - 1 ].set_trans_node( update_row_index )  ; 
							    	for_update[ for_update.size() - 1 ].set_path_weight( 
									table[update_row_index][k].print_path_weight() + min_weight 
								) ; 
							 
							}// end of case_match == false

						}// end of loop k

						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[ non_infinite_used[j] ].set_used(true);

					 
					}// end of scan all non_infinite_used.size() 

					/*-----------------------------------------------------------------------------
					 *  			Origin Code 
					int update_row_index = 0 ; 
					int min_weight = 0 ; 
					unsigned int k = 0 ; 
					
					do{
						min_weight = for_update[ non_infinite_used[k] ].print_path_weight() ;  
						update_row_index = non_infinite_used[k] ;	

						// 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						// 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						// 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						for(unsigned int j = 0  ; j < table[update_row_index].size()  ; j++  ){

							// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
							// 值的比較不能含等號
							if( table[update_row_index][j].print_infinite() == false && for_update[j].print_infinite() == false &&  
							    table[update_row_index][j].print_path_weight() + min_weight < for_update[j].print_path_weight() ){
							    // 把轉接點放入　　，　更新weight ,
								 for_update[j].set_trans_node(update_row_index) ; 
								 for_update[j].set_path_weight( table[update_row_index][j].print_path_weight() + min_weight ) ; 
								 for_update[j].set_used( false ) ; 	
							}
							// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
							else if( table[update_row_index][j].print_infinite() == false && for_update[j].print_infinite() == true ){
							    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
							    for_update[j].set_infinite( false ); 
							    for_update[j].set_trans_node(update_row_index) ; 
							    for_update[j].set_path_weight( table[update_row_index][j].print_path_weight() + min_weight ) ; 
							}
							


						}// end of loop j
						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[update_row_index].set_used(true);
						k++ ;
					}while( k < non_infinite_used.size() );
					 *-----------------------------------------------------------------------------*/

					if( tail_index_in_for_update != - 1    ){
						ok = false ;  
					}

				}// non_infinite_used > 0 才進來 

				
				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				// 沒有點可以拿來　當轉接　，　輸入的點跟obstacle數目　是３０，３０　４０，４０　時
				// 會有 中間沒路的情況　
				// 要連線的[0][0] , [0][1] , 分別是頭跟尾丫　, 第一個到達的點 ， 一定是最短的嗎 
				// if( tail_index_in_for_update != -1 && for_update[tail_index_in_for_update].print_infinite() == true  ){
					/*-----------------------------------------------------------------------------
					 * 				Check Code , May 9  
					cout << endl ; 
					cout << " No path can reach " << endl ;  
					// 用來看是第幾個edge出這樣的錯 
					cout << "variable i is : " << i << endl ; 
					cout << "how many vertex in vertex_need : " << vertex_need.size() << endl ; 
					//cout << "size in edge_need : " << edge_need.size() << endl ; 
					cout << "size in obstacle_crossed :  " << SG_edge[i].which_obstacle_cross_size() << endl ; 
					SG_edge[i].print_kruskal_edge_information() ; 
					 *-----------------------------------------------------------------------------*/

					/*-----------------------------------------------------------------------------
					 *  奇怪了 ， 為什麼 ， 轉接點全是－1 ， 它跟本沒有在做 東西的轉接丫 ， 難怪會找不到
					 *  路 
					cout << "here is debug info : " << endl ; 
					for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
						if(table[0][k].print_infinite() == true ){
							cout << " xx " ;   
						}
						else{
							cout << " " << table[0][k].print_path_weight() << " " ;	
						}	
					}
					cout << endl ; 

					cout << "here is trans_node info : " << endl ; 
					for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
						cout << " " << table[0][j].print_trans_node() << " " ; 
					}
					cout << endl ; 
					 *-----------------------------------------------------------------------------*/
					/*-----------------------------------------------------------------------------
					 *  
						cout << " edge_need 's information :" ;
						for(unsigned int j = 0  ; j < edge_need.size() ; j++  ){
							edge_need[j].print_kruskal_edge_information() ;
						}
					 *-----------------------------------------------------------------------------*/

				//}// end of for_update[tail_index].print_infinite() == true



			 // ok 的意思是說　，　找不到點可以　再拿來做轉接　，　所以　source 能到的點　，　應該都找完了
			 }while( ok == true );
 
			
			 /*-----------------------------------------------------------------------------
			  *  
			 source line 的內容　更新好了　，　接下來要抓出　，　中間轉接了那些點
			 再　把兩兩相鄰的vertex ，　其直線給抓出來　放入　Obstacle_Avoding_Kruskal_Tree 
			 source 的index 是　０　，　destination 的index 是　１
			  所以我判斷　有沒有路過去　，　便是看　index 1 的那個位置　0 能否過的去　
			  *-----------------------------------------------------------------------------*/

		
			 /*-----------------------------------------------------------------------------
			  *                       	origin Code   
			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index ; 
			 do{
			 	// 要確定裡面真是有東西　才行
				if( for_update[start_index].print_infinite() == false  && for_update[start_index].print_trans_node() != -1  ){
					trans_order.push_back( for_update[start_index].print_trans_node() );	
					start_index = for_update[start_index].print_trans_node() ; 
				} 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 
			  *-----------------------------------------------------------------------------*/
			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index_in_for_update ; 
			 // cout << "Find the chain of vertex " << endl ; 
			 do{
			 	// cout << "Do i get in 593 " << endl ; 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				// 丫 ， 我之道了 ， 有可能是 start_index 這裡面沒有東西
				// 所以就當了  , 我 tail_index_in_for_update 一定能抓到東西嗎
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}
			 	// 看來 這裡面 ， 要用個loop  , 來抓出 desitination_vertex_id , 是我們要的 那個path_node 
				// 再一個個 trace 回去 
			 	// 要確定裡面真是有東西　才行
				else {
					trans_order.push_back( for_update[start_index].print_trans_node() );	

					for(unsigned int j = 0 ; j < for_update.size() ; j++ ){
						if( for_update[j].print_desitination_vertex_index() == for_update[start_index].print_trans_node() ){
							start_index = j  ; 
							break ; 
						}
					}// end of loop j 
				} 

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 

			 /*-----------------------------------------------------------------------------
			  *  
			 cout << " how many size in edge_need : " << edge_need.size() << endl ;  
			 
			 cout << "here is trans_order info : " <<  endl  ; 
			 for(unsigned int i = 0 ; i < trans_order.size() ; i++  ){
			  	cout << " " << trans_order[i] << " " ; 
			 }
			cout << endl << endl ;  
			  *-----------------------------------------------------------------------------*/


			/*-----------------------------------------------------------------------------
				 好了， trans_order 內的東西， 我已經抓其了 ， 現在要把 edge 放入
				 Obstacle_Avoding_Kruskal_Tree 內 , 我想把兩兩的index 對的vertex 抓出來放入即可 
			 *  看來是要在這裡加了 ， 把要加入的edge 跟 obstacle_avoding_Kruskal 內的edge 
			 *  做比對， 只要裡面已經有這個edge 便不加入
			 *  下面的for 的j 上限是 －1 ， 因為 內容會用到 j + 1  , 下面三行  用來看  抓 cp edge 
			 *  會不會花太多時間  ， 其實還好哎  
			clock_t start , end ; 
			double  dif  ; 
			start = clock() ; 
			 *-----------------------------------------------------------------------------*/
			for(unsigned int j = 0 ; j < trans_order.size() - 1 ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_z() == vertex_need[ trans_order[j] ].print_z_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_z() == vertex_need[ trans_order[j + 1 ] ].print_z_position() 
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					         obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					         obstacle_avoding_Kruskal[k].print_node1_z() == vertex_need[ trans_order[j + 1 ] ].print_z_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_z() == vertex_need[ trans_order[j] ].print_z_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j] ].print_z_position() ,  
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_z_position() )   ); 
				 }
			}// end of 加入已避開obstacle的edge 
			// end = clock() ; 
			// dif = (double) ( end - start ) ; 
			// cout << "check edge cp  = "  << (double) ( dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ; 
			
			

		}// end of else , 這個區塊　我一次只改一個edge  , end of if
		       
	}// scan 過Kruskal_Tree 中所有的edge ，　並對false的做修改, a for loop  use variable i , so else_block cant use variable i 
	
	// cout << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	// 下面這裡  ， 要把 給了的id 還原  , 而原始的值 ， 就是 －1 
	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		for( unsigned int j = 0 ; j < every_level_info[i].array_vertex_vector.size() ; j++ ){
			int x = every_level_info[i].array_vertex_vector[j].print_x_position() ; 
			int y = every_level_info[i].array_vertex_vector[j].print_y_position() ; 
			every_level_info[i].vertex_id[x][y] = -1 ; 
		}// 把每一層的vertex 都給抓出來  

	}// end of for

}// end of build_obstacle_avoding_kruskal 


void obstacle_avoding_kruskal::build_obstacle_avoding_kruskal_with_projection_vertex_imp( vector<edge> &ThreeD_SG_edge  , vector<edge> &obstacle_avoding_Kruskal   , 
										      vector<edge> &vertical_edge ,  vector<level_info> &every_level_info      ){
	
	/*-----------------------------------------------------------------------------
	 *  				step 1 ,2 
	 *  		check edge ok or not 
	 *  		理應是obstacle的數目比較少　，　所以
	 *  		裡面的loop 是用　my_level_info.complete_obstacle_information
	 *  		這塊 想必也是要改的， 我要把 在各層上的edge 抓出跟它 相 cross 的
	 *  		obstacle  , 跨層的edge 就不用了 ， 不過要把它的edge屬性 設成 false 
	 *  		用routing table 來找路 
	 *  		這裡呢 我想還有一個問題 ， 那就是 ， 不同層 它的obstacle_id 可能會一樣
	 *  		所以我要確定分的層 是正確的 
	 *
	 *  		每個edge 都要跟那一層的 obstacle 做比對 ， 感覺有點麻煩 花時間  
	 *  		又或 是 我依每個 obstacle 的 x 位置來做 分類  ， 只看那幾個位置的obstacle 
	 *-----------------------------------------------------------------------------*/
	for(unsigned int i  = 0 ; i < ThreeD_SG_edge.size() ; i++){
		
		/*-----------------------------------------------------------------------------
		 *  cout << "Here is edge information " << endl ;  
		ThreeD_SG_edge[i].print_kruskal_edge_information() ; 
		cout << "min_x = " ;
		cout << ThreeD_SG_edge[i].print_min_x() << endl ; 
		cout << "max_x = " ;
		cout << ThreeD_SG_edge[i].print_max_x() << endl ;
		cout << "min_y = " ;
		cout << ThreeD_SG_edge[i].print_min_y() << endl ; 
		cout << "max_y = " ;
		cout << ThreeD_SG_edge[i].print_max_y() << endl ;
		cout << "kruskal_edge_ok_or_not = "  <<  ThreeD_SG_edge[i].print_kruskal_edge_ok_not() << endl ;
		 *-----------------------------------------------------------------------------*/
		
		if( ThreeD_SG_edge[i].print_node1_z() != ThreeD_SG_edge[i].print_node2_z() ){

			/*-----------------------------------------------------------------------------
			 false 代表 這個 edge 要重新找路 ， 因為它 cross 到了 obstacle , 
			 那我現在紀路的這種edge  , 要不要 記 它跟那些obstacle cross呢  , 雖然 
			 我有記 edge 跟那些obstacle 相 cross 但是 ， 後面好像沒有用到這個資訊丫
			 *-----------------------------------------------------------------------------*/
			 ThreeD_SG_edge[i].set_kruskal_edge_ok_not(false);
			 ThreeD_SG_edge[i].set_cross_level_edge_y_or_n( true ) ; 
		}// 跨層的 Kruskal edge 
		else{

			int which_level = ThreeD_SG_edge[i].print_node1_z() ; 	
			for(int j = ThreeD_SG_edge[i].print_min_x() ; j <= ThreeD_SG_edge[i].print_max_x() ; j++ ){

				map<int,obstacle>::iterator it ;  
				int key = 0 ; 
				for(unsigned int k = 0 ; k < every_level_info[which_level].obstacle_cross[j].size() ; k++ ){
				 
					 key = every_level_info[which_level].obstacle_cross[j][k] ;
					 it = every_level_info[which_level].obstacle_map.find( key ) ; 
					 bool first_case = false , Second_case = false ; 

					 if( ThreeD_SG_edge[i].print_max_x() >= (*it).second.print_ld_corner_x() && 
					     ThreeD_SG_edge[i].print_min_x() <= (*it).second.print_ru_corner_x() ){
						first_case = true ;  
					 }
					 if( ThreeD_SG_edge[i].print_max_y() >= (*it).second.print_ld_corner_y() && 
					     ThreeD_SG_edge[i].print_min_y() <= (*it).second.print_ru_corner_y() ){
						Second_case = true ;  
					 }

					 /*-----------------------------------------------------------------------------
					  *  cout << "first_case = " << first_case << endl; 
					 cout << "Second_case = " << Second_case << endl ;
					 if( first_case == true || Second_case == true ){
						my_level_info.complete_obstacle_information[j].print_out_for_check()   ;  
					 }
					 cout << "End of " <<    my_level_info.complete_obstacle_information[j].print_obstacle_id() << endl ;
					  *-----------------------------------------------------------------------------*/
					  

					 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false 
					 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
					 //  就不用再往下比了  , 所以我多了個 break ; 
					 if( first_case == true && Second_case == true ){
						ThreeD_SG_edge[i].set_kruskal_edge_ok_not(false);
						ThreeD_SG_edge[i].add_which_obstacle_cross( (*it).second.print_obstacle_id() );
						break ; 
					 }// end of  first_case == true && Second_case == true
				}// end of loop k 
			 
			}// 只 scan SG 有踫到的x 範圍  

			/*-----------------------------------------------------------------------------
			int which_level = ThreeD_SG_edge[i].print_node1_z() ; 	
			for(unsigned int j  = 0 ; j < every_level_info[ which_level ].complete_obstacle_information.size()  ; j++){
			
				 bool first_case = false , Second_case = false ; 

				 if( ThreeD_SG_edge[i].print_max_x() >= every_level_info[ which_level ].complete_obstacle_information[j].print_ld_corner_x() && 
				     ThreeD_SG_edge[i].print_min_x() <= every_level_info[ which_level ].complete_obstacle_information[j].print_ru_corner_x() ){
					first_case = true ;  
				 }
				 if( ThreeD_SG_edge[i].print_max_y() >= every_level_info[ which_level ].complete_obstacle_information[j].print_ld_corner_y() && 
				     ThreeD_SG_edge[i].print_min_y() <= every_level_info[ which_level ].complete_obstacle_information[j].print_ru_corner_y() ){
					Second_case = true ;  
				 }

				 //　確定有overlap，　就要　　記下obstalce id 及　Kruskal_Tree not ok 設成false  ,  這裡好像只有設定  是否有 obstacle 擋到而已
				 //  並不用設說  誰擋到 
				 if( first_case == true && Second_case == true ){
				 	// 喔 ， 這裡是要看  各個kruskal_edge中 ， 那些是要換換徑的 ， 而這裡是 只要看某一層的
					ThreeD_SG_edge[i].set_kruskal_edge_ok_not(false);
					// 由 edge 的z 坐標 我就可以之道 它是在那一層了  ,  感覺下面沒有要用的意思 先弄掉
					//ThreeD_SG_edge[i].add_which_obstacle_cross( every_level_info[ which_level ].
					//complete_obstacle_information[j].print_obstacle_id() );
					 //  這裡 主要是比說  ， 這個 edge 有沒有 必要去避開obstacle , 所以  只要確定它有
					 //  就不用再往下比了  , 所以我多了個 break ; 
					 break ; 
				 }
				//cout << "kruskal_edge_ok_or_not = "  <<  ThreeD_SG_edge[i].print_kruskal_edge_ok_not() << endl ;
			}// end of for　j
			 *-----------------------------------------------------------------------------*/

		}// 在同一層的edge 

	}// end of 抓出edge  是否要更動　，　以及overlap 的obstacle 有那些  ,  指這個kruskal edge 跟那些obstacle有碰到 ， 要改路徑的
	

	

	/*-----------------------------------------------------------------------------
	 * 				Check
	 * 		every Kruskal edge ok or not 
	 * 		and which obstacles it cross  .    
	 *        cout << endl << endl << "Check Kruskal_Tree edge " << endl ; 
		for(unsigned int i = 0 ; i < ThreeD_SG_edge.size() ; i++  ){
		
		cout << "This is edge number " << i+1 << endl ; 
		// 奇怪了　，　只要是設定false 那就一定有值會出來才對丫  
		ThreeD_SG_edge[i].print_kruskal_edge_information() ; 
	}

	 *-----------------------------------------------------------------------------*/
	 
	/*-----------------------------------------------------------------------------
	 *  				Step 3,4
	 *  		因為我已經抓出　有cross 的obstacle了　，　要抓其四個端點
	 *  		很簡單　，　接下來呢，　就建個新的vector 用來存
	 *  		obstacle_avoding_by_kruskal_Tree , 
	 *  		為true 的edge 就直接放入　，　false 的便修改後　，再放入
	 *
	  下面這二個  好像後面還會用到的樣子
	  沒有 只有  第一個 要記得放外面就好了
	  vector<edge> obstacle_avoding_Kruskal ; 	
	  我想呢 ， 我全部建成一張大的table 來查就好了 
	  哎  ， 下面的  ， vertex_seperate , 好像可以拿掉 ， 我在 projection_vertex 內  ， 
	  已經有分類了  ， 所以下面不用再做了   , 哎  不太一樣哎  ， 下面是把各層的  都放進來 
	  但我 projection_vertex 內的  ， 是 各層存自已的   , 哎 ， 分層去找 好像有比較好  
	 *-----------------------------------------------------------------------------*/
	vector<vertex> vertex_need ; 
	// 下面這個結構 等於是把  ， 各層的東西 ， 都放到這層來了 
	// vector<vertex> vertex_seperate[ every_level_info[0].plane_width ] ; 
	// vector<vertex> vertex_seperate[ every_level_info.size() ][ every_level_info[0].plane_width ] ; 
	// vector<vertex> vertex_seperate[ every_level_info.size() ][ every_level_info[0].plane_width ] ; 
	// hash_map<vertex ,int, hash_vertex , equal_vertex > hmap;

	int count = -1 ; 
	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		for( unsigned int j = 0 ; j < every_level_info[i].array_vertex_vector.size() ; j++ ){
			count++ ;
			// 因為我已經寫好了 vertex 的copy constructor 所以vertex 可以直接給了  	
			vertex_need.push_back( every_level_info[i].array_vertex_vector[j] );	
			// 下面三行 ， 應該就把  ， plane上  ， 有點的地方  設定好他的id了  
			//
			int x = every_level_info[i].array_vertex_vector[j].print_x_position() ; 
			int y = every_level_info[i].array_vertex_vector[j].print_y_position() ; 
			every_level_info[i].vertex_id[x][y] = count ; 
		}// 把每一層的vertex 都給抓出來  

	}// end of for
	
	/*-----------------------------------------------------------------------------
	for(unsigned int i = 0 ; i < vertex_need.size() ; i++ ){
		vertex_need[i].set_id( i ) ; 
		vertex_seperate[ vertex_need[i].print_x_position() ].push_back( vertex_need[i] ) ; 	 
	}
	 *-----------------------------------------------------------------------------*/

	 // 這裡把表的初值給建好了
	 // 應該把　path_node 換成　table_node 比較好　
	 vector<path_node> table[vertex_need.size()];			 

	/*-----------------------------------------------------------------------------
	 *  				下面這塊　是建好OAST 的核心
	 *  				要用的edge　我都抓好了　，　
	 *  				接下來便是要抓shortest path 
	 *  	先建出一張表來，　各個vertex 到別的點的weight , 要建幾乘幾的表，
	 *  	可由vertex_need 去看　，　再由edge_need 去一個個掃　，　來建立
	 *  	weight , 那　無法到達的 無限要怎麼代表它呢　
	 *  	, 我建了二個data structure , node_need 跟　edge_need 
	 *  	,node_need中的第０跟１　，　理應是　我要的source跟desitination 
	 *  	表的index 順序，　我想就跟node_need 一樣　，　
	 *  	而表的內容　，　就掃過edge_need , 來一個個建　，　一個edge_need
	 *  	要建　二個位置的值　像(3,1) (1,3)　這樣　
	 *-----------------------------------------------------------------------------*/



	 /*-----------------------------------------------------------------------------
	  接下來　　，　把值弄進去　，　每個edge 都跟vertex_need 掃過一遍
	  抓出edge  二端點的node index ,  下面這個loop 的 OASG 也要改
	  因為是2D 上的 要用的話 必需要是3D 的才行,  我要把各層的OASG edge 跟 vertical_edge 
	  我打算用 threeD_OAThreeD_SG_edge 存起各層的OASG edges ,  以及 vertical_edge , 因為 
	  3D  就是能夠在 ， 不同層下 找到別的路 連到同曾的另外一個 點 
	  *-----------------------------------------------------------------------------*/
	 vector<edge> threeD_OASG_edge ; 
	 //  先把各層的edge 丟進去 , 以及 垂直的 edge 丟入 , 
	 //  這樣我才好建起table , 
	 for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		 for( unsigned int j = 0 ; j < every_level_info[i].my_level_OASG.size() ; j++  ){
			threeD_OASG_edge.push_back( every_level_info[i].my_level_OASG[j] ) ; 	 
		 }

	 }// 總共有幾層

	// vertical_edge 是外面傳進來的 ， 因為 不可能存在各層中 
	for( unsigned int i = 0 ; i < vertical_edge.size()  ; i++ ){
		threeD_OASG_edge.push_back( vertical_edge[i] ) ; 
	}// 垂直的edge 

	// 這裡只是藉由 ， 3D_OASG 的edge 來填滿table內要用的資訊而已 
	// 好像  一個func內 只有一個地方能加那個  平行化的東西  
	for(unsigned int j = 0  ; j < threeD_OASG_edge.size() ; j++  ){

		// 內層的for  , 要抓出兩端點的index  , 下面能加 break 嗎  
		int index_one = -1 , index_two = -1  ; 


		int x = threeD_OASG_edge[j].print_node1_x() , y = threeD_OASG_edge[j].print_node1_y() , z = threeD_OASG_edge[j].print_node1_z() ; 
		index_one = every_level_info[z].vertex_id[x][y]  ; 
		/*-----------------------------------------------------------------------------
		 *   , 怎麼我用hash map 比我原本的還要慢呢  
		vertex temp( threeD_OASG_edge[j].print_node1_x() , threeD_OASG_edge[j].print_node1_y()  , threeD_OASG_edge[j].print_node1_z() ) ; 
		index_one = hmap[temp] ; 

		index_one = hmap[ vertex( threeD_OASG_edge[j].print_node1_x() , threeD_OASG_edge[j].print_node1_y()  , threeD_OASG_edge[j].print_node1_z() ) ] ; 
		還是  我不同層  用不同的 hash_map 

		int x = threeD_OASG_edge[j].print_node1_x() , z = threeD_OASG_edge[j].print_node1_z() ; 
		for(unsigned int k = 0 ; k < vertex_seperate[ z ][ x ].size() ; k++ ){

			if( threeD_OASG_edge[j].print_node1_y() == vertex_seperate[z][ x ][k].print_y_position() &&
			    threeD_OASG_edge[j].print_node1_z() == vertex_seperate[z][ x ][k].print_z_position() ){
				index_one =  vertex_seperate[z][ x ][k].get_id() ;  
				break ; 
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/



		int xx = threeD_OASG_edge[j].print_node2_x() , yy = threeD_OASG_edge[j].print_node2_y() , zz = threeD_OASG_edge[j].print_node2_z() ; 
		index_two = every_level_info[zz].vertex_id[xx][yy]  ; 
		/*-----------------------------------------------------------------------------
		int xx = threeD_OASG_edge[j].print_node2_x() , zz = threeD_OASG_edge[j].print_node2_z() ; 
		for(unsigned int k = 0 ; k < vertex_seperate[zz][ xx ].size() ; k++ ){

			if( threeD_OASG_edge[j].print_node2_y() == vertex_seperate[zz][ xx ][k].print_y_position() &&
			    threeD_OASG_edge[j].print_node2_z() == vertex_seperate[zz][ xx ][k].print_z_position() ){
				index_two = vertex_seperate[zz][ xx ][k].get_id() ;  
				break; 
			}

		}// end of for
		 *-----------------------------------------------------------------------------*/



		/*-----------------------------------------------------------------------------
		for(unsigned int k = 0  ;  k < vertex_need.size() ; k++  ){
			// 有的vertex 會對不到
			if(threeD_OASG_edge[j].print_node1_x() == vertex_need[k].print_x_position() &&
			   threeD_OASG_edge[j].print_node1_y() == vertex_need[k].print_y_position() &&
			   threeD_OASG_edge[j].print_node1_z() == vertex_need[k].print_z_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_one = k ;  
			}
			if(threeD_OASG_edge[j].print_node2_x() == vertex_need[k].print_x_position() &&
			   threeD_OASG_edge[j].print_node2_y() == vertex_need[k].print_y_position() &&
			   threeD_OASG_edge[j].print_node2_z() == vertex_need[k].print_z_position() ){
				// 我要的是vertex_need 內的index 位置　
				index_two = k ;  
			}
		}// 　抓好二個端點的index 了
		 *-----------------------------------------------------------------------------*/
		// 要把值給送進table 
		if( index_one != -1 && index_two != -1 ){
			table[index_one].push_back( path_node( index_one ,  threeD_OASG_edge[j].print_weight() , index_two ) ) ; 
			table[index_two].push_back( path_node( index_two ,  threeD_OASG_edge[j].print_weight() , index_one ) ) ; 
			/*-----------------------------------------------------------------------------
			table[index_one][index_two].set_infinite(false) ;
			// 這邊的 set_path_weight 我想也要改 因為原本是只有 2D 的判斷 
			table[index_one][index_two].set_path_weight( threeD_OASG_edge[j].print_3D_weight() );
			table[index_two][index_one].set_infinite(false) ;
			table[index_two][index_one].set_path_weight( threeD_OASG_edge[j].print_3D_weight() );	 
			 *  
			 *-----------------------------------------------------------------------------*/
		}
		
	}// end of 抓二端點的　index , 且埴滿table內的值　

	/*-----------------------------------------------------------------------------
	 * 				Check Code , May 9 
	cout << endl << "here become single source all desitination : " << endl ;  
	 *-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
	 *  
	 在這之後的東西 就沒有跑出來了 ， 所以 我想 理應是某個func 被我改了 
	 但 effect 到這裡  
	 下面這塊是核心了    , 不之道有沒有東西要改的 , 這塊好像 沒用到 我先前記的
	 有 cross的 obstacle 丫 喔 ，  好像沒必要用到  , 忘記下面的order  是幹麻的了
	 總之是要依順序去 執行thread 但是  ， 我拿掉它  好像沒有差   , 本來想說會不會快一點的說  
	int SG_edge_change = ThreeD_SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	 *-----------------------------------------------------------------------------*/
	int SG_edge_change = ThreeD_SG_edge.size() ;  	
	#pragma omp parallel for ordered 
	for(int i = 0  ; i < SG_edge_change ; i++  ){

		// cout << ThreeD_SG_edge[i].print_kruskal_edge_ok_not() << endl ; 
		// true 代表 ， 它不用找替換的路徑
		if( ThreeD_SG_edge[i].print_kruskal_edge_ok_not() == true  ){
			obstacle_avoding_Kruskal.push_back( ThreeD_SG_edge[i] ) ; 
		}
		// 只要是   , 進下面這區塊的 便是要找路替換的 
		else {
			
			/*-----------------------------------------------------------------------------
			 *  我要把edge_need 放到上面去做 ， 這裡變成 ， 抓edge兩端點的index出來即可
			for(unsigned  int j = 0 ; j < vertex_need.size() ; j++ ){
				// 有可能是這裡沒改到 ， 少加了 z 的參數  ， 可能會拿到別層的東西
				if( ThreeD_SG_edge[i].print_node1_x() == vertex_need[j].print_x_position() &&
				    ThreeD_SG_edge[i].print_node1_y() == vertex_need[j].print_y_position() &&
				    ThreeD_SG_edge[i].print_node1_z() == vertex_need[j].print_z_position() ){
					head_index = j ;  
				}
				else if( ThreeD_SG_edge[i].print_node2_x() == vertex_need[j].print_x_position() &&
				         ThreeD_SG_edge[i].print_node2_y() == vertex_need[j].print_y_position() &&
					 ThreeD_SG_edge[i].print_node2_z() == vertex_need[j].print_z_position() ){
					tail_index = j ;  
				}
			}// end of for
			 *-----------------------------------------------------------------------------*/


			int head_index = -1 , tail_index = -1 ; 	

			int  x = ThreeD_SG_edge[i].print_node1_x() , y = ThreeD_SG_edge[i].print_node1_y() ,  z =  ThreeD_SG_edge[i].print_node1_z() ; 
			head_index =  every_level_info[z].vertex_id[x][y] ;  

			int xx = ThreeD_SG_edge[i].print_node2_x() , yy = ThreeD_SG_edge[i].print_node2_y() ,  zz = ThreeD_SG_edge[i].print_node2_z() ; 
			tail_index =  every_level_info[zz].vertex_id[xx][yy] ;  
			/*-----------------------------------------------------------------------------
			int head_index = -1 , tail_index = -1 ; 	
			int  x = ThreeD_SG_edge[i].print_node1_x() ,  z =  ThreeD_SG_edge[i].print_node1_z() ; 
			for(unsigned int j = 0 ; j < vertex_seperate[z][ x ].size() ; j++ ){

				if( ThreeD_SG_edge[i].print_node1_y() == vertex_seperate[z][ x ][j].print_y_position() &&
				    ThreeD_SG_edge[i].print_node1_z() == vertex_seperate[z][ x ][j].print_z_position() ){
					head_index =  vertex_seperate[z][ x ][j].get_id() ;  
					break ; 
				}

			}// end of for

			int xx = ThreeD_SG_edge[i].print_node2_x() , zz = ThreeD_SG_edge[i].print_node2_z() ; 
			for(unsigned int j = 0 ; j < vertex_seperate[ zz ][ xx ].size() ; j++ ){

				if( ThreeD_SG_edge[i].print_node2_y() == vertex_seperate[ zz ][ xx ][j].print_y_position() &&
				    ThreeD_SG_edge[i].print_node2_z() == vertex_seperate[ zz ][ xx ][j].print_z_position() ){
					tail_index = vertex_seperate[ zz ][ xx ][j].get_id() ;  
					break ; 
				}

			}// end of for
			 *-----------------------------------------------------------------------------*/


			//cout << "my_level_info.total = " << my_level_info.total << ",  vertex_need.size " << vertex_need.size() << endl ;   

			// 這裡要寫些code來看資訊 ， 用來抓bug 的 ， 要印出 ， 此時的i 是多少
			// 以及這個要被改的edge  資訊 ， 
			//cout << " i = " << i << endl ; 
			//ThreeD_SG_edge[i].print_kruskal_edge_information() ; 
			
			/*-----------------------------------------------------------------------------
			 *  
			// 這裡 印出個花費相鄰矩陣來看看 好了  
			cout << "Here print out weight table : " << i << endl ; 
			for(unsigned int j = 0 ; j < vertex_need.size() ; j++ ){
			 	for(unsigned int k = 0 ; k < vertex_need.size() ; k++ ){

					if( j == k ){
						table[j][j].set_infinite(false) ;
						table[j][j].set_path_weight( 0 );	 
					}

					if(table[j][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[j][k].print_path_weight() << " " ;	
					}

				}// inner loop j 
				cout << endl << endl ;; 
			}// end of 印出矩陣來看看
			 *-----------------------------------------------------------------------------*/
	
			/*-----------------------------------------------------------------------------
			 *  表建好後　，　接下來便是執行　one source all destination 的algorithm 
			先抓source 行中　weight  最小的　，　及　此edge node2是誰
			把weight加到　node2那行　再跟 source 行比對，　　如果值有變小　，　或
			原本true 的　變false ，　也可把值納入　
			抓出shortest path 中間要的轉接點後　，　可以用　之前的方法　，　從edge_need 中
			抓出　其edge_need 含這些點集的edge 即可
			還是說　用個　bool 紀路這個點　是否當過轉接點,  轉接點 只會有一個 ， 初值我設成 －1 
			只要是 source  能直接到達的 ， 都設成 －1 ， 
			原先的表是拿來查的 ， 不能直接休改 ， 
			 *-----------------------------------------------------------------------------*/
			 
			 /*-----------------------------------------------------------------------------
			  *  
			  因為table的直是用來查的 不是用來更新的 ， 不然  會出點錯誤 ，
			  所以這裡我要建個一行path_node 來 ， 存原始行的data , 
			  等路抓出來以後 ， 再用這個還原回去 
			  *-----------------------------------------------------------------------------*/
			 // 這是 要被更新的那一行
			 vector<path_node> for_update;			 
			// 我只要把上面的for_update變成 for_update[2] , 就可以一個從頭處理， 
			// 一個從尾巴了 
			for(unsigned int j = 0  ; j <  table[head_index].size() ; j++ ){
				for_update.push_back( table[head_index][j] ); 
			}


			/*-----------------------------------------------------------------------------
			 *  
			  如果抓不到一個可以轉接的點　，　便跳出去了　
			  這個 do while 把整個表給更新好了　，　接下來便是看我怎麼抓中間的edge了
			 *-----------------------------------------------------------------------------*/
			 bool ok = true ; 
			 int tail_index_in_for_update = -1 ; 
			 // int safe_count = 0 ; 
			 do{
			 	
			 	vector<int> non_infinite_used  ; 
				for(unsigned int j = 0  ; j < for_update.size() ; j++  ){

					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					// 這邊有個新的問題 ， 因為我剛初是把全部的東西都放進來
					// 所以  會有 無限的這個情況  那我有改過了  ， 不會有這種
					// 情況了 ， 要不要拿掉呢 
					if( for_update[j].print_used() == false  ){
						non_infinite_used.push_back( j ) ; 
					}// end of 沒當過轉接的點 

					// 基本上 ， 只要 這個目地點給放入 ， 它在for_update 中的 index 都不會變了 
					if( for_update[j].print_desitination_vertex_index() == tail_index ){
						// safe_count++ ;  
						tail_index_in_for_update = j ; 
					}

					/*-----------------------------------------------------------------------------
					 *  
					 *  		origin code 
					// 有值　且未當過　轉接點, 放進一個vector 紀路index  　
					// infinite為 false 代表有值 ， 這二點間可以通
					if( for_update[j].print_infinite() == false && for_update[j].print_used() == false  ){
						non_infinite_used.push_back(j); 
					}
					 *-----------------------------------------------------------------------------*/
				} 


				 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
				if( non_infinite_used.size() > 0 ){

					for(unsigned int j = 0 ; j < non_infinite_used.size() ; j++ ){

						 // 還是說 ， 不是由最小的值去更新 ， 而是從 ， 能往外走的點，去更新
						int update_row_index = 0 ; 
						int min_weight = 0 ; 
						
						min_weight = for_update[ non_infinite_used[j] ].print_path_weight() ;  
						update_row_index = for_update[ non_infinite_used[j] ].print_desitination_vertex_index()  ;	

						/*-----------------------------------------------------------------------------
						 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						 這裡呢 我想平行化  ， 要把下面的 ， for  index 弄成 signed  

						long int size_of_row_index = (long int)table[update_row_index].size()  ;  
						#pragma omp parallel for ordered 
						for(long int j = 0  ; j < size_of_row_index  ; j++  )

						case_one 是指 有找到 相同 目的點的vertex , 可能可以 簡化
						case_two 則是 都沒有找到  所以 push_back 

						用case_match 來代替好了 ， false 相當於上面的 case_two 
									   true  則是 case_one
						 *-----------------------------------------------------------------------------*/
						for(unsigned int k = 0  ; k < table[update_row_index].size()  ; k++  ){
							
							 bool case_match = false ; 
							 int  match_index_in_for_update = -1 ; 
							/*-----------------------------------------------------------------------------
							 *  這裡面 ， 應該還要一個for , 用來看 ， 這個轉接點 ， 能否 簡短
							 *  main_vertex 到其它點的距離(能簡短的 ， 當然是 有相同 目對點的) ，
							 *  如都遇不到相同目的點的 ， 代表 ， 這是個新的路 ， 可以放進來 
							 *  也就是說  有二種case 
							 *-----------------------------------------------------------------------------*/
							 for(unsigned int l = 0 ; l < for_update.size() ; l++ ){
							 	if( table[update_row_index][k].print_desitination_vertex_index() ==
								    for_update[l].print_desitination_vertex_index()              ){
									case_match = true ;  
									match_index_in_for_update = l ; 
									break ;
								}// 抓出  有相同 目地點的vertex位置 
							 }// end of for_update  

							if( case_match == true  ){
								// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
								// 值的比較不能含等號
								if( table[update_row_index][k].print_path_weight() + min_weight < 
								    for_update[ match_index_in_for_update ].print_path_weight()          	 ){
								    // 把轉接點放入　　，　更新weight ,
									 for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 

									 for_update[match_index_in_for_update].set_path_weight( 
									 	table[update_row_index][k].print_path_weight() + min_weight 
									 ) ; 

									 for_update[match_index_in_for_update].set_used( false ) ; 	
								}

								/*-----------------------------------------------------------------------------
								 *  這個case 在我的新方法中 ， 是不會出現的 
								// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
								else if( table[update_row_index][k].print_infinite() == false 			      && 
									 for_update[match_index_in_for_update].print_infinite() == true 	      )
								{
								    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
								    for_update[match_index_in_for_update].set_infinite( false ); 
								    for_update[match_index_in_for_update].set_trans_node(update_row_index) ; 
								    for_update[match_index_in_for_update].set_path_weight( 
								    	table[update_row_index][k].print_path_weight() + min_weight 
									) ; 
								}
								 *  
								 *-----------------------------------------------------------------------------*/
							 
							}// end of case_match == true
							else if( case_match == false ){
								// 除了 ， 把這個 path_node 塞進去外 ， 還要多設定 ， 
								// 它是由 誰 轉接過來的  , 因為原本是到不了它的 
								for_update.push_back( table[update_row_index][k] ) ; 
								for_update[ for_update.size() - 1 ].set_trans_node( update_row_index )  ; 
							    	for_update[ for_update.size() - 1 ].set_path_weight( 
									table[update_row_index][k].print_path_weight() + min_weight 
								) ; 
							 
							}// end of case_match == false

						}// end of loop k

						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[ non_infinite_used[j] ].set_used(true);

					 
					}// end of scan all non_infinite_used.size() 

					/*-----------------------------------------------------------------------------
					 *  			Origin Code 
					int update_row_index = 0 ; 
					int min_weight = 0 ; 
					unsigned int k = 0 ; 
					
					do{
						min_weight = for_update[ non_infinite_used[k] ].print_path_weight() ;  
						update_row_index = non_infinite_used[k] ;	

						// 抓到weight 最小的index , 抓看看　有否可　轉接，　降低weight的地方
						// 第一輪的可以non infinite used ， 但後面的 要用 ， 有被更新到的， 那個點的node_index,
						// 像node_c 突然有路可以接進來了 ， 那下一輪要用來看的行，便是 node_c 
						for(unsigned int j = 0  ; j < table[update_row_index].size()  ; j++  ){

							// update_row_index 那行　，　要有值的地方　，　才有加　以及比較的必要 
							// 值的比較不能含等號
							if( table[update_row_index][j].print_infinite() == false && for_update[j].print_infinite() == false &&  
							    table[update_row_index][j].print_path_weight() + min_weight < for_update[j].print_path_weight() ){
							    // 把轉接點放入　　，　更新weight ,
								 for_update[j].set_trans_node(update_row_index) ; 
								 for_update[j].set_path_weight( table[update_row_index][j].print_path_weight() + min_weight ) ; 
								 for_update[j].set_used( false ) ; 	
							}
							// 下面這個else 的case 是 ， 更新行的值是無限 ， 而 轉接的不是 
							else if( table[update_row_index][j].print_infinite() == false && for_update[j].print_infinite() == true ){
							    // 把轉接點放入　　，　更新weight , 更新infinite = false , 因為有值進去了　
							    for_update[j].set_infinite( false ); 
							    for_update[j].set_trans_node(update_row_index) ; 
							    for_update[j].set_path_weight( table[update_row_index][j].print_path_weight() + min_weight ) ; 
							}
							


						}// end of loop j
						//有值被更新的 , 要給成false , 讓它再 轉接看看
						// 用過就要給 true 
						for_update[update_row_index].set_used(true);
						k++ ;
					}while( k < non_infinite_used.size() );
					 *-----------------------------------------------------------------------------*/

					if( tail_index_in_for_update != - 1    ){
						ok = false ;  
					}

				}// non_infinite_used > 0 才進來 

				
				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				cout << "here is debug info : " << endl ; 
				for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
					if(table[0][k].print_infinite() == true ){
						cout << " xx " ;   
					}
					else{
						cout << " " << table[0][k].print_path_weight() << " " ;	
					}	
				}
				cout << endl ; 

				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 


				我用 i = 18  ， 去跑 ， 它確切有在算 那張表 ， 但為什麼 i = 17 , 33  時確沒有
				初值設定 ， 是有跑 ， 但表的更新那塊 ， 怎麼沒有跑
				 *-----------------------------------------------------------------------------*/


				/*-----------------------------------------------------------------------------
				 *  
				// 輸出trans_node 來看看 是否 會有一個值為 －1 的, why 值都是 0
				cout << "here is trans_node info : " << endl ; 
				for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
					cout << " " << table[0][j].print_trans_node() << " " ; 
				}
				cout << endl ; 
				 *-----------------------------------------------------------------------------*/


				// 沒有點可以拿來　當轉接　，　輸入的點跟obstacle數目　是３０，３０　４０，４０　時
				// 會有 中間沒路的情況　
				// 要連線的[0][0] , [0][1] , 分別是頭跟尾丫　, 第一個到達的點 ， 一定是最短的嗎 
				// if( tail_index_in_for_update != -1 && for_update[tail_index_in_for_update].print_infinite() == true  ){
					/*-----------------------------------------------------------------------------
					 * 				Check Code , May 9  
					cout << endl ; 
					cout << " No path can reach " << endl ;  
					// 用來看是第幾個edge出這樣的錯 
					cout << "variable i is : " << i << endl ; 
					cout << "how many vertex in vertex_need : " << vertex_need.size() << endl ; 
					//cout << "size in edge_need : " << edge_need.size() << endl ; 
					cout << "size in obstacle_crossed :  " << SG_edge[i].which_obstacle_cross_size() << endl ; 
					SG_edge[i].print_kruskal_edge_information() ; 
					 *-----------------------------------------------------------------------------*/

					/*-----------------------------------------------------------------------------
					 *  奇怪了 ， 為什麼 ， 轉接點全是－1 ， 它跟本沒有在做 東西的轉接丫 ， 難怪會找不到
					 *  路 
					cout << "here is debug info : " << endl ; 
					for(unsigned int k = 0 ; k < table[0].size() ; k++ ){
						if(table[0][k].print_infinite() == true ){
							cout << " xx " ;   
						}
						else{
							cout << " " << table[0][k].print_path_weight() << " " ;	
						}	
					}
					cout << endl ; 

					cout << "here is trans_node info : " << endl ; 
					for(unsigned int j = 0 ; j < table[0].size() ; j++ ){
						cout << " " << table[0][j].print_trans_node() << " " ; 
					}
					cout << endl ; 
					 *-----------------------------------------------------------------------------*/
					/*-----------------------------------------------------------------------------
					 *  
						cout << " edge_need 's information :" ;
						for(unsigned int j = 0  ; j < edge_need.size() ; j++  ){
							edge_need[j].print_kruskal_edge_information() ;
						}
					 *-----------------------------------------------------------------------------*/

				//}// end of for_update[tail_index].print_infinite() == true



			 // ok 的意思是說　，　找不到點可以　再拿來做轉接　，　所以　source 能到的點　，　應該都找完了
			 }while( ok == true );
 
			
			 /*-----------------------------------------------------------------------------
			  *  
			 source line 的內容　更新好了　，　接下來要抓出　，　中間轉接了那些點
			 再　把兩兩相鄰的vertex ，　其直線給抓出來　放入　Obstacle_Avoding_Kruskal_Tree 
			 source 的index 是　０　，　destination 的index 是　１
			  所以我判斷　有沒有路過去　，　便是看　index 1 的那個位置　0 能否過的去　
			  *-----------------------------------------------------------------------------*/

		
			 /*-----------------------------------------------------------------------------
			  *                       	origin Code   
			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index ; 
			 do{
			 	// 要確定裡面真是有東西　才行
				if( for_update[start_index].print_infinite() == false  && for_update[start_index].print_trans_node() != -1  ){
					trans_order.push_back( for_update[start_index].print_trans_node() );	
					start_index = for_update[start_index].print_trans_node() ; 
				} 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 
			  *-----------------------------------------------------------------------------*/
			 vector<int> trans_order ; 
			 trans_order.push_back( tail_index ) ; 
			 // 我整個程式有二個ok  ,  一個是表的值 有否更新好的ok  , 一個是路徑的ok 
			 bool ok2 = false ; 

			 int start_index = tail_index_in_for_update ; 
			 // cout << "Find the chain of vertex " << endl ; 
			 do{
			 	// cout << "Do i get in 593 " << endl ; 
				// 等於 －1 代表 ， 這個點跟source node 是直接有連線的  
				// 丫 ， 我之道了 ， 有可能是 start_index 這裡面沒有東西
				// 所以就當了  , 我 tail_index_in_for_update 一定能抓到東西嗎
				if( for_update[start_index].print_trans_node() == - 1    ){
					// 等於true  代表完成了　
					ok2 = true ;  
				}
			 	// 看來 這裡面 ， 要用個loop  , 來抓出 desitination_vertex_id , 是我們要的 那個path_node 
				// 再一個個 trace 回去 
			 	// 要確定裡面真是有東西　才行
				else {
					trans_order.push_back( for_update[start_index].print_trans_node() );	

					for(unsigned int j = 0 ; j < for_update.size() ; j++ ){
						if( for_update[j].print_desitination_vertex_index() == for_update[start_index].print_trans_node() ){
							start_index = j  ; 
							break ; 
						}
					}// end of loop j 
				} 

			 }while( ok2 == false );

			 trans_order.push_back( head_index ) ; 

			 /*-----------------------------------------------------------------------------
			  *  
			 cout << " how many size in edge_need : " << edge_need.size() << endl ;  
			 
			 cout << "here is trans_order info : " <<  endl  ; 
			 for(unsigned int i = 0 ; i < trans_order.size() ; i++  ){
			  	cout << " " << trans_order[i] << " " ; 
			 }
			cout << endl << endl ;  
			  *-----------------------------------------------------------------------------*/


			/*-----------------------------------------------------------------------------
				 好了， trans_order 內的東西， 我已經抓其了 ， 現在要把 edge 放入
				 Obstacle_Avoding_Kruskal_Tree 內 , 我想把兩兩的index 對的vertex 抓出來放入即可 
			 *  看來是要在這裡加了 ， 把要加入的edge 跟 obstacle_avoding_Kruskal 內的edge 
			 *  做比對， 只要裡面已經有這個edge 便不加入
			 *  下面的for 的j 上限是 －1 ， 因為 內容會用到 j + 1  , 下面三行  用來看  抓 cp edge 
			 *  會不會花太多時間  ， 其實還好哎  
			clock_t start , end ; 
			double  dif  ; 
			start = clock() ; 
			我下面原先的做法  ， 是怕  ， 相同的edge 被放進去  ， 所以要一個個去比  ， 
			但  我用already_in_OAST , 這個結構  ， 就可以了  ，  用此結構 ， 看那些點 ， 已放入
			OAST 裡面  ， 要放入的edge , 兩端點 都在  OAST 內 就不用放了 ， 其它情況就要放 
			for(unsigned int j = 0 ; j < trans_order.size() - 1 ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_z() == vertex_need[ trans_order[j] ].print_z_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_z() == vertex_need[ trans_order[j + 1 ] ].print_z_position() 
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					         obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					         obstacle_avoding_Kruskal[k].print_node1_z() == vertex_need[ trans_order[j + 1 ] ].print_z_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_z() == vertex_need[ trans_order[j] ].print_z_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j] ].print_z_position() ,  
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_z_position() )   ); 
				 }
			}// end of 加入已避開obstacle的edge 

			trans_order  是由 頭開始 掃的 ， 似乎  要再看的細 一點   ， 二頭 都沒給加入 ， 代表這個edge 是可以加入的 
			如果 遇到 一個在裡面 ， 一個沒有  ， 而目的點 已在裡面 ， 那就可以停了  
			 *-----------------------------------------------------------------------------*/
			for(unsigned int j = 0 ; j < trans_order.size() - 1 ; j++ ){
				
				 bool insert_ok_not = true ;  
				 for( unsigned int k = 0 ; k < obstacle_avoding_Kruskal.size() ; k++ ){
				 	if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node1_z() == vertex_need[ trans_order[j] ].print_z_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j + 1 ] ].print_x_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					    obstacle_avoding_Kruskal[k].print_node2_z() == vertex_need[ trans_order[j + 1 ] ].print_z_position() 
					    ){
						insert_ok_not = false ;  
					}
					else if( obstacle_avoding_Kruskal[k].print_node1_x() == vertex_need[ trans_order[j + 1] ].print_x_position() &&
					         obstacle_avoding_Kruskal[k].print_node1_y() == vertex_need[ trans_order[j + 1 ] ].print_y_position() &&
					         obstacle_avoding_Kruskal[k].print_node1_z() == vertex_need[ trans_order[j + 1 ] ].print_z_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_x() == vertex_need[ trans_order[j] ].print_x_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_y() == vertex_need[ trans_order[j] ].print_y_position() &&
					         obstacle_avoding_Kruskal[k].print_node2_z() == vertex_need[ trans_order[j] ].print_z_position()
					    ){
						insert_ok_not = false ;  
					}
				 }// end of for check can i insert this edge 

				 if( insert_ok_not == true  ){
						  
					obstacle_avoding_Kruskal.push_back( edge (vertex_need[ trans_order[j] ].print_x_position() ,  
										  vertex_need[ trans_order[j] ].print_y_position() ,
										  vertex_need[ trans_order[j] ].print_z_position() ,  
										  vertex_need[ trans_order[j + 1] ].print_x_position() ,
										  vertex_need[ trans_order[j + 1] ].print_y_position() ,
										  vertex_need[ trans_order[j + 1] ].print_z_position() )   ); 
				 }
			}// end of 加入已避開obstacle的edge 
			// end = clock() ; 
			// dif = (double) ( end - start ) ; 
			// cout << "check edge cp  = "  << (double) ( dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ; 
			
			

		}// end of else , 這個區塊　我一次只改一個edge  , end of if
		       
	}// scan 過Kruskal_Tree 中所有的edge ，　並對false的做修改, a for loop  use variable i , so else_block cant use variable i 



	vector<edge> seperate_group[ obstacle_avoding_Kruskal.size() ] ; 
	int count_2 = 0 ; 
	// int index_delete = -1 ; 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){

		bool delete_or_not = false  ;
		// cout << "i = "	<< i << endl ; 
		
		// cout << " Do i get in "  << endl ;  , 我這樣 ， 沒有把後面的給刪掉 
		
		 // bool connect_graph_complete = false ;  
		int node1_x = 	obstacle_avoding_Kruskal[ i ].print_node1_x() ; 
		int node1_y = 	obstacle_avoding_Kruskal[ i ].print_node1_y() ; 
		int node1_z = 	obstacle_avoding_Kruskal[ i ].print_node1_z() ; 

		int node2_x = 	obstacle_avoding_Kruskal[ i ].print_node2_x() ; 
		int node2_y = 	obstacle_avoding_Kruskal[ i ].print_node2_y() ; 
		int node2_z = 	obstacle_avoding_Kruskal[ i ].print_node2_z() ; 

		// 先判斷那二個點 ， 有沒有其它的subtree   , 沒有的話  ， 就依 count 值 放到 
		// 指定的箱子裡面 
		if( every_level_info[node1_z].connected_graph[node1_x][node1_y] == -1 &&  every_level_info[node2_z].connected_graph[node2_x][node2_y] == -1 ){
			// 給值 ， 再放到  ， vector 中   
			every_level_info[node1_z].connected_graph[node1_x][node1_y] = count_2 ; 
			every_level_info[node2_z].connected_graph[node2_x][node2_y] = count_2 ; 
			// 之後是要利用 ， 下面二個點 ， 去改上面的id 
			seperate_group[count].push_back( obstacle_avoding_Kruskal[i] ) ; 
			count_2++ ; 
			//cout << "line   "<< 1771  << endl ; 
		}
		else if( every_level_info[node1_z].connected_graph[node1_x][node1_y] >= 0 &&  every_level_info[node2_z].connected_graph[node2_x][node2_y] == -1 ){
			// 給值 ， 再放到  ， vector 中   
			every_level_info[node2_z].connected_graph[node2_x][node2_y] = every_level_info[node1_z].connected_graph[node1_x][node1_y] ; 
			// 下面這  好像不是放在 count  所在的箱子 ， 而是放在  >= 0 的那個箱子才對 
			seperate_group[ every_level_info[node1_z].connected_graph[node1_x][node1_y] ].push_back( obstacle_avoding_Kruskal[i] ) ; 
			// cout << "line   "<< 1778  << endl ; 
		}
		else if( every_level_info[node1_z].connected_graph[node1_x][node1_y] == -1 &&  every_level_info[node2_z].connected_graph[node2_x][node2_y] >= 0 ){
			// 給值 ， 再放到  ， vector 中   
			every_level_info[node1_z].connected_graph[node1_x][node1_y] = every_level_info[node2_z].connected_graph[node2_x][node2_y]; 
			seperate_group[  every_level_info[node2_z].connected_graph[node2_x][node2_y] ].push_back(  obstacle_avoding_Kruskal[i] ) ; 
			// cout << "line   "<< 1784 << endl ; 
		}
		else if( every_level_info[node1_z].connected_graph[node1_x][node1_y] >= 0 &&  every_level_info[node2_z].connected_graph[node2_x][node2_y] >= 0 && 
			 every_level_info[node1_z].connected_graph[node1_x][node1_y]  == every_level_info[node2_z].connected_graph[node2_x][node2_y]           ){
		 
			// cout << "line   "<< 1844  << endl ; 
		 	// 這裡  要刪掉  那個 弄出cycle的edge 
			delete_or_not = true ; 
			obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
			i-- ; 
		}
		else if( every_level_info[node1_z].connected_graph[node1_x][node1_y] >= 0 &&  every_level_info[node2_z].connected_graph[node2_x][node2_y] >= 0 && 
			 every_level_info[node1_z].connected_graph[node1_x][node1_y]  != every_level_info[node2_z].connected_graph[node2_x][node2_y] 	   ){

			 // 為什麼進來這個  會出錯呢  
			// cout << "line   "<< 1788 << endl ; 
			
			// 下面二個loop 是依大小 ， 來看 ， 是誰要合併誰  
			// 由下面一行可以之道 ， node2 的 被蓋掉  ， 也就是說  ， node1所在的 subtree  , 量比較大  
			int which_group = every_level_info[node2_z].connected_graph[node2_x][node2_y] ; 
			int des_group   = every_level_info[node1_z].connected_graph[node1_x][node1_y] ; 

			every_level_info[node2_z].connected_graph[node2_x][node2_y] = every_level_info[node1_z].connected_graph[node1_x][node1_y] ; 
			seperate_group[  des_group ].push_back(  obstacle_avoding_Kruskal[i] ) ; 

			// cout <<  "seperate_group[ which_group ].size() = " << seperate_group[ which_group ].size() << endl ; 
			for(unsigned int k = 0 ; k < seperate_group[ which_group ].size() ; k++ ){
				// cout << " j  =" << j << endl ; 
				// 班過去 node1 那個箱子 ， 順便改  ， 在 connected_graph  上的group id 	 
				int tmp_x = -1 , tmp_y = -1 , tmp_z = -1 ; 
				tmp_x = seperate_group[ which_group ][k].print_node1_x() ; 
				tmp_y = seperate_group[ which_group ][k].print_node1_y() ; 
				tmp_z = seperate_group[ which_group ][k].print_node1_z() ; 
				every_level_info[tmp_z].connected_graph[tmp_x][tmp_y] =  every_level_info[node1_z].connected_graph[node1_x][node1_y] ; 

				tmp_x = seperate_group[ which_group ][k].print_node2_x() ; 
				tmp_y = seperate_group[ which_group ][k].print_node2_y() ; 
				tmp_z = seperate_group[ which_group ][k].print_node2_z() ; 
				every_level_info[tmp_z].connected_graph[tmp_x][tmp_y] =  every_level_info[node1_z].connected_graph[node1_x][node1_y] ; 
				// 要把東西都給班過去   , 為什麼下面這行  會讓我的code 當了呢   , 告夭 ， 我之道為什麼了拉  
				// 我把  node1箱子的index 給了  node2  , 又 從它那取箱子的index , 等於我都在處理同個箱子 按  
				seperate_group[ des_group ].push_back( seperate_group[which_group][k] ) ; 
			}// end of loop j 

			// 班完後 ， 把另一邊的東西給刪了 
			seperate_group[  which_group  ].clear()  ;

		}// 都在不同的subtree上 

		/*-----------------------------------------------------------------------------
		// 要怎麼判斷 ， 所有的pin點 都已經連通了呢  , 有二個樹 要合併的時侯 ， 再去比  ， 
		// 合併時 ， 二個tree 的subtree id 都會 統一了  ， 此時 ， 只要用only_pin 去 connect_graph
		// 中 ， 看  是否每個pin的subtree id 都一樣  ， 就可以了  如果 有一個不一樣  ， 就跳出來 
		for( unsigned int j = 0 ; j < only_pin.size() - 1 ; j++ ){
			 
			int  x =  only_pin[j].print_x_position() ;  
			int  y =  only_pin[j].print_y_position() ;  

			int  xx =  only_pin[ j + 1].print_x_position() ;  
			int  yy =  only_pin[ j + 1].print_y_position() ;  

			if(  every_level_info.connected_graph[x][y] >= 0 && every_level_info.connected_graph[xx][yy] >= 0 ){
			 
				if( every_level_info.connected_graph[x][y] != every_level_info.connected_graph[xx][yy] ){
					connect_graph_complete = false ;  
					break ; 
				}
				else if( every_level_info.connected_graph[x][y] == every_level_info.connected_graph[xx][yy] ){
					connect_graph_complete = true ;  
				}

			}


		}// scan all pins 

		if( connect_graph_complete == true ){
			break ;  
		}
		 *-----------------------------------------------------------------------------*/
		 if( delete_or_not == false  ){
			every_level_info[node1_z].degree_per_vertex[ node1_x ][ node1_y]++ ; 
			every_level_info[node2_z].degree_per_vertex[ node2_x ][ node2_y]++ ; 
		 }

	}// scan all edges in obstacle_avoding_Kruskal 
	
	bool all_ok = false ; 
	while( all_ok == false ){
		all_ok = true ; 	
		// 拿掉的edge , degree 記得要--
		for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size() ; i++ ){

			// cout << "i = "	<< i << endl ; 
			
			// cout << " Do i get in "  << endl ;  , 我這樣 ， 沒有把後面的給刪掉 
			
			 // bool connect_graph_complete = false ;  
			int node1_x = 	obstacle_avoding_Kruskal[ i ].print_node1_x() ; 
			int node1_y = 	obstacle_avoding_Kruskal[ i ].print_node1_y() ; 
			int node1_z = 	obstacle_avoding_Kruskal[ i ].print_node1_z() ; 

			int node2_x = 	obstacle_avoding_Kruskal[ i ].print_node2_x() ; 
			int node2_y = 	obstacle_avoding_Kruskal[ i ].print_node2_y() ; 
			int node2_z = 	obstacle_avoding_Kruskal[ i ].print_node2_z() ; 

			// 在 pin_plane 及 obstacle_plane 中  ， 都同為true的  就是 
			if( every_level_info[node1_z].degree_per_vertex[ node1_x ][ node1_y] == 1      && 
			    every_level_info[node1_z].pin_plane[ node1_x ][ node1_y] == false          ){
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
				i-- ; 
				all_ok = false ; 
				every_level_info[node1_z].degree_per_vertex[ node1_x ][ node1_y]-- ; 
				every_level_info[node2_z].degree_per_vertex[ node2_x ][ node2_y]-- ; 
			}
			else if( every_level_info[node2_z].degree_per_vertex[ node2_x ][ node2_y] == 1      && 
				 every_level_info[node2_z].pin_plane[ node2_x ][ node2_y] == false     	){
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i  ) ; 
				i-- ; 
				all_ok = false ; 
				every_level_info[node1_z].degree_per_vertex[ node1_x ][ node1_y]-- ; 
				every_level_info[node2_z].degree_per_vertex[ node2_x ][ node2_y]-- ; 
			}

		}// end of scan all obstacle_avoding_Kruskal 

	}// end of while 


	sort( obstacle_avoding_Kruskal.begin() , obstacle_avoding_Kruskal.end() , sort_overlap_edge_weight );	 

	/*-----------------------------------------------------------------------------
	 *  首先呢 ， 傳進來的 obstacle_avoding_Kruskal 是要 處理成 rectilinear 的 ， 而  我已經對 ， weight 做排序了 
	 *  要二個變數 ， 之道index 指向那二個 ， 以及  二個bool 去看說  ， 現在這二個的情況是 ， 那種case
	 *  也用一個ok變數 ， 直到 obstacle_avoding_Kruskal 內的東西都沒了  都跑出while 
	 *-----------------------------------------------------------------------------*/
	 unsigned int  index_b = 0 ; 




	bool  find_index_b = false ; 
	// 下面這個 loop i 用來 指向 第一個 edge 
	for(unsigned int i = 0 ; i < obstacle_avoding_Kruskal.size()  ; i++ ){

		int temp_overlap_value = 0 ; 
		int max_overlap_value = 0 ; 

		// 有可能發生  index_a 沒指到人丫 , 
		for(unsigned int j = i + 1 ; j < obstacle_avoding_Kruskal.size() ; j++  ){
		 
			// 直接 , 指向  跟 index_a 比  其 overlap 量最多的那個比  , 相交是二個box , 
			// 可以分成 x 軸為主的相交  以及  y 軸為主的相交  ， 相交以短的為相交的量 ， 
			// paper 上分的case 2 , 3 分別是  只有 一個軸的相交 以及  二個軸都有相交 
			// 哇 ， 這裡  ， 沒有區分  ， edge 是否同一層 
			if( two_edge_at_same_floor( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) == true  && 
			    two_edges_overlap_or_not_by_new_method( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] ) == true  ){
				// 這裡面先把  ， 這二個 edge 的overlap 值給抓出來		 , 一定要找到底 ， 
				temp_overlap_value = cal_overlap_value( obstacle_avoding_Kruskal[i] , obstacle_avoding_Kruskal[j] );

				if( temp_overlap_value > max_overlap_value ){
					max_overlap_value = temp_overlap_value ; 	 
					index_b = j ;
					find_index_b = true ; 
				}

			}// end of already_rectilinear[i] == false

		}// end of loop j 

		// 有找到  才要做事 ， 因為這裡只要考慮 二個edge 的  , 只要有找到就可以了  ， 以乎不用去分它的case  是怎樣  
		if(  find_index_b == true  ){
		 
			vertex common_vertex(0 , 0 , 0) , near(0 , 0 , 0), far(0 , 0 , 0)  ; 
			vector<vertex> all_vertex ; 
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node1_x() 
							, obstacle_avoding_Kruskal[i].print_node1_y() , obstacle_avoding_Kruskal[i].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[i].print_node2_x() 
							, obstacle_avoding_Kruskal[i].print_node2_y() , obstacle_avoding_Kruskal[i].print_node2_z() ) );

			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node1_x() 
						, obstacle_avoding_Kruskal[index_b].print_node1_y() , obstacle_avoding_Kruskal[index_b].print_node1_z() ) );
			all_vertex.push_back( vertex( obstacle_avoding_Kruskal[index_b].print_node2_x() 
						, obstacle_avoding_Kruskal[index_b].print_node2_y() , obstacle_avoding_Kruskal[index_b].print_node2_z() ) );
			// 先抓出共用的那個點吧  , 總共四個點 ， 最後會變成 三個  , 二個 loop  , 只要比對到一個一樣的  就把那給刪了
			// 因為只有四個 element  所以  0 ， 1 ， 2  ， 3 四個 index 
			unsigned int index_i = 0 , index_j = 0  ; 
			bool find_common_vertex = false ; 
			for(unsigned int h = 0 ; h < all_vertex.size() - 2 ; h++ ){

				for(unsigned int j = 2 ; j < all_vertex.size()  ; j++ ){

					if( all_vertex[h].print_x_position() == all_vertex[j].print_x_position() &&
					    all_vertex[h].print_y_position() == all_vertex[j].print_y_position() ){

						index_i = h ;  
						index_j = j ;  
						common_vertex.set_position( all_vertex[h].print_x_position() , all_vertex[h].print_y_position() );
						common_vertex.set_z_position( all_vertex[h].print_z_position() );
						find_common_vertex = true ; 
						break ; 

					}// end of ix
				 
				}// loop j 
			 
			}// loop i 
			// 那接下來 ， 因為有抓到了  index 也就是 相同值的那個element 的位置  ， 把 這二個相同的給刪了 ， 只剩下二個了
			// 相同的二個  我先刪了  第一個 ， 所以第二個 要刪的位置  ， 其 index 要少一個位置 
			if( find_common_vertex == true ){
			 
			 	// 這裡是刪掉那個相同的 vertex 
				all_vertex.erase( all_vertex.begin() + index_j ) ; 
				all_vertex.erase( all_vertex.begin() + index_i ) ; 
				// 裡面只剩二個  elements ， 所以就把它給寫死了  指定過去吧 
				near.set_position( all_vertex[0].print_x_position() , all_vertex[0].print_y_position() );
				near.set_z_position( all_vertex[0].print_z_position() );
				far.set_position( all_vertex[1].print_x_position() , all_vertex[1].print_y_position() );
				far.set_z_position( all_vertex[1].print_z_position() );

			}

			// 這裡就是要看 ， 第三個edge有沒有比較短  ， 而且  是否在 OASG 內   , 因為還有 vertex_id 的關係 ， 
			// 所以  ， 很容易之道  ， 那條edge 有沒有在裡面  , 那個 記 點跟那些點有連 edge的那個資結 ， 叫什
			// 麼名字丫   , 喔  ， 叫table  , 先抓出  ， 它們三個點 分別是誰吧   , 點抓好了  ， 那接下來  便是看 
			// near 跟far 這二點的長度 ， 有沒有比  其它二個 edge 短  ， 有的話  ， 才要 換過來  ,  拿最長的 來 換 
			int third_edge_weight = abs( near.print_x_position() - far.print_x_position() ) + 
						abs( near.print_y_position() - far.print_y_position() ) +
						abs( near.print_z_position() - far.print_z_position() ) ; 

			// 下面二個  用來記  ， third_edge 的 ， 兩端點的vertex_id , 看是否存在著這個edge 
			int one = -1 , two = -1 ; 
			one = every_level_info[near.print_z_position()].vertex_id[ near.print_x_position() ][ near.print_y_position() ] ; 
			two = every_level_info[near.print_z_position()].vertex_id[ far.print_x_position()  ][  far.print_y_position() ] ; 
			bool exist_or_not = false ; 
			for(unsigned int j = 0 ; j < table[one].size() ; j++ ){
				if( table[one][j].print_desitination_vertex_index() == two ){
					exist_or_not = true ;  
					break ; 
				}
			}// scan all vertex on index one 

			// 等一下  有可能  ， 二個都比它大  ， 所以 要先比看看 那二個edge 誰比較大   , 等於至少要放在一邊  
			if( exist_or_not == true 									    && 
			    obstacle_avoding_Kruskal[i].print_weight() >=  obstacle_avoding_Kruskal[index_b].print_weight() && 
			    obstacle_avoding_Kruskal[i].print_weight() >   third_edge_weight   				    ){

				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + i ) ; 
				i-- ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// 確實有改到 ， 但  量不多  ， 像 800 x 200 也才改了 3 個而已 
				// cout << "Do i change somthing " << endl ; 
			 
			}
			else if( exist_or_not == true 										&& 
				 obstacle_avoding_Kruskal[index_b].print_weight() >  obstacle_avoding_Kruskal[i].print_weight()	&&
				 obstacle_avoding_Kruskal[index_b].print_weight() >  third_edge_weight 				){

				// 把最長的那個edge 拿掉 ， 換這個短的進去  	 
				obstacle_avoding_Kruskal.erase( obstacle_avoding_Kruskal.begin() + index_b ) ; 
				obstacle_avoding_Kruskal.push_back( edge( near.print_x_position() , near.print_y_position() , 
									  far.print_x_position() , far.print_y_position()   ) ) ; 
				// cout << "Do i change somthing " << endl ; 
			}// end of else if

		}// 兩個  ， 有找到配對的  


	}// scan all elements on obstacle_avoding_Kruskal , what we need is false element , not do rectilinear 


	// cout << "obstacle_avoding_Kruskal'size  : " << obstacle_avoding_Kruskal.size() <<  endl ; 
	// 下面這裡  ， 要把 給了的id 還原  , 而原始的值 ， 就是 －1 
	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){

		for( unsigned int j = 0 ; j < every_level_info[i].array_vertex_vector.size() ; j++ ){
			int x = every_level_info[i].array_vertex_vector[j].print_x_position() ; 
			int y = every_level_info[i].array_vertex_vector[j].print_y_position() ; 
			every_level_info[i].vertex_id[x][y] = -1 ; 
		}// 把每一層的vertex 都給抓出來  

	}// end of for

}// end of build_obstacle_avoding_kruskal 


/*-----------------------------------------------------------------------------
 *   下面這個 應該是處理 2D 的 ， 只不過 我東西的名字 ， 好像沒換掉
 *-----------------------------------------------------------------------------*/
void obstacle_avoding_kruskal::print_obstacle_avoding_kruskal_gun( vector<edge> &SG_edge , level_info &my_level_info ) {
	
	// 先寫出 ,SG的腳本好了  , 這裡有個很奇的現像 , 那就是  我先畫 OASG 再畫 Spanning_graph , 它會把OASG掛上去哎	
	// 所以執行gnuplot 的順序是  PlaneData   Spanning_graph  OASG   很奇怪 , 我上面的其實是complete graph ,
	// 雖然建出來的東西 應該是一樣的 , 但會花較多的時間  
	ofstream SGcout("Obstacle_Avoding_Kruskal_Tree.gnu");
	SGcout << " set title \"Obstacle_Avoding_Kruskal_Tree\" " << endl ; 
	SGcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	SGcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	SGcout << "set xlabel \"X-axis\"" << endl ; 
	SGcout << "set ylabel \"Y-axis\"" << endl ; 
	SGcout << "set output " << endl ; 
	SGcout << "set terminal X11 " << endl ; 
	/*-----------------------------------------------------------------------------
	SGcout << "set terminal postscript color" << endl ; 
	SGcout << "set output \"Obstacle_Avoding_Kruskal_Tree.ps\"" << endl ; 
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

}// end of print_obstacle_kruskal_gun

void obstacle_avoding_kruskal::print_overlap_obstacle_avoding_kruskal_gun( vector<edge> &SG_edge , level_info &my_level_info ) {
	
	// 先寫出 ,SG的腳本好了  , 這裡有個很奇的現像 , 那就是  我先畫 OASG 再畫 Spanning_graph , 它會把OASG掛上去哎	
	// 所以執行gnuplot 的順序是  PlaneData   Spanning_graph  OASG   很奇怪 , 我上面的其實是complete graph ,
	// 雖然建出來的東西 應該是一樣的 , 但會花較多的時間  
	ofstream SGcout("Obstacle_Avoding_Kruskal_Tree.gnu");
	SGcout << " set title \"Obstacle_Avoding_Kruskal_Tree\" " << endl ; 
	SGcout << " set xrange[0:" << my_level_info.plane_width - 1 <<  "]" << endl ; 
	SGcout << " set yrange[0:" << my_level_info.plane_height - 1 <<  "]" << endl ; 
	SGcout << "set xlabel \"X-axis\"" << endl ; 
	SGcout << "set ylabel \"Y-axis\"" << endl ; 
	SGcout << "set output " << endl ; 
	SGcout << "set terminal X11 " << endl ; 
	/*-----------------------------------------------------------------------------
	SGcout << "set terminal postscript color" << endl ; 
	SGcout << "set output \"Obstacle_Avoding_Kruskal_Tree.ps\"" << endl ; 
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
	SGcout << "plot \"Overlap_PlaneData.txt\" w lp pt 7 lc rgb \"black\" , \"Overlap_Vertex.txt\" w points  pt 7  lc rgb  \"green\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	SGcout.close() ;

}// end of print_obstacle_kruskal_gun

void obstacle_avoding_kruskal::print_threeD_obstacle_avoding_kruskal_gun( vector<edge> &ThreeD_obstacle_avoding_Kruskal  , vector<level_info> &every_level_info ){

	// 先寫出 ,SG的腳本好了  , 這裡有個很奇的現像 , 那就是  我先畫 OASG 再畫 Spanning_graph , 它會把OASG掛上去哎	
	// 所以執行gnuplot 的順序是  PlaneData   Spanning_graph  OASG   很奇怪 , 我上面的其實是complete graph ,
	// 雖然建出來的東西 應該是一樣的 , 但會花較多的時間  
	ofstream SGcout("3D_obstacle_avoding_kruskal_tree.gnu");
	SGcout << " set title \"3D_obstacle_avoding_kruskal_tree\" " << endl ; 
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
	for(unsigned int i  = 0 ; i < ThreeD_obstacle_avoding_Kruskal.size() ; i++){
		// 我想這裡 也要改了  ， 下面 這個 func 原是用來處理 2D 的 
		// edge.h 已經改過了 ， 所以裡面是 3D 的 position 
		ThreeD_obstacle_avoding_Kruskal[i].print_3D_edge_information_for_gnu( SGcout , arrow_count + i + 1 );// i + 1 只是obstacle 的編號而己 
	}

	//PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　, 所以要分開　
	SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" , \"projection_vertex.txt\" w points  pt 7  lc rgb  \"blue\" " << endl ; 
	//SGcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" " << endl ; 
	SGcout << "pause -1 " << endl ;
	//SGcout << "pause -1 " << endl ; 
	SGcout.close() ;

}// end of print_threeD_obstacle_avoding_kruskal_gun

#endif
