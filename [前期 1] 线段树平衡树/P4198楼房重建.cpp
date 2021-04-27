#include <iostream>
#include <stdio.h>
#define new_Node( s , v , a , b ) ( & ( t[ cnt++ ] = Node( s , v , a , b ) ) )

using namespace std;

int n , m , x , cnt;
double y;

struct Node
{
	int size , sum; //这里我没有维护l,r，而是直接维护了节点的大小，因为本题不需要l,r 
	double value;
	Node * left , * right;
	Node( int s , double v , Node * a , Node * b ) : size( s ) , value( v ) , left( a ) , right( b ) , sum( 0 ) {}
	Node() {}
} * root , t[200000];

Node * build( int s )
{
	if( s == 1 ) return new_Node( 1 , 0 , 0 , 0 );
	Node * left = build( s >> 1 ) , * right = build( s - ( s >> 1 ) );
	return new_Node( s , 0 , left , right );
}

int cal( double x , Node * cur )
{
	if( cur -> size == 1 ) return cur -> value > x;
	if( x >= cur -> left -> value )	return cal( x , cur -> right );
	return cur -> sum - cur -> left -> sum + cal( x , cur -> left );
}

void modify( int x , double y , Node * cur )
{
	if( cur -> size == 1 ) cur -> value = y , cur -> sum = 1;
	else
	{
		x > cur -> left -> size ? modify( x - cur -> left -> size , y , cur -> right ) : modify( x , y , cur -> left );
		cur -> sum = cur -> left -> sum + cal( cur -> left -> value , cur -> right );
		cur -> value = max( cur -> left -> value , cur -> right -> value );
	}
}

inline int read()
{
	register int x = 0 , ch = getchar();
	while( !isdigit( ch ) ) ch = getchar();
	while( isdigit( ch ) ) x = x * 10 + ch - '0' , ch = getchar();
	return x;
}

char opt[5];

int main()
{
	n = read() , m = read();
	root = build( n );
	while( m-- )
	{
		x = read() , y = read();
		modify( x , y / x , root );
		printf( "%d\n" , root -> sum );
	}
	return 0;
}
