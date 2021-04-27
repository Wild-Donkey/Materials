#include <iostream>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <algorithm>

using namespace std;

const int N=500010;

typedef unsigned long long ull;

int read()
{
	int out=0;
	char c;
	while (!isdigit(c=getchar()));
	for (;isdigit(c);c=getchar()) out=out*10+c-'0';
	return out;
}

void asum(int p,int x); //维护和的树状数组
ull qsum(int p);
void axor(int p,ull x); //维护异或和的树状数组
ull qxor(int p);

int n,m,a[N],lsh[N<<2],tot,op[N],x[N],y[N]; //op、x、y是先把询问存下来，方便离散化
ull p[N<<2],pre[N<<2],sum[N],xsum[N]; //p是随机数，pre是随机数的前缀异或和，后面两个是树状数组

int main()
{
	int i,l,r,mid;
	
	n=read();
	m=read();
	
	for (i=1;i<=n;++i)
	{
		lsh[++tot]=a[i]=read();
		lsh[++tot]=a[i]+1; //离散化的时候把值+1也放进去，这样不连续的值离散化后也不连续
	}
	
	for (i=1;i<=m;++i)
	{
		op[i]=read();
		x[i]=read();
		y[i]=read();
		if (op[i]==1)
		{
			lsh[++tot]=y[i];
			lsh[++tot]=y[i]+1;
		}
	}
	
	sort(lsh+1,lsh+tot+1);
	tot=unique(lsh+1,lsh+tot+1)-lsh;
	
    p[0]=time(0); //~~欢迎大家来卡我~~如果真被卡了我就换成梅森旋转好了..
	for (i=1;i<tot;++i)
	{
		p[i]=p[i-1]*1000000007+19260817; //生成随机数
		pre[i]=pre[i-1]^p[i]; //求随机序列的前缀异或和
	}
	
	for (i=1;i<=n;++i)
	{
		a[i]=lower_bound(lsh+1,lsh+tot,a[i])-lsh; //离散化
		asum(i,a[i]); //初始化前缀和
		axor(i,p[a[i]]); //初始化前缀异或和
	}
	
	for (i=1;i<=m;++i)
	{
		if (op[i]==1)
		{
			y[i] = lower_bound(lsh + 1, lsh + tot, y[i]) - lsh;
			asum(x[i],y[i]-a[x[i]]); //更新前缀和
			axor(x[i],p[y[i]]^p[a[x[i]]]); //更新前缀异或和
			a[x[i]]=y[i]; //更新单点的值
		}
		else
		{
			mid=(qsum(y[i])-qsum(x[i]-1))/(y[i]-x[i]+1); //算出l、r，代表如果区间连续，值域的范围
			l=mid-(y[i]-x[i])/2;
			if ((y[i]-x[i])&1) r=mid+(y[i]-x[i])/2+1;
			else r=mid+(y[i]-x[i])/2;
			if (l<=0||r>=tot) puts("yuanxing");
			else if ((qxor(y[i])^qxor(x[i]-1))==(pre[r]^pre[l-1])) puts("damushen"); //判断实际的区间异或和与值域内异或和是否相等
			else puts("yuanxing");
		}
	}
	
	return 0;
}

void asum(int p,int x)
{
	for (;p<=n;p+=(p&-p)) sum[p]+=x;
}
ull qsum(int p)
{
	ull out=0;
	for (;p;p-=(p&-p)) out+=sum[p];
	return out;
}

void axor(int p,ull x)
{
	for (;p<=n;p+=(p&-p)) xsum[p]^=x;
}
ull qxor(int p)
{
	ull out=0;
	for (;p;p-=(p&-p)) out^=xsum[p];
	return out;
}
