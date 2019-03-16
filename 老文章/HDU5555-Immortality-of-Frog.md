---
title: HDU5555 Immortality of Frog
date: 2018-08-30 13:05:33
tags: [dp,状压dp]
---

#### 题目
N  frogs are attempting to prolong their life-span. They live in the bottom of a well which can be described as a two-dimensional N×N grid. Grid(i,j) is located in the i−th row and the j−th column. At the beginning, the i−th frog lives in the bottom of i−th column, i.e. the place below grid(1,i). 
The frogs are so devout that God decides to give them a chance. In each row i, a horizontal membrane ranging from (i,Li) to (i,Ri) inclusively is created. A capsule of elixir is placed at one of the grids of the membrane with uniform probability. 
<!--more-->
Now the frogs are jumping upwards to pursue immortality. The i−th frog would be in grid(j,i) after j jumps. When a frog arrives at a grid that contains a capsule of elixir, it will eat the capsule and gain immortality. After that, it continues jumping upwards until it gets out of the well. 

A membrane is considered “ bad” if it convers less than N grids. The frogs are very sensitive, so they can only endure passing through 10 bad membrane. When a frog reaches the 11th bad membrane, it thinks that there is no hope to get out of the well, so it will go back to the bottom of well and live there until death, even though it has eaten a capsule of elixir already. 

The frogs are friends, so they want all of them gain immortality and live a happy life out of the well. They want to know the probability P that every frog eats exactly one capsule of elixir and gets out of the well. 

有 n 只青蛙想要追求长生不老，他们住在一个 N*N 的网格图里面，第 i 只青蛙一开始在第 i 列的底部，即
格子（1,i）的下面。万能的上帝在每一行都放置了一些水平的膜，第 i 行的膜会覆盖[Li,Ri]区间上的点。
在每个膜上面会放置一颗长生不老药，膜上的每个点含有这个长生不老药的概率是均等的。每一只青蛙只
会往上跳，经过一个膜时，如果膜上有药，他会把这个膜上的药吃了，然后这个膜会消失，同时他也长生
不老了，接着他会直接跳出去(不吃药了)。对于每一个膜，如果它长度小于 n，那么这就是个坏膜。每只
青蛙不能经过超过 10 个坏膜，请问所有青蛙都能长生不老，并且成功跳出去的概率 P 是多少？

#### 思路
好膜和坏膜分开考虑，对于好膜x个而言，其最后一定会有x只青蛙来来度过，所以方案数为x!

接下来就是考虑坏膜，对于坏膜来说，如果一个点上方有10个及以上的坏膜，那么可以直接输出0（无法使所有的青蛙全部跳出），不然我们可以状压。

将青蛙经过的点转换为1个二进制数储存，由于记录的是其上面的坏膜的相对顺序，所以在转移之前要先将本层的bin转化到下一层（必须完全映射）。

最后因为坏膜必须全部用完，所以$dp[n][(1<<vec[n].size())-1]$即为最终答案。

#### 代码
```c++
#include<bits/stdc++.h>
#define M 1005
#define MOD 105225319
#define LL long long 
#define clr(a,b) memset(a,b,sizeof(a))
using namespace std;
int L[M],R[M];
int dp[M][1<<10];
vector<int>vec[M];//用于存储每个i上面膜的坐标 
int T,n,ca=0,f=0,P[15];
int ans=1,fac[M]={1};
void Init(int x){// 坐标 
	for(int i=0;i<vec[x].size();i++){
		P[i]=-1;
		for(int j=0;j<vec[x+1].size();j++)
			if(vec[x][i]==vec[x+1][j])
				P[i]=j;//映射与下一层重复之点 
	}
}
int nbin(int x,int bin){//必须完全映射 
	int nxt=0;
	for(int i=0;i<vec[x].size();i++){
		if(P[i]==-1){
			if((bin&(1<<i))==0)
				return -1;	
		} 
		else {
			if(bin&(1<<i))
				nxt|=(1<<P[i]);
		}
	}
	return nxt;
} 
int main(){
	for(int i=1;i<=1000;i++)
		fac[i]=1LL*fac[i-1]*i%MOD;
	cin>>T;
	while(T--){
		ans=1;
		ca=0;
		clr(dp,0);
		scanf("%d",&n);
		for(int i=1;i<=n;i++)
			vec[i].clear();
		for(int i=1;i<=n;i++) 
			scanf("%d",&L[i]);
		for(int i=1;i<=n;i++) 
			scanf("%d",&R[i]);
		for(int i=1;i<=n;i++){
			if(L[i]==1&&R[i]==n)ca++;
			else for(int j=L[i];j<=R[i];j++)
				vec[j].push_back(i); 
		}
		bool flag=0;
		for(int i=1;i<=n;i++)
			if(vec[i].size()>10){
				flag=1;
				break;
			}
		if(flag){
			printf("Case #%d: 0\n",++f);
			continue;
		}
		ans*=fac[ca];//好膜的可行情况 
		//下面枚举青蛙穿坏膜的情况总数
		dp[0][0]=1;
		for(int i=0;i<n;i++){
			Init(i);
			for(int j=0;j<1<<vec[i].size();j++){//本层的bin 
				int nxt=nbin(i,j); 
				if(nxt==-1)continue;
				dp[i+1][nxt]+=dp[i][j];dp[i+1][nxt]%=MOD; 
				for(int k=0;k<vec[i+1].size();k++){
					if((nxt&(1<<k))!=0)continue;
					dp[i+1][nxt|(1<<k)]+=dp[i][j];
					dp[i+1][nxt|(1<<k)]%=MOD; 
				}
			} 
		}
		//坏膜是要全部跳完的 
		ans=1LL*ans*dp[n][(1<<vec[n].size())-1]%MOD;
		printf("Case #%d: %d\n",++f,ans);
	}
	return 0;
} 
 
```