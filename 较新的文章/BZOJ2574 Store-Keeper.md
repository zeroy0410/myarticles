#### 题目

[题目地址](https://remmina.github.io/BZPRO/JudgeOnline/2574.html)



#### 思路

仔细分析一下会发现，其实此题的状态数并不多，只有$100*100*4$种，人的移动是不需要的，也就是说，我们只需要知道当箱子在某一个位置时，人能不能从箱子的一个侧面走向另一个侧面。

转化成图论模型也就是说，当一个无向图断掉一个点之后，判断两个点的连通性。

这就让人想到了[COCI2016/2017 Final](http://www.hsin.hr/coci/archive/2006_2007/olympiad_tasks.pdf)的题，这是那道题的一个子问题。

首先用$Tarjan$将$dfs$树建立出来，处理出每个点的$low$值和子树的$dfs$序区间，然后,对于$a$,$b$，判断断掉$x$之后他们是否联通。

分为3种情况讨论：

1. 两者都不在x的子树内，那么会发现x断掉对这两点的连通性没有什么影响。
2. 两者都在x的子树内，那么只需要将他们跳到x的下面那个点，判断该点的low值是否小于dfn[x]，然，则说明底下存在一条返祖边回到x的上方，在x断掉之后仍然能够使图联通。
3. 两者一个在x的子树内一个不在，这种情况与上面类似，仍然讨论是否存在返祖边使图联通即可。



#### 代码

```c++
#include<bits/stdc++.h>
#define M 10005
using namespace std;
int n,m,h[M],tt;
char S[105][105];
bool vis[105][105][4];
int dxy[4][2]={1,0,0,1,0,-1,-1,0};
struct edge{int nxt,to;}G[M<<3];
void Add(int a,int b){
    G[++tt]=(edge){h[a],b};
    h[a]=tt;
}
int dfn[M],low[M],tot,R[M],fa[M][15],dep[M];
int ID(int x,int y){return (x-1)*m+y;}
void Tarjan(int x,int f,int d){
    low[x]=dfn[x]=++tot;fa[x][0]=f;dep[x]=d;
    for(int i=1;i<=14;i++)
        fa[x][i]=fa[fa[x][i-1]][i-1];
    for(int i=h[x];i;i=G[i].nxt){
        int u=G[i].to;
        if(u==f)continue;
        if(!dfn[u]){
            Tarjan(u,x,d+1);
            low[x]=min(low[x],low[u]);
        }
        else low[x]=min(low[x],dfn[u]);
    }
    R[x]=tot; 
}
int Up(int x,int step){
    for(int i=14;i>=0;i--)
        if(step&1<<i)x=fa[x][i];
    return x;
}
struct node{
    int x,y,step,fr;
}Q[10005],Q2[40005];
int mark[105][105];int T;
bool check(int x,int y){
    return x>=1&&x<=n&&y>=1&&y<=m&&mark[x][y]!=T&&S[x][y]!='S';   
}
bool bfs(int sx,int sy,int ex,int ey,int cx,int cy){
    int L=0,R=0;
    Q[R++]=(node){sx,sy};mark[sx][sy]=T;
    while(L!=R){
        node e=Q[L++];if(L==10001)L=0;
        if(e.x==ex&&e.y==ey){++T;return 1;}
        for(int i=0;i<4;i++){
            int x=e.x+dxy[i][0];
            int y=e.y+dxy[i][1];
            if(check(x,y)&&!(x==cx&&y==cy)){
                Q[R++]=(node){x,y};
                if(R==10001)R=0;
                mark[x][y]=T;
            }
        }
    }
    ++T;
    return 0;
}
bool In(int a,int b){
    return dfn[a]>=dfn[b]&&dfn[a]<=R[b];
}
bool Ca(int a,int b,int x){
    if(!In(a,x)&&!In(b,x))return 1;
    if(In(a,x)&&In(b,x)){
        a=Up(a,dep[a]-dep[x]-1);
        b=Up(b,dep[b]-dep[x]-1);
        if(low[a]<dfn[x]&&low[b]<dfn[x]||a==b)return 1;
        return 0;
    }
    if(In(b,x)){
        b=Up(b,dep[b]-dep[x]-1);
        if(low[b]<dfn[x])return 1;
        return 0;;  
    }
    if(In(a,x)){
        a=Up(a,dep[a]-dep[x]-1);
        if(low[a]<dfn[x])return 1;
        return 0;
    }
}
bool mov(int x,int y,int i){
    int nx=x+dxy[3-i][0];
    int ny=y+dxy[3-i][1];
    return check(nx,ny);
}
int sx,sy,sxx,syy,exx,eyy;
int solve(){
    int L=0,R=0;
    for(int i=0;i<4;i++){
        int nx=sxx+dxy[i][0];
        int ny=syy+dxy[i][1];
        if(check(nx,ny)){
            if(bfs(sx,sy,nx,ny,sxx,syy)){
                if(mov(sxx,syy,i)){
                    int nnx=sxx+dxy[3-i][0];
                    int nny=syy+dxy[3-i][1];
                    Q2[R++]=(node){nnx,nny,1,i};
                    vis[nnx][nny][i]=1;
                }
            }
        }
    }
    while(L!=R){
        node e=Q2[L++];if(L==40001)L=0;
        if(e.x==exx&&e.y==eyy)return e.step;
        int frx=e.x+dxy[e.fr][0];
        int fry=e.y+dxy[e.fr][1];
        for(int i=0;i<4;i++){
            int nx=e.x+dxy[i][0];
            int ny=e.y+dxy[i][1];
            if(!check(nx,ny))continue;
            if(Ca(ID(frx,fry),ID(nx,ny),ID(e.x,e.y))){
                if(mov(e.x,e.y,i)){
                    int nxx=e.x+dxy[3-i][0];
                    int nyy=e.y+dxy[3-i][1];
                    if(!vis[nxx][nyy][i]){
                        vis[nxx][nyy][i]=1;
                        Q2[R++]=(node){nxx,nyy,e.step+1,i}; 
                        if(R==40001)R=0;
                    }
                }
            }
        }
    }
    return -1;
}
int main(){
    scanf("%d%d",&n,&m);T=1;
    for(int i=1;i<=n;i++)scanf("%s",S[i]+1);
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++){
            if(S[i][j]=='M'){sx=i,sy=j;}
            if(S[i][j]=='P'){sxx=i,syy=j;}
            if(S[i][j]=='K'){exx=i,eyy=j;}
            if(check(i,j))
                for(int k=0;k<2;k++){
                    int nx=i+dxy[k][0];
                    int ny=j+dxy[k][1];
                    if(check(nx,ny)){
                        Add(ID(nx,ny),ID(i,j));
                        Add(ID(i,j),ID(nx,ny)); 
                    }
                }
        }
    for(int i=1;i<=n*m;i++)
        if(!dfn[i])Tarjan(i,0,0);
    int ans=solve();
    if(ans==-1)printf("NO");
    else printf("%d\n",ans);
    return 0;
}
```

