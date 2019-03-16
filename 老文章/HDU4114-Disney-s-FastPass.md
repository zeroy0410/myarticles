---
title: HDU4114_Disney's_FastPass
date: 2018-08-30 13:01:51
tags: [dp,状压dp]
---

#### 题目
一个图 ,有 n个点 ,有 m条边 ,有 k个点是你要参观的（需排队 ,花时间 t）,然后你到某点时 ,可能以获 得一种加速票 ,这种票可以让你在 i点时排队只需花 ti 的时间 .求参观完这 k个点所需最少时间 .

#### 思路

到达了一个点之后，我们显然要将这个点的票全部取得，于是我们可以开

$dp[i][j][k]$表示到了哪个点，取得了哪些加速票，以及参观了哪些需要参观的点。

这样先Floyd求出两点之间的距离，再用一个数组Ca二进制表示所有点上能够取得的加速票。

注意：一个点可能有很多的**相对位置**,我也不知道造数据的人是怎么想的。
<!--more-->
#### 代码
```c++
#include<bits/stdc++.h>
#define M 55
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int T,n,m,K,f=0;
int G[M][M];
int dp[M][1<<8][1<<8];
vector<int>mp[M];//表示i点的相对位置 
int t1[M],t2[M];//原来所需时间，拿到加速票后所需时间 
int Ca[M];//该点能获得的加速票 
void tomin(int& x,int y){
	if(y<x)x=y;
}
int main(){
	cin>>T;
	while(T--){
		int ans=2e9;
		clr(G,0x3f);clr(dp,0x3f);clr(Ca,0); 
		scanf("%d%d%d",&n,&m,&K);
		for(int i=1;i<=m;i++){
			int a,b,c;
			scanf("%d%d%d",&a,&b,&c);
			G[a][b]=G[b][a]=min(G[a][b],c); 
		}
		for(int i=1;i<=n;i++)G[i][i]=0,mp[i].clear();
		for(int k=1;k<=n;k++)
			for(int i=1;i<=n;i++)
				for(int j=1;j<=n;j++)
					G[i][j]=min(G[i][j],G[i][k]+G[k][j]);
		for(int i=1;i<=K;i++){
			int p,x;
			scanf("%d",&p);mp[p].push_back(i-1);
			scanf("%d%d",&t1[i-1],&t2[i-1]);
			scanf("%d",&p);
			for(int j=1;j<=p;j++) 
				scanf("%d",&x),Ca[x]|=1<<(i-1);
		}
		dp[1][0][0]=0;
		for(int i=0;i<1<<K;i++){//已经去过了哪些点 
			for(int j=0;j<1<<K;j++){//已经获得了哪些加速票 
				for(int las=1;las<=n;las++){
					if(dp[las][i][j]<1e9){
						for(int k=1;k<=n;k++){
							tomin(dp[k][i][j|Ca[k]],dp[las][i][j]+G[las][k]);//由上层转移 
							for(int q=0;q<mp[k].size();q++){
								int v=mp[k][q];
								if(j>>v&1)tomin(dp[k][i|1<<v][j],dp[las][i][j]+G[las][k]+t2[v]);
								else tomin(dp[k][i|1<<v][j],dp[las][i][j]+G[las][k]+t1[v]);
							}
						}
					}
				}
			}
		}
		for(int i=1;i<=n;i++)
			for(int j=0;j<1<<K;j++)
				tomin(ans,dp[i][(1<<K)-1][j]+G[i][1]);//最后还要回到原点 
		printf("Case #%d: %d\n",++f,ans);
	}
	return 0;
} 
```