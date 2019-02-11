---
title: CF587C Duff in the Army（树上倍增_树链剖分）
date: 2018-08-30 13:16:15
tags: [树上问题,树上倍增,树链剖分]
---

#### 题目
有n个城市，由n-1条边连接。两个城市之间的道路是唯一的。 
有m个人，住在这n个城市中，现在给出m个人，生活的城市编号。 
你需要回答，从一个城市u到另一个城市v的路径中，编号前a小的人的编号是哪些？

#### 思路1（树上倍增）
树上倍增，可以开一个$num[i][j]$,表示i这个点向前跳$2^j-1$格后到达的点，这样，对于两个点而言，就可以向上跳跃，直到交汇为止，答案则是归并后的ans数组。

此题的切入点主要在$a<=10$这个条件，这样使我们每个vector只要保存最多10个元素就好了。

倍增这种数据结构，是树上问题求解的常用途径，遇到树的问题要优先考虑。
<!--more-->
#### 代码
```c++
#include<bits/stdc++.h>
#define M 100005
using namespace std;
int n,m,q,fa[M][20],dep[M],top[M],sz[M],son[M];
vector<int>G[M];
vector<int>cty[M];
vector<int>num[M][20];//倍增保存城市往上爬 
vector<int>ans;
vector<int> up(vector<int>&b,vector<int>&c){
	int i=0,j=0;
	vector<int>a;
	a.clear();
	while(i<b.size()&&j<c.size()&&a.size()<10){
		if(b[i]<c[j])a.push_back(b[i++]);
		else a.push_back(c[j++]);
	}
	while(i<b.size()&&a.size()<10)a.push_back(b[i++]);
	while(j<c.size()&&a.size()<10)a.push_back(c[j++]);
	return a;
}
void dfs(int x,int f,int d){
	dep[x]=d;
	fa[x][0]=f;fa[x][1]=fa[f][0];
	num[x][0]=cty[x];num[x][1]=up(cty[x],cty[f]);
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==f)continue;
		dfs(u,x,d+1);
	}
} 
int main(){
	scanf("%d%d%d",&n,&m,&q);
	for(int i=1,a,b;i<n;i++){
		scanf("%d%d",&a,&b);
		G[a].push_back(b);
		G[b].push_back(a);	
	}
	for(int i=1,a;i<=m;i++){
		scanf("%d",&a);
		cty[a].push_back(i);//已经自动有序 
	} 
	dfs(1,0,1);
	for(int j=2;j<=19;j++)
		for(int i=1;i<=n;i++){
			fa[i][j]=fa[fa[i][j-1]][j-1]; 
			num[i][j]=up(num[i][j-1],num[fa[i][j-1]][j-1]);
		}
	for(int i=1,u,v,a;i<=q;i++){
		scanf("%d%d%d",&u,&v,&a);
		ans.clear();
		if(dep[u]>dep[v])swap(u,v);
		int step=dep[v]-dep[u];
		for(int i=19;i>=0;i--)
			if(step&1<<i)ans=up(ans,num[v][i]),v=fa[v][i];
		if(u==v){
			ans=up(ans,num[v][0]);
			goto e;
		}
		for(int i=19;i>=0;i--){
			if(fa[u][i]!=fa[v][i]){
				ans=up(num[u][i],ans);
				ans=up(num[v][i],ans);
				u=fa[u][i];
				v=fa[v][i];
			}
		}
		ans=up(ans,num[u][0]);
		if(u!=v)ans=up(ans,num[v][1]);
		e:;
		int len=ans.size();
		len=min(len,a);
		printf("%d",len);
		for(int i=0;i<len;i++)
			printf(" %d",ans[i]);
		puts("");
	}
	return 0;
} 
```

#### 思路2（树链剖分）
从树剖的角度看，这就是一道裸题。

