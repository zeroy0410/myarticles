#### 题目

有一个长度为$N$的字符串$S[1…N]$，它仅由`C`和`T`两种字母组成。
现在有$Q$个查询，每个查询包含两个整数$L$和$R$，表示：设新字符串 $S'=S[L…R]$，至少在$S'$中要删除多少个字符，才能保证：对于$S'$的每一个前缀与每一个后缀，其`C`的数量都不小于`T`的数量。



#### 思路

##### 28pts

有一个显然的贪心，对于一段区间，先扫描前缀，遇到不合法的点就删掉，再扫描后缀，把不合法的点删掉，这样的方案就是最优的。

##### 100pts

考虑用数据结构优化贪心。

**考虑前缀**

首先将询问离线，从后往前循环，维护一个栈的结构，遇到`T`就将当前坐标入栈，遇到`C`就弹出栈顶元素，这样询问时，对于前缀有影响的点就是栈中的点。

**考虑后缀**

在扫描前缀的过程中，我们是不是可以顺便计算出后缀呢？

首先，继承`28pts`中思路，栈中的元素先忽略，那么不在栈中的元素，我们要找一个后缀**最不满足条件**的点。设$sum[x]$为$[x,n]$的`C的个数-T的个数`，那就是sum最小的点，满足它[^注1]就行了。

利用线段树实现这个功能。

首先将线段树的初值赋值为sum数组的值，然后当元素入栈的时候，我们要“忽略”这个元素，也就是将线段树$[1,i]$区间+1，出栈的时候再把这个元素的影响恢复。

查询$r$的时候，$[r,n]$中还对线段树有影响的点也应该忽略，所以再记录一下$[l,r]$内还在栈中的元素就行了。

具体细节见代码

#### 代码

```c++
#include<bits/stdc++.h>
#define M 500005
using namespace std;
int n,q;
char S[M];
int sum[M];
struct node{
	int mi,c;
	void Add(node x){
		mi=min(mi,x.mi);
		c+=x.c;
	}
};
struct YD_tree{
	struct tr{
		int l,r,mi,cnt,lzy;
		void reset(int x){
			lzy+=x;mi+=x;
		}
	}tree[M<<2];
	#define fa tree[p]
	#define lson tree[p<<1]
	#define rson tree[p<<1|1]
	void up(int p){
		fa.mi=min(lson.mi,rson.mi);
		fa.cnt=lson.cnt+rson.cnt;
	}
	void down(int p){
		if(fa.lzy){
			lson.reset(fa.lzy);
			rson.reset(fa.lzy);
			fa.lzy=0;
		}
	}
	void build(int l,int r,int p){
		fa.l=l;fa.r=r;fa.lzy=0;
		if(l==r){fa.mi=sum[l];return;}
		int mid=(l+r)>>1;
		build(l,mid,p<<1);
		build(mid+1,r,p<<1|1);
		up(p);
	}
	void Addsum(int l,int r,int c,int p){
		if(fa.l==l&&fa.r==r){
			fa.reset(c);
			return;
		}
		int mid=(fa.l+fa.r)>>1;
		down(p);
		if(mid>=r)Addsum(l,r,c,p<<1);
		else if(mid<l)Addsum(l,r,c,p<<1|1);
		else Addsum(l,mid,c,p<<1),Addsum(mid+1,r,c,p<<1|1);
		up(p);
	}
	void Addc(int x,int c,int p){
		if(fa.l==fa.r){
			fa.cnt+=c;
			return;
		}
		int mid=(fa.l+fa.r)>>1;
		down(p);
		if(mid>=x)Addc(x,c,p<<1);
		else Addc(x,c,p<<1|1);
		up(p);
	}
	node Query(int l,int r,int p){
		if(fa.l==l&&fa.r==r)return (node){fa.mi,fa.cnt};
		int mid=(fa.l+fa.r)>>1;
		down(p);
		if(mid>=r)return Query(l,r,p<<1);
		else if(mid<l)return Query(l,r,p<<1|1);
		else {
			node res=Query(l,mid,p<<1);
			res.Add(Query(mid+1,r,p<<1|1));
			return res;
		}
	}
}Tr;
struct que{int r,id;};
vector<que>wk[M];
int stk[M],top,ans[M];
int main(){
//	freopen("elections.in","r",stdin);
//	freopen("elections.out","w",stdout);
	scanf("%d%s%d",&n,S+1,&q);
	for(int i=1,l,r;i<=q;i++)scanf("%d%d",&l,&r),wk[l].push_back((que){r,i});
	for(int i=n;i>=1;i--)sum[i]=sum[i+1]+(S[i]=='T'?-1:1);//C比T多多少个（后缀） 
	Tr.build(1,n,1);
	for(int i=n;i>=1;i--){
		if(S[i]=='T'){
			stk[++top]=i;
			Tr.Addsum(1,i,1,1);
			Tr.Addc(i,1,1);
		}
		else if(top){
			int t=stk[top--];
			Tr.Addsum(1,t,-1,1);
			Tr.Addc(t,-1,1);
		}
		for(int j=0,sz=wk[i].size();j<sz;j++){
			int r=wk[i][j].r,id=wk[i][j].id;
			node e=Tr.Query(i,r,1);
			ans[id]=e.c;//当前区间有哪些点还没有被前缀抵消 
			e.mi-=top-e.c;//[r+1,n]还没被抵消的点要减去，变成最小后缀和 
			if(e.mi<sum[r+1])
				ans[id]+=-(e.mi-sum[r+1]);
		}
	}
	for(int i=1;i<=q;i++)printf("%d\n",ans[i]);
	return 0;
}
```



[^注1]: 把它后面的T都删掉