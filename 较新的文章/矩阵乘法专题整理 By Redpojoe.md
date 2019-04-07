## 高斯消元 & 矩阵乘法 专题整理

### 矩阵乘法

#### 定义

一个$n*p$的矩阵，可以乘上一个$p*m$的矩阵，得到$n*m$的矩阵。设三个矩阵分别为$a$、$b$、$c$，$a\times b=c$，则
$$
c[x][y]=\sum_{i=1}^{p}a[x][i]\cdot b[i][y]
$$

#### 代码实现

```cpp
//h,w表示h*w的矩阵，A代表每个元素的值
Matrix operator *(const Matrix &b)const {
		static Matrix ret;
		ret.h=h,ret.w=b.w;
		for(int i=1; i<=h; i++)
			for(int j=1; j<=b.w; j++) {
				ret.A[i][j]=0;
				for(int q=1; q<=w; q++)
					ret.A[i][j]=(ret.A[i][j]+A[i][q]*b.A[q][j])%P;//%P意义下
			}
		return ret;
	}
```

#### 复杂度

$O(n*p*m)$。一般常见的情况下，计入复杂度的主要运算是两个$n*n$的矩阵相乘，得到一个新的$n*n$的矩阵。复杂度$O(n^3)$。

#### 运算律

矩阵乘法满足分配率，即$(a+b)\times c = a\times c+b\times c​$。（矩阵的加法运算就是两个大小相同的矩阵每一位都加起来）。

矩阵乘法满足结合律，即$(a\times b)\times c=a\times(b\times c)$。

矩阵乘法不一定满足交换律，即可能$a\times b \not= b\times a$。

#### 单位矩阵

长宽相等，且每个$i$行的第$i$列都为1的矩阵，即
$$
\left\{ \begin{matrix} 1 & 0 & \cdots & 0 & 0 \\ 0 & 1 & \cdots & 0 & 0 \\ \vdots & \vdots & \ddots & \vdots & \vdots \\0 & 0 & \cdots & 1 & 0 \\ 0 & 0 & \cdots & 0 & 1   \end{matrix}\right\}
$$


它有一个重要的性质：$n*n$的单位矩阵$m$满足$m\times a = a$，其中a也是一个$n*n$的矩阵。

#### 矩阵快速幂

由于矩阵满足结合律，所以可以用快速幂计算矩阵$a$的$p$次方，即$a^p$。另外，一般快速幂中初始化答案为1的地方，将答案初始化为同大小的单位矩阵。

```cpp
Matrix Pow(Matrix a,int p) {
	static Matrix ret;
	for(ret=Mut; p; p>>=1,a=a*a)if(p&1)ret=ret*a;//Mut是单位矩阵
	return ret;
}
```

#### 向量乘矩阵的意义

这里的向量指$1*n$的矩阵，即不定长数组。

设$v,r$为$1*n$矩阵，$a$为$n*n$矩阵，那么$r=v\times a$表示对于任意不大于$n$的正整数对$i,j$，$v_i$都以$a_{i,j}$的倍率累加到$r_j$上。

#### 矩阵加速递推

很多递推问题，都可以写成如下形式：

定义一个状态，它包含$n$个量，从状态$a$转移到状态$b$，可以看做状态$b$中的每一个元素都是$a$中的各个元素乘上各自特定的倍数累和得到的。例如斐波那契数列问题，一个状态包括两个数$(f_{i-1},f_i)$，转移可以写成$(f_{i-1},f_i)=(0\cdot f_{i-2}+1\cdot f_{i-1},1\cdot f_{i-2}+1\cdot f_{i-1})$。这样，就可以利用向量乘矩阵的意义，写出一个转移矩阵（如，$\bigl\{ \begin{smallmatrix} 0 & 1 \\ 1 & 1 \end{smallmatrix} \bigr\}$），然后利用矩阵快速幂的方法，在轮数$k$极大的情况下，快速求出递推$k$轮的结果（复杂度从$O(n^2k)$变到$O(n^3\log k)$）。

##### 例题1：[广义斐波那契数列](https://cn.vjudge.net/contest/276406#problem/F)

广义的斐波那契数列，形式如下：
$$
f_n=\sum_{i=1}^da_if_{n-i}
$$
其中$d$和长度为$d$的数列$a$给定。

