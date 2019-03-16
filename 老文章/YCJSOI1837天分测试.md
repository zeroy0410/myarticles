#### 题目

Lay博士想检验一下他的助手Yx是否有天分,给出一个n*m的字符矩形,并且用这个矩形无限复制拼接.
假设当前的矩形为 
honi
hsin

那么复制拼接之后就会变为
..............................
...honihonihonihoni... 
...hsinhsinhsinhsin... 
...honihonihonihoni... 
...hsinhsinhsinhsin... 
.............................

在每个方向上都是无限延伸的.
现在Lay博士在无限的字符格子中随机选择一个起点和方向(8个方向之一),从起点开始遍历K个格子(包括起点),可以得到一个长度为K的字符串.
他让助手Yx也按照同样方式选择,得到了另一个长度为K的字符串.
如果两个字符串相同,那么Yx就是有天分的.
Yx很想要通过Lay博士的检验,请帮他求出 这两个字符串相等的概率是多少,答案用分数表示.  

#### 思路

##### 一种自然的想法（70分）

观察到每个矩阵只有100*100，且n=m，所以我们可以暴力地找到循环，然后取模。

也就是说，我们使用3个值来哈希：循环的字符串，结尾的字符串，循环的次数。

这里的代码实现并不复杂，就不放代码了。需要注意的是，在这档数据范围下，直接用string暴力判，跑得比双模数hash还要快，因为hash中取模的次数太多，常数巨大，而这里的string最长只有100，所以直接比较还是比较快的。

##### 正解

注意到如果确定了一个起点，一个方向，那么我们就可以知道它往后跳一步的hash值。那么就可以倍增了。

数组的含义：
$$
fa[x][d][i]表示元素id，方向，步的幂次\\
S[2][x][d][i]第一维用于双模数hash\\
$$
其他就是倍增的板子了。

```cpp
#include<bits/stdc++.h>
#define REP(i,s,t) for(int (i)=(s);(i)<=(t);++(i))
#define DEP(i,s,t) for(int (i)=(s);(i)>=(t);--(i))
#define LL long long 
#define M 205
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
const LL MOD1=1e9+7;
const LL MOD2=1e9+9;
const LL BASE1=131;
const LL BASE2=133;
int n,m,K;
int dxy[8][2]={1,0,0,1,-1,0,0,-1,1,1,-1,-1,1,-1,-1,1};
char A[M][M];
int fa[M*M][8][17];
LL S[2][M*M][8][17];
struct node{
    LL n1,n2;
    bool operator < (const node& res)const{
        if(n2!=res.n2)return n2<res.n2;
        return n1<res.n1;
    }
};
map<node,int>mp;
map<node,int>::iterator it;
LL B[2][17];
LL gcd(LL a,LL b){return b==0?a:gcd(b,a%b);}
int id(int x,int y){return x*m+y;}
node mov(int x,int y,int step,int d){
    LL ans1=0,ans2=0,now=id(x,y);
    DEP(i,16,0)if(step&(1<<i)){
        ans1=(ans1+S[0][now][d][i]*B[0][i]%MOD1)%MOD1;
        ans2=(ans2+S[1][now][d][i]*B[1][i]%MOD2)%MOD2;
        now=fa[now][d][i];
    }
    return (node){ans1,ans2};
}
int main(){
    B[0][0]=BASE1;B[1][0]=BASE2;
    REP(i,1,16)B[0][i]=B[0][i-1]*B[0][i-1]%MOD1,B[1][i]=B[1][i-1]*B[1][i-1]%MOD2;
    cin>>n>>m>>K;mp.clear();K=min(K,n*m);
    REP(i,0,n-1)scanf("%s",A[i]);
    REP(i,0,n-1)REP(j,0,m-1)REP(r,0,7){
        int x=(i+dxy[r][0]+n)%n;
        int y=(j+dxy[r][1]+m)%m;
        fa[id(i,j)][r][0]=id(x,y);
        S[0][id(i,j)][r][0]=S[1][id(i,j)][r][0]=A[i][j]-'a'+1;
    }
    REP(k,1,16)REP(r,0,7)REP(i,0,n-1)REP(j,0,m-1){
        int bh=id(i,j);
        fa[bh][r][k]=fa[fa[bh][r][k-1]][r][k-1];
        S[0][bh][r][k]=(S[0][bh][r][k-1]+S[0][fa[bh][r][k-1]][r][k-1]*B[0][k-1]%MOD1)%MOD1;
        S[1][bh][r][k]=(S[1][bh][r][k-1]+S[1][fa[bh][r][k-1]][r][k-1]*B[1][k-1]%MOD2)%MOD2;
    }   
    REP(i,0,n-1)REP(j,0,m-1)REP(r,0,7){
        node has=mov(i,j,K,r);
        mp[has]++;
    }
    LL p=0,q=64LL*n*m*n*m;
    for(it=mp.begin();it!=mp.end();it++){
        LL k=it->second;
        p+=k*k;
    }
    LL g=gcd(p,q);
    printf("%lld/%lld\n",p/g,q/g);
    return 0;
}
```

~~中间一个ans2打成ans1调了两个小时~~