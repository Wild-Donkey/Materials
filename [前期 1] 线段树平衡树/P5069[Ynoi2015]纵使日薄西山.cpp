#include<cstdio>
#include<set>
const int N=1e5+6;
typedef long long LL;
int n,a[N];
struct BIT{
	LL b[N];
	inline void add(int i,int x){for(;i<N;i+=i&-i)b[i]+=x;}
	inline LL ask(int i){LL x=0;for(;i;i^=i&-i)x+=b[i];return x;}
}odd,even;
std::set<int>s;
LL ans=0;
LL get(int x){
	if(x<1||x>n)return 0;
	auto it=s.find(x);
	if(*it==1){
		auto nxt=it;++nxt;
		if(a[*it]<a[*nxt]){
			if(*nxt&1)return odd.ask(*nxt-1)-odd.ask(*it-1);else
				return even.ask(*nxt-1)-even.ask(*it-1);
		}else return 0;
	}
	if(*it==n){
		auto pre=it;--pre;
		if(a[*pre]>=a[*it]){
			if(*pre&1)return odd.ask(*it)-odd.ask(*pre-1);else
				return even.ask(*it)-even.ask(*pre-1);
		}else return a[n];
	}
	auto pre=it,nxt=it;--pre,++nxt;
	if(a[*pre]>=a[*it]&&a[*it]<a[*nxt]){
		LL ret=0;
		if(*pre&1)ret+=odd.ask(*it-1)-odd.ask(*pre-1);else
			ret+=even.ask(*it-1)-even.ask(*pre-1);
		if(*nxt&1)ret+=odd.ask(*nxt-1)-odd.ask(*it);else
			ret+=even.ask(*nxt-1)-even.ask(*it);
		if((*pre&1)==(*it&1)&&(*nxt&1)==(*it&1))ret+=a[*it];
		return ret;
	}else return 0;
}
int main(){
	scanf("%d",&n);
	for(int i=1;i<=n;++i)
		scanf("%d",a+i),((i&1)?odd:even).add(i,a[i]);
	s.insert(-2),s.insert(-1),s.insert(n+2),s.insert(n+3);
	s.insert(1);
	for(int i=2;i<n;++i)
		if(a[i-1]<a[i]&&a[i]>=a[i+1]||a[i-1]>=a[i]&&a[i]<a[i+1])s.insert(i);
	s.insert(n);
	for(int i:s)
		ans+=get(i);
	int q;
	for(scanf("%d",&q);q--;){
		int x,y;
		scanf("%d%d",&x,&y);
		int nxt=*s.upper_bound(x),pre=*--s.lower_bound(x);
		ans-=get(nxt),ans-=get(pre);
		nxt=*s.upper_bound(nxt),pre=*--s.lower_bound(pre);
		ans-=get(nxt),ans-=get(pre);
		if(s.count(x))ans-=get(x),s.erase(x);
		s.erase(x-1),s.erase(x+1);
		if(x&1)odd.add(x,-a[x]),odd.add(x,y);else
			even.add(x,-a[x]),even.add(x,y);
		a[x]=y; 
		for(int i=x-1;i<=x+1;++i){
			if(i<1||i>n)continue;
			if(i==1||i==n)s.insert(i);else
				if(a[i-1]<a[i]&&a[i]>=a[i+1]||a[i-1]>=a[i]&&a[i]<a[i+1])s.insert(i);
		}
		for(auto l=s.find(pre),r=s.upper_bound(nxt);l!=r;++l)
			ans+=get(*l);
		printf("%lld\n",ans);
	}
	return 0;
}