容易写出$1*d$的状态矩阵$F_n$，其中$F_{n,i}$表示$f_{n-d+i}$。另有$d*d$的转移矩阵$U$，
$$
U=
 \left\{\begin{matrix}
  0 & 0 & 0 & 0 & \cdots & 0 & a_d\\
  1 & 0 & 0 & 0 & \cdots & 0 & a_{d-1}\\
  0 & 1 & 0 & 0 & \cdots & 0 & a_{d-2}\\
  \vdots & \vdots & \vdots & \vdots & \ddots & \vdots & \vdots\\
  0 & 0 & 0 & 0 & \cdots & 0 & a_2 \\
  0 & 0 & 0 & 0 & \cdots & 1 & a_1
  \end{matrix}\right\}
$$
由于$d$的范围不大，可以矩阵快速幂解决。核心代码如下：

```cpp
//Start（下面代码某些的Detire同理）是初始状态，Mut是单位矩阵，Unit是转移矩阵
void Make_Matrix() {
	Mut.h=Mut.w=d;
	for(int i=1; i<=d; i++)Mut.A[i][i]=1;
	Unit.h=Unit.w=d;
	memset(Unit.A,0,sizeof(Unit.A));
	for(int i=1; i<d; i++)Unit.A[i+1][i]=1;
	for(int i=1; i<=d; i++)scanf("%lld",&Unit.A[d-i+1][d]);
	Start.h=1,Start.w=d;
	for(int i=1; i<=d; i++)scanf("%lld",&Start.A[1][i]);
}

int main() {
	while(true) {
		scanf("%d%d%d",&d,&n,&m);
		if(n==0 && m==0 && d==0)break;
		Make_Matrix();
		Unit=Pow(Unit,n-1);
		printf("%lld\n",(Start*Unit).A[1][1]);
	}
	return 0;
}
```

##### 例题2：[有向图概率问题](https://cn.vjudge.net/contest/276406#problem/I)

大同小异。只不过这里的状态矩阵更好构造了，是：
$$
U_{v,u}=\begin{cases}0 &  ,uv之间没有连边 \\ deg_u^{-1} & ,uv之间有连边\end{cases}
$$
当然是在模$10^9+7$意义下的，需要求逆元。$deg_u$表示u的出度。代码略。

##### 例题3：[特殊性质优化矩阵乘法](https://cn.vjudge.net/contest/276406#problem/G)

经典的矩阵加速题。很容易写出转移矩阵，这里略。

需要注意的是，如果使用普通的矩阵乘法快速幂，那么复杂度很高，是$O(n^3\log k)\approx28\times 500^3=2875000000$，T飞在向我们招手。所以我们需要优化复杂度，而优化复杂度，需要我们抓住性质，优化矩阵乘法。

可以发现，这题的转移矩阵非常有特点，就是第$i$行($i>1$)是第$i-1$行向右平移一位得到的。所以每次计算只需要算出第一行，接下来每一行都从上一行copy即可。计算第一行复杂度$O(n^2)$，计算下面行复杂度$O(n(n-1))=O(n^2)$，总复杂度$O(n^2\log k)$。可以AC。

核心的矩阵乘法代码：

```cpp
Matrix operator *(const Matrix &b)const {
		static Matrix ret;
		ret.h=h,ret.w=b.w;
		for(int i=1; i<=h; i++) {
			if(i==1) {
				for(int j=1; j<=b.w; j++) {
					ret.A[i][j]=0;
					for(int q=1; q<=w; q++)
                        ret.A[i][j]=(ret.A[i][j]+A[i][q]*b.A[q][j])%m;
				}
			} else {
				for(int j=2; j<=b.w; j++)ret.A[i][j]=ret.A[i-1][j-1];
				ret.A[i][1]=ret.A[i-1][b.w];
			}
		}
		return ret;
	}
```

##### 例题4：[额外状态位应用](https://cn.vjudge.net/contest/276406#problem/K)

这道题，一道很正常的数位dp，由于只对位数限制，所以可以转变为普通dp。只是位数太多了，需要矩阵加速。

但是我们遇到两个问题：

1. 可能会遇到前导0，对状态转移造成额外的干扰。
2. 要求的不是位数限定为某一个值的方案数，而是位数不大于某一个值的方案数。

