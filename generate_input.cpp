 /*   c  按　　這個c 不之道什麼時侯　，　放在最前頭　，　害你爸　一直debug               CC    
 * ===================================================================================== 
 * 
 *       Filename:  generate_input.cpp 
 *
 *    Description:  as above   
 * 
 *        Version:  1.0
 *        Created:  2010/11/07 13時52分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *          
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */
   
#include	<iostream>
#include	<fstream>
#include	<vector>
#include	<cstdio>
#include	<cstdlib>
#include	<ctime>
/*-----------------------------------------------------------------------------
 下面這個header file 是 ， 要平行化用的東西
 我只記得 角括號是呼叫  內建的file ， 雙引號則是
 自訂的file  , 那上面角括中 ， 有的file要打.h 有的不用
 我記得是 c 跟cpp  之間的差別 
 我有則直試著編 get_input.cpp 而沒有其它的.o檔 ， 
 但compile  出來的並不是 執行檔  ， 我想是因為沒有其它 .o 檔的關係 
#include	<omp.h>
 *-----------------------------------------------------------------------------*/
#include	<omp.h>

/*-----------------------------------------------------------------------------
 *  下面之所以不用再放一個 edge.h 是因為 ,      
	我在vertex.h 內 , 己經include 了 edge.h 了  
	看來 , 最好不要在 自定的header內 , 載入 
	互相載入, 如果是內定的header 就沒關係了       
#include 	"vertex.h"	
#include 	"obstacle.h"	
 *-----------------------------------------------------------------------------*/
// 我想要在下面這個class中  ， 加入 可以 吃3D 的 func   , 因為3D 下的 OASG 都了 投影點 
// 雖然 主要也是在 做2D 的OASG 但 我原先的 OASG 碰沒有對 Projection 這個做判斷
// 所以 我要把這個給加進去  
#include 	"my_non_overlap_OASG.h"	
#include 	"my_overlap_OASG.h"	
#include 	"O_2D_OASG.h"	
#include 	"kruskal_tree.h"	
#include 	"obstacle_avoding_kruskal.h"	
#include 	"rectilinear.h"	
#include 	"level_info.h"	
// #include 	"Projection_handler.h"	

/*-----------------------------------------------------------------------------
 下面這個寫法  ， 是為了避免多重載入相同的class 而寫的
 我上網查的data 說  檔名都要大寫 ， 而且 .h 要寫成 _H
 來改看看吧 , compiler 也是會過拉   , 晚點來試看看別的class看看 
#ifndef         EDGE_H
	#define 	EDGE_H
#endif 
   
	上面這斷code是放在 夾住標頭檔的  ， 不是放在引用的那裡
 *-----------------------------------------------------------------------------*/


#include	<cmath>
#include	<algorithm>
#include	<map>


using namespace std ; 




/*-----------------------------------------------------------------------------
 *  sort_y_position, 用來sort vertex 內的y軸 , 要用降密的方式
bool sort_y_position(vertex v1 , vertex v2){
	return ( v1.print_y_position() > v2.print_y_position() );	
}
 *-----------------------------------------------------------------------------*/

 /*-----------------------------------------------------------------------------
 *  sort_x_position, 用來sort vertex 內的x軸 , 要用升密的方式    
bool sort_x_position(vertex v1 , vertex v2){
	return ( v1.print_x_position() < v2.print_x_position() );	
}
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * sort weight  ,  for SG pick the smallest edge  
bool sort_weight( edge e1 , edge e2 ){
	return ( e1.print_weight() < e2.print_weight() );
}
 *-----------------------------------------------------------------------------*/


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  overlap_horizontal_or_not
 *  Description:  
 * =====================================================================================
 */
bool	
overlap_horizontal_or_not ( edge& edg1 , edge& edg2  )
{

	if( ( edg1.print_max_y() == edg2.print_min_y() )  || 
	    ( edg1.print_min_y() == edg2.print_max_y() )  ){
		return true ;  
	}
	else if( ( edg1.print_max_x() == edg2.print_min_x() )  || 
	         ( edg1.print_min_x() == edg2.print_max_x() )  ){
		return false ;  
	}
	else{
		return false ; 
	}

}		/* -----  end of function overlap_horizontal_or_not  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  how_much_overlap_value
 *  Description:  
 * =====================================================================================
 */
int 	
how_much_overlap_value ( edge& edg1 , edge& edg2 , bool horizontal_case_or_not )
{
	// 第三個參數 ， 是用來看 ， 傳入的二個edge 是overlap_horizontal or vertical  		
	// 只要之道是水平 or 垂直 ， 我就直接 回傳 二個方型中  ， 最短的
	// width or height 
	if( horizontal_case_or_not == true  ){
		if( edg1.print_max_x() - edg1.print_min_x() >=  edg2.print_max_x() - edg2.print_min_x() ){
			return  edg2.print_max_x() - edg2.print_min_x() ; 
		}
		else{
			return edg1.print_max_x() - edg1.print_min_x() ; 	
		}
	}
	else{
		if( edg1.print_max_y() - edg1.print_min_y() >=  edg2.print_max_y() - edg2.print_min_y() ){
			return  edg2.print_max_y() - edg2.print_min_y() ; 
		}
		else{
			return edg1.print_max_y() - edg1.print_min_y() ; 	
		}	
	}
}		/* -----  end of function how_much_overlap_value  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_plane_big_enough
 *  Description:   false  不夠大  true 代表夠大   
 * =====================================================================================
 */
bool check_plane_big_enough (  vector<level_info> &every_level_info )
{

	bool enough = false ; 
	for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
		if( every_level_info[i].percent_used > 100  ){
			enough = false ; 
		}
		else{
			enough = true ; 
		}
	}// 初始化 

	if( enough == true ){
		return true ; 
	}
	else{
		return false ; 	
	}

}		/* -----  end of function check_plane_big_enough  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  overlaps_position_x_y
 *  Description:  
 * =====================================================================================
 */
int 	
overlaps_position_x_y ( edge& edg1  , edge& edg2 , bool h_or_v  )
{
	if( h_or_v == true ){
		// 水平 ， 所以回傳 ， y	 
		if(  edg1.print_max_y() == edg2.print_min_y()    ){
			return edg1.print_max_y();  
		}
		else if( edg1.print_min_y() == edg2.print_max_y() ){
			return edg1.print_min_y()  ;
		}
	}
	else{
		if(  edg1.print_max_x() == edg2.print_min_x()    ){
			return edg1.print_max_x();  
		}
		else if( edg1.print_min_x() == edg2.print_max_x() ){
			return edg1.print_min_x()  ;
		}

	}// end of else block 

	return 0 ;
}		/* -----  end of function overlaps_position_x_y  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Is_the_strange_case
 *  Description:  
 * =====================================================================================
 */
bool	
Is_the_strange_case ( edge& edg , int x , int y  )
{
	// 這裡要抓的是 ， 在edg 中的二端點， 只要有一個 點 ， 是跟 x,y 完全對上
	// 就回傳true 
	if( edg.print_node1_x() == x && edg.print_node1_y() == y ){
		return true ;  
	}
	else if( edg.print_node2_x() == x && edg.print_node2_y() == y ){
		return true ;  
	}
	else{
		return false ; 
	}
}		/* -----  end of function Is_the_strange_case  ----- */






/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_input
 *  Description:  這裡呢  本來是打算 只用來吃2D的input , 像 似乎也可改成3D的 那一層的變數
 *  		  我想用 plane_level ， 而各層的變數  ， 我打算全用一個新的class 包起來
 *  		  要處理到 2D 的routing 時 ， 就把源頭的資料 全換 過來  這就可以了
 * =====================================================================================
 */
	void
