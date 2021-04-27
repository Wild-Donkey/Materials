#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cstdlib>

typedef long long LL;

char getChar() {
  const int L = 10000000;
  static char buf[L], *p = buf, *end = buf;
  if (p == end) {
    if (feof(stdin)) return -1;
    end = buf + fread(p = buf, 1, L, stdin);
  }
  return *(p++);
}

int readInt() {
  int ans = 0, c, f = 1;
  while (!isdigit(c = getChar()))
    if (c == '-') f = -1;
  do ans = ans * 10 + c - '0';
  while (isdigit(c = getChar()));
  return ans * f;
}

const int N = 500050;

int ch[N][2], siz[N], key[N], val[N];
bool revv[N], hset[N];
int setv[N], sumv[N], maxl[N], maxr[N], maxv[N];
int root, pool[N], top;

inline void rev(int o) {
  std::swap(maxl[o], maxr[o]);
  revv[o] ^= 1;
}

inline void set(int o, int v) {
  if (o) {
    hset[o] = true;
    setv[o] = v;
    maxv[o] = std::max(v, sumv[o] = siz[o] * v);
    maxl[o] = maxr[o] = std::max(0, sumv[o]);
  }
}

inline void upd(int o) {
  int lc = ch[o][0], rc = ch[o][1];
  siz[o] = 1 + siz[lc] + siz[rc];
  sumv[o] = val[o] + sumv[lc] + sumv[rc];
  maxl[o] = std::max(maxl[lc], sumv[lc] + val[o] + maxl[rc]);
  maxr[o] = std::max(maxr[rc], sumv[rc] + val[o] + maxr[lc]);
  maxv[o] = std::max(std::max(maxv[lc], maxv[rc]),
                      val[o] + maxl[rc] + maxr[lc]);
}

inline void pushd(int o) {
  if (revv[o]) {
    revv[o] = false;
    rev(ch[o][0]);
    rev(ch[o][1]);
    std::swap(ch[o][0], ch[o][1]);
  }
  if (hset[o]) {
    hset[o] = false;
    val[o] = setv[o];
    set(ch[o][0], setv[o]);
    set(ch[o][1], setv[o]);
  }
}

void Del(int x) {
  if (!x) return;
  pool[top++] = x;
  Del(ch[x][0]);
  Del(ch[x][1]);
}

int Merge(int x, int y) {
  if (!x || !y) return x | y;
  pushd(x);
  pushd(y);
  if (key[x] > key[y]) {
    ch[x][1] = Merge(ch[x][1], y);
    upd(x);
    return x;
  } else {
    ch[y][0] = Merge(x, ch[y][0]);
    upd(y);
    return y;
  }
}

void Split(int x, int k, int &l, int &r) {
  if (k == 0) return void((l = 0, r = x));
  if (k >= siz[x]) return void((l = x, r = 0));
  pushd(x);
  if (siz[ch[x][0]] >= k) {
    Split(ch[x][0], k, l, ch[x][0]);
    upd(r = x);
  } else {
    Split(ch[x][1], k - siz[ch[x][0]] - 1, ch[x][1], r);
    upd(l = x);
  }
}

void Insert(int k, int o) {
  int l, r;
  Split(root, k, l, r);
  root = Merge(l, Merge(o, r));
}

void Remove(int k, int tot) {
  int l, m, r;
  Split(root, k - 1, l, m);
  Split(m, tot, m, r);
  root = Merge(l, r);
  Del(m);
}

void Rev(int k, int tot) {
  int l, m, r;
  Split(root, k - 1, l, m);
  Split(m, tot, m, r);
  rev(m);
  root = Merge(l, Merge(m, r));
}

void Set(int k, int tot, int v) {
  int l, m, r;
  Split(root, k - 1, l, m);
  Split(m, tot, m, r);
  set(m, v);
  root = Merge(l, Merge(m, r));
}

int QuerySum(int k, int tot) {
  int l, m, r;
  Split(root, k - 1, l, m);
  Split(m, tot, m, r);
  int res = sumv[m];
  root = Merge(l, Merge(m, r));
  return res;
}

inline int MaxSum() { return maxv[root]; }

int n, k[N], A[N];

int Build(int l, int r) {
  if (r < l) return 0;
  int m = l;
  for (int i = l; i <= r; ++i)
    if (k[i] > k[m]) m = i;
  int o = pool[--top];
  val[o] = A[m];
  key[o] = rand();
  ch[o][0] = Build(l, m - 1);
  ch[o][1] = Build(m + 1, r);
  revv[o] = hset[o] = 0;
  upd(o);
  return o;
}

void Insert(int posi) {
  for (int i = 0; i < n; ++i) k[i] = rand();
  Insert(posi, Build(0, n - 1));
}

int main() {
  maxv[0] = -1000000000;
  for (int i = 1; i < N; ++i) pool[top++] = i;
  n = readInt();
  int m = readInt();
  for (int i = 0; i < n; ++i)
    A[i] = readInt();
  Insert(0);
  while (m--) {
    char c;
    while (isspace(c = getChar()));
    if (c == 'M') {
      getChar();
      c = getChar();
    }
    while (!isspace(getChar()));
    if (c == 'I') {
      int posi = readInt();
      n = readInt();
      for (int i = 0; i < n; ++i)
        A[i] = readInt();
      Insert(posi);
    } else if (c == 'D') {
      int posi = readInt(), tot = readInt();
      Remove(posi, tot);
    } else if (c == 'K') {
      int posi = readInt(), tot = readInt();
      Set(posi, tot, readInt());
    } else if (c == 'R') {
      int posi = readInt(), tot = readInt();
      Rev(posi, tot);
    } else if (c == 'G') {
      int posi = readInt(), tot = readInt();
      printf("%d\n", QuerySum(posi, tot));
    } else
      printf("%d\n", MaxSum());
  }
}
