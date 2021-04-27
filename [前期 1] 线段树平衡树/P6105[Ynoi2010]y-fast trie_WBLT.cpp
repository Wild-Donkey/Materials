#include <iostream>
#include <stdio.h>
#include <set>
#define MAXN 500010
#define ratio 4
#define new_Node( s , v , a , b ) ( & ( * st[ cnt++ ] = Node( s , v , a , b ) ) )
#define update( cur ) if( cur -> left -> size ) cur -> size = cur -> left -> size + cur -> right -> size , cur -> value = merge( cur -> left -> value , cur -> right -> value )

using namespace std;

struct heap
{
	multiset < int > a;
	inline void insert( int x )
	{
		a.insert( x );
	}
	inline void erase( int x )
	{
		a.erase( a.find( x ) );
	}
	inline int get_max1()
	{
		if( !a.empty() )
			return * --a.end();
		return 2147483646;
	}
	inline int get_max2()
	{
		if( a.size() >= 2 )
			return * -- a.end() + * ---- a.end();
		return -1;
	}
} heap1 , heap2;
//heap1：所有数，heap2：<=C/2的所有数

int n , C , cnt , num;

struct data
{
	int minA , maxB , maxAB , ans;
	data( int minA , int maxB , int maxAB , int ans ) : minA( minA ) , maxB( maxB ) , maxAB( maxAB ) , ans( ans ) {}
	data() {}
};

inline data adddata( int x )
{
	return data( x & 1 ? 2147483647 : x , x & 1 ? x : 0 , x , 2147483646 );
}

inline int max( int a , int b )
{
	return a > b ? a : b;
}

inline int min( int a , int b )
{
	return a < b ? a : b;
}

inline data merge( const data & a , const data & b )
{
	return data( min( a.minA , b.minA ) , max( a.maxB , b.maxB ) , max( a.maxAB , b.maxAB ) , min( min( a.ans , b.ans ) , a.maxB ? b.minA - a.maxB : 2147483646 ) );
}

struct Node
{
	int size;
	data value;
	Node * left , * right;
	Node( int s , data v , Node * a , Node * b ) : size( s ) , value( v ) , left( a ) , right( b ) {}
	Node() {}
} * root , t[ MAXN << 1 ] , * st[ MAXN << 1 ] , * father , * null;
//平衡树维护除了heap2堆顶的东西，a变成a*2+1，C-a变成(C-a)*2 

inline Node * merge( Node * a , Node * b )
{
	return new_Node( a -> size + b -> size , merge( a -> value , b -> value ) , a , b );
}

inline void rotate( bool x , register Node * cur )
{
	if( x ) cur -> left = merge( cur -> left , cur -> right -> left ) , st[ --cnt ] = cur -> right , cur -> right = cur -> right -> right;
	else cur -> right = merge( cur -> left -> right , cur -> right ) , st[ --cnt ] = cur -> left , cur -> left = cur -> left -> left;
}

inline void maintain( Node * cur )
{
	if( cur -> left -> size > cur -> right -> size * ratio ) rotate( 0 , cur );
	else if( cur -> right -> size > cur -> left -> size * ratio ) rotate( 1 , cur );
	if( cur -> left -> size > cur -> right -> size * ratio ) rotate( 1 , cur -> left ) , rotate( 0 , cur );
	else if( cur -> right -> size > cur -> left -> size * ratio ) rotate( 0 , cur -> right ) , rotate( 1 , cur );
}

inline void insert( int x , Node * cur )
{
	if( cur -> size == 1 )
		if( x > cur -> value.maxAB ) cur -> left = new_Node( cur -> size , cur -> value , null , null ) , cur -> right = new_Node( 1 , adddata( x ) , null , null );
		else cur -> left = new_Node( 1 , adddata( x ) , null , null ) , cur -> right = new_Node( cur -> size , cur -> value , null , null );
	else
		insert( x , x > cur -> left -> value.maxAB ? cur -> right : cur -> left );
	update( cur ) , maintain( cur ); 
}

inline void erase( int x , Node * cur )
{
	if( cur -> size == 1 )
		* father = cur == father -> left ? * father -> right : * father -> left;
	else
		father = cur , erase( x , x > cur -> left -> value.maxAB ? cur -> right : cur -> left );
	update( cur ) , maintain( cur );
}

inline int read()
{
	register int x = 0 , ch = getchar();
	while( !isdigit( ch ) ) ch = getchar();
	while( isdigit( ch ) ) x = x * 10 + ch - '0' , ch = getchar();
	return x;
}

struct io
{
	char ibuf[1 << 24] , * s , obuf[1 << 23] , * t;
	int a[24];
	io() : t( obuf )
	{
		fread( s = ibuf , 1 , 1 << 24 , stdin );
	}
	~io()
	{
		fwrite( obuf , 1 , t - obuf , stdout );
	}
	inline int read()
	{
		register int u = 0;
		while( * s < 48 )
			s++;
		while( * s > 32 )
			u = u * 10 + * s++ - 48;
		return u;
	}
	template < class T >
	inline void print( T u , int v )
	{
		if( u == -1 )
			* t++ = 'E' , * t++ = 'E';
		else
			print( u );
		* t++ = v;
	}
	template < class T >
	inline void print( register T u )
	{
		static int * q = a;
		if( !u ) * t++ = 48;
		else
		{
			if( u < 0 )
				* t++ = 45 , u *= -1;
			while( u ) * q++ = u % 10 + 48 , u /= 10;
			while( q != a )
				* t++ = * --q;
		}
	}
} ip;

#define read ip.read
#define print ip.print

int main()
{
	n = read() , C = read();
	for( int i = 0 ; i < MAXN << 1 ; i++ ) st[i] = & t[i];
	null = new_Node( 0 , data( 2147483647 , 0 , 0 , 2147483646 ) , 0 , 0 );
	root = new_Node( 1 , data( 2147483647 , 0 , 0 , 2147483646 ) , null , null );
	while( n-- )
	{
		int opt = read() , x = read() % C;
		if( opt == 1 )
		{
			num++;
			heap1.insert( x );
			if( x <= C / 2 )
			{
				if( !heap2.a.empty() )
					if( x > heap2.get_max1() )
						insert( heap2.get_max1() << 1 | 1 , root );
					else
						insert( x << 1 | 1 , root );
				heap2.insert( x );
				
			}
			else
				insert( C - x << 1 , root );
		}
		else
		{
			num--;
			heap1.erase( x );
			if( x <= C / 2 )
			{
				heap2.erase( x );
				if( !heap2.a.empty() )
					if( x >= heap2.get_max1() )
						erase( heap2.get_max1() << 1 | 1 , root );
					else
						erase( x << 1 | 1 , root );

			}
			else
				erase( C - x << 1 , root );
		}
		if( num >= 2 )
		{
			int a = heap1.get_max2() % C , b = heap2.get_max2() % C , ans = 0;
			ans = max( ans , a );
			ans = max( ans , b );
			if( !heap2.a.empty() )
			{
				multiset < int > :: iterator i = heap1.a.lower_bound( C - heap2.get_max1() );
				if( i != heap1.a.begin() ) i--;
				if( * i == heap2.get_max1() )
					if( i == heap1.a.begin() )
						goto out;
					else
						i--;
				ans = max( ans , * i + heap2.get_max1() );
			}
			out:
			print( max( C - ( root -> value.ans + 1 >> 1 ) , ans ) , 10 );
		}
		else print( -1 , 10 );
	}
	return 0;
}
