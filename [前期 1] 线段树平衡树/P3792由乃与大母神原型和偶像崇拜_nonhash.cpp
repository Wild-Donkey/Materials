#include <set>
#include <map>
#include <cstdio>
using namespace std;
const int Maxn=500000;
int n,m;
map<int,int> id_buc;
set<int> num[Maxn+5];
int sta[Maxn+5],top;
int a[Maxn+5],b[Maxn+5],pre[Maxn+5];
int id_tot;
int get_id(int x){
	if(id_buc.count(x)>0){
		return id_buc[x];
	}
	if(top>0){
		return id_buc[x]=sta[top--];
	}
	id_tot++;
	num[id_tot].insert(0);
	return id_buc[x]=id_tot;
}
struct Segment_Node_Val{
	int minn,maxn;
	int pre_max;
};
Segment_Node_Val make_SNV(int val,int pre){
	static Segment_Node_Val ans;
	ans.minn=ans.maxn=val;
	ans.pre_max=pre;
	return ans;
}
struct Segment_Node{
	Segment_Node_Val val;
}seg[1048577];
int mn(int a,int b){
	return a<b?a:b;
}
int mx(int a,int b){
	return a>b?a:b;
}
Segment_Node_Val push_up(Segment_Node_Val p,Segment_Node_Val q){
	Segment_Node_Val ans;
	ans.minn=mn(p.minn,q.minn);
	ans.maxn=mx(p.maxn,q.maxn);
	ans.pre_max=mx(p.pre_max,q.pre_max);
	return ans;
}
void build(int root,int left,int right){
	if(left==right){
		seg[root].val=make_SNV(a[left],pre[left]);
		return;
	}
	int mid=(left+right)>>1;
	build(root<<1,left,mid);
	build(root<<1|1,mid+1,right);
	seg[root].val=push_up(seg[root<<1].val,seg[root<<1|1].val);
}
void update(int root,int x,Segment_Node_Val v,int left=1,int right=n){
	if(left==right){
		seg[root].val=v;
		return;
	}
	int mid=(left+right)>>1;
	if(x<=mid){
		update(root<<1,x,v,left,mid);
	}
	else{
		update(root<<1|1,x,v,mid+1,right);
	}
	seg[root].val=push_up(seg[root<<1].val,seg[root<<1|1].val);
}
Segment_Node_Val query(int root,int l,int r,int left=1,int right=n){
	if(l<=left&&r>=right){
		return seg[root].val;
	}
	int mid=(left+right)>>1;
	if(r<=mid){
		return query(root<<1,l,r,left,mid);
	}
	if(l>mid){
		return query(root<<1|1,l,r,mid+1,right);
	}
	return push_up(query(root<<1,l,r,left,mid),query(root<<1|1,l,r,mid+1,right));
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++){
		scanf("%d",&a[i]);
		b[i]=get_id(a[i]);
		pre[i]=*(--num[b[i]].end());
		num[b[i]].insert(i);
	}
	build(1,1,n);
	int op,l,r;
	set<int>::iterator it;
	Segment_Node_Val tmp;
	for(int i=1;i<=m;i++){
		scanf("%d",&op);
		if(op==1){
			scanf("%d%d",&l,&r);
			it=num[b[l]].find(l);
			it++;
			if(it!=num[b[l]].end()){
				pre[(*it)]=pre[l];
				update(1,(*it),make_SNV(a[*it],pre[*it]));
			}
			it--;
			num[b[l]].erase(it);
			if(num[b[l]].size()==1){
				id_buc.erase(a[l]);
				sta[++top]=b[l];
			}
			a[l]=r;
			b[l]=get_id(a[l]);
			num[b[l]].insert(l);
			it=num[b[l]].find(l);
			it--;
			pre[l]=(*it);
			it++;
			it++;
			if(it!=num[b[l]].end()){
				pre[*it]=l;
				update(1,*it,make_SNV(a[*it],pre[*it]));
			}
			update(1,l,make_SNV(a[l],pre[l]));
		}
		else{
			scanf("%d%d",&l,&r);
			tmp=query(1,l,r);
			if(tmp.maxn-tmp.minn+1==r-l+1&&tmp.pre_max<l){
				puts("damushen");
			}
			else{
				puts("yuanxing");
			}
		}
	}
	return 0;
}
