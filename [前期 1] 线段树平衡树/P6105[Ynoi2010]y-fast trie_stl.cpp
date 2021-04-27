#include <bits/stdc++.h>
using namespace std;
int n,c,sz;
multiset<int> a,b;
multiset<int>::iterator it;
inline int best(int x,int op)
{
	if(x==-1) return -1;
	it=a.upper_bound(c-1-x);
	if(it==a.begin()) return -1;
	it--;
	if(op==1 && *it==x && a.count(x)==1)
		return (it==a.begin())?-1:*--it;
	else
		return *it;
}
inline void insert(int x)
{
	sz++;
	if(sz==1) { a.insert(x); return; }
	int y=best(x,0),z=best(y,1),w=best(z,1);
	if(y!=-1 && z<x)
	{
		if(z!=-1 && y==w) b.erase(b.find(y+z));
		b.insert(x+y);
	}
	a.insert(x);
}
inline void erase(int x)
{
	a.erase(a.find(x)),sz--;
	if(!sz) return;
	int y=best(x,0),z=best(y,1),w=best(z,1);
	if(y!=-1 && z<x)
	{
		if(z!=-1 && y==w) b.insert(y+z);
		b.erase(b.find(x+y));
	}
}
inline int query()
{
	it=--a.end();
	if(a.count(*it)>=2) return *it*2%c;
	else return (*it+*--it)%c;
}
int main()
{
	scanf("%d%d",&n,&c);
	int op,x,lastans=0;
	while(n--)
	{
		scanf("%d%d",&op,&x); x^=lastans;
		if(op==1) insert(x%c);
        else erase(x%c);
		if(sz<2) puts("EE"),lastans=0;
		else printf("%d\n",lastans=max(query(),b.empty()?0:*--b.end()));
	}
	return 0;
}
