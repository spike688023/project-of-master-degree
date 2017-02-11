/*
 * =====================================================================================
 *
 *       Filename:  level_info.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/02/13 14時52分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jack_Lin (), jack_6880@hotmail.com
 *        Company:  NTUST
 *
 * =====================================================================================
 */

#ifndef       LEVEL_INFO_H 
	#define LEVEL_INFO_H  

#include	<iostream>
#include	<vector>
#include        <cstdio>
#include 	<ctime> 
#include        <string>
#include        <sstream>
#include	<map>
#include 	"vertex.h"	
#include 	"obstacle.h"	

/*-----------------------------------------------------------------------------
 *  
#ifndef         EDGE_H
	#define 	EDGE_H
#endif 
 *-----------------------------------------------------------------------------*/

using namespace std ; 

class level_info{

	//private:

	public:
	/*-----------------------------------------------------------------------------
	 *  
	下面這二個  我想不用特別存 
	static int  plane_width  ;
	static int plane_height  ;
	 *-----------------------------------------------------------------------------*/
	// static int test ;  可以不用先給值哎 
	// 為什麼下面這二個 都會被說是 non define symbol 
	// 跟我把它設成是 static 有關嗎  ， 因為其它 
	// 的變數 都不會有這個問題丫 
	// 拿掉來看看好了  , 果然是哎 
	// 那也就是說 static 變數  我一關始給 就會把值給它了
	int plane_width ; 
	int plane_height ; 
	// 下面這二個變數 ， 在後面的stage 好像都用不到了 
	// 等整個都修改完  再看要不要換 
	// 要解是一下 下面二個plane 的差別 ， pin_plane 是要建pin時用的
	// 因為pin可以放在obstacle的外圍 ， 而obstacle 則是把外圍都給弄成true
	//  only_vertex 是只有存  ， pin 及  obstacle_corner 
	bool **obstacle_plane ; 
	bool **pin_plane ; 
	bool **only_vertex ; 
	// 多了下面這個 vertex_id  , 是打算 以最快的速度  ， 查到 vertex 的id , 好快 去
	// table 中  ， 找到 它的位置  
	// 下面第二個屬性  ， obstacle_id , 是用來 ， 存 擋在這個位置的 obstacle_id 是誰
	// 因為是 non_overlap , 所以  ， 一個位置只會有一個數值  ， 不像overlap ， 
	// 而  ， 沒有被擋到的 ， 就給他 －1 吧  
	int **vertex_id; 
	int **obstacle_id ; 
	int **connected_graph ; 
	int **which_forest ; 
	// 下面這個 資料  是用來存  obstacle 的left bound ， 但只有存他中間那塊
	//  ， 最上和下的則沒有   , 因為是用來判斷 ， 這個點 還能否 往後 連線 
	bool **only_left_bound; 
	// 要多建一張表了  ， 用來看 ， 那些點 已經放進去  path了 
	int **degree_per_vertex ; 
	int pin_nu  ; 
	int obstacle_nu  ; 
	int which_level  ; 
	// plane_level = 3 , 代表 0 ~ 2 
	int plane_level ; 
	int total ; 
	float percent_used ; 

	/*-----------------------------------------------------------------------------
	 一個 * 如同一個 [] , 二者是相同意思的 如土哥講的
	 但前者的較有彈性 ， 不用在使用前定太多規矩 ， 如
	 你要用多少空間
	 哎丫 ， 下面這個放 vertex 的 我幹麻不用 vector  , 搞我的還要用
	 total 去記裡面的size 
	 *-----------------------------------------------------------------------------*/
	vertex *array_vertex ;
	vector<obstacle> complete_obstacle_information ; 
	// 這邊我想都設一個東西出來 , 是用來存 projection 而生在各層的點
	vector<vertex> projection_vertex ; 	
	// 下面這個 是含有 projection 出來的點 , 還有原先 array_vertex 內的點  , 
	// projection_vertex 的屬性是 false  , corner_id = 0 
	// 那應該就是 在project class 裡做的
	vector<vertex> array_vertex_vector ; 
	vector<vertex> only_pin ; 
	// 我要用個新vector 來存各層的 OASG  ,  我想呢 ， 這個用來放我method 的 OASG 
	vector<edge> my_level_OASG ; 
	// 這邊要另外寫一個 , 也是用來存 OASG 的 ， 但是 ， 是存作者方法的 OASG 
	vector<edge> author_level_OASG ; 
	vector<vertex> overlap_vertex ; 
	// 我要把一些map  丫  ， obstacle 的相關資訊 ， 放到下面去 
	// *obstacle_body_cross  = obstacle_body_cross[] , 這個的話  是map
	// 用id  來抓到  obstacle 
	map<int,obstacle> obstacle_map ; 
	// 下面二個 是overlap 要用的  , 下面二個 存的是 obstacle 的id  
	// 但是  是分開存的  也就是  分開成存  body , 以及 左右二個bound 
	vector<int>  *obstacle_body_cross ;
	vector<int>  *obstacle_bound_cross ;
	vector<int>  *obstacle_cross ;
	// 下面要塞個  ，  array_vertex_on_position[ plane_width ] ,   這樣 才不用比對所有的vertex 
	// 下面的內容  ， 會在  Projection_handler 內實作   
	vector<vertex>  *array_vertex_on_position ;


	level_info(){
		obstacle_plane = 0 ; 
		pin_plane = 0 ; 
		vertex_id = 0 ; 
		only_vertex = 0 ; 
		degree_per_vertex = 0 ; 
		connected_graph = 0  ; 
		only_left_bound = 0 ;
		which_forest = 0  ; 
		obstacle_id = 0 ; 
		plane_width = 0 ; 
		plane_height = 0 ; 
		plane_level = 0 ; 
		pin_nu = 0 ; 
		obstacle_nu = 0 ; 
		which_level = -1 ;
		total = 0  ; 
		obstacle_body_cross  = new vector<int> [plane_width] ;
		obstacle_bound_cross = new vector<int> [plane_width] ;
		obstacle_cross = new vector<int> [plane_width] ;
		array_vertex_on_position = new vector<vertex> [plane_width] ; 
	}

	level_info( const int planewidth , const int planeheight , const int pinnu , const int obstaclenu , const int whichlevel ){
		plane_width = planewidth ; 	
		plane_height = planeheight ; 
		pin_nu = pinnu ; 
		obstacle_nu = obstaclenu ; 
		which_level = whichlevel ;
		total = pin_nu + 4 * obstacle_nu ; 
		array_vertex = new vertex [ pin_nu + 4 * obstacle_nu ] ;
		obstacle_body_cross  = new vector<int> [plane_width] ;
		obstacle_bound_cross = new vector<int> [plane_width] ;
		obstacle_cross = new vector<int> [plane_width] ;
		array_vertex_on_position = new vector<vertex> [plane_width] ; 

		/*-----------------------------------------------------------------------------
		array_vertex[ pin_nu + 4 * obstacle_nu ] ;
		 要設二塊版子　，　因為pin 跟obstacle 間的要求不一樣 
		 土哥說　，　因為os 會有限制　local mem  的大小　
		 所以要用new 去動態抓　才不會有這種問題　
		 但動態的要怎麼宣告　
		bool obstacle_plane[plane_width][plane_height]  ;  
		bool pin_plane[plane_width][plane_height]  ;  
		 讓我解是 一下  下面這個 宣告 ， 首先呢 obstacle_plane 是個指向
		 二維bool 的 指標 ，  可以這樣看  **obstacle_plane = *[plane_width]
		 這樣對著看  似乎就清楚了一點 ， 第一個* 對 到第一個點 ， 
		 *obstacle_plane 對到  [plane_width]  也就是說  obstacle_plane 第一次當指標
		 是指向 一個 一維陣列的指標 ， 我要去查一下  是回傳什麼東西
		   test[][] 等同於  **test ， test 就是一個二維指標 如此而已
		   可能是這裡的錯喔  我上面已經宣告了   ， 所以這裡
		   只要給位置就好了  應該不用宣告 
		 *-----------------------------------------------------------------------------*/
		obstacle_plane = new bool *[plane_width] ;
		pin_plane = new bool *[plane_width] ;
		vertex_id = new int *[plane_width] ; 
		only_vertex = new bool *[plane_width] ; 
		only_left_bound = new bool *[plane_width] ; 
		obstacle_id = new int *[plane_width] ; 
		degree_per_vertex = new int *[plane_width] ; 
		connected_graph = new int  *[plane_width] ;
		which_forest = new int  *[plane_width] ;

		// 下面兩個fox 是用來 初始上面的二個 bool table 
		for( int i = 0 ; i < plane_width ; i++ ){
			obstacle_plane[i] = new bool[plane_height];
			pin_plane[i] = new bool[plane_height];
			vertex_id[i] = new int [plane_height] ;
			only_vertex[i] = new bool [plane_height] ;
			only_left_bound[i] = new bool [plane_height] ;
			degree_per_vertex[i] = new int [plane_height] ;
			obstacle_id[i] = new int [plane_height] ;
			connected_graph[i] = new int [plane_height] ;
			which_forest[i] = new int [plane_height] ;
		}
		/*-----------------------------------------------------------------------------
		 *  initialization of plane , false 代表上面都沒有東西　，　沒有pin or obstacle
		 *-----------------------------------------------------------------------------*/
		for(int i = 0  ;  i < plane_width ;  i++   ){
			for(int j = 0  ;  j < plane_height ;  j++   ){
			obstacle_plane[i][j] = false ; 			
			pin_plane[i][j] = false ; 			
			vertex_id[i][j] = -1 ; 
			only_vertex[i][j] = false  ;
			only_left_bound[i][j] = false  ;
			degree_per_vertex[i][j] = 0 ;
			obstacle_id[i][j] = -1 ; 
			connected_graph[i][j] = -1 ; 
			which_forest[i][j] = -1 ; 
			} 
		}

	}// end of second constructor 

