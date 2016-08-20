#include "Alphabets.h"
#include"Game.h"
#include"Texturemanager.h"
#include<algorithm>
#include<map>
#include<iostream>


Alphabets::Alphabets(vector< vector<bool> > & srcset , int x_begin)
{
    vector<int> max_x;   ///storing intersections in x
    vector<int> max_y;   ///storing intersections in y
    int counter = x_begin;
    int x_start;
    int x_end;
    ///mark begining and ending of letter
    vector<int> arr_size;
    while(arr_size.size()== 0 && counter!= TheGame::getInstance()->get_width())
    {
        arr_size = TheGame::getInstance()->intersect_y(counter, 0 ,
                            TheGame::getInstance()->get_height());
        if(arr_size.size() != 0)
            x_start = counter;
        counter++;
    }
    while(arr_size.size()!= 0)
    {
        arr_size = TheGame::getInstance()->intersect_y(counter , 0 ,
                                        TheGame::getInstance()->get_height());
        if(arr_size.size() ==0)
            x_end = counter;
        counter++;
    }
    if(counter!= TheGame::getInstance()->get_width())
    ending = x_end+1;
    else ending = -1;   /// no further letters left
    if(ending != -1){
    for(int i = x_start ; i<ending ; i++)
    {
        for(int j = 0 ; j<TheGame::getInstance()->get_height() ; j++)
        {
            if(srcset[i][j]== true)
            {
                max_x.push_back(i);
                max_y.push_back(j);
            }
        }
    }
    std::sort(max_x.begin() ,max_x.end() );
    std::sort(max_y.begin() ,max_y.end() );
    int x =   *(max_x.end()-1) - *max_x.begin() +1 ;  ///max seperation in x
    int y =   *(max_y.end()-1) -  *max_y.begin()+1 ;  ///max seperation in y

    for(int i = 0 ; i<x ; i++)
        testData.push_back( vector<bool> (y , false));
    /// storing letter in testdata from screen
    for(int i = 0 ; i<x ; i++)
    {
        for(int j = 0 ; j<y  ; j++)
        {
            testData[i][j] = srcset[i+*max_x.begin()][j+ *max_y.begin()];
        }
    }

    height = testData[0].size();
    width = testData.size();
    }

}
vector<int> Alphabets::intersect_x(int y , int x1 , int x2)
{
    std::vector<int> intersections;
    for(int i = x1 ; i< x2 ; i++)
    {
        if(testData[i][y]==true)
            intersections.push_back(i);
    }
    return intersections;

}
vector<int> Alphabets::intersect_y(int x , int y1 , int y2)
{
    std::vector<int> intersections;
    for(int j = y1 ; j< y2 ; j++)
    {
        if(testData[x][j]==true)
            intersections.push_back(j);
    }
    return intersections;
}
bool Alphabets::Algo_for_A()
{
    bool condition_approx_interX=false;
    bool condition_approx_interY=false;
    bool condition_intersectionsX = false;
    //bool condition_top = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;

    if(ratio1 >4 || ratio2 > 3)      ///check for approriate height and width
      return false;
    vector<int> diffrence;
    int apparent_height = 0 ;         /// height neglecting end part straight line
    int counter_approx = 0 ;          /// on descending down distance
    for(int j = 0 ; j<height ; j++)     /// must increase or const
    {
        vector<int> temp = intersect_x(j , 0 , width);
        if(temp.size()!= 1)
            apparent_height++;
        if(temp.size()!= 0){
        diffrence.push_back(*(temp.end()-1) - *temp.begin());
        }
        if(approx_intersects_x(j , 0 , width) == 1)
            counter_approx++;
    }
    int counterX = 0 ;                  ///checks if distance increase on descending
    for(int i = 1 ; i<diffrence.size() ; i++)
    {
        if(diffrence[i] - diffrence[i-1] >=0)
            counterX++;
    }
    int counterY = 0;        ///intersection in Y
    for(int i = 0 ;i < width ; i++)
    {
        int cuts = approx_intersects_y(i , 0 , height);
        if(cuts == 1)
            counterY++;
    }
    int mid_point_approx = *((intersect_y(width/2 ,0, height)).end()-1);
    if(counterX > 0.8*apparent_height)
        condition_intersectionsX=true;
    if(counter_approx > 0.8*apparent_height)
        condition_approx_interX=true;

    if(counterY > width/3 && mid_point_approx <(3*apparent_height)/4 &&
       mid_point_approx > apparent_height/4)
        condition_approx_interY = true;

    int counter_top = 0;
    vector<int> temp = intersect_x(height/10 , 0 , width);
    int begin = *temp.begin();
    int end = *(temp.end()-1);
    /*for(int i = begin ; i!= end ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height/10);
        if(cut == 0)
            ++counter_top;
    }
    if(counter_top > 0.8*(end - begin))
        condition_top = true;*/

    if(condition_approx_interX && condition_intersectionsX &&
       apparent_height> (3*height)/4 && condition_approx_interY
       /*&& condition_top*/)
        return true;

    return false;
}
bool Alphabets::Algo_for_E()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_upper_part = false;
    bool condition_lower_part = false;
    bool condition_middle_part = false;
    bool condition_end_lower = false; ///avoid confusion with S

    int counterX = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j , 0 , width)==0)
            counterX++;
    }
    if(counterX > 0.7*height)
        condition_approxX=true;

    int cut3end;
    int cut2end;
    int counterY=0;

    for(int i = 0 ; i< width ; i++)
    {
        int cuts =approx_intersects_y(i , 0 , height);
        if(cuts==2)
            counterY++;
        if(cuts == 1 && i > width/3)
            cut3end = i;
        if(cuts == 0 && i > width/3)
            cut2end = i;
    }

    if(cut3end > width/2 && cut2end > (4*width)/5 && counterY > width/3)
        condition_approxY = true;

    int counter_mid = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , height/3 , (2*height)/3 )==0)
            counter_mid++;
    }
    if(counter_mid > width/3)
        condition_middle_part=true;

    int counter_upper = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0,height/3  )==0)
            counter_upper++;
    }
    if(counter_upper > 0.7*width)
        condition_upper_part=true;

    int counter_lower = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , (2*height/3) ,height  )==0)
            counter_lower++;
    }
    if(counter_lower > 0.7*width)
        condition_middle_part=true;

    int counter_end = 0 ;
    for(int j = (2*height)/3 ; j< height;j++)
    {
        if(approx_intersects_x(j , width/2,width)==-1)
            counter_end++;
    }
    if(counter_end > 0.7*height/3)
        condition_end_lower = true;
    //std::cout<<condition_lower_part<<condition_middle_part<<condition_upper_part;
    if(condition_approxX && condition_approxY && condition_upper_part
       && condition_middle_part&& condition_upper_part && condition_end_lower)
        return true;

    return false;

}
bool Alphabets::Algo_for_C()
{
    bool condition_no_of_cutsX = false;
    bool condition_no_of_cutsY = false;
    bool condition_mid_line = false;  ///to avoid confusion with I
    bool condition_lower_part = false;///to avoid confusion with F
    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 3 || ratio2 >3) ///check for proper height and width
        return false;
    vector<int> intersects;
    for(int j = 0 ; j<height ; j++)
    {
        intersects.push_back(approx_intersects_x(j , 0 , width));
    }
    int cutsX = 0;
    int wrong_cuts = 0;                      /// 3 cuts instead of 1 for G
    for(int i = 0 ; i<intersects.size() ; i++){ ///how many times straight lines || to x
        if(intersects[i] == 0)                 ///cuts letter at places
         cutsX++;
         if(intersects[i]== 2)
            wrong_cuts++;
    }
    if(cutsX > height/3)
        condition_no_of_cutsX = true;
    if(wrong_cuts > height/10)
        condition_no_of_cutsX = false;

    intersects.clear();        ///now test in y
    for(int i = 0 ; i<width ; i++)
    {
        intersects.push_back(approx_intersects_y(i ,0 , height));
    }
    int cutsY = 0;
    for(int i = 0 ; i<intersects.size() ; i++)
        if(intersects[i]==1)
        cutsY++;
    if(cutsY > (width)/2)
        condition_no_of_cutsY = true;   /// if cuts = 1 set to true

    ///condition mid line
    int counter_mid = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        vector<int> temp = intersect_x(j, 0 , width);
        if(temp.size()!=0)
        {
            if(temp[0] < width/4 )
                counter_mid++;
        }
    }
    if(counter_mid > 0.7*height)
        condition_mid_line = true;

    int counter_lower = 0;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , (3*height)/4 , height)== 0)
            ++counter_lower;
    }
    if(counter_lower > 0.8* width)
        condition_lower_part = true;
    if(condition_no_of_cutsX && condition_no_of_cutsY && condition_mid_line
       && condition_lower_part)
        return true;
    else return false;
}
bool Alphabets::Algo_for_R()
{
    bool condition_min_dis = false;     ///min x dis at approx h/2
    bool condition_conti_lowerpart =false;
    bool condition_conti_upperpart =false;
    bool condition_intersect=false;
    std::multimap< int , int > j_diffrence;   /// dis diff and y co-ordinate
    std::vector<int> difference(height);
    for(int j = 0 ; j<height ; j++)
    {
        vector<int> temp = intersect_x(j , 0 , width);
        if(temp.size()!=0){
        int diff = *(temp.end()-1) - *temp.begin();
        j_diffrence.insert( std::pair<int,int> (diff,j));
        difference[j] = diff;
        }
    }
    std::sort(difference.begin() , difference.end());
    int min_diff;
    for(int i = height/7 ; i<difference.size() ; i++)   ///find min dis neglecting upper
    {                                                   ///part
        if(difference[i]!= 0){
            min_diff = difference[i];
            break;
        }
    }
    auto arr_min_dis  = j_diffrence.equal_range(min_diff); ///no of y with min dis
    int min_dis_j;
    for(auto it = arr_min_dis.first ; it!= arr_min_dis.second ; it++){
        if((*it).second > height/4 && (*it).second < (3*height)/4){
        condition_min_dis = true;
        min_dis_j = (*it).second;
        }
    }

    int lowerpart_counter = 0;
    int lowerwidth_end = *(intersect_x((4*height)/5 , 0 , width).end()-1);
    int lowerwidth_begin = *(intersect_x((4*height)/5 , 0 , width).begin());
    for(int i = lowerwidth_begin ; i<lowerwidth_end ; i++)
    {
        if(approx_intersects_y(i , (4*height)/5 ,height )== -1)
            lowerpart_counter++;
    }
    if(lowerpart_counter > 0.7*(lowerwidth_end-lowerwidth_begin))
        condition_conti_lowerpart = true;  /// must be dis conti in lower part
    if(min_diff > (3*width/4))
        condition_min_dis = false;


    int upperpart_counter = 0 ;
    int upperwidth_end = *(intersect_x(height/5 , 0 , width).end()-1);
    int upperwidth_begin = *(intersect_x(height/5 , 0 , width).begin());
    for(int i = upperwidth_begin ; i<upperwidth_end ; i++)
    {
        if(approx_intersects_y( i , 0 , height/5) == 0)
            upperpart_counter++;
    }
    if(upperpart_counter > 0.9*(upperwidth_end-upperwidth_begin))
        condition_conti_upperpart=true;  ///must be continous in upper part

    int true_count = 0 ;
    for(int j = 0 ; j< (7*height)/8 ; j++)   ///no of cuts on descending down must be 1
    {
        int cuts = approx_intersects_x(j , 0 , width);
        if(cuts == 1 || (cuts ==0 && j>height/3 && j< (2*height)/3))
            true_count++;
    }
    if(true_count > 0.9*((7*height)/8))
        condition_intersect = true;

    if(condition_min_dis && condition_conti_lowerpart && condition_intersect
       && condition_conti_upperpart)
        return true;
    else return false;

}
bool Alphabets::Algo_for_D()
{
    bool condition_approxX= false;
    bool condition_approxY= false;
    bool condition_initial_part = false; ///avoid confusion with O
    bool condition_end_part = false; ///avoid confusion with o

    int counterX = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j,0,width)== 1)
            counterX++;
    }
    if(counterX > 0.6*height)
        condition_approxX = true;

    int counterY = 0 ;
    for(int i = 0 ; i<width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==1)
            ++counterY;
    }
    if(counterY > 0.6*width)
        condition_approxY = true;

    for(int i = 0 ; i< width/6 ; i++)
    {
        vector<int> temp = intersect_y(i , 0 , height);
        if(temp.size()!=0){
        int diff = *(temp.end()-1) - *temp.begin();

        if(*(temp.end()-1) > (10*height)/11 && diff > (4*height)/5)
            condition_initial_part = true;
        }
    }
    int counter_end = 0 ;
    for(int i = (5*width)/6 ; i<width ;i++)
    {
        vector<int> temp = intersect_y(i , 0 , height);
        if(temp.size()!=0){
        int diff = (*(temp.end()-1) - *temp.begin());
        if(diff < height/1.5 && temp.size()!=1 && diff > (height)/6)
            counter_end++;
    }
    }
    if(counter_end > width/10)
        condition_end_part=true;

    //std::cout<<condition_approxX<<condition_approxY;
    //std::cout<<condition_initial_part<<condition_end_part;
    if(condition_approxX&& condition_approxY&& condition_initial_part
       && condition_end_part)
        return true;

    return false;
}
bool Alphabets::Algo_for_L()
{
    bool condition_intersectX = false; /// mostly cuts = 1 on moving in +y
    bool condition_intersectY = false; /// mostly cuts = 1 on moving in +x
    bool condition_lowerpart = false;  /// lower part must be wide
    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 10 || ratio2 >2)
        return false;
    ///condition intersectsX
    int counter_approxX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j , 0 , width)==0)
            counter_approxX++;
    }
    if(counter_approxX> 0.8*height)
        condition_intersectX = true;

    ///condition intersectsY
    int counter_approxY = 0 ;
    for(int i = 0 ; i<width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==0)
            counter_approxY++;
    }
    if(counter_approxY > 0.7*width)
        condition_intersectY = true;
    ///condition lowerpart
    for(int j = (5*height)/6 ; j<height ; j++)
    {
        vector<int> temp = intersect_x(j , 0 , width);
        if(*(temp.end()-1) == (width-1))
            condition_lowerpart = true;
    }

    if(condition_intersectX && condition_intersectY&& condition_lowerpart)
        return true;

    return false;

}
bool Alphabets::Algo_for_V()
{
    bool condition_intersects=false;  ///on descending down distance must decrease
    bool condition_approxX = false;  ///cuts in X must be1
    bool condition_approxY = false;   ///cuts in y must be 1
    bool condition_lowerWidth = false;  /// lower part must be least wide
    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 7 || ratio2 >2)
        return false;
    int apparent_heightstart;  /// actual height where pattern v start
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j , 0 , width)==1)
        {
            apparent_heightstart = j;
            break;
        }
    }
    if(apparent_heightstart > height/5) /// pattern v is small compared to height
        return false;
    ///condition approx x
    int counter_approxX=0;
    for(int j = apparent_heightstart ; j< height ; j++)
    {
        if(approx_intersects_x(j , 0 , width)==1)
            counter_approxX++;
    }
    if(counter_approxX  > 0.7*(height-apparent_heightstart))
        condition_approxX = true;

    ///condition approx y
    int counter_approxY = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==0)
            counter_approxY++;
    }
    if(counter_approxY > 0.9*width)
        condition_approxY = true;

    ///condition_intersects
    vector<int>difference;
    for(int j = apparent_heightstart ; j< height ; j++)
    {
        vector<int> temp = intersect_x(j , 0 , width);
        if(temp.size()!=0)
        difference.push_back( *(temp.end()-1) - *temp.begin());
    }
    int counter_inter=0;
    for(int i = 1 ; i< difference.size() ; i++)
    {
        if(difference[i] <= difference[i-1] )
            counter_inter++;
         else  counter_inter--;
    }
    if(counter_inter > 0.8*(height - apparent_heightstart))
        condition_intersects = true;
    ///condition_lowerWidth
    vector<int>lower_part = intersect_x( (9*height)/10 , 0 , width);
    if(*(lower_part.end()-1) - *lower_part.begin() < width/4)
        condition_lowerWidth = true;
    /*std::cout<<condition_approxX<<condition_approxY<<condition_intersects;
    std::cout<<condition_lowerWidth;*/
    if(condition_approxX&&condition_approxY&&
       condition_intersects && condition_lowerWidth)
        return true;

    return false;
}

