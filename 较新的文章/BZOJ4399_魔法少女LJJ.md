#### 题目

在森林中见过会动的树，在沙漠中见过会动的仙人掌过后，魔法少女LJJ已经觉得自己见过世界上的所有稀奇古怪的事情了  LJJ感叹道“这里真是个迷人的绿色世界,空气清新、淡雅,到处散发着醉人的奶浆味；小猴在枝头悠来荡去,好不自在；各式各样的鲜花争相开放,各种树枝的枝头挂满沉甸甸的野果；鸟儿的歌声婉转动听,小河里飘着落下的花瓣真是人间仙境”  SHY觉得LJJ还是太naive，一天，SHY带着自己心爱的图找到LJJ，对LJJ说：“既然你已经见识过动态树，动态仙人掌了，那么今天就来见识一下动态图吧”  
LJJ“要支持什么操作？”  SHY：“  
1.新建一个节点，权值为x。  
2.连接两个节点。  
3.一个节点a所属于的联通快内权值小于x的所有节点权值变成x。  
4.将一个节点a所属于的联通快内权值大于x的所有节点权值变成x。  
5.询问一个节点a所属于的联通块内的第k小的权值是多少。  
6.询问一个节点a所属联通快内所有节点权值之积与另一个节点b所属联通快内所有节点权值之积的大小。  
7.询问a所在联通快内节点的数量  
8.若两个节点a，b直接相连，将这条边断开。  
9.若节点a存在，将这个点删去。  ”  LJJ：“我可以离线吗？”  SHY：“可以，每次操作是不加密的，”  LJJ：“我可以暴力吗？”  SHY：“自重”  LJJ很郁闷，你能帮帮他吗 



#### 思路

一开始想：8,9根本搞不了。

仔细看题ing。。。

**c<=7!**

毒瘤出题人石锤。

接着就变成了一道码农题。

用并查集维护联通块之间的分与合。

对每一个联通块建一棵动态开点线段树。

对于3,4操作，先query区间中<=或>=x的有多少个，之后再用del函数把原来的这一部分删掉，再通通加到x上。

```cpp
void del(int l,int r,int &p,int L,int R){
	if(l==L&&r==R){p=cnt[p]=sum[p]=Lson[p]=Rson[p]=0;return;}
	int mid=(L+R)>>1;
	if(r<=mid)del(l,r,Lson[p],L,mid);
	else if(l>mid)del(l,r,Rson[p],mid+1,R);
	else {
		del(l,mid,Lson[p],L,mid);
		del(mid+1,r,Rson[p],mid+1,R);	
	}
	cnt[p]=cnt[Lson[p]]+cnt[Rson[p]];
	sum[p]=sum[Lson[p]]+sum[Rson[p]];
}
```



对于6操作，注意一个性质：if($a<b$) $log(a)<log(b)$,又$log(a*b)=log(a)+log(b)$

所以只要用一个sum数组记录每个联通块内权值的log和，就可以把乘化为加了。

#### 代码

