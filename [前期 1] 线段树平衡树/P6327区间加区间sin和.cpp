#include<bits/stdc++.h>
const int N=2e5+7;
struct cplx{
	double c,s;
	cplx(double _c=0,double _s=0):c(_c),s(_s){}
	cplx operator+(const cplx&w)const{return cplx(c+w.c,s+w.s);}
	cplx operator*(const cplx&w)const{return cplx(c*w.c-s*w.s,s*w.c+c*w.s);}
};
int _l,_r;
cplx _a;
double _ans;
struct node{
	node*lc,*rc;
	int L,R,M;
	cplx s,a;
	void up(){
		s=lc->s+rc->s;
	}
	void dn(){
		lc->mul(a);
		rc->mul(a);
		a=cplx(1,0);
	}
	void mul(const cplx&_a){
		s=s*_a;
		a=a*_a;
	}
	void mul(){
		if(_l<=L&&R<=_r)return mul(_a);
		dn();
		if(_l<=M)lc->mul();
		if(_r>M)rc->mul();
		up();
	}
	void sum(){
		if(_l<=L&&R<=_r)return void(_ans+=s.s);
		dn();
		if(_l<=M)lc->sum();
		if(_r>M)rc->sum();
	}
}ns[N*2],*np=ns;
int v0[N];
node*build(int L,int R){
	node*w=np++;
	w->L=L;w->R=R;
	w->a=cplx(1,0);
	if(L<R){
		int M=w->M=(L+R)>>1;
		w->lc=build(L,M);
		w->rc=build(M+1,R);
		w->up();
	}else w->s=cplx(cos(v0[L]),sin(v0[L]));
	return w;
}
int _(int l,int r){
	int x;
	assert(scanf("%d",&x)==1&&l<=x&&x<=r);
	return x;
}
int main(){
	int n=_(1,200000);
	for(int i=1;i<=n;++i)v0[i]=_(1,200000);
	node*rt=build(1,n);
	for(int q=_(1,200000);q;--q){
		int o=_(1,2);
		_l=_(1,n),_r=_(_l,n);
		if(o==1){
			int v=_(1,200000);
			_a=cplx(cos(v),sin(v));
			rt->mul();
		}else if(o==2){
			_ans=0;
			rt->sum();
			printf("%.1f\n",_ans);
		}
	}
	return 0;
}