bool Alphabets::Algo_for_I()
{
    bool condition_approxX = false;  ///cuts in x = 1
    bool condition_approxY = false;  ///cuts in y = 2
    bool condition_line_mid = false; /// vertical line should be at width/2 approx

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 10 || ratio2 >4)
        return false;
    /// condition approx x
    int counterX = 0;
    for(int j = 0 ; j<height ; j++ )
    {
        if(approx_intersects_x(j,0,width) == 0)
            counterX++;
    }
    if(counterX > 0.8*height)
        condition_approxX=true;
    /// condition approx y
    int counterY = 0;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height) == 1)
            counterY++;
    }
    if(counterY > 0.7*width)
        condition_approxY = true;
    ///condition mid line
    int counter_mid = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        vector<int> temp = intersect_x(j, 0 , width);
        if(temp.size()!=0)
        {
            if(temp[0] < (3*width)/4 && temp[0] > width/4 )
                counter_mid++;
        }
    }
    if(counter_mid > 0.67*height)
        condition_line_mid = true;

    if(condition_approxX&& condition_approxY&& condition_line_mid)
        return true;

    return false;
}
bool Alphabets::Algo_for_Z()
{
    bool condition_approxX = false; ///cuts in X must be 1
    bool condition_approxY = false;///cuts in y must be 3
    bool condition_mirror = false ;/// avoid confusion with mirror image S
    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 3 || ratio2 > 3)   ///check for proper height & width
     return false;
    ///condition_approx X
    int counterX = 0;
    for(int j = 0 ; j<height ; j++)
    {
        if(approx_intersects_x(j , 0 , width) == 0)
            counterX++;
    }
    if(counterX > 0.7*height)
        condition_approxX = true;

    ///condition_approxY
    int counterY = 0;
    for(int i = 0 ; i< width ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height) ;
        if(cut== 2)
            counterY++;
    }
    if(counterY > 0.5*width)
        condition_approxY = true;
    ///condition_mirror
    int mirror_counter = 0 ;
    vector<int> difference;
    for(int i = 0 ; i< width ; i++)
    {
        vector<int> temp = intersect_y(i , height/8 , height); ///below height/8
        if(temp.size()!= 0)                                   ///pattern should
        difference.push_back(*(temp.end()-1) - *temp.begin());///look like V rotated
    }                                                    ///by 90 to left

    for(int i = 1 ; i< difference.size() ; i++)
    {
        if(difference[i] > difference[i-1])
            mirror_counter++;
    }
    if(mirror_counter> 0.5*width)
        condition_mirror = true;
    if(condition_approxX && condition_approxY && condition_mirror)
        return true;

    return false;

}
bool Alphabets::Algo_for_N()
{
    bool condition_approxX = false;///cuts = 3 in X
    bool condition_approxY = false;///cuts = 1 in Y

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 2)   ///check for proper height & width
     return false;
    ///condition_approxX
    int counterX = 0;
    for(int j = 0 ; j<height ; j++)
    {
        if(approx_intersects_x(j , 0 , width) == 2)
            counterX++;
    }
    if(counterX > 0.6*height)
        condition_approxX = true;

    ///condition_approxY
    int counterY = 0;
    for(int i = 0 ; i< width ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height);
        if(cut== 0)
            counterY++;

    }
    if(counterY > 0.7*width)
        condition_approxY = true;
    if(condition_approxX && condition_approxY)
        return true;

    return false;
}
bool Alphabets::Algo_for_U()
{
    bool condition_approxX = false; ///cut in X = 2
    bool condition_approxY = false; ///cut in Y = 1
    bool condition_intersects= false;///on descending down diff must remain constant
                                     ///or decrease in lowerpart
    bool condition_lowerpart = false; /// avoid confusion with H . lowerpart must be
                                      ///continous
    int ratio1 = (float)height/width;
    int ratio2 = (float)width/height;

    if(ratio1> 4 || ratio2 > 3)    ///check for proper height & width
        return false;

    int apparent_height = -1; /// height where cuts = 2 starts
    ///condition_approxX
    int counterX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        int cut = approx_intersects_x( j , 0 , width);
        if(cut == 1)
        {
            counterX++;
            if(apparent_height==-1)
                apparent_height = j;
        }
    }
    if(counterX > 0.7*height)
        condition_approxX = true;

    ///condition_intersects
    vector<int> difference;
    for(int j =apparent_height ; j<height ; j++)
    {
        if(j >0){
        vector<int> temp = intersect_x( j , 0, width);
        if(temp.size() != 0)
        difference.push_back( *(temp.end()-1) - *temp.begin());
        }
    }
    int counter_inter = 0;
    for(int i = 1 ; i< difference.size() ; i++)
    {
        if(difference[i-1] == difference[i] || difference[i-1] <difference[i])
            counter_inter++;
    }
    if(counter_inter > 0.6*(height-apparent_height))
        condition_intersects = true;
    ///condition_approxY
    int counterY = 0;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==0)
            counterY++;
    }

    if(counterY >0.8*width)
        condition_approxY = true;

    ///condition_lowerpart
    int counter_lower = 0;  ///mark the x co-ordinates of points where line y=4h/5
    int lowerwidth_start ; ///cuts the pattern. between this range pattern must be
    int lowerwidth_end ;   ///continous
    vector<int> temp = intersect_x((4*height)/5 , 0 , width);
    lowerwidth_start = *temp.begin();
    lowerwidth_end = *(temp.end()-1);
    for(int i = lowerwidth_start ; i<lowerwidth_end ; i++)
    {
        if(approx_intersects_y(i , (4*height)/5 , height)== 0)
            counter_lower++;
    }
    if(counter_lower > 0.8*(lowerwidth_end-lowerwidth_start))
        condition_lowerpart++;


    if(condition_approxX&& condition_approxY&&condition_intersects
       && condition_lowerpart)
        return true;

    return false;

}
bool Alphabets::Algo_for_O()
{
    bool condition_approxX= false; ///cuts = 2 in X
    bool condition_approxY= false; ///cuts = 2 in y
    bool condition_mid_line = false; /// line x = w/2 must intersect pattern at max
                                     ///distance
    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 2 || ratio2 > 2) ///check for proper height & width
     return false;
    ///condition_approxX
    int counterX = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j,0,width)== 1)
            counterX++;
    }
    if(counterX > 0.6*height)
        condition_approxX = true;

    ///condition_approxY
    int counterY = 0 ;
    for(int i = 0 ; i<width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==1)
            ++counterY;
    }
    if(counterY > 0.6*width)
        condition_approxY = true;

    vector<int> temp = intersect_y(width/2 , 0 , height);
    int diff = *(temp.end()-1) - *temp.begin();

    if(diff > (5*height)/6 )
        condition_mid_line = true;

    if(condition_approxX&& condition_approxY && condition_mid_line)
        return true;

    return false;
}
bool Alphabets::Algo_for_T()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_midline = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;

    if(ratio1 > 5 || ratio2> 3)
        return false;

    int counterX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j , 0 , width)==0)
            counterX++;
    }
    if(counterX > 0.8*height)
        condition_approxX = true;

    int counterY = 0;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==0)
            counterY++;
    }
    if(counterY > 0.8*width)
        condition_approxY = true;
    for(int i = width/3 ; i< (2*width)/3 ; i++)
    {
        vector<int> temp = intersect_y(i , 0 , height);
        if(*(temp.end()-1) > (7*height)/8 )
            condition_midline=true;
    }

    if(condition_approxX && condition_approxY && condition_midline)
        return true;

    return false;
}
bool Alphabets::Algo_for_M()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_intersects = false ;/// avoid confusion with W

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 2)
        return false;

    int counterY = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height) == 0)
            counterY++;
    }
    if(counterY > 0.8*width )
        condition_approxY = true;

    int cut4end;
    int counterX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if(cut == 3)
            counterX++;
        if(cut == 1 && j > height/5)
        cut4end = j;
    }
    if(cut4end > height/4 && counterX > height/5)
        condition_approxX = true;

    int counter_inter = 0;
    for(int j = 1 ; j<height ; j++)
    {
        vector<int> temp1 = intersect_x(j-1 , 0 , width);
        vector<int> temp2 = intersect_x( j , 0 , width);
        int diff1 = *(temp1.end()-1) - *temp1.begin();
        int diff2 = *(temp2.end()-1) - *temp2.begin();
        if(diff2 >= diff1)
            counter_inter++;
    }
    if(counter_inter > 0.8*height)
        condition_intersects = true;

    if(condition_approxX && condition_approxY && condition_intersects)
        return true;

    return false;

}
bool Alphabets::Algo_for_B()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_min_dis = false;
    bool condition_initial_part = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 3 || ratio2 > 2)
     return false;
    int counterYcut2 = 0 ;
    int counterYcut3 = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height);
        if(cut == 2 || cut == 3)
            counterYcut2++;
        if(cut == 3)
            counterYcut3++;
    }
    if( counterYcut2 > 0.7*width && counterYcut3 > width/10)
        condition_approxY = true;

    int counterX = 0;
    for(int j = 0 ; j < height ; j++)
    {
        if(approx_intersects_x(j , 0 , width) == 1)
            counterX++;
    }
    if(counterX > 0.7*height)
        condition_approxX = true;

    int counter_int = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        vector<int> temp = intersect_x(j , 0 , width/5);
        if(temp.size()!= 0 )
            counter_int++;
    }
    if(counter_int > 0.7*height)
        condition_initial_part = true;

    if(condition_approxX && condition_approxY&& condition_initial_part)
        return true;

    return false;

}
bool Alphabets::Algo_for_F()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_upper = false;
    bool condition_middle = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 2)
     return false;

    int counterX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j ,0 , width) == 0)
            counterX++;
    }
    if(counterX > 0.8*height)
        condition_approxX = true;

    int counterY = 0;
    for(int i = 0 ; i< width ; i++)
    {
        int cuts = approx_intersects_y(i , 0 , height);
        if(cuts == 1)
            ++counterY;
        else if( cuts == 0 && i > width/3)
            ++counterY;
    }
    if(counterY > 0.8*width)
        condition_approxY = true;

    int counter_upper = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height/5)==0)
            counter_upper++;
    }
    if(counter_upper > 0.6*width )
        condition_upper = true;

    int counter_mid = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , height/3 , (3*height)/4) == 0)
            counter_mid ++;
    }
    if(counter_mid > width/3)
        condition_middle=true;
    //std::cout<<condition_middle<<condition_upper;
    if(condition_approxX && condition_approxY && condition_middle
       && condition_upper)
        return true;

    return false;
}
bool Alphabets::Algo_for_P()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_initial = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 2)
     return false;
    int counterX = 0;
    int cut2end;
    for(int j = 0 ; j< height ; j++)
    {
        int cut = approx_intersects_x( j , 0 , width);
        if(cut == 1)
            ++counterX;
        if(cut == 0 && j > height/3)
        {
            cut2end = j;
            break;
        }
    }
    for(int j = cut2end ; j< height ; j++)
    {
        int cut = approx_intersects_x( j , 0 , width);
        if(cut == 0)
            ++counterX;
    }
    if(counterX > 0.8*height)
        condition_approxX = true;

    for(int i = 0 ; i<width/8 ; i++)
    {
        vector<int> temp = intersect_y(i , 0 , height);
        if(*(temp.end()-1) < (2*height)/3)
            condition_initial=true;
    }

    int counterY = 0;
    for(int i = 0 ; i<width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)==1)
            ++counterY;
    }
    if(counterY > 0.7*width)
        condition_approxY=true;

    if(condition_approxX && condition_approxY && condition_initial)
        return true;

    return false;

}
bool Alphabets::Algo_for_K()
{
    bool condition_approxX = false;
    bool condition_approxY = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 3)
     return false;

    int counterX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j ,0 , width)== 1)
            counterX++;
    }
    if(counterX > 0.7*height)
        condition_approxX = true;

    int counterY = 0;
    for(int i = 1 ; i< width ; i++)
    {
        vector<int> temp1 = intersect_y(i-1 , 0, height);
        vector<int> temp2 = intersect_y(i , 0 , height);
        int diff1;
        int diff2;
        if(temp1.size()!= 0 && temp2.size()!= 0){
         diff1 = *(temp1.end()-1) - *temp1.begin();
         diff2 = *(temp2.end()-1) - *temp2.begin();
        }
        if(diff2 > diff1)
            counterY++;
    }
    if(counterY > 0.6*width)
        condition_approxY=true;

    if(condition_approxX&& condition_approxY)
        return true;

    return false;
}
bool Alphabets::Algo_for_H()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_mid = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 5 || ratio2 > 3)
     return false;
    int counterX = 0;
    for(int j = 0 ; j< height ; j++)
    {
        if(approx_intersects_x(j , 0 , width) == 1)
            counterX++;
    }
    if(counterX > 0.7* height)
        condition_approxX = true;

    int counterY = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , 0 , height)== 0)
            counterY++;
    }
    if(counterY > 0.7*width)
        condition_approxY=true;

    int counter_mid = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        if(approx_intersects_y(i , height/3 , (2*height)/3) == 0)
            ++counter_mid;
    }
    if(counter_mid > 0.7*width)
        condition_mid = true;

    if(condition_approxX && condition_approxY && condition_mid)
        return true;

    return false;

}
bool Alphabets::Algo_for_Y()
{
    bool condition_approxX = false;
    bool condition_approxY = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 2)
     return false;

    int counterY=0;
    for(int i = 0 ; i<width ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height);
        if((cut == 1 && i<width/2) || cut==0)
            ++counterY;
    }
    if(counterY > 0.8*width)
        condition_approxY=true;

    int counterX=0;
    for(int j = 0 ; j<height ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if((cut== 1 && j<height/3.5) || (cut==0 && j > height/3.5))
            counterX++;
    }
    if(counterX > 0.7*height)
        condition_approxX=true;

    if(condition_approxX && condition_approxY)
        return true;

    return false;

}
bool Alphabets::Algo_for_W()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    //bool condition_dis = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 3)
    return false;

    int apparent_height;
    int cut1end =-1;
    int counterX=0;
    for(int j = 0 ; j< height ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if(cut ==1){
            cut1end = j;
            break;
        }
    }
    apparent_height = height-cut1end;
    if(cut1end==-1)
        return false;
    for(int j = cut1end ; j< height ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if((cut == 1 && j< (2*apparent_height)/3 ) || cut== 3 )
            counterX++;
    }
    if(counterX > 0.6*(apparent_height))
        condition_approxX = true;

    int counterY = 0 ;
    for(int i = 0 ; i<width ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height);
        if(cut == 0)
            counterY++;
    }
    if(counterY > 0.8* width)
        condition_approxY = true;

   /* int counter_dis = 0 ;
    for(int j = cut1end ; j< height ; j++)
    {
        vector<int>temp1 = intersect_x(j-1,0 , width);
        vector<int>temp2 = intersect_x(j,0 , width);
        if(temp1.size()!=0 && temp2.size()!=0){
        int diff1 = *(temp1.end()-1) - *temp1.begin();
        int diff2 = *(temp2.end()-1) - *temp2.begin();
        //std::cout<<(diff1>diff2);
        if(diff1 >= diff2)
            counter_dis++;
        }
    }
    if(counter_dis > 0.8*apparent_height)
        condition_dis=true;*/

    //std::cout<<condition_approxX<<condition_approxY<<condition_dis;
    if(condition_approxX && condition_approxY /*&& condition_dis*/)
        return true;

    return false;
}
bool Alphabets::Algo_for_G()
{
   bool condition_approxX = false;
   bool condition_approxY = false;
   bool condition_top = false;

   float ratio1 = (float)height/width;
   float ratio2 = (float)width/height;
   if(ratio1 > 3 || ratio2 > 2)
    return false;

   int counterX = 0 ;
   int cut1end = 0 ;
   for(int j = 0 ; j< height ; j++)
   {
       int cut = approx_intersects_x(j , 0 , width);
       if((cut == 1 || cut==0) && j < (2*height/3) )
        counterX++;
       if(cut ==2 && j > height/3 )
        counterX++;
       if(cut==2 && cut1end ==0)
        cut1end = j;
   }
   if(counterX > 0.7*height && cut1end > height/4 && cut1end < (3*height)/4)
    condition_approxX = true;

   int counterY = 0 ;
   for(int i = 0 ; i<width/2 ; i++)
   {
       int cut = approx_intersects_y(i , 0 , height);
       if(cut == 0 || cut==1)
        ++counterY;
   }
   if(counterY > 0.8*(width/2))
    condition_approxY = true;

   int counter_top = 0 ;
   vector<int>temp = intersect_x(height/10 , 0 , width);
   int begin = *temp.begin();
   int end = *(temp.end()-1);
   for(int i = begin ; i< end ; i++)
   {
       int cut = approx_intersects_y(i , 0 , height/5);
       if(cut == 0)
        counter_top++;
   }
   //std::cout<<counter_top;
   if(counter_top >= 0.8*(end-begin))
    condition_top = true;
   if((end - begin) == 1)
    return false;

   if(condition_approxX && condition_approxY && condition_top)
    return true;

   return false;
}
bool Alphabets::Algo_for_X()
{
    bool condition_approxX = false;
    bool condition_approxY = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 3 || ratio2 > 3)
        return false;
    std::multimap<int , int > diff_i;
    vector<int> differences;
    for(int i = width/4 ; i<(3*width)/4 ; i++)
    {
        vector<int> temp = intersect_y(i , 0 , height);
        if(temp.size()!=0){
        int diff = *(temp.end()-1) - *temp.begin();
        diff_i.insert( std::pair<int,int> (diff , i));
        differences.push_back(diff);
        }
    }

    std::sort(differences.begin() , differences.end());
    int min_dis = differences[0];
    int min_dis_i = 0 ;
    int min_dis_j=0;
    auto arr_min_dis = diff_i.equal_range(min_dis);
    for(auto it = arr_min_dis.first ; it!= arr_min_dis.second ; it++)
    {
        if((*it).second < (2*width/3) && (*it).second > width/3)
            min_dis_i = (*it).second;
    }

    if(min_dis_i!=0)
    min_dis_j = *(intersect_y(min_dis_i ,0 , height).begin());
    else return false;

    int counterX = 0;
    for(int i = 1 ; i< width ; i++)
    {
        vector<int> temp1= intersect_y(i-1 , 0 , height);
        vector<int> temp2= intersect_y(i , 0 , height);
        if(temp1.size()!=0 && temp2.size()!=0){
        int diff1 = *(temp1.end()-1) - *temp1.begin();
        int diff2 = *(temp2.end()-1) - *temp2.begin();
        if(diff1 > diff2 && i< min_dis_i)
        ++counterX;
        if(diff2 > diff1 && i> min_dis_i)
        ++counterX;
        }
    }
    if(counterX > 0.6*width)
        condition_approxX = true;
    int counterY = 0 ;
    for(int j = 1 ; j<height ; j++)
    {
        vector<int> temp1= intersect_x(j-1 , 0 , width);
        vector<int> temp2= intersect_x(j , 0 , width);
        if(temp1.size()!=0 && temp2.size()!=0){
        int diff1 = *(temp1.end()-1) - *temp1.begin();
        int diff2 = *(temp2.end()-1) - *temp2.begin();
        if(diff1 > diff2 && j< min_dis_j)
        ++counterY;
        if(diff2 > diff1 && j> min_dis_j)
        ++counterY;
        }
    }

    if(counterY > 0.6*height)
        condition_approxY = true;

    if(condition_approxX && condition_approxY)
        return true;

    return false;

}
bool Alphabets::Algo_for_J()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_after_mid = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 4 || ratio2 > 2)
        return false;
    int counterX = 0 ;
    for(int j = 0 ; j<height/2 ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if(cut == 0)
            counterX++;
    }
    for(int j = height/2 ; j<height ; j++)
    {
        int cut = approx_intersects_x(j ,0 , width);
        if(cut==1)
            counterX++;
        if(cut == 0 && j< (9*height)/10 )
            counterX++;
        if(j > (9*height)/10 && cut == 0)
            counterX--;
    }
    if(counterX > 0.8*height)
        condition_approxX = true;

    int counterY = 0 ;
    for(int i = 0 ; i< width ;i++)
    {
        int cut = approx_intersects_y(i , 0 , height/2);
        if(cut == 0)
            counterY++;
    }
    if(counterY > 0.8*width)
        condition_approxY=true;

    int counter_after_mid = 0;
    for(int i = (2*width)/3 ; i<width ; i++)
    {
        int cut = approx_intersects_y(i , (2*height)/3 , height);
        if(cut == -1)
            counter_after_mid++;
    }
    if(counter_after_mid > 0.8*(width/3))
        condition_after_mid = true;

    if(condition_approxX&& condition_approxY && condition_after_mid)
        return true;

    return false;

}
bool Alphabets::Algo_for_S()
{
    bool condition_approxX = false;
    bool condition_approxY = false;

    int ratio1 = (float)height/width;
    int ratio2 = (float)width/height;
    if(ratio1 > 3 || ratio2 > 2)
        return false;

    int counterX = 0 ;
    for(int j = 0 ; j< height ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if(cut ==0)
            counterX++;
        if(cut== 1 && (j <height/3  || j>(2*height)/3))
            counterX++;
    }
    if(counterX > 0.7*height)
        condition_approxX = true;

    int counterY = 0 ;
    for(int i = 0 ; i< width ; i++)
    {
        int cut = approx_intersects_y(i , 0 ,height);
        if(cut== 2)
            ++counterY;
    }
    if(counterY > 0.7*width)
        condition_approxY = true;

    if(condition_approxX && condition_approxY)
        return true;

    return false;

}
bool Alphabets::Algo_for_Q()
{
    bool condition_approxX = false;
    bool condition_approxY = false;
    bool condition_end = false;

    float ratio1 = (float)height/width;
    float ratio2 = (float)width/height;
    if(ratio1 > 3 || ratio2 > 2)
        return false;
    int counterX=0;
    for(int j = 0 ; j<height;j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if(cut == 2)
            ++counterX;
    }
    if(counterX > width/10)
        condition_approxX = true;

    int counterY = 0 ;
    for(int i = 0 ; i<width ; i++)
    {
        int cut = approx_intersects_y(i , 0 , height);
        if(cut == 2)
            ++counterY;
    }
    if(counterY > height/10)
        condition_approxY = true;

    int counter_end = 0;
    for(int j = 0 ; j<height ; j++)
    {
        int cut = approx_intersects_x(j , 0 , width);
        if(cut == 0)
            counter_end++;

    }
    if(counter_end > height/9)
        condition_end = true;
    if(condition_approxX && condition_approxY && condition_end)
        return true;

    return false;
}

