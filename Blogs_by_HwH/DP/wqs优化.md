# WQS 优化 DP

WQS 优化又名凸优化，带权二分，

## 引例
给定一个长度为 $n$ 数列，将其划分为 $k$ 段，其划分的总价值为 $\displaystyle\sum_{i=1}^{k}{sum(i)^2}$ ，求最小的总价值，其中 $sum(i)$ 表示一段划分的权值

如果把分出来的段数看作为横坐标，其划分的价值为纵坐标，其点坐标表示为 $(x,f(x))$ ，平面上这些点所构成的图形会是一个下凸包，即两点之间的斜率单调不减，如下图所示

![](https://cdn.luogu.com.cn/upload/image_hosting/pyqiw1fk.png)

对于这种情况，我们可以考虑使用 wqs 二分优化

那么，我可以去找一条直线去和这个凸包相切，当且仅当直线与凸包相切时，有最小的纵截距

![](https://cdn.luogu.com.cn/upload/image_hosting/5lamlmqn.png)

如图，对于同一条直线，当它与 $A$ 点相切时，有纵截距最小

如果我们调整一条直线的斜率，使线的斜率增大，那么这条直线可以切到的点的横坐标就会增大，那么，两者是单调相关的

设在相切时，直线可以表示为 $y=kx+b$ ，其中纵截距 $b$ 记为 $g(x)$ 

移项，有 $g(x) = y - k * x$ ，即 $g(x) = f(x) - k*x$ ，因为斜率是要小于 $0$ 的，那么设 $k>0$ ，则表达式可以化为 $g(x) = f(x) + k * x$

观察这个式子，那么我要求一个 $g(x)$ ，即为选 $x$ 段，而每段物品都加上 $k$ 的价值之后的和

此时，我们的问题可以转化为对于没有选择段数限制的问题，求任意选择多少段得到的价值之和最小，那么这个问题可以轻松解决

所以，我们可以二分一个斜率 $k_x$ ，并对于这个 $k_x$ 做一遍没有限制的 DP ，如果最后选出来的段数要比所求的划分段数 $k$ 多，那么就需要重新调整 $k_x$ 的值，使其增大，同理，若最后选出来的段数要比所求的划分段数 $k$ 少，那么就需要重新调整 $k_x$ 的值，使其减小，最后可以不断逼近到点 $(x,f(x))$ ，求得答案

同时，考虑一个特殊情况，若凸包上出现三点（甚至更多）共线的情况，那么我们可以再对此情况进行特殊处理，添加条件，如答案为使得 $g(x)$ 最小化的最小的 $x$ 等限制

对于维护总价值最大的问题，此时图像就是一个上凸包，计算的时候类比以上分析求解即可

## 例题
* [P4983忘情](https://www.luogu.com.cn/problem/P4983)

显然，本题就是一个 WQS 优化的套路题，关键是对问题中的式子进行处理

对于式子的求解

$\frac{((\displaystyle\sum_{i=1}^{n}{x_i \overline{x}})+\overline{x})^2}{\overline{x}^2}$

$= \frac{((\displaystyle\sum_{i=1}^{n}{x_i \overline{x}})^2) + (2\overline{x}\displaystyle\sum_{i=1}^{n}{x_i \overline{x}}) + (\overline{x})^2}{\overline{x}^2}$

$=\frac{(\overline{x})^2(\displaystyle \sum_{i=1}^{n}{x_i})^2 + 2(\overline{x}^2)\displaystyle \sum_{i=1}^{n}{x_i} + (\overline{x}^2)}{\overline{x}^2}$

$= (\displaystyle \sum_{i=1}^{n}{x_i})^2 + 2\displaystyle \sum_{i=1}^{n}{x_i} + 1$

设 $sum[i]$ 为 $x[1],\cdots,x[i]$ 的前缀和，则原式可化为

$$
sum[n]^2 + 2 \times sum[n] + 1
$$

由于我们的 DP 只需要做没有限制的情况用于检验当前答案，那么可以设 $f[i]$ 表示当前到第 $i$ 个数，在此划分的最小总价值

则
$$
f[i] = \min\{f[k]+val(k+1,i)\}
$$

展开，处理：
$$
f[i] = f[k] + (sum[i]-sum[k])^2 + 2 \times (sum[i]-sum[k]) + 1
\\
f[i] = f[k] + sum[i]^2 + sum[k]^2 - 2 \times sum[i] \times sum[k] + 2 \times sum[i] - 2 \times sum[k] +1
\\
f[k] - 2\times sum[k] + sum[k]^2 = 2 \times sum[i] \times sum[i] + f[i] - sum[i]^2 -2 \times sum[i]
$$

显然可以使用斜率优化进行处理

我们每一次二分一个斜率 $mid$ ，代入检验，并且每次更新时将 $f[i]$ 加上 $mid$ ，根据其段数关系进行检验调整二分即可，记得在最后输出的时候将答案减去最后符合条件的 $m$ 个 $mid$ 的价值

### Code

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<math.h>
#include<queue>
#include<climits>
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

const ll maxn=1e5+10;
ll n,m,l,r,L,R=1e16,mid,ans;
ll a[maxn],f[maxn],s[maxn],q[maxn],d[maxn];
struct node
{
	ll x,val;
};

inline ll Y(ll x)
{
	return f[x]-2*s[x]+s[x]*s[x];
}

inline ld xl(ll x,ll y)
{
	return 1.0*(1.0*(Y(x)-Y(y))/(1.0*(s[x]-s[y])));
}

inline node judge(ll v)
{
	memset(d,0,sizeof(d));
	memset(f,0,sizeof(f));
	l=1,r=1;
	q[1]=0;
	for(int i=1;i<=n;i++)
	{
		while(l<r&&(xl(q[l],q[l+1])<=2*s[i])) l++;
		f[i]=f[q[l]]+(s[i]-s[q[l]]+1)*(s[i]-s[q[l]]+1)+v;
		d[i]=d[q[l]]+1;
		while(l<r&&(xl(q[r],q[r-1])>=xl(i,q[r]))) r--;
		q[++r]=i;
	}
	node ret;
	ret.x=d[n];
	ret.val=f[n];
	return ret;
}

int main(void)
{
	n=read(),m=read();
	
	for(int i=1;i<=n;i++)
	{
		a[i]=read();
		s[i]=s[i-1]+a[i];
	}
	
	while(L<R)
	{
		mid=(L+R)>>1;
		node x=judge(mid);
		if(x.x>=m)
		{
			ans=x.val-mid*m;
			L=mid+1;
		}
		else R=mid;
	}
	
	printf("%lld\n",ans);
	
	return 0;
}
```

## Some Other Questions

* [P4383 [八省联考2018]林克卡特树](https://www.luogu.com.cn/problem/P4383)
* [CF739E Gosha is hunting](https://www.luogu.com.cn/problem/CF739E)

## 参考
[[总结] wqs二分学习笔记](https://blog.csdn.net/weixin_30681121/article/details/95018953)

[关于WQS二分算法以及其一个细节证明](https://www.cnblogs.com/CreeperLKF/p/9045491.html)