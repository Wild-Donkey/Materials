# 四边形不等式

## Definition

设 $w(x,y)$ 为定义在整数集合上的二元函数，若对于定义域上的任意整数 $a,b,c,d$ ，其中 $a\leq b \leq c \leq d$ ，都有 $w(a,d)+w(b,c) \geq w(a,c) + w(b,d)$ ，则称 $w(x,y)$ 满足四边形不等式

## Theorem

设 $w(x,y)$ 为定义在整数集合上的二元函数，若对于定义域上的任意两个整数 $a,b$ ，满足 $a < b$ ，都有 $w(a,b) + w(a+1,b+1) \leq w(a+1,b) + w(a,b+1)$ ，则函数 $w(x,y)$ 满足四边形不等式

### Prove

对于 $a<c$ ，有 
$$
w(a,c+1) + w(a+1,c) \geq w(a,c) + w(a+1,c+1)
$$ 

对于 $a+1<c$ ，有 
$$
w(a+1,c+1)+w(a+2,c) \geq w(a+1,c) + w(a+1,c+2)
$$ 

两式相加，有
$$
w(a,c+1)+(a+2,c) \geq w(a,c)+w(a+2,c+1)
$$

因此，对于任意的 $a \leq b \leq c$ ，有
$$
w(a,c+1) + w(b,c) \geq (a,c) + w(b,c+1)
$$

同理，对于任意的 $a \leq b \leq c \leq d$ ，有
$$
w(a,d) + w(b,c) \geq w(a,c) + w(b,d)
$$

证毕 .

## 一维 DP 的四边形不等式优化

对于形如 $f[i] = \displaystyle \min_{0 \leq j < i} \{f[j] + val(j,i)\}$ 的一维 DP ，设对于 $f[i]$ ，记 $p[i]$ 为令 $f[i]$ 取得最小价值的 $j$ ，即 $p[i]$ 为 $f[i]$ 的最优决策

若 $g$ 对应的决策序列单调不降，则称 $f$ 具有 **决策单调性**

* ### Theorem
  
  在状态转移方程 $f[i] = \displaystyle \min_{0 \leq j < i} \{f[j] + val(j,i)\}$ 中，若函数 $val(i,j)$ 满足四边形不等式，则 $f$ 具有决策单调性

* ### Prove
  
  $\forall i \in [1,n] , \forall j \in [0,p[i]-1]$ ，由于 $p[i]$ 为当前 $f[i]$ 的最优决策，那么必然有
  $$
  f[p[i]] + val(p[i],i) \leq f[j] + val(j,i)
  $$

  又 $\forall i' \in [i+1,n]$ ，由于 $val(j,i)$ 满足四边形不等式，则
  $$
  val(j,i') + val(p[i],i) \geq val(j,i) + val(p[i],i')
  $$

  移项

  $$
  val(p[i],i') - val(p[i],i) \leq val(j,i') - val(j,i) 
  $$

  两不等式相加，即得
  $$
  f[p[i]] + val(p[i],i') \leq f[j] + val(j,i')
  $$

  含义为以 $p[i]$ 为 $f[i']$ 的最优决策，比以 $j \leq p[i]$ 作为 $f[i']$ 的决策更优

  那么，$f[i']$ 的最优决策不可能小于 $p[i]$ ，即 $p[i'] \geq p[i]$ ，故 $f$ 有决策单调性

对于一段序列，我们可以对 $p$ 维护一个队列，其中保存若干个三元组 $(l,r,j)$ ，表示 $p[l]$ ~ $p[r]$ 均为 $j$

对于每一次查询，我们进行如下操作：
1. 从队头开始扫，将小于 $p[1]$ ~ $p[i-1]$ 的部分弹出
   
2. 取队头三元组所保存的 $j$ ，对 $f[i]$ 进行更新
   
3. 从队尾开始扫，依次弹出，直到找到一个第一个三元组 $(l,r,j)$ ，满足 $f[i] + val(i,l) \leq f[j] + val(j,l)$ 
   
4. 同时，若对于该三元组 $(l,r,j)$ ，有 $f[j] + val(j,r) \leq f[i] + val(i,r)$ , 直接在队尾插入新的三元组 $(i,r+1,n)$
   
5. 否则，在 $[l,r]$ 上进行二分查找，求得在此之前决策 $j$ 更优，在此之后决策 $i$ 更优的位置 $pos$ 
   
6. 将三元组 $(i,pos,n)$ 插入队尾