	level_info( const int planewidth , const int planeheight , const int whichlevel , const int planelevel ){

		plane_width = planewidth ; 	
		plane_height = planeheight ; 
		plane_level= planelevel ; 
		which_level = whichlevel ;
		obstacle_body_cross  = new vector<int> [plane_width] ;
		obstacle_bound_cross = new vector<int> [plane_width] ;
		obstacle_cross = new vector<int> [plane_width] ;
		array_vertex_on_position = new vector<vertex> [plane_width] ; 

		cout << "In level  " << whichlevel << " : "  << endl ; 		 
		cout << "How many obstacle you want : " ;
		cin >> obstacle_nu ; 	
		cout << "How many pins you want : " ;
		cin >> pin_nu ; 
		// pin_nu = pinnu ; 
		// obstacle_nu = obstaclenu ; 
		 total = pin_nu + 4 * obstacle_nu ; 
		 array_vertex = new vertex [ pin_nu + 4 * obstacle_nu ] ;

		 /*-----------------------------------------------------------------------------
		 array_vertex[ pin_nu + 4 * obstacle_nu ] ;
		 要設二塊版子　，　因為pin 跟obstacle 間的要求不一樣 
		 土哥說　，　因為os 會有限制　local mem  的大小　
		 所以要用new 去動態抓　才不會有這種問題　
		 但動態的要怎麼宣告　
		bool obstacle_plane[plane_width][plane_height]  ;  
		bool pin_plane[plane_width][plane_height]  ;  
		 讓我解是 一下  下面這個 宣告 ， 首先呢 obstacle_plane 是個指向
		 二維bool 的 指標 ，  可以這樣看  **obstacle_plane = *[plane_width]
		 這樣對著看  似乎就清楚了一點 ， 第一個* 對 到第一個點 ， 
		 *obstacle_plane 對到  [plane_width]  也就是說  obstacle_plane 第一次當指標
		 是指向 一個 一維陣列的指標 ， 我要去查一下  是回傳什麼東西
		   test[][] 等同於  **test ， test 就是一個二維指標 如此而已
		   可能是這裡的錯喔  我上面已經宣告了   ， 所以這裡
		   只要給位置就好了  應該不用宣告 
		  *-----------------------------------------------------------------------------*/
		obstacle_plane = new bool *[plane_width] ;
		pin_plane = new bool *[plane_width] ;
		vertex_id = new int *[plane_width] ; 
		only_vertex = new bool *[plane_width] ; 
		only_left_bound= new bool *[plane_width] ; 
		degree_per_vertex = new int *[plane_width] ; 
		obstacle_id = new int *[plane_width] ; 
		connected_graph = new int *[plane_width] ; 
		which_forest = new int *[plane_width] ; 

		// 下面兩個fox 是用來 初始上面的二個 bool table 
		for( int i = 0 ; i < plane_width ; i++ ){
			obstacle_plane[i] = new bool[plane_height];
			pin_plane[i] = new bool[plane_height];
			vertex_id[i] = new int [plane_height] ;
			only_vertex[i] = new bool [plane_height] ;
			only_left_bound[i] = new bool [plane_height] ;
			degree_per_vertex[i] = new int [plane_height] ;
			obstacle_id[i] = new int [plane_height] ;
			connected_graph[i] = new int [plane_height] ;
			which_forest[i] = new int [plane_height] ;
		}
		/*-----------------------------------------------------------------------------
		 *  initialization of plane , false 代表上面都沒有東西　，　沒有pin or obstacle
		 *-----------------------------------------------------------------------------*/
		for(int i = 0  ;  i < plane_width ;  i++   ){
			for(int j = 0  ;  j < plane_height ;  j++   ){
			obstacle_plane[i][j] = false ; 			
			pin_plane[i][j] = false ; 			
			vertex_id[i][j] = -1 ; 
			only_vertex[i][j] = false  ; 
			only_left_bound[i][j] = false  ; 
			degree_per_vertex[i][j] = 0; 
			obstacle_id[i][j] = -1 ; 
			connected_graph[i][j] = -1 ; 
			which_forest[i][j] = -1 ; 
			} 
		}

	}// end of third constructor 

	level_info( const int planewidth , const int planeheight , const int whichlevel , const int planelevel , const int obs_nu , const int p_nu ){

		plane_width = planewidth ; 	
		plane_height = planeheight ; 
		plane_level= planelevel ; 
		which_level = whichlevel ;
		obstacle_body_cross  = new vector<int> [plane_width] ;
		obstacle_bound_cross = new vector<int> [plane_width] ;
		obstacle_cross = new vector<int> [plane_width] ;
		array_vertex_on_position = new vector<vertex> [plane_width] ; 
		obstacle_nu = obs_nu ; 
		pin_nu = p_nu ; 

		// pin_nu = pinnu ; 
		// obstacle_nu = obstaclenu ; 
		 total = pin_nu + 4 * obstacle_nu ; 
		 array_vertex = new vertex [ pin_nu + 4 * obstacle_nu ] ;

		 /*-----------------------------------------------------------------------------
		 array_vertex[ pin_nu + 4 * obstacle_nu ] ;
		 要設二塊版子　，　因為pin 跟obstacle 間的要求不一樣 
		 土哥說　，　因為os 會有限制　local mem  的大小　
		 所以要用new 去動態抓　才不會有這種問題　
		 但動態的要怎麼宣告　
		bool obstacle_plane[plane_width][plane_height]  ;  
		bool pin_plane[plane_width][plane_height]  ;  
		 讓我解是 一下  下面這個 宣告 ， 首先呢 obstacle_plane 是個指向
		 二維bool 的 指標 ，  可以這樣看  **obstacle_plane = *[plane_width]
		 這樣對著看  似乎就清楚了一點 ， 第一個* 對 到第一個點 ， 
		 *obstacle_plane 對到  [plane_width]  也就是說  obstacle_plane 第一次當指標
		 是指向 一個 一維陣列的指標 ， 我要去查一下  是回傳什麼東西
		   test[][] 等同於  **test ， test 就是一個二維指標 如此而已
		   可能是這裡的錯喔  我上面已經宣告了   ， 所以這裡
		   只要給位置就好了  應該不用宣告 
		  *-----------------------------------------------------------------------------*/
		obstacle_plane = new bool *[plane_width] ;
		pin_plane = new bool *[plane_width] ;
		vertex_id = new int *[plane_width] ; 
		only_vertex = new bool *[plane_width] ; 
		only_left_bound= new bool *[plane_width] ; 
		degree_per_vertex= new int *[plane_width] ; 
		obstacle_id = new int *[plane_width] ; 
		connected_graph = new int *[plane_width] ; 
		which_forest= new int *[plane_width] ; 

		// 下面兩個fox 是用來 初始上面的二個 bool table 
		for( int i = 0 ; i < plane_width ; i++ ){
			obstacle_plane[i] = new bool[plane_height];
			pin_plane[i] = new bool[plane_height];
			vertex_id[i] = new int [plane_height] ;
			only_vertex[i] = new bool [plane_height] ;
			only_left_bound[i] = new bool [plane_height] ;
			degree_per_vertex[i] = new int [plane_height] ;
			obstacle_id[i] = new int [plane_height] ;
			connected_graph[i] = new int [plane_height] ;
			which_forest[i] = new int [plane_height] ;
		}
		/*-----------------------------------------------------------------------------
		 *  initialization of plane , false 代表上面都沒有東西　，　沒有pin or obstacle
		 *-----------------------------------------------------------------------------*/
		for(int i = 0  ;  i < plane_width ;  i++   ){
			for(int j = 0  ;  j < plane_height ;  j++   ){
			obstacle_plane[i][j] = false ; 			
			pin_plane[i][j] = false ; 			
			vertex_id[i][j] = -1 ; 
			only_vertex[i][j] = false  ;
			only_left_bound[i][j] = false  ;
			degree_per_vertex[i][j] = 0 ;
			obstacle_id[i][j] = -1 ; 
			connected_graph[i][j] = -1 ; 
			which_forest[i][j] = -1 ; 
			} 
		}

	}// end of third constructor 

