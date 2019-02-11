title: CF129E Games with Rectangle
author: zryabc
tags:
  - 数学
  - 计数dp
  - DP
categories: []
date: 2018-09-19 21:46:00
---
#### 题目
In this task Anna and Maria play the following game. Initially they have a checkered piece of paper with a painted n × m rectangle (only the border, no filling). Anna and Maria move in turns and Anna starts. During each move one should paint inside the last-painted rectangle a new lesser rectangle (along the grid lines). The new rectangle should have no common points with the previous one. Note that when we paint a rectangle, we always paint only the border, the rectangles aren't filled.

Nobody wins the game — Anna and Maria simply play until they have done k moves in total. Count the number of different ways to play this game.
大意是在一个区间内画方格，方格里面套方格，画k次问有多少种画法。
<!--more-->
#### 思路
~~抄了题解~~
其实思路很简单，我们将行和列分开考虑，当成线段处理，分析终态（每一种终态对应最后的一种画法），那么我们会发现，每一个终态最后不过是2×k条横线，与2×k条竖线，这两者之间是互相不影响的，所以$ans=C_{n-1}^k×C_{m-1}^k$
#### 代码
```c++
#include<bits/stdc++.h>
#define M 1005
#define LL long long
using namespace std;
const int MOD=1e9+7;
int n,m,k;
LL C[M*2][M*2];
void Init(){
    C[0][0]=1;
    for(int i=1;i<=2*M-5;i++){
        C[i][0]=1;
        for(int j=1;j<=i;j++){
            C[i][j]=(C[i-1][j-1]+C[i-1][j])%MOD;
        }
    }
}
int main(){
    Init();
    cin>>n>>m>>k;
    cout<<1LL*(C[n-1][2*k]*C[m-1][2*k])%MOD<<endl;
    return 0;	
}
```