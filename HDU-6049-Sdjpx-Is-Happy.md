---
title: HDU 6049 Sdjpx Is Happy
date: 2018-08-30 13:06:55
tags: [dp,区间dp]
---

#### 题目
Sdjpx is a powful man,he controls a big country.There are n soldiers numbered 1~n(1<=n<=3000).But there is a big problem for him.He wants soldiers sorted in increasing order.He find a way to sort,but there three rules to obey.
1.He can divides soldiers into K disjoint non-empty subarrays.
2.He can sort a subarray many times untill a subarray is sorted in increasing order.
3.He can choose just two subarrays and change thier positions between themselves.
Consider A = [1 5 4 3 2] and P = 2. A possible soldiers into K = 4 disjoint subarrays is:A1 = [1],A2 = [5],A3 = [4],A4 = [3 2],After Sorting Each Subarray:A1 = [1],A2 = [5],A3 = [4],A4 = [2 3],After swapping A4 and A2:A1 = [1],A2 = [2 3],A3 = [4],A4 = [5].
But he wants to know for a fixed permutation ,what is the the maximum number of K?
Notice: every soldier has a distinct number from 1~n.There are no more than 10 cases in the input.

大意是给你一个序列要你进行如下操作：
1. 把它劈成k段，将每个区间中的数进行排序
2. 取出其中的两个区间，进行交换，使得整个区间有序
<!--more-->
#### 思路1   $O(n^3)$
先区间dp求出所有的$f[i][j]$表示$[i,j]$区间内的最大分割（不交换），具体方法是：枚举区间长度，同时记录每一个左端点对应的最右值$R[i]$。

记录$R[i]$的正确性体现在，区间具有单调性（区间越大$f$值越大）,枚举要使后面的区间与前面不产生冲突，显然后面的区间越小，不产生冲突的可能性越大，这就是我们基本的思路。

首先判断区间是否合法，若合法，则再分为两种情况：
1. 最小值在$R[i]$之后，那么此区间无法被劈开，只合成1个完整的区间。
2. 不然可以分为两个部分，$f[i][R[i]]$,$f[R[i]+1][j]$,两者直接合并即可。

其实处理这个$f$还有更简单的方法，我们可以枚举一个左端点，向后扫其右端点，开一个cnt记录这个las，如果遇到合法的区间则++cnt
不然cnt=0；
```c++

for(int i=1;i<=n;i++){
    int cnt=1;
    for(int j=i+1;j<=n;j++){
        if(mi(i,j)!=mi(i,j-1))cnt=0;
        if(mx(i,j)-mi(i,j)==j-i)f[i][j]=++cnt;
    }
}


```
其实这个和上面用$R[i]$的方法也没有什么本质区别，只是写起来更简单。

处理完了这个，我们就该考虑dp的问题。我们枚举的是两个区间是否进行交换，设对于区间$[l,r]$来说，其交换到的区间为$[t,k]$那么$k$为$mx[l][r]$,那么我们可以枚举t点。

这样我们就把区间劈成了$[1,l-1]$,$[l,r]$,$[r+1,t-1]$,$[t,k]$,$[k+1,n]$这几个部分，分别对每个部分进行判断就好了。

#### 代码1
```c++
#include<bits/stdc++.h>
#define M 3005
#define bit(x) (1<<x)
#define mx(l,r) query(l,r,1)
#define mi(l,r) query(l,r,0)
using namespace std;
int A[M],T,n;
int st[2][M][15],lg[M];
int f[M][M];//在此区间内最多能被分成多少段 
int R[M];
void Init_RMQ(bool f){
	for(int i=1;i<=n;i++)st[f][i][0]=A[i];
	for(int j=1;bit(j)<=n;j++)
		for(int i=1;i+bit(j)-1<=n;i++){
			if(f==1)
				st[f][i][j]=max(st[f][i][j-1],st[f][i+bit(j-1)][j-1]);
			else st[f][i][j]=min(st[f][i][j-1],st[f][i+bit(j-1)][j-1]);
		}
} 
int query(int l,int r,int f){
	int k=lg[r-l+1];
	if(f==1)
		return max(st[f][l][k],st[f][r-bit(k)+1][k]);
	return min(st[f][l][k],st[f][r-bit(k)+1][k]);
}
int main(){
	lg[1]=0;
	for(int i=2;i<=M-5;i++)lg[i]=lg[i>>1]+1;
	cin>>T;
	while(T--){
		memset(f,0,sizeof(f));
		scanf("%d",&n);
		for(int i=1;i<=n;i++)
			scanf("%d",&A[i]),R[i]=i,f[i][i]=1;
		Init_RMQ(0);
		Init_RMQ(1); 
		for(int i=2;i<=n;i++){
			for(int j=1;j+i-1<=n;j++){
				int r=i+j-1;
				if(mx(j,r)-mi(j,r)+1!=i)continue;//此区间不合法 
				if(mi(j,r)!=mi(j,R[j]))f[j][r]=1;
				else f[j][r]=f[j][R[j]]+f[R[j]+1][r];
				R[j]=r;
			}
		}
		int ans=f[1][n];//不进行交换 
		for(int i=1;i<=n;i++){
			for(int j=i;j<=n;j++){
				if(f[i][j]==0)continue;
				//此区间可用也
				int k=mx(i,j);
				if((i==1 || (f[1][i-1]>0 && mi(1,i-1)==1)) && (k==n || f[k+1][n]>0 && mx(k+1,n)==n)){
					for(int t=j+1;t<=k;t++){
						if(f[t][k]&&mi(t,k)==i)
							ans=max(ans,f[1][i-1]+1+f[j+1][t-1]+1+f[k+1][n]);
					}
				} 
			}
		}
		cout<<ans<<endl;
	}
	return 0;
}
```

#### 思路2 $O(n^2)$
转Little_Jian巨佬的说明。

> 　先考虑不能交换的情况。维护区间最大最小值mx[l,r],mn[l,r]。如果mx[l,r]−mn[l,r]=r−l，那么[l,r]就可以作为一个块。如果同时[l,r]还满足mn[l,r]=l，那么这个块在内部排序后就不需要动它了（已经处于应在的位置）。
　　设las[l]为l对应的最远的右端点满足[l,r]区间可以作为至少一个块，dp[l,r]为[l,r]区间能分的最多块数。当[l,r]可以作为一个块时，dp[l,r]=dp[l,las[r]]+1。但这个dp其实有更简单的写法，记录一个cnt代表dp[l,las[r]]。当mn[l,r]不等于mn[l,r-1]时，cnt=0。当[l,r]可以作为一个块时，cnt++。
　　接下来考虑交换，如果希望交换后答案大于不交换，那么肯定要在原来的基础上破坏块。枚举同一个块中的i,j。设l,rl,r为这个块的左右端点。要保证交换后合法，只能交换[l,i],[j,r]。那么只要保证交换后序列合法就能更新答案了。因为交换后中间的[i+1,j-1]为移动，所以在check时都传入了一个移动距离（见代码）。最后复杂度是O(n^2))的。

更好的阅读体验请点这》》[.](http://www.chenhowe.me/?p=1188#comment-188)


