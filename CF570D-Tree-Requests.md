---
title: CF570D_Tree_Requests
date: 2018-08-30 13:13:39
tags: dsu_on_tree
---

#### 题目
一棵以1为根的树，每个节点上都有1个字母，有m个询问。每次询问v对应的子树中，深度为h的这层节点的字母，能否打乱重排组成回文串。根的深度为1，每个点的深度为到根的距离。

#### 思路1(DSU)
这应该是比较无脑的一种思路，开一个$cnt[M][i]$记录i这个颜色在某个深度出现了多少次，再开一个tmp记录对于这个深度，cnt是奇数的个数（如果在某个深度，奇数的个数是0或1，那么其就能组成回文串）
<!--more-->
```c++
#include<bits/stdc++.h>
#define M 500005
using namespace std;
struct que{
	int id,h;
};
vector<que>Q[M];
vector<int>G[M];
int dep[M],n,m;
char S[M+5];
int A[M];
int cnt[M][26],L[M],R[M],ln[M],tt=0,sz[M],son[M];
int tmp[M];//表示本层中奇数的个数 
bool ans[M];
void dfs(int x,int d){
	dep[x]=d;
	L[x]=++tt;
	ln[tt]=x;son[x]=0;sz[x]=1;
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		dfs(u,d+1);
		if(sz[u]>sz[son[x]])son[x]=u;
		sz[x]+=sz[u];
	}
	R[x]=tt;
}
void Add_point(int x,int d){//x表示点 
	if(cnt[dep[x]][A[x]]&1)tmp[dep[x]]--;
	if(!(cnt[dep[x]][A[x]]&1))tmp[dep[x]]++;
	cnt[dep[x]][A[x]]+=d;
}
void Add_tree(int x,int d){
	for(int i=L[x];i<=R[x];i++)
		Add_point(ln[i],d);
}
void DSU(int x){
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==son[x])continue;
		DSU(u);Add_tree(u,-1);
	}
	if(son[x])DSU(son[x]);
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==son[x])continue;
		Add_tree(u,1);
	}
	Add_point(x,1);
	for(int i=0;i<Q[x].size();i++){
		int id=Q[x][i].id,h=Q[x][i].h;
		ans[id]=((tmp[h]==1)||(tmp[h]==0));
	}
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=2,a;i<=n;i++){
		scanf("%d",&a);
		G[a].push_back(i);
	}
	scanf("%s",S+1);
	for(int i=1;i<=n;i++)
		A[i]=S[i]-'A';
	for(int i=1,v,h;i<=m;i++){
		scanf("%d%d",&v,&h);
		Q[v].push_back((que){i,h});
	}
	dfs(1,1);
	DSU(1);
	for(int i=1;i<=m;i++){
		if(ans[i])puts("Yes");
		else puts("No");
	}
	return 0;
}
///
```

#### 思路2（dfs作差）
这种做法优雅且巧妙，对于统计子树信息的问题，在进入子树之前统计一个，之后，统计一个，二者作差即为子树内的信息。

对于题目认真分析，我们可以发现，表示某种颜色可以使用二进制，如果是1即表明是奇数，0则表明是偶数，对于新加入的颜色，亦或即可。

又亦或满足前缀和的性质，dfs作差就顺理成章了。

```c++
#include<bits/stdc++.h>
#define M 500005
using namespace std;
struct que{
	int id,h;
};
vector<que>Q[M];
vector<int>G[M];
int n,m;
char S[M+5];
int A[M];
int cnt[M];
bool ans[M];
void dfs(int x,int d){
	vector<int>tmp;tmp.clear();
	for(int i=0;i<Q[x].size();i++)tmp.push_back(cnt[Q[x][i].h]);
	cnt[d]^=(1<<A[x]);
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		dfs(u,d+1);
	}
	for(int i=0;i<Q[x].size();i++){
		int k=cnt[Q[x][i].h]^tmp[i];
		if(k==0||k==(k&-k))ans[Q[x][i].id]=1;
		else ans[Q[x][i].id]=0;
	}
	tmp.clear();
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=2,a;i<=n;i++){
		scanf("%d",&a);
		G[a].push_back(i);
	}
	scanf("%s",S+1);
	for(int i=1;i<=n;i++)
		A[i]=S[i]-'A';
	for(int i=1,v,h;i<=m;i++){
		scanf("%d%d",&v,&h);
		Q[v].push_back((que){i,h});
	}
	dfs(1,1);
	for(int i=1;i<=m;i++){
		if(ans[i])puts("Yes");
		else puts("No");
	}
	return 0;
}
```

#### 总结

对于一个问题，思考有一个由浅入深的过程，能用简单算法的尽量用简单算法，就如本题，思路2显然更加优美。


