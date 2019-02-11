#### 题目

平面中有$ n$ 个点 $(x_i,y_i)$ ，有 $m$ 条直线，斜率 $k$ 已经确定，需要在给定的 $n$ 个点中，选出一个点 $(x,y)$ ，使得 $kx+y$ 最大。

#### 思路

这里有一个关于凸包的版子。

我们令$K=-K$

那么这题就化成了我们很熟悉的$y-kx$的格式了。

然后显然要构造凸包。

然后将$k$从大到小排序，离线扫一遍就好了。

正确性这里提供几幅图，感性理解：

![tu](https://i.loli.net/2018/10/20/5bcae405a7dac.png)

就可以看成几根线把一个凸包箍住，这样答案显然是最优的。图画的不好，勿喷。

#### 代码

```cpp
#include<bits/stdc++.h>
#define M 100005
#define LL long long
using namespace std;
struct node{
    LL x,y;
    bool operator < (const node & res) const {
        return x<res.x;
    }
}A[M],Q[M];
bool cmp(node a,node b,int K){
    return b.y-K*b.x>=a.y-K*a.x;
}   
bool check(node a,node b,node c){
    return (c.y-a.y)*(a.x-b.x)>=(a.y-b.y)*(c.x-a.x); 
}
int n,m;
int l,r=-1;
struct que{
    LL k;
    int id;
    bool operator < (const que& res) const{
        return k>res.k;
    }
};
vector<que>q;
LL Ans[M];
int main(){
    cin>>n>>m;q.clear();
    for(int i=1;i<=n;i++){
        scanf("%lld%lld",&A[i].x,&A[i].y);
    }
    sort(A+1,A+n+1);
    for(int i=1;i<=n;i++){
        while(l<=r-1&&check(Q[r],Q[r-1],A[i]))r--;
        Q[++r]=A[i];
    }
    for(int i=1,K;i<=m;i++){
        scanf("%d",&K);K=-K;
        q.push_back((que){K,i});
    }
    sort(q.begin(),q.end());
    for(int i=0;i<(int)q.size();i++){
        LL k=q[i].k;
        while(l<=r-1&&cmp(Q[l],Q[l+1],k))l++;
        Ans[q[i].id]=Q[l].y-Q[l].x*k;
    }
    for(int i=1;i<=m;i++)
        printf("%lld\n",Ans[i]);
    return 0;   
} 
```

