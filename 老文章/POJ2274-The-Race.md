---
title: POJ2274_The_Race
date: 2018-08-30 10:12:05
tags: 链表
---

#### 题目

给出n(n<=250000)辆赛车，i赛车初始在xi(xi从小到大给出)，速度为vi，赛车在比赛时会发生超车（不会相撞）。求超车次数%1000000，并输出前10000次(不足10000次有几次输出几次)超车，格式为x y，表示x超过y，时间靠前的先输出，时间相同位置靠前的先输出，保证没有时间相同位置也相同的超车。 

![图一](https://odzkskevi.qnssl.com/8c2afd22e2106d55d6c6b242ba5ef0f3?v=1531299433)

<!--more-->

#### 思路

第一个问题比较简单，考虑到V的范围只有100，所以我们可以O(100*n)的方法进行运算。

我们必须洞察这样一个信息：对于每一次超车来说，其一定发生于相邻两车，这是本题最重要的一个性质，所以，我们可以想到使用链表来维护信息。先处理出相邻两车超越所需的时间。再使用优先队列进行维护。

将这个元素加入答案之后，我们还需要将此元素交换。

#### 代码

```c++
#include<stdio.h>
#include<iostream>
#include<queue>
#include<algorithm>
#define M 250005
#define MOD 1000000
#define LL long long 
using namespace std;
struct car{
	int x,v,num;
	bool operator < (const car& res) const{
		return x<res.x;
	}
}A[M];
struct node{
	int a,b;//相邻的两对 
	int a1,b1;//对应原序列中的位置 
	double time,x;
	bool operator < (const node& res) const{
		if(time!=res.time)return time>res.time;
		return x>res.x;
	}
};
priority_queue<node>Q;
int n;
int pre[M],nxt[M];
int cnt[105];
node Ans[10005];
int cans=0;
node calc(int i,int j){//在链表中的编号 
	node c;
	c.a=i;c.b=j;c.a1=A[i].num;c.b1=A[j].num; 
	c.time=1.0*(A[j].x-A[i].x)/(A[i].v-A[j].v);
	c.x=A[i].x+1.0*A[i].v*c.time;
	return c;
}
void Swap(int i,int j){
	nxt[pre[i]]=j;
	pre[j]=pre[i];
	pre[i]=j;
	nxt[i]=nxt[j];
	pre[nxt[j]]=i;
	nxt[j]=i;
	if(A[j].v<A[pre[j]].v&&pre[i]!=0)Q.push(calc(pre[j],j));
	if(A[i].v>A[nxt[i]].v&&nxt[i]!=n+1)Q.push(calc(i,nxt[i]));
}
int main(){
	LL ans=0;
	bool flag=0;
	cin>>n;
	for(int i=1;i<=n;i++){
		scanf("%d%d",&A[i].x,&A[i].v);
		A[i].num=i;//编号 
		for(int j=100;j>A[i].v;j--){
			ans+=cnt[j];
			if(ans>MOD)ans-=MOD,flag=1;
		}
		cnt[A[i].v]++;
	}
	cout<<ans<<endl;
	LL T=ans;
	if(flag)T=10000;
	sort(A+1,A+n+1);
	for(int i=0;i<=n+1;i++)pre[i]=i-1,nxt[i]=i+1;
	for(int i=nxt[0];i!=pre[n+1];i=nxt[i]){
		if(A[i].v>A[nxt[i]].v)Q.push(calc(i,nxt[i]));
	}
	while(!Q.empty()&&cans<10000){
		if(T==0)break;
		node k=Q.top();Q.pop();
		if(nxt[k.a]!=k.b||pre[k.b]!=k.a)continue;//链式结构被破坏，无法作为答案 
		Ans[++cans]=k;T--;
		Swap(k.a,k.b);
	}
	for(int i=1;i<=cans;i++){
		cout<<Ans[i].a1<<' '<<Ans[i].b1<<endl;
	} 
	return 0;
}
```


