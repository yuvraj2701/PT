#include <bits/stdc++.h>

using namespace std;

struct Point
{
    int x;int y;
};

int     AreaSign( Point a, Point b, Point c )
{
    double area2;

    area2 = ( b.x - a.x ) * (double)( c.y - a.y ) -
            ( c.x - a.x ) * (double)( b.y - a.y );

    /* The area should be an integer. */
    if      ( area2 >  0.5 ) return  1;
    else if ( area2 < -0.5 ) return -1;
    else                     return  0;
}

bool Collinear( Point a, Point b, Point c )
{
   return  AreaSign( a, b, c ) == 0;
}

int main()
{
    Point a{0,0},b{2,0},c{3,0};
    cout<<Collinear(b,c,a);
    return 0;
}