### 例：诗人小 G
* [P1912](https://www.luogu.com.cn/problem/P1912)

设 $f[i]$ 为对前 $i$ 句诗进行排版的最小不协调度，记 $a[i]$ 为第 $i$ 句诗的长度，设 $s[i]$ 表示 $a[i]$ 的前缀和

则有
$$
f[i] = \displaystyle \min_{0 \leq j <i}\{f[j] + |s[i] - s[j] + (i-j-1) - L |^P\}
$$

那么记该式中的 $|s[i] - s[j] + (i-j-1) - L |^P$ 为 $val(j,i)$

则设 
$$
u = i - j + s[i] - s[j] - (L+1)
\\
v = i - (j+1) + s[i] - s[j+1] - (L+1)
$$

那么对于四边形不等式 
$$
val(j,i+1) + val(j+1,i) \geq val(j,i) + val(j+1,i+1)$$

仅需证明
$$
val(j+1,i) - val(j+1,i+1) \geq val(j,i) - val(j,i+1)
$$

即为
$$
|v|^P - |v + a[i+1] + 1|^P \leq |u|^P - |u+a[i+1]+1|^P
$$

设 $c = a[i+1]+1$

显然，有 $v > u$ ，那么设 $y = |x|^P - |x + c|^P$ ，只需要证明 $y$ 函数的单调递减的性质

记 $y'$ 为 $y$ 函数的一阶导数

P 为奇数， $x \in [-c ,0]$ 
   
则
$$
y = - x^P - (x+c)^P
\\
y' = -P x^{P-1} - P (x+c)^{P-1}
$$

由于 $P-1$ 为偶数，$x+c > 0$ ，故 $y' < 0$

有 $y$ 单调递减

对于其他 $5$ 种情况，使用求导的方式均可证明 $y$ 函数的单调性

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<math.h>
#include<queue>
#define ll long double
using namespace std;

const int maxn=1e5+10;
const ll inf=1e18;
ll L;
long long T,n,p,top;
long long head=1,tail=1;
ll f[maxn];
long long a[maxn];
long long sum[maxn];
long long sta[maxn];
long long pre[maxn];
char ch[maxn][32];
struct node
{
	long long l,r,j;
} s[maxn];

inline long long aabs(long long x)
{
	return x<0 ? -x : x; 
}

inline ll ksm(ll a,long long b)
{
	ll ret=1;
	while(b)
	{
		if(b&1) ret=ret*a;
		a=a*a;
		b>>=1;
	}
	return ret;
}

inline ll val(long long j,long long i)
{
	return f[j]+ksm(aabs((sum[i]-sum[j])-L+(i-j-1)),p);
}

inline void binary_s(long long x)
{
	long long l=s[tail].l,r=s[tail].r,c=s[tail].j;
	long long ret=r+1;
	while(l<=r)
	{
		int mid=(l+r)>>1;
		if(val(x,mid)<=val(c,mid)) r=mid-1,ret=mid;
		else l=mid+1;
	}
	if(ret!=s[tail].l) s[tail].r=ret-1;
	else tail--;
	if(ret<=n)
	{
		s[++tail].j=x;
		s[tail].l=ret;
		s[tail].r=n;
	}
	return ;
}

int main(void)
{
	cin>>T;
	
	while(T--)
	{
		cin>>n>>L>>p;
		top=0;
		head=1,tail=1;
		memset(pre,0,sizeof(pre));
		memset(sum,0,sizeof(sum));
		memset(ch,0,sizeof(ch));
		memset(s,0,sizeof(s));
		for(long long i=1;i<=n;i++)
		{
			scanf("%s",ch[i]);
			int len=strlen(ch[i]);
			sum[i]=sum[i-1]+len;
			f[i]=0;
		}
		f[0]=0;
		s[1].j=0;
		s[1].l=1;
		s[1].r=n;
		for(long long i=1;i<=n;i++)
		{
			while(head<tail&&s[head].r<=i-1) head++;
			s[head].l=i;
			f[i]=val(s[head].j,i);
			pre[i]=s[head].j;
			while(head<tail&&(val(s[tail].j,s[tail].l)>=val(i,s[tail].l))) tail--;
			binary_s(i);
		}
		if(f[n]>inf) 
		{
			printf("Too hard to arrange\n");
		}
		else 
		{
			printf("%lld\n",(long long)(f[n]+0.5));
			for(long long i=n;i;i=pre[i])
			{
				sta[++top]=i;
			}
			sta[++top]=0;
			for(long long i=top;i>1;i--)
			{
				for(long long j=sta[i]+1;j<sta[i-1];j++)
				{
					printf("%s ",ch[j]);
				}
				printf("%s",ch[sta[i-1]]);
				putchar(10);
			}
		}
		printf("--------------------");
		if(T>0)
		{
			putchar(10);
		}
	}
	return 0;
}
```

## 二维 DP 的四边形不等式优化

### Theorem 1

对于二维 DP 状态转移方程 $F[i,j] = \displaystyle \min_{i \leq k < j} \{ F[i,j]+F[k+1,j]+w(i,j)\}$ ，同时， $F[i,i] = w(i,i) = 0$ ，如果下面两个条件
$$
1.~ w 满足四边形不等式
\\
2.~ \forall a\leq b \leq c \leq d , w(a,d) \geq w(b,c) 
$$

成立

那么 $F$ 就满足四边形不等式

### Prove

对于 $i+1 = j$ 时
$$
F[i,j+1]+F[i+1,j] = F[i,i+2] + F[i+1,i+1] = F[i,i+2]
$$

若 $F[i,i+2]$ 的最优决策为 $i+1$ ，则
$$
F[i,i+2] = F[i,i+1]+F[i+2,i+2] + w(i,i+2) = w(i,i+1)+w(i,i+2)\\
w(i,i+1)+w(i,i+2) \geq w(i,i+1)+w(i+1,i+2) = F[i,i+1]+F[i+1,i+2]=F[i,j]+F[i+1,j+1]
$$

若 $F[i,i+2]$ 的最优决策是 $i$ ，则
$$
F[i,i+2] = F[i,i] + F[i+1,i+2] + w(i,i+2) = w(i+1,i+2)+w(i,i+2)
\\
w(i+1,i+2)+w(i,i+2) \geq w(i+1,i+2) + w(i,i+1) = F[i+1,j+1] + F[i,j]
$$

即 $F[i,j+1] + F[i+1,j] \geq F[i,j] + F[i+1,j+1]$

故当 $j-1 = i$ 时，有 $F[i,j]$ 满足四边形不等式

使用数学归纳法 ~~（数归 , yyds）~~

设当 $j-i <k$ 时，四边形不等式对 $F[i,j]$ 成立，那么对于 $j-i = k$ 时，设 $F[i,j+1]$ 以 $x$ 为最优决策， $F[i+1,j]$ 以 $y$ 为最优决策

不妨设 $i+1 \leq x \leq y$ 

由于 $x$ 和 $y$ 是对应状态的最优决策，那么
$$
F[i,j+1]+F[i+1,j] = F[i,x]+F[x+1,j+1]+w(i,j+1) + F[i+1,y]+F[y+1,j] + w(i+1,j) 
$$

对于 $F[i,j]$ 和 $F[i+1,j+1]$ ，$x$ 和 $y$ 是任意决策（不一定最优），故
$$
F[i,j]+F[i+1,j+1] \leq F[i,x] + F[x+1,j] + w(i,j) + F[i+1,y] + F[y+1,j+1] + w(i+1,j+1) 
$$

由于 $w$ 满足四边形不等式，则
$$
w(i+1,j)+w(i,j+1) \geq w(i,j) + w(i+1,j+1)
$$

根据归纳假设，有
$$
F[x+1,j+1] + F[y+1,j] \geq F[x+1,j] + F[y+1,j+1]
$$

那么结合上述四个不等式，即可得到
$$
F[i,j+1] + F[i+1,j] \geq F[i,j] + F[i+1,j+1]
$$

证毕 .

### Theorem 2

* 二维决策单调性定理

在状态转移方程 $F[i,j] = \displaystyle \min_{i \leq k < j} \{F[i,k]+F[k+1,j]+w(i,j)\} , F[i,i]=w(i,i)=0$ 中，设 $P[i,j]$ 为令 $F[i,j]$ 取到最小值的 $k$ 值

如果 $F$ ，满足四边形不等式，那么对于任意 $i<j$ ，有 $P[i,j-1] \leq P[i,j] \leq P[i+1,j]$ 

### Prove

记 $p = P[i,j]$ ，对于 $\forall i < k \leq p$ ，因为 $F$ 满足四边形不等式
$$
F[i,p] + F[i+1,k] \geq F[i,k] + F[i+1,p]
$$

移项得
$$
F[i+1,k] - F[i+1,p] \geq F[i,k] - F[i,p] 
$$

由于 $p$ 为当前状态的最优决策，那么
$$
F[i,k] + F[k+1,j] \geq F[i,p] + F[p+1,j]
$$

故

$(F[i+1,k]+F[k+1,j]+w(i+1,j)) - (F[i+1,p]+F[p+1,j]+w(i+1,j))$

$= (F[i+1,k]-F[i+1,p]) + (F[k+1,j]-F[p+1,j])$

$\geq (F[i,k]-F[i,p]) + (F[k+1,j] - F[p+1,j])$

$= (F[i,k]+F[k+1,j])-(F[i,p]+F[p+1,j])$

$\geq 0$

那么，对于 $F[i+1,j]$ ， $p$ 比任意的 $k \leq p$ 都更优，因此
$$
P[i+1,j] \geq P[i,j]
$$

同理，可证 $p[i,j-1] \leq P[i,j]$

证毕 .

### 例： 邮局
* [P4767](https://www.luogu.com.cn/problem/P4767)

设 $F[i,j]$ 代表前 $i$ 个村庄，建立了 $j$ 个邮局的最小价值 （第 $i$ 个村庄处不一定建立邮局）

则
$$
f[i][j] = \displaystyle \min_{ 0 \leq k <i} \{f[k][j-1] + val(k+1,i) \}
$$

$val(i,j)$ 代表第 $i$ 个村庄到第 $j$ 个村庄之间的最小价值和

这个式子中 $val(i,j)$ 的计算，显然，对于 $(i,j)$ 这一段区间，总的价值最小时，邮局应该选择放在其中位数处

那么要证明 $val(i,j)$ 满足四边形不等式，即需要证明
$$
val(i+1,j) + val(j+1,i) \geq val(i,j) + val(i+1,j+1)
$$

对于 $(i+1,j) ~,~ (i,j+1)$ 两段区间，其中位数都是 $\lfloor{\frac{i+j+1}{2}}\rfloor$ ，那么记其中位数为 $mid$

则有 $w(i+1,j)+w(i,j+1)=2*w(i+1,j)+a[mid]-a[i]+a[j+1]-a[mid]$

又
$$
w(i,j) \leq w(i+1,j) + a[mid] - a[i]
\\
w(i+1,j+1) \leq w(i+1,j) + a[j+1] - a[mid] 
$$

成立（分别考虑 $w(i,j) ~,~ w(i+1,j+1)$ 中的中位数的取值即可证明）

由四边形不等式性质定理，则
$$
\forall a \leq b \leq c \leq d , w(a,d) + w(b,c) \geq w(a,c) + w(b,d)
$$

成立

其次，$\forall a \leq b \leq c \leq d , w(a,d) \leq w(b,c)$

当 $a < b \leq c < d$ 时，区间 $(a,d)$ 的中位数是要在 $(b,c)$ 以内的，且 $(a,d)$ 区间是要涵盖 $(b,c)$ 区间的，相当于中位数变化不大，但是左右两侧均增添了新数，那么必然有 
$$
w(a,d) \leq w(b,c)
$$ 

当 $a = b \leq c < d$ 时，区间 $w(a,d)  = w(b,d)$ ，那么中位数的位置向右移动，相对而言，中位数左侧的求和值是增加的，而右侧，虽然中位数位置向右移动，但是右侧相对区间 $(b,c)$ 添加的数要更多一些，故总的值还是有
$$
w(a,d) = w(b,d) \leq w(b,c)
$$

同理，可证明 $a < b \leq c = d$ 亦有
$$
w(a,d) \leq w(b,c)
$$

故综上所述，由二维 DP 的四边形不等式优化的性质，可以得到 $f[i][j]$ 是满足决策单调性的

又由二维 DP 的四边形不等式优化的二位决策单调性定理，有
$$
P[i,j-1] \leq P[i,j] \leq P[i+1,j]
$$

那么对于每一个状态的转移，我们只需要枚举在 $P[i,j-1]$ 和 $P[i+1,j]$ 之间的状态寻找最优情况即可

* Code
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<math.h>
#include<queue>
#include<climits>
#define ll int

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

const ll maxn=3e3+10;
ll V,P;
ll a[maxn],s[maxn];
ll g[maxn][maxn];
ll f[maxn][maxn];

inline ll val(ll L,ll R)
{
	ll mid=(L+R+1)>>1;
	
	return a[mid]*(mid-(L-1))-(s[mid]-s[L-1])+(s[R]-s[mid])-a[mid]*(R-mid);
}

int main(void)
{
	V=read(),P=read();
	
	for(int i=1;i<=V;i++)
	{
		a[i]=read();
		s[i]+=s[i-1]+a[i];
	}
	
	memset(f,0x3f,sizeof(f));
	
	for(int i=1;i<=V;i++)
	{
		f[i][1]=val(1,i);
		g[i][1]=1;
	}
	
	for(int i=2;i<=P;i++)
	{
		g[V+1][i]=V;
		
		for(int j=V;j;j--)
		{
			for(int k=g[j][i-1];k<=g[j+1][i];k++)
			{
				if(f[j][i]>f[k][i-1]+val(k+1,j))
				{
					f[j][i]=f[k][i-1]+val(k+1,j);
					g[j][i]=k;
				}
			}
		}
	}
	
	printf("%d\n",f[V][P]);
	
	return 0;
}
```

## PS

对于利用四边形不等式证明决策单调性的题目，其 $val(i,j)$ 函数一般较为复杂，不是很容易分析，考试时可以打表观察其是否具有决策单调性，以节省时间

而且对于满足决策单调性的动态规划问题，其解决方式不一定是维护一个三元组，还有利用分治的思想解决的方案，可以灵活处理

QWQ