#### 题目

 	可怕的流感在AC之城爆发了！AC之城有M个居民，每个人有一个ID编号，从0到M-1。由于病毒十分强大，每个人可能被多次感染。在流感爆发第一天，病毒携带者被我们称作“病原体”，他们的编号已知，病毒要靠他们传播。  

 	在接下来的每一天，如果有一个前一天感染病毒的、编号为a的病人，以及一个编号为$b$的“病原体”，那么编号为p的居民将在今天被感染，其中$p=(a*b)%M$。另外，$a$和$b$不一定要严格不同。  

 	举一个例子，假设AC之城有101个居民，其中5号和50号为“病原体”。在第一天，感染病毒的人为5号和50号。第二天，25号、48号(250%101)和76号(2500%101)将被感染。第三天，77号将成为在这天被感染的人中的一个((48*50)%101=77)。  

 	现在，我们想要知道，将在第$K$天被感染？

#### 思路

首先思考60分的写法。

STL中有一个很好用的数据结构bitset帮助我们强搞。

本地跑了4s+？不要紧，lemon上开了O(2)快上天。至少笔者在本地手动开了O(2)让4s变成了300ms-！！

```cpp
struct P60{
    bitset<1505>num[1505];//当前点的下一个点能到的情况 
    bitset<1505>Q[2]; 
    int B[M],bcnt;
    void solve(){
        Q[0].reset();Q[1].reset();
        for(int i=1,x;i<=n;i++){
            scanf("%d",&x);
            B[++bcnt]=x;
            Q[1][x]=1;
        }
        for(int j=0;j<m;j++){
            num[j].reset();
            for(int i=1;i<=bcnt;i++){
                num[j][j*B[i]%m]=1; 
            }
        } 
        int cur=1;
        for(int i=2;i<=K;i++){
            cur=!cur;
            Q[cur].reset();
            for(int j=0;j<m;j++){
                if(Q[!cur][j])Q[cur]|=num[j];   
            }
        }
        for(int i=0;i<m;i++){
            if(Q[cur][i])printf("%d ",i);
        }
        cout<<endl;
    }
}p60;
```

然而P100的数据范围那么大，常见的算法肯定是不行了，所以只能思考log级别的算法。

再看一遍题面，每次都是两列的数相互乘起来。
$$
假设初始的序列是\\
s1，s2，s3，s4....sn\\
那么一天之后的序列就是\\
s1^2，s1*s2,s1*s3,s1*s4...sn*sn
$$
天数越来越多，上面的幂次是以加法的形式增长的，这就满足了结合律的性质了。目前处理这个的算法就是快速幂。

```cpp
#include<bits/stdc++.h>
#define LL long long
#define M 1505
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
struct node{
    int A[M],tot;   
};
LL K;
int m,n;
node x;
bool mark[M];
node mul(node& a,node &b){
    memset(mark,0,sizeof(mark));
    for(int i=1;i<=a.tot;i++)
        for(int j=1;j<=b.tot;j++)
            mark[a.A[i]*b.A[j]%m]=1;    
    node ans;
    ans.tot=0;
    for(int i=0;i<m;i++)
        if(mark[i])ans.A[++ans.tot]=i;  
    return ans;
}
node qkpow(node &a,LL b){
    node ans;ans.tot=1;ans.A[1]=1;
    while(b){
        if(b&1)ans=mul(ans,a);
        a=mul(a,a);
        b>>=1;    
    }
    return ans;
}
int main(){//文件名 LL mod 内存 极限数据 
    cin>>K>>m>>n;
    for(int i=1;i<=n;i++){
        scanf("%d",&x.A[i]);
    }
    x.tot=n;
    node ans=qkpow(x,K);
    for(int i=1;i<=ans.tot;i++){
        printf("%d ",ans.A[i]);
    }   
    return 0;
}
```

