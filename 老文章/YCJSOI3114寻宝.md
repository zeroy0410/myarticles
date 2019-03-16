#### 题目

冒险岛由$n$个岛屿组成,用$n−1$座桥梁连接,保证每座岛屿可以相互抵达。小C得到一份藏宝信息,有$m$个藏宝室在这$n$个岛屿中。已知这$m$个藏宝室的位置以及打开每个藏宝室的钥匙在哪座岛屿。

小C可以通过传送阵到达一座岛,然后开始寻宝,每座岛被拿走宝藏之后就会消失(等小C离开后消失),小C可以在某个岛利用传送阵离开。可以认为整个寻宝过程,就是路过从一座岛到达另一座岛的简单路径上所有的岛。

每到达一个岛之后,**一定会**先收集钥匙,**一定会**再打开可以打开的藏宝室。到得到的宝藏可能为负数,表示必须缴纳一定的钱才能离开藏宝室。请你帮小C计算,最多可以获得钱数。

#### 思路

前面那些dalao已经讲得很详细了。这里将几种分类的情况简单汇总一下：
这里线段树维护的是以某一段区间为起点的val值。
终点则在当前dfs到的那个点上。
将需要发送给线段树的命令量化，在退出当前子树时消除影响。

这里约定$x$为终点，$y$为起点。

**1.当x!=y并且x与y不具有祖先关系的时候。更新另一棵子树中的点的影响。update(L[y],R[y],val)**

**2.当x!=y并且两者具有祖先关系的时候。**

1.当y是x的父亲节点
跳到y的下一层节点u，
update(1,L[u]-1,val),update(R[u]+1,n,val)

2.当x是y的父亲节点

此种情况较为特殊，我们要一开始就将y的影响加入，当进入u子树的时候把影响消除，离开子树的时候再把影响加入。

**3.x==y**

这是最毒瘤的一种情况，在$x$对应的子树外，起点区间为$[L[x],R[x]]$,在$x$对应的子树中，起点区间除了子树外的点之外，还有当前子树之于$x$而言的兄弟子树。

```cpp
#include<bits/stdc++.h>
#define M 100005
#define fa tree[p]
#define lson tree[p<<1]
#define rson tree[p<<1|1]
using namespace std;
int n,m;
int h[M],tot;
struct tre{int ky,v;};
vector<tre>tr[M];//treasure
struct node{int l,r,v;};
vector<node>dif[M];
int equ[M];//挂下每个点x==y时的val 
struct edge{
	int nxt,to;
}G[M<<1];
void Add(int a,int b){
	G[++tot]=(edge){h[a],b};
	h[a]=tot;
}
struct YD_tree{
	//区间加值，查询区间最大值 
	struct YD{
		int l,r,mx,add;
	}tree[M<<2];
	void build(int l,int r,int p){
		fa.l=l;fa.r=r;fa.add=0;fa.mx=0;
		if(l==r)return;
		int mid=(l+r)>>1;
		build(l,mid,p<<1);
		build(mid+1,r,p<<1|1);
	}
	void down(int p){
		if(fa.add==0)return;
		lson.add+=fa.add;
		lson.mx+=fa.add;
		rson.add+=fa.add;
		rson.mx+=fa.add;
		fa.add=0;
	}
	void up(int p){
		fa.mx=max(lson.mx,rson.mx);
	}
	void update(int l,int r,int d,int p){
		if(l>r)return;
		if(fa.l==l&&fa.r==r){
			fa.add+=d;
			fa.mx+=d;
		}	
		else {
			down(p);
			int mid=(fa.l+fa.r)>>1;
			if(r<=mid)update(l,r,d,p<<1);
			else if(l>mid)update(l,r,d,p<<1|1);
			else {update(l,mid,d,p<<1);update(mid+1,r,d,p<<1|1);}
			up(p);
		}
	}
}T; 
#undef fa
int tt=0,L[M],R[M],dep[M],ans=0,ln[M],fa[M][20];
void dfs(int x,int f,int d){
	L[x]=++tt;ln[tt]=x;fa[x][0]=f;dep[x]=d;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f)continue;
		dfs(u,x,d+1);
	}
	R[x]=tt;
}
int Up(int x,int step){
	for(int i=17;i>=0;i--)
		if(step&1<<i)x=fa[x][i];
	return x;
}
//如果没有父亲的关系，直接在线段树上更新，如果具有父子的关系，那么说明 
void redfs(int x,int f){
	for(int i=0;i<(int)dif[x].size();i++){
		int l=dif[x][i].l,r=dif[x][i].r,v=dif[x][i].v;
		T.update(l,r,v,1);
	}
	ans=max(ans,T.tree[1].mx);
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==f)continue;
		T.update(L[x]+1,L[u]-1,equ[x],1);
		T.update(R[u]+1,R[x],equ[x],1);	
		redfs(u,x);	
		T.update(L[x]+1,L[u]-1,-equ[x],1);
		T.update(R[u]+1,R[x],-equ[x],1);	
	}
}
int main(){
//	freopen("treasure.in","r",stdin);
	cin>>n>>m;tt=0;T.build(1,n,1);
	for(int i=1,a,b;i<n;i++){
		scanf("%d%d",&a,&b);
		Add(a,b);Add(b,a);	
	}
	for(int i=1,x,y,v;i<=m;i++){
		scanf("%d%d%d",&x,&y,&v);
		tr[y].push_back((tre){x,v});
	}
	dfs(1,0,0);
	for(int j=1;j<=17;j++)
		for(int i=1;i<=n;i++)
			fa[i][j]=fa[fa[i][j-1]][j-1];
	for(int i=1;i<=n;i++){
		for(int j=0;j<(int)tr[i].size();j++){
			int x=i,y=tr[i][j].ky,v=tr[i][j].v;
			if(x==y){
				dif[1].push_back((node){L[x],R[x],v});
				dif[x].push_back((node){L[x],R[x],-v});
				dif[ln[R[x]+1]].push_back((node){L[x],R[x],v});
				dif[x].push_back((node){1,n,v});
				dif[ln[L[x]+1]].push_back((node){1,n,-v});
				dif[ln[L[x]+1]].push_back((node){1,L[x],v});
				dif[ln[L[x]+1]].push_back((node){R[x]+1,n,v});
				dif[ln[R[x]+1]].push_back((node){1,L[x],-v});
				dif[ln[R[x]+1]].push_back((node){R[x]+1,n,-v});
				equ[x]+=v; 
			}
			else if(L[x]>=L[y]&&L[x]<=R[y]){//y是x的祖先 
				int u=Up(x,dep[x]-dep[y]-1);
				dif[x].push_back((node){1,L[u]-1,v});
				dif[x].push_back((node){R[u]+1,n,v});
				dif[ln[R[x]+1]].push_back((node){1,L[u]-1,-v});
				dif[ln[R[x]+1]].push_back((node){R[u]+1,n,-v});
			}
			else if(L[y]>=L[x]&&L[y]<=R[x]){//x是y的祖先 
				int u=Up(y,dep[y]-dep[x]-1);
				dif[1].push_back((node){L[y],R[y],v});
				dif[u].push_back((node){L[y],R[y],-v});
				dif[ln[R[u]+1]].push_back((node){L[y],R[y],v});
			}
			else {//没有祖先关系 
				dif[x].push_back((node){L[y],R[y],v});
				dif[ln[R[x]+1]].push_back((node){L[y],R[y],-v});	
			}
		}
	}
	redfs(1,0);
	printf("%d\n",ans);
	return 0;
}
```