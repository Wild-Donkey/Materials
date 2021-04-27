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

void asum(int p,int x); //ά���͵���״����
ull qsum(int p);
void axor(int p,ull x); //ά�����͵���״����
ull qxor(int p);

int n,m,a[N],lsh[N<<2],tot,op[N],x[N],y[N]; //op��x��y���Ȱ�ѯ�ʴ�������������ɢ��
ull p[N<<2],pre[N<<2],sum[N],xsum[N]; //p���������pre���������ǰ׺���ͣ�������������״����

int main()
{
	int i,l,r,mid;
	
	n=read();
	m=read();
	
	for (i=1;i<=n;++i)
	{
		lsh[++tot]=a[i]=read();
		lsh[++tot]=a[i]+1; //��ɢ����ʱ���ֵ+1Ҳ�Ž�ȥ��������������ֵ��ɢ����Ҳ������
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
	
    p[0]=time(0); //~~��ӭ���������~~����汻�����Ҿͻ���÷ɭ��ת����..
	for (i=1;i<tot;++i)
	{
		p[i]=p[i-1]*1000000007+19260817; //���������
		pre[i]=pre[i-1]^p[i]; //��������е�ǰ׺����
	}
	
	for (i=1;i<=n;++i)
	{
		a[i]=lower_bound(lsh+1,lsh+tot,a[i])-lsh; //��ɢ��
		asum(i,a[i]); //��ʼ��ǰ׺��
		axor(i,p[a[i]]); //��ʼ��ǰ׺����
	}
	
	for (i=1;i<=m;++i)
	{
		if (op[i]==1)
		{
			y[i] = lower_bound(lsh + 1, lsh + tot, y[i]) - lsh;
			asum(x[i],y[i]-a[x[i]]); //����ǰ׺��
			axor(x[i],p[y[i]]^p[a[x[i]]]); //����ǰ׺����
			a[x[i]]=y[i]; //���µ����ֵ
		}
		else
		{
			mid=(qsum(y[i])-qsum(x[i]-1))/(y[i]-x[i]+1); //���l��r�������������������ֵ��ķ�Χ
			l=mid-(y[i]-x[i])/2;
			if ((y[i]-x[i])&1) r=mid+(y[i]-x[i])/2+1;
			else r=mid+(y[i]-x[i])/2;
			if (l<=0||r>=tot) puts("yuanxing");
			else if ((qxor(y[i])^qxor(x[i]-1))==(pre[r]^pre[l-1])) puts("damushen"); //�ж�ʵ�ʵ�����������ֵ���������Ƿ����
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