	// 有用到上面的 建構子  應該就用不到 應該就用不到set 這個函數了吧
	// 那我就只  多寫三個  print func 好了 

	int print_plane_width(){
		return plane_width ; 	
	}

	int print_plane_height(){
		return plane_height ; 	
	}

	int print_plane_level(){
		return plane_level ; 	
	}

	int print_total(){
		return total ; 
	}

	// 下面這個func 只要回傳 array_vertex 是因為
	// 它本來就是宣告成 指標了 
	vertex* print_array_vertex(){
		return array_vertex ; 
	}

	// 這個的話  因為不是指標所以要用個 & 去回傳位置來用
	vector<obstacle>* print_complete_obstacle_info(){
		return &complete_obstacle_information ; 
	}	
	
	int print_pin_nu(){
		return pin_nu ; 
	}

	int print_obstacle_nu(){
		return obstacle_nu ; 
	}

	int print_which_level(){
		return which_level ; 
	}
	
	// 下面這個 func 我現在  在這些封裝的data 裡面 ， 
	// 還有需要 傳那麼多的 data 過去嗎  , 應該是不用了 
	void generate_obstacle_pin (  ) ; 

	void print_PlaneData_gnu (  )  ;

	// 下面這個 func 是要用來產 overlap_obsatcle 的
	// 我只要從上面的func 當範本來改  應該就可以了 
	void generate_overlap_obstacle_pin (  ) ; 

	void print_Overlap_PlaneData_gnu (  )  ;
};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Is_vertical_or_horizontal_line
 *  Description:  因為這裡是要做obstacle  , 所以要排除成水平或垂直的line  
 * =====================================================================================
 */
bool	
Is_vertical_or_horizontal_line ( int xx_1 , int yy_1 , int xx_2 , int yy_2 )
{
	if( xx_1 == xx_2 || yy_1 == yy_2  ){
		return true ;			
	}
	else {
		return false ; 
	}
}		/* -----  end of function Is_vertical_or_horizontal_line  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Is_area_large
 *  Description: 判斷這個obstacle 的面積會否太大　，　免的塞不下　，　我要的obstacle數目  
 * =====================================================================================
 */

bool	
Is_area_large ( int obs_nu , int pl_wd , int pl_he , int xx_1 , int yy_1 , int xx_2 , int yy_2 ) 
{
	int properly_area = pl_wd * pl_he / obs_nu ; 
	int actual_area = abs(xx_1 - xx_2) * abs(yy_1 - yy_2) ;
	if( actual_area > properly_area ){
		return true ; 
	}
	else {
		return false ; 	
	}
}		/* -----  end of function Is_area_large  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  change_to_left_bottom_right_up
 *  Description: 要把node 1 跟　2 變成左下　右上　，　好方便　我後面的處理  
 * =====================================================================================
 */
	void
change_to_left_bottom_right_up (int &node1_x , int &node1_y , int &node2_x , int &node2_y )
{
	  /*-----------------------------------------------------------------------------
	   *  我想把　，　形成　obstacle 的　左下右上　　，　左上右下　的二種case , 
	   *  都弄成第一種case , 而且　點１是左下的點　，　點２是右上的點
	   *  這樣我下面更新plane 以及　加點的code  就不會這麼長了
	   *-----------------------------------------------------------------------------*/
	if( (node2_x -node1_x ) * (node2_y - node1_y ) > 0  ){
		// 下面這個發生　，　代表　點１在右上　，　點２在左下　，所以二個點要互換
		if( (node2_x - node1_x  ) < 0 ){
		int temp_x , temp_y ; 
		temp_x = node2_x ;
		temp_y = node2_y ;
		node2_x = node1_x ; 
		node2_y = node1_y ;
		node1_x = temp_x ; 
		node1_y = temp_y ; 
		}	

	}
	else if( (node2_x - node1_x  ) * (node2_y - node1_y  ) < 0  ){
		// 這裡　我只要抓出　　最大x,y  及最小的x,y 放到node 1 ,2 及可
		int x_min , y_min , x_max , y_max ; 

		if( node2_x - node1_x > 0 ){
			 x_max = node2_x ; 
			 x_min = node1_x ; 
		}
		else {
			 x_max = node1_x ; 
			 x_min = node2_x ; 
		}


		if( node2_y - node1_y > 0 ){
			 y_max = node2_y ; 
			 y_min = node1_y ; 
		}
		else {
			 y_max = node1_y ; 
			 y_min = node2_y ; 
		}

		node1_x = x_min ; 
		node1_y = y_min ; 
		node2_x = x_max ; 
		node2_y = y_max ; 
	}// end of 把點１變左下角　，　點２變右上角　

}		/* -----  end of function change_to_left_bottom_right_up  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  void level_info::generate_obstacle_pin()
 *  Description:  這裡生產的是 2D 的東西  ， 而且 obstacle間沒有 overlap 
 *  		  我要再做一個 generate_overlap_obstacle_pin() 用來生產 .gnu 的檔
 *
 *   =====================================================================================
 */
