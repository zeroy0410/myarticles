---
title: HDU4487_Maximum_Random_Walk
date: 2018-09-05 14:38:03
tags: [dp,概率dp]
---

#### 题目

> Consider the classic random walk: at each step, you have a 1/2 chance of taking a step to the left and a 1/2 chance of taking a step to the right. Your expected position after a period of time is zero; that is, the average over many such random walks is that you end up where you started. A more interesting question is what is the expected rightmost position you will attain during the walk.

<!--more-->

大意是说，让你走n步，求最后能走到最远距离的期望步数是多少。

#### 思路

开一个$dp[i][j][k]$用于当前状态，已经走了$j$步，最远到达了$k$ 的距离，然后就是转移。

略了。。

#### 代码

```c++
#include<bits/stdc++.h>
#define M 205
using namespace std;
double L,R;
int T,n;
double dp[2][M][M],ans=0;//表示走了多少步，现在在哪，最远到了哪 
int main(){
	cin>>T;
	while(T--){
		int ff;ans=0;
		scanf("%d%d%lf%lf",&ff,&n,&L,&R);
		int lim=n+100;
		int cur=1;
		memset(dp[cur],0,sizeof(dp[cur]));
		dp[cur][100][0]=1;
		for(int k=1;k<=n;k++){
			cur=(k&1);
			memset(dp[cur^1],0,sizeof(dp[cur^1]));
			for(int i=1;i<=lim;i++){
				for(int j=0;j<=n;j++){
					if(dp[cur][i][j]>0){
						if(i+1-100>j)dp[cur^1][i+1][i+1-100]+=dp[cur][i][j]*R;
						else dp[cur^1][i+1][j]+=dp[cur][i][j]*R;
						dp[cur^1][i-1][j]+=dp[cur][i][j]*L;
						dp[cur^1][i][j]+=dp[cur][i][j]*(1-L-R);
					}
				}
			}
		}
		for(int i=0;i<=lim;i++)
			for(int j=0;j<=n;j++)
				ans+=j*dp[cur^1][i][j];
		printf("%d %.4lf\n",ff,ans);
	}
	return 0;
}
```

