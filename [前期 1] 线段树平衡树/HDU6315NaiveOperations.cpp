#include<cstdio>
#include<algorithm>
#include<string.h>
 
using namespace std;
 
typedef long long ll;
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
const int INF = 0x3f3f3f3f;
const long long mod = 1e9 + 7;
const int maxn = 1e5 + 10;
 
double add[maxn<<2], sum[maxn<<2];
struct Node {
    int l, r;
    ll b;
    ll value;
    int lazy;
    int mi;
    int mid() { return (l+r)>>1;}
}tree[maxn<<2];
void pushdown(int rt)
{
    if(tree[rt].lazy > 0) {
        tree[rt<<1].lazy += tree[rt].lazy;
        tree[rt<<1|1].lazy += tree[rt].lazy;
        tree[rt<<1].mi -= tree[rt].lazy;
        tree[rt<<1|1].mi -= tree[rt].lazy;
        tree[rt].lazy = 0;
    }
}
void pushup(int rt)
{
    tree[rt].mi = min(tree[rt<<1].mi, tree[rt<<1|1].mi);
    tree[rt].value = tree[rt<<1].value + tree[rt<<1|1].value;
}
 
void build(int l, int r, int rt)
{
    tree[rt].l = l;
    tree[rt].r = r;
    tree[rt].lazy = 0;
    if(l == r) {
        scanf("%I64d", &tree[rt].b);
        tree[rt].value = 0;
        tree[rt].mi = tree[rt].b;
        return;
    }
    int m = tree[rt].mid();
    build(lson);
    build(rson);
    pushup(rt);
}
void update(int l, int r, int rt, ll add)
{
    if(tree[rt].l == l && tree[rt].r == r || add == 0) {
        tree[rt].lazy += add;
        tree[rt].mi -= add;
        if(tree[rt].mi > 0) {
            return;
        }
        else if(l != r) {
            pushdown(rt);
            update(l, tree[rt].mid(), rt<<1, 0);
            update(tree[rt].mid()+1, r, rt<<1|1, 0);
            pushup(rt);
            return;
        }
    }
    if(tree[rt].l == tree[rt].r) {
        if(tree[rt].mi <= 0) {
            tree[rt].mi = tree[rt].b;
            tree[rt].value++;
        }
        return;
    }
    pushdown(rt);
    int m = tree[rt].mid();
    if(r<=m) update(l, r, rt<<1, add);
    else if(l > m) update(l, r, rt<<1|1, add);
    else {
        update(l, m, rt<<1, add);
        update(m+1,r,rt<<1|1, add);
    }
    pushup(rt);
}
 
ll query(int l, int r, int rt)
{
    if(l == tree[rt].l&&r == tree[rt].r) {
        return tree[rt].value;
    }
    pushdown(rt);
    int m = tree[rt].mid();
    ll res = 0;
    if(r <= m) res+=query(l, r, rt<<1);
    else if(l > m) res+=query(l, r, rt <<1|1);
    else {
        res+=query(lson);
        res+=query(rson);
    }
    return res;
}
int main()
{
    int n, q;
    while(scanf("%d%d", &n, &q)!=EOF)
    {
        build(1, n, 1);
        char str[20];
        int l, r;
        for(int i = 0; i < q; i++)
        {
            scanf("%s%d%d", str, &l, &r);
            if(str[0] == 'a') {
                update(l, r, 1, 1);
            }
            else {
                printf("%I64d\n", query(l, r, 1));
            }
        }
    }
    return 0;