这两个问题，我们可以在定义状态矩阵的时候，加入额外状态来完成。

首先，基础的状态矩阵中有元素70个，分别表示模7的余数为0~7，上一位为0~9的70种状态。为了解决前导0问题，我们可以再加上一个元素，变成71个元素，多出来的一个表示前导0的状态。

> 题外话：事实上，这个状态永远只有1。可以认为，无论在dp哪一位，都可以“新建”一个数，即从前导0转移来。这个前导0状态就是一个“源”。
>
> 在某些矩阵题目中（比如我课外看到的一道），转移需要在做变量加法和的同时加上某一个常数，这个时候就在可以状态矩阵中多定义一个元素（“源”），它的值永远是1。需要加常数的时候，就从这个1中，拿"k倍的1"累加到新的状态里，并且这个1向自己转移1，保证自己不消失。可惜这个专题里没有类似的题。

另外，状态矩阵中还需要维护一个值，表示“递推的结果”，即答案。由于前导0能从任意位数转移到一般状态上，所以解决了“不大于”的问题。
$$
U=\left\{\begin{matrix}
  (70中基本状态之间的转移) & 0 & (累加答案，合法为1，不合法为0)\\
  (从前导0状态转移，有的是1，有的是0) & 1 & 0\\
  0 & 0 & 0
  \end{matrix}\right\}
$$
其中的第一行概括了70行，第一列概括了70列，这实际上是一个$72*72$的矩阵。

下面，给出构造转移矩阵的代码：

```cpp
inline int Id(int mod7,int lst) {
	return mod7*10+lst+1;
}

Unit.h=Unit.w=72;//1~70 普通状态 71 前导零 72 答案记录位
for(int i=0; i<7; i++)
		for(int j=0; j<10; j++)
			for(int q=0; q<10; q++)
				if(j+q != k) {
					int r=(i*10+q)%7;
					int Ida=Id(i,j),Idb=Id(r,q);
					Unit.A[Ida][Idb]=1;
				}
	for(int i=1; i<10; i++)
		Unit.A[71][Id(i%7,i)]=1;
	Unit.A[71][71]=1;
	for(int i=0; i<10; i++)Unit.A[Id(0,i)][72]=1;
```

##### 例题5：[减少矩阵的规模](https://cn.vjudge.net/contest/276406#problem/N)

现在，我们考虑最原始的矩阵加速算法。

我们可以用一个8位二进制数描述某一层，其中，0表示这一层的这一个位置上是没有东西的，1表示这个位置是有东西的。具体的，“有东西”表示在进行决策的时候，这个地方已经被下面层摆放了竖着的砖块，所以不能再放东西。（我们约定，按照下图进行编号）。
$$
\begin{matrix}0&1&2\\7&&3\\6&5&4\end{matrix}
$$
我们可以发现，总共有3种转移的方法：

1. 这一位原本有东西，无需摆放，但由于增高了一层，新的一层没有东西。即：1到0。
2. 这一位原本没有东西，放置一个竖的砖块，使新的一层有东西。即：0到1。
3. 相邻两位（包括0和7相接）原本有东西，放置一个横的砖块，保持有东西的状态。即：1,1到1,1。

可以看做：每向上转移一层，合法的方案可以看做：将状态取反，然后取若干个不重合的相邻1，再反回来。

对于每种状态进行一次bfs，可以得到所有它通过合法转移方案转移一层得到的新状态。所以可以预处理出一个$256*256$的转移矩阵，然后用矩阵快速幂加速递推。

这个算法的复杂度是$O(256^3T\log k)=O(16,777,216T\log k)\approx3,858,759,680$（取$\log k=23$），T飞在向我们招手。所以，优化势在必行。

说到优化，最重要的一点就是减少矩阵的规模。

首先，我们发现一个性质：无论在那一层，由于总砖块所占的格子数是偶数，一层的状态中1的个数也是偶数。这样，矩阵的规模就减小到了128，复杂度变为$O(2,097,152T\log k)\approx 482,344,960$。理论上，由于时限20s，卡一卡说不定真能过去，不过我们最好还是不要止步于此。

