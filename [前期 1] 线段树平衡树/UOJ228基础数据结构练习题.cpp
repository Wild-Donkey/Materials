#include<cstdio>
#include<cmath>
typedef long long i64;
const int N=1e5+7;
char ib[N*100],*ip=ib,ob[N*20],*op=ob;
int _(){
	int x=0;
	while(*ip<48)++ip;
	while(*ip>47)x=x*10+*ip++-48;
	return x;
}
void pr(i64 x){
	int ss[25],sp=0;
	do ss[++sp]=x%10;while(x/=10);
	while(sp)*op++=ss[sp--]+48;
	*op++=10;
}
int _l,_r;
i64 _a;
i64 min(i64 a,i64 b){return a<b?a:b;}
i64 max(i64 a,i64 b){return a>b?a:b;}
struct node{
	node*lc,*rc;
	int L,R,M,D;
	i64 mn,mx,s,a;
	void dn(){
		if(a){
			lc->add(a);
			rc->add(a);
			a=0;
		}
	}
	void up(){
		mn=min(lc->mn,rc->mn);
		mx=max(lc->mx,rc->mx);
		s=lc->s+rc->s;
	}
	void init(i64 x){
		mn=mx=s=x;
		a=0;
	}
	void add(i64 x){
		mn+=x,mx+=x;
		s+=x*D;
		a+=x;
	}
	void add(){
		if(_l<=L&&R<=_r){
			add(_a);
			return;
		}
		dn();
		if(_l<=M)lc->add();
		if(_r>M)rc->add();
		up();
	}
	void sqrt(){
		if(_l<=L&&R<=_r&&mx-mn<=1){
			int a=::sqrt(mx),b=::sqrt(mn);
			if(a-mx==b-mn)return add(a-mx);
		}
		dn();
		if(_l<=M)lc->sqrt();
		if(_r>M)rc->sqrt();
		up();
	}
	void sum(){
		if(_l<=L&&R<=_r){
			_a+=s;
			return;
		}
		dn();
		if(_l<=M)lc->sum();
		if(_r>M)rc->sum();
	}
}ns[N*2],*np=ns,*rt;
node*build(int L,int R){
	node*w=np++;
	w->L=L,w->R=R;
	w->D=R-L+1;
	if(L<R){
		int M=w->M=L+R>>1;
		w->lc=build(L,M);
		w->rc=build(M+1,R);
		w->up();
	}else w->init(_());
	return w;
}
int n,m;
int main(){
	fread(ib,1,sizeof(ib),stdin);
	n=_();m=_();
	rt=build(1,n);
	for(int i=0,o;i<m;++i){
		o=_(),_l=_(),_r=_();
		if(o==1){
			_a=_();
			rt->add();
		}else if(o==2){
			rt->sqrt();
		}else{
			_a=0;
			rt->sum();
			pr(_a);
		}
	}
	fwrite(ob,1,op-ob,stdout);
	return 0;
}
