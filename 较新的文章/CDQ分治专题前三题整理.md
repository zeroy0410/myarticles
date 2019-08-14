### CDQ分治专题前三题整理



#### A. Mokia

[题目链接](https://www.luogu.org/problemnew/show/P4390)

##### 题目

给定一个二维平面，每一次操作加一个点和询问矩形内有几个点。

##### 思路

先利用树状数组消除一个维度，再利用CDQ分治分治掉一个维度。

具体而言：

对于，我们把一个插入操作看成3个参数：$x_i$,$y_i$,$t_i$。$t_i$表示时间。那么对于一个查询，$x_1$,$x_2$,$y_1$,$y_2$,$qt$来说，实际上如果它被包含的话，那么就是:$x_2>=x_i>=x_1$,$y_2>=y_i>=y_1$,$t_i<qt$。

然后对于修改和查询操作，我们利用前缀和的性质将其拆分。

这就是一个三维偏序的板子了。

##### 代码

``` c++

#include<bits/stdc++.h>
#define M 2000005
#define lowbit(x) (x&-x)
using namespace std;
int n,op;
struct BIT{
    int C[M];
    BIT(){
        memset(C,0,sizeof(C));
    }
    int sum(int x){
        int res=0;
        while(x){
            res+=C[x];
            x-=lowbit(x);
        }
        return res;
    }
    void add(int x,int d){
        while(x<=n){
            C[x]+=d;
            x+=lowbit(x);
        }
    }
}T;
struct node{//排序x，add y 
    int x,y,a,id,as;//query的id,答案 
    bool flag;//是query还是add
    int mp;//映射回原来的a[i] 
    bool operator < (const node& res) const {
        if(x!=res.x)return x<res.x;
        return flag<res.flag;
    }
}A[M<<2],q[M<<2];//左边的更新，右边的查询 
int tot=0;
struct que{
    int x1,y1,x2,y2,as;
    void solve(node res){
        if(res.x==x1-1&&res.y==y1-1)as+=res.as;
        else if(res.x==x2&&res.y==y2)as+=res.as;
        else as-=res.as; 
    }
}ans[M<<2]; 
int ID=0;
vector<node>now;
void CDQ(int l,int r){
    if(l>=r)return;
    int mid=(l+r)>>1;
    CDQ(l,mid);CDQ(mid+1,r);
    now.clear();
    int s=0;
    for(int i=l;i<=mid;i++)
        if(!A[i].flag)q[++s]=A[i],q[s].mp=i;
    for(int i=mid+1;i<=r;i++)
        if(A[i].flag)q[++s]=A[i],q[s].mp=i;
    sort(q+1,q+s+1);
    for(int i=1;i<=s;i++){
        if(q[i].flag==0)T.add(q[i].y,q[i].a),now.push_back(q[i]);
        else if(q[i].flag==1)A[q[i].mp].as+=T.sum(q[i].y);
    }
    for(int i=0;i<(int)now.size();i++){
        T.add(now[i].y,-now[i].a);
    }
}
int main(){
    while(1){
        scanf("%d",&op);
        if(op==3)break;
        if(op==0)scanf("%d",&n);
        if(op==1){
            int x,y,a;
            scanf("%d%d%d",&x,&y,&a);
            A[++tot]=(node){x,y,a,0,0}; 
        }
        if(op==2){
            int x1,y1,x2,y2;
            scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
            ans[++ID]=(que){x1,y1,x2,y2,0};
            A[++tot]=(node){x1-1,y1-1,0,ID,0,1};
            A[++tot]=(node){x1-1,y2,0,ID,0,1};
            A[++tot]=(node){x2,y1-1,0,ID,0,1};
            A[++tot]=(node){x2,y2,0,ID,0,1};
        }
    }
    CDQ(1,tot);
    for(int i=1;i<=tot;i++)
        if(A[i].flag)ans[A[i].id].solve(A[i]);
    for(int i=1;i<=ID;i++)
        printf("%d\n",ans[i].as);
    return 0;
}
```
#### B. 动态逆序对

##### 题目
相信是大部分人的CDQ入门题，这里就不作说明了。

##### 代码

```c++
#include<bits/stdc++.h>
#define M 100005
#define lowbit(x) (x&-x)
#define LL long long 
using namespace std;
int n,m;
LL res=0; 
LL ans[M];
int pos[M];
struct node{
    int t,x;//时间，权值 
}A[M],tmp[M];
struct BIT{
    int C[M];
    BIT(){
        memset(C,0,sizeof(C));
    }
    int sum(int x){
        int res=0;
        while(x){
            res+=C[x];
            x-=lowbit(x);
        }
        return res;
    }
    void add(int x,int d){
        while(x<=n){
            C[x]+=d;
            x+=lowbit(x);
        }
    }
}T1,T2;
void CDQ(int l,int r){
    if(l==r)return;
    int mid=(l+r)>>1;
    CDQ(l,mid);CDQ(mid+1,r);
    for(int i=l;i<=mid;i++)T1.add(A[i].t,1);
    int i=l,j=mid+1,k=l;
    while(i<=mid&&j<=r){
        if(A[i].x<A[j].x){//如果有一个A[i]小于A[j] 
            res+=j-mid-1;
            T1.add(A[i].t,-1);
            if(A[i].t!=1)ans[m-A[i].t+1]+=T2.sum(A[i].t);//归并排序求逆序对顺便更新ans 
            tmp[k++]=A[i++];
        }else{
            T2.add(A[j].t,1);
            if(A[j].t!=1)ans[m-A[j].t+1]+=T1.sum(A[j].t);
            tmp[k++]=A[j++];
        }
    } 
    while(i<=mid){
        res+=j-mid-1;
        T1.add(A[i].t,-1);
        if(A[i].t!=1)ans[m-A[i].t+1]+=T2.sum(A[i].t);
        tmp[k++]=A[i++];
    }
    while(j<=r){
        T2.add(A[j].t,1);
        if(A[j].t!=1)ans[m-A[j].t+1]+=T1.sum(A[j].t);
        tmp[k++]=A[j++];
    }
    for(int q=mid+1;q<=r;q++)T2.add(A[q].t,-1);
    for(int q=l;q<=r;q++)A[q]=tmp[q]; 
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++)
        scanf("%d",&A[i].x),pos[A[i].x]=i,A[i].t=1;
    int ti=m;
    for(int i=1,x;i<=m;i++){
        scanf("%d",&x);
        A[pos[x]].t=ti--;//被删的时间点 
    }
    CDQ(1,n);
    for(int i=1;i<=m;i++){
        cout<<res<<endl;
        res-=ans[i]; 
    }
    return 0;
}
```
#### C. 天使玩偶

##### 题目

[题目链接](https://www.luogu.org/problemnew/show/P4169)

Ayu 在七年前曾经收到过一个天使玩偶，当时她把它当作时间囊埋在了地下。而七年后 的今天，Ayu 却忘了她把天使玩偶埋在了哪里，所以她决定仅凭一点模糊的记忆来寻找它。

我们把 Ayu 生活的小镇看作一个二维平面坐标系，而 Ayu 会不定时地记起可能在某个点 (xmy) 埋下了天使玩偶；或者 Ayu 会询问你，假如她在 (x,y) ，那么她离近的天使玩偶可能埋下的地方有多远。

因为 Ayu 只会沿着平行坐标轴的方向来行动，所以在这个问题里我们定义两个点之间的距离为dist(A,B)=|Ax-Bx|+|Ay-By|。其中 Ax 表示点 A的横坐标，其余类似。

##### 思路

首先将询问分为几个方向，单独考虑一个方向。

先考虑回忆出来的点都在询问的点左下方时：（A为询问的点）

$$
Dis(A,B)=|x_A-x_B|+|y_A-y_B|=(x_A+y_A)-(x_B+y_B)
$$

则当$x_B + y_B$取到最大值时,$Dis(A,B)$有最小值。

于是问题转化为3维偏序问题，仍可以用CDQ分治求解。

~~代码难写难调细节多，而且还卡常。~~

##### 代码

```c++
#include<bits/stdc++.h>
#define M 500005
#pragma GCC optimize(2)
#define max(a,b) (a>b?a:b)
#define min(a,b) (a>b?b:a)
int n,m,acnt,bcnt,inf;
struct node{
    int x,y,t,id;
}A[M<<1],bf[M<<1],tmp[M<<1];
int B[M<<2];
int ans[M<<1];
int wk[M<<1],wcnt;
#define rd(n) FastIO::read(n)
namespace FastIO {
    const int SIZE = 1 << 16;
    char buf[SIZE], obuf[SIZE], str[60];
    int bi = SIZE, bn = SIZE, opt;
    int read(char *s) {
        while (bn) {
            for (; bi < bn && buf[bi] <= ' '; bi++);
            if (bi < bn) break;
            bn = fread(buf, 1, SIZE, stdin);
            bi = 0;
        }
        int sn = 0;
        while (bn) {
            for (; bi < bn && buf[bi] > ' '; bi++) s[sn++] = buf[bi];
            if (bi < bn) break;
            bn = fread(buf, 1, SIZE, stdin);
            bi = 0;
        }
        s[sn] = 0;
        return sn;
    }
    bool read(int& x) {
        int n = read(str), bf;
  
        if (!n) return 0;
        int i = 0; if (str[i] == '-') bf = -1, i++; else bf = 1;
        for (x = 0; i < n; i++) x = x * 10 + str[i] - '0';
        if (bf < 0) x = -x;
        return 1;
    }
};
struct BIT1{
    int C[M<<1];
    BIT1(){memset(C,128,sizeof(C));}
    void add(int x,int d,bool f){
        if(f==0){
            while(x<=bcnt){
                C[x]=max(C[x],d);
                x+=(x&-x);
            }
        }
        else {
            while(x){
                C[x]=max(C[x],d);
                x-=(x&-x);
            }
        }
    }
    int sum(int x,bool f){
        int res=-1e9;
        if(f==0){
            while(x){
                res=max(res,C[x]);
                x-=(x&-x);
            }
        }
        else {
            while(x<=bcnt){
                res=max(res,C[x]);
                x+=(x&-x);
            }
        }
        return res;
    }
    void clear(int x,int d,bool f){
        if(f==0){
            while(x<=bcnt){
                C[x]=d;
                x+=(x&-x);
            }
        }
        else {
            while(x){
                C[x]=d;
                x-=(x&-x);
            }
        }
    }
}T1,T2;
void CDQ1(int l,int r){
    if(l==r)return;
    int mid=(l+r)>>1;
    CDQ1(l,mid);CDQ1(mid+1,r);
    int i=l,j=mid+1,k=l;wcnt=0;
    while(i<=mid&&j<=r){
        if(A[i].x<=A[j].x){
            if(A[i].t==1){
                T1.add(A[i].y,B[A[i].x]+B[A[i].y],0);
                T2.add(A[i].y,B[A[i].x]-B[A[i].y],1);
                wk[++wcnt]=A[i].y;
            }
            tmp[k++]=A[i++];
        }
        else {
            if(A[j].t==2){
                ans[A[j].id]=min(ans[A[j].id],B[A[j].x]+B[A[j].y]-T1.sum(A[j].y,0));
                ans[A[j].id]=min(ans[A[j].id],B[A[j].x]-B[A[j].y]-T2.sum(A[j].y,1));
            }
            tmp[k++]=A[j++];
        }
    }
    while(i<=mid)tmp[k++]=A[i++];
    while(j<=r){
        if(A[j].t==2){
            ans[A[j].id]=min(ans[A[j].id],B[A[j].x]+B[A[j].y]-T1.sum(A[j].y,0));
            ans[A[j].id]=min(ans[A[j].id],B[A[j].x]-B[A[j].y]-T2.sum(A[j].y,1));
        }
        tmp[k++]=A[j++];
    }
    for(int i=l;i<=r;i++)A[i]=tmp[i];
    for(int i=1;i<=wcnt;i++)T1.clear(wk[i],-1e9,0),T2.clear(wk[i],-1e9,1);
}
void CDQ2(int l,int r){
    if(l==r)return;
    int mid=(l+r)>>1;
    CDQ2(l,mid);CDQ2(mid+1,r);
    int i=l,j=mid+1,k=l;wcnt=0;
    while(i<=mid&&j<=r){
        if(A[i].x>=A[j].x){
            if(A[i].t==1){
                T1.add(A[i].y,-B[A[i].x]+B[A[i].y],0);
                T2.add(A[i].y,-B[A[i].x]-B[A[i].y],1);
                wk[++wcnt]=A[i].y;
            }
            tmp[k++]=A[i++];
        }
        else {
            if(A[j].t==2){
                ans[A[j].id]=min(ans[A[j].id],-B[A[j].x]+B[A[j].y]-T1.sum(A[j].y,0));
                ans[A[j].id]=min(ans[A[j].id],-B[A[j].x]-B[A[j].y]-T2.sum(A[j].y,1));
            }
            tmp[k++]=A[j++];
        }
    }
    while(i<=mid)tmp[k++]=A[i++];
    while(j<=r){
        if(A[j].t==2){
            ans[A[j].id]=min(ans[A[j].id],-B[A[j].x]+B[A[j].y]-T1.sum(A[j].y,0));
            ans[A[j].id]=min(ans[A[j].id],-B[A[j].x]-B[A[j].y]-T2.sum(A[j].y,1));
        }
        tmp[k++]=A[j++];
    }
    for(int i=1;i<=wcnt;i++)T1.clear(wk[i],-1e9,0),T2.clear(wk[i],-1e9,1);
    for(int i=l;i<=r;i++)A[i]=tmp[i];
}
int main(){
//  freopen("data.in","r",stdin);
//  freopen("data.out","w",stdout);
    memset(ans,127,sizeof(ans));inf=ans[0];
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
        rd(A[i].x),rd(A[i].y),A[i].t=1,B[++bcnt]=A[i].x,B[++bcnt]=A[i].y;
    for(int i=1;i<=m;i++)
        rd(A[n+i].t),rd(A[n+i].x),rd(A[n+i].y),A[n+i].id=n+i,B[++bcnt]=A[n+i].x,B[++bcnt]=A[n+i].y;
    std::sort(B+1,B+bcnt+1);
    bcnt=std::unique(B+1,B+bcnt+1)-B-1;
    n=n+m;
    for(int i=1;i<=n;i++){
        A[i].x=std::lower_bound(B+1,B+bcnt+1,A[i].x)-B;
        A[i].y=std::lower_bound(B+1,B+bcnt+1,A[i].y)-B;
    }
    for(int i=1;i<=n;i++)bf[i]=A[i];
    CDQ1(1,n);
    for(int i=1;i<=n;i++)A[i]=bf[i];
    CDQ2(1,n);
    for(int i=1;i<=n;i++)
        if(ans[i]!=inf)printf("%d\n",ans[i]);
    return 0;
}
```