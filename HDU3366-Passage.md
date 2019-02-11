---
title: HDU3366 Passage
date: 2018-09-02 08:46:16
tags: [dp,概率dp]
---

#### 题目

一个富翁被困在迷宫之中，在他面前有$n$条路可以选。他已经得知每条路的信息。每条路有两个属性值$P_i$和$Q_i$,$P_i$表示直接出去的概率，$Q_i$表示碰到卫兵的概率，碰到卫兵就必须支付1百万，再返回，如果没有钱支付，就会被杀。同时这条路还有$1-P_i-Q_i$的概率是死胡同。现在富翁有$m$百万，求如何决策使得他逃出的概率最大。

<!--more-->

#### 思路

首先我们要明确一点：富翁走的门越多能够得到的逃出去的概率越大。

对于一个门来说，它的情况无非有3点：

1. 逃出去
2. 被抓住，消耗一些钱
3. 没被抓住，原路返回

这三种状态都会对后面的门产生影响。

使用$dp[i][j]$表示已经到了第i扇门，消耗了k百万能逃出去的概率，那么可以得到三条状态转移方程
$$
dp[i+1][j]+=dp[i][j]·(1-A[i].p-A[i].q)
$$

$$
dp[i+1][j+1]+=dp[i][j]·(A[i].q)
$$

这两条是将前面的不可行情况累到后面

最后一条：
$$
dp[i][j]·=A[i].p
$$
这就是最后在这个点逃出去的概率。

不难发现，ans就是这些概率的和值，而且，经过门的顺序不同，造成的结果也不同。

设两个点为a，b

这两个走廊对后面造成的贡献为
a在前，b在后：
$$
(Q_a+(1-Q_a-P_a)·P_b+Q_a·P_b)·dp[x][m]
$$


a在后，b在前：
$$
(Q_b+(1-Q_b-P_b)·P_a+Q_b·P_a)·dp[x][m]
$$


假设a更优，则作差得：

$$
2dp[x][m]·(P_a·Q_b-P_b·Q_a)>0
$$


--> 
$$
P_a/Q_a>P_b/Q_b
$$


按此顺序dp即可



```c++
#include<bits/stdc++.h>
#define M 1005
using namespace std;
struct node{
	double p,q;
	bool operator < (const node& res) const {
		return q/p<res.q/res.p;
	}
}A[M];
int T;
int n,m,ff=0;
double dp[M][15];//被抓了k次，逃出去的概率 
double ans=0;
int main(){
	cin>>T;
	while(T--){
		ans=0;
		scanf("%d%d",&n,&m);
		for(int i=1;i<=n;i++)for(int j=0;j<=m;j++)dp[i][j]=0; 
		for(int i=1;i<=n;i++)
			scanf("%lf%lf",&A[i].p,&A[i].q);
		sort(A+1,A+n+1);
		dp[0][0]=1;
		for(int i=0;i<=n;i++){
			for(int j=0;j<=m;j++){
				dp[i+1][j]+=dp[i][j]*(1-A[i].p-A[i].q);
				dp[i+1][j+1]+=dp[i][j]*A[i].q;
				dp[i][j]*=A[i].p;
				ans+=dp[i][j];
			}
		}
		printf("Case %d: %.5lf\n",++ff,ans);
	}
	return 0;
}
```