// 哇 ， 我不之道  將資料 封裝起來  會這麼的方便 , 我下面這個func 原本是要傳
// 6，7 個 參數的 ， 現在  一個都不用傳了  多方便丫
void level_info::generate_obstacle_pin(){

	// 想不到下面這個 srand , 只要放到func 的最頭就可以了 
	srand((unsigned)time(0)); 
	int index_in_array_vertex = 0 ;
	int obstacle_id_nu = 1 ;  // 這個值是用來給vertex ，　看那些vertex 是同一組的	
	int how_many_pin = 0 ;
	int how_many_obstacle = 0;

	/*-----------------------------------------------------------------------------
	 這裡來檢查看　，　我的東西　，　有沒有對
	cout << "node1  = ( " << temp_x_cord_1 << " , " << temp_y_cord_1 << " ) "<< endl ; 
	cout << "node2  = ( " << temp_x_cord_2 << " , " << temp_y_cord_2 << " ) "<< endl ; 
	int stop ;
	cout << "stop and observation " << endl; 
	cin >> stop ;  
	 抓完使用者要的obstacle 數目　，　便不在進來了		
	 *-----------------------------------------------------------------------------*/
	do{

		// 直接在這裡產生點　，　而且判斷　會否水平　垂直　跟面積太大　	
		int temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2 ;
		do{
				
		 temp_x_cord_1 = rand() % plane_width ; 
		 temp_y_cord_1 = rand() % plane_height;		
		
		 temp_x_cord_2 = rand() % plane_width ; 
		 temp_y_cord_2 = rand() % plane_height;	

		}while( Is_vertical_or_horizontal_line(temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2) ||
		     Is_area_large(obstacle_nu , plane_width , plane_height ,  
		                   temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2)  ) ;
				
		 //這裡要加func 去調成我要的　左下右上的　規格
		 change_to_left_bottom_right_up(temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2);

		// 點１　在左下　，　點２　在右上
		// 用來看　要偵查的區域　，　是否被別人使用了
		// 像　(5,6) , (8,7)  的這種　，　好像就不會進來設定了,這種case 
		// 沒有設到true  ,　所以別人會cross 它　
		// 看來　　這個obstacle 間　，　的obstacle overlap , 要判斷的詳細一點

		// 我之前預設是false   , 所以　　有可能　一些不合格的obstacle 跑進來
		bool obstacle_ok_or_not  = false ;
		bool before_state = true ; 
		// obstacle_plane 的設定　，　外圍也要設成true , 不然會有另外
		
		for(int i = temp_x_cord_1 + 1  ;  i < temp_x_cord_2  ;  i++   ){
			for(int j = temp_y_cord_1 + 1 ;  j <  temp_y_cord_2  ;  j++   ){
				// 這裡面　要確定　，　這個區域裡　每個pixel  都是false
				// 代表　，　都沒有人用，　則before_state會一直維持true 
				before_state = before_state && !(obstacle_plane[i][j]) ; 
			}
		}
		 //下面這行不能少　，　會出錯 , 因為上面那二個for 沒辦法，　擋住這種case 　　
		if( temp_x_cord_2 - temp_x_cord_1 == 1 || temp_y_cord_2 - temp_y_cord_1 == 1){
			for(int i = temp_x_cord_1   ;  i <= temp_x_cord_2  ;  i++   ){
				for(int j = temp_y_cord_1  ;  j <=  temp_y_cord_2  ;  j++   ){
					before_state = before_state && !(obstacle_plane[i][j]) ; 
				}
			}
			
		}
		
		obstacle_ok_or_not = before_state ;
			// count > 0  就不能加這個obstacle 
			// count == 0 就可以加
			if( obstacle_ok_or_not  == true  ){

				//１。 把這些點加入　obstacle_plane 及 pin_plane 內　
 				for(int i = temp_x_cord_1  ;  i <= temp_x_cord_2  ;  i++   ){
					for(int j = temp_y_cord_1  ;  j <= temp_y_cord_2 ;  j++   ){
						obstacle_plane[i][j] = true ; 
						// 看來我的pin_plane 是沒有含到那四個角點的 
						if( temp_x_cord_1 < i && i < temp_x_cord_2 && temp_y_cord_1 < j && j < temp_y_cord_2 ){
							pin_plane[i][j] = true ; 
						}
					}
				}// end of 把obstacle 的資料加入　，　二個plane中　

				// 看能不能在這裡 ， 判斷 ， 要加入的四個點 ， 有沒有被擋到 
				//２。　抓出四個角點　塞入array_vertex 內
				int which_corner = 1 ; // 指名那個位置的corner  
				for(int i = index_in_array_vertex  ;  i < index_in_array_vertex + 4 ;  i++   ){
 					array_vertex[i].set_pin(false);
					array_vertex[i].set_obstacle_id(obstacle_id_nu) ;

					if(which_corner == 1 ){
					array_vertex[i].set_position(temp_x_cord_1 , temp_y_cord_1);
					}
					else if(which_corner == 2 ){
					array_vertex[i].set_position(temp_x_cord_1 , temp_y_cord_2);
					}
					else if( which_corner == 3 ){
					array_vertex[i].set_position(temp_x_cord_2 , temp_y_cord_2);
					}
					else if( which_corner == 4){
					array_vertex[i].set_position(temp_x_cord_2 , temp_y_cord_1);
					}
					array_vertex[i].set_corner(which_corner) ;
					which_corner++ ;
				}		
				// 這是完整的記路每個obstalce 的資訊
				complete_obstacle_information.push_back( 
								obstacle( temp_x_cord_1 , temp_y_cord_1 , 
									  temp_x_cord_2 , temp_y_cord_2 , obstacle_id_nu ) 
								); 	
				// 下面三行是微調的變數
				index_in_array_vertex = index_in_array_vertex + 4 ; 
				obstacle_id_nu++ ; 
				how_many_obstacle++;

				// 看來我可以在這裡加　，　完整的obstacle 資訊　
				// 以便我之後　，　用來跟Kruskal_Tree 比對的東西　
			  }//end of 判斷 這個obstacle 是否ok  

		}while(   how_many_obstacle != obstacle_nu   );
	/*-----------------------------------------------------------------------------
	 *  			Check complete_obstacle_information
	 *  for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++ ){
	 	complete_obstacle_information[i].print_out_for_check() ; 
	}
	 *-----------------------------------------------------------------------------*/
	
	// 這個loop , 是要做map 的動作 ， 1 個key值對應到一個 obsatcle 
	for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++  ){
		obstacle_map[ complete_obstacle_information[i].print_obstacle_id() ] =  complete_obstacle_information[i] ; 
	}

	for(unsigned int j = 0 ; j < complete_obstacle_information.size() ; j++ ){
		// 先把頭尾  ， 放到 obstacle_bound_cross 內 
		int left_bound_x = complete_obstacle_information[j].print_ld_corner_x() ;
		int right_bound_x = complete_obstacle_information[j].print_ru_corner_x() ;

		for(int k = left_bound_x  ; k <= right_bound_x ; k++ ){
			obstacle_cross[k].push_back( complete_obstacle_information[j].print_obstacle_id() ) ; 	
		}// scan body of obstacle 

	}// inner loop j 
	
	//好了　，　這個loop可以拿來加pin 點　
	do{
		int pin_temp_x , pin_temp_y ; 

		do{
			pin_temp_x = rand() % plane_width  ; 
			pin_temp_y = rand() % plane_height  ; 
		}while(pin_plane[pin_temp_x][pin_temp_y] == true );

		// 如果那個位置沒被用到　，　就把點加入　，　且把false 變　true 
		pin_plane[pin_temp_x][pin_temp_y] = true ; 
		obstacle_plane[pin_temp_x][pin_temp_y] = true ; 
		array_vertex[index_in_array_vertex].set_pin(true) ;
		array_vertex[index_in_array_vertex].set_position(pin_temp_x , pin_temp_y) ;
		array_vertex[index_in_array_vertex].set_z_position( which_level ) ;
		index_in_array_vertex++ ;
		how_many_pin++ ;

	}while( how_many_pin != pin_nu );

	//下面這裡的東西　　，　　一直沒讀到　　，　八成是　在while  裡面　出不來	
	


	/*-----------------------------------------------------------------------------
	 * 
	 * for(int i = 0  ;  i < pin_nu + 4*obstacle_nu  ;  i++   ){
		array_vertex[i].print_out() ;
		cout << endl ; 
	} 
	 *-----------------------------------------------------------------------------*/

	cout << endl << endl << " 			This is level " << which_level << " :  " << endl << endl ; 
	cout << "Obstacle created = " << how_many_obstacle << endl  ;  
	cout << "Pin created = " << how_many_pin << endl ;

	/*-----------------------------------------------------------------------------
	 *  			好  ， 接下來 ， 是要把 plane 的密度給算出來
	 *  	每個pin , 1個算1 的加上去就好了 
	 *  	想不到這裡會遇到問題 ， 是 ， 我二個int 的整數 ， 在做相除的動作
	 *  	因為是int ， 所以不會出現小數後面的值出來 
	 *  	, 我還忘記一樣東西 ， 那就是 obstacle 的厚度如果 是 1 ， 
	 *  	那算出來 ， 會否有負的面積 
	 *-----------------------------------------------------------------------------*/
	 float area = plane_width * plane_height ; 
	 float area_used = 0  ; 
	 for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++ ){
		area_used += (  complete_obstacle_information[i].print_ru_corner_x()   -  complete_obstacle_information[i].print_ld_corner_x()  + 1  )  *
			     (  complete_obstacle_information[i].print_ru_corner_y()   -  complete_obstacle_information[i].print_ld_corner_y()  + 1  )  ;
	 }// scan all obstacle 

	// 因為之道 pin_nu  , 所以 直接加上去 就好了 
	area_used += pin_nu ; 
	// 我打算把 下面這個變數 ， 放在class的這個層級 ， 以利我計算出來大小後， 
	// 超過 plane 大小的 便不 往下run 了
	percent_used = (float)( area_used / area ) * 100  ; 
	cout << "Density =  " << percent_used << "%"  << endl ; 

	/*-----------------------------------------------------------------------------
	 *  
	這裡要用新方法　，　我打算每個Stage 都建一個script language, 
	我之道怎麼用gnuplot 去建obstacle , 那pin 呢　，　我先查查
	看來是　Pin 要存在PlaneData 內　，　obstacle 要用script lang 去輸出
	　我先弄好Pin 的東西　
	 *-----------------------------------------------------------------------------*/
	ofstream fcout("PlaneData.txt");
	for(int i = 0 ; i < total ; i++ ){
		if( array_vertex[i].Is_pin_or_not() == true ){
			array_vertex[i].print_position(fcout) ;	
			// 下面這行 是用來測試 ， 是否真的能讓 數值變成真的變數
			// cout << " pin_x = " << array_vertex[i].print_x_position() ; 
		}
	}

	fcout.close();	


}// end of generate_input 



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_PlaneData_gnu
 *  Description:  
 * =====================================================================================
 */
	void
