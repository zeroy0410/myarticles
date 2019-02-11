---
title: ZOJ1654 Place the Robots（二分构图题）
date: 2018-08-30 13:20:44
tags: 二分图
---
#### 题目

现在要在一个地图中，摆放机器人。每个机器人启动之后会向左右上下四个方向发生激光。机器人只能放在空地上。如何摆放机器人，使得其他之间不会攻击到对方。同一行或者同一列的机器人会伤害到对方（除非中间有墙）。

#### 思路
将行列分块，显然一个块中最多只能取一个点，并将有交叠的行列之间连一条边，这样行列之间就构成了二分图，一条边连接的含义就是取了这两个块之间交叠的那个点。所以显然最大匹配就是答案了。

此题应用了二分图匹配中一个点只能使用一次的性质，从而恰好契合了本题中一个块只能用一次的约束条件。
<!--more-->
#### 代码
```c++
#include<bits/stdc++.h>
#define M 2505
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
vector<int>G[M];
bool vis[M];
int mk[M];
int n,m,T,ff=0;
char Map[55][55];
int x[M],y[M];//此点所处的行编号和列编号 
bool sp(int x){
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(!vis[u]){
			vis[u]=1;
			if(!mk[u]||sp(mk[u])){
				mk[u]=x;
				return true;
			}
		}
	}
	return false;
}
int calc(int x,int y){
	return (x-1)*m+y;
}
int main(){
	cin>>T;
	while(T--){
		scanf("%d%d",&n,&m);
		for(int i=1;i<=n*m;i++)G[i].clear();
		for(int i=1;i<=n;i++)
			scanf("%s",Map[i]+1);
		int row=0,col=0;//行列块之编号 
		char last='#';
		for(int i=1;i<=n;i++){
			last='#';
			for(int j=1;j<=m;j++){
				if(last=='#'&&Map[i][j]!='#')row++;
				x[calc(i,j)]=row; 
				last=Map[i][j];
			} 
		}
		for(int j=1;j<=m;j++){
			last='#';
			for(int i=1;i<=n;i++){
				if(last=='#'&&Map[i][j]!='#')col++;
				y[calc(i,j)]=col; 
				last=Map[i][j];
			} 
		}
		for(int i=1;i<=n;i++)
			for(int j=1;j<=m;j++)
				if(Map[i][j]=='o')G[x[calc(i,j)]].push_back(y[calc(i,j)]);
		clr(mk,0);
		int ans=0;
		for(int i=1;i<=row;i++) {
			clr(vis,0);
			if(sp(i)){
				ans++;
			}
		}
		printf("Case :%d\n",++ff);
		printf("%d\n",ans);
	}
	return 0;
}
```