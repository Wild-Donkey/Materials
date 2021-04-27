# NOI 嘉年华
* [P1973 [NOI2011] NOI 嘉年华](https://www.luogu.com.cn/problem/P1973)

yysy ， hack 数据是真的妙 qwq

~~（直接被卡自闭）~~

## 题意简述
给定两个会场，有 $n$ 个活动，你可以选择这个活动在哪个会场举办，但是不能有两个会场同时在举办活动

1. 活动可以任意举办，求活动相对较少的会场的活动数量的最大值
2. 若第 $i$ 个活动必须举办，求求活动相对较少的会场的活动数量的最大值

## 分析

首先，本题的时间并不直接参与计算，只是用作大小比较来确定活动的开始和结束顺序，而又 $0 \leq S_i \leq 10^9, 0 \leq T_i \leq 10^9$

设 $T$ 为举办到最晚时间的活动结束的时间

我们在设计状态的时候要考虑到两边会场不能同时存在一个活动在举办

则设 $pre[i][j]$ 表示在 $1$ ~ $i$ 时间内一个会场举办了 $j$ 个活动，另一个会场举办的最大的活动数量

设 $lst[i][j]$ 表示在 $i+1$ ~ $T$ 时间内一个会场举办 $j$ 个活动，另一个会场举办的最大的活动数量

同时，有 $val(i,j)$ 表示起始时间完全在 $[i,j]$ 时间段以内的活动的数量

那么对于 $pre[i][j]$ ，转移为
$$
pre[i][j] = \displaystyle \max_{k=1}^{i-1}\{pre[k][j]+val(k,i) , pre[k][j-val(k,i)]\}
$$

因为如果时间范围从 $[1,k]$ 变为 $[1,i]$ ，时间范围变大了，那么另一个会场可以举办的最多活动就会增多 $val(k,i)$ 个，而当 $j \geq val(k,i)$ 时，我们还需要比较在 $[1,k]$ 以内举办 $j-val(k,i)$ 个活动时另一会场可以举办的最多活动数量，用以更新当前的 $pre[i][j]$ 数组

而 $lst[i][j]$ 转移为
$$
lst[i][j] = \displaystyle \max_{k=i+1}^{T} \{lst[k][j]+val(i,k) , lst[k][j-val(i,k)] \}
$$

与 $pre[i][j]$ 同理

这样，我们就可以枚举在 $1$ ~ $T$ 时间内一个会场举办了 $i$ 个活动，另一会场的最大活动数量我们就可以通过 $pre[T][i]$ 得知

至此，第一个问题已经得到解决

考虑第二个问题，如果我必须要选择某一个活动，那么我可以枚举一段选择的区间，使得这段区间可以包含我必须要选择的那个活动

同时，我可以再枚举在 $1$ ~ $l$ 的时间内该会场举办 $x$ 个活动，在 $r$ ~ $T$ 的时间内举办 $y$ 个活动  

故有

$$
ans = \displaystyle \max\{\max\{ x+y+val(l,r),pre[l][x]+lst[r][y]\}\}
$$

但是，考虑一个问题，对于这种计算方式，如果一个活动被 $(l,r)$ 分割开，那么在计算的时候，这个活动两个会场是都不会去选择的，也就是说我们的 $f[Start_i][End_i]$ 只能够保证是局部最优解，而不一定是全局最优解

所以，我们需要使用一个数组 $f[l][r]$ 进行记录，然后在最后统计答案的时候枚举该活动 $i$ 的起止时间，统计满足条件的答案的最大值

复杂度：枚举 $l,r,x,y$ ，为 $O(n^4)$ 

考虑优化

对于一个 $x$ 和一个 $y$ ，当 $x$ 增大时， $pre[l][x]$ 是在减小的，而如果同时 $y$ 也在增大，那么 $lst[r][y]$ 也是减小的，那么显然， $pre[l][x]+lst[r][y]$ 必然就不会对 $f[l][r]$ 造成任何影响，那么我们就可以控制 $y$ 的大小，如果当前的 $y$ 已经不能继续更新答案，那么我们直接退出该循环，而当 $x$ 增大时，最优的 $y$ 一定不会比当前的 $y$ 更大，故直接从上一次的 $y$ 开始判断即可

这样，复杂度可以降为 $O(n^3)$

## Solution

1. 离散化：本题的最后一组 hack 数据中两个起始时间相同的活动，如果是一般离散化的话会导致在转移的时候被直接累加，可以进行特殊处理（或者是对状态的转移进行特殊处理）
2. 状态转移：在优化的时候直接初始化一个 $y=n$ ，在 $x$ 的循环枚举中进行判断，不断减小即可

## Code
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<math.h>
#include<queue>
#include<map>
#include<vector>
#include<climits>
#define ll int
#define ld double
using namespace std;

inline ll read()
{
	ll x=0,f=1;
	char ch=getchar();
	while(!isdigit(ch))
	{
		if(ch=='-') f=-1;
		ch=getchar();
	}
	while(isdigit(ch))
	{
		x=(x<<1)+(x<<3)+ch-'0';
		ch=getchar();
	}
	return x*f;
}

inline ll _min(ll a,ll b)
{
	return a<b ? a : b;
}

inline ll _max(ll a,ll b)
{
	return a>b ? a : b;
}

const ll inf=1e9+10;
const ll maxn=410;
ll n,tot,T,p,ans=-inf,y;
ll s[maxn],t[maxn],cnt[maxn][maxn],b[maxn];
ll pre[maxn][maxn],lst[maxn][maxn];
ll f[maxn][maxn];
struct node
{
	ld st,en;
	
	bool operator < (const node &x) const
	{
		return st+en < x.st + x.en;
	}
	
} a[maxn];
map<pair<ld,ld>,ll> mp;
vector<ld> e;

int main(void)
{
	memset(f,128,sizeof(f));
	scanf("%d",&n);
	
	for(int i=1;i<=n;i++)
	{
		scanf("%lf %lf",&a[i].st,&a[i].en);
		a[i].en+=a[i].st;

		if(mp.find(make_pair(a[i].st,a[i].en))!=mp.end())
		{
			a[i].en=a[i].en-mp[make_pair(a[i].st,a[i].en)]*0.001;
		}
		mp[make_pair(a[i].st,a[i].en)]++;
		e.push_back(a[i].st);
		e.push_back(a[i].en);
	}
	
	sort(e.begin(),e.end());
	e.erase(unique(e.begin(),e.end()),e.end());
	
	for(int i=1;i<=n;i++)
	{
		s[i]=lower_bound(e.begin(),e.end(),a[i].st)-e.begin()+1;
		t[i]=lower_bound(e.begin(),e.end(),a[i].en)-e.begin()+1;
	}
	
	T=e.size();

    //离散化，对于相同的区间进行加权

	for(int i=1;i<=T;i++)
	{
		for(int j=i+1;j<=T;j++)
		{
			for(int k=1;k<=n;k++) 
			{
				if(i<=s[k]&&t[k]<=j)
				{
					cnt[i][j]++;
				}
			}
		}
	}
	
	for(int i=1;i<=T;i++)
	{
		for(int j=1;j<=n;j++)
		{
			pre[i][j]=lst[i][j]=-inf;
		}
	}
	
	for(int i=1;i<=T;i++)
	{
		for(int j=0;j<=n;j++)
		{
			for(int k=1;k<i;k++)
			{
				pre[i][j]=_max(pre[i][j],pre[k][j]+cnt[k][i]);
				
				if(j>=cnt[k][i])
				{
					pre[i][j]=_max(pre[i][j],pre[k][j-cnt[k][i]]);
				}
			}
		}
	}
	
	for(int i=T;i;i--)
	{
		for(int j=0;j<=n;j++)
		{
			for(int k=i+1;k<=T;k++)
			{
				lst[i][j]=_max(lst[i][j],lst[k][j]+cnt[i][k]);
				if(j>=cnt[i][k]) lst[i][j]=_max(lst[i][j],lst[k][j-cnt[i][k]]);
			}
		}
	}
	
    //预处理 val(i,j) 、pre[i][j] 和 lst[i][j]

	for(int i=1;i<=n;i++)
	{
		ans=_max(ans,_min(i,pre[T][i]));
	}
	
	printf("%d\n",ans);
	
    //求解第一个问题

	for(int l=1;l<=T;l++)
	{
		for(int r=l+1;r<=T;r++)
		{
			ll nw=0,nxt=0;
			for(int y=n,x=0;x<=n;x++)
			{
				nw=_min(x+y+cnt[l][r],pre[l][x]+lst[r][y]);
				
				while(y&&(nw<=(nxt=_min(x+y-1+cnt[l][r],pre[l][x]+lst[r][y-1]))))
				{
					nw=nxt;
					y--;
				}
				
				f[l][r]=_max(f[l][r],_min(x+y+cnt[l][r],pre[l][x]+lst[r][y]));
			}
		}
	}
	
    //转移

	for(int i=1;i<=n;i++)
	{
		ans=0;
		
		for(int j=1;j<=s[i];j++)
		{
			for(int k=t[i];k<=T;k++)
			{
				ans=_max(ans,f[j][k]);
			}
		}
		
        //枚举寻找一个符合条件的全局最优解

		printf("%d\n",ans);
	}
	
	return 0;
}
```