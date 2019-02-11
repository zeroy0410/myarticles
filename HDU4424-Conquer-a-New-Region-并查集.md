---
title: HDU4424 Conquer a New Region(并查集)
date: 2018-08-30 12:52:18
tags: 并查集
---

#### 题目
给定一棵无根树，求以树上某一点为根，使得这个点到其他点路径上的边权最小值之和最大。

#### 思路
很容易想到使用类克鲁斯卡尔的算法，将边从大到小进行排序，之后利用并查集维护，则通过某条边连接起来的两个集合，他们之间的点的路径的最小值就是此边。

但信息的维护就非常棘手了，一开始想到的是带权并查集维护每一个点的答案，但是它貌似又不满足带权并查集的性质（disr2==0,信息不会被重复更新到），因此卡了一万年。
<!--more-->
后来根据**dalaoWZY**的提示，发现其实只要维护一个集合中的最优解，并把它放在根结点的信息上就可以了，因为本题要求的只是求取这个最优解，并不要求输出这个最优解在哪个结点上，于是过了。

#### 代码
```c++
#include<bits/stdc++.h>
#define M 200005 
#define LL long long
using namespace std;
struct node{
	int fr,to,co;
	bool operator < (const node& res) const {
		return co>res.co;
	}
};
int n;
int fa[M],sz[M];
LL dis[M];
int getfa(int x){
	return fa[x]==x?x:fa[x]=getfa(fa[x]);
}
node G[M];
int main(){
	while(~scanf("%d",&n)){
		for(int i=1;i<=n;i++)fa[i]=i,sz[i]=1,dis[i]=0;
		for(int i=1,a,b,c;i<=n-1;i++){
			scanf("%d%d%d",&a,&b,&c);
			G[i]=(node){a,b,c};
		}
		sort(G+1,G+n);
		for(int i=1;i<=n-1;i++){
			int n1=G[i].fr,n2=G[i].to,v=G[i].co;
			//因为当前边的连通所相互连接的点，此边一定是边权最短的边
			int r1=getfa(n1),r2=getfa(n2);
			if(r1!=r2){
				dis[r2]=max(dis[r2]+1LL*sz[r1]*v,dis[r1]+1LL*sz[r2]*v);
				sz[r2]+=sz[r1];
				sz[r1]=0;
				fa[r1]=r2;
			}
			else break;
		}
		LL ans=0;
		int k=getfa(1);
		ans=dis[k];
		printf("%lld\n",ans);
	}
	return 0;
}
```

。。。

后来发现，其实输出答案也很简单，只要将每个点中的最优解作为根结点保存就好了，最后getfa(1)就是答案了。