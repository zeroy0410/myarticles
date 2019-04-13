---
title: JOISC2014 历史研究
---



#### 题目

IOI 国历史研究的大牛——JOI 教授，最近获得了一份被认为是古代 IOI 国的住民写下的日记。JOI 教授为了通过这份日记来研究古代 IOI 国的生活，开始着手调查日记中记载的事件。
日记中记录了连续 $N$ 天发生的事件，每天发生一件事件。
事件有种类之分。第$i$天发生的事件的种类用一个整数$X_i$表示，$X_i$​越大，事件的规模就越大。
JOI 教授决定用如下的方法分析这些日记：

1. 选择日记中连续的一些天作为分析的时间段；
2. 事件种类 $t$ 的重要度为 $t\times(这段时间内重要度为 t 的事件数)$；
3. 计算出所有事件种类的重要度，输出其中的最大值。

请制作一个帮助教授分析的程序，每次给出分析的区间，你需要输出重要度的最大值。



#### 思路

~~此题之前根本不会莫队~~

首先先介绍一下莫队吧。

如果对于一个二元组$[l,r]$，它转移到$[l+1,r],[l-1,r],[l,r+1],[l,r-1]$都只需要$O(1)$的复杂度，那么就可以使用莫队。

**基础莫队的流程**

首先将询问按照$l$分块，不妨设块大小为$S$，那么$l$所属的块就是$\frac{l}{s}$。

在块内的元素按照$r​$为关键字排序。

对于每一次转移来说，显然复杂度就是$(l_1,r_1)$到$(l_2,r_2)$的曼哈顿距离，对于纵坐标来说，由于块内已经排序过了，所以处理一个块的复杂度是$O(n)$，而对于横坐标来说，一次转移最高复杂度为$O(S)$，然后相邻的块之间的转移为$O(n)$。

所以总的复杂度为$O(\frac{n^2}{S}+mS+\frac{n^2}{S})$，将$n,m$看作同阶，令$S=\sqrt{n}$，可达到最优复杂度$O(n\sqrt{n})$

**模板（from Sengxian）**

```c++
int l = 0, r = 0, nowAns = 0;

inline void move(int pos, int sign) {
    // update nowAns
}

void solve() {
    BLOCK_SIZE = int(ceil(pow(n, 0.5)));
      sort(querys, querys + m);
    for (int i = 0; i < m; ++i) {
        const query &q = querys[i];
        while (l > q.l) move(--l, 1);
        while (r < q.r) move(r++, 1);
        while (l < q.l) move(l++, -1);
        while (r > q.r) move(--r, -1);
        ans[q.id] = nowAns;
    }
}
```



#### 这道题的特殊之处

确认了这个方向，接下来再看此题，莫队是没错，但是如何在两种情况间$O(1)$的转移呢？直接套板子显然是做不到的，所以这里放宽限制，只在$r$处套用板子，对于$l$，要利用题目的特殊之处。

注意看一下代码：

```c++
void update(int x,int d){
	x=A[x];
	Ans=max(Ans,val[x]+=1LL*d*B[x]);
}
```

当$d>0$时它是正确的，所以说，可以把题目转化为只加点的形式，对于一个块来说，当$r>块的边界$的时候，才用莫队的方法统计ans，然后暴力扫$[l,块的边界]$，用上面的update实现只加不减，保证正确性，这样一次的复杂度也是$O(S)$的，如果$[l,r]$都在同一个块内，就可以直接暴力了，复杂度也是$O(S)$的，所以是总的复杂度是$O(n\sqrt n)$的。



#### 代码

```c++
#include<bits/stdc++.h>
#define M 100005
#define S 330 
#define LL long long 
using namespace std;
struct node{
	int l,r,id;
	bool operator < (const node& res) const{return r<res.r;}
};
int n,m,A[M],B[M],bn,cnt[M]; 
LL ans[M],val[M],Ans;
vector<node>vec[S];
void update(int x,int d){
	x=A[x];
	Ans=max(Ans,val[x]+=1LL*d*B[x]);
}
int main(){
	freopen("history.in","r",stdin);
	freopen("history.out","w",stdout); 
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)scanf("%d",&A[i]),B[i]=A[i];
	sort(B+1,B+n+1);bn=unique(B+1,B+n+1)-B-1;
	for(int i=1;i<=n;i++)A[i]=lower_bound(B+1,B+bn+1,A[i])-B;
	int s=320,c=n/s;
	for(int i=1,l,r;i<=m;i++){
		scanf("%d%d",&l,&r);
		vec[l/s].push_back((node){l,r,i});
	}
	int L=0,R=0;
	for(int i=0;i<=c;i++){
		sort(vec[i].begin(),vec[i].end());
		if(vec[i].size()){ 
			Ans=0;memset(val,0,sizeof(val)); 
			int lim=min(i*S+S-1,n),L=lim,R=lim;
			LL tmp;update(L,1);
			for(int j=0;j<vec[i].size();j++){
				int l=vec[i][j].l,r=vec[i][j].r,id=vec[i][j].id; 
				if(r>=lim){
					while(R<r)update(++R,1);
					tmp=Ans;
					while(L>l)update(--L,1); 
					ans[id]=Ans;
					while(L<lim)update(L++,-1);
					Ans=tmp;
				}
				else{
					LL &mx=ans[id];
					for(int k=l;k<=r;k++)mx=max(mx,1LL*B[A[k]]*(++cnt[A[k]]));	
					for(int k=l;k<=r;k++)cnt[A[k]]=0;
				}
			}
		}
	}
	for(int i=1;i<=m;i++)
		printf("%lld\n",ans[i]);
	return 0;
}
```



