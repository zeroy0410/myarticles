---
title: HDU5481 Desiderium
date: 2018-09-22 15:35:40
tags: [DP,期望dp]
---

#### 题目

有一条数轴，还有一个区间的集合，集合大小为n。 现在等概率的从集合中选出集合的一个子集，求取出的子集的区间并集的期望长度。 空集的区间并长度被认为是0。答案乘以2^n后对1e9+7求余

<!--more-->

#### 思路

首先肯定是要离散化，然后单独考虑每一块最终对答案的贡献。

那么每一块的贡献是多少呢？我们先算出一共有多少个集合包含了这一块的区间，那么最终的贡献就是
$$
\sum_{i=1}^{cnt} 这些线段中有i条被取到的种数
$$
那么只用预处理一个$sum[]$数组，表示当前覆盖数的种数就好了。

具体看代码：

```c++
#include<bits/stdc++.h>
#define M 100005
#define MOD 1000000007
#define LL long long
using namespace std;
int TT,n; 
struct node{
	int l,r;
}A[M];
LL qkpow(LL a,LL b){
	LL base=a,res=1;
	while(b){
		if(b&1)res*=base,res%=MOD;
		base*=base;base%=MOD;
		b>>=1;
	}
	return res;
}
struct so{
	int x,f;
	bool operator < (const so& res) const{
		return x<res.x;
	}
}Q[2*M];
int qcnt=0;
LL cf[M*2];
LL mp[M*2];
LL sum[M*2];
int main(){
	cin>>TT;
	while(TT--){
		memset(mp,0,sizeof(mp));
		memset(cf,0,sizeof(cf));
		scanf("%d",&n);qcnt=0;
		for(int i=1;i<=n;i++){
			scanf("%d%d",&A[i].l,&A[i].r);
			Q[++qcnt].x=A[i].l;Q[qcnt].f=0;
			Q[++qcnt].x=A[i].r;Q[qcnt].f=1;
		}
		sort(Q+1,Q+qcnt+1);
		sum[1]=qkpow(2,n-1);
		for(int i=2;i<=n;i++)sum[i]=(sum[i-1]+qkpow(2,n-i))%MOD;
		int q=0;Q[0].x=-2e9;
		for(int i=1;i<=qcnt;i++){
			if(Q[i].x!=Q[i-1].x)q++;
			mp[q]=Q[i].x;
			if(Q[i].f==0)cf[q]++;
			else if(Q[i].f==1)cf[q]--;
		}
		LL ans=0,res=0;
		for(int i=1;i<=q;i++){
			res+=cf[i];
			ans+=sum[res]*(mp[i+1]-mp[i]);
			ans%=MOD;
		}
		cout<<ans<<endl;
	}
	return 0;	
}
```

