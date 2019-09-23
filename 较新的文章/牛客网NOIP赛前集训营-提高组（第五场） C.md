#### 题目

给定非负整数a,b,c,d，求有多少对01串(S,T)，满足以下条件：

 \- S 由 a 个 0，b 个 1 组成
 \- T 由 c 个 0，d 个 1 组成
 \- T 可以由 S 删掉一些字符得到

 由于答案可能过大，你只需要输出答案对$1000000007$取模后的值。



#### 思路

考虑先构造一个T，然后往其中补0，1，扩充到S。

这个构造的方案有$C_{c+d}^{c}$种，最后乘上即可。

一共要添加$a-c$个$0$，$b-d$个$1$，我们看看怎么统计不会算重和算漏。

设$e=a-c$，$f=b-d$。

首先，考虑一种朴素的会算重的情况：

假设T为`0000`，往其中添加`0`，会发现，无论加在哪一个位置，其最终的答案都是`00000`，考虑还有好多这种类似的情况，于是，为了避免这一点，我们规定:

1. 任何一个`0`只能放在末尾或者`1`的前面。
2. 任何一个`1`只能放在末尾或者`0`的前面。

这样就不会算重了。

然后对于一个既得序列，我们假设其$0$的个数为$cnt[0]$，$1$的个数为$cnt[1]$，那么往其中插$1$的方案数就是$cnt[0]+1$，$0$反之。

但是，这样还是会算重，因为插入结尾的01数字是有排列的顺序的，所以，直接把它们拎出来分类讨论就行了。



#### 代码

```c++
#include<stdio.h>
#include<algorithm>
#include<string.h>
#define LL long long
#define M 4005
using namespace std;
const int mod=1e9+7;
int a,b,c,d;
LL C[M][M],ans;
void Init(){
	C[0][0]=1;
	for(int i=1;i<M;i++){
		C[i][0]=1;
		for(int j=1;j<=i;j++)
			C[i][j]=(C[i-1][j]+C[i-1][j-1])%mod;
	}
}
LL calc(int x,int y){
    if(x==0)return (y==0);
    return C[x+y-1][x-1];
}
int main(){
	Init();
	scanf("%d%d%d%d",&a,&b,&c,&d);
	int e=a-c,f=b-d;
	for(int i=0;i<=e;i++)
		for(int j=0;j<=f;j++)
			ans=(ans+C[i+j][i]*calc(c,f-j)%mod*calc(d,e-i)%mod)%mod;
	ans=ans*C[c+d][c]%mod;
	printf("%lld\n",ans);
	return 0;
}
```

