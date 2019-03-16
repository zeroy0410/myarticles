#### 题目

[题面](http://acm.hdu.edu.cn/showproblem.php?pid=4871)

大意是给你一个图，要你构建出一棵最短路树，再询问经过k个点的最长路径长度以及最长路径条数。



#### 思路

点分治。
点分治的思路是这样的：
对于一个点$x$而言，对答案有影响的路径要么经过点$x$要么不经过，利用这点进行分治。
点分治首先要找出一个重心。重心是指以该点为根所有的子树中sz最大的最小。
然后，对于每一棵子树进行处理，统计答案（点分治题目不同的地方就在这，其它都是板子）
之后再递归进重心的子树中处理。

对于一个根来说，如果统计答案的复杂度是$O(n)$，那么总的复杂度就是$O(nlogn)$.

#### 代码
```cpp
#include<bits/stdc++.h>
#define M 30005
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int T,n,m,K;
int g[M],tt;
int ansa=0,ansc=0;//最长路，最长路的种数 
struct Node{
	int to,co;
	bool operator < (const Node& res) const{
		return to<res.to;	
	}
};
vector<Node>G[M]; 
struct edge{
	int nxt,to,co;	
}H[M<<1];
void add(int a,int b,int c){
	H[++tt]=(edge){g[a],b,c};
	g[a]=tt;
}
struct node{
	int x,ds;
	bool operator < (const node& res) const{
		return ds>res.ds;	
	}
};
priority_queue<node>Q;
int dis[M];
bool vis[M];
void Dij(){
	clr(dis,0x3f);clr(vis,0);
	dis[1]=0;
	Q.push((node){1,0});
	while(!Q.empty()){
		node e=Q.top();Q.pop();
		if(vis[e.x])continue;vis[e.x]=1;
		for(int i=0;i<(int)G[e.x].size();i++){
			int u=G[e.x][i].to,v=G[e.x][i].co;
			if(dis[u]>dis[e.x]+v){
				dis[u]=dis[e.x]+v;
				Q.push((node){u,dis[u]});	
			}
		}
	}
}
bool solved[M];
int sz[M],ttot;
void dfs(int x,int f,int ds){//构造树 
	for(int i=0;i<(int)G[x].size();i++){
		int u=G[x][i].to,v=G[x][i].co;
		if(u==f||vis[u]||ds+v>dis[u])continue;
		vis[u]=1;
		add(u,x,v);add(x,u,v);
		dfs(u,x,ds+v);
	}
}
int mi=1e9,zx=1;
void dfs_zx(int x,int f){//寻找重心 
	sz[x]=1;int mm=0;
	for(int i=g[x];i;i=H[i].nxt){
		int u=H[i].to;
		if(u==f||solved[u])continue;
		dfs_zx(u,x);
		sz[x]+=sz[u];
		mm=max(mm,sz[u]);
	}
	mm=max(mm,ttot-sz[x]);
	if(mm<mi)mi=mm,zx=x;
}
void find_zx(int rt){
	mi=1e9,zx=1;
	dfs_zx(rt,0);
}
void getans(int x,int y){
	if(x>ansa)ansa=x,ansc=y;
	else if(x==ansa)ansc+=y;
}
int mx[M],mn[M];
int num[M],siz=0;
void dfs_ans(int x,int f,int ds,int c){
	dis[++siz]=ds;num[siz]=c;
	for(int i=g[x];i;i=H[i].nxt){
		int u=H[i].to,v=H[i].co;
		if(solved[u]||u==f)continue;
		dfs_ans(u,x,ds+v,c+1);	
	}
}
void solve(int rt){
	find_zx(rt);
	solved[zx]=1;
	for(int i=0;i<=K;i++)mx[i]=mn[i]=0;mn[0]=1;
	for(int i=g[zx];i;i=H[i].nxt){
		int u=H[i].to,v=H[i].co;
		if(solved[u])continue;
		siz=0;
		dfs_ans(u,zx,v,1); 
		for(int j=1;j<=siz;j++){
			if(num[j]>=K)continue;
			int tmp=K-1-num[j];
			getans(mx[tmp]+dis[j],mn[tmp]);
		}
		for(int j=1;j<=siz;j++){
			if(num[j]>=K)continue;
			if(dis[j]>mx[num[j]]){
				mx[num[j]]=dis[j];
				mn[num[j]]=1;
			}
			else if(dis[j]==mx[num[j]])mn[num[j]]++;
		}
	}
	ttot--;
	for(int i=g[zx];i;i=H[i].nxt){
		int u=H[i].to;
		if(solved[u])continue;
		solve(u);
	}
}
int main(){
	cin>>T;
	while(T--){
		clr(g,0);clr(solved,0);tt=ansa=ansc=0;
		scanf("%d%d%d",&n,&m,&K);ttot=n;
		for(int i=1;i<=n;i++)G[i].clear(); 
		for(int i=1,a,b,c;i<=m;i++){
			scanf("%d%d%d",&a,&b,&c);
			G[a].push_back((Node){b,c});
			G[b].push_back((Node){a,c});
		}
		for(int i=1;i<=n;i++)sort(G[i].begin(),G[i].end()); 
		Dij();
		clr(vis,0);dfs(1,0,0);
		solve(1);
		printf("%d %d\n",ansa,ansc);
	}
	return 0;
}
```