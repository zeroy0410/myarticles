#### 题目

Shin是一个由$n$个城市构成的国度，$n$个城市由$n−1$条双向道路连接在一起。其中第$i$条路连接$ui,vi$,路边有$l_i$片农田。

现在有$q$个询问：

设想有一场干旱，只有城市$x$有水源，最多只能挖$y$条沿道路设置的河道，要让河道经过尽可能多的农田，求最多有多少农田可以灌溉到水。

注意：河道之间可以沟通，每条河道必须可以直接到达或者通过其他河道间接到达城市$x​$；

#### 思路（P70）

这题满分写法很难，所以只写了P70，有空填坑。**（估计没空了，马上就退役了）**

第一步先把$x$提到根的位置，把一条路径拆分开来，变成从$x$出发往下一步步深入的路径。这样如果$x$不是叶节点，那么就需要$2*y$条路，否则需要$2*y-1$条路。

然后，显然有一个贪心的思路，就是尽量取能使答案增加的最长的那一段，取了之后再把这条路上的点都标记掉，不计入下一次取边的贡献，反复此过程直到取完。

数组含义：

$mx[x]$:表示x深入下去最远的距离（相对x点而言）

$son[x]$:表示x的儿子中最远的点最远的点编号（有点拗口，代码还是比较清晰的）

那么，对于每一个$x$，如果它不是它父亲那棵子树中的重儿子，那么它就要被计算$dis(fa-->x)$否则则不用计算。

这样把每个元素的权值算出来，排个序，从大往小取，我们得到了一个看似很有道理的贪心。

然而，它真的正确吗？

要注意题目还有一个条件：**水道要和x连通**

![](https://i.loli.net/2018/11/05/5be04603679d7.png)

那么观察得到，如果一种情况，我们所取的点都在一棵子树中，那么无论怎样连，都是不可能产生经过x的路径的。

所以还要加上一步处理：

先把包含x的最长的那一条路径处理出来，其他的边都作为此路径的“附属”，这样就可以保证上述情况不再发生了。

#### 代码

```cpp
#include<bits/stdc++.h>
#define M 100005
using namespace std;
int n,q;
int h[M],tot,rt;
struct edge{
    int nxt,to,co;
}G[M<<1];
void Add(int a,int b,int c){
    G[++tot]=(edge){h[a],b,c};
    h[a]=tot;   
}
int mx[M],son[M],fa[M];
void dfs(int x,int f){
    mx[x]=0;son[x]=x;fa[x]=f;
    for(int i=h[x];i;i=G[i].nxt){
        int u=G[i].to,ds=G[i].co;
        if(u==f)continue;
        dfs(u,x);
        if(mx[x]<mx[u]+ds)mx[x]=mx[u]+ds,son[x]=u;
    }
}
priority_queue<int>Q;
int main(){
    cin>>n>>q;
    for(int i=1,a,b,c;i<n;i++){
        scanf("%d%d%d",&a,&b,&c);
        Add(a,b,c);Add(b,a,c);
    }
    for(int i=1,x,y;i<=q;i++){
        while(!Q.empty())Q.pop();
        scanf("%d%d",&x,&y);
        dfs(x,0);
        int res=0,pos=-1;
        for(int j=h[x];j;j=G[j].nxt){
            int u=G[j].to,ds=G[j].co;
            if(u==son[x])continue;
            if(mx[u]+ds>res)res=mx[u]+ds,pos=u;
        }
        res+=mx[x];
        for(int j=1;j<=n;j++){
            if(j!=x){
                for(int k=h[j];k;k=G[k].nxt){
                    int u=G[k].to,ds=G[k].co;
                    if(u==son[j]||u==fa[j])continue;
                    Q.push(mx[u]+ds);
                }
            }
            else {
                for(int k=h[j];k;k=G[k].nxt){
                    int u=G[k].to,ds=G[k].co;
                    if(u==son[j]||u==pos)continue;
                    Q.push(mx[u]+ds);
                }   
            }
        }
        while((--y)&&!Q.empty()){
            if(!Q.empty())res+=Q.top();Q.pop();
            if(!Q.empty())res+=Q.top();Q.pop(); 
        } 
        printf("%d\n",res);
    }
    return 0;
}
```

