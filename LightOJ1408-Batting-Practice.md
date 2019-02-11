---
title: LightOJ1408 Batting Practice
date: 2018-09-22 15:26:11
tags: [DP,期望dp]
---

#### 题目

一个人在练习棒球，击不中的概率为p。如果连续k1次击中，或者连续k2 次击不中，就停止练习。求练习的期望次数。

<!--more-->

#### 思路

推式子。

设$W_i$表示已经连续击中了$i$次最终完成目标的期望步数。

$L_i$表示已经连续击不中$i$次最终完成目标的期望步数。

那么不难发现以下状态转移方程：
$$
W_i=(W_{i+1}+1)×p+(L_1+1)×(1-p)
$$

$$
L_i=(L_{i+1}+1)×(1-p)+(W_1+1)×p
$$

而且我们已知$W_{k1}=0$,$L_{k2}=0$

之后把这个式子展开，将所有的量都用$$L_1$$表示，最后就可以求出$L_1$，那么所有的量就可以知道了。

#### 代码

```c++
#include<bits/stdc++.h>
#define M 55
using namespace std;
int T,k1,k2;
long double p;//击中的概率
struct node{
	long double k;
	long double con;
}W[M],L[M];
long double L_1,W_1;
int main(){
	cin>>T;
	int cas=0;
	while(T--){
		scanf("%Lf%d%d",&p,&k1,&k2);
		p=1.0-p;
		W[k1].k=0;W[k1].con=0;
		for(int i=k1-1;i>=1;i--){
			W[i].k=p*(W[i+1].k)+1-p;//倍数项 
			W[i].con=p*W[i+1].con+1;//常数项 
		}
		L[k2].k=L[k2].con=0;
		for(int i=k2-1;i>=1;i--){
			L[i].k=L[i+1].k*(1-p)+W[1].k*p;
			L[i].con=L[i+1].con*(1-p)+1+W[1].con*p;	
		}
		L_1=1.0*L[1].con/(1-L[1].k);
		W_1=W[1].k*L_1+W[1].con;
		long double ans=(L_1+1)*(1-p)+(W_1+1)*p;
		printf("Case %d: %.10Lf\n",++cas,ans);
	}
	return 0;
}
```

