---
title: POJ1417_True_Liars(并查集)
date: 2018-08-30 10:22:46
tags: 并查集
---
#### 题目
有一群好人和一群坏人，好人永远说真话，坏人永远说假话。现给出一组话，问能否唯一确定每个人是好人还是坏人。

```c++
样例输入：
2 1 1
1 2 no
2 1 no
3 2 1
1 1 yes
2 2 yes
3 3 yes
2 2 1
1 2 yes
2 3 no
5 4 3
1 2 yes
1 3 no
4 5 yes
5 6 yes
6 7 no
0 0 0
```
<!--more-->
#### 思路

不难发现，如果是A B yes就说明A,B是同类，反之则说明A,B是异类。题目的关键就在于，题目对于好人的个数有着精确的限制，我们无法确定每个集合中，究竟代表这好人，还是坏人。所以需要额外跑一个**背包**，这点还是比较难想的。

我们可以用带权并查集预处理出所有的**小集团**，再得到这些集团中的**小集合**，之后使用$A[i][0]$,$A[i][1]$表示集团中两种人的数目，再跑一遍dp

$dp[i][j]$记录的是已经遍历了i个集团，有j个好人所具有的种数，由于我们答案只要求$dp[tot][p]$是否==1，>1，<1，所以对于>1的情况，我们都视为2，不然会爆int。

最后就是打印方案了。我的方法是:**暴力**,毕竟数据只有600。

#### 代码
```c++
#include<stdio.h>
#include<string.h>
#include<vector>
#include<algorithm>
#define M 1005
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int fa[M],dis[M];
char S[5];
int getfa(int x){
	if(fa[x]!=x){
		int t=getfa(fa[x]);
		dis[x]^=dis[fa[x]];
		fa[x]=t;
	} 
	return fa[x];
}
int n,p,q,tot=0;
int A[M][2];//表示一个集合中的两种元素的个数 
int be[M],dp[2][M],pa[M][M];
int ans[M],acnt=0;
int main(){
	while(~scanf("%d%d%d",&n,&p,&q)&&(n||p||q)){
		clr(dis,0);tot=0;clr(A,0);clr(dp,0);clr(pa,0);
		for(int i=1;i<=M-5;i++)fa[i]=i;
		bool flag=1;
		for(int i=1,x,y;i<=n;i++){
			scanf("%d%d%s",&x,&y,S);
			int r1=getfa(x),r2=getfa(y);
			if(S[0]=='n'){
				if(r1==r2&&dis[x]==dis[y]){flag=0;continue;}
				else {
					dis[r1]=dis[x]^dis[y]^1;
					fa[r1]=r2;
				}
			}
			else {
				if(r1==r2&&dis[x]!=dis[y]){flag=0;continue;}
				else {
					dis[r1]=dis[x]^dis[y]^0;
					fa[r1]=r2;
				}
			}
		}
		if(!flag){puts("no");continue;}
		for(int i=1;i<=p+q;i++){
			if(getfa(i)==i){
				tot++;
				be[i]=tot;
			}
		} 
		for(int i=1;i<=p+q;i++){
			A[be[getfa(i)]][dis[i]]++;
		} 
		dp[0][0]=1;
		for(int i=1;i<=tot;i++){//选了若干集合 
			int cur=(i&1);
			clr(dp[cur],0);
			for(int j=0;j<=p;j++){//挑出了一些点作为好人 
				if(j>=A[i][0])dp[cur][j]+=dp[!cur][j-A[i][0]];
				if(j>=A[i][1])dp[cur][j]+=dp[!cur][j-A[i][1]];
				if(j>=A[i][0]&&dp[!cur][j-A[i][0]]==1)pa[i][j]=0;
				else if(j>=A[i][1]&&dp[!cur][j-A[i][1]]==1)pa[i][j]=1;
				if(dp[cur][j]>1)dp[cur][j]=2;
			}
		}
		if(dp[tot&1][p]!=1){puts("no");continue;}
		acnt=0;
		int i=tot,j=p;
		while(i>0&&j>0){
			int k=pa[i][j];
			for(int r=1;r<=p+q;r++){
				if(be[getfa(r)]==i&&dis[r]==k)ans[++acnt]=r;
			}
			j-=A[i][k];i--;
		} 
		sort(ans+1,ans+acnt+1);
		for(i=1;i<=acnt;i++)
			printf("%d\n",ans[i]);
		puts("end");
	}
	return 0;
}
```