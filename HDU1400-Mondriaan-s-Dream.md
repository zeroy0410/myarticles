---
title: HDU1400 Mondriaan's Dream
date: 2018-08-30 13:02:58
tags: [dp,状压dp]
---


#### 题目

用1*2的矩形填满n * m的方格，求方案数。

#### 思路

由于是状压的专题，所以很自然的想到状压，我们可以这样编码，对于是竖着的，且会影响下一层的方块，我们使用1将其标记，其余使用0

#### exam：

对于样例第一层：00100001100.

这样本层对应的二进制数j与上一层的k之间就不能在同样的位子上是1。且j^k的值所产生的0，其对应的位置就是本层的横放的格子。所以连续的0格子必须是偶数。
<!--more-->
#### 代码

```c++
#include<bits/stdc++.h>
#define clr(haha,a) memset(haha,(a),sizeof(haha))
#define LL long long
using namespace std;
LL dp[15][1<<11];
int n,m;
bool check(int s){
	int cnt=0;bool ok=0;
	int k=m;
	while(k--){
		int tmp=(s&1);
		if(tmp){
			if(cnt&1)return false;
			cnt=0;
		}
		else cnt++;
		s>>=1;
	}
	return !(cnt&1);
}
int main(){
	//0为横着,1为竖着 
	while(scanf("%d%d",&n,&m)&&n&&m){
		if(((n*m)&1)){
			puts("0");
			continue;
		}
		clr(dp,0);
		dp[0][0]=1;
		for(int i=1;i<=n;i++)
			for(int j=0;j<1<<m;j++)//上层是1本层只能为0 
				for(int k=0;k<1<<m;k++)
					if(((j&k)==0)&&check(j^k))
						dp[i][j]+=dp[i-1][k];
		cout<<dp[n][0]<<endl;
	} 
	return 0;
}
```
