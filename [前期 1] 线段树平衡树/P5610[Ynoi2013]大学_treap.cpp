#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int NR=5e5+10;
const int MR=34e6+10;
int n,m; 
int mx=0;//找到a[i]中的最大值
int la=0;//强制在线，要^la 
int tot,len;//分别表示目前的总节点数以及建树的时候的长度 
int a[NR],q[NR];//分别表示读入的a[i]已经建树时的那个数组 
int rt[NR],son[MR][2];//rt[x]就是我解释中的rt，也就是那个能整除x的平衡树的根 
int val[MR],siz[MR];//val[u]表示u对应在a[i]数组的下标，siz表示fhq中的子树的结点数 
int re[MR],resiz;//re回收结点的数组，resiz是这个数组中都几个待回收的结点 
ll c[NR];//这个数树状数组的数组 
vector<int> ys[NR];//用一个vector统计因数 

int lowbit(int x){//树状数组标配lowbit 
	return x&(-x);
}

void add(int x,ll val)//树状数组单点修改函数 
{
	while(x<=n)
	{
		c[x]+=val;
		x+=lowbit(x);
	}
}

ll query(int x)//查询区间和 
{
	ll ans=0;
	while(x)
	{
		ans+=c[x];
		x-=lowbit(x);
	}
	return ans;
}

int newnode(int x)//新建一个结点 
{
	int u;
	if(resiz) u=re[resiz--];//如果有待回收的点，就废物利用 
	else u=++tot;//否则结点总是+1 
	val[u]=x;siz[u]=1;//初始化 
	return u;
}

void update(int x){//计算x的siz值 
	siz[x]=siz[son[x][0]]+siz[son[x][1]]+1;
}

int build(int l,int r)//用O(n)建树，否则可能会TLE 
{
	if(r<l) return 0;
	int mid=(l+r>>1);
	int u=newnode(q[mid]);//新建一个u结点 
	son[u][0]=build(l,mid-1);//建左子树 
	son[u][1]=build(mid+1,r);//建右子树 
	update(u);//计算u的siz 
	return u;
}

void split(int x,int &u,int &v,int k)//fhqtreap的分裂函数 
{
	if(!x){u=v=0;return;}
	if(val[x]<=k) split(son[u=x][1],son[x][1],v,k);
	else split(son[v=x][0],u,son[x][0],k);
	update(x);
}

int merge(int u,int v)//fhq的合并函数 
{
	if(!u||!v) return u|v;
	int root;
	if(rand()<rand()) son[root=u][1]=merge(son[u][1],v);
	else son[root=v][0]=merge(u,son[v][0]);
	update(root);
	return root;
}

void dfs(int u,int k)//处理操作1 
{
	if(!u) return;//如果到了叶子，直接return 
	if(son[u][0]) dfs(son[u][0],k);//如果它有左儿子，则将它左儿子里的点也除一下k 
	if(a[val[u]]%k==0)//如果能整除k 
	{
		add(val[u],-a[val[u]]+a[val[u]]/k);//直接改变一下这个值 
		a[val[u]]/=k;//除一下，方便之后判断是否能够整除k 
	}
	if(a[val[u]]%k==0) q[++len]=val[u];//如果仍然能整除，重新加入平衡树 
	if(son[u][1]) dfs(son[u][1],k);//如果它有右儿子，则将它右儿子里的点也除一下k 
	re[++resiz]=u;//待回收的点增加一个 
	son[u][0]=son[u][1]=0;//情况这些点 
	siz[u]=val[u]=0;
}

int read()
{
	int x=0,f=1;char ch=getchar();
	while(ch>'9'||ch<'0'){if(ch=='-')f=-1;ch=getchar();}
	while(ch<='9'&&ch>='0'){x=(x<<3)+(x<<1)+(ch^48);ch=getchar();}
	return x*f;
}
int main()
{
//	freopen("1.in","r",stdin);
//	freopen("1.out","w",stdout);
	srand(15402533);
	n=read(),m=read();
	for(int i=1;i<=n;i++)
	{
		a[i]=read();
		mx=max(mx,a[i]);
		add(i,a[i]);
	}
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j*j<=a[i];j++)
		{
			if(a[i]%j==0)
			{
				ys[j].push_back(i);
				if(j*j!=a[i]) ys[a[i]/j].push_back(i);
				//将因数加入这个vector 
			}
		}
	}
	for(int i=2;i<=mx;i++)
	{
		len=0;
		for(vector<int>::iterator it=ys[i].begin();it!=ys[i].end();it++)
			q[++len]=*it;
		rt[i]=build(1,len);
		//将vector里的元素建树 
	}
	for(int i=1;i<=m;i++)
	{
		int op=read(),l=read(),r=read();
		l^=la,r^=la;
		//强制在线 
		if(op==1)
		{
			int k=read();
			k^=la;
			if(k==1) continue;
			int a,b,c;
			split(rt[k],a,c,r);
			split(a,a,b,l-1);
			//分裂成三部分 
			len=0;
			dfs(b,k);
			//dfs修改 
			b=build(1,len);
			rt[k]=merge(merge(a,b),c);
			//合并 
		}
		else
		{
			ll ans=query(r)-query(l-1);
			printf("%lld\n",ans);
			//输出答案 
			la=ans;
		}
	}
	return 0;
}

