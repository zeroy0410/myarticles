#### 题目

欧贝利斯克的巨神兵很喜欢有向图，有一天他找到了一张$n$个点$m$条边的有向图。

欧贝利斯克认为一个没有环的有向图是优美的，请问这张图有多少个子图（即选定一个边集）是优美的？

答案对$10^9+7$取模。



对于**40%**的数据$n≤5,m≤20$

对于**60%**的数据$n≤10$；

对于**80%**的数据$n≤15$；

对于**100%**的数据$n≤17$。

#### 思路

考场上只会写枚举边集的纯暴力，后来发现自己蠢爆了。

首先这题的关键在于如何设计出一个算法，把复杂度堆在点数上而不是边上，之后所做的工作都是在此基础之上进行优化。

看看在40分的暴力中，判环时用了拓扑排序，考虑以拓扑排序为基础对原图分层。

##### p60

定义$dp[i][j]$表示当前已经使用了哪些点，最后一层有哪些点，一层层的转移，每一层内部不能有边，层与层之间的边可选可不选。

```c++
struct P60{
	int dp[1<<10][1<<10];
	int cnt[1<<10];
	int Pow[15];
	void Print(int x){
		for(int i=0;i<n;i++)
			if(x&1<<i)printf("1");
			else printf("0");
		puts("");
	}
	void Init(){
		for(int i=1;i<1<<n;i++)cnt[i]=cnt[i>>1]+(i&1);
		for(int i=1;i<1<<n;i++)dp[i][i]=1;
		Pow[0]=1;
		for(int i=1;i<=n;i++)Pow[i]=1LL*Pow[i-1]*2%mod;
	}
	void solve(){
		Init();
		int base=(1<<n)-1;
		for(int i=1;i<base;i++){//diandeshiyongqingkuang
			for(int j=i;j;j=(j-1)&i){//dangqiancengyijingyonglezhemeduodian
				for(int k=(base^i);k;k=(k-1)&(base^i)){//qianyicengyaoyongzhexiedian
					LL tmp=1;
					for(int r=0;r<n;r++)
						if(k&1<<r){
							tmp=tmp*(Pow[cnt[j&st[r]]]-1)%mod;
							tmp=tmp*Pow[cnt[(i^j)&st[r]]]%mod;
						}
					Add(dp[i|k][k],tmp*dp[i][j]%mod);
				}
			}
		}
		int ans=0;
		for(int i=1;i<=base;i++)
			Add(ans,dp[base][i]);
		printf("%d\n",ans);
	}
}p60;
```

##### p100

进一步发现，第二个状态$j$可以省略，使用容斥的思想解决问题。

按照加入的点集来进行容斥。

```c++
struct P100{
	int dp[1<<17],tmp[1<<17],S[1<<17];
	int cnt[1<<17];
	int Pow[505];
	void Init(){
		cnt[0]=-1;
		for(int i=1;i<1<<n;i++)cnt[i]=-cnt[i&(i-1)];
		Pow[0]=1;
		for(int i=1;i<=n*n;i++)Pow[i]=1LL*Pow[i-1]*2%mod;
	}
	void solve(){
		Init();
		int base=(1<<n)-1;
		dp[0]=1;
		for(int i=0;i<base;i++){
			S[0]=0;
			for(int j=0;j<n;j++)tmp[1<<j]=0;
			for(int j=0;j<n;j++){if(i&1<<j){for(int k=0;k<n;k++){if(st[j]&1<<k)tmp[1<<k]++;}}}
			int to=(base^i);
			for(int T=(to&(to-1));;T=(T-1)&to){
				int ne=(T^to);
				S[ne]=S[ne&(ne-1)]+tmp[ne&(-ne)];
				Add(dp[i|ne],1LL*dp[i]*Pow[S[ne]]%mod*cnt[ne]%mod);
				if(!T)break;
			}
		}
		printf("%d\n",dp[base]);
	}
}p100;
```



#### 完整代码

```c++
#include<bits/stdc++.h>
#define LL long long
const int mod=1e9+7;
using namespace std;
int n,m;
int st[20],ed[20];
void Add(int &x,int y){
	x+=y;
	if(x>=mod)x-=mod;
	if(x<0)x+=mod;
}
struct P60{
	int dp[1<<10][1<<10];
	int cnt[1<<10];
	int Pow[15];
	void Print(int x){
		for(int i=0;i<n;i++)
			if(x&1<<i)printf("1");
			else printf("0");
		puts("");
	}
	void Init(){
		for(int i=1;i<1<<n;i++)cnt[i]=cnt[i>>1]+(i&1);
		for(int i=1;i<1<<n;i++)dp[i][i]=1;
		Pow[0]=1;
		for(int i=1;i<=n;i++)Pow[i]=1LL*Pow[i-1]*2%mod;
	}
	void solve(){
		Init();
		int base=(1<<n)-1;
		for(int i=1;i<base;i++){//diandeshiyongqingkuang
			for(int j=i;j;j=(j-1)&i){//dangqiancengyijingyonglezhemeduodian
				for(int k=(base^i);k;k=(k-1)&(base^i)){//qianyicengyaoyongzhexiedian
					LL tmp=1;
					for(int r=0;r<n;r++)
						if(k&1<<r){
							tmp=tmp*(Pow[cnt[j&st[r]]]-1)%mod;
							tmp=tmp*Pow[cnt[(i^j)&st[r]]]%mod;
						}
					Add(dp[i|k][k],tmp*dp[i][j]%mod);
				}
			}
		}
		int ans=0;
		for(int i=1;i<=base;i++)
			Add(ans,dp[base][i]);
		printf("%d\n",ans);
	}
}p60;
struct P100{
	int dp[1<<17],tmp[1<<17],S[1<<17];
	int cnt[1<<17];
	int Pow[505];
	void Init(){
		cnt[0]=-1;
		for(int i=1;i<1<<n;i++)cnt[i]=-cnt[i&(i-1)];
		Pow[0]=1;
		for(int i=1;i<=n*n;i++)Pow[i]=1LL*Pow[i-1]*2%mod;
	}
	void solve(){
		Init();
		int base=(1<<n)-1;
		dp[0]=1;
		for(int i=0;i<base;i++){
			S[0]=0;
			for(int j=0;j<n;j++)tmp[1<<j]=0;
			for(int j=0;j<n;j++){if(i&1<<j){for(int k=0;k<n;k++){if(st[j]&1<<k)tmp[1<<k]++;}}}
			int to=(base^i);
			for(int T=(to&(to-1));;T=(T-1)&to){
				int ne=(T^to);
				S[ne]=S[ne&(ne-1)]+tmp[ne&(-ne)];
				Add(dp[i|ne],1LL*dp[i]*Pow[S[ne]]%mod*cnt[ne]%mod);
				if(!T)break;
			}
		}
		printf("%d\n",dp[base]);
	}
}p100;
int main(){
	freopen("obelisk.in","r",stdin);
	freopen("obelisk.out","w",stdout);
	scanf("%d%d",&n,&m);
	for(int i=1,a,b;i<=m;i++){
		scanf("%d%d",&a,&b);a--;b--;
		st[a]|=1<<b;
	}
	p100.solve();
	return 0;
}
```

