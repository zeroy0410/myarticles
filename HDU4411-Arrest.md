---
title: HDU4411 Arrest
date: 2018-08-30 12:58:50
tags: [dp,树形dp]
---

#### 题目
有n个城市，构成一棵树。

现在有一些罪犯越狱,需要安排警察抓捕。罪犯的位置不确定。 

求安排最少的警察，一定能抓到任何可能下的罪犯。只有警察和罪犯在同一点上就能抓住，无论在城市中，还是在路上。警察可以守在某个点，或者移动！罪犯也可能不动，或者移动！
<!--more-->
#### 思路
换句话而言，对于每一个路口，一定要有一个警察堵住，剩下的警察前去抓捕罪犯，或者一个警察顺着这个路口抓捕到所有的罪犯。

感性的来看，对于一个分叉来说，我们要派一个警察守住，之后所有的警察去搜最大的路口。如果最大的路口的个数是1，那么就没有必要守住这个分叉了。

#### 代码
```c++
#include<bits/stdc++.h>
#define M 1005
#define clr(x,y) memset(x,y,sizeof(x)) 
using namespace std;
vector<int>G[M];
int n;
int dp[M],ans,root;//此点之最大情况
//保证每个点下去抓小偷的时候，父结点至少有一个点在守着 
void dfs(int x,int f){
    int cnt=0;
    for(int i=0;i<G[x].size();i++){
        int u=G[x][i];
        if(u==f)continue;
        dfs(u,x);
        dp[x]=max(dp[x],dp[u]);
    } 
    for(int i=0;i<G[x].size();i++){
    	int u=G[x][i];
    	if(u==f)continue;
    	if(dp[x]==dp[u])cnt++;
	}
    if(cnt>1)dp[x]++;
}
int main(){
    while(~scanf("%d",&n)){
        ans=2e9;
        for(int i=1;i<=n;i++)G[i].clear();
        for(int i=1,a,b;i<n;i++){
            scanf("%d%d",&a,&b);
            G[a].push_back(b);
            G[b].push_back(a);
        }
        for(int i=1;i<=n;i++){
            clr(dp,0);
            root=i;
            dfs(i,0);
            ans=min(ans,dp[i]+1);
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
