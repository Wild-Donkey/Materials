const int INF = 0x3f3f3f3f;
int n, a[3000005], m;

#define GetSize(p) (p ? p->siz : 0)
#define GetSum(p) (p ? p->sum : 0)
#define GetCnt(p, x) (p ? p->cnt[x] : 0)
#define GetMlen(p, x) (p ? p->minlen[x] : INF)
struct Node {
    int l, r, sum, cnt[2], minlen[2], siz, rnd, ld[2], rd[2];
    bool val;
    Node *ls, *rs;
    Node() {
        val = l = r = sum = cnt[0] = cnt[1] = minlen[0] = minlen[1] = siz = rnd = ld[0] = ld[1] = rd[0] = rd[1] = 0;
        ls = rs = NULL;
    }
    inline void Update() {
        siz = GetSize(ls) + GetSize(rs) + 1;
        sum = GetSum(ls) + GetSum(rs) + (r - l + 1) * val;
        cnt[val] = GetCnt(ls, val) + GetCnt(rs, val) + 1;
        cnt[!val] = GetCnt(ls, !val) + GetCnt(rs, !val);
        minlen[val] = Min(Min(GetMlen(ls, val), GetMlen(rs, val)), r - l + 1);
        minlen[!val] = Min(GetMlen(ls, !val), GetMlen(rs, !val));
    }
    inline void PutTag(int lld0, int lld1, int rrd0, int rrd1) {
        ld[0] += lld0;
        ld[1] += lld1;
        rd[0] += rrd0;
        rd[1] += rrd1;
        minlen[0] += rrd0 - lld0;
        minlen[1] += rrd1 - lld1;
        sum += (rrd1 - lld1) * cnt[1];
        if (val) {
            l += lld1;
            r += rrd1;
        } else {
            l += lld0;
            r += rrd0;
        }
    }
    inline void Pushdown() {
        if (!ld[0] && !ld[1] && !rd[0] && !rd[1]) return;
        if (ls) ls->PutTag(ld[0], ld[1], rd[0], rd[1]);
        if (rs) rs->PutTag(ld[0], ld[1], rd[0], rd[1]);
        ld[0] = rd[0] = ld[1] = rd[1] = 0;
    }
};
Node nd[4200005];
int top;
struct FhqTreap {
    Node *_root;
    inline void New(Node *&p, int l, int r, int v) {
        if (l > r) return;
        p = &nd[top++];
        p->ld[0] = p->ld[1] = p->rd[0] = p->rd[1] = 0;
        p->l = l;
        p->r = r;
        p->val = v;
        p->sum = (r - l + 1) * v;
        p->cnt[v] = 1;
        p->minlen[v] = r - l + 1;
        p->minlen[!v] = INF;
        p->siz = 1;
        p->rnd = rand();
    }
    inline void split1(Node *now, int spk, Node *&lt, Node *&rt) {
        if (!now) {
            lt = rt = NULL;
            return;
        }
        now->Pushdown();
        if (now->r < spk) {
            lt = now;
            split1(now->rs, spk, now->rs, rt);
        } else {
            rt = now;
            split1(now->ls, spk, lt, now->ls);
        }
        now->Update();
    }
    inline void split2(Node *now, int spk, Node *&lt, Node *&rt) {
        if (!now) {
            lt = rt = NULL;
            return;
        }
        now->Pushdown();
        if (now->l <= spk) {
            lt = now;
            split2(now->rs, spk, now->rs, rt);
        } else {
            rt = now;
            split2(now->ls, spk, lt, now->ls);
        }
        now->Update();
    }
    inline Node* merge(Node *lt, Node *rt) {
        if (!lt) return rt;
        if (!rt) return lt;
        if (lt->rnd < rt->rnd) {
            lt->Pushdown();
            lt->rs = merge(lt->rs, rt);
            lt->Update();
            return lt;
        } else {
            rt->Pushdown();
            rt->ls = merge(lt, rt->ls);
            rt->Update();
            return rt;
        }
    }
    inline Node* LLeft(Node *p) {
        p->Pushdown();
        if (p->ls) return LLeft(p->ls);
        else return p;
    }
    inline Node* RRight(Node *p) {
        p->Pushdown();
        if (p->rs) return RRight(p->rs);
        else return p;
    } 
    inline void Assign(int l, int r, int val) {
        register Node *p1 = NULL, *p2 = NULL, *p3 = NULL;
        split1(_root, l, p1, p2);
        split2(p2, r, p2, p3);
        Node *lt = LLeft(p2), *rt = RRight(p2);
        if (lt->val == val) l = lt->l;
        else {
            Node *tmp = NULL;
            New(tmp, lt->l, l - 1, lt->val);
            p1 = merge(p1, tmp);
        }
        if (rt->val == val) r = rt->r;
        else {
            Node *tmp = NULL;
            New(tmp, r + 1, rt->r, rt->val);
            p3 = merge(tmp, p3);
        }
        Node *tmp = NULL;
        if (p1) {
            lt = RRight(p1);
            if (lt->val == val) {
                l = lt->l;
                split1(p1, lt->l, p1, lt);
            }
        }
        if (p3) {
            rt = LLeft(p3);
            if (rt->val == val) {
                r = rt->r;
                split1(p3, rt->r + 1, rt, p3);
            }
        }
        New(tmp, l, r, val);
        _root = merge(merge(p1, tmp), p3);
    }
    inline void Modify(int l, int r, int ld0, int ld1, int rd0, int rd1, bool _exp, bool _dir) {
        Node *p1 = NULL, *p2 = NULL, *p3 = NULL;
        split1(_root, l, p1, p2);
        split2(p2, r, p2, p3);
        Node *ll = LLeft(p2), *rr = RRight(p2);
        if (ll->val == (_exp ^ _dir)) {
            split2(p2, ll->l, ll, p2);
            p1 = merge(p1, ll);
            if (p2) ll = LLeft(p2);
            else {
                _root = merge(p1, p3);
                return;
            }
        }
        if (rr->val != (_exp ^ _dir)) {
            split1(p2, rr->l, p2, rr);
            p3 = merge(rr, p3);
            if (p2) rr = RRight(p2);
            else {
                _root = merge(p1, p3);
                return;
            }
        }
        p2->PutTag(ld0, ld1, rd0, rd1);
        _root = merge(merge(p1, p2), p3);
    }
    inline int Getsum(int l, int r) {
        Node *p1 = NULL, *p2 = NULL, *p3 = NULL;
        split1(_root, l, p1, p2);
        split2(p2, r, p2, p3);
        Node *ll = LLeft(p2), *rr = RRight(p2);
        register int ans = GetSum(p2) - (l - ll->l) * ll->val - (rr->r - r) * rr->val;
        _root = merge(merge(p1, p2), p3);
        return ans;
    }
    inline void Maintain(Node *p) {
        p->Pushdown();
        if (p->l > p->r) {
            Node *p1 = NULL, *p2 = NULL, *p3 = NULL;
            if (p->l == 1) {
                split1(_root, 1, p1, _root);
                return;
            } else if (p->r == n) {
                split2(_root, n, _root, p1);
                return;
            }
            split1(_root, p->r, p1, p2);
            split2(p2, p->l, p2, p3);
            Node *ll = LLeft(p2), *rr = RRight(p2);
            New(p2, ll->l, rr->r, ll->val);
            _root = merge(merge(p1, p2), p3);
            return;
        }
        if (Min(GetMlen(p->ls, 0), GetMlen(p->ls, 1)) == 0) {
            Maintain(p->ls);
            return;
        }
        if (Min(GetMlen(p->rs, 0), GetMlen(p->rs, 1)) == 0) {
            Maintain(p->rs);
        }
    }
    inline void Output(Node *p) {
        if (!p) return;
        Output(p->ls);
        printf("%d %d %d\n", p->l, p->r, p->val);
        Output(p->rs);
    }
};
FhqTreap tr;

