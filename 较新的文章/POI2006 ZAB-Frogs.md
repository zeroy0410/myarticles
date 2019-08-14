#### 题目

一群青蛙正在摧毁Byteotia所有的庄稼. 一个叫Byteasar的农夫决定使用一种放在田里的奇特的"scarefrogs"来吓跑他们, 所有的青蛙在跳跃过程中都尽量使自己离他们越远越好, 即是让自己离最近的scarefrog越远越好. 
Byteasar的田是块矩形的土地. 青蛙们跳跃的方向平行于坐标轴并且每次跳跃的距离为1. 一条跳跃路线的scarefrogs-距离为路径上所有点距离所有scarefrogs的最近距离. 
Byteasar已经知道青蛙们的出发位置和目的地位置, 所以他在田里放置了若干个scarefrogs. 他请求你帮忙写一个程序找一条路径使得该路径的scarefrogs-距离最大. 

[题目链接](https://remmina.github.io/BZPRO/JudgeOnline/1514.html)



#### 思路

斜率优化dp。

首先，题目可以分成两个部分:
第一部分：求出每个点到最近的scarefrogs的距离。
第二部分：二分答案，bfs求出青蛙是否可以达到终点。

主要卡在第一部分。

先来考虑冗余的信息：单独考虑一列时，对于某一行而言，显然只有离这一列最近的那个点对这一列的答案有影响，所以实际上对答案有影响的点是很少的，不超过2000个。

然后思考如何一下子把一行的信息都处理出来。

先推一个式子：

对于$a$和$b$两个点和一组$x$,$y$。

若$b$优于$a$：
$$
(X_b-x)^2+(Y_b-y)^2\leq(X_a-x)^2+(y_a-y)^2
$$
将其展开：
$$
\frac{x_b^2-x_a^2+2x(x_a-x_b)}{y_b-y_a}+y_a+y_b\lt 2y
$$
观察这个式子，发现可以斜率优化。

之后就是斜率优化裸题了，主要考验代码功底。



#### 代码

```c++
#include<bits/stdc++.h>
#define M 1005
#define LL long long
using namespace std;
int n,m,sx,sy,ex,ey,t,top,l;
int dxy[4][2]={1,0,0,1,-1,0,0,-1};
LL  G[M][M];
bool vis[M][M]; 
struct node{LL x,y;}stk[M],A[M];
vector<int>wk[M];
bool check(int x,int y){
    return x>=1&&x<=n&&y>=1&&y<=m&&!vis[x][y];
}
double get(node B,node A,LL x){
    return 1.0*(B.x*B.x-A.x*A.x+2*x*(A.x-B.x))/(B.y-A.y)+A.y+B.y;
}
void Init(){
    for(int i=1;i<=m;i++)sort(wk[i].begin(),wk[i].end());
    for(int i=1;i<=n;i++){
        top=l=0;
        for(int j=1;j<=m;j++){
            int len=wk[j].size();
            if(!len)continue;
            int t=lower_bound(wk[j].begin(),wk[j].end(),i)-wk[j].begin()-1;
            if(t==-1||(t<len-1&&i-wk[j][t]>wk[j][t+1]-i))t++;
            A[++l]=(node){wk[j][t],j};
        }
        for(int j=1;j<=l;j++){
            while(top>1&&get(A[j],stk[top],i)<=get(stk[top],stk[top-1],i))top--;
            if(top<=1||get(A[j],stk[top],i)>get(stk[top],stk[top-1],i))stk[++top]=A[j];   
        }
        for(int j=m;j>=1;j--){
            while(top>1&&get(stk[top],stk[top-1],i)>2*j)top--;
            G[i][j]=1LL*(i-stk[top].x)*(i-stk[top].x)+1LL*(j-stk[top].y)*(j-stk[top].y);
        }
    }
}
queue<node>Q;
bool bfs(int lim){
    memset(vis,0,sizeof(vis));
    while(!Q.empty())Q.pop();
    Q.push((node){sx,sy});vis[sx][sy]=1;
    if(G[sx][sy]<lim)return 0;
    while(!Q.empty()){
        node e=Q.front();Q.pop();
        if(e.x==ex&&e.y==ey)return 1;
        for(int i=0;i<4;i++){
            int nx=e.x+dxy[i][0];
            int ny=e.y+dxy[i][1];
            if(check(nx,ny)&&G[nx][ny]>=lim){
                vis[nx][ny]=1;
                Q.push((node){nx,ny});
            }
        }
    }
    return 0;
}
int main(){
    scanf("%d%d%d%d%d%d%d",&n,&m,&sx,&sy,&ex,&ey,&t);
    for(int i=1,x,y;i<=t;i++){
        scanf("%d%d",&x,&y);
        wk[y].push_back(x);
    }
    Init();
    int l=0,r=0,ans=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            r=max(r,(int)G[i][j]);
    while(l<=r){
        int mid=(l+r)>>1;
        if(bfs(mid)){
            ans=mid;
            l=mid+1;
        }
        else r=mid-1;
    }
    printf("%d\n",ans);
    return 0;
} 
```

