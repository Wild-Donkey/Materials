#include <bits/stdc++.h>
using namespace std;

# define Rep(i,a,b) for(int i=a;i<=b;i++)
# define _Rep(i,a,b) for(int i=a;i>=b;i--)
# define RepG(i,u) for(int i=head[u];~i;i=e[i].next)

typedef long long ll;

const int N=1e5+5;

template<typename T> void read(T &x){
   x=0;int f=1;
   char c=getchar();
   for(;!isdigit(c);c=getchar())if(c=='-')f=-1;
   for(;isdigit(c);c=getchar())x=(x<<1)+(x<<3)+c-'0';
    x*=f;
}

int n,m;
int son[N*10][2],siz[N*10],val[N*10],treap[N*10];
int bin[N*10],binsiz;
int root[N],tot;

struct node{
    int maxid,max;
}seg[N<<2];

struct chtholly_almeria{
    int bit[N];
    int lowbit(int o){
        return o&-o;
    }
    void add(int o,int x){
        for(;o<=n;o+=lowbit(o))bit[o]+=x;
    }
    int ask(int o){
    	if(o>n)o=n;
        int res=0;
        for(;o;o-=lowbit(o))res+=bit[o];
        return res;
    }
}Chtholly,Almeria;

struct ithea{
    int newnode(){
        if(!binsiz)return ++tot;
        else return bin[binsiz--];
    }
    void delnode(int x){
        bin[++binsiz]=x;
        siz[x]=val[x]=treap[x]=son[x][0]=son[x][1]=0;
    }
    void update(int x){
        siz[x]=siz[son[x][0]]+siz[son[x][1]]+1;
    }
    int merge(int u,int v){
        if(!u||!v)return u|v;
        int rt;
        if(treap[u]<treap[v])son[rt=u][1]=merge(son[u][1],v);
        else son[rt=v][0]=merge(u,son[v][0]);
        return update(rt),rt;
    }
    void split(int o,int &u,int &v,int k){
        if(!o){u=v=0;return;}
        if(val[o]<=k)split(son[u=o][1],son[o][1],v,k);
        else split(son[v=o][0],u,son[o][0],k);
        update(o);
    }
    void ins(int l,int r){
        int lft,rht;
        split(root[l],lft,rht,r);
        int u=newnode();
        treap[u]=rand(),siz[u]=1,val[u]=r;
        root[l]=merge(merge(lft,u),rht);
    }
    void del(int l,int r){
        int lft,mid,rht;
        split(root[l],lft,rht,r);
        split(lft,lft,mid,r-1);
        root[l]=merge(lft,rht);
        delnode(mid);
    }
    int max(int l){
        int u=root[l];
        while(son[u][1])u=son[u][1];
        return val[u];
    }
}Ithea;

struct nephren{
    # define lc (u<<1)
    # define rc (u<<1|1)
    node merge(node l,node r){
        if(l.max>r.max)return l;
        else return r;
    }
    void update(int u,int l,int r,int x){
        if(l==r){
            if(root[l])seg[u].max=Ithea.max(l),seg[u].maxid=l;
            else seg[u].max=seg[u].maxid=0;
            return;
        }   
        int mid=l+r>>1;
        if(x<=mid)update(lc,l,mid,x);
        else update(rc,mid+1,r,x);
        seg[u]=merge(seg[lc],seg[rc]);
    }
    node query(int u,int l,int r,int ql,int qr){
        if(l>=ql&&r<=qr)return seg[u];
        int mid=l+r>>1;
        if(qr<=mid)return query(lc,l,mid,ql,qr);
        if(ql>mid)return query(rc,mid+1,r,ql,qr);
        return merge(query(lc,l,mid,ql,qr),query(rc,mid+1,r,ql,qr));
    }
}Nephren;

int main()
{
    // freopen("testdata.in","r",stdin);
    srand(19260817);
    read(n),read(m);
    Rep(i,1,n){
        Chtholly.add(i,1);
        Ithea.ins(1,i),Nephren.update(1,1,n,1);
    }
    Rep(i,1,m){
        int opt,x,y;
        read(opt),read(x);
        if(opt==1){
            Almeria.add(x,1);
            while(1){
                node now=Nephren.query(1,1,n,1,x);
                if(now.max<x)break;
                Ithea.del(now.maxid,now.max),Nephren.update(1,1,n,now.maxid);
                int bel=now.max-now.maxid+1,l=now.maxid,r=now.max;
                while(Almeria.ask(r)-Almeria.ask(l-1)>0&&l<=n)
                    l+=bel,r+=bel,Chtholly.add(bel,1);
                if(l<=n)Ithea.ins(l,r),Nephren.update(1,1,n,l);
            }
        }
        else read(y),printf("%d\n",Chtholly.ask(y)-Chtholly.ask(x-1));
    }
    return 0;
}
