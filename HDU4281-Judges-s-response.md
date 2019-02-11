---
title: HDU4281 Judges's response
date: 2018-08-30 13:04:04
tags: [dp,状压dp]
---
#### 题目
已知 n 个点的坐标,裁判在 1 号点。2-n 号上有人提问，需要过去回答。每个人的问题需要的解答时间为 Ci
,每个裁判回答问题的总时间不能超过 m.求：

1:最少需要的裁判数量

2:如果裁判数量无限，最少在路上的时间是多少?
<!--more-->
#### 思路
这题当初写着貌似比较卡，但这也是状压中比较常见的一种题型。
对于问题1而言，我们可以状压回答问题的情况，来枚举其需要的裁判数量，比较简单。
这里我使用$sta[j]$来保存所有可行的情况（只需一个裁判的情况）之后就是状压中比较常见的转移。
```c++
for(int i=0;i<1<<(n-1);i++){
			for(int j=1;j<=scnt;j++){
				if((i&sta[j])!=0)continue;
				dp[i|sta[j]]=min(dp[i|sta[j]],dp[i]+1);
			}
		} 
```

而问题2相对来说比较棘手，我们要思考：对于状压的问题，其本质是枚举，我们在状压的过程中，枚举的是所有可行的情况。
在问题2中，是否可以和问题1同时转移呢？

#### 可以！

```c++

ds[i|sta[j]]=min(ds[i|sta[j]],ds[i]+d[sta[j]]);

```

其本质和问题1是一样的。
这道题关键在于对于状压本质的理解。

#### 完整代码
```c++
#include<bits/stdc++.h>
using namespace std;
int n,m;
int x[20],y[20],c[20];
int G[20][20];
int d[1<<16];//保存中间经过了某些节点的路程最小值 
int dp[1<<16];//保存每种状态至少需要的教练数 
int ds[1<<16];
int dis[1<<16][20];//中间数组，记录每个状态，最后一个元素是k的最小值 
int sta[1<<16],scnt=0;//保存可行的状态 
int bin[1<<16];
int INF;
//都把起点抹掉 
int calc(int i,int j){
	return ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
}
void Init(){
	for(int i=0;i<16;i++)
		bin[1<<i]=i+1;
}
void makesta(){
	scnt=0;
	for(int i=0;i<1<<(n-1);i++){
		int tmp=i,tot=0;
		while(tmp){
			int x=tmp&(-tmp);
			tot+=c[bin[x]];
			tmp-=x;
		}
		if(tot<=m)sta[++scnt]=i; 
	}
}
void makeG(){
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			G[i][j]=calc(i,j);
}
void makedis(){
	memset(d,0x3f,sizeof(d));
	memset(dis,0x3f,sizeof(dis));
	for(int i=1;i<n;i++)dis[1<<(i-1)][i-1]=G[0][i];
	for(int i=0;i<1<<(n-1);i++){
		for(int j=0;j<n-1;j++){
			if((i&(1<<j))==0)continue;
			d[i]=min(d[i],dis[i][j]+G[j+1][0]);
			for(int k=0;k<n-1;k++){
				if((i&(1<<k))!=0)continue;
				dis[i|(1<<k)][k]=min(dis[i|(1<<k)][k],dis[i][j]+G[j+1][k+1]);
			}
		}
	}
}
int main(){
	Init();
	while(scanf("%d%d",&n,&m)!=EOF){
		for(int i=0;i<n;i++)
			scanf("%d%d",&x[i],&y[i]);
		for(int i=0;i<n;i++)
			scanf("%d",&c[i]);
		makesta();
		makeG();
		makedis();
		memset(dp,0x3f,sizeof(dp));
		memset(ds,0x3f,sizeof(ds));
		INF=dp[0];
		dp[0]=0;ds[0]=0;
		for(int i=0;i<1<<(n-1);i++){
			for(int j=1;j<=scnt;j++){
				if((i&sta[j])!=0)continue;
				dp[i|sta[j]]=min(dp[i|sta[j]],dp[i]+1);
				ds[i|sta[j]]=min(ds[i|sta[j]],ds[i]+d[sta[j]]);
			}
		} 
		if(dp[(1<<(n-1))-1]==INF)printf("-1 ");
		else printf("%d ",dp[(1<<(n-1))-1]);
		if(ds[(1<<(n-1))-1]==INF)printf("-1");
		else printf("%d",ds[(1<<(n-1))-1]);
		printf("\n");
	} 
	return 0;
} 
```

主要是前面预处理的部分比较卡细节。
