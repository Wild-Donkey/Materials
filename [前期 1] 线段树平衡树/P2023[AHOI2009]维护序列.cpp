#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#define R(i, r, l) for (int _end = (l), i = (r); i >= _end; --i)
#define F(i, l, r) for (int _end = (r), i = (l); i <= _end; ++i)
typedef long long LL;
int readInt() { 
  int ans = 0, c; 
  while (!isdigit(c = getchar())); 
  do ans = ans * 10 + c - '0'; while (isdigit(c = getchar()));
  return ans;
}
const int N = 1000050;
LL A[N], P;
LL sumv[N * 4], addv[N * 4], mulv[N * 4];
#define _mul(o, x) mulv[o] = mulv[o] * (x) % P, addv[o] = addv[o] * (x) % P
#define _add(o, x) addv[o] = (addv[o] + (x)) % P
inline void upd(int o, int l, int r) {
  if (l == r) sumv[o] = (A[l] * mulv[o] % P + addv[o]) % P;
  else sumv[o] = ((sumv[o << 1] + sumv[o << 1 | 1]) * mulv[o] % P + addv[o] * (r - l + 1) % P) % P;
}
inline void pd(int o, int l, int r) {
  if (l == r)
    return;
  int lc = o << 1, rc = o << 1 | 1, mid = (l + r) >> 1;
  _mul(lc, mulv[o]);
  _mul(rc, mulv[o]);
  mulv[o] = 1;
  _add(lc, addv[o]);
  _add(rc, addv[o]);
  addv[o] = 0;
  upd(lc, l, mid);
  upd(rc, mid + 1, r);
}
int x, L, R;
void mul(int o, int l, int r) {
  if (L > r || l > R) return;
  if (L <= l && r <= R) {
    _mul(o, x);
    upd(o, l, r);
    return;
  }
  pd(o, l, r);
  int mid = (l + r) >> 1;
  mul(o << 1, l, mid);
  mul(o << 1 | 1, mid + 1, r);
  upd(o, l, r);
}
void add(int o, int l, int r) {
  if (L > r || l > R) return;
  if (L <= l && r <= R) {
    _add(o, x);
    upd(o, l, r);
    return;
  }
  pd(o, l, r);
  int mid = (l + r) >> 1;
  add(o << 1, l, mid);
  add(o << 1 | 1, mid + 1, r);
  upd(o, l, r);
}
LL query(int o, int l, int r) {
  if (L > r || l > R) return 0;
  if (L <= l && r <= R) {
    return sumv[o];
  }
  int mid = (l + r) >> 1;
  pd(o, l, r);
  return (query(o << 1, l, mid) + query(o << 1 | 1, mid + 1, r)) % P;
}
void build(int o, int l, int r) {
  addv[o] = 0, mulv[o] = 1;
  if (l != r) {
    int mid = (l + r) >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
  }
  upd(o, l, r);
}
int main() {
  int n = readInt();
  P = readInt();
  F(i, 1, n) A[i] = readInt() % P;
  build(1, 1, n);
  F(i, 1, readInt()) {
    int op;
    op = readInt(), L = readInt(), R = readInt();
    if (op == 3) {
      printf("%lld\n", query(1, 1, n));
    } else {
      x = readInt();
      op == 1 ? mul(1, 1, n) : add(1, 1, n);
    }
  }
  return 0;
}
