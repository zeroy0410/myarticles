---
title: HDU5877 Weak Pair
date: 2018-08-30 13:08:31
tags: [树上问题,dfs作差]
---

#### 题目
在一棵有根树上求出有多少个点对（u，v），满足 u是v的祖先且a[u]*a[v] ≤k.

#### 思路1（归并树 | 离线+树状数组）
秒敲归并树，此题刨去树的部分和上次的super mario本质上是一个模板。所以，由此扩展出的还有一种离线+树状数组的写法，本质上是一样的，此处不再赘述。
<!--more-->
```c++
#include<bits/stdc++.h>
#define M 100005
#define LL long long
#define fa tree[p]
#define lson tree[p<<1]
#define rson tree[p<<1|1]
#define pus(haha,x) haha.num.push_back(x)
#define nb fa.num.begin()
#define ne fa.num.end()
using namespace std;
int T,n;
LL m;
vector<int>G[M];
LL ans=0;
LL A[M];
int root;
bool mark[M];
int tt=0,L[M],R[M],ln[M];
struct node{
	int l,r;
	vector<int>num;
}tree[M<<2];
void up(int p){
	int i=0,j=0;
	while(i<lson.num.size()&&j<rson.num.size()){
		if(lson.num[i]<rson.num[j])pus(fa,lson.num[i++]);
		else pus(fa,rson.num[j++]);
	}
	while(i<lson.num.size())pus(fa,lson.num[i++]);
	while(j<rson.num.size())pus(fa,rson.num[j++]);
}
void build(int l,int r,int p){
	fa.l=l;fa.r=r;fa.num.clear();
	if(l==r){
		pus(fa,A[ln[l]]);
		return;
	}
	else {
		int mid=l+r>>1;
		build(l,mid,p<<1);
		build(mid+1,r,p<<1|1);
		up(p);
	}
}
int query(int l,int r,LL d,int p){
	if(fa.l==l&&fa.r==r){
		int cur=upper_bound(nb,ne,d)-nb;
		return cur;
	}
	else {
		int mid=fa.l+fa.r>>1;
		if(r<=mid)return query(l,r,d,p<<1);
		else if(l>mid) return query(l,r,d,p<<1|1);
		else {
			return query(l,mid,d,p<<1)+query(mid+1,r,d,p<<1|1);
		}
	}
}
void dfs(int x,int f){
	L[x]=++tt;
	ln[tt]=x;
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==f)continue;
		dfs(u,x);
	}
	R[x]=tt;
}
int main(){
	cin>>T;
	while(T--){
		ans=0;
		tt=0;
		scanf("%d%lld",&n,&m);
		memset(mark,0,sizeof(mark));
		for(int i=1;i<=n;i++)G[i].clear();
		for(int i=1;i<=n;i++)
			scanf("%lld",&A[i]);
		for(int i=1;i<n;i++){
			int a,b;
			scanf("%d%d",&a,&b);
			G[a].push_back(b);
			mark[b]=1; 
		}
		for(int i=1;i<=n;i++)if(!mark[i])root=i;
		dfs(root,0);
		build(1,n,1);
		for(int i=1;i<=n;i++){
			if(!A[i]){
				ans+=R[i]-L[i];
				continue;
			}
			if(L[i]==R[i])continue;
			ans+=query(L[i]+1,R[i],m/A[i],1);
		}
		printf("%lld\n",ans);
	}
	return 0;
} 
```
#### 思路2（dfs作差）
2469c大佬的方法。对于一个点而言，在遍历其子树之前，存在一个状态，遍历完成后，也存在一个状态，两者相减，就是子树带给答案的贡献。

```c++
#include<bits/stdc++.h>
#define M 100005
#define LL long long
#define lowbit(x) (x&-x)
using namespace std;
int T,n,root;
LL C[M];
LL ans=0;
LL m;
LL A[M],B[M];
bool mark[M];
vector<int>G[M]; 
LL sum(int x){
	LL res=0;
	while(x>0){
		res+=C[x],x-=lowbit(x);
	}
	return res;
}
void add(int x,LL d){
	while(x<=n){
		C[x]+=d,x+=lowbit(x);
	} 
}
void dfs(int x,int f){
	int id=n;
	if(A[x])id=upper_bound(B+1,B+n+1,m/A[x])-B-1;
	LL pre=sum(id);
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==f)continue;
		dfs(u,x);
	}
	ans+=sum(id)-pre;
	add(lower_bound(B+1,B+n+1,A[x])-B,1);
}
int main(){
	cin>>T;
	while(T--){
		ans=0;
		memset(mark,0,sizeof(mark));
		memset(C,0,sizeof(C));
		scanf("%d%lld",&n,&m);
		for(int i=1;i<=n;i++)G[i].clear(),scanf("%lld",&A[i]),B[i]=A[i];
		for(int i=1;i<n;i++){
			int a,b;
			scanf("%d%d",&a,&b);
			G[a].push_back(b);
			mark[b]=1;
		} 
		sort(B+1,B+n+1);
		for(int i=1;i<=n;i++)if(!mark[i]){root=i;break;}
		dfs(root,0);
		printf("%lld\n",ans);
	}
	return 0;
}
```

