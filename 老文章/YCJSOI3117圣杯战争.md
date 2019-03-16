#### 题目

“这场圣杯战争……从一开始，就很不对劲。”

由于这场圣杯战争过于异常，作为中立调停的「裁定者Ruler」贞德被大圣杯召唤。

圣杯战争在 n 座城市内进行，**这些城市之间通过 m 条路径相互连接彼此可以相互到达**，由于某些特殊规则，所有的从者只能通过这 m 条路径从一个城市到达另一个城市。开始时，每个城市都会出现有且仅有一位从者。

令贞德不安的是，从者们既不是各自为战，也没有完整的阵营组合：每位从者分属一个阵营 kiki  ，相同阵营的从者之间不会相互攻击，并且所有阵营均没有固定的从者数目。

在混战中，被击杀的从者会直接回归英灵座而消失，退出这场圣杯战争。大圣杯继续召唤一位英灵作为从者，让他/她降临在被击杀的从者所在的城市。

陷入迷惘的贞德整理了思绪，决定先去监督不同阵营的从者之间相距最近的一组战斗（指两个从者到达对方城市的最短距离），并希望知道每次更换（接着上一次的更新进行）了从者后所有战斗的最短距离。请帮助贞德完成这个任务。

大圣杯保证不会只剩下一种阵营。



#### 思路

个人认为是比较暴力的。



##### 首先证明两个结论：

1. 最后的答案一定是一条边
2. 最后的答案一定在最小生成树上

##### 证明1：

反证，如果答案不在一条边上，那么一定有一条更小的边能够松弛当前边，而这条更小的边一定是满足边的两边端点不同的，所以原命题成立。

##### 证明2：

如果存在一条边作为答案，且这条边不在最小生成树上，那么这条边连接的两个点一定被其它的边所松弛，那么就可以回到证明1了。



有了这两个性质，我们就可以把最小生成树建出来了。

**下面开始数据结构暴力强搞**

我们对于每个**非叶子结点**，建一棵**线段树**，元素为它的子节点，在线段树中，下标为颜色，权值就是父节点与子节点的边的长度。那么我们设父节点的颜色为C，那么当前的答案就是$min(query(1,C-1)，query(C+1,k))$ 。但是一棵线段树中可能有很多的点有相同的颜色，删点的时候怎么办呢？

所以，我们在线段树的每个叶子结点开一个**multiset**，来解决多个元素的问题。

然后再开一个全局的multiset，ans，把所有答案都push到ans中，最后的答案就是*(ans.begin())。

我们来分析一下这个如此暴力的代码的时间复杂度：
由于在改变颜色重新query的时候，multiset和线段树其实都只会被用一次，所以他们是同阶的，故总的复杂度为$O(nlogn)$



```cpp
#include<bits/stdc++.h>
#define N 200005
#define M 400005
using namespace std;
int n,m,k,Q;
int be[M],ver[M],id[M*30],tt,las[M],val[M];
struct edge{
    int fr,to,v;
    bool operator < (const edge& res) const {
        return v<res.v;
    }
}E[M];
multiset<int>ans,st[M<<2];
struct YD_tree{
    int lson[M*30],rson[M*30],mi[M*30],tot;
    YD_tree(){
        tot=0;
        mi[0]=1e9;
    } 
    void up(int p){
        mi[p]=min(mi[lson[p]],mi[rson[p]]);
    }
    void update(int l,int r,int x,int d,int& p,bool f){
        if(!p)p=++tot,mi[p]=1e9;
        if(l==x&&r==x){
            if(!id[p])id[p]=++tt;
            if(f)st[id[p]].insert(d);
            else st[id[p]].erase(st[id[p]].find(d));
            if(st[id[p]].empty())mi[p]=1e9;
            else mi[p]=*(st[id[p]].begin());    
            return;
        }
        int mid=(l+r)>>1;
        if(x<=mid)update(l,mid,x,d,lson[p],f);
        else update(mid+1,r,x,d,rson[p],f);
        up(p);
    }
    int query(int l,int r,int L,int R,int p){
        if(!p||l>r)return 1e9;
        if(l==L&&r==R)return mi[p];
        int mid=(l+r)>>1;
        if(R<=mid)return query(l,mid,L,R,lson[p]);
        else if(L>mid)return query(mid+1,r,L,R,rson[p]);
        return min(query(l,mid,L,mid,lson[p]),query(mid+1,r,mid+1,R,rson[p]));
    }   
}T;
int fa[M];
int getfa(int x){
    return fa[x]==x?x:fa[x]=getfa(fa[x]);   
}
int h[M],tot;
struct Edge{
    int nxt,to,co;
}G[M<<1];
void Add(int a,int b,int c){
    G[++tot]=(Edge){h[a],b,c};
    h[a]=tot;
}
void dfs(int x,int f){
    fa[x]=f;
    for(int i=h[x];i;i=G[i].nxt){
        int u=G[i].to,c=G[i].co;
        if(u==f)continue;
        val[u]=c;
        dfs(u,x);
        T.update(1,k,be[u],c,ver[x],1); 
    }
    ans.insert(las[x]=min(T.query(1,k,1,be[x]-1,ver[x]),T.query(1,k,be[x]+1,k,ver[x])));
}
void Kruskal(){
    sort(E+1,E+m+1); 
    for(int i=1;i<=n;i++)fa[i]=i;
    for(int i=1;i<=m;i++){
        int a=E[i].fr,b=E[i].to,c=E[i].v;
        if(getfa(a)!=getfa(b)){
            fa[getfa(a)]=getfa(b);
            Add(a,b,c);
            Add(b,a,c); 
        }
    }
    dfs(1,0);
}
int main(){
    cin>>n>>m>>k>>Q;
    for(int i=1;i<=m;i++)
        scanf("%d%d%d",&E[i].fr,&E[i].to,&E[i].v);
    for(int i=1;i<=n;i++)
        scanf("%d",&be[i]);
    Kruskal();
    for(int i=1,a,b;i<=Q;i++){
        scanf("%d%d",&a,&b);
        if(be[a]!=b){
            ans.erase(ans.find(las[a]));
            las[a]=min(T.query(1,k,1,b-1,ver[a]),T.query(1,k,b+1,k,ver[a]));
            ans.insert(las[a]);
        }
        if(a!=1){
            int p=fa[a];
            ans.erase(ans.find(las[p]));
            T.update(1,k,be[a],val[a],ver[p],0);
            T.update(1,k,b,val[a],ver[p],1);
            las[p]=min(T.query(1,k,1,be[p]-1,ver[p]),T.query(1,k,be[p]+1,k,ver[p]));
            ans.insert(las[p]);
        }
        be[a]=b;
        printf("%d\n",*(ans.begin()));
    }
    return 0;
}
```