进一步，我们发现这个图形不仅是个轴对称图形，还是个中心对称图形，很多状态实质上是一样的，例如：
$$
\begin{matrix}1&1&0\\0&&0\\0&1&1\end{matrix}=\begin{matrix}0&0&1\\1&&1\\1&0&0\end{matrix}=\begin{matrix}0&1&1\\0&&0\\1&1&0\end{matrix}
$$
可以发现经过翻转和旋转，一个状态可能会得到最多8种不同的“扩展状态”。由于8种“扩展状态”本质上没有差别，可以选出一种“代表状态”，代表这8种“扩展状态”。

其实，除了轴对称、中心对称以外，还有一种更一般的解释方法：由于题目的转移，本质上是在01串上将相邻的两位进行操作，那么无论怎么改变这些相邻为的绝对位置，只要相对位置不变，就足以代表相同类型的状态（不知道是巧合还是什么，最后状态中不考虑旋转也会得到同样的结果，所以下面的代码就没有放旋转，加上也很好写）。

我们可以求出每个状态的8种“扩展状态”，然后用8种“扩展状态”种最小的一种作为该状态的“代表状态”。

一种获取“代表状态”的代码：

```cpp
int Getexpr(int p) {
	int ans=p;
	for(int i=1; i<=7; i++) {
		p=(p>>1)|((p&1)<<7);
		ans=min(ans,p);
	}
	return ans;
}
```

我们可以证明，两种“代表状态”相同的状态，转移得到的状态的“代表状态”也相同。然后，只需考虑所有的“代表状态”（即“代表状态”等于自己的状态），求出转移方法即可。

经过代码验证，0到255中，含有偶数个1的不同的“代表状态”只有18种。所以我们的矩阵规模可以减小到$18*18$，复杂度$O(18^3T\log k)\approx 1,341,360 $，这已经很小了。

此外，还有一种优化方法：我们不妨定义图中0、2、4、6号为“角块”，1、3、5、7号为“边块”，任意一种合法状态中“角块”里1的数量都等于”边块“。如果观察转移过程不难发现，每种转移方法都可以归结为将角块和边块上增加/减少相同数目的1。经代码验证，只有11种这样的状态。不信的话，这里给出11种“代表状态”的状压表示：

```
00000000
11000000
10010000
11110000
11011000
11100100
10110100
11001100
11111100
11110110
11111111
```

所以可以做到复杂度$O(11^3T\log k)\approx306130$。（这份代码在我写到这里的时候是杭电OJ全网第一，15ms）

另外，初始状态、结束状态的值都为00000000，其”代表状态“也是00000000。此外，构建矩阵时要考虑到从00000000到11111111（“代表状态”为11111111）有两种方案。

核心代码：

```cpp
const int P=1000000007;
const int M=256;

int Getexpr(int p) {
	int ans=p;
	for(int i=1; i<=7; i++) {
		p=(p>>1)|((p&1)<<7);
		ans=min(ans,p);
	}
	return ans;
}

int Bincnt0(int p) {//与Bincnt1分别获取边块、角块上1个数
	int ret=0;
	for(int i=1; i<=4; i++) {
		if(p&1)ret++;
		p>>=2;
	}
	return ret;
}

int Bincnt1(int p) {
	int ret=0;
	p>>=1;
	for(int i=1; i<=4; i++) {
		if(p&1)ret++;
		p>>=2;
	}
	return ret;
}

int Id[M];
int Shad[M],tot;

int Unf[10];
bool vis[M];
void Init() {
	for(int i=0; i<M; i++) {
		if(Bincnt0(i) != Bincnt1(i))continue;
		int expr=Getexpr(i);
		if(expr==i)Shad[expr]=++tot;
		Id[i]=Shad[expr];
	}
	for(int i=1; i<=8; i++)Unf[i]=(1<<(i-1))|(1<<(i&7));
}

struct Matrix {
	int h,w;
	long long A[15][15];
	Matrix operator *(const Matrix &b)const {
		...
	}
} Unit,Mut,Pw,Detire;

int Q[M],L,R;
void Make_Matrix() {
	Mut.h=Mut.w=tot;
	for(int i=1; i<=tot; i++)Mut.A[i][i]=1;
	Unit.h=Unit.w=tot;
	Unit.A[1][1]=1;
	for(int i=0; i<M; i++) {
		if(Getexpr(i) != i || Bincnt0(i) != Bincnt1(i))continue;
		L=R=0;
		memset(vis,0,sizeof(vis));
		Q[R++]=0;//暴力广搜获取转移矩阵
		while(L<R) {
			int x=Q[L++];
			Unit.A[Id[i]][Id[255^i^x]]++;
			for(int d=1; d<=8; d++)
				if((i&Unf[d])==0 && (x&Unf[d])==0) {
					int t=x|Unf[d];
					if(!vis[t])vis[t]=true,Q[R++]=t;
				}
		}
	}
}

Matrix Pow(Matrix a,int p) {
	...
}

long long Solve(int n) {
	Detire.h=1,Detire.w=tot;
	for(int i=2; i<=tot; i++)Detire.A[1][i]=0;
	Detire.A[1][1]=1;
	Pw=Pow(Unit,n);
	Detire=Detire*Pw;
	return Detire.A[1][1];
}

int main() {
	Init();
	Make_Matrix();
	int T;
	scanf("%d",&T);
	for(int kase=1; kase<=T; kase++) {
		int n;
		scanf("%d",&n);
		printf("Case %d: %lld\n",kase,Solve(n));
	}
	return 0;
}
```

