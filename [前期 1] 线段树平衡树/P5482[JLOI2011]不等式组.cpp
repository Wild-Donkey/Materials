//分类思想+转化思想 
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdlib>

using namespace std;

const int N=1e6+10,M=1e5+10;

int n,C[N*2],c[N*2],top,kind[M],k[M],cnt;
//kind：哪一类不等式；	k：不等式合法需要的 k (k=(c-b)/a)；
//C/c：两类不等式（k < x | k > x） 
//cnt：恒成立不等式个数； 
bool used[M];//记录这个不等式是否被删除 
char s[20];

void modify(int x,int y,int t[])//不要问我为什么起这个名字，问就是lxl 
{
	x+=N;//防负数 
	for(;x<=2e6+10;x+=x&(-x))//lowbit(x)=x&(-x)
		t[x]+=y;
	return ;
}

int sum(int x,int t[])
{
	x+=N;
	int res=0;
	for(;x;x-=x&(-x))
		res+=t[x];
	return res;
}

int main()
{
// 	freopen("work.in","r",stdin); freopen("work.out","w",stdout);
	scanf("%d",&n);
	int x,y,z;
	while(n--)
	{
		while(1)
		{
			scanf("%s",s);//输入操作 
			if(s[0] == 'A' || s[0] == 'Q' || s[0] == 'D')
				break ;
		}
		if(s[0] == 'A')
		{
			scanf("%d%d%d",&x,&y,&z);
			if(!x)//x=0 不等式转化为 b > c 
			{
				if(y > z)
				{
					cnt++;
					kind[++top]=3;//表示恒成立 
				}
				else kind[++top]=0;//恒不成立
			}
			if(x > 0)//x > (c-b)/a
			{
				k[++top]=floor((z*1.0-y)/x);//下取整 
				kind[top]=1;//表示合法的 k < x
				//k < x 且k已经上溢  
				if(k[top] > 1e6) kind[top]=0;//表示恒不成立 
				else if(k[top] < -1e6)//k < x 且k已经下溢 
				{
					cnt++;//恒成立 
					kind[top]=3;
				}
				else modify(k[top],1,C);
			}
			if(x < 0)//同理，可类比上面理解
			{
				k[++top]=ceil((z*1.0-y)/x);
				kind[top]=2;
				if(k[top] < -1e6) kind[top]=0;
				else if(k[top] > 1e6)
				{
					cnt++;
					kind[top]=3;
				}
				else modify(k[top],1,c);
			}
		}
		if(s[0] == 'Q')
		{
			scanf("%d",&x);
			printf("%d\n",sum(x-1,C)+(sum(1e6,c)-sum(x,c))+cnt);
			//合法的(k < x) + 合法的(k > x) + 恒成立 
		}
		if(s[0] == 'D')//Delete
		{
			scanf("%d",&x);
			if(used[x]) continue ;
			used[x]=true;
			if(kind[x] == 3) cnt--;
			if(kind[x] == 1) modify(k[x],-1,C);
			if(kind[x] == 2) modify(k[x],-1,c);
		}
	}
// 	fclose(stdin); fclose(stdout);
	return 0;
}
