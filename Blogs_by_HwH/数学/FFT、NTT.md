
* 理论：<https://www.luogu.com.cn/blog/306957/qbxt-shuo-xue-day7-post>

# FFT 实现
## 蝴蝶操作

由
$$
A(\omega_n^k) = A_1(w_{\frac{n}{2}}^k) + \omega_n^k A_2(w_{\frac{n}{2}}^k)
\\
A(\omega_n^{k+\frac{n}{2}}) = A_1(w_{\frac{2}{n}}^k) - \omega_n^k A_2(w_{\frac{n}{2}}^k)
$$

那么我们可以不用每次都计算一次 $\omega_n^k A_2(w_{\frac{n}{2}}^k)$

直接将其记录，$O(1)$ 求出对应项即可

## 二进制反转

对于一个多项式原序列的下标的递归树

初始：$0,1,2,3,4,5,6,7$

递归结束： $0,4,2,6,1,5,3,7$

将上述结果利用二进制写出：初始时： $000,001,010,011,100,101,110,111$

递归结束： $000,100,010,110,001,101,011,111$

显然，最后的递归到的下标就是原下标的二进制反转形式

所以我们可以直接预处理每一位的二进制反转后的情况，在迭代实现的时候直接调用就行了

# Code
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<math.h>
#include<queue>
#include<cmath>
#include<climits>
#define ll int
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

const ll maxn=5e6+10;
const ld pi=acos(-1.0);
ll n,m,lim=1,cnt;
ll r[maxn];

struct complex
{
	ld x,y;
	complex(ld a=0,ld b=0){x=a,y=b;}
} a[maxn],b[maxn];

complex operator + (complex a,complex b)
{
	complex c;
	c.x=a.x+b.x;
	c.y=a.y+b.y;
	return c;
}

complex operator - (complex a,complex b)
{
	complex c;
	c.x=a.x-b.x;
	c.y=a.y-b.y;
	return c;
}

complex operator * (complex a,complex b)
{
	complex c;
	c.x=a.x*b.x-a.y*b.y;
	c.y=a.x*b.y+a.y*b.x;
	return c;
}

inline void fft(complex *s,ll flag)
{
	for(int i=0;i<lim;i++)
	{
		if(i<r[i]) std::swap(s[i],s[r[i]]);
	}
	
	for(int i=1;i<lim;i<<=1)
	{	
		complex w(cos(pi/i),flag*sin(pi/i));
		
		for(int R=i<<1,L=0;L<lim;L+=R)
		{
			complex x(1,0);
			
			for(int k=0;k<i;k++,x=x*w)
			{
				complex A=s[k+L],B=x*s[k+L+i];
				s[L+k]=A+B;
				s[L+k+i]=A-B;
			}
		}
	}
}

int main(void)
{
	n=read(),m=read();
	
	for(int i=0;i<=n;i++) a[i].x=read();
	for(int i=0;i<=m;i++) b[i].x=read();
	
	while(lim<=m+n)
	{
		lim<<=1;
		cnt++;
	}
	
	for(int i=0;i<lim;i++)
	{
		r[i]=(r[i>>1]>>1)|((i&1)<<(cnt-1));
	}
	
	fft(a,1);
	fft(b,1);
	
	for(int i=0;i<=lim;i++)
	{
		a[i]=b[i]*a[i];
	}
	
	fft(a,-1);
	
	for(int i=0;i<=n+m;i++)
	{
		printf("%d ",(int)(a[i].x/(1.0*lim)+0.5));
	}
	return 0;
}
```

# NTT 实现

与 FFT 同理

这里使用原根的概念，假设有一个质数 $p$ 恰好是 $a\times 2^k+1$ 的形式，其原根为 $g$

那么每次迭代求解的时候，只需要将 $\omega = g^{\frac{p-1}{n}}$ 即可

对于 IDFT 操作，直接将 $\omega$ 赋值为 $inv[g]^{\frac{p-1}{n}}$ 即可

此处 $inv[g]$ 表示 $g$ 的逆元

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

const ll mod=998244353,yg=3;
const ll maxn=5e6+10;
ll n,m,lim=1,cnt,yg_inv;
ll a[maxn],b[maxn],r[maxn];

inline ll ksm(ll a,ll b,ll p)
{
	ll ret=1;
	while(b)
	{
		if(b&1) ret=ret*a%p;
		a=a*a%p;
		b>>=1;
	}
	return ret%p;
}

inline void NTT(ll *s,ll flag)
{
	for(int i=0;i<lim;i++)
	{
		if(i<r[i]) std::swap(s[i],s[r[i]]);
	}
	
	for(ll mid=1;mid<lim;mid<<=1)
	{
		ll w=ksm(flag==1 ? yg : yg_inv,(mod-1)/(mid<<1),mod);
		
		for(ll R=(mid<<1),j=0;j<lim;j+=R)
		{
			ll x=1;
			
			for(ll k=0;k<mid;k++,x=x*w%mod)
			{
				ll A=s[k+j]%mod,B=x%mod*s[k+j+	mid]%mod;
				s[k+j]=(A+B)%mod;
				s[k+j+mid]=(A-B+mod)%mod;
			}
		}
	}
}

int main(void)
{
	n=read(),m=read();
	
	yg_inv=ksm(3ll,mod-2,mod);
	
	for(int i=0;i<=n;i++) a[i]=read();
	for(int i=0;i<=m;i++) b[i]=read();
	while(lim<=n+m)
	{
		lim<<=1;
		cnt++;
	}
	
	for(ll i=0;i<lim;i++)
	{
		r[i]=(r[i>>1]>>1)|((i&1)<<(cnt-1));
	}
	
	NTT(a,1);
	NTT(b,1);
	 
	for(int i=0;i<lim;i++) a[i]=a[i]*b[i]%mod;
	
	NTT(a,0);
	
	ll inv=ksm(lim,mod-2,mod);
	
	for(int i=0;i<=n+m;i++)
	{
		printf("%lld ",(a[i]*inv+mod)%mod);
	}
	
	return 0;
}
```