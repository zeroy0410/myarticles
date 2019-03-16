#### 题目

在Fat_Tiger的Fat王国里，有N座城镇依次编号为$1∼N$。这$N$座城镇由$N$条道路连接，道路是双向的。其中第ii条道路连接$X_i$和$Y_i$，长度均为$1$。

王国初始情况是连通的。由于有$N$条道路，不免出现了环，而Fat_Tiger是讨厌环的。

于是Fat_Tiger希望通过删边使王国中不出现环，但是他必须保证删边后王国仍保持连通，以保持王国的稳定。在所有的删边方案中，Fat_Tiger希望找到一种方案，使王国的不方便度最小。

用$dist(i,j)$表示i和j之间的最短路径，定义一种方案的不方便$\sum_{i=1}^n \sum_{j=i+1}^n \ dist(i,j)$

现在Fat_Tiger希望你告诉他最优方案的不方便度。

#### 思路

首先考察60分的情况，我们暴力拆环就好了。

按照惯常的套路，我们要单独考虑每条边的贡献：$sz(u)*(n-sz(u))$

然后dfs一遍就行了。



然后看正解：

首先我们提供一幅俯视图：



那么：

（未完）

#### 代码

```cpp
#include<bits/stdc++.h>
#define M 1000005
#define LL long long
#define REP(i,s,t) for(int i=s;i<=t;i++)
using namespace std;
int fa[M],s,t,stk[M],top,sz[M],h[M],tot,n;
LL sum[2][M],Tmp[2][M],ans=2e9,res=0;
bool mark[M],fl=0; 
int getfa(int x){return fa[x]==x?x:fa[x]=getfa(fa[x]);} 
struct edge{
    int nxt,to; 
}G[M<<1];
void Add(int a,int b){
    G[++tot]=(edge){h[a],b};
    h[a]=tot;   
}
void dfs(int x,int f){
    stk[++top]=x;
    if(x==t){fl=1;return;}
    for(int i=h[x];i&&!fl;i=G[i].nxt){
        int u=G[i].to;
        if(u==f)continue;
        dfs(u,x);   
    }
    if(!fl)top--;
}
void redfs(int x,int f){
    sz[x]=1;
    for(int i=h[x];i;i=G[i].nxt){
        int u=G[i].to;
        if(u==f||mark[u])continue;
        redfs(u,x);
        sz[x]+=sz[u];
        res+=1LL*sz[u]*(n-sz[u]);
    }
}
int main(){
    cin>>n;
    REP(i,1,n)fa[i]=i;
    for(int i=1,a,b;i<=n;i++){
        scanf("%d%d",&a,&b);
        if(getfa(a)!=getfa(b)){
            fa[getfa(a)]=getfa(b);
            Add(a,b);
            Add(b,a);
        }
        else s=a,t=b;
    }
    dfs(s,0);
    REP(i,1,top)mark[stk[i]]=1; 
    REP(i,1,top)redfs(stk[i],0);
    REP(i,1,top)sum[0][i]=sum[0][i-1]+sz[stk[i]];
    REP(i,1,top-1)res+=sum[0][i]*(n-sum[0][i]);
    REP(i,1,top)sum[1][i]=sum[1][i-1]+sz[stk[i]]*i;
    REP(i,1,top)Tmp[0][i]=sum[0][i-1]*i-sum[1][i-1]+(sum[0][top]-sum[0][i])*(top+i)-(sum[1][top]-sum[1][i]);
    REP(i,1,top)Tmp[1][i]=(sum[1][top]-sum[1][i])-(sum[0][top]-sum[0][i])*i+sum[1][i-1]+sum[0][i-1]*(top-i);
    ans=res;
    REP(i,1,top-1){
        res=res+1LL*sz[stk[i]]*(Tmp[0][i]-Tmp[1][i]);
        ans=min(ans,res);
    }
    printf("%lld\n",ans);
    return 0;
}
```

