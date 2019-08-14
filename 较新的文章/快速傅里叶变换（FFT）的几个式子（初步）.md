### 快速傅里叶变换（FFT）的几个式子



#### 单位根$\omega_n^k$=$(\cos \frac{k}{n}2\pi, \sin \frac{k}{n}2\pi)$

单位根就是将复平面上的一个单位圆等分为n个点。

![](https://images2017.cnblogs.com/blog/1129536/201712/1129536-20171212213656597-2074851247.png)



#### 单位根的性质

$\omega_{2n}^{2k} = \omega_{n}^{k}$

$\omega_{n}^{k + \frac{n}{2}} = -\omega_{n}^{k}$



#### 一个结论

把多项式$A(x)$的离散傅里叶变换结果作为另一个多项式$B(x)$的系数，取单位根的倒数即$\omega_{n}^{0}, \omega_{n}^{-1}, \omega_{n}^{-2}, ..., \omega_{n}^{-(n - 1)}$作为$x$代入$B(x)$，得到的每个数再除以$n$，得到的是$A(x)$的各项系数。



#### 几个推论

$A(x) = (a_0 + a_2x^2 + ... + a_{n - 2}x^{n - 2}) + (a_1x + a_3x^3 + ... + a_{n-1}x^{n-1})$

设：

$A_1(x) = a_0 + a_2x + ... + a_{n - 2}x^{\frac{n}{2} - 1}$

$A_2(x) = a_1 + a_3x + ... + a_{n - 1}x^{\frac{n}{2} - 1}$

则：

$A(x) = A_1(x^2) + xA_2(x^2)$



so：

当$k<\frac{n}{2}​$
$$
\begin{align*}
A(\omega_n^k) &= A_1(\omega_n^{2k}) + \omega_n^kA_2(\omega_n^{2k}) \\
&= A_1(\omega_{\frac{n}{2}}^{k}) + \omega_n^kA_2(\omega_{\frac{n}{2}}^{k})
\end{align*}
$$
那么：
$$
\begin{align*}
A(\omega_n^{k + \frac{n}{2}}) 
&= A_1(\omega_n^{2k + n}) + \omega_n^{k + \frac{n}{2}}A_2(\omega_n^{2k + n}) \\
&= A_1(\omega_{\frac{n}{2}}^{k} \times \omega_n^n) + \omega_n^{k + \frac{n}{2}} A_2(\omega_{\frac{n}{2}}^{k} \times \omega_n^n) \\
&= A_1(\omega_{\frac{n}{2}}^{k}) - \omega_n^kA_2(\omega_{\frac{n}{2}}^{k})
\end{align*}
$$



靠着几个式子和结论，我们就能构建出FFT的代码。



#### 板子

```cpp
struct Complex{
	double x,y;
	Complex(){}
	Complex(double _x,double _y):x(_x),y(_y){}
	Complex operator +(const Complex &A)const{
		return (Complex){x+A.x,y+A.y};
	} 
	Complex operator -(const Complex &A)const{
		return (Complex){x-A.x,y-A.y};
	}
	Complex operator *(const Complex &A)const{
		return (Complex){x*A.x-y*A.y,x*A.y+y*A.x};
	}
};
Complex A[M],B[M];
double pi=acos(-1.0);
void FFT(Complex *y,int n,int f){
	if(n==1)return;
	Complex L[n>>1],R[n>>1];
	for(int i=0;i<n;i+=2)L[i>>1]=y[i],R[i>>1]=y[i+1];
	FFT(L,n>>1,f);FFT(R,n>>1,f);
	Complex wn(cos(2*pi/n),f*sin(2*pi/n)),w(1,0);
	for(int i=0;i<(n>>1);i++,w=w*wn){
		y[i]=L[i]+w*R[i];
		y[i+(n>>1)]=L[i]-w*R[i];
	}
}
```

