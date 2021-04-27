#include <bits/stdc++.h>
#define LL long long
using namespace std;
const int N = 50005,D = 60;
inline int calcphi(int n){
	static int t,i,ans; t = n,ans = n;
	for (i = 2; i * i <= n; ++i) if (t % i == 0){
		while (!(t%i)) t /= i;
		ans = ans / i * (i-1);
	}
	if (t > 1) ans = ans / t * (t-1);
	return ans;
}
int P;
int c;
int p[D],cntp,c1[D][1<<15],c2[D][1<<15];
inline int Mo(LL n,int p){ return n >= p ? (n % p + p) : n; }
inline void init_c(){
	int i,j;
	for (i = 0; i <= cntp; ++i){
		c1[i][0] = c2[i][0] = 1;
		for (j = 1; j < 1<<15; ++j) c1[i][j] = Mo((LL)c1[i][j-1] * c,p[i]);
		c2[i][1] = Mo((LL)c1[i][(1<<15)-1] * c,p[i]);
		for (j = 2; j < 1<<15; ++j) c2[i][j] = Mo((LL)c2[i][j-1] * c2[i][1],p[i]);
	}
}
inline int power(int n,int i){ return Mo((LL)c1[i][n&32767]*c2[i][n>>15],p[i]); }
int n,m,a[N][D];
inline int calc(int x,int cnt,int i){
	if (!cnt) return Mo(x,p[i]); if (i == cntp) return c ? 1 : 0;
	return power(calc(x,cnt-1,i+1),i);
}
int sum[N<<2],mn[N<<2];
inline void up(int o){
	mn[o] = min(mn[o<<1],mn[o<<1|1]);
	sum[o] = sum[o<<1] + sum[o<<1|1];
	if (sum[o] >= P) sum[o] -= P; 
}
inline void Build(int o,int l,int r){
	mn[o] = 0; if (l == r){ sum[o] = a[l][0]; return; }
	int mid = l+r>>1; Build(o<<1,l,mid); Build(o<<1|1,mid+1,r); up(o);
}
int ll,rr;
inline void Add(int o,int l,int r){
	if (mn[o] > cntp) return;
	if (l == r){ ++mn[o],sum[o] = a[l][mn[o]]; return; } 
	if (ll <= l && rr >= r){ int mid = l+r>>1; Add(o<<1,l,mid); Add(o<<1|1,mid+1,r); up(o); return; }
	int mid = l+r>>1; if (ll <= mid) Add(o<<1,l,mid); if (rr > mid) Add(o<<1|1,mid+1,r); up(o);
}
int qans;
inline void Ask(int o,int l,int r){
	if (ll <= l && rr >= r){ qans = (qans + sum[o] >= P) ? (qans + sum[o] - P) : (qans + sum[o]); return; }
	int mid = l+r>>1; if (ll <= mid) Ask(o<<1,l,mid); if (rr > mid) Ask(o<<1|1,mid+1,r);
}
int main(){
	int i,j,op;
	cin >> n >> m >> P >> c;
	p[cntp = 0] = P; while (p[cntp] > 1) ++cntp,p[cntp] = calcphi(p[cntp-1]);
	init_c();
	for (i = 1; i <= n; ++i){
		cin >> a[i][0];
		for (j = 1; j <= cntp+1; ++j) a[i][j] = calc(a[i][0],j,0) % P;
		a[i][0] %= P;
	}
	Build(1,1,n);
	while (m--){
		cin >> op >> ll >> rr;
		if (op == 0) Add(1,1,n);
		else qans = 0,Ask(1,1,n),cout << qans << '\n';
	}
	return 0;
}