```cpp
#include<bits/stdc++.h>
#define M 400005
using namespace std;
const double esp=1e-10;
struct que{
	int c,a,b;
}Q[M];
int B[M],bcnt;
int n,m,c;
int Lson[M*18],Rson[M*18],cnt[M*18],ver[M],tt;
double sum[M*18];
int fa[M];
int getfa(int x){return fa[x]==x?x:fa[x]=getfa(fa[x]);}
void Insert(int &p,int x,int d,int L,int R){
	if(!p)p=++tt;
	cnt[p]+=d;sum[p]+=(double)d*log2(B[x]);
	if(L==R)return;
	int mid=(L+R)>>1;
	if(x<=mid)Insert(Lson[p],x,d,L,mid);
	else Insert(Rson[p],x,d,mid+1,R);
}
int query(int p,int L,int R,int k){
	if(L==R)return L;
	int mid=(L+R)>>1;
	if(cnt[Lson[p]]<k)return query(Rson[p],mid+1,R,k-cnt[Lson[p]]);
	return query(Lson[p],L,mid,k);
}
int query_cnt(int l,int r,int p,int L,int R){
	if(l==L&&r==R)return cnt[p];
	int mid=(L+R)>>1;
	if(r<=mid)return query_cnt(l,r,Lson[p],L,mid);
	else if(l>mid)return query_cnt(l,r,Rson[p],mid+1,R);
	return query_cnt(l,mid,Lson[p],L,mid)+query_cnt(mid+1,r,Rson[p],mid+1,R);
}
void del(int l,int r,int &p,int L,int R){
	if(l==L&&r==R){p=cnt[p]=sum[p]=Lson[p]=Rson[p]=0;return;}
	int mid=(L+R)>>1;
	if(r<=mid)del(l,r,Lson[p],L,mid);
	else if(l>mid)del(l,r,Rson[p],mid+1,R);
	else {
		del(l,mid,Lson[p],L,mid);
		del(mid+1,r,Rson[p],mid+1,R);	
	}
	cnt[p]=cnt[Lson[p]]+cnt[Rson[p]];
	sum[p]=sum[Lson[p]]+sum[Rson[p]];
}
int merge(int x,int y){
	if(!x||!y)return (x|y);
	cnt[x]+=cnt[y];sum[x]+=sum[y];
	Lson[x]=merge(Lson[x],Lson[y]);
	Rson[x]=merge(Rson[x],Rson[y]);
	return x;
}
int main(){
	scanf("%d",&m);
	for(int i=1,c,a,b;i<=m;i++){
		scanf("%d",&c);
		if(c==1)scanf("%d",&a),B[++bcnt]=a;
		if(c==2)scanf("%d%d",&a,&b);
		if(c==3)scanf("%d%d",&a,&b),B[++bcnt]=b;
		if(c==4)scanf("%d%d",&a,&b),B[++bcnt]=b;
		if(c==5)scanf("%d%d",&a,&b);
		if(c==6)scanf("%d%d",&a,&b);
		if(c==7)scanf("%d",&a);
		Q[i].c=c;Q[i].a=a;Q[i].b=b;
	}
	sort(B+1,B+bcnt+1);
	bcnt=unique(B+1,B+bcnt+1)-B-1;
	for(int i=1;i<=m;i++){
		int c=Q[i].c,a=Q[i].a,b=Q[i].b;
		if(c==1){
			n++;fa[n]=n;
			a=lower_bound(B+1,B+bcnt+1,a)-B;
			Insert(ver[n],a,1,1,bcnt);
		}
		if(c==2){
			a=getfa(a);b=getfa(b);
			if(a==b)continue;
			merge(ver[a],ver[b]);
			fa[b]=a;
		}
		if(c==3){
			a=getfa(a);
			b=lower_bound(B+1,B+bcnt+1,b)-B;
			if(b==1)continue;
			int ct=query_cnt(1,b-1,ver[a],1,bcnt);
			Insert(ver[a],b,ct,1,bcnt);
			del(1,b-1,ver[a],1,bcnt);
		}
		if(c==4){
			a=getfa(a);
			b=lower_bound(B+1,B+bcnt+1,b)-B;
			if(b==bcnt)continue;
			int ct=query_cnt(b+1,bcnt,ver[a],1,bcnt);
			Insert(ver[a],b,ct,1,bcnt);
			del(b+1,bcnt,ver[a],1,bcnt);
		}
		if(c==5){
			a=getfa(a);
			printf("%d\n",B[query(ver[a],1,bcnt,min(b,cnt[ver[a]]))]);	
		}
		if(c==6){
			a=getfa(a);b=getfa(b);
			puts(sum[ver[a]]>sum[ver[b]]+esp?"1":"0");	
		}
		if(c==7)printf("%d\n",cnt[ver[getfa(a)]]);
	}
	return 0;
}
```



