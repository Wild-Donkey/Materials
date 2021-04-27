# [CF617E XOR and Favorite Number](https://www.luogu.com.cn/problem/CF617E)

## 分析

首先，这个题我们要快速的查找一段序列的值，显然用一次前缀和即可

之后，在莫队上那一个桶来记录一下之前扫到过的值即可

## 实现

虽然思路很简单，但是还是有几个要注意的地方

1. 在跑莫队查询的时候，记得将左端点减一再进行移动
2. 考虑 $a xor b \leq a+b$ ，所以对于桶的大小，要开 $2e6$ 
3. 本题的数据中有 $m=0$ 的情况，可以更改块长为 $\sqrt{n}$ ，或者特判一下

被 $m=0$ 的情况卡死 

![](https://cdn.jsdelivr.net/gh/BoringHacker/cdn/emojis/majsoul/maj-37.png)

## Code

```cpp
#include<iostream>
#include<cstdio>
#include<math.h>
#include<algorithm>
#include<cstring>
#include<string>
#include<queue>
#include<vector>
#define ll long long
#define ld long double

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

const ll maxn=2e6+10;
ll n,m,k,block,size,l=1,r,now;
ll sum[maxn],belong[maxn],cnt[maxn],ans[maxn];
struct node
{
	ll l,r,id;
} q[maxn];

inline bool cmp(node a,node b)
{
	return (belong[a.l]^belong[b.l]) ? (a.l<b.l) : ((belong[a.l]&1) ? (a.r<b.r) : (a.r>b.r));
}

inline void add(ll x)
{
	now+=cnt[sum[x]^k];
	cnt[sum[x]]++;
}

inline void del(ll x)
{
	cnt[sum[x]]--;
	now-=cnt[sum[x]^k];
}

int main(void)
{
	n=read(),m=read(),k=read();
	
	block=sqrt(n);
	size=ceil((double)(n/block));
	
	for(int v,i=1;i<=n;i++)
	{
		v=read();
		sum[i]=sum[i-1]^v;
	}
	
	for(int i=1;i<=size;i++)
	{
		for(int j=block*(i-1)+1;j<=block*i;j++)
		{
			belong[j]=i;
		}
	}	
	
	for(int i=1;i<=m;i++)
	{
		q[i].l=read()-1,q[i].r=read();
		q[i].id=i;
	}
	
	std::sort(q+1,q+m+1,cmp);
	
	for(int i=1;i<=m;i++)
	{
		ll ql=q[i].l,qr=q[i].r;
		
		while(l<ql) del(l++);
		while(l>ql) add(--l);
		while(r<qr) add(++r);
		while(r>qr) del(r--);
		
		ans[q[i].id]=now;
	}
	
	for(int i=1;i<=m;i++) printf("%lld\n",ans[i]);
	
	return 0;
}
```

ps : [二倍经验](https://www.luogu.com.cn/problem/P4462)