level_info::print_PlaneData_gnu (  )
{
	// 我想 把下面的 PlaneData.gnu  變成
	// PlaneData_in_level_1.gnu 所以我要把上面的
	// which_level 轉成 字元 再加上去 
	// 我就記得有 + 運算元可以用麻
	// 我記得有個 可以 int 轉 string 
	// cplusplus.com
	// C++ : Reference : C Library : cstdlib (stdlib.h) : itoa 想不道
	// 是在 stdlib.h 內  我還以為在 string 內呢 
	// 若用C++，stringstream是個很好用的東西，stringstream無論是<<或>>，都會自動轉型，要做各型別間的轉換，stringstream是個很好的媒介。
	
	/*-----------------------------------------------------------------------------
	 *  stringstream sstr;
	 *--------int转string-----------
	int a=100;
	string str;
	sstr<<a;
	sstr>>str;
	cout<<str<<endl;
	
	<< 在 stringstream 中也是用來吃輸入的東西
	>> 當然就是用來 輸出的東西了 
	上面二個 << >>  有點像  bash 裡的 輸出入導向的東西
	stream 感覺是中間的一個處理字串 ， 最後輸出  也是要用 str 來接
	哇  好像很方便 
	串流可以重復使用嗎 
	 *-----------------------------------------------------------------------------*/
	 // 下面的變數 name 是存 PlaneData_in_level_1.gnu 你看到的檔名 
	//string descrip_name  ; 
	stringstream ss ;
	ss << "Plane Information In Level " << which_level  ; 
	//ss >> descrip_name ; 	
	
	//--------string转char[]--------
	////如果你想通过使用同一stringstream对象实现多种类型的转换，请注意在每一次转换之后都必须调用clear()成员函数。
	
	ofstream fscout( "PlaneData.gnu");
	fscout << " #  Pin  = " << pin_nu << "  	" << "obstacle  = " << obstacle_nu << endl ;  
	fscout << " set title \"PlaneData\" " << endl ; 
	fscout << " set xrange[0:" << plane_width - 1 <<  "]" << endl ; 
	fscout << " set yrange[0:" << plane_height - 1 <<  "]" << endl ; 
	fscout << "set xlabel \"X-axis\"" << endl ; 
	fscout << "set ylabel \"Y-axis\"" << endl ; 
	fscout << "set terminal postscript color" << endl ; 
	fscout << "set output \"PlaneData.ps\"" << endl ; 
	// SGcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 
	// 告夭 我記起來了  ， 只要多下個參數 ， 就可以 不產生 邊線 ， 最後面加個
	// noborder 
	fscout << "set style rect fc lt -1 fs solid 0.15  " << endl ; 
	// 這裡要讀obstacle 出來　　，　寫在PlaneData.gnu 裡面
	for( int i = 0 ; i < total ; i++ ){
		if( !( array_vertex[i].Is_pin_or_not() )    )
		{

			if( array_vertex[i].print_corner_id() == 1){
				fscout << "set object "<< array_vertex[i].print_obstacle_id() <<" rect from " ; 
				array_vertex[i].print_obstacle_for_script(fscout )  ;
				fscout << "to " ;	
			}
			else if( array_vertex[i].print_corner_id() == 3){
				array_vertex[i].print_obstacle_for_script(fscout ) ;
				fscout << endl ; 
			}

		}
	}
	fscout << "plot \"PlaneData.txt\" w lp pt 7 lc rgb \"black\" " << endl ; 
	fscout.close() ;

}		/* -----  end of function print_PlaneData_gnu  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_overlap_obstacle_pin
 *  Description:  哇  它媽的  之前打的一大段都被吃掉了  ， 
 *  		  總之這個func 是用來  生產 overlap 的 input 的  ，
 *  		  雖然會共用 但沒關係  因為這二個func 我不會同時 call 
 *  		  我會叫使用者輸入 是要用那種  再去call 相對應的 
 *  		  所以不用怕  以 non_overlap 的 來當範本  
 *  		  稍微改一下  應該就可以了 
 * =====================================================================================
 */
	void
