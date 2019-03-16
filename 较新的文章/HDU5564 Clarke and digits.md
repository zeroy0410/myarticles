#### 前置知识

$dp[i][j][k]$表示$i$长，$mod7=j$，这个位置选了$k$的方案数。

$dp[i+1][(j*10+x)mod7][x]+=dp[i][j][y];//x+y!=K​$

$Ma.a[i][j]=1$表示$i$状态->$j$状态可以转移

如果设$ans=qkpow(Ma,K)$。

则$ans.a[i][j]$表示走$K$步之后状态$i$到状态$j$的方案数。

如果对这个不熟悉可以看看[HDU5607](http://acm.hdu.edu.cn/showproblem.php?pid=5607)



#### 题解

然后对于本题而言，就要构建转移的矩阵。

注意到对于后两维而言，其实他们的转移都是一样的，与$i$没有关系，而且其状态只有7*10，所以可以构建$70·70$的转移矩阵。

初始矩阵就是第一位随便填哪个数字都无所谓，所以

```cpp
for(int i=1;i<10;i++)
	s.a[0][st(i%7,i)]=1;
```



接下来还有一个问题：如何统计前缀和？

我们要求的不仅仅是$ans(R)，ans(L)$而是$\sum_{i=L}^Rans(i)$

所以，这里有一个巧妙的操作：

```cpp
for(int i=0;i<10;i++)
    Ma.a[i][70]=1;
Ma.a[70][70]=1;
```

这样就可以保证我们最后要求的$ans=\sum_{i=0}^9a[0][i]$每一层都会被加到70这个点上，所以最后$a[0][70]$就是答案。



#### 代码

```cpp
#include<bits/stdc++.h>
#define M 105
using namespace std;
const int mod=1e9+7;
int l,r,K,T;
struct Matrix{
    int n,m,a[M][M];
    void clear(){memset(a,0,sizeof(a));}
    void resize(int _n,int _m){n=_n;m=_m;}
    Matrix operator *(const Matrix &_)const{
        Matrix res;res.resize(n,_.m);
        for(int i=0;i<=n;i++){
            for(int j=0;j<=_.m;j++){
                res.a[i][j]=0;
                for(int k=0;k<=m;k++){
                    res.a[i][j]+=1LL*a[i][k]*_.a[k][j]%mod;
                    if(res.a[i][j]>=mod)res.a[i][j]-=mod;    
                }
            }
        }
        return res;
    }
}Ma,s;
Matrix qkpow(Matrix a,int b){
    Matrix res;res.clear();res.resize(a.n,a.n);
    for(int i=0;i<=a.n;i++)res.a[i][i]=1;
    while(b){if(b&1)res=res*a;a=a*a;b>>=1;}
    return res;
}
int st(int i,int j){return i*10+j;}
int main(){
	cin>>T;
	s.clear();s.resize(70,70);
	for(int i=1;i<10;i++)
		s.a[0][st(i%7,i)]=1;
	while(T--){
		scanf("%d%d%d",&l,&r,&K);
		Ma.clear();Ma.resize(70,70);
		for(int y=0;y<10;y++){
			for(int x=0;x<10;x++){
				if(x+y==K)continue;
				for(int j=0;j<7;j++){
					Ma.a[st(j,y)][st((j*10+x)%7,x)]=1;
				}
			}
		}
		for(int i=0;i<10;i++)
			Ma.a[i][70]=1;
		Ma.a[70][70]=1;
		Matrix R=s*qkpow(Ma,r);
		Matrix L=s*qkpow(Ma,l-1);
		printf("%d\n",(R.a[0][70]-L.a[0][70]+mod)%mod);
	}
	return 0;
}
```

