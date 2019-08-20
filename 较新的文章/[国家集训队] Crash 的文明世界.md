#### 题目
[题目链接](https://www.luogu.org/problem/P4827)

#### Pre

$$
m^n=\sum_{i=0}^{m}{\begin{Bmatrix}n\\i\end{Bmatrix}}C_m^i*i!\\
$$

#### Part1

$$
ans_x=\sum_{i=0}^{n}dis(i,x)^k\\
=\sum_{i=0}^{n}\sum_{j=0}^{dis(i,x)}{\begin{Bmatrix}k\\j\end{Bmatrix}}
C_{dis(i,x)}^j*j!\\
=\sum_{i=0}^{n}\sum_{j=0}^{k}{\begin{Bmatrix}k\\j\end{Bmatrix}}
C_{dis(i,x)}^j*j!\\
=\sum_{j=0}^{k}{\begin{Bmatrix}k\\j\end{Bmatrix}}*j!\sum_{i=0}^{n}
C_{dis(i,x)}^j\\
=\sum_{j=0}^{k}{\begin{Bmatrix}k\\j\end{Bmatrix}}*j!\sum_{i=0}^{n}
(C_{dis(i,x)-1}^{j}+C_{dis(i,x)-1}^{j-1})
$$

#### Part2

问题转换为求$\sum_{i=0}^{n}C_{dis(i,x)}^{j}$。

令$f[x][j]$表示$x$为根时，上面这个式子的答案。

先考虑指定某个点为根的答案统计。

令$dp[x][j]=\sum_{i\ is\ son}C_{dis(i,x)}^{j}$

然后看上面的最后一个式子，可以发现：
$$
dp[x][j]=\sum_{u\ is\ son}dp[u][j]+dp[u][j-1]
$$



然后进行一波换根$dp$就好了。

```c++
#include<bits/stdc++.h>
#define LL long long
#define M 50005
using namespace std;
const int mod=10007;
int n,K,h[M],tt;
int now,A,B,Q,L;
struct edge{
	int nxt,to;
}G[M<<1];
void Add(int a,int b){
	G[++tt]=(edge){h[a],b};
	h[a]=tt;
}
int dp[M][155],f[M][155],tmp[155];
int str[155][155],fac[M];
void Init(){
	str[0][0]=str[1][1]=1;
	for(int i=2;i<=K;i++)
		for(int j=1;j<=i;j++)
			str[i][j]=(str[i-1][j-1]+1LL*j*str[i-1][j])%mod;
	fac[0]=1;
	for(int i=1;i<=n;i++)fac[i]=1LL*fac[i-1]*i%mod;
}
void dfs(int x,int fa){
	dp[x][0]=1;
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==fa)continue;
		dfs(u,x);
		for(int j=1;j<=K;j++)dp[x][j]=(dp[x][j]+dp[u][j]+dp[u][j-1])%mod;
		dp[x][0]=(dp[x][0]+dp[u][0])%mod;
	}
}
void redfs(int x,int fa){
	for(int i=0;i<=K;i++)
		f[x][i]=dp[x][i];
	if(fa){
		for(int i=0;i<=K;i++)
			tmp[i]=(f[fa][i]-dp[x][i]+mod-dp[x][i-1]+mod)%mod;
		tmp[0]=(f[fa][0]-dp[x][0]+mod)%mod;
		for(int i=1;i<=K;i++)
			f[x][i]=(f[x][i]+tmp[i]+tmp[i-1])%mod;
		f[x][0]=(f[x][0]+tmp[0])%mod;
	}
	for(int i=h[x];i;i=G[i].nxt){
		int u=G[i].to;
		if(u==fa)continue;
		redfs(u,x);
	}
}
int main(){
	freopen("civilization.in","r",stdin);
	freopen("civilization.out","w",stdout);
	scanf("%d%d%d",&n,&K,&L);
	Init();
	scanf("%d%d%d%d",&now,&A,&B,&Q);
	int tmp,x,y;
	for(int i=1;i<n;i++){
	    now=(now*A+B)%Q,tmp=i<L?i:L;
	    x=i-now%tmp,y=i+1;
	    Add(x,y);Add(y,x);
	}
	dfs(1,0);
	redfs(1,0);
	for(int i=1;i<=n;i++){
		LL res=0;
		for(int j=0;j<=K;j++)
			res=(res+1LL*str[K][j]*fac[j]*f[i][j])%mod;
		printf("%lld\n",res);
	}
	return 0;
}

```