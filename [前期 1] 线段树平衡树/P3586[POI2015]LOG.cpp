#include <bits/stdc++.h>
#define ll long long
#define res register
using namespace std;
const ll maxn=1000000+10;
ll n,m,a[maxn],b[maxn],mp[maxn],tot,num;
//a[i]是目前的序列情况,b[i]是第几次修改后离散化1-num,mp记录离散化后的数组1-1e9
//tot是几次修改,num是离散化后的修改,c1[i]是1-i的个数，c2[i]是1-i的值的和 
ll c1[maxn],c2[maxn];
struct node{
	ll k,x,id;
}q[maxn];
//id是第几次修改 
inline ll read(){
	res ll x=0,f=1;char ch=getchar();
	while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
	while(ch>='0'&&ch<='9'){x=(x<<3)+(x<<1)+ch-'0';ch=getchar();}
	return (f==1)?x:-x;
}
ll lowbit(ll x){return x & (-x);}
void add_num(ll x,ll y){
	for(;x<maxn;x+=lowbit(x))
		c1[x]+=y;	
}
void add_sum(ll x,ll y){
	for(;x<maxn;x+=lowbit(x))
		c2[x]+=y;
}
ll query_num(ll x){
	ll ans=0;
	for(;x>0;x-=lowbit(x))
		ans+=c1[x];
	return ans;
}
ll query_sum(ll x){
	ll ans=0;
	for(;x>0;x-=lowbit(x))
		ans+=c2[x];
	return ans;
}

int main()
{
	n=read(),m=read();
	res ll k,x,c,s;char ch=getchar();
	for(ll i=1;i<=m;i++){
		while(ch!='U'&&ch!='Z') ch=getchar();
		q[i].k=read(),q[i].x=read();
		if(ch=='U') {
			q[i].id=++tot;
			b[tot]=mp[tot]=q[i].x;
		}
		else q[i].id=-1;
		if(i!=m) ch=getchar();
	}
	sort(mp+1,mp+tot+1);
	num=unique(mp+1,mp+tot+1)-mp-1;
	for(ll i=1;i<=tot;i++)
		b[i]=lower_bound(mp+1,mp+num+1,b[i])-mp;
	for(ll i=1;i<=m;i++)
		if(q[i].id!=-1) q[i].x=b[q[i].id];
	for(ll i=1;i<=m;i++){
		if(q[i].id!=-1){//q[i].x被离散化 
			if(a[q[i].k]>0){
				ll real=lower_bound(mp+1,mp+num+1,a[q[i].k])-mp;
				add_num(real+1,-1);
				add_sum(real+1,-a[q[i].k]);
			}
			a[q[i].k]=mp[q[i].x];
			if(mp[q[i].x]>0){
				add_num(q[i].x+1,1);
				add_sum(q[i].x+1,mp[q[i].x]);
			}
		}
		else {//q[i].x没被离散化,real是去q[i].x在mp中的位置 
			ll real=upper_bound(mp+1,mp+num+1,q[i].x)-mp-1;
			ll bigans=query_num(maxn+1)-query_num(real+1);
			ll smallans=query_sum(real+1);
			if(smallans>=(q[i].k-bigans)*q[i].x) printf("TAK\n");
			else printf("NIE\n");
		}
	}
	return 0;
}
