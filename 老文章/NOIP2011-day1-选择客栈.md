---
title: NOIP2011 day1 Mayan游戏
date: 2018-09-05 11:07:31
tags: [搜索,剪枝]
---

#### 题目
[题目链接](https://www.luogu.org/problemnew/show/P1312)

#### 思路
搜索题，关键是剪枝。

对于每个操作来说，我们要维护几个函数：down，del和Print(保存路径用)，这是一个基本的雏形，考试的时候就是这样打的（然而大数据要70s+，所以会挂掉）

下面就要开始剪枝了。

1. 对于相邻两个都涂有颜色的格子来说，显然应该让左边向右换。
2. 对于当一个图中的某种颜色只有2个或1个时，此图不可能最终消完。
3. 两个相同的颜色之间不必交换

<!--more-->

加了这些之后就可以过了。

#### 代码
```c++
#include<bits/stdc++.h>
using namespace std;
int Map[15][15];//x表示横格，y表示竖格 
bool mark[15][15];
int n;
struct node{
    int x,y,d,fid;
};
node fa[1000005];
int dx[5]={1,-1};
bool check(){
    for(int i=0;i<5;i++)
        for(int j=0;j<7;j++)
            if(Map[i][j]!=0)return 0;
    return 1;
}
void Print(int id){
    if(id==0)return;
    Print(fa[id].fid);
    if(fa[id].d!=0)printf("%d %d %d\n",fa[id].x,fa[id].y,fa[id].d);
}
void down(){
    for(int i=0;i<5;i++){
        int t=-1;
        for(int j=0;j<7;j++){
            int tmp=Map[i][j];Map[i][j]=0;
            if(tmp)Map[i][++t]=tmp;
        }
    }
}
void Del(){//控制消去 
    bool flag=1;
    for(int i=0;i<5;i++)for(int j=0;j<7;j++)mark[i][j]=0;
    for(int i=0;i<5;i++)
        for(int j=0;j<7;j++){
            if(Map[i][j]){
                if(j>1&&Map[i][j]==Map[i][j-1]&&Map[i][j]==Map[i][j-2])mark[i][j]=mark[i][j-1]=mark[i][j-2]=1;
                if(i>1&&Map[i][j]==Map[i-1][j]&&Map[i][j]==Map[i-2][j])mark[i][j]=mark[i-1][j]=mark[i-2][j]=1;
            }
        }
    for(int i=0;i<5;i++)
        for(int j=0;j<7;j++)
            if(mark[i][j])Map[i][j]=0,flag=0;
    down();
    if(!flag)Del();
} 
int id=0;
int cnt[15];
void dfs(int step,int fid,int x,int y,int d){//当前状态是怎么来的 
    int tmp[5][7];
    for(int i=0;i<5;i++) 
        for(int j=0;j<7;j++) 
            tmp[i][j]=Map[i][j]; 
    if(step>n){
        if(check()){
            Print(fid);
            if(d!=0)printf("%d %d %d\n",x,y,d); 
            exit(0);
        }
        return;
    }
    memset(cnt,0,sizeof(cnt));
    for(int i=0;i<5;i++)for(int j=0;j<7;j++)cnt[tmp[i][j]]++;
    for(int i=1;i<=10;i++)if(cnt[i]==1||cnt[i]==2)return ;
    for(int i=0;i<5;i++){
        for(int j=0;j<7;j++){
            if(tmp[i][j]==0)continue;
            for(int r=0;r<2;r++){
                int x1=i+dx[r];
                if(tmp[i][j]==tmp[x1][j])continue;
                if(x1<0||x1>4)continue;
                if(r==1&&(tmp[i][j]&&tmp[x1][j]))continue;
                for(int q=0;q<5;q++) 
                    for(int w=0;w<7;w++) 
                        Map[q][w]=tmp[q][w]; 
                swap(Map[x1][j],Map[i][j]);
                down();
                Del();
                ++id;
                fa[id]=(node){x,y,d,fid};
                dfs(step+1,id,i,j,(r==0?1:-1));
            }
        }
    }
}
int main(){//文件名 LL 内存 mod 
    scanf("%d",&n);
    memset(Map,0,sizeof(Map));
    for(int i=0;i<=4;i++){
        int x,p=0;
        while(1){
            scanf("%d",&x);
            if(x==0)break;
            Map[i][p]=x;
            p++;
        } 
    }
    dfs(1,0,0,0,0);
    puts("-1");
    return 0;
}
```
