---
title: ZOJ3822 Domination
date: 2018-09-22 15:13:30
tags: [DP,期望dp]
---

#### 题目

>Edward is the headmaster of Marjar University. He is enthusiastic about chess and often plays chess with his friends. What's more, he bought a large decorative chessboard with N rows and M columns.
>
>Every day after work, Edward will place a chess piece on a random empty cell. A few days later, he found the chessboard was *dominated* by the chess pieces. That means there is at least one chess piece in every row. Also, there is at least one chess piece in every column.
>
>"That's interesting!" Edward said. He wants to know the expectation number of days to make an empty chessboard of N × M dominated. Please write a program to help him.

<!--more-->

简述题意：有若干天，每天在棋盘上放一个棋子，求使得每行至少有一个棋子，每列至少有一个棋子的期望天数。

#### 思路

对于这种终态确定的问题，一个显然的思路是压缩状态。

对于本题来说，我们有一个显然的思路是状压，再判断是否已经符合条件，但这显然会T，所以思考，一个棋盘中，其实有用的只有**有几个符合条件的行**，**有几个符合条件的列**，这些就是我们的dp状态。

那么转移时我们只用考虑增加行的概率，增加列的概率，既增加行又增加列的概率和既不增加行又不增加列的概率即可。

```c++
#include<bits/stdc++.h>
#define M 55
using namespace std;
double dp[M*M][M][M];//满足了j行，k列的概率
int T,n,m;
int main(){
	cin>>T;
	while(T--){
		scanf("%d%d",&n,&m);
		for(int i=0;i<=n*m;i++)
			for(int j=0;j<=n;j++)
				for(int k=0;k<=m;k++)
					dp[i][j][k]=0.0;
		int tot=n*m;
		dp[0][0][0]=1.0;
		for(int i=0;i<tot;i++){
			for(int j=0;j<=n;j++)
				for(int k=0;k<=m;k++){
					if(j==n&&k==m)continue;
					dp[i+1][j+1][k]+=dp[i][j][k]*(k*(n-j))/(tot-i);//只增加行
					dp[i+1][j][k+1]+=dp[i][j][k]*(j*(m-k))/(tot-i);//只增加列
					dp[i+1][j+1][k+1]+=dp[i][j][k]*(n-j)*(m-k)/(tot-i);//既增加行，又增列
					dp[i+1][j][k]+=dp[i][j][k]*(j*k-i)/(tot-i);//既不增加行，又不增加列
				}
		}
		double ans=0;
		for(int i=1;i<=tot;i++)
			ans+=i*dp[i][n][m];
		printf("%.12lf\n",ans);
	}
	return 0;
}
```

