#include<bits/stdc++.h>
using namespace std;
#define ull unsigned long long
int read() {
	int x=0, f=1; char ch=' ';
	while(!isdigit(ch)) {ch=getchar(); if(ch=='-') f=-1;}
	while(isdigit(ch)) x=(x<<3)+(x<<1)+(ch^48), ch=getchar();
	return x*f;
}
int n, root, size;
ull p[1000005];
struct AB{
	int val, f, son[2], sz;
	ull hs;
}s[1000005];
int get_son(int x) {
	return x==s[s[x].f].son[1];
}
void update(int x) {
	if(!x) return;
	s[x].sz=1;
	if(s[x].son[0]) s[x].sz+=s[s[x].son[0]].sz;
	if(s[x].son[1]) s[x].sz+=s[s[x].son[1]].sz;
	s[x].hs=s[s[x].son[0]].hs+(ull)s[x].val*p[s[s[x].son[0]].sz] + s[s[x].son[1]].hs*p[s[s[x].son[0]].sz+1];
}
void build(int l, int r, int x) {
	if(l>r) return;
	int mid=(l+r)>>1;
	if (mid >= x) s[x].son[1] = mid;
	else s[x].son[0] = mid;
	s[mid].f = x, s[mid].sz = 1;
	if (l==r) return;
	build(l, mid-1, mid), build(mid+1, r, mid);
	update(mid);
	return ;
}
void rotate(int x) {
	int fa=s[x].f, ffa=s[fa].f, wson=get_son(x);
	s[fa].son[wson]=s[x].son[wson^1];
	s[s[fa].son[wson]].f=fa;
	s[x].son[wson^1]=fa;
	s[fa].f=x;
	s[x].f=ffa;
	if(ffa) s[ffa].son[s[ffa].son[1]==fa]=x;
	update(fa);
	update(x);
} 
void splay(int x, int goal) {
	for(int fa;(fa=s[x].f)!=goal;rotate(x)) {
		if(s[fa].f!=goal) {
			rotate((get_son(x)==get_son(fa))?fa:x);
		}
	}
	update(x);
	if(goal==0) root=x;
}
int find(int x) {
	int now=root;
	while(1) {
		if (s[s[now].son[0]].sz+1 == x) return now;
		if (s[s[now].son[0]].sz+1 < x) x -= (s[s[now].son[0]].sz+1), now = s[now].son[1];
		else now = s[now].son[0];
	}
}
int get_hash(int l, int r) {
	int x=find(l), y=find(r+2);
	splay(x, 0), splay(y, root);
	return s[s[s[root].son[1]].son[0]].hs;
}
char ch[1000005];
int t;
int main() {
	scanf("%s", ch+1);
	n=strlen(ch+1);
	p[0]=1;
	for(int i=1;i<=n*2;i++) p[i]=p[i-1]*27ull;
	for(int i=2;i<=n+1;i++) {
		s[i].val=s[i].hs=ch[i-1]-'a'+1;
	}
	build(1, n+2, root);
	root = (n+3) >> 1, size=n+2;
	cin>>t;
	while(t--) {
		char type[3], ch[3];
		scanf("%s", type);
		if(type[0]=='Q') {
			int x=read(), y=read();
			if(x>y) swap(x, y);
			int l=0, r=size-y-1, mid;
			while(l<r) {
				mid=(l+r+1)>>1;
				if(get_hash(x, x+mid-1)==get_hash(y, y+mid-1)) l=mid;
				else r=mid-1;
			}
			printf("%d\n", l);
		}
		if(type[0]=='R') {
			int x=read();
			scanf("%s", ch);
			splay(find(x+1), 0);
			s[root].val=ch[0]-'a'+1;
			update(root);
		}
		if(type[0]=='I') {
			int u=read();
			scanf("%s", ch);
			int x=find(u+1), y=find(u+2);
			splay(x, 0); splay(y, x);
			s[s[root].son[1]].son[0] = ++size;
			s[size].f = s[root].son[1];
			s[size].val = s[size].hs = ch[0]-'a'+1;
			splay(size, 0);
		}
	}
}
