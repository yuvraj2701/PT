// PT using ear clipping
// Initialising ear vertices (ears function) takes O(n^2) time while
// Triangulate takes O(n^2) time resulting in O(n^2) ear clipping algorithm.
// Both functions iterate through each vertex which takes O(n) time
// and both call is_diagonal function at each vertex to check diagonal which takes 
// O(n) time resulting in O(n^2) complexity


#include <iostream>
#include <cmath>

using namespace std;

//helper structure to store coordinate of vertex
struct Point
{
    int x;
    int y;
};

struct Vertex
{
    int idx;  // stores index of vertex
    Point v;  // coordinates of vertex
    bool ear; // whether vertex is ear vertex

    //pointers to next and previous vertices in doubly linked list
    Vertex *next;
    Vertex *prev;
};

Vertex *head = NULL; // "head" pointer
int nvertices = 0;   // total number of polygon vertices
int diag_count = 0;  // store number of diagonals

Vertex *get(int x, int y) //allocates memory and returns a new vertex
{
    Vertex *temp = new Vertex;

    //fill in coordinates
    temp->v.x = x;
    temp->v.y = y;

    return temp;
}

void add_vertex(Vertex *temp) //add new polygon vertex using dll
{
    if (head) //some vertices already present
    {
        temp->next = head;
        temp->prev = head->prev;
        head->prev = temp;
        temp->prev->next = temp;
    }
    else //if temp is first vertex to be added
    {
        head = temp;
        head->next = head->prev = temp;
    }
}

