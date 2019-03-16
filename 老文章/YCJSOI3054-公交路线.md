#### 题目

到了周末,C校的同学都决定出去玩!

一共有$M$位同学,每位同学都有一个想到达的目的地,勤俭节约的他们都决定乘坐公交车到达.

C校的公交车一共有$T$条线路,$N$个站点.$N$个站点之间会构成一棵树.

现在给出站点的分布,以及$T$条路线的起点和终点,对于每个学生求出他回到家需要换多少次车才能回到家

如果无法到家,输出$−1$.



#### 思路

简化题目：

给你一棵树，和一些区间，一个点可以在区间中自由移动。问从$s$到$t$最多需要跨越多少区间。

一道很常规的树上题目。

像这种只有查询而没有修改的问题，一般往**倍增**上面想。

#### part1（一些预处理与链上的情况）

**一个小贪心：对于一个点对来说，一个在LCA之下的点显然在使用相同的切换次数时跳得越高越好。**

那么我们使用$F[i][j]$表示$i$这个点跳切换$2^j-1$次时，能够跳到的最高点。

思考如何处理$F[i][0]$,首先当读入一条路径时，我们先将$F[s][0]$,$F[t][0]$都设为其LCA，再dfs一遍。这时
$$
F[x][0]=F[y][0]\ y是x的儿子 \&\& dep[F[y][0]] 最小
$$


之后与正常的倍增是一样的。

那么现在我们就成功解决了链上的情况。



#### part2（正解）

显然现在已经可以求出$x$往上跳到某个点的最少步数了（在倍增数组上二分即可）

现在关键就是对于没有祖先关系的点对$x$,$y$,拐弯处是否需要换乘需要考虑。

![tu](https://i.loli.net/2018/10/29/5bd70a843a2e5.png)

下面我们面临两个问题：如何判-1，如何求出答案。

对于1，可以判断两个点是不是都能到达LCA，如果都可以，那么这一定是可行情况了，不然就是-1；

然后，如果已经确认不是-1了，那么还有图中所示的两种情况，一种是两者都冲向深度更浅的点，一种是能够“拐弯”，从而不需要在换乘了。

那么问题的核心就转变为了求拐点之间是否有路径覆盖。

转化一下，就是是否有一条路径的起点在$x$子树内，终点在$y$子树内。

## 显然的dfs作差啊

在进入$x$子树时统计一下，$y$子树中的点个数，出子树时，如果$y$子树中的点多了，说明$x$子树中一定有点更新了$y$子树中的点，存在覆盖拐点的路径。



#### 代码

```cpp
#include<bits/stdc++.h>
#define M 100005
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,m,T;
struct BIT{
	int C[M];
	BIT(){
		memset(C,0,sizeof(C));	
	}
	void add(int x,int d){
		while(x<=n){
			C[x]+=d;
			x+=(x&-x);
		}	
	}
	int sum(int x){
		int res=0;
		while(x){
			res+=C[x];
			x-=(x&-x);
		}
		return res;
	} 
}Tr;
int h[M],tot;
struct edge{
	int nxt,to;
}G[M<<1];
void Add(int a,int b){
	G[++tot]=(edge){h[a],b};
	h[a]=tot;	
}
int fa[M][18],dep[M],L[M],R[M],tt,F[M][18];//当前点换乘多少班最"高"(dep)能够到达的点 
int Ans[M];
bool ned[M];//当前询问是否需要加1 
void dfs(int x,int f,int d){
	dep[x]=d;L[x]=++tt;fa[x][0]=f;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f)continue;
		dfs(u,x,d+1);	
	}
	R[x]=tt;
}
void redfs(int x,int f){
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f)continue;
		redfs(u,x);
		if(dep[F[u][0]]<=dep[x]){
			if(dep[F[u][0]]<dep[F[x][0]])F[x][0]=F[u][0]; 
		}
	}
}
int up(int s,int t){
	for(int i=17;i>=0;i--){
		if(dep[F[s][i]]>dep[t])
			s=F[s][i];
	}
	return s;
}
int LCA(int a,int b){
	if(dep[a]>dep[b])swap(a,b);
	int step=dep[b]-dep[a];
	for(int i=17;i>=0;i--)
		if(step&1<<i)b=fa[b][i];
	if(a==b)return b;
	for(int i=17;i>=0;i--)
		if(fa[a][i]!=fa[b][i])
			a=fa[a][i],b=fa[b][i];
	return fa[a][0];
}
int Up(int s,int t){
	if(dep[F[s][17]]>dep[t])return -1;
	int step=0;
	for(int i=17;i>=0;i--){
		if(dep[F[s][i]]>dep[t]&&F[s][i]!=0){
			s=F[s][i];
			step|=(1<<i);
		}
	}
	return step;
}
vector<int>Q[M]; 
struct que{
	int to,id,res;
};
vector<que>q[M];
void dfs_ans(int x,int f){
	for(int i=0;i<(int)q[x].size();i++){
		int l=L[q[x][i].to],r=R[q[x][i].to];
		q[x][i].res=Tr.sum(r)-Tr.sum(l-1);
	}
	for(int i=0;i<(int)Q[x].size();i++)Tr.add(L[Q[x][i]],1);
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f)continue;
		dfs_ans(u,x);
	}
	for(int i=0;i<(int)q[x].size();i++){
		int l=L[q[x][i].to],r=R[q[x][i].to];
		if(Tr.sum(r)-Tr.sum(l-1)>q[x][i].res)ned[q[x][i].id]=0;
		else ned[q[x][i].id]=1;
	}
}	
int main(){
	cin>>n>>T>>m;
	for(int i=1,a,b;i<n;i++){
		scanf("%d%d",&a,&b);
		Add(a,b);Add(b,a);
	}
	dfs(1,0,0);dep[0]=2e9;
	for(int j=1;j<=17;j++)
		for(int i=1;i<=n;i++)
			fa[i][j]=fa[fa[i][j-1]][j-1];
	for(int i=1,s,t;i<=T;i++){
		scanf("%d%d",&s,&t);
		int lca=LCA(s,t);
		if(dep[lca]<dep[F[s][0]])F[s][0]=lca;
		if(dep[lca]<dep[F[t][0]])F[t][0]=lca; 
		if(lca!=s&&lca!=t){
			Q[s].push_back(t);
			Q[t].push_back(s);
		}
	}
	redfs(1,0); 
	for(int j=1;j<=17;j++)
		for(int i=1;i<=n;i++)
			F[i][j]=F[F[i][j-1]][j-1];
	for(int i=1,s,t;i<=m;i++){
		scanf("%d%d",&s,&t);
		if(dep[s]>dep[t])swap(s,t);
		int lca=LCA(s,t);
		if(s==lca){
			Ans[i]=Up(t,lca);
		} 
		else {
			if(Up(s,lca)==-1||Up(t,lca)==-1){Ans[i]=-1;continue;}
			int u1=up(s,lca),u2=up(t,lca);
			Ans[i]=Up(s,lca)+Up(t,lca);
			q[u1].push_back((que){u2,i});
		}
	}
	dfs_ans(1,0);
	for(int i=1;i<=m;i++){
		printf("%d\n",Ans[i]+ned[i]);
	}
	return 0;
}
```