level_info::generate_overlap_obstacle_pin (  )
{
	srand((unsigned)time(0)); 
	int debug = 0  ; 

	int index_in_array_vertex = 0 ;
	int obstacle_id_nu = 1 ;  // 這個值是用來給vertex ，　看那些vertex 是同一組的	
	int how_many_pin = 0 ;
	int how_many_obstacle = 0;
		// 這裡來檢查看　，　我的東西　，　有沒有對
		//cout << "node1  = ( " << temp_x_cord_1 << " , " << temp_y_cord_1 << " ) "<< endl ; 
		//cout << "node2  = ( " << temp_x_cord_2 << " , " << temp_y_cord_2 << " ) "<< endl ; 
		//int stop ;
		//cout << "stop and observation " << endl; 
		//cin >> stop ;  
	// 抓完使用者要的obstacle 數目　，　便不在進來了		
	do{

		// 直接在這裡產生點　，　而且判斷　會否水平　垂直　跟面積太大　	
		int temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2 ;
		do{
				
		 temp_x_cord_1 = rand() % plane_width ; 
		 temp_y_cord_1 = rand() % plane_height;		
		
		 temp_x_cord_2 = rand() % plane_width ; 
		 temp_y_cord_2 = rand() % plane_height;	

		}while( Is_vertical_or_horizontal_line(temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2) ||
		     Is_area_large(obstacle_nu , plane_width , plane_height ,  
		                   temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2)  ) ;
				
		 //這裡要加func 去調成我要的　左下右上的　規格
		 change_to_left_bottom_right_up(temp_x_cord_1 , temp_y_cord_1 , temp_x_cord_2 , temp_y_cord_2);

		/*-----------------------------------------------------------------------------
		  抓出來的 obstacle 先跟  complete_obstacle_information 內的比 
		  限制是  角點不能有相同 ， 且  自已不能把任一個 obstacle 包住 or 被人
		  包住  上面這幾點  都沒發生  應該就可以了 
		  要記得設定  obstacle 及  pin Plane  , 
		  最後  要記得  把 array_vertex 塞入  但塞入的 應該是 替換點 
		  點如果被 coverd  就會產生 它的替身  我想 array_vertex 應是放入 
		  它的替身  
		 *-----------------------------------------------------------------------------*/

		bool obstacle_ok_or_not  = true ;
		for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++ ){

			// 我不希望有點共用
			if( ( complete_obstacle_information[i].print_ld_corner_x() == temp_x_cord_1   && 
			      complete_obstacle_information[i].print_ld_corner_y() == temp_y_cord_1 ) || 
			    ( complete_obstacle_information[i].print_ru_corner_x() == temp_x_cord_2 && 
			      complete_obstacle_information[i].print_ru_corner_y() == temp_y_cord_2 ) ){
				obstacle_ok_or_not = false ; 	 
				break ; 
			}
			else if( ( complete_obstacle_information[i].print_ru_corner_x() == temp_x_cord_1   && 
			           complete_obstacle_information[i].print_ru_corner_y() == temp_y_cord_1 ) || 
				 ( complete_obstacle_information[i].print_ld_corner_x() == temp_x_cord_2   && 
			           complete_obstacle_information[i].print_ld_corner_y() == temp_y_cord_2 ) ){
				obstacle_ok_or_not = false ; 	 
				break ; 
			}

			// 不要 大到把 裡面的某個 obstacle 包起來
			if( temp_x_cord_1 <= complete_obstacle_information[i].print_ld_corner_x() &&
			    temp_y_cord_1 <= complete_obstacle_information[i].print_ld_corner_y() &&
			    temp_x_cord_2 >= complete_obstacle_information[i].print_ru_corner_x() &&
			    temp_y_cord_2 >= complete_obstacle_information[i].print_ru_corner_y() ){
				obstacle_ok_or_not = false ; 
			}
			
			// 這裡便是 反過來  不要小到被人家包起來 
			if( temp_x_cord_1 >= complete_obstacle_information[i].print_ld_corner_x() &&
			    temp_y_cord_1 >= complete_obstacle_information[i].print_ld_corner_y() &&
			    temp_x_cord_2 <= complete_obstacle_information[i].print_ru_corner_x() &&
			    temp_y_cord_2 <= complete_obstacle_information[i].print_ru_corner_y() ){
				obstacle_ok_or_not = false ; 
			}

		}// 跟 complete_obstacle_information 內的 obstacle 比較



		if( obstacle_ok_or_not == true  ){
		 
			//１。 把這些點加入　obstacle_plane 內　 ,  及 pin_plane 內 
			for(int i = temp_x_cord_1  ;  i <= temp_x_cord_2  ;  i++   ){
				for(int j = temp_y_cord_1  ;  j <= temp_y_cord_2 ;  j++   ){
					obstacle_plane[i][j] = true ; 
					if( temp_x_cord_1 < i && i < temp_x_cord_2 && temp_y_cord_1 < j && j < temp_y_cord_2 ){
						pin_plane[i][j] = true ; 
					}
				}
			}// end of 把obstacle 的資料加入　，　二個plane中　

			// 

			//２。　抓出四個角點　塞入array_vertex 內
			int which_corner = 1 ; // 指名那個位置的corner  
			for(int i = index_in_array_vertex  ;  i < index_in_array_vertex + 4 ;  i++   ){
				// 記起來了 ， 我這裡是 ， 預先做好了 ， 要的size , 再一個個
				// 把它的資料給塞進去的
				array_vertex[i].set_pin(false);
				array_vertex[i].set_obstacle_id(obstacle_id_nu) ;

				if(which_corner == 1 ){
					array_vertex[i].set_position( temp_x_cord_1 , temp_y_cord_1);
				}
				else if(which_corner == 2 ){
					array_vertex[i].set_position(temp_x_cord_1 , temp_y_cord_2);
				}
				else if( which_corner == 3 ){
					array_vertex[i].set_position(temp_x_cord_2 , temp_y_cord_2);
				}
				else if( which_corner == 4){
					array_vertex[i].set_position(temp_x_cord_2 , temp_y_cord_1);
				}
				array_vertex[i].set_corner(which_corner) ;
				which_corner++ ;
			}		

			// 這是完整的記路每個obstalce 的資訊
			complete_obstacle_information.push_back( 
							obstacle( temp_x_cord_1 , temp_y_cord_1 , 
								  temp_x_cord_2 , temp_y_cord_2 , obstacle_id_nu ) 
							); 	

			index_in_array_vertex = index_in_array_vertex + 4 ; 
			obstacle_id_nu++ ; 
			how_many_obstacle++;
		}// ok  的 obstacle 可以拿進來  , overlap 的 obstacle 這樣就好了嗎 

		/*-----------------------------------------------------------------------------
		 *  
		// 點１　在左下　，　點２　在右上
		// 用來看　要偵查的區域　，　是否被別人使用了
		// 像　(5,6) , (8,7)  的這種　，　好像就不會進來設定了,這種case 
		// 沒有設到true  ,　所以別人會cross 它　
		// 看來　　這個obstacle 間　，　的obstacle overlap , 要判斷的詳細一點

		// 我之前預設是false   , 所以　　有可能　一些不合格的obstacle 跑進來
		bool obstacle_ok_or_not  = false ;
		bool before_state = true ; 
		// obstacle_plane 的設定　，　外圍也要設成true , 不然會有另外
		
		for(int i = temp_x_cord_1 + 1  ;  i < temp_x_cord_2  ;  i++   ){
			for(int j = temp_y_cord_1 + 1 ;  j <  temp_y_cord_2  ;  j++   ){
				// 這裡面　要確定　，　這個區域裡　每個pixel  都是false
				// 代表　，　都沒有人用，　則before_state會一直維持true 
				//  這個 obstacle 沒有跟別人 overlap 所以可放進來 
				before_state = before_state && !(obstacle_plane[i][j]) ; 
			}
		}
		 //下面這行不能少　，　會出錯 , 因為上面那二個for 沒辦法，　擋住這種case 　　
		 //喔  這種 case  是指 obstacle 的 其中一邊長度為 1 ， 
		 //因為上面那個  我的obstacle  是可以相鄰的 所以 上面 那個 檢查 是不看 外圍 
		 //只看中間那塊 
		if( temp_x_cord_2 - temp_x_cord_1 == 1 || temp_y_cord_2 - temp_y_cord_1 == 1){

			for(int i = temp_x_cord_1   ;  i <= temp_x_cord_2  ;  i++   ){
				for(int j = temp_y_cord_1  ;  j <=  temp_y_cord_2  ;  j++   ){
					before_state = before_state && !(obstacle_plane[i][j]) ; 
				}
			}
			
		}// end of special case  我忘記了 
		
		obstacle_ok_or_not = before_state ;
			// count > 0  就不能加這個obstacle 
			// count == 0 就可以加
			if( obstacle_ok_or_not  == true  ){

				//１。 把這些點加入　obstacle_plane 內　
 				for(int i = temp_x_cord_1  ;  i <= temp_x_cord_2  ;  i++   ){
					for(int j = temp_y_cord_1  ;  j <= temp_y_cord_2 ;  j++   ){
						obstacle_plane[i][j] = true ; 
						if( temp_x_cord_1 < i && i < temp_x_cord_2 && temp_y_cord_1 < j && j < temp_y_cord_2 ){
							pin_plane[i][j] = true ; 
						}
					}
				}// end of 把obstacle 的資料加入　，　二個plane中　

				//２。　抓出四個角點　塞入array_vertex 內
				int which_corner = 1 ; // 指名那個位置的corner  
				for(int i = index_in_array_vertex  ;  i < index_in_array_vertex + 4 ;  i++   ){
 					array_vertex[i].set_pin(false);
					array_vertex[i].set_obstacle_id(obstacle_id_nu) ;

					if(which_corner == 1 ){
					array_vertex[i].set_position(temp_x_cord_1 , temp_y_cord_1);
					}
					else if(which_corner == 2 ){
					array_vertex[i].set_position(temp_x_cord_1 , temp_y_cord_2);
					}
					else if( which_corner == 3 ){
					array_vertex[i].set_position(temp_x_cord_2 , temp_y_cord_2);
					}
					else if( which_corner == 4){
					array_vertex[i].set_position(temp_x_cord_2 , temp_y_cord_1);
					}
					array_vertex[i].set_corner(which_corner) ;
					which_corner++ ;
				}		
				// 這是完整的記路每個obstalce 的資訊
				complete_obstacle_information.push_back( 
								obstacle( temp_x_cord_1 , temp_y_cord_1 , 
									  temp_x_cord_2 , temp_y_cord_2 , obstacle_id_nu ) 
								); 	
				// 下面三行是微調的變數
				index_in_array_vertex = index_in_array_vertex + 4 ; 
				obstacle_id_nu++ ; 
				how_many_obstacle++;

				// 看來我可以在這裡加　，　完整的obstacle 資訊　
				// 以便我之後　，　用來跟Kruskal_Tree 比對的東西　
			  }//end of 判斷 這個obstacle 是否ok  
		 *-----------------------------------------------------------------------------*/

	}while(   how_many_obstacle != obstacle_nu   );

	// 竟然  ， 我這裡 ， 都把 要的 obstacle_corner 都給放入了 ， array_vertex , 那我 只要scan 每個點 
	// 看它在 相對應的 pin_plane 中 ， 會不會被擋到  不就好了麻 , 下面這個fox 不能用 total  , 因為我要設定的只有
	// obstacle_corners  沒有pin 點在內丫
	int total_obstacle_corners = 4 * obstacle_nu ;	
	for(int i = 0  ;  i < total_obstacle_corners ;  i++   ){
		if( pin_plane[ array_vertex[i].print_x_position() ][ array_vertex[i].print_y_position()] == true ){
			array_vertex[i].set_core_state( false ) ;  
		}
	}// scan all array_vertex 

	/*-----------------------------------------------------------------------------
	 *  			Check complete_obstacle_information
	 *  for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++ ){
	 	complete_obstacle_information[i].print_out_for_check() ; 
	}
	 *-----------------------------------------------------------------------------*/
	
	/*-----------------------------------------------------------------------------
	 * 	下面二塊要處理overlap的 ， 要在建pin前先拿來用
	 *  我要在這個區塊 ， 做二件事 ， 把obstacle_corner 的alive_or_not 給抓出來
	 *  還有就是 ， 把要用到的overlap_vertex 給抓出來 
	 *
	 *  第一件事 ， 先scan all obstacle , 看其 四個端點是否在 人家用過的位置上 
	 *  去設定其屬性 , 看來是要丟入去計算 interval 時 ， 才要看下面的屬性
	 *  如果是false  就不用 ， 設定其interval  
	 *-----------------------------------------------------------------------------*/
	 for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++ ){
	  
	  	// 用四個if , 來分別查看 ， 四個obstacle 的情況 , 在把下面四個點 轉成 vertex 放進
		// 各個 x_position vector 內 ， 才要設定  core = false 
		if( pin_plane[ complete_obstacle_information[i].print_ld_corner_x() ][ complete_obstacle_information[i].print_ld_corner_y() ] == true  ){
			// 給 false  代表死了  ， 
			complete_obstacle_information[i].set_ld_covered( false ) ;  
		}
		if( pin_plane[ complete_obstacle_information[i].print_lu_corner_x() ][ complete_obstacle_information[i].print_lu_corner_y() ] == true ){
			complete_obstacle_information[i].set_lu_covered( false ) ;  
		}
		if( pin_plane[ complete_obstacle_information[i].print_rd_corner_x() ][ complete_obstacle_information[i].print_rd_corner_y() ] == true ){
			complete_obstacle_information[i].set_rd_covered( false ) ;  
		}
		if( pin_plane[ complete_obstacle_information[i].print_ru_corner_x() ][ complete_obstacle_information[i].print_ru_corner_y() ] == true ){
			complete_obstacle_information[i].set_ru_covered( false ) ;  
		}
		// 這樣的設定 ， 即便是  ， 厚度為1 的obstacle 好像也沒有影響 
		int x = complete_obstacle_information[i].print_lu_corner_x() ; 
		unsigned int y_min = complete_obstacle_information[i].print_ld_corner_y() ; 
		unsigned int y_max = complete_obstacle_information[i].print_lu_corner_y() ; 
		for( unsigned int j = y_min + 1 ; j < y_max ; j++ ){
			only_left_bound[ x ][ j ] = true ; 
		}// loop j to set left_bound = true 

	 }// scan all obsatcle 

	/*-----------------------------------------------------------------------------
	 *  抓好上述的 ， 每個obstacle corner 的 alive or not  , 那接下來
	 *  要抓 overlap vertex  , 我在 level_info 中有建vecotr 給它放 ， 
	 *  看來 要把這個部份 ， 放到還沒建pin點的位置 ， 因為  建完obstacle 
	 *  就會把 pin 也給放進去了  print_ld_covered 
	 *  看來也是 scan all obstacle , 還要分左右邊界去抓哎  , 之前都只有考慮
	 *  左邊   ,  要看圖 得知 目前的overlap_vertex 是不是對的很簡單
	 *  只要看 內角90度的 ， 位置 ， 有沒有點 ， 就可以了 
	 *  Overlap_vertex 的產生 ， 是 obstacle body 跟 bound  , 有相交才會生的 , 
	 *  所以我要拿來兩兩比對 
	 *-----------------------------------------------------------------------------*/

	// 這個loop , 是要做map 的動作 ， 1 個key值對應到一個 obsatcle  , 我可以反過來 ， 給 obstacle 然後 要那個key
	for(unsigned int i = 0 ; i < complete_obstacle_information.size() ; i++  ){
		obstacle_map[ complete_obstacle_information[i].print_obstacle_id() ] =  complete_obstacle_information[i] ; 
	}

	// 各個 以 x_position 分類的vector  , 分別存著 ， 有那些obsacle 有擋到這個區域   ,  存的是其obstacle  id , 
	// 也就是用來   obstacle_map 裡去抓東西的 , 
	for(unsigned int j = 0 ; j < complete_obstacle_information.size() ; j++ ){
		// 先把頭尾  ， 放到 obstacle_bound_cross 內 
		int left_bound_x = complete_obstacle_information[j].print_ld_corner_x() ;
		int right_bound_x = complete_obstacle_information[j].print_ru_corner_x() ;
		obstacle_bound_cross[left_bound_x].push_back( complete_obstacle_information[j].print_obstacle_id() ) ; 	
		obstacle_bound_cross[right_bound_x].push_back( complete_obstacle_information[j].print_obstacle_id() ) ; 	

		for(int k = left_bound_x + 1 ; k < right_bound_x ; k++ ){
			obstacle_body_cross[k].push_back( complete_obstacle_information[j].print_obstacle_id() ) ; 	
		}// scan body of obstacle 

		for(int k = left_bound_x  ; k <= right_bound_x ; k++ ){
			obstacle_cross[k].push_back( complete_obstacle_information[j].print_obstacle_id() ) ; 	
		}// scan body of obstacle 

	}// inner loop j 


	map<int,obstacle>::iterator bound_it ;  
	map<int,obstacle>::iterator body_it ;  

	// 只有 obstacle的bound 跟別的obstacle body 撞到才會產生 Overlap_vertex , 所以我接下來就要二二比對 
	for(  int i = 0 ; i < plane_width ; i++ ){

		int bound_key = 0 , body_key = 0 ; 
	 	if( obstacle_bound_cross[i].size() > 0 && obstacle_body_cross[i].size() > 0 ){
		 
			 for(unsigned int j = 0 ; j < obstacle_bound_cross[i].size()  ; j++ ){
				 bound_key = obstacle_bound_cross[i][j] ;
				 bound_it = obstacle_map.find( bound_key ) ; 
				 for(unsigned int k = 0 ; k < obstacle_body_cross[i].size()  ; k++ ){
					 body_key = obstacle_body_cross[i][k] ;
					 body_it = obstacle_map.find( body_key ) ; 
					 bool intersect = false ; 
					 int overlap_vertex_x = -1  ; 
					 if( (*bound_it).second.print_ru_corner_y() > (*body_it).second.print_ld_corner_y() &&
					     (*bound_it).second.print_ld_corner_y() < (*body_it).second.print_ru_corner_y() ){
					 	intersect = true ;  
						overlap_vertex_x = i ; 
					 }
					 // 有相交後 ， 便是抓 ， 在 bound_obstacle 內的那個y , 但有可能二點都在裡面
					 // 只要body 的y 位置 有在 bound 上下邊界裡面 ， 就可以抓來當overlap_vertex
					 //   body 的ru 點的 y  , 相交的點抓出來後 ， 看在pin_palne   中  ， 可以用嗎
					 if( intersect == true &&  (*body_it).second.print_ru_corner_y() < (*bound_it).second.print_ru_corner_y() &&
					     			   (*body_it).second.print_ru_corner_y() > (*bound_it).second.print_ld_corner_y() &&
								   pin_plane[i][(*body_it).second.print_ru_corner_y()] == false 			  ){
								  overlap_vertex.push_back( vertex( i , (*body_it).second.print_ru_corner_y() , which_level ) );  
					  
					 }
					 if( intersect == true &&  (*body_it).second.print_ld_corner_y() < (*bound_it).second.print_ru_corner_y() &&
					     			   (*body_it).second.print_ld_corner_y() > (*bound_it).second.print_ld_corner_y() && 
								   pin_plane[i][(*body_it).second.print_ld_corner_y()] == false 			  ){
								  overlap_vertex.push_back( vertex( i , (*body_it).second.print_ld_corner_y() , which_level ) );  
								  // obstacle plane 就不用給 true 了  ， 因為overlap_vertex  是在 obstacle的邊界產生的
								  // 所以本來就一定會被蓋住
					 }

				 }// scan all obsatcle 

			 }// scan all obsatcle 

		}// 這個 x_position 有bound obstacle 存在  才進來 

	}// scan all x_position 
	

	//好了　，　這個loop可以拿來加pin 點　 , 下面加點的這塊  ， 
	//我想 應該不用動吧  ,  可以直接拿來用  我只要改上面那塊就可以了  
	do{
		int pin_temp_x , pin_temp_y ; 

		do{
			pin_temp_x = rand() % plane_width  ; 
			pin_temp_y = rand() % plane_height  ; 
		}while( pin_plane[pin_temp_x][pin_temp_y] == true );

		// 如果那個位置沒被用到　，　就把點加入　，　且把false 變　true 
		pin_plane[pin_temp_x][pin_temp_y] = true ; 
		obstacle_plane[pin_temp_x][pin_temp_y] = true ; 
		array_vertex[index_in_array_vertex].set_pin(true) ;
		array_vertex[index_in_array_vertex].set_position(pin_temp_x , pin_temp_y) ;
		array_vertex[index_in_array_vertex].set_z_position( which_level ) ;
		index_in_array_vertex++ ;
		how_many_pin++ ;
		// 多一個 叫  only_pin 的變數  來存 ， 單只有pin 點資訊的 ， vector 
		only_pin.push_back( vertex( pin_temp_x , pin_temp_y ) ) ; 

	}while( how_many_pin != pin_nu );
	


	/*-----------------------------------------------------------------------------
	 * 
	下面這裡的東西　　，　　一直沒讀到　　，　八成是　在while  裡面　出不來	
	 * for(int i = 0  ;  i < pin_nu + 4*obstacle_nu  ;  i++   ){
		array_vertex[i].print_out() ;
		cout << endl ; 
	} 
	 *-----------------------------------------------------------------------------*/

	cout << endl << endl << " 			This is level " << which_level << " :  " << endl << endl ; 
	cout << "Obstacle created = " << how_many_obstacle << endl  ;  
	cout << "Pin created = " << how_many_pin << endl ;
	if( debug == 1 ){
		// 哇  ， 我這裡 雖然 抓好了  ，  pin 點 ， 但是還沒有放入 array_vertex_vector , 
		// 也忘了  ， array_vertex_vector 是放在那裡的 
		// 奇怪了 ， 值到這裡 ， 都很正常丫   ， 那到底是那裡出的問題呢 
		for( int i = 0 ; i < total ; i++ ){
			array_vertex[i].print_out_for_check() ;  
		}// print all pin 

		for( unsigned int i  = 0 ; i < complete_obstacle_information.size() ; i++ ){
			complete_obstacle_information[i].print_out_for_check() ; 
		}// print all obstacle info 
	}

	/*-----------------------------------------------------------------------------
	 *  			好  ， 接下來 ， 是要把 plane 的密度給算出來
	 *  	每個pin , 1個算1 的加上去就好了 
	 *  	想不到這裡會遇到問題 ， 是 ， 我二個int 的整數 ， 在做相除的動作
	 *  	因為是int ， 所以不會出現小數後面的值出來 
	 *  	, 我還忘記一樣東西 ， 那就是 obstacle 的厚度如果 是 1 ， 
	 *  	那算出來 ， 會否有負的面積 
	 *
	 *  	等一 下 ， 這裡是要算 overlap 的面積  ， 跟原本用在non_overlap的地方
	 *  	不太一樣 ， 要重新弄  , 這裡有個比較笨的方法 ， 就是 ， 直接去掃 ， 
	 *  	pin_plane 的 看每個位置的存活狀況 ， 但這應該要多很多時間
	 *-----------------------------------------------------------------------------*/
	 float area = plane_width * plane_height ; 
	 float area_used = 0  ; 
	 // 應該把下面這個for 修改一下  ， 應該就可以了
	 // 哇 ， 光是 100x100 , 就要跑一萬次了 
	for(int i = 0  ;  i < plane_width ;  i++   ){
		for(int j = 0  ;  j < plane_height ;  j++   ){
			if( pin_plane[i][j] == true ){
				area_used += 1 ;  
			} // true means used 
		} 
	} // end of for loop i 

	// 因為之道 pin_nu  , 所以 直接加上去 就好了 
	area_used += pin_nu ; 
	// 我打算把 下面這個變數 ， 放在class的這個層級 ， 以利我計算出來大小後， 
	// 超過 plane 大小的 便不 往下run 了
	percent_used = (float)( area_used / area ) * 100  ; 
	cout << "Density =  " << percent_used << "%"  << endl ; 

	/*-----------------------------------------------------------------------------
	這裡要用新方法　，　我打算每個Stage 都建一個script language, 
	我之道怎麼用gnuplot 去建obstacle , 那pin 呢　，　我先查查
	看來是　Pin 要存在PlaneData 內　，　obstacle 要用script lang 去輸出
	　我先弄好Pin 的東西　
	 *-----------------------------------------------------------------------------*/
	ofstream fcout("Overlap_PlaneData.txt");
	for(int i = 0 ; i < total ; i++ ){
		if( array_vertex[i].Is_pin_or_not() == true ){
			array_vertex[i].print_position(fcout) ;	
		}
	}
	fcout.close();	

	// 這裡 要在多建個 ， Overlap_vertex.txt , 用緑點來表示它  , 
	// 這東西 建在這裡 ， 應該不會跟 我後面 level 跟level 比對所產生的
	// projection_vertex ， 弄在一起吧
	ofstream ffcout("Overlap_Vertex.txt");
	// cout << "overlap_vertex.size = " << overlap_vertex.size() << endl ; 真的只有抓到四個 ， 怎麼會這樣 
	for(unsigned int i = 0 ; i < overlap_vertex.size()  ; i++ ){
		if( overlap_vertex[i].Is_pin_or_not() == false && overlap_vertex[i].print_corner_id() == 0 ){
			overlap_vertex[i].print_position(ffcout) ;	
		}
	}
	ffcout.close();	

	// no , 這裡要做的是 obstacle corner , 會落在 別人的 obstacle body內 	

}		/* -----  end of function generate_overlap_obstacle_pin  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_Overlap_PlaneData_gnu
 *  Description:   因然如剛神說的一班  ， 前面沒加 level_info , 這個 func 就如同 副程式一班 
 * =====================================================================================
 */
	void
