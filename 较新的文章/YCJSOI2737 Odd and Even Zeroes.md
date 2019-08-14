#### 题目

在数学中，一个数n的阶乘写作n!，且定义如下：
n! = 1 × 2 × 3 × 4 × • • • × (n − 1) × n
0!被认为是1。n!将会随着n迅速增长。如下一些数的阶乘：
0! = 1 5! = 120
1! = 1 10! = 3628800
2! = 2 14! = 87178291200
3! = 6 18! = 6402373705728000
4! = 24 22! = 1124000727777607680000
通过观察可以发现，有些数的阶乘末尾中有奇数个零(即5！和18！)，而有些数的阶乘的末尾中有偶数个零(即0！，10！，20！)。那么问题来了，给定一个数n，求在1~n中有多少数的阶乘的末尾有偶数个0？



#### 思路

考场从看到开始想，一直想到结束都没想出来，解法很妙。

首先先把$n$转化为$5$进制，这是解题的关键。

也就是把n化为$a*1+b*5+c*25+d*125...$的形式，然后会发现，$n$最终含有$5$的个数是否是偶数只与偶数位有关，只要偶数位上的数码之和是偶数，那么它末尾就会有偶数个$0$。

然后就是数位dp了。



#### 代码

```c++
#include<bits/stdc++.h>
#define LL long long
using namespace std;
LL n,dp[30][2];
int A[30],l;
LL dfs(int x,int t,bool lim){
    if(x==0)return !t;
    if(!lim&&dp[x][t]!=-1)return dp[x][t];
    LL res=0;
    int ed=lim?A[x]:4;
    for(int i=0;i<=ed;i++){
        if(x&1)res+=dfs(x-1,t,lim&(i==ed));
        else res+=dfs(x-1,(t+i)&1,lim&(i==ed)); 
    }
    if(!lim)dp[x][t]=res;
    return res;
}
int main(){
    memset(dp,-1,sizeof(dp));
    while(scanf("%lld",&n)&&~n){
        l=0;while(n)A[++l]=n%5,n/=5;
        printf("%lld\n",dfs(l,0,1));
    }
    return 0;
}
```

