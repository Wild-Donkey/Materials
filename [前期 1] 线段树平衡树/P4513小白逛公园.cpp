#include<bits/stdc++.h>
using namespace std;
const int MAXN=500005;
int n,m,a[MAXN];
struct SegmentTree
{
    int l,r,sum,lmax,rmax,maxx;
    #define l(a) tree[a].l
    #define r(a) tree[a].r
    #define s(a) tree[a].sum
    #define lm(a) tree[a].lmax
    #define rm(a) tree[a].rmax
    #define m(a) tree[a].maxx
}tree[MAXN<<2];
int read()
{
    int f=1,re=0;
    char ch=getchar();
    while(!isdigit(ch)){if(ch=='-')f=-1;ch=getchar();}
    while(isdigit(ch)) re=(re<<3)+(re<<1)+ch-'0',ch=getchar();
    return f*re;
}
inline void update(int p)
{
    s(p)=s(p<<1)+s(p<<1|1);
    lm(p)=max(s(p<<1)+lm(p<<1|1),lm(p<<1));
    rm(p)=max(s(p<<1|1)+rm(p<<1),rm(p<<1|1));
    m(p)=max(max(m(p<<1),m(p<<1|1)),rm(p<<1)+lm(p<<1|1));
}
void build(int p,int ll,int rr)
{
    l(p)=ll,r(p)=rr;
    if(ll==rr)
    {
        s(p)=lm(p)=rm(p)=m(p)=a[ll];
        return ;
    }
    int mid=(l(p)+r(p))>>1;
    build(p<<1,ll,mid);
    build(p<<1|1,mid+1,rr);
    update(p);
}
void change(int p,int des,int num)
{
    if(l(p)==r(p))
    {
        s(p)=lm(p)=rm(p)=m(p)=num;
        return ;
    }
    int mid=(l(p)+r(p))>>1;
    if(mid>=des) change(p<<1,des,num);
    else change(p<<1|1,des,num);
    update(p);
}
SegmentTree ask(int p,int ll,int rr)
{
    if(ll<=l(p)&&r(p)<=rr) return tree[p];
    int mid=(l(p)+r(p))>>1;
    if(rr<=mid) return ask(p<<1,ll,rr);
    else if(ll>mid) return ask(p<<1|1,ll,rr);
    else
    {
        SegmentTree x=ask(p<<1,ll,rr),y=ask(p<<1|1,ll,rr),re;
        re.sum=x.sum+y.sum;
        re.lmax=max(x.sum+y.lmax,x.lmax);
        re.rmax=max(y.sum+x.rmax,y.rmax);
        re.maxx=max(max(x.maxx,y.maxx),x.rmax+y.lmax);
        return re;
    }
}
int main()
{
    n=read(),m=read();
    for(int i=1;i<=n;i++) a[i]=read();
    build(1,1,n);
    while(m--)
    {
        int opt=read();
        if(opt==1)
        {
            int x=read(),y=read();
            if(x>y) swap(x,y);
            SegmentTree ans=ask(1,x,y);
            printf("%d\n",ans.maxx);
        }
        else if(opt==2)
        {
            int x=read(),y=read();
            change(1,x,y);
        }
    }
    return 0;
}
