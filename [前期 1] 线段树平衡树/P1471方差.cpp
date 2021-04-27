#include<bits/stdc++.h>
using namespace std;
/*
sigma (a[i]-(a-increase))^2  //推导过程，其中的increase 就是题解中的 decrease 
sigma (a[i]-a+increase)^2
sigma (a[i]-a)^2+increase^2+2*(a[i]-a)*increase
 fang*len+ sigma increase^2+2*a[i]*increase-2*a*increase
fang*len+2*sum*increase+len*increase^2 -2*a*len*increase
*/
typedef double ld;
const int N=1e5+5; 
struct node{  //表示区间的结构体 
	ld tag,sum,fang,l,r,aver,len;  //fang 表示方差 
	node() { tag=sum=fang=l=r=0;}
}tr[N<<2];
node none;   //利用构造函数构造一个空区间 
ld Plus(node a,ld increase)   //将 sigma (a[j]-aver_a)^2 变为 sigma (a[j]-aver)^2 
{
	
	return a.fang*a.len+a.len*increase*increase;
}
node operator +(const node &a,const node &b)  //区间合并 
{
	node c;
	c.l=a.l;
	c.r=b.r;  //区间左右端点
			  //从这里可以看出，这个方法并不满足交换律，打代码时千万别写反 
	c.sum=a.sum+b.sum;
	c.len=a.len+b.len;    //区间和以及区间长度 
	c.aver=c.sum/c.len;   //区间平均 
	ld increase1=a.aver-c.aver,increase2=b.aver-c.aver;  //左右两边的decrease 
	c.fang=Plus(a,increase1)+Plus(b,increase2);  //合并方差 
	c.fang/=c.len;   //记得除 
	c.tag=0;
	return c;
}
int n,m;
ld val[N];
#define ls (k<<1)
#define rs (k<<1|1)   //宏定义使码风更加简洁 
void build(int k,int l,int r)  //建树 
{
	tr[k].l=l;
	tr[k].r=r;
	if(l==r)
	{
		tr[k].len=1;
		tr[k].sum=tr[k].aver=val[l];   //初始的平均值当然是自己了 
		tr[k].tag=0;
		tr[k].fang=0;  //方差显然为0 
		return;
	}
	int mid=l+r>>1;
	build(ls,l,mid);
	build(rs,mid+1,r);
	tr[k]=tr[ls]+tr[rs];  //区间合并 
}
inline void Add(int k,ld v)  //区间整体加，方差不变 
{
	tr[k].tag+=v;  //叠加标记 
	tr[k].sum+=tr[k].len*v;
	tr[k].aver+=v;    //平均数加上v
}
inline void pushdown(int k)  //下传标记 
{
	if(tr[k].tag==0) return;
	Add(ls,tr[k].tag);
	Add(rs,tr[k].tag);
	tr[k].tag=0;
}
inline void modify(int k,int x,int y,ld v)  //区间修改 
{
	if(tr[k].l>y||tr[k].r<x) return;
	if(x<=tr[k].l&&tr[k].r<=y)
	return Add(k,v);
	pushdown(k);   //记得下传标记 
	modify(ls,x,y,v);
	modify(rs,x,y,v);
	tr[k]=tr[ls]+tr[rs];
}
inline node query(int k,int x,int y)
{
	if(tr[k].l>y||tr[k].r<x) return none;  //返回一个空区间 
	if(x<=tr[k].l&&tr[k].r<=y)
	return tr[k];  //返回维护好的区间 
	pushdown(k);
	return query(ls,x,y)+query(rs,x,y);   //合并结果 
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++) scanf("%lf",&val[i]);
	build(1,1,n);
	for(int i=1;i<=m;i++)
	{
		int opt,x,y;ld v;
		scanf("%d",&opt);
		if(opt==1)   //最好不要用 switch,有点危险 
		{
			scanf("%d%d%lf",&x,&y,&v);
			modify(1,x,y,v);
		}
		if(opt==2)
		scanf("%d%d",&x,&y),printf("%.4lf\n",query(1,x,y).aver);
		if(opt==3) scanf("%d%d",&x,&y),printf("%.4lf\n",query(1,x,y).fang);
	}
}
