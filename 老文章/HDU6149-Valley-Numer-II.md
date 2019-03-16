---
title: HDU6149 Valley Numer II
date: 2018-08-30 13:00:30
tags: [dp,状压dp]
---

#### 题目
众所周知，度度熊非常喜欢图。

它最近发现了图中也是可以出现 valley —— 山谷的，像下面这张图。

![image](http://bestcoder.hdu.edu.cn/data/images/C777-1006-1.jpg)

为了形成山谷，首先要将一个图的顶点标记为高点或者低点。标记完成后如果一个顶点三元组<X, Y, Z>中，X和Y之间有边，Y与Z之间也有边，同时X和Z是高点，Y是低点，那么它们就构成一个valley。

度度熊想知道一个无向图中最多可以构成多少个valley，一个顶点最多只能出现在一个valley中。
<!--more-->
#### 思路
这道题的特殊性就在于，对于一个低点，它要匹配的是两个高点，所以我们可以用$dp[i][S]$表示对于当前低点，消耗的高点的最大值，其他的也就是标准的状压dp了。

#### 代码

```c++
#include<bits/stdc++.h>
#define clr(x,y) memset(x,y,sizeof(x))
#define M 35
using namespace std;
int n,m,k,T;
bool G[M][M];
int v[M];
bool ma[M];
int dp[2][1<<15];
void tomax(int &x,int y){
	if(x<y)x=y;
}
int main(){
	scanf("%d",&T);
	while(T--){
		scanf("%d%d%d",&n,&m,&k);
		clr(G,0);clr(v,0);clr(ma,0);clr(dp,0);
		for(int i=1,a,b;i<=m;i++){
			scanf("%d%d",&a,&b);
			G[a][b]=G[b][a]=1;
		}
		for(int i=0,x;i<k;i++){
			scanf("%d",&x);
			ma[x]=1;
			v[i]=x;
		}
		int now=0;
		for(int i=1;i<=n;i++){
			if(ma[i])continue;
			now=(!now);
			memcpy(dp[now],dp[now^1],sizeof(dp[now]));
			for(int j=0;j<1<<k;j++){
				for(int r=0;r<k;r++){
					if((j&(1<<r))||!G[i][v[r]])continue;
					for(int t=r+1;t<k;t++){
						if((j&(1<<t))||!G[i][v[t]])continue;
						tomax(dp[now][j|(1<<r)|(1<<t)],dp[now^1][j]+1);
					}
				}
			}
		}
		int ans=0;
		for(int i=0;i<1<<k;i++)tomax(ans,dp[now][i]);
		printf("%d\n",ans);
	}
	return 0;
}

```

