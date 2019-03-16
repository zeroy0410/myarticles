---
date: 2018-10-03 22:21
status: draft
tags:
  - 树的直径
  - 离线赛
title: 'YCJSOI3804 远离班主任'
---

#### 题目

n 个同学在一棵树上。现在班主任来了。树上有两个点是机房。问所有同学躲进 机房最少要花多少时间。 假设树上每条边都要花费 1 的时间。 

#### 思路
相信结论大家已经知道了：
设两个机房为$u​$,$v​$。
先抽直径，考虑直径的两个端点可能会对答案造成的影响，这样$u​$,$v​$两边的点就不用考虑了（肯定不可能长过直径端点造成的贡献）。

```c++
int Q[M],qcnt=0;
bool mark[M];//标记直径上的点 
int mdep[M],be[M],rt=0,dep[M];
void redfs(int x,int f,int d){//处理直径挂下来的"小树 "的最大深度 
	mdep[x]=d;be[x]=rt;dep[x]=d;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f||mark[u])continue;
		redfs(u,x,d+1);
		mdep[x]=max(mdep[x],mdep[u]);	
	}
}
void Init_ZJ(){//抽直径 
	mxdis=-1;dfs(1,0,0); 
	mxdis=-1;dfs(ed,0,0);
	int st=ed;
	mxdis=-1;dfs(ed,0,0);
	int x=ed;
	while(x!=st){
		Q[++qcnt]=x;
		x=fa[x];
	}
	Q[++qcnt]=st;
	for(int i=1;i<=qcnt;i++)mark[Q[i]]=1;
	for(int i=1;i<=qcnt;i++)rt=i,redfs(Q[i],0,0); 
}
```



下面考虑中间的点。

首先，奇偶性的问题就不用纠结了，因为中间那个点无论放在哪边都是一样的。

只用考虑一种情况，如果中点不在直径的点上怎么办？

很简单，直接附到直径上就好了，因为子树中的点是不会对答案造成贡献的。

现在就是计算答案的问题：

如果我们设每个点在Q中的编号为$be[u]$

那么假设$be[a]$<$be[b]$，

则
$$
dis[a]=dep[a]+mdep[x]+x-be[a]
$$

$$
dis[b]=dep[b]+mdep[b]+be[b]-x
$$

注意到$mdep[x]+x$和$mdep[x]-x$可以分离变量，所以维护两个ST表就可以了

#### 完整代码

```c++
#include<bits/stdc++.h>
#define M 100005
using namespace std;
int n,m;
int h[M],tot;
struct edge{
	int nxt,to;
}G[M<<1];
void Add(int a,int b){
	G[++tot]=(edge){h[a],b};
	h[a]=tot;	
}
int mxdis=0,ed,fa[M];
void dfs(int x,int f,int w){
	fa[x]=f;
	if(w>mxdis)mxdis=w,ed=x;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f)continue;
		dfs(u,x,w+1);		
	}
}
int Q[M],qcnt=0;
bool mark[M];//标记直径上的点 
int mdep[M],be[M],rt=0,dep[M];
void redfs(int x,int f,int d){//处理直径挂下来的"小树 "的最大深度 
	mdep[x]=d;be[x]=rt;dep[x]=d;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f||mark[u])continue;
		redfs(u,x,d+1);
		mdep[x]=max(mdep[x],mdep[u]);	
	}
}
void Init_ZJ(){//抽直径 
	mxdis=-1;dfs(1,0,0); 
	mxdis=-1;dfs(ed,0,0);
	int st=ed;
	mxdis=-1;dfs(ed,0,0);
	int x=ed;
	while(x!=st){
		Q[++qcnt]=x;
		x=fa[x];
	}
	Q[++qcnt]=st;
	for(int i=1;i<=qcnt;i++)mark[Q[i]]=1;
	for(int i=1;i<=qcnt;i++)rt=i,redfs(Q[i],0,0); 
}
struct ST{
	int mx[M][18],lg2[M];
	void Init_RMQ(bool f){
		for(int i=1;i<=qcnt;i++){
			if(f)mx[i][0]=mdep[Q[i]]+i;
			else mx[i][0]=mdep[Q[i]]-i;
		}
		lg2[1]=0;
		for(int i=2;i<=qcnt;i++)lg2[i]=lg2[i>>1]+1;
		for(int j=1;(1<<j)<=qcnt;j++)
			for(int i=1;i+(1<<j)-1<=qcnt;i++)
				mx[i][j]=max(mx[i][j-1],mx[i+(1<<(j-1))][j-1]); 
	}
	int query(int l,int r){
		if(l>r)return -2e9;
		int k=lg2[r-l+1];
		return max(mx[l][k],mx[r-(1<<k)+1][k]);
	}	
}S1,S2;
int main(){
	cin>>n;
	for(int i=1,a,b;i<n;i++){
		scanf("%d%d",&a,&b);
		Add(a,b);
		Add(b,a);	
	}
	Init_ZJ(); 
	S1.Init_RMQ(1);
	S2.Init_RMQ(0);
	scanf("%d",&m);
	for(int i=1,a,b;i<=m;i++){
		scanf("%d%d",&a,&b);
		int mx=-2e9;
		if(be[a]==be[b]){
			if(dep[a]>dep[b])swap(a,b);
			mx=max(dep[a]+be[a]-1,dep[a]+qcnt-be[a]);
			printf("%d\n",mx);
			continue;
		}
		if(be[a]>be[b])swap(a,b);
		mx=max(min(dep[a]+be[a]-1,dep[b]+be[b]-1),min(dep[a]+qcnt-be[a],dep[b]+qcnt-be[b]));
		int lmid=(dep[a]+be[b]-be[a]+dep[b])/2;
		int mid=lmid-dep[a]+be[a];
		if(mid<=be[a])mid=be[a];
		else if(mid>=be[b])mid=be[b];
		int k1=S1.query(be[a]+1,mid);
		int k2=S2.query(mid+1,be[b]-1);
		mx=max(mx,max(k1+dep[a]-be[a],k2+dep[b]+be[b]));
		printf("%d\n",mx);
	}
	return 0;
}	
```