char Alphabets::check_alphabet()
{
    //std::cout<<"hello\n";
   bool c = Algo_for_C();
    if(c)
        return 'C';
    //std::cout<<"hello\n";
    bool r = Algo_for_R();
    if(r)
        return 'R';
    //std::cout<<"hello\n";
    bool a = Algo_for_A();
    if(a)
        return 'A';
    //std::cout<<"hello\n";
    bool l = Algo_for_L();
    if(l)
        return 'L';
    //std::cout<<"hello\n";
    bool v = Algo_for_V();
    if(v)
        return 'V';
    //std::cout<<"hello\n";
    bool i = Algo_for_I();
    if(i)
        return 'I';
    //std::cout<<"hello\n";
    bool z = Algo_for_Z();
    if(z)
        return 'Z';
    //std::cout<<"hello\n";
    bool n = Algo_for_N();
    if(n)
        return 'N';
    //std::cout<<"hello\n";
    bool u = Algo_for_U();
    if(u)
        return 'U';
    //std::cout<<"hello\n";
    bool d = Algo_for_D();
    if(d)
        return 'D';
     //std::cout<<"hello\n";
    bool o = Algo_for_O();
    if(o)
        return 'O';
     //std::cout<<"hello\n";
    bool t = Algo_for_T();
    if(t)
        return 'T';
     //std::cout<<"hello\n";
    bool e = Algo_for_E();
    if(e)
        return 'E';
     //std::cout<<"hello\n";
    bool m = Algo_for_M();
    if(m)
        return 'M';
     //std::cout<<"hello\n";
    bool b = Algo_for_B();
    if(b)
        return 'B';
     //std::cout<<"hello\n";
    bool f = Algo_for_F();
    if(f)
        return 'F';
     //std::cout<<"hello\n";
    bool p = Algo_for_P();
    if(p)
        return 'P';
     //std::cout<<"hello\n";
    bool k = Algo_for_K();
    if(k)
        return 'K';
     //std::cout<<"hello\n";
    bool h = Algo_for_H();
    if(h)
        return 'H';
     //std::cout<<"hello\n";
    bool y = Algo_for_Y();
    if(y)
        return 'Y';
     //std::cout<<"hello\n";
    bool w = Algo_for_W();
    if(w)
        return 'W';
     //std::cout<<"hello\n";
    bool g = Algo_for_G();
    if(g)
        return 'G';
     //std::cout<<"hellog\n";
    bool x = Algo_for_X();
    if(x)
        return 'X';
     //std::cout<<"hellox\n";
    bool j = Algo_for_J();
    if(j)
        return 'J';
     //std::cout<<"hello\n";
    bool s = Algo_for_S();
    if(s)
        return 'S';
     //std::cout<<"hello\n";
    bool q = Algo_for_Q();
    if(q)
        return 'Q';
     //std::cout<<"hello\n";
    return '0';
}

int Alphabets::approx_intersects_x(int y , int x1 , int x2)
{
     int result = 0;
     vector<int> intersects = intersect_x(y , x1 , x2);
     if(intersects.size() == 0)
        return -1;
     int max_gap = (x2-x1)/10;
     vector<int> differences;
     for(int i = 1 ; i< intersects.size() ; i++)
     {
         differences.push_back(intersects[i]-intersects[i-1]);
     }
     for(int i = 0 ; i<differences.size() ; i++)
     {
         if(differences[i] > max_gap)
            result++;
     }
     return result;
}
int Alphabets::approx_intersects_y(int x , int y1 , int y2)
{
    int result = 0;
    vector<int> intersects = intersect_y(x , y1 , y2);
    if(intersects.size() == 0)
        return -1;
    int max_gap = (y2-y1)/10;
    vector<int> differences;
    for(int i = 1 ; i<intersects.size() ; i++)
    {
        differences.push_back(intersects[i]-intersects[i-1]);
    }
    for(int i = 0 ; i<differences.size() ; i++)
    {
        if(differences[i] > max_gap)
            result++;
    }
    return result;
}
