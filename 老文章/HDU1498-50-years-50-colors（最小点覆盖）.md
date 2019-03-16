---
title: 'HDU1498_50_years,_50_colors（最小点覆盖）'
date: 2018-08-30 13:18:30
tags: 二分图
---
#### 题目
给你一个矩阵，矩阵中有一些数，对于每个数而言，一次可以选择将它所在行或列的**这个数**全部消去，问哪些数的最大消去次数>k。如果没有这样的数，输出-1。

#### 思路
对于一个数来说，如果它的行被消了，那么它的列就不能由它消去，即**无法在一次消去这一行和这一列**，那么我们就将这一行和这一列连边，又对于一个数来说，如果它的行已经被消去了，或者它的列已经被消去了，那么这个数就无法使用了（被其它的数消去了），这恰好满足最小点覆盖的定义。

>最小点覆盖：就是点的个数最少的S集合。使这些点能够覆盖到所有的边。

所以，此图中最小点覆盖的数目就是最少消去的次数，直接上匈牙利算法就可以解了。
<!--more-->
#### 代码
```c++
#include<bits/stdc++.h>
#define M 105
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,k;
vector<int>G[M][M];//当前颜色的此编号能引爆的行或列 
int Map[M][M];
int mk[M];
bool vis[M];
int ans[M],acnt=0;
bool sp(int x,int clo){
	for(int i=0;i<G[clo][x].size();i++){
		int u=G[clo][x][i];
		if(!vis[u]){
			vis[u]=1;
			if(!mk[u]||sp(mk[u],clo)){
				mk[u]=x;
				return true;
			}
		}
	}
	return false;
}
int main(){
	while(scanf("%d%d",&n,&k)&&n+k){
		acnt=0;
		for(int i=1;i<=50;i++)
			for(int j=1;j<=n;j++)
				G[i][j].clear();
		for(int i=1;i<=n;i++)
			for(int j=1;j<=n;j++)
				scanf("%d",&Map[i][j]),G[Map[i][j]][i].push_back(j);
		for(int i=1;i<=50;i++){
			clr(mk,0);
			int tot=0;
			for(int j=1;j<=n;j++){
				clr(vis,0);
				if(sp(j,i))tot++; 
			}
			if(tot>k)ans[++acnt]=i;
		}
		if(acnt==0)puts("-1");
		else {
			for(int i=1;i<=acnt;i++){
				printf("%d",ans[i]);
				if(i!=acnt)printf(" ");
			}
			puts("");
		} 
	}
	return 0;
}
```