level_info::print_Overlap_PlaneData_gnu (  )
{
	// 依這個為 範本來改 
	// 因為 3D 不易觀察  所以 
	// 我先用2D 的來看看 
	ofstream fscout( "Overlap_PlaneData.gnu");
	fscout << " #  Pin  = " << pin_nu << "  	" << "obstacle  = " << obstacle_nu << endl ;  
	fscout << " set title \"Overlap_PlaneData\" " << endl ; 
	fscout << " set xrange[0:" << plane_width - 1 <<  "]" << endl ; 
	fscout << " set yrange[0:" << plane_height - 1 <<  "]" << endl ; 
	//fscout << " set zrange[0:" << every_level_info.size() <<  "]" << endl ; 
	fscout << "set xlabel \"X-axis\"" << endl ; 
	fscout << "set ylabel \"Y-axis\"" << endl ; 
	//fscout << "set zlabel \"Z-axis\"" << endl ; 

	fscout << "set terminal postscript color" << endl ; 
	fscout << "set output \"Overlap_PlaneData.ps\"" << endl ; 
	// SGcout << "set style rect fc lt -1 fs solid 0.15 noborder  " << endl ; 
	// 告夭 我記起來了  ， 只要多下個參數 ， 就可以 不產生 邊線 ， 最後面加個
	// noborder 
	fscout << "set style rect fc lt -1 fs solid 0.15  " << endl ; 
	// 這裡要讀obstacle 出來　　，　寫在Overlap_PlaneData.gnu 裡面
	// 哎 ， 我記得 可以只 畫obstacle 而不畫它的 邊線丫  ， 我之前怎麼弄的
	for( int i = 0 ; i < total ; i++ ){
		if( !( array_vertex[i].Is_pin_or_not() )    )
		{

			if( array_vertex[i].print_corner_id() == 1){
				fscout << "set object "<< array_vertex[i].print_obstacle_id() <<" rect from " ; 
				array_vertex[i].print_obstacle_for_script(fscout )  ;
				fscout << "to " ;	
			}
			else if( array_vertex[i].print_corner_id() == 3){
				array_vertex[i].print_obstacle_for_script(fscout ) ;
				fscout << endl ; 
			}

		}
	}
	fscout << "plot \"Overlap_PlaneData.txt\" w lp pt 7 lc rgb \"black\" ,  \"Overlap_Vertex.txt\" w lp pt 7 lc rgb \"green\" " << endl ; 
	fscout.close() ;

}		/* -----  end of function print_Overlap_PlaneData_gnu  -----  */

#endif
