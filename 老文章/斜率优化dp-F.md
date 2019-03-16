#### 题目

给一个$N$个点的有根树，每条边有一个长度。从一个点出发，需要一个准备时间$S_i$，有一个速度，表示每个单位路程需要的时间$V_i$。去根的路上，会经过其他点$ j$，可以选择花费$S_j$的时间准备后，按$V_j​$的速度再次启程。 除了根节点，1号为根节点。出发时没有速度,也需要花费时间来调整速度。

#### 思路

首先dp状态还是比较显然的，
$$
dp[x]表示当前点选做起始点的最优解
$$
那么不难定义出转移：
$$
dp[x]=min(dp[j]-V[x]*dis[j])+S[x]+V[x]*dis[x]
$$
令：
$$
x=dis[j]\\\
y=dp[j] \\\
k=V[x]
$$
然后就是板子了。

剩下的只有一点很棘手：我们要维护一个从x到1的dfs栈的凸包，因为K不是单调的，所以只能用二分，这一部分的代码不复杂：

```cpp
int find(LL K,int r){//当前K的最优解 
	int l=2,ans=1;//判断当前点是否比前面的点优，如果是的，那么ans=它 
	while(l<=r){
		int mid=(l+r)>>1;
		if(cmp(Q[mid-1],Q[mid],K)){
			ans=mid;
			l=mid+1;
		}
		else r=mid-1;
	}
	return ans;
}
```

下面我们就要考虑这个凸包的维护了。每次遍历到一个点的时候，按照常规的做法，都要把队列中的元素弹出，然而这是树上的问题，子树之间会相互影响，所以不能真的弹掉，我们再用一个二分，找到如果要弹的话，当前点应该加入到什么位置，并把这个位置记录下来，等到退栈的时候再把当前点换回原来的点，这样就可以保证还原栈的复杂度了。

#### 代码

```cpp
#include<bits/stdc++.h>
#define M 100005
#define LL long long
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int h[M],tot;
struct edge{
	int nxt,to;
	LL co;
}G[M<<1];
void Add(int a,int b,LL c){
	G[++tot]=(edge){h[a],b,c};
	h[a]=tot;
}
struct node{
	LL x,y;
}Q[M];
bool cmp(node a,node b,LL K){//b比a优 
	return 1.0*b.y-1.0*b.x*K<=1.0*a.y-1.0*a.x*K;
}
bool check(node a,node b,node c){//c,a斜率>a,b 
	return 1.0*(c.y-a.y)*(a.x-b.x)<=1.0*(a.y-b.y)*(c.x-a.x);
}
int n,top=0;
LL dis[M];
LL dp[M];//从当前点出发（需要蓄力），到最后的最优时间
LL S[M],V[M];
/*
dp[x]=min(dp[stk[i]]-V[x]*dis[stk[i])+S[x]+V[x]*dis[x]
x=dis[stk[i]]
y=dp[stk[i]] 
k=V[x]
*/
int Find(node tmp,int l,int r){//当前点应该放在序列的哪个位置 (斜率大于它的第一个点)
	int ans=r+1;
	while(l<=r){
		int mid=(l+r)>>1;
		if(check(Q[mid],Q[mid-1],tmp)){
			ans=mid;
			r=mid-1;
		}else l=mid+1;
	}
	return ans;//找不到的话就新开一个 
}
int find(LL K,int r){//当前K的最优解 
	int l=2,ans=1;//判断当前点是否比前面的点优，如果是的，那么ans=它 
	while(l<=r){
		int mid=(l+r)>>1;
		if(cmp(Q[mid-1],Q[mid],K)){
			ans=mid;
			l=mid+1;
		}
		else r=mid-1;
	}
	return ans;
}
void dfs(int x,int f,LL d){
	LL k;int now,cur;node tmp,tp;
	if(x!=1){ 
		dis[x]=d;
		k=V[x];
		cur=find(k,top);
		dp[x]=Q[cur].y-k*Q[cur].x+S[x]+V[x]*dis[x];
		tmp=(node){dis[x],dp[x]};
		now=top;cur=Find(tmp,2,top);
		top=cur;
		tp=Q[cur];Q[cur]=tmp;//找到该放的点之后换 
	}
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to,v=G[i].co;
		if(u==f)continue;
		dfs(u,x,d+v);
	}
	if(x!=1)top=now,Q[cur]=tp;//还原现场
}
int main(){
	cin>>n;
	for(int i=1,a,b,c;i<n;i++){
		scanf("%d%d%d",&a,&b,&c);
		Add(a,b,c);
		Add(b,a,c);
	}
	for(int i=2;i<=n;i++)
		scanf("%lld%lld",&S[i],&V[i]);
	clr(dp,0x3f);
	dp[1]=0;
	Q[++top]=(node){0,0};
	dfs(1,0,0);
	for(int i=2;i<=n;i++)
		printf("%lld ",dp[i]);
	return 0;
}
```



