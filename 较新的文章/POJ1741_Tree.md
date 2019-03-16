#### 题目

给一棵边带权树，问两点之间<=K的点对有多少个。



#### 思路

题目很简单，但是思路很经典。

首先确定点分治的基本框架，假设一定要经过一个根。

下面还要用到容斥的思维。

对于一个根，我们没法直接统计路径长度不超过k的路径条数，那需要一点技巧。

处理出子树中所有的dis值放入B数组中，再对于当前A子树，加上其对于B所做的贡献，减去其对自身所做的贡献，就是它对其他子树所做的贡献。

看似简单，其实在点分治问题中这是非常常用的技巧。

#### 代码

```cpp
#include<stdio.h>
#include<algorithm>
#include<string.h>
#include<iostream>
#define M 10005
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,K,h[M],tot;
struct edge{
	int nxt,to,co;
}G[M<<1];
void Add(int a,int b,int c){
	++tot;
	G[tot].nxt=h[a],G[tot].to=b,G[tot].co=c;
	h[a]=tot;
}
int sz[M],mx=2e9,zx,tt,ans;
bool vis[M];
void dfs_zx(int x,int f){
	sz[x]=1;int mm=0;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f||vis[u])continue;
		dfs_zx(u,x);
		sz[x]+=sz[u];
		if(sz[u]>mm)mm=sz[u];
	}
	mm=max(mm,tt-sz[x]);
	if(mm<mx)mx=mm,zx=x;
}
void find_zx(int x){
	mx=2e9;
	dfs_zx(x,0);
}
int A[M],top,B[M],top2;
void dfs(int x,int f,int ds){
	A[++top]=ds;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to,v=G[i].co;
		if(vis[u]||u==f)continue;
		dfs(u,x,ds+v);
	}
}
int find(int *s,int l,int r,int x){
	s[r]=2e9;
	int as=-1;
	while(l<=r){
		int mid=(l+r)>>1;
		if(s[mid]>x){
			as=mid;
			r=mid-1;	
		}
		else l=mid+1;
	}
	if(as==-1)as=1;
	return as-1;
}
void solve(int x){
	find_zx(x);
	x=zx;
	vis[x]=1;
	top=0;
	dfs(x,0,0);top2=0;
	for(int i=2;i<=top;i++)B[++top2]=A[i];
	sort(B+1,B+top2+1);
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to,v=G[i].co;
		if(vis[u])continue;
		top=0;dfs(u,x,v);
		sort(A+1,A+top+1);
		for(int j=1;j<=top;j++)ans+=find(B,1,top2+1,K-A[j])-find(A,1,top+1,K-A[j]);	
		ans+=find(A,1,top+1,K)*2;
	}
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(vis[u])continue;
		tt=sz[u];
		solve(u);
	}
}
int main(){
	while(scanf("%d%d",&n,&K),n+K){
		for(int i=1;i<=n;i++)h[i]=vis[i]=0;		
		tot=ans=0;tt=n;
		for(int i=1,a,b,c;i<n;i++){
			scanf("%d%d%d",&a,&b,&c);
			Add(a,b,c);Add(b,a,c);
		}
		solve(1);
		printf("%d\n",ans/2);
	}
	return 0;	
}
```