我们按轻重链破树成链，利用归并树，记录l,r区间内的前10个元素，最后查找一下就好了，对于树链剖分不是很熟悉的同学，可以看一下洛谷日报的[一篇文章](https://www.luogu.org/blog/communist/shu-lian-pou-fen-yang-xie)

还要注意的是这题迷之卡vector，czj大佬考场上就是因为这个没能AK，（tql），所以封装了一下。

```c++
#include<bits/stdc++.h>
#define M 100005
#define faa tree[p]
#define lsn tree[p<<1]
#define rsn tree[p<<1|1]
using namespace std;
struct arr{
	int Q[10],len;
	int size(){
		return len;
	}
	void push_back(int x){
		Q[len++]=x;
	}
	void clear(){
		for(int i=0;i<10;i++)
			Q[i]=0;
		len=0;
	}
};
int n,m,q;
//树链剖分 
vector<int>G[M];
vector<int>cty[M];
int son[M],sz[M],dep[M],ID[M],fa[M],ln[M],tt=0,top[M];
arr ans;
void dfs(int x,int f,int d){
	dep[x]=d;fa[x]=f;sz[x]=1;son[x]=0;
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==f)continue;
		dfs(u,x,d+1); 
		if(sz[u]>sz[son[x]])son[x]=u;
		sz[x]+=sz[u];
	}
}
void dfs_top(int x,int tp){
	top[x]=tp;ID[x]=++tt;ln[tt]=x;
	if(son[x])dfs_top(son[x],tp);
	for(int i=0;i<G[x].size();i++){
		int u=G[x][i];
		if(u==son[x]||u==fa[x])continue;
		dfs_top(u,u);
	}
}
struct node{
	int l,r;
	arr num;
}tree[M<<2];
arr up(arr &B,arr &C){
	arr A;
	A.clear();
	int i=0,j=0;
	while(i<B.size()&&j<C.size()&&A.size()<10){
		if(B.Q[i]<C.Q[j])A.push_back(B.Q[i++]);
		else A.push_back(C.Q[j++]);
	}
	while(i<B.size()&&A.size()<10)A.push_back(B.Q[i++]);
	while(j<C.size()&&A.size()<10)A.push_back(C.Q[j++]);
	return A;
}
void build(int l,int r,int p){
	tree[p].l=l;tree[p].r=r;tree[p].num.clear();
	if(l==r){
		for(int i=0;i<cty[ln[l]].size();i++){
			tree[p].num.push_back(cty[ln[l]][i]);
			if(tree[p].num.size()==10)break;
		}
	}
	else {
		int mid=l+r>>1;
		build(l,mid,p<<1);
		build(mid+1,r,p<<1|1);
		tree[p].num=up(tree[p<<1].num,tree[p<<1|1].num);
	}
}
void query(int l,int r,int p){
	if(tree[p].l==l&&tree[p].r==r){
		ans=up(ans,tree[p].num);
	}
	else {
		int mid=tree[p].l+tree[p].r>>1;
		if(r<=mid)query(l,r,p<<1);
		else if(l>mid)query(l,r,p<<1|1);
		else{
			query(l,mid,p<<1);
			query(mid+1,r,p<<1|1);
		}
	}
}
int main(){
	scanf("%d%d%d",&n,&m,&q);
	for(int i=1,a,b;i<n;i++){
		scanf("%d%d",&a,&b);
		G[a].push_back(b);
		G[b].push_back(a);
	}
	for(int i=1,a;i<=m;i++){
		scanf("%d",&a);
		cty[a].push_back(i);
	}
	dfs(1,0,1);
	dfs_top(1,1);
	build(1,n,1);
	for(int i=1,u,v,c;i<=q;i++){
		scanf("%d%d%d",&u,&v,&c);
		ans.clear();
		while(top[u]!=top[v]){
			if(dep[top[u]]>dep[top[v]]){
				query(ID[top[u]],ID[u],1);
				u=fa[top[u]];
			}
			else {
				query(ID[top[v]],ID[v],1);
				v=fa[top[v]];
			}
		} 
		if(dep[u]>dep[v])query(ID[v],ID[u],1);
		else query(ID[u],ID[v],1);
		int len=ans.size();
		len=min(len,c);
		printf("%d",len);
		for(int i=0;i<len;i++)
			printf(" %d",ans.Q[i]);
		puts("");
	}
	return 0;
}
```


