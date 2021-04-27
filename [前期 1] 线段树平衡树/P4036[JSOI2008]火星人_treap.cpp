#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>

typedef unsigned UInt;

int readInt() {
  int ans = 0, c, f = 1;
  while (!isdigit(c = getchar()))
    if (c == '-') f = -1;
  do ans = ans * 10 + c - '0';
  while (isdigit(c = getchar()));
  return ans * f;
}

const int N = 100050;
const int base = 31;

int n;
int root, ls[N], rs[N], key[N], val[N], len[N], cnt;
UInt hash[N], pbase[N];

void upd(int x) {
  len[x] = len[ls[x]] + len[rs[x]] + 1;
  hash[x] = (hash[ls[x]] * base + val[x]) * pbase[len[rs[x]]] + hash[rs[x]];
}

int Merge(int x, int y) {
  if (!x || !y) return x | y;
  int res;
  if (key[x] < key[y])
    ls[res = y] = Merge(x, ls[y]);
  else
    rs[res = x] = Merge(rs[x], y);
  upd(res);
  return res;
}

void Split(int x, int k, int &l, int &r) {
  if (k == len[x]) {
    l = x, r = 0;
  } else if (k == 0) {
    l = 0, r = x;
  } else if (k <= len[ls[x]]) {
    Split(ls[x], k, l, ls[x]);
    upd(r = x);
  } else {
    Split(rs[x], k - len[ls[x]] - 1, rs[x], r);
    upd(l = x);
  }
}

UInt Query(int l, int r) {
  int a, b, c;
  Split(root, r, a, c);
  Split(a, l, a, b);
  UInt ans = hash[b];
  root = Merge(Merge(a, b), c);
  return ans;
}

void Insert(int k, int x) {
  int ne = cnt++, a, b;
  key[ne] = rand();
  val[ne] = x;
  ls[ne] = rs[ne] = 0;
  upd(ne);
  Split(root, k, a, b);
  root = Merge(Merge(a, ne), b);
}

void Modify(int k, int x) {
  int a, b, c;
  Split(root, k + 1, a, c);
  Split(a, k, a, b);
  val[b] = x;
  upd(b);
  root = Merge(Merge(a, b), c);
}

void Init() {
  root = cnt++;
  pbase[0] = 1;
  for (int i = 1; i < N; ++i)
    pbase[i] = pbase[i - 1] * base;
}

char s[N];

int LCS(int a, int b) {
  int l = 0, r = n - std::max(a, b);
  while (l < r) {
    int mid = (l + r + 1) / 2;
    if (Query(a, a + mid) == Query(b, b + mid))
      l = mid;
    else
      r = mid - 1;
  }
  return l;
}

int main() {
  scanf("%s", s);
  Init();
  for (n = 0; s[n]; ++n)
    Insert(n, s[n] - 'a');
  int m = readInt();

  while (m--) {
    scanf("%s", s);
    if (*s == 'Q')
      printf("%d\n", LCS(readInt() - 1, readInt() - 1));
    else if (*s == 'R') {
      int x = readInt() - 1;
      scanf("%s", s);
      Modify(x, *s - 'a');
    } else {
      int x = readInt();
      scanf("%s", s);
      Insert(x, *s - 'a');
      ++n;
    }
  }
  return 0;
}
