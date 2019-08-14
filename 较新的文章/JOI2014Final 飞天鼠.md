#### 题目

フクロモモンガの JOI 君が住んでいる森にはユーカリの木が N 本生えており，それらの木には 1 から N
の番号がついている．木 i の高さは Hi メートルである．
JOI 君が相互に直接飛び移ることのできる木の組が M 組あり，各組の木の間を飛び移るためにかかる時
間が定まっている．JOI 君が木の間を飛び移っている間は，地面からの高さが 1 秒あたり 1 メートル下が
る．すなわち，JOI 君の現在の地面からの高さが h メートル，木の間を飛び移るためにかかる時間が t 秒で
あるとき，飛び移った後の地面からの高さは h − t メートルとなる．ただし，h − t が 0 よりも小さくなる場
合や行き先の木の高さよりも大きくなる場合は飛び移ることができない．
さらに，JOI 君は木の側面を上下に移動することによって，地面からの高さを 0 メートルから今いる木
の高さの範囲で増減させることができる．JOI 君が地面からの高さを 1 メートル増加または減少させるた
めには 1 秒の時間がかかる．
JOI 君は，木 1 の高さ X メートルの位置から木 N の頂上 (高さ HN メートルの位置) に行こうとしており，
そのためにかかる時間の最小値を知りたい．



翻译过来就是：点之间的转移类似于滑翔，从$x->y$为$h[x]-t$,如果$0<=h[x]-t<=h[y]$才是合法的，问从1的$X$处转移到$n$点的顶端要多久。



#### 思路

一开始有一个初步思路就是将状态定义为当前在哪个点，以及高度是多少，再用map离散，set判重，优先队列跑Dijkstra。本地开$O(2)$1.5s+,不开3s+，交上去果断TLE50分。

这是代码~~

```cpp
#include<bits/stdc++.h>
#define M 100005
#define LL long long
//#pragma GCC optimize(3)
using namespace std;
int n,m,X,h[M],tot,A[M];
struct edge{
	int nxt,to,co;	
}G[M*6];
void Add(int a,int b,int c){
	G[++tot]=(edge){h[a],b,c};
	h[a]=tot;
}
struct node{
	LL co;int x,h;
	bool operator < (const node& res)const{
		return co>res.co;
	}
	bool operator == (const node& res)const{
		return (co==res.co);
	}
};
struct hs{
	int x,h;
	bool operator < (const hs& res) const{
		if(x!=res.x)return x<res.x;
		return h<res.h;
	}
};
bool vis[M];
priority_queue<node>Q;
map<hs,LL>mp;
set<hs>S;
node dis(int x,int h,int y,int t){
	node tmp;
	if(h-t>A[y]){
		tmp.co=h-A[y];
		tmp.h=A[y];
	}
	else if(h-t<0){
		tmp.co=t+t-h;
		tmp.h=0;
		if(t>A[x])
			tmp.co=-1;	
	}
	else {
		tmp.co=t;
		tmp.h=h-t;	
	}
	return tmp;
}
LL Dij(){
	LL ans=1e18;
	mp.clear();S.clear();
	mp[(hs){1,X}]=0;
	Q.push((node){0,1,X});
	while(!Q.empty()){
		node e=Q.top();Q.pop();
		if(e.x==n)ans=min(ans,e.co+abs(A[n]-e.h));
		if(S.find((hs){e.x,e.h})!=S.end())continue;
		S.insert((hs){e.x,e.h});
		for(int i=h[e.x];i;i=G[i].nxt){
			int u=G[i].to,v=G[i].co;
			node ds=dis(e.x,e.h,u,v);
			if(ds.co==-1)continue;
			if(mp.find((hs){u,ds.h})==mp.end()||mp[(hs){u,ds.h}]>e.co+ds.co){
				mp[(hs){u,ds.h}]=e.co+ds.co;
				Q.push((node){e.co+ds.co,u,ds.h});
			}
		}
	}
	if(ans==1e18)return -1;
	return ans;
}
int main(){
//	freopen("data.in","r",stdin);
	cin>>n>>m>>X;
	for(int i=1;i<=n;i++)scanf("%d",&A[i]);
	for(int i=1,a,b,c;i<=m;i++){
		scanf("%d%d%d",&a,&b,&c);
		Add(a,b,c);Add(b,a,c);
	}
	printf("%lld\n",Dij());
	return 0;
}
```



然后理了一下思路发现不可能这么复杂。然后就知道，对于一个点来说，靠**滑翔向下走和向上走都是没有关系的**，

最后直接+$abs(X-dis[n]-A[n])$j就行了，只有向下走比较棘手。

然后再考虑一下Dijkstra的原理，每个点是逐层扩展的，那么我们将约束条件加上不就行了吗？

因为一个点到1点经过的距离一定$>=X-A[u]$,如果不是这样的话，就说明有一些向下走的操作被忽略了。直接将这个dis值赋为$X-A[u]$就行了，这是一定能取到的，因为Dijkstra算法的原理决定了这一点。



#### 代码

```cpp
#include<bits/stdc++.h>
#define M 100005
#define LL long long
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,m,X,h[M],tot,A[M];
struct edge{
	int nxt,to,co;	
}G[M*6];
void Add(int a,int b,int c){
	G[++tot]=(edge){h[a],b,c};
	h[a]=tot;
}
struct node{
	LL co,x;
	bool operator < (const node& res)const{
		return co>res.co;
	}
};
bool vis[M];
LL dis[M];
priority_queue<node>Q;
LL Dij(){
	LL ans=1e18;
	Q.push((node){0,1});
	clr(dis,63);dis[1]=0;
	while(!Q.empty()){
		node e=Q.top();Q.pop();
		if(vis[e.x])continue;
		vis[e.x]=1;
		for(int i=h[e.x];i;i=G[i].nxt){
			int u=G[i].to;
			LL v=G[i].co;
			if(A[e.x]<v)continue;
			v=dis[e.x]+v;
			if(X>A[u])v=max(v,1LL*(X-A[u]));
			if(dis[u]>v){
				dis[u]=v;
				Q.push((node){dis[u],u});
			}
		}
	}
	if(dis[n]>1e18)return -1;
	return dis[n]+abs(X-dis[n]-A[n]);
}
int main(){
//	freopen("data.in","r",stdin);
	cin>>n>>m>>X;
	for(int i=1;i<=n;i++)scanf("%d",&A[i]);
	for(int i=1,a,b,c;i<=m;i++){
		scanf("%d%d%d",&a,&b,&c);
		Add(a,b,c);Add(b,a,c);
	}
	printf("%lld\n",Dij());
	return 0;
}
```