get_input (  vector<level_info> &every_level_info  , int &Overlap_or_not )
{
	
	int plane_width = 0 , plane_height = 0  , plane_level = 0  , obstacle_nu = 0 , pin_nu = 0 ;

	cout << endl << " ---------------------------------Input:--------------------------"  << endl ; 

	cout << " (0) Non_Overlap_Mode (1) Overlap_Mode " << endl   ; 
	cout << "Choice the Mode you want :" ; 
	cin >> Overlap_or_not ; 

	cout << "Input plane_level : "   ;
	cin >> plane_level ; 

	cout << "Input plane_width : "   ;
	cin >> plane_width ; 

	cout << "Input plane_height: "   ;
	cin >> plane_height ;
	
	cout << "How many obstacle you want : " ;
	cin >> obstacle_nu ; 	

	cout << "How many pins you want : " ;
	cin >> pin_nu ; 

	// non_overalp case 
	if( Overlap_or_not == 0  ){
	 
		/*-----------------------------------------------------------------------------
		 只要看 ， every_level_info 內 ， 建了幾個元素  就之道
		 user 是要建幾層了, 我想 還是由第 0 層開始好了  
		 重改以後 我有個想法  ， 用class 切開以後 ， 自已要的變數
		 自已去抓 
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < plane_level ; i++ ){
		 
			// 下面的 level_info 建構子  總共有三個參數
			// 第一個是 長  ， 第二個是 寬  ， 第三個是第幾層 
			// 看來  我還是要 把 總共有幾層 要記下來  
			every_level_info.push_back( level_info( plane_width , plane_height ,  i  , plane_level , obstacle_nu , pin_nu ) );	
			// 讓 obstacle_nu , pin_nu 有個初值

			/*-----------------------------------------------------------------------------
			 *  
			if( i == 1 ){
				obstacle_nu = temp_obstacle_nu ;  
				pin_nu = temp_pin_nu ; 
			}
			 *-----------------------------------------------------------------------------*/

		}// end of for
		
		cout << endl << " --------------------------Below is info u want :--------------------------"  ; 
		for(int i = 0 ; i < plane_level ; i ++ ){
		 
			/*-----------------------------------------------------------------------------
			 *  
			 下面才是真正  在抓出 obstacle_pin ， 上面只是抓下使用者要的東西而已 
			 奇怪了  我把下面這個 func 放到上面的 loop 中  它會多一次  問你
			 要多少個 obstacle 及 pin  哎  
			 主要是下面這個func 在 work pin 跟 obstacle 所以 
			 要弄出關係點 ， 就要從 下面這個func 著手   
			 哇 ， 不行  因為 你只能看到自已那層的資訊
			 但project 要 能看到各層才行  要在這裡 額外再寫個
			 func 來處理 projection 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].generate_obstacle_pin() ; 	 

		}// end of 3D generate_obstacle_pin

	}// end of non_overalp case 
	else if( Overlap_or_not == 1  ){
	 
	// 那我不管是一層還是多層的 ， overlap 我的input 都弄好了
		/*-----------------------------------------------------------------------------
		 只要看 ， every_level_info 內 ， 建了幾個元素  就之道
		 user 是要建幾層了, 我想 還是由第 0 層開始好了  
		 重改以後 我有個想法  ， 用class 切開以後 ， 自已要的變數
		 自已去抓 
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < plane_level ; i++ ){
		 
			// 下面的 level_info 建構子  總共有三個參數
			// 第一個是 長  ， 第二個是 寬  ， 第三個是第幾層 
			// 看來  我還是要 把 總共有幾層 要記下來  
			every_level_info.push_back( level_info( plane_width , plane_height ,  i  , plane_level , obstacle_nu , pin_nu ) );	
			// 讓 obstacle_nu , pin_nu 有個初值

			/*-----------------------------------------------------------------------------
			 *  
			if( i == 1 ){
				obstacle_nu = temp_obstacle_nu ;  
				pin_nu = temp_pin_nu ; 
			}
			 *-----------------------------------------------------------------------------*/

		}// end of for
		
		cout << endl << " --------------------------Below is info u want :--------------------------"  ; 
		for(int i = 0 ; i < plane_level ; i ++ ){
		 
			/*-----------------------------------------------------------------------------
			 *  
			 下面才是真正  在抓出 obstacle_pin ， 上面只是抓下使用者要的東西而已 
			 奇怪了  我把下面這個 func 放到上面的 loop 中  它會多一次  問你
			 要多少個 obstacle 及 pin  哎  
			 主要是下面這個func 在 work pin 跟 obstacle 所以 
			 要弄出關係點 ， 就要從 下面這個func 著手   
			 哇 ， 不行  因為 你只能看到自已那層的資訊
			 但project 要 能看到各層才行  要在這裡 額外再寫個
			 func 來處理 projection 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].generate_overlap_obstacle_pin() ; 	 

		}// end of 3D generate_obstacle_pin
	}// end of overlap_case

}		/* -----  end of function get_input_and_initialize_plane  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_input_auto , 的多型 ， 有 8個參數 
 *  Description:  這裡呢  本來是打算 只用來吃2D的input , 像 似乎也可改成3D的 那一層的變數
 *  		  我想用 plane_level ， 而各層的變數  ， 我打算全用一個新的class 包起來
 *  		  要處理到 2D 的routing 時 ， 就把源頭的資料 全換 過來  這就可以了
 *  		  好怪喔 ， 不過這裡又多學到一個東西 ， 就可以 由外部傳入的變數值 ， 是不能改的 
 *  		  也就是argv[0] 內的值 
 * =====================================================================================
 */
	void
get_input_auto (  vector<level_info> &every_level_info  , const int &Overlap_or_not  , const int &levels , const int &width , const int &height , const int &obstacles , const int &pins)
{
	
	int plane_width = 0 , plane_height = 0  , plane_level = 0  , obstacle_nu = 0 , pin_nu = 0 ;
	plane_level = levels ; 
	plane_width = width ; 
	plane_height = height ; 
	obstacle_nu = obstacles ; 
	pin_nu = pins ; 
	


	// non_overalp case 
	if( Overlap_or_not == 0  ){
	 
		/*-----------------------------------------------------------------------------
		 只要看 ， every_level_info 內 ， 建了幾個元素  就之道
		 user 是要建幾層了, 我想 還是由第 0 層開始好了  
		 重改以後 我有個想法  ， 用class 切開以後 ， 自已要的變數
		 自已去抓 
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < plane_level ; i++ ){
		 
			// 下面的 level_info 建構子  總共有三個參數
			// 第一個是 長  ， 第二個是 寬  ， 第三個是第幾層 
			// 看來  我還是要 把 總共有幾層 要記下來  
			every_level_info.push_back( level_info( plane_width , plane_height ,  i  , plane_level , obstacle_nu , pin_nu ) );	
			// 讓 obstacle_nu , pin_nu 有個初值

			/*-----------------------------------------------------------------------------
			 *  
			if( i == 1 ){
				obstacle_nu = temp_obstacle_nu ;  
				pin_nu = temp_pin_nu ; 
			}
			 *-----------------------------------------------------------------------------*/

		}// end of for
		
		cout << endl << " --------------------------Below is info u want :--------------------------"  ; 
		for(int i = 0 ; i < plane_level ; i ++ ){
		 
			/*-----------------------------------------------------------------------------
			 *  
			 下面才是真正  在抓出 obstacle_pin ， 上面只是抓下使用者要的東西而已 
			 奇怪了  我把下面這個 func 放到上面的 loop 中  它會多一次  問你
			 要多少個 obstacle 及 pin  哎  
			 主要是下面這個func 在 work pin 跟 obstacle 所以 
			 要弄出關係點 ， 就要從 下面這個func 著手   
			 哇 ， 不行  因為 你只能看到自已那層的資訊
			 但project 要 能看到各層才行  要在這裡 額外再寫個
			 func 來處理 projection 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].generate_obstacle_pin() ; 	 

		}// end of 3D generate_obstacle_pin

	}// end of non_overalp case 
	else if( Overlap_or_not == 1  ){
	 
	// 那我不管是一層還是多層的 ， overlap 我的input 都弄好了
		/*-----------------------------------------------------------------------------
		 只要看 ， every_level_info 內 ， 建了幾個元素  就之道
		 user 是要建幾層了, 我想 還是由第 0 層開始好了  
		 重改以後 我有個想法  ， 用class 切開以後 ， 自已要的變數
		 自已去抓 
		 *-----------------------------------------------------------------------------*/
		for(int i = 0 ; i < plane_level ; i++ ){
		 
			// 下面的 level_info 建構子  總共有三個參數
			// 第一個是 長  ， 第二個是 寬  ， 第三個是第幾層 
			// 看來  我還是要 把 總共有幾層 要記下來  
			every_level_info.push_back( level_info( plane_width , plane_height ,  i  , plane_level , obstacle_nu , pin_nu ) );	
			// 讓 obstacle_nu , pin_nu 有個初值

			/*-----------------------------------------------------------------------------
			 *  
			if( i == 1 ){
				obstacle_nu = temp_obstacle_nu ;  
				pin_nu = temp_pin_nu ; 
			}
			 *-----------------------------------------------------------------------------*/

		}// end of for
		
		cout << endl << " --------------------------Below is info u want :--------------------------"  ; 
		for(int i = 0 ; i < plane_level ; i ++ ){
		 
			/*-----------------------------------------------------------------------------
			 *  
			 下面才是真正  在抓出 obstacle_pin ， 上面只是抓下使用者要的東西而已 
			 奇怪了  我把下面這個 func 放到上面的 loop 中  它會多一次  問你
			 要多少個 obstacle 及 pin  哎  
			 主要是下面這個func 在 work pin 跟 obstacle 所以 
			 要弄出關係點 ， 就要從 下面這個func 著手   
			 哇 ， 不行  因為 你只能看到自已那層的資訊
			 但project 要 能看到各層才行  要在這裡 額外再寫個
			 func 來處理 projection 
			 *-----------------------------------------------------------------------------*/
			every_level_info[i].generate_overlap_obstacle_pin() ; 	 

		}// end of 3D generate_obstacle_pin
	}// end of overlap_case

}		/* -----  end of function get_input_and_initialize_plane  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_3D_obstacle_pin_graph
 *  Description:  用來做出 3D 下 ， 要的 txt 點圖 及 .gnu 檔 , 這個好像只能用副程式來寫
 *  		  好像不能 跟其它的class 弄在一起 
 * =====================================================================================
 */
	void
generate_3D_obstacle_pin_graph ( vector<level_info> &every_level_info )
{
	
	/*-----------------------------------------------------------------------------
	 *  
	 這裡 要做一個檔 如上面一班 是用來畫 3D 的 
	 上面是藉由 vertex  這個class 來處理 但
	 這不能用來處理3D 的東西 因為我裡面沒有放 
	 3D 的資訊 進去  哎  我想到了 
	 我多傳一個 3D 的資訊給它  不就好了麻 
	 哇  我要先 把pin 點的資訊弄進去
	 2D 下我是寫在 level_info 但 3D下的話 不行
	 2D 下 只能之道自已那層的而已 其它層的不之道
	 *-----------------------------------------------------------------------------*/
	ofstream threeDfcout("PlaneData_3D.txt");
	for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
	 
		for(int j = 0 ; j < every_level_info[i].total ; j++ ){
			// 這裡直接 拿 public 的 data member 有點難看 
			if( every_level_info[i].array_vertex[j].Is_pin_or_not() == true ){
				every_level_info[i].array_vertex[j].print_3D_position_gnu( threeDfcout , every_level_info[i].which_level ) ;	
			}
		}
	}// end of for , get every_level_info 


	threeDfcout.close();	

	// 差點忘記 ， 總共幾層level 是去算 level_info 的size 就好了 
	ofstream threeDcout("3D_PlaneData.gnu");
	threeDcout << " set title \"3D_PlaneData\" " << endl ; 
	threeDcout << " set xrange[0:" << every_level_info[0].plane_width - 1 <<  "]" << endl ; 
	threeDcout << " set yrange[0:" << every_level_info[0].plane_height - 1 <<  "]" << endl ; 
	threeDcout << " set zrange[0:" << every_level_info.size() <<  "]" << endl ; 
	threeDcout << "set xlabel \"X-axis\"" << endl ; 
	threeDcout << "set ylabel \"Y-axis\"" << endl ; 
	threeDcout << "set zlabel \"Z-axis\"" << endl ; 
	// 下面這個 我想換掉 
	
	threeDcout << "set output " << endl ; 
	threeDcout << "set terminal X11 " << endl ; 
	/*-----------------------------------------------------------------------------
	 *  
	threeDcout << "set terminal postscript color" << endl ; 
	threeDcout << "set output \"OASG.ps\"" << endl ; 
	 *-----------------------------------------------------------------------------*/
	threeDcout << "# 這裡要印出　obstacle 的資訊　" << endl ; 
	// gnu 內  object 的編號要大於0
	int arrow_count = 1 ; 
	for(unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
			 
		//   用下面這個範本來改好了  
		//threeDcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 
		// 這裡要讀obstacle 出來　　，　寫在PlaneData.gnu 裡面
		for(unsigned int j = 0 ; j < every_level_info[i].complete_obstacle_information.size() ; j++ ){

			// 中間這塊 ， 要藉由各層的 vertex 判斷其 是否為 obstacle_corner 
			// 來畫出 obstacle  , 那是之前的方法 ， 我現在要用complete_obstacle_information  
			// 來畫    
			
			every_level_info[i].complete_obstacle_information[j].print_3D_obstacle(threeDcout ,  every_level_info[i].which_level , 
											       arrow_count                        ) ; 	

		}// end of print out all obstacle  variable j 

	}// end of get every_level_info to print out  variable i 

	// PlaneData 只有含　pin 點的資訊而已 ,　我打算把edge 的資訊　都弄在.gnu裡面
	// 因為用txt 去讀出來畫　　會連obstacle 的四個角點　都畫出來　　
	// 我忘記 PlaneData 是在那裡產生出來的東西哎  , 我要另外做一個 3D 資訊的
	// 用在這裡 , 在 level_info.h 中的 generate_obstacle_pin 這個 func 的最下面 
	//threeDcout << "splot \"PlaneData_3D.txt\" w lp pt 7 lc rgb \"black\" " << endl ; 
	//  ps 1.2  point size 的意思
	threeDcout << "splot \"PlaneData_3D.txt\" w points  pt 7  lc rgb  \"black\" " << endl ; 
	threeDcout << "pause -1 " << endl ; 
	threeDcout.close() ;
	// 上面的這個 3D 的 整個 plane  , 只是 呈見在 terminal 那就沒有畫 ps 檔
	// 所以  如果 我要 連圖也畫  那我就在下面 再開個 ofstream 來寫進去就好拉  
}		/* -----  end of function generate_3D_obstacle_pin_graph  ----- */




/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculation_all_edge_weight
 *  Description:  用來計算 ， 這個stage , 全部 edge 的總weight 數 , 我都忘了  ， 還有
 *  		  vertical_edge , 不管 ， 先把這個給建起來好了 
 * =====================================================================================
 */
int 	
calculation_all_edge_weight ( vector<edge> &all_edge )
{
	int total_weight = 0 ; 
	for(unsigned int i = 0 ; i < all_edge.size() ; i++ ){
		total_weight += all_edge[i].print_3D_weight() ;  
	}// scan all edge 

	return total_weight ; 

}		/* -----  end of function calculation_all_edge_weight  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main    
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char* argv[])
{
	bool debug = false ; 
	/*-----------------------------------------------------------------------------
	 *   				這裡 ， 要把整下面的所有 step 都給放進來
	 *   		要一塊塊的改成獨立的模式 ， 以方便我以後使用 
	 *           
	 *    哇  放不進來  ， 我終於  了解  什麼叫 千一髮動全身了  
	 *    我初值設定的所有 input 在後面的所有stage 都會用到哎  
	 *
	 *    我想到了個 好方法 就是 依使用者 輸入的 level 看是要幾層 == 1 就用2D 的
	 *    > 1 就用 3D 的 , 好了 ， 接下來就是看 要怎麼切了  ,  用 every_level_info.size() 
	 *    來看 
	 *-----------------------------------------------------------------------------*/


	/*-----------------------------------------------------------------------------
	 *  				SETP 1 
	 *
	 *  		         	Generate_Input 
	 *    2D下的所有階段 我都完成了 ， 接下來呢  
	 *    是要把各個階段都做切割 ， 最好呢 是能做成 class的型式來用 
	 *    每塊都切割好了  且獨立的話  就能把 obstacle_generate 變成有overlap 
	 *    跟non_overalp 完看 我要用那一個  
	 *    OASG 也是 
	 *    雖然用 class 存起了各層的 obstacle_nu , pin_nu  , 但是這二個主要變數的值
	 *    我還沒有給 我打算 各層要 route 時 ， 再用
	 *    用物件記路 各層的資訊 包含total array_vertex 等等的 ， 下面各個stage 要用到
	 *    我再抽出來給它們用 就好了   , 我要把step 1 做出的所有 input , 都包在各個
	 *    不同的物件之中           
	 *-----------------------------------------------------------------------------*/
	vector<level_info> every_level_info ; 

	/*-----------------------------------------------------------------------------
	 下面的func 我不在傳 plane_width , plane_height 等等的這些東西了 
	 所以要之道使用者 是要畫 單曾還是多層 no ， 那個還是要放外面
	  不用哎  我只要看 every_level_info 這個vector 內 有幾個元素
	  就之道 建了幾層的 資訊  , 下面這個func 已經把 各層的東西
	  都弄出來了
	 *-----------------------------------------------------------------------------*/
	clock_t all_start , TwoD_end  ; 
	double TwoD_dif  ; 
	int  Overlap_or_not = 0 ; 

	if( argc == 1 ){
		get_input( every_level_info , Overlap_or_not ) ; 	
	}
	else if( argc == 7 ){
		// 7 個參數  ， 分別是 ./generate_input mode levels width height obstacle pin   , 除了  第一個代表執行檔的東西 ， 不用以外 ， 其它都要傳入 
		Overlap_or_not = atoi( argv[1] ) ; 
		get_input_auto( every_level_info , Overlap_or_not , atoi(argv[2])  , atoi(argv[3]) , atoi(argv[4]) , atoi(argv[5]) , atoi(argv[6]) ) ; 	
	}
	// time ( &all_start ) ; 
	all_start = clock() ; 
	


	// check_plane_big_enough ,  true 代表 夠大 ，  false  代表不夠大 
	if( Overlap_or_not == 0 && every_level_info.size() == 1 && check_plane_big_enough( every_level_info ) == true ){
	 
		/*-----------------------------------------------------------------------------
		 下面要畫 Plane_Data 的圖 , 我設定成  誰呼叫 就畫那一層  
		 但是  我還要把 PlaneData.gnu 改成  看是那一層的呼叫
		 不然  我多層有呼叫 也是只有一個 PlaneData.gnu 檔
		 先維持這樣好了  , 下面這個也只是 畫出 第一層的圖而已 
		 *-----------------------------------------------------------------------------*/
		every_level_info[0].print_PlaneData_gnu()  ;

		cout << endl << " ---------------------------------Here begain Routing Progress :--------------------------"  << endl ; 
		/*-----------------------------------------------------------------------------
		 *          
		 *
		 *
		 *		
		 *				STEP  2   
		 *				OASG  
		 *
		 *
		 * 
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  這裡要把　，　array_vertex 內的東西　，　放到相對應的x_position內
		 *  , 還要把  各個位置的 , head and tail ,  的true false 弄上去
		 *  head_tail_state 指的是  一個x 位置上  最高跟最低點 是否有 有vertex 在上面   
		 *  下面這個 vector<edge> OASG ， 能不能 放到 my_non_overlap_OASG 裡面當data 用
		 *  這樣我處理各層的OASG 只要把 那一層的資訊 傳進去  就可以了哎  
		 *  不是 更簡結 方便嗎
		 *  這裡 有弄出二個物件 ， 分別是  my_non_overlap_OASG , O_2D_OASG , 這二個都是用來
		 *  產生 OASG 的 只不過 方法不同 ， 前者是用我想的 ， 後者是 原作者的
		 *-----------------------------------------------------------------------------*/
		vector<edge> OASG ; 
		my_non_overlap_OASG my_OASG ; 	

		/*-----------------------------------------------------------------------------
		 我把下面的func 傳弟  ， 原本是打  &sort_y_position
		 但 我按rr 去作執行檔 有錯  ， 我現在之道那是什麼意思了  
		 應該是指  我之前compile過 應該有個相同時間的 執行檔
		 但是確沒有  只有一個時間點 就的執行檔  , 所以不給執行
		 我下面改成 不放&& 看能不能執行
		 file '/Users/nickboy/Program/Nov 29/generate_input' does not exist or is not executable or object/source older then executable 
		 看來 obstacle_plane 可以不用存丫  ， 下面都用不到
		 *-----------------------------------------------------------------------------*/
		clock_t start , end ; 
		clock_t O_start , O_end ; 
		double my_OASG_dif , O_OASG_dif  ; 

		start = clock() ; 
		// cout << endl << "Time of My_OASG start = " << start << endl ; 
		my_OASG.build_OASG( OASG , every_level_info[0] ) ;
		end = clock() ; 
		// cout << "Time of My_OASG end = " << end << endl ; 
		// 由下面可以發現 ， difftime 這個 func 第一個參數 是傳入 ， 較晚的時間  
		// 下面輸出us 的地方 ， 有點錯 ， 因為除以 CLOCKS_PER_SEC , 後 出來的東西是 ， secs , 所以還要乘上10的6次方
		my_OASG_dif = (double) ( end - start ) ; 
		// my_OASG_dif = (double)difftime ( end , start ) ; 
		cout << "Time of My_OASG spend = "  << (double) ( my_OASG_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ; 
		// 接下來 ， 我要印出 此時的 edge 總數 ， 如果都一樣 那就成功了  我覺得  
		cout << "Edges of My_OASG = " << OASG.size() << endl ; 

		/*-----------------------------------------------------------------------------
		  我要先把各個x位置的edge 畫上去　，　看有沒有畫錯　，　接下來才用
		  我想的bouding box interval  , ok 了  , 先試看看  丟一個去算 能不能跑出來
		  OASG.gnu
		  告夭 ， 我差點忘了 ， 這個是 畫出  ， ps檔 ， 所以不會直接把圖弄出來  
		  還是說 ， 我都弄成 ， 直接用terminal show出來  , 但這樣 ， 兩張圖如果 
		  有差異 ， 我很難比出來   
		 *-----------------------------------------------------------------------------*/
		my_OASG.print_OASG_gnu(  OASG , every_level_info[0] ) ;	

		vector<edge> OASG_O_2D ; 
		O_2D_OASG orignal ; 

		double difference = 0 ;
		O_start = clock() ; 
		// time ( &O_start ) ; 
		difference = orignal.build_OASG_fast_version( OASG_O_2D , every_level_info[0] ) ; 	
		O_end = clock() ; 
		// time ( &O_end   ) ; 
		// cout << "Time of O_OASG end = " << O_end << endl ; 
		O_OASG_dif = (double) ( O_end - O_start )  ; 
		cout << "Time of O_OASG spend = "  << (double) ( ( O_OASG_dif - difference )  / CLOCKS_PER_SEC ) * 1000000 << " us " << endl; 
		cout << "Edges of O_OASG = " << OASG_O_2D.size() << endl ; 

		// O_2D_OASG.gnu 
		orignal.print_OASG_gnu( OASG_O_2D , every_level_info[0] ) ; 	
		

		/*-----------------------------------------------------------------------------
		 *   這裡 ， 要比看看  ， 差了那些edges ， 以及 那些edge 的info  
		 *   這個我有用平行化哎  , 按 ， 平行化 的那個 loop 好像要signed 
		 *
		 *
		 if( OASG.size() == OASG_O_2D.size() ){
		 	cout <<  " Edges is equal " << endl << endl ;   
		 }
		 else if( OASG.size() > OASG_O_2D.size() ){
		 	//  真怕這一塊會比超久的  , edges 數目多的 ， 才用 index i 去指向吧  
		 	bool match = false ; 
			int OASG_size = OASG.size() , count = 0 ; 
			#pragma omp parallel for ordered 
			for(int i = 0 ; i < OASG_size ; i++ ){
				match = false ; 
				for(unsigned int j = 0 ; j < OASG_O_2D.size() ; j++ ){
					if( OASG[i].print_node1_x() == OASG_O_2D[j].print_node1_x() &&
					    OASG[i].print_node1_y() == OASG_O_2D[j].print_node1_y() &&
					    OASG[i].print_node2_x() == OASG_O_2D[j].print_node2_x() &&
					    OASG[i].print_node2_y() == OASG_O_2D[j].print_node2_y() ){
					    	match = true ; 
						break ; 
					}
					else if( OASG[i].print_node1_x() == OASG_O_2D[j].print_node2_x() &&
						 OASG[i].print_node1_y() == OASG_O_2D[j].print_node2_y() &&
						 OASG[i].print_node2_x() == OASG_O_2D[j].print_node1_x() &&
						 OASG[i].print_node2_y() == OASG_O_2D[j].print_node1_y() ){
						match = true ;  
						break ; 
					}
				}// 跟作者的方法比  

				if( match == false ){
					count++ ; 
					OASG[i].print_all_node_info_2( count ) ;  
				}

			}// my_OASG.size
			
		 
		 }// 這裡 要把 有差距的edge info 給弄出來看看  , 這裡只要考慮2D 的就可以了        
		 else if( OASG.size() < OASG_O_2D.size() ){
		 	//  真怕這一塊會比超久的  , edges 數目多的 ， 才用 index i 去指向吧  
		 	bool match = false ; 
			int OASG_O_2D_size = OASG_O_2D.size() , count = 0  ; 
			#pragma omp parallel for ordered 
			for(int i = 0 ; i < OASG_O_2D_size ; i++ ){
				match = false ; 
				for(unsigned int j = 0 ; j < OASG.size() ; j++ ){
					if( OASG[j].print_node1_x() == OASG_O_2D[i].print_node1_x() &&
					    OASG[j].print_node1_y() == OASG_O_2D[i].print_node1_y() &&
					    OASG[j].print_node2_x() == OASG_O_2D[i].print_node2_x() &&
					    OASG[j].print_node2_y() == OASG_O_2D[i].print_node2_y() ){
					    	match = true ; 
						break ; 
					}
					else if( OASG[j].print_node1_x() == OASG_O_2D[i].print_node2_x() &&
						 OASG[j].print_node1_y() == OASG_O_2D[i].print_node2_y() &&
						 OASG[j].print_node2_x() == OASG_O_2D[i].print_node1_x() &&
						 OASG[j].print_node2_y() == OASG_O_2D[i].print_node1_y() ){
						match = true ;  
						break ; 
					}
				}// 跟作者的方法比  

				if( match == false ){
					count++ ; 
					OASG[i].print_all_node_info_2( count ) ;  
				}

			}// OASG_O_2D.size
			
		 
		 }// 這裡 要把 有差距的edge info 給弄出來看看  , 這裡只要考慮2D 的就可以了  
		 *-----------------------------------------------------------------------------*/
		
		/*-----------------------------------------------------------------------------
		 *                               STEP 3    
		 *                  Kruskal tree , OAST  ,  rectilinear 
		 *                                 
		 *                   		Kruskal tree    
		 *-----------------------------------------------------------------------------*/

		// time_t stage3_start , stage3_end ; 
		clock_t stage3_start , stage3_end ;
		double stage3_dif  ; 

		vector<edge> SG_edge ; 
		kruskal_tree Kruskal_Tree ;  
		// 下面二個是弄2D 的
		
		// time( &stage3_start ) ; 
		stage3_start = clock() ; 
		Kruskal_Tree.build_2D_kruskal_tree(  SG_edge , every_level_info[0] ) ; 
		// time( &stage3_end ) ; 
		stage3_end = clock() ; 

		Kruskal_Tree.print_2D_kruskal_tree_gun(  SG_edge , every_level_info[0] );
		stage3_dif = (double) ( stage3_end - stage3_start ) ; 
		// 看來預設是 10 的6次方 ， 也就是us 
		// cout << "CLOCKS_PER_SEC " << CLOCKS_PER_SEC << endl ;  
		cout << endl << " Time spend of Kruskal tree = " << (double) (stage3_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  

		/*-----------------------------------------------------------------------------
			第二小階段 , 建OAST   
			1. 抓出Kruskal_Tree 內　那些edge 是ok , 那些有跨過obstacle 是non_ok 
			   我想呢，　還是直接用　完整的obstacle , 資訊　，　obstacle一個個看　
			   所以　只要判斷edge 形成的方形　跟obstacle的方形　有沒有交集　即可
 
			2. non_ok 的edge 中　，　是穿過了那些obstacle , 其四個端點很重要
 
			3. 把上面抓出的重要點，　到OSAG 內比對　，　只要有edge  的二端點，　是同時含那到那些點的edge 
			   便可以
 
			4. 抓出那些edge 後，　便是　求其最短路徑　
		 *-----------------------------------------------------------------------------*/

		// 下面這個是  用來放 處理好的edge 
		// 下面這個 vector  在我的 3D 下 是存了各層的OAST , 但沒關係 我只要給它 依
		clock_t stage4_start , stage4_end ; 
		double stage4_dif  ; 

		vector<edge> obstacle_avoding_Kruskal ;
		// OAST 是處理的物件
		obstacle_avoding_kruskal OAST ; 
		stage4_start = clock() ; 
		// time( &stage4_start ) ; 
		//  奇怪了  ， 我的 OASG 那裡有改東西  ， 變成錯的了   , 要再改一下  ， 所以  ， 先用 O_2D_OASG 的edge 來用 
		// OAST.build_obstacle_avoding_kruskal( SG_edge  , obstacle_avoding_Kruskal ,  OASG ,  every_level_info[0]);	
		OAST.build_obstacle_avoding_kruskal_imp( SG_edge  , obstacle_avoding_Kruskal ,  OASG ,  every_level_info[0]);	
		// time( &stage4_end  ) ; 
		stage4_end = clock() ; 
		stage4_dif = (double)( stage4_end - stage4_start ) ; 
		cout << endl << " Time spend of OAST = "  << (double) (stage4_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  

		/*-----------------------------------------------------------------------------
		 *  這裡要先　印出　，　obstacle_avoding_Kruskal 來看看 
		 *  我的想法是缺了　那些edge , 以至於　會找不到path
		 *-----------------------------------------------------------------------------*/
		OAST.print_obstacle_avoding_kruskal_gun(  obstacle_avoding_Kruskal , every_level_info[0] ) ;	


		/*-----------------------------------------------------------------------------
		 *   				Rectilinear
		 *   		這是我在2D下的最後一步了 。 
		 *   			a. 首先要分群 ，每個edge 以一個rect 來看，
		 *   			   相互有overlap , 設成同一群 ， 但不能單
		 *   			   獨 只看x or y 來看 ， 前提要先相鄰 ， 也就
		 *   			   是說 ， 新納入的edge  ,其兩端點 ，要跟某一群
		 *   			   的點集有關係 才行 ， 
		 *
		 *   		我想在外面 把 obstacle_avoding_Kruskal 內的edge 先依 level 分類 
		 *   		再丟到 rectilinear.h 內去處理  ， 然而 存那個處理後的edge 
		 *   		是用 obstacle_avoding_rectilinear_Kruskal 
		 *   			   
		 *-----------------------------------------------------------------------------*/

		clock_t stage5_start , stage5_end ; 
		clock_t stage6_start , stage6_end ; 
		double stage5_dif  ; 
		double stage6_dif  ; 

		if( debug == true ){
			cout << " Before get in OARK to do rectilinear " << endl ; 
		}

		rectilinear OARK ; 	

		vector<edge> obstacle_avoding_rectilinear_Kruskal ; 
		stage5_start = clock() ; 
		// time( &stage5_start ) ; 
		OARK.change_to_rectilinear_by_author(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal );	
		// OARK.change_to_rectilinear(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal );	
		// time( &stage5_end ) ; 
		stage5_end = clock() ; 
		stage5_dif = (double)( stage5_end - stage5_start ) ; 
		cout << endl << " Time spend of OARK by author ="  << (double) (stage5_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  


		vector<edge> obstacle_avoding_rectilinear_Kruskal_by_teacher ; 
		stage6_start = clock() ; 
		// time( &stage6_start ) ; 
		OARK.change_to_rectilinear_by_teacher(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal_by_teacher );	
		// OARK.change_to_rectilinear(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal );	
		// time( &stage6_end ) ; 
		stage6_end = clock() ; 
		stage6_dif = (double)( stage6_end - stage6_start ) ; 
		cout << endl << " Time spend of  OARK by teacher ="  << (double) (stage6_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  
		// cout << " obstacle_avoding_rectilinear_Kruskal_by_teacher.size = " << 	obstacle_avoding_rectilinear_Kruskal_by_teacher.size() << endl ;

		if( debug == true ){
			cout << " End of get in OARK to do rectilinear " << endl ; 
		}

		/*-----------------------------------------------------------------------------
		 *  				這裡要印出  我最後一步的輸出
		 *  		obstacle_avoding_rectilinear_Kruskal   , 因為我二塊都有做  
		 *  		所以  ， 只要輸出  那個要看的就可以了   , 沒有理由丫 ， 那個
		 *  		真的是對的  沒有錯  我只是換個方法而已  怎麼會圖就有 錯了呢 
		 *-----------------------------------------------------------------------------*/


		 // OARK.obstacle_avoding_rectilinear_Kruskal_gnu(  obstacle_avoding_rectilinear_Kruskal_by_teacher , every_level_info[0] );	
		  OARK.obstacle_avoding_rectilinear_Kruskal_gnu(  obstacle_avoding_rectilinear_Kruskal , every_level_info[0] );	
		
		/*-----------------------------------------------------------------------------
		 *  下面是要印出  ， 用我的方法跟 ， 別人的方法下  ， 總體所花的時間   , 
		 *  要抓出我的方法所花的時間  ， 就把 TwoD_dif 減去  ， 作者  run OASG 所花的時間
		 *  就可以了 
		 *-----------------------------------------------------------------------------*/
		// time ( &TwoD_end ) ; 
		TwoD_end = clock() ; 
		TwoD_dif = (double)( TwoD_end - all_start ) ; 
		// 這裡因為  ， 最後那個rectilinear 的階段 ， 所以  要分二個 分方去弄   , 我的就加上我的方法  ， 他的就弄他的
		cout << endl << "Complete routing time by my_OASG = " << ( (TwoD_dif - O_OASG_dif - stage6_dif )/ CLOCKS_PER_SEC ) * 1000000 << " us" << endl ;  
		//cout << "Complete routing time by Author_OASG = " << ( (TwoD_dif - my_OASG_dif - stage5_dif - difference ) / CLOCKS_PER_SEC )* 1000000 << " us " << endl ;  
		
		/*-----------------------------------------------------------------------------
		 *  
		int finish = 0 ; 
		cout << "Finish = " ; 	
		cin >> finish ; 
		if( finish == 1 ){
			cout << " All done " << endl ;
		}
		 *-----------------------------------------------------------------------------*/
		
		/*-----------------------------------------------------------------------------
		 *  
		 下面是 呼叫 gnupot 畫圖時 要給的參數
		 我在想 是不是中間要有二個空格 才不會互相影響
		 gnuplot PlaneData.gnu  Kruskal_Tree.gnu  Obstacle_Avoding_Kruskal_Tree.gnu  obstacle_avoding_rectilinear_Kruskal.gnu  OASG.gnu 
		 哎  我想到了  其實不用畫那麼多個  我只要畫最後一個出來看  不就不會出現那種
		 圖會互相影響的問題了嗎 

		 這邊 我要自已弄一個 overlap_obstacle_pin 的 Overlap_PlaneData  ,
		 我本來是想說  我只把 obstacle 跟 pin 切給 level_info 自已去抓 
		 現在想一想  我在給建 構子時， 自已 給就好了丫 
		 都忘了  我有 三個建構子 ， 另一個  五個參數的應該可以 

		level_info build_overlap_planedata( 100 , 100 ,50 ,50 , 0 ) ; 
		build_overlap_planedata.generate_overlap_obstacle_pin() ;
		build_overlap_planedata.print_Overlap_PlaneData_gnu() ; 

		cout << "OASG.size() =  " << OASG.size() << endl ;  
		cout << "OASG_O_2D.size() =  " << OASG_O_2D.size() << endl ;  

		//cout << " ThreeD_SG_edge.size() = " << ThreeD_SG_edge.size() ;  
		 *-----------------------------------------------------------------------------*/

		

		/*-----------------------------------------------------------------------------
		 *    
		cout << "Here check the difference of two OASG : " << endl ; 
		// 這裡呢 ， 要寫一個 check 一個 OASG 的edge 有那裡有差別 ， 大概呢 在pin 跟 obstacle  在 31 ,31 的情況下
		// edge 數 分別是 352 , 351 , 但因為這個 case 太大了 ， 所以 ， 無法用肉眼去 trace 所以要用 寫code 來看 
		if( OASG.size() > OASG_O_2D.size() ){
		 
			// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
			for( unsigned int i = 0 ; i < OASG.size() ; i++ ){
				bool already_have = false ; 
				for( unsigned int j = 0 ; j < OASG_O_2D.size() ; j++ ){
					if( OASG_O_2D[j].print_node1_x() == OASG[i].print_node1_x() &&
					    OASG_O_2D[j].print_node1_y() == OASG[i].print_node1_y() && 
					    OASG_O_2D[j].print_node2_x() == OASG[i].print_node2_x() && 
					    OASG_O_2D[j].print_node2_y() == OASG[i].print_node2_y() ){
						already_have = true ;  
					}
					else if( OASG_O_2D[j].print_node2_x() == OASG[i].print_node1_x() &&
						 OASG_O_2D[j].print_node2_y() == OASG[i].print_node1_y() && 
						 OASG_O_2D[j].print_node1_x() == OASG[i].print_node2_x() && 
						 OASG_O_2D[j].print_node1_y() == OASG[i].print_node2_y() ){
							already_have = true ;  
					}
				}// end of loop j 
				if( already_have == false  ){
					// 這塊要把東西給印出來  
					cout << "OASG is bigger :" << endl ; 
					cout << "node1 = (" <<  OASG[i].print_node1_x() << " , " << OASG[i].print_node1_y() << ") " << endl ; 
					cout << "node2 = (" <<  OASG[i].print_node2_x() << " , " << OASG[i].print_node2_y() << ") " << endl ; 
				}

			}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  
		}
		else if( OASG_O_2D.size() > OASG.size()  ){
		 
			// 最後還要做 ， 看  ， 我納入的 edge 是否已經在 OASG 裡面了  
			for( unsigned int i = 0 ; i < OASG_O_2D.size() ; i++ ){
				bool already_have = false ; 
				for( unsigned int j = 0 ; j < OASG.size() ; j++ ){
				  
					if( OASG_O_2D[i].print_node1_x() == OASG[j].print_node1_x() &&
					    OASG_O_2D[i].print_node1_y() == OASG[j].print_node1_y() && 
					    OASG_O_2D[i].print_node2_x() == OASG[j].print_node2_x() && 
					    OASG_O_2D[i].print_node2_y() == OASG[j].print_node2_y() ){
						already_have = true ;  
						break ; 
					}
					else if( OASG_O_2D[i].print_node2_x() == OASG[j].print_node1_x() &&
						 OASG_O_2D[i].print_node2_y() == OASG[j].print_node1_y() && 
						 OASG_O_2D[i].print_node1_x() == OASG[j].print_node2_x() && 
						 OASG_O_2D[i].print_node1_y() == OASG[j].print_node2_y() ){
							already_have = true ;  
							break ; 
					}
				}// end of loop j 
				if( already_have == false  ){
					// 這塊要把東西給印出來   , 這塊是要印 , OASG_O_2D
					cout << "OASG_O_2D is bigger :" << endl ; 
					cout << "node1 = (" <<  OASG_O_2D[i].print_node1_x() << " , " << OASG_O_2D[i].print_node1_y() << ") " << endl ; 
					cout << "node2 = (" <<  OASG_O_2D[i].print_node2_x() << " , " << OASG_O_2D[i].print_node2_y() << ") " << endl ; 
				}

			}// 要看看  是否有重複的 edge 在裡面 如果 沒有  就可以跟這個 vertex 連線了  
		}
		else if( OASG.size() == OASG_O_2D.size()){
			cout << " Here is info about two kind of OASG : " ; 
			cout << "Great !! " << endl ; 
		}

		// 另一種 case 那就是 我建的 OASG ， 裡面有 相同的 edge 存在 
		// 它媽的 ， 竟然是 ， 自已跟自已連的 edge 
		cout << "Check inside of OASG : " << endl ;
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
			}// end of loop j 
			if( already_have == true  ){
				cout << "Yes , There is same edge in my_OASG" << endl; 
			}

		}// end of loop i    

		// 奇怪了 ， 數目沒有錯 那為什麼畫出來  ， 卻有少呢  
		for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
			cout << "In level " << i << ", size of obstalce is " << every_level_info[i].complete_obstacle_information.size() << endl  ; 	 
		}
		 *-----------------------------------------------------------------------------*/

	}// 只有一層
	else if( Overlap_or_not == 0 && every_level_info.size() > 1 && check_plane_big_enough( every_level_info ) == true ){


		cout << "Not avaible Now " << endl ; 	

	 
	}// > 1 層 
	else if( Overlap_or_not == 1 && every_level_info.size() == 1 && check_plane_big_enough( every_level_info ) == true ){

		// == 1 是run overlap 的case   , my_overlap_OASG , 我已經寫好了
		// 接下來呢 ， 要改的應該就只有 ， 中間建 overlap_OASG 那裡要換掉而已
		
		/*-----------------------------------------------------------------------------
		 下面要畫 Plane_Data 的圖 , 我設定成  誰呼叫 就畫那一層  
		 但是  我還要把 PlaneData.gnu 改成  看是那一層的呼叫
		 不然  我多層有呼叫 也是只有一個 PlaneData.gnu 檔
		 先維持這樣好了  , 下面這個也只是 畫出 第一層的圖而已 
		 要把下面改成overlap  的一些屬性
		 喔 ， 我之道 為什麼要額外用一個 func 來畫gnu 了 ， 
		 因為我用不同的txt檔 ， 來存 點的位置  ， 所以 non_overalp 是存在 PlaneData.txt
		 Overlap 則是存在 Overlap_PlaneData.txt , 但現在想想  ， 我幹麻額外寫一個 ，
		 用原本的 不也可以嗎 , 不管了 ， 先來看看 ， 跑出來的圖對不對  ， 且是
		 在那裡的比對 ， 跟我原本的 non_overalp  有什麼不同  會讓它當掉 
		 *-----------------------------------------------------------------------------*/
		every_level_info[0].print_Overlap_PlaneData_gnu()  ;
		bool overlap_debug = false ; 

		cout << endl << " ---------------------------------Here begain Overlap Routing Progress :--------------------------"  << endl ; 
		/*-----------------------------------------------------------------------------
		 *          
		 *
		 *
		 *		
		 *				STEP  2   
		 *				OASG  
		 *
		 *
		 * 
		 *-----------------------------------------------------------------------------*/

		/*-----------------------------------------------------------------------------
		 *  這裡要把　，　array_vertex 內的東西　，　放到相對應的x_position內
		 *  , 還要把  各個位置的 , head and tail ,  的true false 弄上去
		 *  head_tail_state 指的是  一個x 位置上  最高跟最低點 是否有 有vertex 在上面   
		 *  下面這個 vector<edge> OASG ， 能不能 放到 my_non_overlap_OASG 裡面當data 用
		 *  這樣我處理各層的OASG 只要把 那一層的資訊 傳進去  就可以了哎  
		 *  不是 更簡結 方便嗎
		 *  這裡 有弄出二個物件 ， 分別是  my_non_overlap_OASG , O_2D_OASG , 這二個都是用來
		 *  產生 OASG 的 只不過 方法不同 ， 前者是用我想的 ， 後者是 原作者的
		 *-----------------------------------------------------------------------------*/
		vector<edge> OASG ; 
		my_overlap_OASG my_OASG ; 	

		/*-----------------------------------------------------------------------------
		 我把下面的func 傳弟  ， 原本是打  &sort_y_position
		 但 我按rr 去作執行檔 有錯  ， 我現在之道那是什麼意思了  
		 應該是指  我之前compile過 應該有個相同時間的 執行檔
		 但是確沒有  只有一個時間點 就的執行檔  , 所以不給執行
		 我下面改成 不放&& 看能不能執行
		 file '/Users/nickboy/Program/Nov 29/generate_input' does not exist or is not executable or object/source older then executable 
		 看來 obstacle_plane 可以不用存丫  ， 下面都用不到
		 *-----------------------------------------------------------------------------*/
		 if( overlap_debug == true  ){
		 	cout << "Start to build overlap_OASG "  << endl ; 
		 }
		clock_t start , end ; 
		double my_OASG_dif   ; 

		// cout << "Line 1430 , before into My_Overlap_OASG " << endl ; 
		start = clock() ; 
		// cout << endl << "Time of My_OASG start = " << start << endl ; 
		my_OASG.build_OASG( OASG , every_level_info[0] ) ;
		end = clock() ; 
		// cout << "Time of My_OASG end = " << end << endl ; 
		// 由下面可以發現 ， difftime 這個 func 第一個參數 是傳入 ， 較晚的時間  
		// 下面輸出us 的地方 ， 有點錯 ， 因為除以 CLOCKS_PER_SEC , 後 出來的東西是 ， secs , 所以還要乘上10的6次方
		my_OASG_dif = (double) ( end - start ) ; 
		// my_OASG_dif = (double)difftime ( end , start ) ; 
		cout << "Time of My_Overlap_OASG spend = "  << (double) ( my_OASG_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ; 
		// 接下來 ， 我要印出 此時的 edge 總數 ， 如果都一樣 那就成功了  我覺得  
		cout << "Edges of My_Overlap_OASG = " << OASG.size() << endl ; 

		// cout << "Line 1443 , finish my_overlap_OASG " << endl ; 

		/*-----------------------------------------------------------------------------
		  我要先把各個x位置的edge 畫上去　，　看有沒有畫錯　，　接下來才用
		  我想的bouding box interval  , ok 了  , 先試看看  丟一個去算 能不能跑出來
		  OASG.gnu
		  告夭 ， 我差點忘了 ， 這個是 畫出  ， ps檔 ， 所以不會直接把圖弄出來  
		  還是說 ， 我都弄成 ， 直接用terminal show出來  , 但這樣 ， 兩張圖如果 
		  有差異 ， 我很難比出來   
		 *-----------------------------------------------------------------------------*/
		my_OASG.print_OASG_gnu(  OASG , every_level_info[0] ) ;	
		 if( overlap_debug == true  ){
		 	cout << "End build overlap_OASG "  << endl ; 
		 }
		
		/*-----------------------------------------------------------------------------
		 *                               STEP 3    
		 *                  Kruskal tree , OAST  ,  rectilinear 
		 *                                 
		 *                   		Kruskal tree    
		 *-----------------------------------------------------------------------------*/

		 if( overlap_debug == true  ){
		 	cout << "Start to build Kruskal "   ; 
		 }
		// time_t stage3_start , stage3_end ; 
		clock_t stage3_start , stage3_end ;
		double stage3_dif  ; 

		vector<edge> SG_edge ; 
		kruskal_tree Kruskal_Tree ;  
		// 下面二個是弄2D 的 , 這一塊呢 ， SG_edge ， 才是 我用來放 Kruskal_Tree 的真的vector 
		// 下面  因為  我有分群  ， 所以  ， 我要先之道 ， 分了幾群  ，  原來 my_level_info  
		// 是傳到別的地方用的名字 
		vector<int> how_much_group  ; 
		how_much_group.push_back( every_level_info[0].only_pin[0].which_group ) ; 
		for( unsigned int i = 1  ; i < every_level_info[0].only_pin.size() ; i++ ){
			// 這裡面  就很好寫拉 ， 只要 跟 how_much_group , 內 的每個值相比   ， 
			// 有相同  就出去  ， 沒有 就＋＋ 這樣  ， 就可以之道 ， 總共有幾個
			// 群 
			bool match_or_not = false ; 
			for(unsigned int j = 0 ; j < how_much_group.size() ; j++ ){

				if( how_much_group[j] ==  every_level_info[0].only_pin[i].which_group ){
					match_or_not = true ; 
					break ; 
				}

			}// loop j 

			if( match_or_not == false ){
				how_much_group.push_back( every_level_info[0].only_pin[i].which_group ) ; 
			}
		}// scan all only_pin 

		// 接下來  ， 要把各個pin 放到 相對應的vector 內  ， 等下好方便 ， 各個去做他們的Kruskal tree 
		vector<vertex> groups[ how_much_group.size() ] ; 
		cout << " Total groups  = " << how_much_group.size() << endl ; 
		for( unsigned int i = 0  ; i < every_level_info[0].only_pin.size() ; i++ ){
		
			// 跟 how_much_group 比  ， 看是要那一個  
			for(unsigned int j = 0 ; j < how_much_group.size() ; j++ ){

				if( how_much_group[j] ==  every_level_info[0].only_pin[i].which_group ){
					groups[j].push_back( every_level_info[0].only_pin[i] ) ; 	
					break ; 
				}

			}// loop j 
			
		}// scan all only_pin 
		
		// time( &stage3_start ) ; 
		stage3_start = clock() ; 
		vector<edge> temp ; 
		for( unsigned int i = 0 ; i < how_much_group.size() ; i++ ){
			 
			 if( groups[i].size() > 1  ){
			  
				// 哇 ， 下面建 Kruskal_Tree 的 func  , 看來是要改一下了 
				Kruskal_Tree.build_2D_overlap_kruskal_tree(  temp , every_level_info[0] , groups[i] ) ; 

				// 接下來 ， 要把 temp內的edge 給班過去  ， SG_edge 內   
				for( unsigned int j = 0 ; j < temp.size() ; j++ ){
					SG_edge.push_back( temp[j]) ; 	 
				}// scan all temp elements 
				temp.clear() ; 
			 }//  這個forest 的size > 1 才有必要去做Kruskal 

		}// loop i  

		// time( &stage3_end ) ; 
		stage3_end = clock() ; 

		Kruskal_Tree.print_2D_overlap_kruskal_tree_gun(  SG_edge , every_level_info[0] );
		stage3_dif = (double) ( stage3_end - stage3_start ) ; 
		// 看來預設是 10 的6次方 ， 也就是us 
		// cout << "CLOCKS_PER_SEC " << CLOCKS_PER_SEC << endl ;  
		cout << endl << " Time spend of Kruskal tree = " << (double) (stage3_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  

		 if( overlap_debug == true  ){
		 	cout << "End build Kruskal "  << endl ; 
		 }


		
		 /*-----------------------------------------------------------------------------
		  * 				Step 4 
		  * 				OAST 
		  *-----------------------------------------------------------------------------*/
		 if( overlap_debug == true  ){
		 	cout << "Start to build OAST "  ; 
		 }
		clock_t stage4_start , stage4_end ; 
		double stage4_dif  ; 

		vector<edge> obstacle_avoding_Kruskal ;
		// OAST 是處理的物件
		obstacle_avoding_kruskal OAST ; 
		stage4_start = clock() ; 
		// time( &stage4_start ) ; 
		//  奇怪了  ， 我的 OASG 那裡有改東西  ， 變成錯的了   , 要再改一下  ， 所以  ， 先用 O_2D_OASG 的edge 來用 
		// OAST.build_obstacle_avoding_kruskal( SG_edge  , obstacle_avoding_Kruskal ,  OASG ,  every_level_info[0]);	
		OAST.build_overlap_obstacle_avoding_kruskal_imp( SG_edge  , obstacle_avoding_Kruskal ,  OASG ,  every_level_info[0]);	
		// time( &stage4_end  ) ; 
		stage4_end = clock() ; 
		stage4_dif = (double)( stage4_end - stage4_start ) ; 
		cout << endl << " Time spend of OAST = "  << (double) (stage4_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  

		/*-----------------------------------------------------------------------------
		 *  這裡要先　印出　，　obstacle_avoding_Kruskal 來看看 
		 *  我的想法是缺了　那些edge , 以至於　會找不到path
		 *-----------------------------------------------------------------------------*/
		OAST.print_overlap_obstacle_avoding_kruskal_gun(  obstacle_avoding_Kruskal , every_level_info[0] ) ;	

		 if( overlap_debug == true  ){
		 	cout << "End  build OAST "  << endl ; 
		 }

		
		/*-----------------------------------------------------------------------------
		 *   				Rectilinear
		 *   		這是我在2D下的最後一步了 。 
		 *   			a. 首先要分群 ，每個edge 以一個rect 來看，
		 *   			   相互有overlap , 設成同一群 ， 但不能單
		 *   			   獨 只看x or y 來看 ， 前提要先相鄰 ， 也就
		 *   			   是說 ， 新納入的edge  ,其兩端點 ，要跟某一群
		 *   			   的點集有關係 才行 ， 
		 *
		 *   		我想在外面 把 obstacle_avoding_Kruskal 內的edge 先依 level 分類 
		 *   		再丟到 rectilinear.h 內去處理  ， 然而 存那個處理後的edge 
		 *   		是用 obstacle_avoding_rectilinear_Kruskal 
		 *   			   
		 *-----------------------------------------------------------------------------*/

		 if( overlap_debug == true  ){
		 	cout << "Start to build rectilinear "   ; 
		 }
		clock_t stage5_start , stage5_end ; 
		clock_t stage6_start , stage6_end ; 
		double stage5_dif  ; 
		double stage6_dif  ; 

		if( debug == true ){
			cout << " Before get in OARK to do rectilinear " << endl ; 
		}

		rectilinear OARK ; 	

		vector<edge> obstacle_avoding_rectilinear_Kruskal ; 
		stage5_start = clock() ; 
		// time( &stage5_start ) ; 
		OARK.change_to_rectilinear_by_author(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal );	
		// OARK.change_to_rectilinear(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal );	
		// time( &stage5_end ) ; 
		stage5_end = clock() ; 
		stage5_dif = (double)( stage5_end - stage5_start ) ; 
		cout << endl << " Time spend of OARK by author ="  << (double) (stage5_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  


		vector<edge> obstacle_avoding_rectilinear_Kruskal_by_teacher ; 
		stage6_start = clock() ; 
		// time( &stage6_start ) ; 
		OARK.change_to_rectilinear_by_teacher(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal_by_teacher );	
		// OARK.change_to_rectilinear(  obstacle_avoding_Kruskal , obstacle_avoding_rectilinear_Kruskal );	
		// time( &stage6_end ) ; 
		stage6_end = clock() ; 
		stage6_dif = (double)( stage6_end - stage6_start ) ; 
		cout << endl << " Time spend of  OARK by teacher ="  << (double) (stage6_dif / CLOCKS_PER_SEC ) * 1000000 << " us " << endl ;  
		// cout << " obstacle_avoding_rectilinear_Kruskal_by_teacher.size = " << 	obstacle_avoding_rectilinear_Kruskal_by_teacher.size() << endl ;

		if( overlap_debug == true ){
			cout << " End of get in OARK to do rectilinear " << endl ; 
		}

		/*-----------------------------------------------------------------------------
		 *  				這裡要印出  我最後一步的輸出
		 *  		obstacle_avoding_rectilinear_Kruskal   , 因為我二塊都有做  
		 *  		所以  ， 只要輸出  那個要看的就可以了   , 沒有理由丫 ， 那個
		 *  		真的是對的  沒有錯  我只是換個方法而已  怎麼會圖就有 錯了呢 
		 *-----------------------------------------------------------------------------*/


		 // OARK.obstacle_avoding_rectilinear_Kruskal_gnu(  obstacle_avoding_rectilinear_Kruskal_by_teacher , every_level_info[0] );	
		  OARK.overlap_obstacle_avoding_rectilinear_Kruskal_gnu(  obstacle_avoding_rectilinear_Kruskal , every_level_info[0] );	
		  OARK.overlap_obstacle_avoding_rectilinear_Kruskal_gnu_trans_eps(  obstacle_avoding_rectilinear_Kruskal , every_level_info[0] );	
		
		/*-----------------------------------------------------------------------------
		 *  下面是要印出  ， 用我的方法跟 ， 別人的方法下  ， 總體所花的時間   , 
		 *  要抓出我的方法所花的時間  ， 就把 TwoD_dif 減去  ， 作者  run OASG 所花的時間
		 *  就可以了 
		 *-----------------------------------------------------------------------------*/
		// time ( &TwoD_end ) ; 
		TwoD_end = clock() ; 
		TwoD_dif = (double)( TwoD_end - all_start ) ; 
		// 這裡因為  ， 最後那個rectilinear 的階段 ， 所以  要分二個 分方去弄   , 我的就加上我的方法  ， 他的就弄他的
		cout << endl << "Complete routing time by my_OASG = " << ( (TwoD_dif  - stage6_dif )/ CLOCKS_PER_SEC ) * 1000000 << " us" << endl ;  



		/*-----------------------------------------------------------------------------
		 *  
		int finish = 0 ; 
		cout << "Finish = " ; 	
		cin >> finish ; 
		if( finish == 1 ){
			cout << " All done " << endl ;
		}
		 *-----------------------------------------------------------------------------*/
		
		/*-----------------------------------------------------------------------------
		 *  
		 下面是 呼叫 gnupot 畫圖時 要給的參數
		 我在想 是不是中間要有二個空格 才不會互相影響
		 gnuplot PlaneData.gnu  Kruskal_Tree.gnu  Obstacle_Avoding_Kruskal_Tree.gnu  obstacle_avoding_rectilinear_Kruskal.gnu  OASG.gnu 
		 哎  我想到了  其實不用畫那麼多個  我只要畫最後一個出來看  不就不會出現那種
		 圖會互相影響的問題了嗎  , 沒錯  

		 這邊 我要自已弄一個 overlap_obstacle_pin 的 Overlap_PlaneData  ,
		 我本來是想說  我只把 obstacle 跟 pin 切給 level_info 自已去抓 
		 現在想一想  我在給建 構子時， 自已 給就好了丫 
		 都忘了  我有 三個建構子 ， 另一個  五個參數的應該可以 

		level_info build_overlap_planedata( 100 , 100 ,50 ,50 , 0 ) ; 
		build_overlap_planedata.generate_overlap_obstacle_pin() ;
		build_overlap_planedata.print_Overlap_PlaneData_gnu() ; 

		cout << "OASG.size() =  " << OASG.size() << endl ;  
		cout << "OASG_O_2D.size() =  " << OASG_O_2D.size() << endl ;  

		//cout << " ThreeD_SG_edge.size() = " << ThreeD_SG_edge.size() ;  
		 *-----------------------------------------------------------------------------*/

		


	}// end of 2D overlap 
	else if( Overlap_or_not == 1 && every_level_info.size() > 1 && check_plane_big_enough( every_level_info ) == true ){


		cout << "Not avaible Now " << endl ; 	


	}
	else{
		cout << "We need a big Plane !!!! " << endl ; 
	}

	
	/*-----------------------------------------------------------------------------
	 * 				check code  
	for( unsigned int i = 0 ; i < every_level_info.size() ; i++ ){
		cout << "level " << i << " : " << endl  ; 
		cout << "my_level_OASG.size = " << every_level_info[i].my_level_OASG.size() << "   " ;	
		cout << "author_level_OASG.size = " << every_level_info[i].author_level_OASG.size() << endl  ;	
	}// check 二種方法出來的 edge 數目是否相同 

	for(unsigned int i = 0 ; i < every_level_info.size() ; i++  ){
		cout << endl << " every_level_info.which_level = " << every_level_info[i].which_level << endl ; 
		for(unsigned int j = 0  ; j < every_level_info[i].author_level_OASG.size() ; j++ ){
			int which_level = i ; 
			if( every_level_info[i].author_level_OASG[j].print_node1_z() != which_level  ){
				cout << endl << "This is what i talking about " << endl ;  
			}
		}
	}// check 是否有建edge 到別層 
	 *-----------------------------------------------------------------------------*/

	return 0 ;
	
}				/* ----------  end of function main  ---------- */
