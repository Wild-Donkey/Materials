#include<algorithm>
#include<set>
#include<vector>
#include<cctype>
#include<cstdio>
using namespace std;
inline int readint(){
	int x=0;
	bool f=0;
	char c=getchar();
	while(!isdigit(c)&&c!='-') c=getchar();
	if(c=='-'){
		f=1;
		c=getchar();
	}
	while(isdigit(c)){
		x=x*10+c-'0';
		c=getchar();
	}
	return f?-x:x;
}
const int maxn=5e5+5;
int n,m,w,a[maxn];
set<int> s[maxn];
typedef set<int>::iterator iter;
struct node{
	int l,r;
	node* ch[2];
	int mx;
	void pushup(){
		mx=max(ch[0]->mx,ch[1]->mx);
	}
	node(int l,int r):l(l),r(r),mx(0){
		if(l<r){
			int mid=l+(r-l)/2;
			ch[0]=new node(l,mid);
			ch[1]=new node(mid+1,r);
			pushup();
		}
	}
	void modify(int x,int k){
		if(l==r) mx=k;
		else{
			if(x<=ch[0]->r) ch[0]->modify(x,k);
			else ch[1]->modify(x,k);
			pushup();
		}
	}
	int query(int ql,int qr){
		if(ql<=l&&qr>=r) return mx;
		else{
			int ans=0;
			if(ql<=ch[0]->r) ans=max(ans,ch[0]->query(ql,qr));
			if(qr>=ch[1]->l) ans=max(ans,ch[1]->query(ql,qr));
			return ans;
		}
	}
}*rt;
int pre(int x){
	iter it1=s[a[x]].lower_bound(x),it2=s[w-a[x]].lower_bound(x);
	if(it2==s[w-a[x]].begin()) return 0;
	else if(it1==s[a[x]].begin()) return *--it2;
	else if(*--it1>*--it2) return 0;
	else return *it2;
}
int main(){
	#ifdef LOCAL
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	#endif
	n=readint();
	m=readint();
	w=readint();
	for(int i=1;i<=n;i++) a[i]=readint();
	rt=new node(1,n);
	for(int i=1;i<=n;i++){
		rt->modify(i,pre(i));
		s[a[i]].insert(i);
	}
	int cnt=0;
	while(m--){
		int op=readint();
		if(op==1){
			int x,k;
			x=readint();
			k=readint();
			vector<int> res;
			iter it=s[a[x]].upper_bound(x);
			if(it!=s[a[x]].end()) res.push_back(*it);
			it=s[w-a[x]].upper_bound(x);
			if(it!=s[w-a[x]].end()) res.push_back(*it);
			s[a[x]].erase(x);
			s[a[x]=k].insert(x);
			res.push_back(x);
			it=s[a[x]].upper_bound(x);
			if(it!=s[a[x]].end()) res.push_back(*it);
			it=s[w-a[x]].upper_bound(x);
			if(it!=s[w-a[x]].end()) res.push_back(*it);
			for(int i=0;i<(int)res.size();i++) rt->modify(res[i],pre(res[i]));
		}
		else{
			int l,r;
			l=readint()^cnt;
			r=readint()^cnt;
			if(rt->query(l,r)>=l){
				cnt++;
				printf("Yes\n");
			}
			else printf("No\n");
		}
	}
	return 0;
}
