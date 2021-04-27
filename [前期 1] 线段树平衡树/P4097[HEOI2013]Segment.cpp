#include <iostream>
#include <stdio.h>
#include <math.h>
#define eps 1e-8
#define MAXN 100020
#define A 39989
#define B 1000000001

using namespace std;

struct Node
{
	int l , r , value;
	Node * left , * right;
	Node( int l , int r , int v , Node * a , Node * b ) : l( l ) , r( r ) , value( v ) , left( a ) , right( b ) {}
} * root;

double k[ MAXN ] , b[ MAXN ] , maxnum;
int now , ans;

inline double height( int a , int x )
{
	return k[a] * x + b[a];
}

inline double cal( int x , int y )
{
	return ( b[y] - b[x] ) / ( k[x] - k[y] );
}

inline void update( int a )
{
	double value = height( a , now );
	if( fabs( maxnum - value ) <= eps ) ans = min( ans , a );
	else if( value > maxnum ) maxnum = value , ans = a;
}

void modify( int l , int r , int x , Node * cur )
{
	if( !cur ) return;
	int mid = ( cur -> l + cur -> r ) >> 1;
	if( cur -> l == l && cur -> r == r )
	{
		if( !cur -> value )
		{
			cur -> value = x;
			return;
		}
        if( fabs( height( cur -> value , l ) - height( x , l ) ) <= eps && fabs( height( cur -> value , r ) - height( x , r ) ) <= eps )
        {
        	cur -> value = min( cur -> value , x );
        	return;
        }
        if( height( cur -> value , l ) > height( x , l ) + eps && height( cur -> value , r ) > height( x , r ) + eps ) return;
        if( height( cur -> value , l ) + eps < height( x , l ) && height( cur -> value , r ) + eps < height( x , r ) )
        {
        	cur -> value = x;
        	return;
        }
		double pos = cal( cur -> value , x );
		if( pos <= mid )
			if( height( cur -> value , l ) > height( x , l ) + eps ) modify( cur -> l , mid , cur -> value , cur -> left ) , cur -> value = x;
			else modify( cur -> l , mid , x , cur -> left );
		else
			if( height( cur -> value , r ) > height( x , r ) + eps ) modify( mid + 1 , cur -> r , cur -> value , cur -> right ) , cur -> value = x;
			else modify( mid + 1 , cur -> r , x , cur -> right );
	}
	else if( l > mid ) modify( l , r , x , cur -> right );
	else if( mid >= r ) modify( l , r , x , cur -> left );
	else modify( l , mid , x , cur -> left ) , modify( mid + 1 , r , x , cur -> right );
}

void find( int x , Node * cur )
{
	if( cur -> value ) update( cur -> value );
	if( cur -> l == cur -> r ) return;
	int mid = ( cur -> l + cur -> r ) >> 1;
	return find( x , x > mid ? cur -> right : cur -> left );
}

Node * build( int l , int r )
{
	if( l == r ) return new Node( l , r , 0 , 0 , 0 );
	return new Node( l , r , 0 , build( l , ( l + r ) >> 1 ) , build( ( ( l + r ) >> 1 ) + 1 , r ) );
}

inline int read()
{
    register int x = 0 , f = 1 , ch = getchar();
    while( ch < '0' || ch > '9' ){ if( ch == '-' ) f = -1;ch = getchar(); }
    while( ch >= '0' && ch <= '9' ){ x = x * 10 + ch - '0';ch = getchar(); }
    return x * f;
}

int n , opt , x0d , x1d , y0d , y1d , cnt;

int main()
{
	n = read();
	root = build( 1 , 40000 );
	while( n-- )
	{
		opt = read();
		if( opt )
		{
			x0d = ( read() + ans - 1 ) % A + 1 , y0d = ( read() + ans - 1 ) % B + 1;
			x1d = ( read() + ans - 1 ) % A + 1 , y1d = ( read() + ans - 1 ) % B + 1;
			k[ ++cnt ] = ( ( double ) ( y0d - y1d ) ) / ( ( double ) ( x0d - x1d ) );
			b[ cnt ] = ( ( y0d - k[ cnt ] * x0d ) + ( y1d - k[ cnt ] * x1d ) ) / 2;
			modify( min( x0d , x1d ) , max( x0d , x1d ) , cnt , root );
		}
		else
		{
			now = ( read() + ans - 1 ) % A + 1;
			ans = 0 , maxnum = 0;
			find( now , root );
			printf( "%d\n" , ans );
		}
	}
	return 0;
}