#### 线段树维护矩阵

在某些题目中，为了求在一段区间上的某些计数问题（或者要求实现修改操作），需要用线段树维护矩阵。具体的，这种问题一般都可以写出一个暴力算法，在一个区间上直接递推求解；另外，这个递推式可以写成矩阵的形式。这时就可以拿线段树维护矩阵，直接查询区间的矩阵乘积，或者实现修改操作。

~~（据说黑科技动态dp也是用了这个原理？）~~

##### [例题](https://cn.vjudge.net/contest/276406#problem/M)

这道题的直接递推方式没有那么明显，也需要一些思考。考虑一个01序列形成新01子序列的过程：

初始，该串为空，没有01子序列（空序列不计，下同）。

当加入一个元素时，会形成一些新的01子序列。但是某些01子序列会造成重复，所以还需要排除掉一些重复计数。现在，我们考察在尾部加入一个0，会对不同的01子序列数量造成的影响。

我们不妨把之前的已经形成的01子序列分成两类，在第一类末尾加0会形成已经产生过的01子序列，在第二类末尾加0不会。可以发现，第一类01子序列一定能在上一个0之前找到（特别的，如果之前没有0，那么有-1个这样的子序列，具体理由到下面再说），而在上一个0之前能找到的01子序列也一定是第一类01子序列。所以，新的01子序列的个数，等于上一个0之后（包括上一个0）形成的新的01子序列，等于目前不同的01子序列个数，减去在上一个0之前不同的01子序列个数。

在尾部加入一个1同理。

所以，我们可以这样表示一个状态：
$$
\left[\begin{matrix}c_0&c_1&c_t\end{matrix}\right]
$$
其中，$c_0,c_1$分别表示上一个0/1之前形成的不同01子序列个数。在尾部加入0可以看做：
$$
\left[\begin{matrix}c_0&c_1&c_t\end{matrix}\right]\to \left[\begin{matrix}c_t&c_1&c_t+(c_t-c_0)\end{matrix}\right]
$$
在尾部加入1可看做：
$$
\left[\begin{matrix}c_0&c_1&c_t\end{matrix}\right]\to \left[\begin{matrix}c_0&c_t&c_t+(c_t-c_1)\end{matrix}\right]
$$
所以我们得到0和1分别对应的转移矩阵：


当然，在初始时，为了得到01子序列“0”或“1”，要设置初始状态为：
$$
D=\left[\begin{matrix}-1&-1&0\end{matrix}\right]
$$
这样才能通过初始递推得到1。（事实上，可以看做初始状态为$\bigl[\begin{matrix}0&0&1\end{matrix}\bigr]$，其中的1是空序列，最后再把1减去。所以可以先把1减去）

题目还要求支持翻转操作。观察翻转操作，等价于把所有的0变成1，所有的1变成0，即把0和1的含义倒转。所以交换对应矩阵的第一行和第二行，再交换对应矩阵的第一列和第二列即可。可以表示如下：
$$
\left\{\begin{matrix}A&B&C\\D&E&F\\G&H&I\end{matrix}\right\}\to\left\{\begin{matrix}E&D&F\\B&A&C\\H&G&I\end{matrix}\right\}
$$
所以可以用如下代码表示：

