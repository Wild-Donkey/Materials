#include<cstdio>
#include<set>
#include<map>
#include<iostream>
#include<algorithm>
using namespace std;
const int N=6e5+50;
inline void rad(int &_){
    static char ch;
    while(ch=getchar(),ch<'0'||ch>'9');_=ch-48;
    while(ch=getchar(),ch<='9'&&ch>='0')_=_*10+ch-48;
}
int gcd(int u,int v){return v?gcd(v,u%v):u;}
int n,m,x,y,z,cnt,opt,las;
int a[N],pre[N],net[N];
map<int,int>mp;
set<int>s[N];
typedef set<int>::iterator IT;
struct node{
    int lval,rval,mx,mn,mnp,gc;
    node(){}
    node(int A,int B,int C,int D,int E,int F):lval(A),rval(B),mx(C),mn(D),mnp(E),gc(F){}
}r;
struct tree{
    node tre[N<<3];
    node update(node u,node v){
        return node(u.lval,v.rval,max(u.mx,v.mx),min(u.mn,v.mn),max(u.mnp,v.mnp),gcd(gcd(u.gc,v.gc),abs(u.rval-v.lval)));
    }
    void mktree(int now,int ls,int rs){
        if(ls==rs){
            tre[now]=(node){a[ls],a[ls],a[ls],a[ls],pre[ls],0};
            return;
        }
        int noww=now<<1,nrs=ls+rs>>1;
        mktree(noww,ls,nrs);
        mktree(noww|1,nrs+1,rs);
        tre[now]=update(tre[noww],tre[noww|1]);
    }
    void change(int now,int ls,int rs,int mb,node val){
        if(ls>mb||rs<mb)return;
        if(ls==rs){tre[now]=val;return;}
        int noww=now<<1,nrs=ls+rs>>1;
        change(noww,ls,nrs,mb,val);
        change(noww|1,nrs+1,rs,mb,val);
        tre[now]=update(tre[noww],tre[noww|1]);
    }
    node query(int now,int ls,int rs,int zuo,int you){
        if(ls>=zuo&&rs<=you)return tre[now];
        int noww=now<<1,nrs=ls+rs>>1;
        if(you<=nrs)return query(noww,ls,nrs,zuo,you);
        if(zuo>nrs)return query(noww|1,nrs+1,rs,zuo,you);
        return update(query(noww,ls,nrs,zuo,you),query(noww|1,nrs+1,rs,zuo,you));
    }
}t;
int main(){
    rad(n);rad(m);
    for(int i=1;i<=n;i++){
        rad(x);a[i]=x;
        if(mp.find(x)==mp.end())mp[x]=++cnt,s[cnt].insert(i);
        else{
            y=mp[x];
            z=*--s[y].end();
            pre[i]=z;
            net[z]=i;//记录每个数的前驱后继
            s[y].insert(i);//丢到set里
        }
    }
    t.mktree(1,1,n);
    for(int i=1;i<=m;i++){
        rad(opt);
        if(opt==1){
            rad(x);rad(y);
            x^=las;y^=las;
            net[pre[x]]=net[x];
            pre[net[x]]=pre[x];//修改前驱后继
            z=a[net[x]];
            t.change(1,1,n,net[x],node(z,z,z,z,pre[x],0));//修改后继在线段树内的信息
            s[mp[a[x]]].erase(x);//set中删除
            if(mp.find(y)==mp.end()){
                mp[y]=++cnt,s[cnt].insert(i);
                pre[x]=net[x]=0;
            }else{
                z=mp[y];
                IT it=s[z].lower_bound(x);//set的lower_bound找前驱后继
                if(it==s[z].begin()){//有后继无前驱
                    pre[x]=0;net[x]=*it;
                    pre[*it]=x;
                    t.change(1,1,n,*it,node(a[*it],a[*it],a[*it],a[*it],x,0));//修改后继在线段树内的信息
                }else if(it==s[z].end()){//有前驱无后继
                    --it;
                    pre[x]=*it;net[x]=0;
                    net[*it]=x;
                }else{//有前驱有后继
                    IT It=--it;++it;
                    pre[x]=*It;net[x]=*it;
                    net[*It]=x;pre[*it]=x;
                    t.change(1,1,n,*it,node(a[*it],a[*it],a[*it],a[*it],x,0));//修改后继在线段树内的信息
                }
            }
            a[x]=y;
            t.change(1,1,n,x,node(y,y,y,y,pre[x],0));//修改自身信息
        }else{
            rad(x);rad(y);rad(z);
            x^=las;y^=las;z^=las;
            r=t.query(1,1,n,x,y);
            if((r.mx-r.mn==z*(y-x))&&(r.gc==z||!r.gc)&&(r.mnp<x||!z))las++,puts("Yes");//这里注意一下
            else puts("No");
        }
    }
}
