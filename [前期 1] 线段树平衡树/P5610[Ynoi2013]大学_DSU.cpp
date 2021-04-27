#include<algorithm>
#include<cstdio>
#define ll long long
#define MaxN 100500
#define MaxA 500500 
using namespace std;
ll read(){
  ll X=0;char ch=0;
  while(ch<48||ch>57)ch=getchar();
  while(ch>=48&&ch<=57)X=X*10+(ch^48),ch=getchar();
  return X;
}
int n;ll t[MaxN];
#define lbit(p) ((p)&-(p))
void add(int p,int x)
{while(p<=n){t[p]+=x;p+=lbit(p);}}
ll qry(int p){
  ll ret=0;
  while(p){ret+=t[p];p^=lbit(p);}
  return ret;
}
struct UFS{
  int *f;
  void Init(int n)
  {for (int i=0;i<n;i++)f[i]=i;}
  int find(int u)
  {return f[u]==u ? u : f[u]=find(f[u]);}
}T[MaxA];
int c[MaxA],*p[MaxA],_o[MaxN*405],*tp=_o,x[MaxN];
void div(int l,int r,int d)
{
  if (d==1)return ;
  l=lower_bound(p[d],p[d]+c[d],l)-p[d];
  r=upper_bound(p[d],p[d]+c[d],r)-p[d]-1;
  if (l>r)return ;
  for (int u=T[d].find(l);u<=r;){
    int to=p[d][u];
    if (x[to]%d==0){
      add(to,x[to]/d-x[to]);
      x[to]/=d;
    }if (u>=r)break;
    if (x[to]%d)
      u=T[d].f[u]=T[d].find(u+1);
    else u=T[d].find(u+1);
  }
}
int q,m;
int main()
{
  n=read();q=read();
  for (int i=1;i<=n;i++){
    add(i,x[i]=read());
    m=max(m,x[i]);
    c[x[i]]++;
  }
  for (int i=1;i<=m;i++)
    for (int j=i+i;j<=m;j+=i)
      c[i]+=c[j];
  for (int i=1;i<=m+1;i++)
    if (c[i]){
      p[i]=tp;tp+=c[i];
      T[i].f=tp;T[i].Init(c[i]);tp+=c[i];
      c[i]=0;
    }
  for (int i=1;i<=n;i++)
    for (int d=1;d*d<=x[i];d++)
      if (x[i]%d==0){
        p[d][c[d]++]=i;
        if (d*d!=x[i])
          p[x[i]/d][c[x[i]/d]++]=i;
     }
  ll l,r,x,las=0;
  for (int i=1,op;i<=q;i++){
    op=read();l=read();r=read();
    l^=las;r^=las;
    if (op==1)div(l,r,read()^las);
    else printf("%lld\n",las=qry(r)-qry(l-1));
  }
}

