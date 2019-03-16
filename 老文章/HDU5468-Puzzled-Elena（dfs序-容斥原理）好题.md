---
title: HDU5468 Puzzled Elena（dfs序+容斥原理）好题
date: 2018-08-30 13:12:05
tags: [树上问题,dfs序]
---

#### 题目
给定一棵以1为根的树，求每个节点对应的子树中有多少个节点权值与它互质。

#### 思路
首先要预处理出所有的数的质因子。

很容易想到，一棵子树里与根节点互质的点的个数是多于与根节点不互质的点的个数的，所以，我们可以计算与根节点不互质的点，最后用子树的大小减一下。

然而如何计算呢？

题目中有关于使用容斥原理的提示，但是对于一棵子树来说，单独开辟空间存储一个质因子出现了几次显然会爆内存，于是要使用dfs作差。
<!--more-->
在递归下去之前算一下，递归下去之后再算一下，其中的差值就是子树的信息。

#### 代码
```c++
#include<bits/stdc++.h>
#define M 100005
using namespace std;
int n,A[M];
vector<int>G[M];
int tt=0,ff=0;
int cnt[M],ans[M];
vector<int>son[M];//此数的因子
void Init(){
    for(int i=2;i<=M-5;i++){
    	if(son[i].size()!=0)continue;
    	for(int j=i;j<=M-5;j+=i)
    		son[j].push_back(i);
	}
}
int calc(int x,int d){
	int tot=0,ans=0,ct=0,p=son[x].size();
	for(int i=1;i<1<<p;i++){
		tot=1,ct=0;
		for(int j=0;j<p;j++){
			if(i&(1<<j)){
				tot*=son[x][j];
				ct++;
			}
		}
		if(ct&1)ans+=cnt[tot];
		else ans-=cnt[tot];
		cnt[tot]+=d;
	}
	return ans;
}
void dfs(int x,int f){
	int L=++tt;
	int pre=calc(A[x],0);
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==f)continue;
		dfs(u,x);
	}
	int nxt=calc(A[x],1);
	ans[x]=(tt-L)-(nxt-pre);
	if(A[x]==1)ans[x]++;
}
int main(){
	Init();
	while(~scanf("%d",&n)){
		tt=0;
		memset(cnt,0,sizeof(cnt));
		for(int i=1;i<=n;i++)G[i].clear(); 
		for(int i=1,a,b;i<n;i++){
			scanf("%d%d",&a,&b);
			G[a].push_back(b);
			G[b].push_back(a); 
		}
		for(int i=1;i<=n;i++)scanf("%d",&A[i]);
		dfs(1,0);
		printf("Case #%d:",++ff);
		for(int i=1;i<=n;i++)
			printf(" %d",ans[i]);
		puts("");
	}
	return 0;
}
```

#### 总结
dfs作差适用于计数问题，对于那些子树之间的信息相互影响，且子树的情况唯一确定的问题，比较适合使用。

