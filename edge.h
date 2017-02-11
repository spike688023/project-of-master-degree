/*
 * =====================================================================================
 *
 *       Filename:  edge.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010/11/15 11時54分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef         EDGE_H
	#define 	EDGE_H

#include	<cstdio>
#include	<iostream>
#include	<fstream>
#include	<vector>
#include	<cstdlib>
#include	<cmath>
using namespace std ; 
class edge{

private :
  int node1_x ;
  int node1_y ; 
  int node1_z ; 
  int node2_x ;
  int node2_y ;
  int node2_z ;
  bool obstacle_seg_or_not ; 
  bool cross_level_edge ; 
  int weight ; 
	// 這裡有個現像 , 如果把這裡的變數 , 直接給值 , compiler 會把它當成是 static 
  int subtree_id  ; 
  int group_id ; 
  int overlap_count ; 
  bool kruskal_edge_ok_or_not ; 
  vector<int> which_obstacle_cross ; 
  int rec_id ; 
public :
  int node1_id , node2_id ; 
  // 初值設定用-1 ,  免得會產生有的沒的　的error 
   edge(){
  	node1_x = -1 ;	
  	node1_y = -1 ;	
  	node1_z = -1 ;	
  	node2_x = -1 ;	
  	node2_y = -1 ;	
  	node2_z = -1 ;	
	obstacle_seg_or_not = false ; 
	cross_level_edge  = false ; 
	weight = 0 ; 
	// group_id 是用在最後一步的，分群要用的, 0 代表沒分到群 
	group_id = 0 ; 
	subtree_id = 0 ; 
	kruskal_edge_ok_or_not = true ; 
	overlap_count = 0 ; 
	rec_id = 0; 
	node1_id = -1 ; 
	node2_id = -1 ; 
  } 
   
  // 最後一個參數 , 我是直接給一個預設的false  , 所以這個constructor , 給四個變數也是可以的 ,  我想錯誤 可能是這裡 ， 因為
  // 我private data 內 有放 z1 , z2 這二個 但這裡面  我沒給他設定 值  我想會是這裡出錯 
  edge(int x1 ,int  y1 ,int  x2 ,int  y2 , bool xxx = false , bool yyy = true , bool zzz = false ){
  	node1_x = x1 ;	
  	node1_y = y1 ;	
  	node2_x = x2 ;	
  	node2_y = y2 ;	
	obstacle_seg_or_not = xxx  ; 
	cross_level_edge  = zzz ; 
	group_id = 0 ; 
	weight = 0 ; 
	subtree_id = 0 ; 
	kruskal_edge_ok_or_not = yyy ; 
	overlap_count = 0 ; 
	rec_id = 0 ; 
	node1_id = -1 ; 
	node2_id = -1 ; 
  }


  // 我怕這個建構子 ， 會跟後面的 ， 有分不清的情況 , 這裡總共 ， 9個參數
  edge(int x1 ,int  y1 , int z1 ,int  x2 ,int  y2 ,int  z2, bool xxx = false , bool yyy = true , bool zzz = false ){
  	node1_x = x1 ;	
  	node1_y = y1 ;	
  	node1_z = z1 ;	
  	node2_x = x2 ;	
  	node2_y = y2 ;	
  	node2_z = z2 ;	
	obstacle_seg_or_not = xxx  ; 
	cross_level_edge  = zzz ; 
	group_id = 0 ; 
	weight = 0 ; 
	subtree_id = 0 ; 
	kruskal_edge_ok_or_not = yyy ; 
	overlap_count = 0 ; 
	rec_id = 0 ; 
	node1_id = -1 ; 
	node2_id = -1 ; 
  }

  edge(int x1 ,int  y1 ,int  x2 ,int  y2 , int rec_id_2 , bool xxx = false , bool yyy = true , bool zzz = false ){
  	node1_x = x1 ;	
  	node1_y = y1 ;	
  	node2_x = x2 ;	
  	node2_y = y2 ;	
	obstacle_seg_or_not = xxx  ; 
	cross_level_edge  = zzz ; 
	group_id = 0 ; 
	weight = 0 ; 
	subtree_id = 0 ; 
	kruskal_edge_ok_or_not = yyy ; 
	overlap_count = 0 ; 
	rec_id = rec_id_2 ; 
	node1_id = -1 ; 
	node2_id = -1 ; 
  }


  // 我是怕會跟這個弄不清楚 ， 這裡有10個參數  ， 但我在用的時侯都是給七個 ， 我怕會用到上面那個 
  edge(int x1 ,int  y1 , int z1 ,int  x2 ,int  y2 , int z2 , int rec_id_2 , bool xxx = false , bool yyy = true , bool zzz = false ){
  	node1_x = x1 ;	
  	node1_y = y1 ;	
  	node1_z = z1 ;	
  	node2_x = x2 ;	
  	node2_y = y2 ;	
  	node2_z = z2 ;	
	obstacle_seg_or_not = xxx  ; 
	cross_level_edge  = zzz ; 
	group_id = 0 ; 
	weight = 0 ; 
	subtree_id = 0 ; 
	kruskal_edge_ok_or_not = yyy ; 
	overlap_count = 0 ; 
	rec_id = rec_id_2 ; 
	node1_id = -1 ; 
	node2_id = -1 ; 
  }

  // 這裡我想建一個新的constructor ，只不過 是傳入二個vertex 

  // 這就是所謂的copy contructor 
  edge( const edge  &cp_edge ){
 	node1_x = cp_edge.node1_x ;
 	node1_y = cp_edge.node1_y ;
 	node1_z = cp_edge.node1_z ;
 	node2_x = cp_edge.node2_x ;
 	node2_y = cp_edge.node2_y ;
 	node2_z = cp_edge.node2_z ;
	group_id = cp_edge.group_id ; 
	obstacle_seg_or_not = cp_edge.obstacle_seg_or_not ; 
	cross_level_edge = cp_edge.cross_level_edge ;  
	weight = cp_edge.weight ;
	subtree_id = cp_edge.subtree_id ;
	kruskal_edge_ok_or_not = cp_edge.kruskal_edge_ok_or_not ;
	overlap_count = cp_edge.overlap_count ; 
	rec_id = cp_edge.rec_id ; 
	node1_id = cp_edge.node1_id ; 
	node2_id = cp_edge.node2_id ; 
  }
  
  // overload assignement operator 
 edge& operator=( const edge& temp ){

	if( this != &temp ){
		node1_x = temp.node1_x ;
		node1_y = temp.node1_y ;
		node1_z = temp.node1_z ;
		node2_x = temp.node2_x ;
		node2_y = temp.node2_y ;
		node2_z = temp.node2_z ;
		group_id = temp.group_id ; 
		obstacle_seg_or_not = temp.obstacle_seg_or_not ; 
		cross_level_edge = temp.cross_level_edge ;  
		weight = temp.weight ;
		subtree_id = temp.subtree_id ;
		kruskal_edge_ok_or_not = temp.kruskal_edge_ok_or_not ;
		overlap_count = temp.overlap_count ; 
		rec_id = temp.rec_id ; 
		node1_id = temp.node1_id ; 
		node2_id = temp.node2_id ; 
	}// end of if 不能把相同的東西 ， assign 給自已   , this 是指 = 左邊 呼叫這個func 的物件
	
	return *this ; 
 }// end of oberload assignement operator  

  void set_position_of_node(int x1 ,int  y1 ,int  x2 ,int  y2){
  	node1_x = x1 ;	
  	node1_y = y1 ;	
  	node2_x = x2 ;	
  	node2_y = y2 ;	
  }


  void set_position_of_node(int x1 ,int  y1 , int z1 ,int  x2 ,int  y2 , int z2 ){
  	node1_x = x1 ;	
  	node1_y = y1 ;	
  	node1_z = z1 ;	
  	node2_x = x2 ;	
  	node2_y = y2 ;	
  	node2_z = z2 ;	
  }

  //這裡要寫個func , 把這個edge 印出來　，　方便我之後寫　gnuplot的腳本
  void print_edge_information_for_gnu(ofstream &cout , int line_nu ){
  	cout << "set arrow " << line_nu << " from " << node1_x << " , " << node1_y  << " to " 
             << node2_x << " , " << node2_y << "  nohead lt 1 lc rgb \"black\" lw 1 " << endl ; 
  }

  void print_3D_edge_information_for_gnu(ofstream &cout , int line_nu ){
  	cout << "set arrow " << line_nu << " from " << node1_x << " , " << node1_y << " , " << node1_z << " to " 
             << node2_x << " , " << node2_y << " , " << node2_z << "  nohead lt 1 lc rgb \"black\" lw 1 " << endl ; 
  }

  void print_edge_information_for_gnu(ofstream &cout , int which_level , int line_nu ){
  	cout << "set arrow " << line_nu << " from " << node1_x << " , " << node1_y << " , " << which_level << " to " 
             << node2_x << " , " << node2_y << " , " << which_level << "  nohead lt 1 lc rgb \"black\" lw 1 " << endl ; 
  }
  	// output my Segment 
	void print_seg(){
	cout << "seg  = ( " << node1_y << " , " << node2_y << " ) , obstacle_seg_or_not  = " << obstacle_seg_or_not <<   endl ; 	
	}

	bool print_seg_state(){
		return obstacle_seg_or_not ; 		
	}
	int print_left_of_seg(){
		return node1_y ; 		
	}

	int print_right_of_seg(){
		return node2_y ; 		
	}
	// 我記得 次方的func是  pow(a,2)  => a的平方
	// 我這裡的設定有錯 ， 這樣一來 走直線的weight會比起 ， 
	// 走 斜的  weight 還要多 
	int print_weight(){
			// 這裡不能用a^2 + b^2 開根號的方法去加 ， 會有誤差 ， 會導致錯誤 
			// 所以要用 水平加 垂直的方法 去當weight 才行 ， 先試試吧 
			// return sqrt ( (node1_x - node2_x) * (node1_x - node2_x) + (node1_y - node2_y) * (node1_y - node2_y) ) ;	
			return fabs( node1_x - node2_x ) + fabs( node1_y - node2_y )  ;	
	}

	int print_3D_weight(){
			// 這裡不能用a^2 + b^2 開根號的方法去加 ， 會有誤差 ， 會導致錯誤 
			// 所以要用 水平加 垂直的方法 去當weight 才行 ， 先試試吧 
			// return sqrt ( (node1_x - node2_x) * (node1_x - node2_x) + (node1_y - node2_y) * (node1_y - node2_y) ) ;	
			return fabs( node1_x - node2_x ) + fabs( node1_y - node2_y )  + fabs( node1_z - node2_z );	
	}
	//  以下四個 , 分別印出  , 這二個node的x , y 	
	void print_all_node_info(){
		cout << " node1_x = " << node1_x << endl ;  
		cout << " node1_y = " << node1_y << endl ;  
		cout << " node2_x = " << node2_x << endl ;  
		cout << " node2_y = " << node2_y << endl ;  
	}

	void print_all_node_info_2( int count ){
		cout << "This is edge " << count << " : " << "( " << node1_x << "," << node1_y << ") to " << "( " << node2_x << "," << node2_y << ") " << endl ; 
	}
	int print_node1_x(){
		return node1_x ;  
	}	

	int print_node1_y(){
		return node1_y ;  
	}

	int print_node1_z(){
		return node1_z ;  
	}

	int print_node2_x(){
		return node2_x ;  
	}	

	int print_node2_y(){
		return node2_y ;  
	}

	int print_node2_z(){
		return node2_z ;  
	}

	void set_cross_level_edge_y_or_n( bool bo ){
		cross_level_edge = bo ; 
   	}

	bool print_cross_level_edge_y_or_n( ){
		return cross_level_edge  ; 
   	}

	// 建Tree			
	void set_subtree_id(int id){
		subtree_id = id ; 
   	}
        int return_subtree_id(){
		return subtree_id ;
        }
	// 判斷kruskal_edge_ok_or_not 
	void set_kruskal_edge_ok_not(bool xxx){
		kruskal_edge_ok_or_not = xxx ; 
	} 
	bool print_kruskal_edge_ok_not(){
		return kruskal_edge_ok_or_not  ; 
	} 
	// 下面三個　func ，　用來記路　那些obstacle跟這條edge 有相交　 
	// 我要再多加個func ， 把這個vector 內的元素淨空　，　以便加後面的
	void add_which_obstacle_cross(int which){
		which_obstacle_cross.push_back(which);	
	}
	int which_obstacle_cross_size(){
		return which_obstacle_cross.size() ; 
	}
	int print_which_obstacle_cross(int which){
		return which_obstacle_cross[which] ;	
	}
	void erase_all_elements(){
		which_obstacle_cross.erase( which_obstacle_cross.begin() , which_obstacle_cross.end() );
	}
	/*-----------------------------------------------------------------------------
	 *  void delete_the_same_id_of_obstacle(){
		for(unsigned int i = 0 ; i < which_obstacle_cross.size() - 1  ; i++  ){
			for(unsigned int j = 0 ; j < which_obstacle_cross.size() ; j++  ){
				 
			}
		}
	}// end of func delete_the_same_id_of_obstacle

	 *-----------------------------------------------------------------------------*/
	
        // 這裡我要寫四個func 來輸出,這個edge 中
	// 最大及最小的　x , y 
	int print_min_x(){

		if( node1_x > node2_x  ){
			return node2_x ; 
		}
		else{
			return node1_x ; 
		}

	}// end of min_x

	int print_max_x(){

		if( node1_x > node2_x  ){
			return node1_x ; 
		}
		else{
			return node2_x ; 
		}
	}// end of max_x

	int print_min_y(){

		if( node1_y > node2_y  ){
			return node2_y ; 
		}
		else{
			return node1_y ; 
		}
	}// end of min_y	

	int print_max_y(){

		if( node1_y > node2_y  ){
			return node1_y ; 
		}
		else{
			return node2_y ; 
		}
	}// end of max_y

	// 用在kruskal Tree 中的　，　edge 　，　印出　它是tree or false ,
	// 以及其　　cross 的obstacle 有誰
	void print_kruskal_edge_information(){
		cout << "First node's location = ( " << node1_x << " ,  "  << node1_y << ") " << endl; 
		cout << "Second node's location = ( " << node2_x << " ,  "  << node2_y << ") " << endl; 
		cout << "Below is information of obstacle crossed : " ; 
		if(  which_obstacle_cross.size() > 0 ){
			for(unsigned int i = 0 ; i < which_obstacle_cross.size() ; i++  ){
				cout << which_obstacle_cross[i] << "  " ; 
			}
			cout << endl << endl ; 
		}
	}// end of print_kruskal_edge_information 

	// 分群用的func 
	void set_group_id( const int id ){
		group_id = id ; 	
	}
	int print_group_id(){
		return group_id ; 
	}
	// 用來判斷這個edge 是否水平 or 垂直 , 應該不是這樣解是 ， 
	//  此edge 是 水平 or 垂直 就回傳 true  , 如果 是斜線  就回傳 fasle 
	bool Is_vertical_or_horizontal_line( ){
		
		if( (node1_x == node2_x) || ( node1_y == node2_y ) ){
			return true ; 			 
		}
		else {
			return false ; 
		}
	}// en of func Is_vertical_or_horizontal_line 	

	// Is_horizontal_line
	bool Is_horizontal_line(){
		if( node1_y == node2_y ){
			return true ; 
		}
		else{
			return false ; 
		}
	}// end of Is_horizontal_line

	// 來判斷edge 的case 是左下右上 or 左上右下 
	// 是左下右上的case 便回傳true  另一種則回傳false 
	bool Is_left_down_right_up_case(){
		int x_interval = node1_x - node2_x ; 
		int y_interval = node1_y - node2_y ;
		if( x_interval * y_interval > 0 ){
			return true ;  
		}
		else if( x_interval * y_interval < 0 ){
			return false ;  
		}
		return true ; 
	}// end of 判斷edge的型態 
	// overlap_count func
	void overlap_count_add_add(){
		overlap_count++;
	}
	int print_overlap_count(){
		return overlap_count ; 
	}
	// for_rectilinear  set and print rec_id
	void set_rec_id( int temp ){
		rec_id = temp ; 	
	}
	int print_rec_id( ){
		return rec_id ; 	
	}

	void set_two_node1_2_id( int id_1 , int id_2 ){
		node1_id = id_1 ;  
		node2_id = id_2 ;  
	}
};

#endif 
