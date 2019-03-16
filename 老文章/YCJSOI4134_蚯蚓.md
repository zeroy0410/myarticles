---
title: NOIP2016 蚯蚓
---

#### 题目

[题目链接](https://www.luogu.org/problemnew/show/P2827)

大意是给你一些线段，每一次都取出最长的那一段，按比例分成两个线段，再放回去，同时其他的线段都增加一个长度。问你每次拿出来的那个线段长度是多少，后来$m$此操作后的序列是什么样的。

数据范围：

![tu](https://cdn.luogu.org/upload/pic/3458.png)

#### 思路

NOIP2016的题切分真的是很多。。

先来看看如何优雅地切分。

首先对于$m<=150000$的这些点，可以直接用优先队列跑。  65分！

这里的思考点只有一个：对于每次所有线段都增加一个长度的这一过程，我们把当前线段取出，手动减掉一个add,即改动局部而不是全部。

```c++
struct P65{
	priority_queue<int>Q;
	void solve(){
		while(!Q.empty())Q.pop();
		for(int i=1;i<=n;i++){
			Q.push(A[i]);
		}
		int add=0;//每只蚯蚓增加的值
		for(int i=1;i<=m;i++){
			int e=Q.top();Q.pop();
			e+=add; 
			if(i%t==0)printf("%d ",e);
			int a=1LL*u*e/v;
			int b=e-a;
			add+=q;
			Q.push(a-add);
			Q.push(b-add); 
		}
		cout<<endl;
		int cnt=0;
		while(!Q.empty()){
			cnt++;
			int e=Q.top();Q.pop();
			if(cnt%t==0){
				printf("%d ",e+add);
			}
		}
		cout<<endl;
	}
}p65;
```

再考虑$q=0$&&$a[i]<=1e7$的点，如果$q=0$我们不妨用一个数组存储每一个长度有多少个，由于蚯蚓的长度一定是单调递减的，所以我们用一个$mx$随着枚举而减少，就可以在O(n)的时间内完成运算了。 80分！

```c++
struct P80{
	int cnt[10000005];
	void solve(){
		clr(cnt,0);	
		int mx=0;
		for(int i=1;i<=n;i++){
			cnt[A[i]]++;
			mx=max(A[i],mx);
		}
		for(int i=1;i<=m;i++){
			cnt[mx]--;
			if(i%t==0)printf("%d ",mx);
			int a=1LL*u*mx/v;
			int b=mx-a;
			cnt[a]++;cnt[b]++;
			if(cnt[mx]==0){
				for(int j=mx-1;;j--)
					if(cnt[j]!=0){
						mx=j;
						break;	
					}
			}
		}
		cout<<endl;
		int ct=0;
		for(int i=mx;i>=0;i--){
			int cur=(ct+cnt[i])/t-ct/t;
			for(int j=1;j<=cur;j++)
				printf("%d ",i); 
			ct+=cnt[i];
		}
		cout<<endl;
	}
}p80;
```

这样水到80分，在比赛的时候也够了。

再来看正解：

首先，我们会发现答案隐隐然存在某种单调性。

先看q=0

那么我们每次取出的点必然是单调递减的。将它分成的点看作两个集合p一段，1-p一段，在这两个集合中的元素也一定是单调递减的（显然）

那么p!=0呢？

延续上述65%时的局部修改的思路，我们加进去的点是$val-add$e而val在变小，而add在变大，所以同样是递减的（显然）

那么就可以做了。

维护三个单调队列（单调的队列），每次从三个的头部取一个最大的切了，再放到相应的队列（p一个，1-p一个）中，就好了。

```c++
#include<bits/stdc++.h>
#define M 100005
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,m,q,u,v,t;
int A[2*M];
int Q1[70*M],Q2[70*M];
int L1=0,R1=-1,L2=0,R2=-1;//前闭后闭 
int l=1,r=1;
bool cmp(int a,int b){
    return a>b;	
}
int check(){
    int a1=-2e9,a2=-2e9,a3=-2e9;
    if(L1<=R1)a1=Q1[L1];
    if(L2<=R2)a2=Q2[L2];
    if(l<=r)a3=A[l];
    if(a1==-2e9&&a2==-2e9&&a3==-2e9)return 0;
    if(a1>=a2&&a1>=a3)return 1;
    else if(a2>=a1&&a2>=a3)return 2;
    return 3;
}
int main(){
    scanf("%d%d%d%d%d%d",&n,&m,&q,&u,&v,&t);
    for(int i=1;i<=n;i++){
        scanf("%d",&A[i]); 
    }
    sort(A+1,A+n+1,cmp);
    r=n;
    int add=0;
    int now;
    for(int i=1;i<=m;i++){
        int f=check();
        if(f==1){
            now=Q1[L1]+add;
            int a1=1LL*now*u/v;
            int a2=now-a1;
            add+=q;
            Q1[++R1]=a1-add;
            Q2[++R2]=a2-add;
            L1++;
        }
        else if(f==2){
            now=Q2[L2]+add;
            int a1=1LL*now*u/v;
            int a2=now-a1;
            add+=q;
            Q1[++R1]=a1-add;
            Q2[++R2]=a2-add;
            L2++;
        }
        else {
            now=A[l]+add;
            int a1=1LL*now*u/v;
            int a2=now-a1;
            add+=q;
            Q1[++R1]=a1-add;
            Q2[++R2]=a2-add;
            l++;
        }
        if(i%t==0)printf("%d ",now);
    }
    cout<<endl;
    int ct=0;
    while(1){
        ct++;
        int f=check();
        if(!f)break;
        if(f==1)now=Q1[L1]+add,L1++;
        else if(f==2)now=Q2[L2]+add,L2++;
        else now=A[l]+add,l++;
        if(ct%t==0)printf("%d ",now);
    }
    cout<<endl;
    return 0;	
}
```

