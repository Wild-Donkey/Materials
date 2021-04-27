#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
typedef long long LL;
const int N=1e5+5,md=1e9+7;
inline void upd(int&a){a+=a>>31&md;}
inline int pwr(int a,int b){
	int ret=1;
	for(;b;b>>=1,a=(LL)a*a%md)if(b&1)ret=(LL)ret*a%md;
	return ret;
}
struct info{
	int leftlen,rightlen,leftmul,rightmul;
	int sum,mul;
	int ans;bool oper;int sz;
};
inline info merge(const info&a,const info&b){
	info r;
	r.sz=a.sz+b.sz;
	upd(r.sum=a.sum+b.sum-md),r.mul=(LL)a.mul*b.mul%md;
	if(a.leftlen==a.sz&&a.oper==1){
		r.leftlen=a.sz+b.leftlen;
		r.leftmul=(LL)a.mul*b.leftmul%md;
	}else{
		r.leftlen=a.leftlen;
		r.leftmul=a.leftmul;
	}
	if(b.rightlen==b.sz&&a.oper==1){
		r.rightlen=b.sz+a.rightlen;
		r.rightmul=(LL)b.mul*a.rightmul%md;
	}else{
		r.rightlen=b.rightlen;
		r.rightmul=b.rightmul;
	}
	r.oper=b.oper;
	if(a.oper==1){
		r.ans=((LL)a.ans+b.ans-a.rightmul-b.leftmul+(LL)a.rightmul*b.leftmul+2*md)%md;
	}else upd(r.ans=a.ans+b.ans-md);
	return r;
}
struct Data{
	int leftlen,rightlen,leftmul,rightmul;
	// 左连乘区间长度，右连乘区间长度，左连乘积，右连乘积
	int sum,mul;// 区间和，区间乘积 
	int ans;bool oper;// 当前答案、最右边的符号 
	int tag_num;int tag_oper;// 区间修改数值、符号的标记 
	int sz,lim;// 区间长度，阈值大小 
	int*buc;// 记录小于等于阈值的多项式的出现次数 
	vector<int>vec;// 记录所有大于阈值的多项式，要保证有序 
	int left_num;//最左边的数 
	inline void makeLeaf(int _num,bool op){
		ans=sum=mul=leftmul=left_num=_num;
		leftlen=sz=lim=1;
		oper=op;
		if(op)rightlen=1,rightmul=_num;else rightmul=1,rightlen=0;
		buc=new int[2];
		buc[0]=0,buc[1]=1;
		tag_num=-1,tag_oper=-1;
	}
	inline void init(int len){
		sz=len,lim=sqrt(sz);
		buc=new int[lim+1];
		for(int i=0;i<=lim;++i)buc[i]=0;
		tag_num=-1,tag_oper=-1;
	}
	void merge(const Data&a,const Data&b){
		upd(sum=a.sum+b.sum-md),mul=(LL)a.mul*b.mul%md;
		left_num=a.left_num;
		if(a.leftlen==a.sz&&a.oper==1){//左边全是乘号
			leftlen=a.sz+b.leftlen;
			leftmul=(LL)a.mul*b.leftmul%md;
		}else{
			leftlen=a.leftlen;
			leftmul=a.leftmul;
		}
		if(b.rightlen==b.sz&&a.oper==1){// 右边全是乘号且左边最后一个为乘号 
			rightlen=b.sz+a.rightlen;
			rightmul=(LL)b.mul*a.rightmul%md;
		}else{
			rightlen=b.rightlen;
			rightmul=b.rightmul;
		}
		oper=b.oper;
		int sublen1=0,sublen2=0,addlen=999999;
		if(a.oper==1){
			ans=((LL)a.ans+b.ans-a.rightmul-b.leftmul+(LL)a.rightmul*b.leftmul+2*md)%md;
			sublen1=a.rightlen,sublen2=b.leftlen;addlen=sublen1+sublen2;
		}else upd(ans=a.ans+b.ans-md);
		for(int i=0;i<=lim;++i)buc[i]=0;
		vec.clear();
		if(sublen1&&sublen1<=lim)--buc[sublen1],sublen1=0;
		if(sublen2&&sublen2<=lim)--buc[sublen2],sublen2=0;
		if(addlen<=lim)++buc[addlen],addlen=999999;
		for(int i=1;i<=a.lim;++i)
		buc[i]+=a.buc[i];
		for(int i=1;i<=b.lim;++i)
		buc[i]+=b.buc[i];
		int it1=0,it2=0;
		for(;it1<a.vec.size()&&it2<b.vec.size();){
			int val=(a.vec[it1]<b.vec[it2])?a.vec[it1++]:b.vec[it2++];
			if(addlen<val){
				vec.push_back(addlen),addlen=999999;
			}
			if(val==sublen1){
				sublen1=0;continue;
			}
			if(val==sublen2){
				sublen2=0;continue;
			}
			if(val<=lim)++buc[val];else
			vec.push_back(val);
		}
		for(;it1<a.vec.size();){
			int val=a.vec[it1++];
			if(addlen<val){
				vec.push_back(addlen),addlen=999999;
			}
			if(val==sublen1){
				sublen1=0;continue;
			}
			if(val==sublen2){
				sublen2=0;continue;
			}
			if(val<=lim)++buc[val];else
			vec.push_back(val);
		}
		for(;it2<b.vec.size();){
			int val=b.vec[it2++];
			if(addlen<val){
				vec.push_back(addlen),addlen=999999;
			}
			if(val==sublen1){
				sublen1=0;continue;
			}
			if(val==sublen2){
				sublen2=0;continue;
			}
			if(val<=lim)++buc[val];else
			vec.push_back(val);
		}
		if(addlen<999999)
		vec.push_back(addlen);
	}
	void putTag_num(int val){
		sum=(LL)val*sz%md,mul=pwr(val,sz),left_num=val;
		leftmul=pwr(val,leftlen),rightmul=pwr(val,rightlen),ans=0;
		int nx=1;
		for(int i=1;i<=lim;++i){
			nx=(LL)nx*val%md;
			ans=(ans+(LL)buc[i]*nx)%md;
		}
		int lst=lim;
		for(int i=0;i<vec.size();++i){
			int p=vec[i];
			nx=(LL)nx*pwr(val,p-lst)%md,lst=p,upd(ans+=nx-md);
		}
		tag_num=val;
	}
	void putTag_op(bool op){
		oper=op;tag_oper=op;
		if(op==1){//*
			for(int i=0;i<=lim;++i)buc[i]=0;vec.clear();
			if(sz>lim)vec.push_back(sz);else ++buc[sz];
			leftlen=rightlen=sz;
			leftmul=rightmul=ans=mul;
		}else{//+
			for(int i=0;i<=lim;++i)buc[i]=0;vec.clear();
			buc[1]=sz;
			leftlen=1,rightlen=0;
			leftmul=left_num,rightmul=1;
			ans=sum;
		}
	}
	inline void pushdown(Data&ls,Data&rs){
		if(tag_num!=-1){
			ls.putTag_num(tag_num);
			rs.putTag_num(tag_num);
			tag_num=-1;
		}
		if(tag_oper!=-1){
			ls.putTag_op(tag_oper);
			rs.putTag_op(tag_oper);
			tag_oper=-1;
		}
	}
	inline info out(){
		return(info){
			leftlen,rightlen,leftmul,rightmul
			,sum,mul
			,ans,oper,sz
		};
	}
}d[N<<2];
int n,m;
int _num[N];int _op[N];
void build(int l,int r,int o){
	if(l==r){
		d[o].makeLeaf(_num[l],_op[l]);
		return;
	}
	d[o].init(r-l+1);
	const int mid=l+r>>1;
	build(l,mid,o<<1),build(mid+1,r,o<<1|1);
	d[o].merge(d[o<<1],d[o<<1|1]);
}
void modify_num(int l,int r,int o,const int&L,const int&R,const int&val){
	if(L<=l&&r<=R)
	d[o].putTag_num(val);else{
		d[o].pushdown(d[o<<1],d[o<<1|1]);
		const int mid=l+r>>1;
		if(L<=mid)modify_num(l,mid,o<<1,L,R,val);
		if(mid<R)modify_num(mid+1,r,o<<1|1,L,R,val);
		d[o].merge(d[o<<1],d[o<<1|1]); 
	}
}
void modify_op(int l,int r,int o,const int&L,const int&R,const bool&op){
	if(L<=l&&r<=R)
	d[o].putTag_op(op);else{
		d[o].pushdown(d[o<<1],d[o<<1|1]);
		const int mid=l+r>>1;
		if(L<=mid)modify_op(l,mid,o<<1,L,R,op);
		if(mid<R)modify_op(mid+1,r,o<<1|1,L,R,op);
		d[o].merge(d[o<<1],d[o<<1|1]); 
	}
}
info query(int l,int r,int o,const int&L,const int&R){
	if(L<=l&&r<=R)return d[o].out();
	d[o].pushdown(d[o<<1],d[o<<1|1]);
	const int mid=l+r>>1;
	if(L<=mid&&mid<R)return merge(query(l,mid,o<<1,L,R),query(mid+1,r,o<<1|1,L,R));
	if(L<=mid)return query(l,mid,o<<1,L,R);
	return query(mid+1,r,o<<1|1,L,R);
}
int main(){
	ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	cin>>n>>m;unsigned x;
	for(int i=1;i<=n;++i)cin>>x,_num[i]=x%md;
	for(int i=1;i<n;++i)cin>>_op[i];_op[n]=0;
	build(1,n,1);
	while(m--){
		int op,l,r;
		cin>>op>>l>>r;
		switch(op){
			case 1:{
				unsigned v;
				cin>>v;
				modify_num(1,n,1,l,r,v%md);
				break;
			}
			case 2:{
				int v;
				cin>>v;
				modify_op(1,n,1,l,r,v);
				break;
			}
			case 3:{
				info ret=query(1,n,1,l,r);
				cout<<ret.ans<<'\n';
				break;
			}
		}
	}
	return 0;
}
