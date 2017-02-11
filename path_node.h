/*
 * =====================================================================================
 *
 *       Filename:  path_node.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010/12/07 14時11分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */


#ifndef       PATH_NODE_H 
	#define  PATH_NODE_H 

#include	<iostream>
#include	<vector>
#include        <cstdio>
#include        "edge.h"

/*-----------------------------------------------------------------------------
 *  
#ifndef         EDGE_H
	#define 	EDGE_H
#endif 
 *-----------------------------------------------------------------------------*/

using namespace std ; 
class path_node{
	private:
		bool infinite ; 
		bool used ; 
		int path_weight ; 
		int trans_node ; 
		int belong_which_row ; 
		int desitination_vertex_id ; 
	public:
	path_node(){
		used = false ; 
		infinite = true ; 
		path_weight = 0 ; 
		trans_node = -1 ; 
		desitination_vertex_id = - 1 ; 
		belong_which_row = - 1 ;  
	}
	// copy constructor
	path_node(const path_node &pn ){
		used = pn.used ; 
		infinite = pn.infinite ; 
		path_weight = pn.path_weight ; 
		trans_node = pn.trans_node ; 
		desitination_vertex_id = pn.desitination_vertex_id  ; 
		belong_which_row = pn.belong_which_row  ;  
	}

	path_node( bool inf , int path ){
		used = false ; 
		infinite = inf ; 
		path_weight = path ; 
		trans_node = -1 ; 
		desitination_vertex_id = - 1 ; 
		belong_which_row = - 1 ;  
	}

	path_node(int source_row ,  int path , int des ){
		used = false ; 
		infinite = false ; 
		path_weight = path ; 
		trans_node = -1 ; 
		desitination_vertex_id = des  ; 
		belong_which_row = source_row ;  
	}

	path_node( bool inf , int path , int des ){
		used = false ; 
		infinite = inf ; 
		path_weight = path ; 
		trans_node = -1 ; 
		desitination_vertex_id = des  ; 
		belong_which_row = - 1 ;  
	}
	// infinite 
	void set_infinite(bool inf){
		infinite = inf ; 	
	}
	bool print_infinite(){
		return infinite; 	
	}
	// belong_which_row  
	void set_belong_which_row( int source ){
		belong_which_row = source ; 
	}
	int print_belong_which_row( ){
		return belong_which_row ; 	
	}

	// path_weight
	void set_path_weight( int path ){
		path_weight = path  ; 	
	}
	int print_path_weight( ){
		return path_weight  ; 	
	}
	// desitination_vertex_index 
	void set_desitination_vertex_index( int des ){
		desitination_vertex_id = des ; 	
	}
	int print_desitination_vertex_index( ){
		return desitination_vertex_id ; 	
	}
	// trans_node
	void set_trans_node(int number ){
		trans_node = number ; 
	}
	int print_trans_node(){
		return trans_node ; 
	}
	// used
	void set_used(bool us){
		used = us ; 	
	}
	bool print_used(){
		return used ; 
	}
	// 
	void print_path_node_info(){
		cout <<  " infinite = " << infinite ;
		cout <<  " , used = " << used ;
		cout <<  " , path_weight = " << path_weight ;
		cout <<  " , trans_node = " << trans_node << endl ;
	}
	// test ifndef 這個東西  上面有寫了個這樣的東西以後
	// 我能不能呼叫edge 內的東西
	
	/*-----------------------------------------------------------------------------
	 *   
	void edge_test(){
		edge edge(1,1,1,1);		
	}
	ok 了  原來  這就是組合  ， 

	下面這斷code是放在 夾住標頭檔的  ， 不是放在引用的那裡
	#ifndef         EDGE_H
		#define 	EDGE_H
	#endif 
	 *-----------------------------------------------------------------------------*/
};

#endif
