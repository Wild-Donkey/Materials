#include<bits/stdc++.h>
using namespace std;
/*
sigma (a[i]-(a-increase))^2  //�Ƶ����̣����е�increase ��������е� decrease 
sigma (a[i]-a+increase)^2
sigma (a[i]-a)^2+increase^2+2*(a[i]-a)*increase
 fang*len+ sigma increase^2+2*a[i]*increase-2*a*increase
fang*len+2*sum*increase+len*increase^2 -2*a*len*increase
*/
typedef double ld;
const int N=1e5+5; 
struct node{  //��ʾ����Ľṹ�� 
	ld tag,sum,fang,l,r,aver,len;  //fang ��ʾ���� 
	node() { tag=sum=fang=l=r=0;}
}tr[N<<2];
node none;   //���ù��캯������һ�������� 
ld Plus(node a,ld increase)   //�� sigma (a[j]-aver_a)^2 ��Ϊ sigma (a[j]-aver)^2 
{
	
	return a.fang*a.len+a.len*increase*increase;
}
node operator +(const node &a,const node &b)  //����ϲ� 
{
	node c;
	c.l=a.l;
	c.r=b.r;  //�������Ҷ˵�
			  //��������Կ�������������������㽻���ɣ������ʱǧ���д�� 
	c.sum=a.sum+b.sum;
	c.len=a.len+b.len;    //������Լ����䳤�� 
	c.aver=c.sum/c.len;   //����ƽ�� 
	ld increase1=a.aver-c.aver,increase2=b.aver-c.aver;  //�������ߵ�decrease 
	c.fang=Plus(a,increase1)+Plus(b,increase2);  //�ϲ����� 
	c.fang/=c.len;   //�ǵó� 
	c.tag=0;
	return c;
}
int n,m;
ld val[N];
#define ls (k<<1)
#define rs (k<<1|1)   //�궨��ʹ�����Ӽ�� 
void build(int k,int l,int r)  //���� 
{
	tr[k].l=l;
	tr[k].r=r;
	if(l==r)
	{
		tr[k].len=1;
		tr[k].sum=tr[k].aver=val[l];   //��ʼ��ƽ��ֵ��Ȼ���Լ��� 
		tr[k].tag=0;
		tr[k].fang=0;  //������ȻΪ0 
		return;
	}
	int mid=l+r>>1;
	build(ls,l,mid);
	build(rs,mid+1,r);
	tr[k]=tr[ls]+tr[rs];  //����ϲ� 
}
inline void Add(int k,ld v)  //��������ӣ������ 
{
	tr[k].tag+=v;  //���ӱ�� 
	tr[k].sum+=tr[k].len*v;
	tr[k].aver+=v;    //ƽ��������v
}
inline void pushdown(int k)  //�´���� 
{
	if(tr[k].tag==0) return;
	Add(ls,tr[k].tag);
	Add(rs,tr[k].tag);
	tr[k].tag=0;
}
inline void modify(int k,int x,int y,ld v)  //�����޸� 
{
	if(tr[k].l>y||tr[k].r<x) return;
	if(x<=tr[k].l&&tr[k].r<=y)
	return Add(k,v);
	pushdown(k);   //�ǵ��´���� 
	modify(ls,x,y,v);
	modify(rs,x,y,v);
	tr[k]=tr[ls]+tr[rs];
}
inline node query(int k,int x,int y)
{
	if(tr[k].l>y||tr[k].r<x) return none;  //����һ�������� 
	if(x<=tr[k].l&&tr[k].r<=y)
	return tr[k];  //����ά���õ����� 
	pushdown(k);
	return query(ls,x,y)+query(rs,x,y);   //�ϲ���� 
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++) scanf("%lf",&val[i]);
	build(1,1,n);
	for(int i=1;i<=m;i++)
	{
		int opt,x,y;ld v;
		scanf("%d",&opt);
		if(opt==1)   //��ò�Ҫ�� switch,�е�Σ�� 
		{
			scanf("%d%d%lf",&x,&y,&v);
			modify(1,x,y,v);
		}
		if(opt==2)
		scanf("%d%d",&x,&y),printf("%.4lf\n",query(1,x,y).aver);
		if(opt==3) scanf("%d%d",&x,&y),printf("%.4lf\n",query(1,x,y).fang);
	}
}
