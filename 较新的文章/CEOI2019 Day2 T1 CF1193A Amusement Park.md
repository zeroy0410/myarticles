#### 题目

You have been hired to supervise the project of a new amusement park. The park will have a special gimmick: directed slides that can get customers from one attraction to another quickly and in an entertaining way.

The park owner has given you the current project: a list of planned attractions and a list of slides that should be built between them. However, him being a businessman, he casually envisioned the impossible: among other things, he projected a slide coming from the Haunted Castle to the Roller Coaster, another from the Roller Coaster to the Drop Tower, and a third from the Drop Tower to the Haunted Castle. As the slides can only go downhill, it is evident why this is a problem. You don't have the luxury of ignoring the laws of physics when building the park, so you have to request changes in the project. Maybe he would accept reversing the slide between the Drop Tower and the Haunted Castle?

Formally:

- The **project** is a list of attractions and a list of directed slides. Each slide starts at one attraction and ends at another attraction.
- A **proposal** is obtained from the project by reversing the directions of some slides (possibly none or all of them).
- A proposal is **legal** if there is a way to assign an elevation to each attraction in such a way that every slide goes downhill.
- The **cost** of a proposal is the number of slides whose directions were reversed.

For a given project, find and report the sum of costs all legal proposals. Since this number may be large, output it modulo $998244353​$

[题目链接](<https://codeforces.com/contest/1193/problem/A>)

n个点，m条边的有向图，边方向由你决定。问有使图无环的方案的贡献之和。

#### 思路

##### 19point

暴力二进制枚举每条边的方向，$O(n)​$check，总复杂度$O(2^mn)​$。

##### 42point

暴力阶乘枚举最后的拓扑序，$O(n)​$check，总复杂度$O(n!n)​$

##### 正解

Paulliant考场打出了正解，先%一下。

考虑$O(3^n)​$的写法。

考虑加入一个集合时的操作。

假设现在我们要得到集合S的答案，假设它最后一次加入了点集s。

只要在加入的时候保证s中的点全部入度为0或者全部出度为0，就可以保证集合S依然是个$DAG$

保证了这点之后，我们定义$dp[S]​$为集合$S​$为$DAG​$的方案数。

看上去转移方程已经确定了：$dp[S]=\sum_{s∈S}{dp[S\oplus s]}​$

然而这样会算重好多情况。可能一个相同的集合，我们分了好几次加入，和一次加入是同一种情况，所以就要利用容斥了。

稍微想一下会发现这就是一个普通的子集容斥。

下面再证我们覆盖到了所有的解。

对于一个DAG来说，它一定存在某个拓扑序列，只要按照拓扑序从大到小或者从小到大加边，就一定是满足上面的情况，所以是答案是不会由遗漏的。



#### 代码

```c++
#include<bits/stdc++.h>
#define LL long long
#define have(x,y) (((x)>>((y)-1))&1)
using namespace std;
const int P=998244353;
int n,m;
int U[405],V[405];
LL dp[1<<18|5];int cnt[1<<18|5];
bool mark[1<<18|5];
void Add(LL &x,LL y){
	x+=y;
	if(x>=P)x-=P;
	if(x<0)x+=P;
}
void Solve(){
	cnt[0]=-1;for(int i=1;i<1<<n;i++)cnt[i]=-cnt[i&(i-1)];
	dp[0]=1;
	for(int i=1;i<1<<n;i++)
		for(int j=1;j<=m;j++)
			if(have(i,U[j]) and have(i,V[j])){
				mark[i]=true;break;
			}
	for(int i=1;i<1<<n;i++)
		for(int j=i;j>=1;j=(j-1)&i)
			if(!mark[j])Add(dp[i],dp[i^j]*cnt[j]);
	printf("%lld\n",dp[(1<<n)-1]*m%P*499122177%P);
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=m;i++)scanf("%d%d",&U[i],&V[i]);
	Solve();
	return 0;
}
```

