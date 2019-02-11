---
title: HDU4649 Professor Tian
date: 2018-09-22 15:48:10
tags: [DP,期望dp]
---

#### 题目

给你一个式子，第i位（符号Oi和数字Ai+1）有pi的概率消失掉，要计算最后值的期望 运算符包括“&”，“|”和“^”

#### 思路

本题之精髓所在就在于分步处理。

我们并不需要整体来看最终的每个数出现的概率是多少，只需要看**每一位上的1出现的概率是多少**，整题的核心就是这一点。之后就是一波状压了。

<!--more-->

#### 代码

```c++
#include<bits/stdc++.h>
#define M 205
using namespace std;
int TT;
int A[M],n;
char f[M];
char S[5];
double p[M];
double res[25];//每一位是1的概率 
double tmpp[2][2];
int calc(char c,int a,int b){
	if(c=='&')return (a&b);
	if(c=='|')return (a|b);
	if(c=='^')return (a^b);	
	return 12138;	
}
int main(){
	int cas=0;
	while(~scanf("%d",&n)){
		for(int i=0;i<=20;i++)res[i]=0.0; 
		for(int i=1;i<=n+1;i++)scanf("%d",&A[i]);
		for(int i=2;i<=n+1;i++){
			scanf("%s",S);
			f[i]=S[0];	
		}
		for(int i=2;i<=n+1;i++)scanf("%lf",&p[i]),p[i]=1.0-p[i];
		for(int i=0;i<=20;i++)
			if(A[1]&(1<<i))res[i]=1.0;
		for(int i=2;i<=n+1;i++){
			for(int j=0;j<=20;j++){
				tmpp[0][0]=tmpp[0][1]=0;
				tmpp[1][0]=tmpp[1][1]=0;
				if(A[i]&1<<j){
					tmpp[1][calc(f[i],1,1)]=p[i];//上层是1，本层是它的概率 
					tmpp[0][calc(f[i],0,1)]=p[i];//上层是0，本层是它的概率 
					res[j]=res[j]*tmpp[1][1]+(1-res[j])*tmpp[0][1]+res[j]*(1-p[i]); 
				}
				else {
					tmpp[1][calc(f[i],1,0)]=p[i];//上层是1，本层是它的概率 
					tmpp[0][calc(f[i],0,0)]=p[i];//上层是0，本层是它的概率 
					res[j]=res[j]*tmpp[1][1]+(1-res[j])*tmpp[0][1]+res[j]*(1-p[i]); 
				}
			}
		}
		double ans=0;
		for(int i=0;i<=20;i++){
			ans+=1.0*res[i]*(1<<i);
		}
		printf("Case %d:\n",++cas);
		printf("%.6lf\n",ans);
	}
	return 0;	
}
```

