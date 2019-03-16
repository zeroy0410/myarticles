---
title: ZOJ3231 Apple Transportation
date: 2018-08-30 12:57:31
tags: [dp,树形dp]
---
#### 题目
在一棵树上有n个节点(从0开始)，在每个节点上都有一些苹果，现在需要在这棵树上把一些苹果从一个节点搬到另一个节点，使得这些节点上的苹果数的方差最小，从a节点搬运x个苹果到b节点的花费为x*dist(a,b)，求满足条件的最小花费是多少？

#### 思路

想了很久。。。

一开始发现，对于一个节点而言，其上最终的苹果数一定是平均数（取整后的）或是平均数+1，我们定义前者为情况1，后者为情况2，这样我们就可以把方差这一维**消掉**。
<!--more-->
之后，我们跑一个分组背包。

$dp[i][j]​$表示当前节点中，情况2的点的个数。

我们看得不是点的贡献，而是每条边的贡献（这也是在处理树上路径问题点权决策复杂的基本套路）。

接下来就是初值的问题，对于叶子节点而言，dp的值显然都是0（此子树内部的边不发生代价）。

那么对于非叶子节点来说呢？

我们无法简单地赋一个初值给它，因为这个节点的所有信息显然都是来自它的子孙的，但如果我们不赋初值，那么dp就没法更新。

所以，对于$dp[x][0]$和$dp[x][1]$我们分别表示，对于x自身，把它变成1情况和2情况的最小花费。而这是不需要花费的，所以，我们把这两个dp赋成0，本层就可以更新了。

但是，dp$[x][0]$和$dp[x][1]$表示的不仅仅是它自己的状态，也包含了子树中的解，所以我们不能把它当做最终的结果，它的存在，只是为了能让自己和其他的解进行更新，所以要先开一个tmp，最后再赋回去。

#### 代码
```c++
#include<bits/stdc++.h>
#define M 105
#define LL long long
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int h[M],tot;
struct edge{
	int nxt,to,co;
}G[M*2];
void Add(int a,int b,int c){
	G[++tot]=(edge){h[a],b,c};
	h[a]=tot;
}
#define DEG(a,st) for(int a=h[st];a;a=G[a].nxt)
int A[M],sz[M],n,sum[M],res;
LL dp[M][M],S,pj;//在当前节点有几个是pj+1，其余的全是pj,的最小花费 
void dfs(int x,int f){
	sz[x]=1;sum[x]=A[x];
	dp[x][0]=dp[x][1]=0;
	DEG(i,x){
		int u=G[i].to,v=G[i].co;
		if(u==f)continue;
		dfs(u,x);
		sz[x]+=sz[u];sum[x]+=sum[u];
		for(int j=res;j>=0;j--){
			LL tmp=1e18;
			for(int r=0;r<=min(sz[u],j);r++){
				int dis=abs(r*(pj+1)+(sz[u]-r)*pj-sum[u])*v;
				tmp=min(tmp,dp[x][j-r]+dp[u][r]+dis);
			}
			dp[x][j]=tmp;
		} 
	}
}
int main(){
	while(~scanf("%d",&n)){
		clr(h,0);tot=0;S=0;
		for(int i=0;i<n;i++)scanf("%d",&A[i]),S+=A[i];
		pj=S/n;
		res=S-pj*n; 
		for(int i=1,a,b,c;i<n;i++){
			scanf("%d%d%d",&a,&b,&c);
			Add(a,b,c);
			Add(b,a,c);
		}
		for(int i=0;i<=n;i++)
			for(int j=0;j<=n;j++)dp[i][j]=1e18;
		dfs(0,-1);
		cout<<dp[0][res]<<endl;
	}
	return 0;
} 
```