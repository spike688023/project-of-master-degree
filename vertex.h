/*
 * =====================================================================================
 *
 *       Filename:  vertex.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010/11/07 15時48分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef         VERTEX_H
	#define 	VERTEX_H
#include	<iostream>
#include	<fstream>
#include	"edge.h"
#include	<vector>
#include        <cstdio>
using namespace std ; 

 class vertex {

 private:
   bool pin ;
   int x_cord  ;
   int y_cord  ;
   int z_cord  ;
   int obstacle_id ; 
   int corner_id ;  
   bool  core ; 
   // 這裡也要記住　，　l_boundary 是比　r_boundary 大
   int l_boundary ;
   int r_boundary ;
   int id ; 

 public:
   int which_group ; 

   vertex( ){
   	// pin 給 false 代表我預設 它是 obstacle corner 
	pin = false ;
	x_cord = 0 ; 
	y_cord = 0 ; 
	z_cord = 0 ; 
	obstacle_id = 0 ;
	corner_id = 0 ;
	core = true  ; // true 代表這個　core還活著　　，　core 的位置　就是vertex 的y_cord
	l_boundary = - 1;
	r_boundary = -1 ;
	id = -1 ; 
	which_group = -1 ; 
   }
   vertex( int x , int y , int z = 0 ,  bool state = false ){
	pin = state ;
	x_cord = x ; 
	y_cord = y ; 
	/*-----------------------------------------------------------------------------
	 z_cord = z ; 它媽的 ， 原來是少了這個
	 難怪 ， 我上面參數  z ＝ 0  , 是想說 ， 第三個參數預設給 0 
	 也就是說  我只放前二個參數 ， 但body 內 ， 我竟然少放了 z_cord = z 
	 使得 我給三個參數時 ， 第三個會給的很怪 
	 *-----------------------------------------------------------------------------*/
	z_cord = z ; 
	obstacle_id = 0 ;
	corner_id = 0 ;
	core = true  ;
	l_boundary = - 1;
	r_boundary = -1 ;
	id = -1 ; 
	which_group = -1 ; 
   }

   // 建個copy constructor 
   vertex(const vertex &cp_vertex){
   	// 總共要定的值  九個 沒錯丫
	pin = cp_vertex.pin ;
	x_cord = cp_vertex.x_cord ; 
	y_cord = cp_vertex.y_cord ; 
	z_cord = cp_vertex.z_cord ; 
	obstacle_id = cp_vertex.obstacle_id ;
	corner_id = cp_vertex.corner_id ;
	core = cp_vertex.core ; 
	l_boundary = cp_vertex.l_boundary ;
	r_boundary = cp_vertex.r_boundary ;
	id = cp_vertex.id ;
	which_group = cp_vertex.which_group ; 
   }


 vertex& operator=( const vertex& temp ){

	if( this != &temp ){
   		// 總共要定的值  九個 沒錯丫
		pin = temp.pin ;
		x_cord = temp.x_cord ; 
		y_cord = temp.y_cord ; 
		z_cord = temp.z_cord ; 
		obstacle_id = temp.obstacle_id ;
		corner_id = temp.corner_id ;
		core = temp.core ; 
		l_boundary = temp.l_boundary ;
		r_boundary = temp.r_boundary ;
		id = temp.id ; 
		which_group = temp.which_group ; 
	}// end of if 不能把相同的東西 ， assign 給自已   , this 是指 = 左邊 呼叫這個func 的物件
	
	return *this ; 
 }// end of oberload assignement operator  

   //建plane_information 要用到的
   void set_position(int xx , int yy){
	    x_cord = xx ; 
	    y_cord = yy ; 
	} 

   void set_z_position(int zz ){
	    z_cord = zz ; 
	} 

   void set_pin(bool p_or_o){ pin = p_or_o; }
   
   void set_corner(int corer){ corner_id = corer ; }
   void set_obstacle_id(int nu){ obstacle_id = nu ;}

   //依x位置，　將所有vertex 做分類
   int  print_x_position() const{ return x_cord;}
   int  print_y_position() const{ return y_cord;}
   int  print_z_position() const{ return z_cord;}
   int  print_obstacle_id(){ return obstacle_id;}

   //建gnu script 
   void print_position(ofstream &fcout){
   	fcout << x_cord << "  " << y_cord  << "  " << endl << endl ; 
   }

   //  下面這個func 我在用 z_cord 時 會有錯 ， 我想是  我在 產 Projection_vertex 時  ， 
   //  建構z 的參數時 ， 有傳錯東酉進去
   void print_3D_position_gnu(ofstream &fcout , const int &which_level ){
   	fcout << x_cord << "  " << y_cord  << "  " << which_level << " " << endl << endl ; 
   }

   void print_3D_position_gnu(ofstream &fcout ){
   	fcout << x_cord << "  " << y_cord  << "  " << z_cord << " " << endl << endl ; 
   }

   void print_position(){
   	cout << " ( " << x_cord << " ,  " << y_cord  << " , " << z_cord <<  " ) " <<  endl ; 
   }
   void print_obstacle_for_script(ofstream &fcout ){
	 fcout << x_cord  << "," << y_cord  << "  "  ;
   }
   bool Is_pin_or_not(){
   	return pin ; 
   }
   
   int print_corner_id(){
   	return corner_id ; 
   } 
   // 建立每個vertex 的 bounding box interval 
   void set_interval( int l , int r ){
   	// vertex::print_corner_id() ; 
	// 奇怪了  好像可以丫  
	l_boundary = l ;
	r_boundary = r ;
   }
   void set_l_interval( int l  ){
	l_boundary = l ;
   }
   void set_r_interval( int r  ){
	r_boundary = r ;
   }
   int print_l_interval(   ){
	return l_boundary  ;
   }
   int print_r_interval(   ){
	return r_boundary   ;
   } 	
   bool  modify_interval_and_add_edgs_to_OASG(bool obstacle_seg_or_not ,int x ,   int y1  , int y2 , vector<edge> &OASG , int  , bool , bool , int );

   void print_interval(){
   	cout << "which corner = "<< corner_id << "  interval  =  ( " << l_boundary << " ,  " << y_cord << " , " << r_boundary << " ) "  << 
	" , core = " << core << endl ; 
   }
   // 
   void set_core_state( bool core_state ){
   	core = core_state ;
   }
   bool core_state(){
   	return  core ; 
   }
   
   void set_id( int idd  ){
   	id = idd ;
   }

   int get_id(  )const{
   	return id ; 
   }

   void print_out_for_check(){
  	cout << endl << " pin or not = " << pin  << endl; 	 
  	cout << " x_position = " << x_cord      << endl; 	 
  	cout << " y_position = " << y_cord      << endl; 	 
  	cout << " z_position = " << z_cord      << endl; 	 
  	cout << " corner_id  = " << corner_id   << endl; 	 
   }  

};

   //傳入seg 性質　，　及seg 左右邊界　，　來看要否更新此interval 
   // y1 > y2 , 因為是由上往下掃
   // 我想　回傳值　要設成bool  , 如有更新到則回傳true , 好跑向下一個vertex 
   bool  vertex::modify_interval_and_add_edgs_to_OASG(bool obstacle_seg_or_not ,int x ,   int y1  , int y2 , vector<edge> &OASG , 
   						       int height , bool head_state , bool tail_state  , int which_level ){
		// 先定出二種規格  那就是 , 如果interval 的core 在seg 內 , 便回傳true 讓h 往下個interval 跑
		// 反之回傳false , 因為一個interval 只會被一個seg effect , 要記得plane的頭跟尾 不能讓它連線 
		// 先寫個架構  看我 傳入的seg 是否有寫好  	
		if( y_cord <= y1 && y_cord >= y2 ){
			// 先解決 , 傳入的segment 兩端點  , 有否打中我的y_cord , 這個case的優先權很高 
			//  || (y1 == height && head_state == true) 會導致我錯很多東西　，　why 
			if( y_cord == y1 && (y1 != height)  ){
				core = false ; 
				OASG.push_back( edge(x_cord , y_cord , which_level , x , y1 , which_level ) );
				return true ; 
			}
			else  if( y_cord == y1 &&  (y1 == height && head_state == true) ){
				core = false ; 
				OASG.push_back( edge(x_cord , y_cord , which_level  , x , y1 , which_level  ) );
				return true ; 
			}
			// tail_state == true , 代表底是個vertex 
			//   || (y2 == 0 && tail_state == true) 
			else if(  y_cord == y2 && (y2 != 0)  ){
				core = false ; 
				OASG.push_back( edge(x_cord , y_cord , which_level ,  x , y2 , which_level ) );
				return true ; 
			}
			else if(  y_cord == y2 &&  (y2 == 0 && tail_state == true) ){
				core = false ; 
				OASG.push_back( edge(x_cord , y_cord ,  which_level ,  x , y2 , which_level ) );
				return true ; 
			}

			if( obstacle_seg_or_not == true && core == true ){
				// 之後還要再處理一個case 那就是 頭尾 其實是vertex , 目前先不考慮
				// head segment
				if( y1 == height  && y2 != 0 ){
					//這個case , 我只要考慮 , y2 有沒有在這個interval 的右區塊(不含cordy) , 但有含r_boundary  
					if(  y2 < y_cord && y2 >= r_boundary ){
 							OASG.push_back( edge(x_cord , y_cord , which_level , x , y2 , which_level ) );
					}
					if(  y1 > y_cord && y1 <= l_boundary && head_state == true ){
							OASG.push_back( edge(x_cord , y_cord , which_level ,  x , y1 , which_level ) );
					}
				}
				// tail segment
				else if( y2 == 0 && y1 != height ){
					if(  y1 > y_cord && y1 <= l_boundary ){
							OASG.push_back( edge(x_cord , y_cord , which_level ,  x , y1 , which_level ) );
					}
					if(  y2 < y_cord && y2 >= r_boundary && tail_state == true ){
 							OASG.push_back( edge(x_cord , y_cord , which_level  ,  x , y2 , which_level ) );
					}
				}
				// normal segment 
				else{
					// 因為己經在segment 內了 , 所以 不會有 y1 跑到 右區塊  , y2 跑到左區塊的case 
					if( y1 > y_cord && y1 <= l_boundary   ){
 						OASG.push_back( edge(x_cord , y_cord , which_level , x , y1 , which_level ) );
					}
					if( y2 < y_cord && y2 >= r_boundary   ){
 						OASG.push_back( edge(x_cord , y_cord , which_level  ,  x , y2 , which_level ) );
					}
				}
				// 有擋住 , 最後一定要給false  , 所以上面的body 只是判斷連線的東西 
				core  = false ; 
			}// end of obstacle_seg_or_not == true 
			else if( obstacle_seg_or_not == false && core == true  ){
				// 因為core 還活著 , 所以要設 左右邊界的新值
				if( y1 == height && y2 != 0 ){
					//這個case , 我只要考慮 , y2 有沒有在這個interval 的右區塊(不含cordy) , 但有含r_boundary  
					if(  y1 > y_cord && y1 <= l_boundary && head_state == true  ){
 							OASG.push_back( edge(x_cord , y_cord , which_level  , x , y1 , which_level ) );
							l_boundary = y1 - 1 ;// 加完  剛好跟y_cord 相同 有什麼不好的事嗎   
					}
					if(  y2 < y_cord && y2 >= r_boundary   ){
 							OASG.push_back( edge(x_cord , y_cord , which_level , x , y2 , which_level ) );
							r_boundary = y2 + 1 ;// 加完  剛好跟y_cord 相同 有什麼不好的事嗎   
					}
				}
				// tail segment
				else if( y2 == 0 && y1 != height  ){
					if(  y1 > y_cord && y1 <= l_boundary   ){
 							OASG.push_back( edge(x_cord , y_cord , which_level  ,  x , y1 , which_level ) );
							l_boundary = y1 - 1 ;// 加完  剛好跟y_cord 相同 有什麼不好的事嗎   
					}
					if(  y2 < y_cord && y2 >= r_boundary && tail_state == true   ){
 							OASG.push_back( edge(x_cord , y_cord , which_level  , x , y2 , which_level ) );
							r_boundary = y2 + 1 ;// 加完  剛好跟y_cord 相同 有什麼不好的事嗎   
					}
				}
				// 這是個少見的case 
				else if( y1 == height && y2 == 0 ){
					 if(  y1 > y_cord && y1 <= l_boundary  && head_state == true ){
 							OASG.push_back( edge(x_cord , y_cord , which_level  , x , y1 , which_level ) );
							l_boundary = y1 - 1 ;// 加完  剛好跟y_cord 相同 有什麼不好的事嗎   
					 }
					 if(  y2 < y_cord && y2 >= r_boundary && tail_state == true   ){
 							OASG.push_back( edge(x_cord , y_cord , which_level  , x , y2 , which_level ) );
							r_boundary = y2 + 1 ;// 加完  剛好跟y_cord 相同 有什麼不好的事嗎   
					 } 
				}
				// normal segment , 想不到有傳入的seg 是 頭到尾的  
				else{
					// 因為己經在segment 內了 , 所以 不會有 y1 跑到 右區塊  , y2 跑到左區塊的case 
					if( y1 > y_cord && y1 <= l_boundary   ){
 						OASG.push_back( edge(x_cord , y_cord , which_level  , x , y1 , which_level ) );
						l_boundary = y1 - 1 ;
					}
					if( y2 < y_cord && y2 >= r_boundary    ){
 						OASG.push_back( edge(x_cord , y_cord , which_level , x , y2 , which_level ) );
						r_boundary = y2 + 1 ;
					}
				}
				core  = true ; 
			}// end of obstacle_seg_or_not == false

			// 我之道為什麼會出現 , 一直無限loop了 , 因為我的終止點設在 , h到底 
			//core = false ; // 少這個會無限loop  
 			return true ; 
		}
		else{
			return false ; 
		}
		
		/*-----------------------------------------------------------------------------
			 		return false ; 		
		 *-----------------------------------------------------------------------------*/

	}// end of func

#endif