//calculates twice area of triangle using determinant of vertex co-ordinates
int area_le(Point a, Point b, Point c)
{
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

// calculates twice area of polygon with fixing a point and moving anti-clockwise
// to sum area of individual triangles
// Here extra area gets subtracted
int area_poly()
{
    int sum = 0; //  to store area

    Vertex *p, *a;

    p = head;    // fixed
    a = p->next; // moving
    while (a->next != head)
    {
        sum += area_le(p->v, a->v, a->next->v); //area of individual triangle
        a = a->next;
    }
    return sum;
}

//returns sign of area calculated by taking a-b-c anticlockwise
int area_sign(Point a, Point b, Point c)
{
    double area2 = (b.x - a.x) * (double)(c.y - a.y) - (c.x - a.x) * (double)(b.y - a.y);

    if (area2 > 0.5)
        return 1;
    else if (area2 < -0.5)
        return -1;
    else
        return 0;
}

// if point c to left of line a-b
bool to_left(Point a, Point b, Point c)
{
    // a b c taken in anti-clockwise order should give +ve area
    return area_sign(a, b, c) > 0;
}

// if point c to left of line a-b or on it
bool left_or_on(Point a, Point b, Point c)
{
    return area_sign(a, b, c) >= 0;
}

//collinearity
bool collinear(Point a, Point b, Point c)
{
    return area_sign(a, b, c) == 0;
}

// check if c is on a-b
// helps to check incomplete intersection like _/_
bool between(Point a, Point b, Point c)
{
    if (!collinear(a, b, c))
        return false;

    // if ab not vertical, check betweenness on x; else on y
    if (a.x != b.x)
        return ((a.x <= c.x) && (c.x <= b.x)) || ((a.x >= c.x) && (c.x >= b.x));

    else
        return ((a.y <= c.y) && (c.y <= b.y)) || ((a.y >= c.y) && (c.y >= b.y));
}

// if a-b intersects with c-d
bool intersect(Point a, Point b, Point c, Point d)
{
    // atleast one point of a line should be between 2 points of
    // next line in case of collinearity
    if (collinear(a, b, c) || collinear(a, b, d) || collinear(c, d, a) || collinear(c, d, b))
    {
        if (between(a, b, c) || between(a, b, d) || between(c, d, a) || between(c, d, b))
            return true;
        else
            return false;
    }
    // complete intersection
    // using xor property
    else if ((((int)to_left(a, b, c)) ^ ((int)to_left(a, b, d))) && (((int)to_left(c, d, a)) ^ ((int)to_left(c, d, b))))
        return true;

    return false;
}

// check if a-b lies inside polygon
bool interior(Vertex *a, Vertex *b)
{
    Vertex *a0, *a1; /* a0,a,a1 are consecutive head. */

    a1 = a->next;
    a0 = a->prev;

    // if a is a convex vertex
    if (left_or_on(a->v, a1->v, a0->v))
        return to_left(a->v, b->v, a0->v) && to_left(b->v, a->v, a1->v);

    // else a is reflex
    return !(left_or_on(a->v, b->v, a1->v) && left_or_on(b->v, a->v, a0->v));
}

//is a-b diagonal
//it should be interior and non intersecting to polygon
bool is_diagonal(Vertex *a, Vertex *b)
{
    Vertex *c, *c1;
    bool flag = true;

    // for each edge (c,c1) of polygon
    c = head;
    while (1)
    {
        c1 = c->next;
        // skip edges incident to a or b and check if others intersect a-b
        if ((c != a) && (c1 != a) && (c != b) && (c1 != b) && intersect(a->v, b->v, c->v, c1->v))
        {
            flag = false;
            break;
        }

        c = c->next;

        if (c == head)
            break;
    }
    return interior(a, b) && interior(b, a) && flag;
}

// initialise ear vertices which will be updated as we addis_ddiagonals
void ears(void)
{
    //cout<<"Hi\n"; //debug
    Vertex *v0, *v1, *v2; /* three consecutive head */

    // initialize v1->ear for all head
    v1 = head;

    while (1)
    {
        v2 = v1->next;
        v0 = v1->prev;
        v1->ear = is_diagonal(v0, v2);
        v1 = v1->next;

        if (v1 == head)
            break;
    }
}

void PrintDiagonal(Vertex *a, Vertex *b)
{
    cout << "( " << a->idx << "," << b->idx << " )\n";
}

void Triangulate(void)
{
    //cout<<"Hi\n"; //debug
    Vertex *v0, *v1, *v2, *v3, *v4; // five consecutive vertices
    int n = nvertices;              // number of vertices which decreases upto 3
    bool earfound;                  // check if ear vertex is found
    ears();

    // outer loop removes one ear in each iteration
    while (n > 3)
    {
        // inner loop searches for an ear and updates the vertices
        v2 = head;
        earfound = false;
        while (1)
        {
            if (v2->ear)
            {
                earfound = true;

                diag_count++;

                // ear found and now fill variables
                v3 = v2->next;
                v4 = v3->next;
                v1 = v2->prev;
                v0 = v1->prev;

                // (v1,v3) is a diagonal
                PrintDiagonal(v1, v3);

                // update status of diagonal endpoints
                v1->ear = is_diagonal(v0, v3);
                v3->ear = is_diagonal(v1, v4);

                // cut off the ear v2
                v1->next = v3;
                v3->prev = v1;
                head = v3; // in case the head was v2
                n--;
                break;
            } /* end if ear found */
            v2 = v2->next;

            if (v2 == head)
                break;
        }
        //if no ear is found
        if (!earfound)
        {
            printf("Error in Triangulate:  No ear found.\n");
            exit(1);
        }
    } // end outer while loop
}

int main()
{
    int n;
    cout << "Enter number of vertices - ";
    cin >> n; //input number of vertices

    if (n < 3)
    {
        cout << "n must be >= 3\n";
        return 0;
    }

    Vertex *temp;
    int x, y;
    int idx = 0;

    cout << "\nEnter their x,y (x(space)y) co-ordinates in anti-clockwise order-\n";
    while (n--)
    {
        //input data and add new vertex to dll
        cin >> x >> y;
        temp = get(x, y);
        temp->idx = idx++;

        add_vertex(temp);
    }

    nvertices = idx;

    Triangulate();

    if (!diag_count)
        cout << "No diagonal could be added\n";

    return 0;
}