---
title: 动态逆序对（CDQ分治）
date: 2018-08-30 20:42:19
tags: [CDQ分治,树套树]
---



#### 题目

对于序列 $A$ ，它的逆序对数定义为满足 $i<j$ ，且 $Ai>Aj$ 的数对 $(i,j)$ 的个数。给定 $1$ 到 $n$ 的一个排列，按照某种顺序依次删除 $m$ 个元素，你的任务是在每次删除一个元素之前统计整个序列的逆序对数。



#### 思路1

我们反向来看，**离线询问**，将问题转变成一个一个往序列中加入元素。

接着很容易发现这是一道三元组的题目，对于两个数来说，必须在满足题目条件的同时，满足其中一数加入的时间比询问的那个数要早，所以一重的数据结构一般难以解决，所以我们要采用多重的数据结构，这里采用了树状数组套到动点线段树。
<!--more-->
```c++
#include<bits/stdc++.h>
#define M 100005 
#define LL long long
#define lowbit(x) (x&-x) 
using namespace std;
int pos[M],n,m,ver[M];
int tot=0;
struct Seg{
    int Lson[M*400],Rson[M*400],sum[M*400];
    void Insert(int x,int L,int R,int& rt){
        if(!rt)rt=++tot;
        sum[rt]++;
        if(L==R)return;
        int mid=(L+R)>>1;
        if(x<=mid)Insert(x,L,mid,Lson[rt]);
        else Insert(x,mid+1,R,Rson[rt]);
    }
    int query(int l,int r,int L,int R,int rt){
        if(!rt)return 0;
        if(L==l&&R==r)return sum[rt];
        int mid=(L+R)>>1;
        if(r<=mid)return query(l,r,L,mid,Lson[rt]);
        else if(l>mid)return query(l,r,mid+1,R,Rson[rt]);
        return query(l,mid,L,mid,Lson[rt])+query(mid+1,r,mid+1,R,Rson[rt]);
    } 
}T; 
int sum(int x,int l,int r){
    int res=0;
    while(x){
        res+=T.query(l,r,1,n,ver[x]);
        x-=lowbit(x);
    }
    return res;
}
void add(int x,int d){
    while(x<=n){
        T.Insert(d,1,n,ver[x]);
        x+=lowbit(x);
    }
}
int del[M];
LL ans[M];
LL tt=0;
bool mark[M];
int main(){
    scanf("%d%d",&n,&m);
    for(int i=1,x;i<=n;i++)  
        scanf("%d",&x),pos[x]=i;
    for(int i=1;i<=m;i++){
        scanf("%d",&del[i]);
        mark[del[i]]=1;
    } 
    for(int i=1;i<=n;i++){
        if(!mark[i]){
            add(pos[i],i);
            tt+=sum(pos[i]-1,i+1,n)+sum(n,1,i-1)-sum(pos[i],1,i-1);
        }
    } 
    for(int i=m;i>=1;i--){
        add(pos[del[i]],del[i]);
        tt+=sum(pos[del[i]]-1,del[i]+1,n)+sum(n,1,del[i]-1)-sum(pos[del[i]],1,del[i]-1);
        ans[i]=tt;
    }
    for(int i=1;i<=m;i++)
        printf("%lld\n",ans[i]);
    return 0;
}
```



#### 思路2

CDQ分治。

我们用CDQ分治一维，树状数组维护一维，排序一维，可以解决掉这个问题。

重点看分治的部分。



此题的特殊性就在于，比普通的用归并排序求逆序对多了时间这个维度，所以我们可以用树状数组维护时间这维，然后就是归并来算了。



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



