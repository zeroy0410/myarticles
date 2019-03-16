---
title: hihocoder1676_树上的等差数列
date: 2018-08-30 12:56:10
tags: [dp,树形dp]
---

#### 题目
  给定一棵包含N个节点的无根树，节点编号1~N。
  中每个节点都具有一个权值，第i个节点的权值是Ai。
　　
　　小Hi希望你能找到树上的一条最长路径，满足沿着路径经过的节点的权值序列恰好构成等差数列。
　　
#### 思路
其实思路很简单，对于一个节点来说，假设已知它到一个儿子的公差为a，则只要知道这个儿子延伸下去的最长链，和另一个公差为-a的最长链即可。因此我们可以用$mx[x][j]$表示当前x节点，公差为j的最长链是多少。一开始想用数组存，然而会爆空间，所以改用了map。
<!--more-->
```c++
#include<bits/stdc++.h>
#define M 100005
#define clr(x,y) memset(x,y,sizeof(x)) 
using namespace std;
struct P100{
	int h[M],tot;
	struct edge{
		int nxt,to;
	}G[M*2];
	void Add(int a,int b){
		G[++tot]=(edge){h[a],b};
		h[a]=tot;
	}
	int A[M],B[M],n,ans;
	//一条链要构成等差数列，要么是在与其父亲节点相连，要么是与它相邻的兄弟节点相连 
	//处理出一个点出发，公差是i的最长等差数列的长度 
	int dp[M];//到达每个点，公差是与它父节点距离的最长等差数列的那一条链长度。
		//到达每个点，子树中公差为**的最长等差数列长度 
	int pcnt;
	map<int,int>mx[M];//表示此点的mx达到某一个值的最大追 
	void dfs(int x,int f,int ds){ 
		mx[x].clear();
		for(int i=h[x];i;i=G[i].nxt){
			int u=G[i].to;
			if(u==f)continue;
			int gc=A[u]-A[x];
			dfs(u,x,gc+M-5);
			ans=max(ans,dp[u]+mx[x][-gc+M-5]+1);
			mx[x][gc+M-5]=max(mx[x][gc+M-5],dp[u]);
			if(gc+M-5==ds)dp[x]=mx[x][gc+M-5]+1;
		}
//		cout<<x<<' '<<dp[x]<<endl;
	}
	void solve(){
		cin>>n;tot=0;ans=1;pcnt=0;clr(h,0);
		for(int i=1;i<=n;i++)dp[i]=1;
		for(int i=1;i<=n;i++)
			scanf("%d",&A[i]),B[i]=A[i];
		for(int i=1,a,b;i<n;i++){
			scanf("%d%d",&a,&b);
			Add(a,b);
			Add(b,a);
		} 
		sort(B+1,B+n+1);
		dfs(1,0,0);
		printf("%d\n",ans);
	}
}p100;
int main(){
//	printf("%lf",sizeof(p100)/1024.0);
	p100.solve();
	return 0;
} 
```

#### 然而

其实还有另一种神奇的思路。能使复杂度降到只有排序的logn。

具体看Little_Jian dalao的博客吧。

[博文链接](http://www.chenhowe.me/?p=1438#comment-200)