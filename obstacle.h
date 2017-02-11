/*
 * =====================================================================================
 *
 *       Filename:  obstacle.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010/12/04 17時52分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef         OBSTACLE_H	
	#define 	OBSTACLE_H	
#include	<cstdio>
#include	<iostream>
#include	<fstream>
#include	<vector>
#include	<cstdlib>

using namespace std ; 
class obstacle{

	private:
	//這裡的node1 , node2  是採　左下右上來看
	// 我果然只有存 對角線的二個點
	//  之後 要改成四個點  還要記它們的生死 
	//  看是否活著
	//   node3 是左上  node4 是右下 
	int node1_x ;
	int node1_y ; 	

	int node2_x ;
	int node2_y ; 	

	int node3_x ;
	int node3_y ;

	int node4_x ; 	
	int node4_y ; 	

	// 記得沒錯的話  ， 我 obstacle_id , 在這四個corner 
	// 分別是取 1 2 3 4  , 我想 0 呢 可以用在 我接下來要產生的
	// 侯選點 ， 但 我又有另一種名字 叫關係點  
	int obstacle_id ; 

	// 不更改內部的, 1 2 3 4 , 直接設定名字好了 
	bool ld_covered ; 
	bool lu_covered ; 
	bool rd_covered ; 
	bool ru_covered ; 

	public:

	obstacle(){
	
	}

	obstacle( int x , int y , int xx , int yy , int nu ){
		// 奇怪了 ， 我怎麼看不懂 ， 這個obstacle 是怎麼建的
		// 告夭 ， 我怎麼會把 ， 這樣定義node1 , 跟2 呢
		node1_x = x ;
		node1_y = y ; 	
		node2_x = xx ;
		node2_y = yy ; 	
		node3_x = node1_x ;
		node3_y = node2_y ; 	
		node4_x = node2_x ;
		node4_y = node1_y ; 	

		obstacle_id = nu ; 

		// true 代表活著
		ld_covered = true ; 
		lu_covered = true ; 
		rd_covered = true ; 
		ru_covered = true ; 
	}

	// copy constructor
	obstacle(const obstacle & obstacle_ref ){
		node1_x = obstacle_ref.node1_x  ;
		node1_y =  obstacle_ref.node1_y ; 	
		node2_x = obstacle_ref.node2_x ;
		node2_y = obstacle_ref.node2_y ; 	
		node3_x = obstacle_ref.node3_x ;
		node3_x = obstacle_ref.node3_x ;
		node4_y = obstacle_ref.node4_y ; 	
		node4_y = obstacle_ref.node4_y ; 	
		obstacle_id = obstacle_ref.obstacle_id ;

		ld_covered =  obstacle_ref.ld_covered ; 
		lu_covered =  obstacle_ref.lu_covered ; 
		rd_covered =  obstacle_ref.rd_covered ; 
		ru_covered =  obstacle_ref.ru_covered ; 
	}

	void set_obstacle_id( int id ){
		obstacle_id = id ; 	
	}

	// 下面幾個 func 是要用來設定overlap 的東西
	void set_ld_covered ( bool alive_or_not ){
		ld_covered = alive_or_not ;  	
	}

	void set_lu_covered ( bool alive_or_not ){
		lu_covered = alive_or_not ;  	
	}

	void set_rd_covered ( bool alive_or_not ){
		rd_covered = alive_or_not ;  	
	}

	void set_ru_covered ( bool alive_or_not ){
		ru_covered = alive_or_not ;  	
	}

	bool print_ld_covered (  ){
		return ld_covered  ;  	
	}

	bool print_lu_covered (  ){
		return lu_covered  ;  	
	}

	bool print_rd_covered (  ){
		return rd_covered  ;  	
	}

	bool print_ru_covered (  ){
		return ru_covered  ;  	
	}
	
	// 下面這些是設定non_overlap的
	int print_ld_corner_x(){
		return node1_x ; 
	}
	int print_ld_corner_y(){
		return node1_y ; 
	}

	int print_ru_corner_x(){
		return node2_x ; 
	}
	int print_ru_corner_y(){
		return node2_y ; 
	}

	int print_rd_corner_x(){
		return node2_x ; 
	}
	int print_rd_corner_y(){
		return node1_y ; 
	}

	int print_lu_corner_x(){
		return node1_x ; 
	}
	int print_lu_corner_y(){
		return node2_y ; 
	}

	int print_obstacle_id(){
		return obstacle_id ; 
	}
	void print_out_for_check(){
		cout << " obstacle_id = " << obstacle_id << endl ; 
		cout << " Left_Down corner = ( " << node1_x << " , " << node1_y << " ) " << endl ;  
		cout << " Right_Up corner = ( " << node2_x << " , " << node2_y << " ) " << endl  << endl ;
	}
	
	void print_3D_obstacle( ofstream &fcout , int &which_level , int &arrow_count ){
			
		/*-----------------------------------------------------------------------------
		  void print_edge_information_for_gnu(ofstream &fcout , int which_level , int line_nu ){
			fcout << "set arrow " << line_nu << " from " << node1_x << " , " << node1_y << " , " << which_level << " to " 
			     << node2_x << " , " << node2_y << " , " << which_level << "  nohead lt 1 lc rgb \"black\" lw 1 " << endl ; 
		  }
		  上面那是一個 畫線的 範例 ， 這裡呢  我要畫四個 線  用來當 obstacle 
		 *-----------------------------------------------------------------------------*/
		//  吃剛進來的那個count 所以不用加 左邊垂直線
		fcout << "set arrow " << arrow_count << " from " << node1_x << " , " << node1_y << " , " << which_level << " to " 
		     << node1_x << " , " << node2_y << " , " << which_level << "  nohead lt 1 lc rgb \"red\" lw 1 " << endl ; 

		arrow_count++ ; 
		fcout << "set arrow " << arrow_count << " from " << node1_x << " , " << node2_y << " , " << which_level << " to " 
		     << node2_x << " , " << node2_y << " , " << which_level << "  nohead lt 1 lc rgb \"red\" lw 1 " << endl ; 
		
		arrow_count++ ; 
		fcout << "set arrow " << arrow_count << " from " << node2_x << " , " << node2_y << " , " << which_level << " to " 
		     << node2_x << " , " << node1_y << " , " << which_level << "  nohead lt 1 lc rgb \"red\" lw 1 " << endl ; 

		arrow_count++ ; 
		fcout << "set arrow " << arrow_count << " from " << node2_x << " , " << node1_y << " , " << which_level << " to " 
		     << node1_x << " , " << node1_y << " , " << which_level << "  nohead lt 1 lc rgb \"red\" lw 1 " << endl ; 

		// 給下個線用的 
		arrow_count++ ; 
	}// end of print_3D_obstacle
};

#endif 
