#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int NR=5e5+10;
const int MR=34e6+10;
int n,m; 
int mx=0;//�ҵ�a[i]�е����ֵ
int la=0;//ǿ�����ߣ�Ҫ^la 
int tot,len;//�ֱ��ʾĿǰ���ܽڵ����Լ�������ʱ��ĳ��� 
int a[NR],q[NR];//�ֱ��ʾ�����a[i]�Ѿ�����ʱ���Ǹ����� 
int rt[NR],son[MR][2];//rt[x]�����ҽ����е�rt��Ҳ�����Ǹ�������x��ƽ�����ĸ� 
int val[MR],siz[MR];//val[u]��ʾu��Ӧ��a[i]������±꣬siz��ʾfhq�е������Ľ���� 
int re[MR],resiz;//re���ս������飬resiz����������ж����������յĽ�� 
ll c[NR];//�������״��������� 
vector<int> ys[NR];//��һ��vectorͳ������ 

int lowbit(int x){//��״�������lowbit 
	return x&(-x);
}

void add(int x,ll val)//��״���鵥���޸ĺ��� 
{
	while(x<=n)
	{
		c[x]+=val;
		x+=lowbit(x);
	}
}

ll query(int x)//��ѯ����� 
{
	ll ans=0;
	while(x)
	{
		ans+=c[x];
		x-=lowbit(x);
	}
	return ans;
}

int newnode(int x)//�½�һ����� 
{
	int u;
	if(resiz) u=re[resiz--];//����д����յĵ㣬�ͷ������� 
	else u=++tot;//����������+1 
	val[u]=x;siz[u]=1;//��ʼ�� 
	return u;
}

void update(int x){//����x��sizֵ 
	siz[x]=siz[son[x][0]]+siz[son[x][1]]+1;
}

int build(int l,int r)//��O(n)������������ܻ�TLE 
{
	if(r<l) return 0;
	int mid=(l+r>>1);
	int u=newnode(q[mid]);//�½�һ��u��� 
	son[u][0]=build(l,mid-1);//�������� 
	son[u][1]=build(mid+1,r);//�������� 
	update(u);//����u��siz 
	return u;
}

void split(int x,int &u,int &v,int k)//fhqtreap�ķ��Ѻ��� 
{
	if(!x){u=v=0;return;}
	if(val[x]<=k) split(son[u=x][1],son[x][1],v,k);
	else split(son[v=x][0],u,son[x][0],k);
	update(x);
}

int merge(int u,int v)//fhq�ĺϲ����� 
{
	if(!u||!v) return u|v;
	int root;
	if(rand()<rand()) son[root=u][1]=merge(son[u][1],v);
	else son[root=v][0]=merge(u,son[v][0]);
	update(root);
	return root;
}

void dfs(int u,int k)//�������1 
{
	if(!u) return;//�������Ҷ�ӣ�ֱ��return 
	if(son[u][0]) dfs(son[u][0],k);//�����������ӣ������������ĵ�Ҳ��һ��k 
	if(a[val[u]]%k==0)//���������k 
	{
		add(val[u],-a[val[u]]+a[val[u]]/k);//ֱ�Ӹı�һ�����ֵ 
		a[val[u]]/=k;//��һ�£�����֮���ж��Ƿ��ܹ�����k 
	}
	if(a[val[u]]%k==0) q[++len]=val[u];//�����Ȼ�����������¼���ƽ���� 
	if(son[u][1]) dfs(son[u][1],k);//��������Ҷ��ӣ������Ҷ�����ĵ�Ҳ��һ��k 
	re[++resiz]=u;//�����յĵ�����һ�� 
	son[u][0]=son[u][1]=0;//�����Щ�� 
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
				//�������������vector 
			}
		}
	}
	for(int i=2;i<=mx;i++)
	{
		len=0;
		for(vector<int>::iterator it=ys[i].begin();it!=ys[i].end();it++)
			q[++len]=*it;
		rt[i]=build(1,len);
		//��vector���Ԫ�ؽ��� 
	}
	for(int i=1;i<=m;i++)
	{
		int op=read(),l=read(),r=read();
		l^=la,r^=la;
		//ǿ������ 
		if(op==1)
		{
			int k=read();
			k^=la;
			if(k==1) continue;
			int a,b,c;
			split(rt[k],a,c,r);
			split(a,a,b,l-1);
			//���ѳ������� 
			len=0;
			dfs(b,k);
			//dfs�޸� 
			b=build(1,len);
			rt[k]=merge(merge(a,b),c);
			//�ϲ� 
		}
		else
		{
			ll ans=query(r)-query(l-1);
			printf("%lld\n",ans);
			//����� 
			la=ans;
		}
	}
	return 0;
}