```cpp
swap(A[1][1],A[2][2]);
swap(A[1][2],A[2][1]);
swap(A[1][3],A[2][3]);
swap(A[3][1],A[3][2]);
```

大概到此，这道题的大致框架就出来了。代码如下：

```cpp
#include<bits/stdc++.h>
using namespace std;
const int M=1e5+5;
const int P=1e9+7;

int n,m;
char str[M];

struct Matrix {
	int h,w;
	long long A[4][4];
	void Be_0() {//变为0转移矩阵
		h=w=3;
		A[1][1]=0,A[1][2]=0,A[1][3]=P-1;
		A[2][1]=0,A[2][2]=1,A[2][3]=0;
		A[3][1]=1,A[3][2]=0,A[3][3]=2;
	}
	void Be_1() {//变为1转移矩阵
		h=w=3;
		A[1][1]=1,A[1][2]=0,A[1][3]=0;
		A[2][1]=0,A[2][2]=0,A[2][3]=P-1;
		A[3][1]=0,A[3][2]=1,A[3][3]=2;
	}
	void Be_Detire() {//变为初始矩阵
		h=1,w=3;
		A[1][1]=P-1,A[1][2]=P-1,A[1][3]=0;
	}
	void Flip() {//翻转
		swap(A[1][1],A[2][2]);
		swap(A[1][2],A[2][1]);
		swap(A[1][3],A[2][3]);
		swap(A[3][1],A[3][2]);
	}
	Matrix operator *(const Matrix &b)const {
		...
	}
} Det,Mul;

struct Seg_Tree {
	struct node {
		int L,R;
		Matrix Mat;
		bool Lazy;//由于有区间修改操作，需要Lazy Tag
	} T[M<<2];
#define tp T[p]
#define lp T[p<<1]
#define rp T[p<<1|1]
#define ls (p<<1)
#define rs (p<<1|1)
	void Up(int p) {
		tp.Mat=lp.Mat*rp.Mat;
	}
	void Down(int p) {
		if(!tp.Lazy)return;
		lp.Lazy^=1,lp.Mat.Flip();
		rp.Lazy^=1,rp.Mat.Flip();
		tp.Lazy=false;
	}
	void Build(int L,int R,int p) {
		tp.L=L,tp.R=R,tp.Lazy=false;
		if(L==R) {
			if(str[L]=='0')tp.Mat.Be_0();
			else tp.Mat.Be_1();
			return;
		}
		int mid=(L+R)>>1;
		Build(L,mid,ls),Build(mid+1,R,rs),Up(p);
	}
	void Update(int L,int R,int p) {
		if(tp.L == L && tp.R == R) {
			tp.Mat.Flip();
			tp.Lazy^=1;
			return;
		}
		Down(p);
		int mid=(tp.L+tp.R)>>1;
		if(R <= mid)Update(L,R,ls);
		else if(L > mid)Update(L,R,rs);
		else Update(L,mid,ls),Update(mid+1,R,rs);
		Up(p);
	}
	Matrix Query(int L,int R,int p) {
		if(tp.L == L && tp.R == R)return tp.Mat;
		Down(p);
		int mid=(tp.L+tp.R)>>1;
		if(R <= mid)return Query(L,R,ls);
		if(L > mid)return Query(L,R,rs);
		return Query(L,mid,ls)*Query(mid+1,R,rs);//不满足交换律，注意顺序
	}
#undef tp
#undef lp
#undef rp
#undef ls
#undef rs
} YD;

void Solve() {
	YD.Build(1,n,1);
	while(m--) {
		int ty,L,R;
		scanf("%d%d%d",&ty,&L,&R);
		if(ty == 1)YD.Update(L,R,1);
		else {
			Det.Be_Detire();
			Mul=YD.Query(L,R,1);
			Det=Det*Mul;
			printf("%lld\n",Det.A[1][3]);
		}
	}
}

int main() {
	int T;
	scanf("%d",&T);
	while(T--) {
		scanf("%d%d%s",&n,&m,str+1);
		Solve();
	}
	return 0;
}
```

