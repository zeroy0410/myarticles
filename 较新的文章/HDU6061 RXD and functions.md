#### 题目

[题目链接](http://acm.hdu.edu.cn/showproblem.php?pid=6061)



#### 思路

显然可以发现，那些a其实和最终的结果没有关系，我们关心的仅仅是所有a的总和，设这个和为$a$。

也就是说$g(x)=f(x-a)$

通过二项式定理展开得：$\sum_{i=0}^{n}c_i \sum_{j=0}^{i} \binom{i}{j} (-a)^{i-j}x^{j}$

因为要求的是$x^j$的系数，所以整理一下，把$j$写在前面。
$$
\sum_{j=0}^{n}\sum_{i=j}^{n} c_i \frac{i!}{(j-i)!j!} (-a)^{i-j}x^{j}
$$
对于$j$而言，答案就是：
$$
\sum_{i=j}^{n} c_i \frac{i!}{(j-i)!j!} (-a)^{i-j}
$$
用$i$替换$i-j$:
$$
\sum_{i=0}^{n-j}\frac{C_{i+j}*(i+j)!}{i!j!}*(-a)^{i}
$$
然后令$A[i]=\frac{(-a)^{i}}{i!},B[i]=c_i*(i!)$

就化为了卷积的形式，可以用FFT或NTT求。

这是第一道NTT的题，所以讲一下NTT。

在FFT中，我们用的是单位复数根：$w_n=(cos(2*pi/n),sin(2*pi/n))$

我们用到了它的如下性质：

![](https://i.loli.net/2019/03/12/5c87aca01264c.png)



![](https://i.loli.net/2019/03/12/5c87ad1896987.png)



根据这几点，原根就等于$g^{\frac{(P-1)}{n}}$



#### 代码

```cpp
#include<bits/stdc++.h>
#define LL long long
#define M 400005 
using namespace std;
const int P=998244353;
LL rev[M],fac[M],Ifac[M];
LL qkpow(LL a,LL b){
	LL res=1;
	while(b){
		if(b&1)res=res*a%P;
		a=a*a%P;
		b>>=1;
	}
	return res;
}
void NTT(LL *A,int n,LL f){
	for(int i=0;i<n;i++)
		if(rev[i]<i)swap(A[i],A[rev[i]]);
	for(int m=1;m<n;m<<=1){
		LL wn=qkpow(3,(P-1)/(m<<1)*f%(P-1));
		for(int j=0;j<n;j+=m<<1){
			LL w=1;
			for(int k=0;k<m;k++){
				int u=A[j+k];
				int t=w*A[j+k+m]%P;
				A[j+k]=u+t;
				if(A[j+k]>=P)A[j+k]-=P;
				A[j+k+m]=u-t;
				if(A[j+k+m]<0)A[j+k+m]+=P; 
				w=w*wn%P;
			}
		}
	}
}
LL C[M],A[M],B[M],n,m,a;
int op[]={1,-1};
int main(){
//	freopen("data.in","r",stdin);
//	freopen("test.out","w",stdout);
	fac[0]=Ifac[0]=1;
	for(int i=1;i<M;i++){
		fac[i]=1LL*fac[i-1]*i%P; 
		Ifac[i]=Ifac[i-1]*qkpow(i,P-2)%P;
	}
	while(~scanf("%d",&n)){
		memset(A,0,sizeof(A));
		memset(B,0,sizeof(B));n++;
		for(int i=0;i<n;i++)scanf("%lld",&C[i]);
		scanf("%d",&m);a=0;LL x;
		for(int i=1;i<=m;i++)scanf("%lld",&x),a=(a+x)%P;
		LL tmp=A[0]=1;
        for(int i=1;i<n;i++){
            tmp*=a;
            tmp%=P;
            tmp=P-tmp;
            A[i]=tmp*Ifac[i]%P;
        }
		for(int i=0;i<n;i++)B[n-1-i]=1LL*C[i]*fac[i]%P;
		int nn=n,mm=n+n,c=0;
		for(nn=1;nn<mm;nn<<=1)c++;
		for(int i=0;i<nn;i++)rev[i]=(rev[i>>1]>>1)|((i&1)<<(c-1));
		NTT(A,nn,1);NTT(B,nn,1);
		for(int i=0;i<nn;i++)A[i]=1LL*A[i]*B[i]%P;
		NTT(A,nn,P-2);
		LL inv=qkpow(nn,P-2);
		for(int i=0;i<n;i++)
			printf("%d ",1LL*A[n-i-1]*inv%P*Ifac[i]%P);	
		printf("\n");
	}
	return 0;
}
```
参考资料：
[从多项式乘法到快速傅里叶变换 by Miskcoo](http://blog.miskcoo.com/2015/04/polynomial-multiplication-and-fast-fourier-transform#i-13)
