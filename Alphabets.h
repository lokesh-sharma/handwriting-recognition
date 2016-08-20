#ifndef ALPHABETS_H
#define ALPHABETS_H
#include<vector>
using std::vector;

class Alphabets
{
private:
    vector<vector<bool> > testData;
    vector<int> intersect_y( int x , int y1,
         int y2);
    vector<int> intersect_x(int y , int x1 ,
        int x2);

    bool Algo_for_A();
    bool Algo_for_B();
    bool Algo_for_C();
    bool Algo_for_D();
    bool Algo_for_E();
    bool Algo_for_F();
    bool Algo_for_G();
    bool Algo_for_H();
    bool Algo_for_I();
    bool Algo_for_J();
    bool Algo_for_K();
    bool Algo_for_L();
    bool Algo_for_M();
    bool Algo_for_N();
    bool Algo_for_O();
    bool Algo_for_P();
    bool Algo_for_Q();
    bool Algo_for_R();
    bool Algo_for_S();
    bool Algo_for_T();
    bool Algo_for_U();
    bool Algo_for_V();
    bool Algo_for_W();
    bool Algo_for_X();
    bool Algo_for_Y();
    bool Algo_for_Z();

    int approx_intersects_x(int y , int x1 , int x2 );
    int approx_intersects_y(int x , int y1 , int y2 );
    int height ;
    int width;
    int ending;
public:
    Alphabets(vector< vector<bool> > & srcset , int x);
    char check_alphabet();
    void render();
    int get_ending()const { return ending;}

};

#endif // ALPHABETS_H
