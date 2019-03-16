#### 题目

给出一个长度为n的01串S，有Q个操作：
1.翻转区间[l,r]（0变1,1变0）
2.求区间[l,r]有多少不同的子串



#### 思路

这是一道好题，首先考虑没有修改操作的dp状态，

则$dp[i][j]$表示到了第$i$个位置，**结尾**为$j$的串的方案数

则我们可以得到以下递推关系：
$$
dp[i][j]=dp[i-1][0]+dp[i-1][1]+1,j==S[i]\\
dp[i][!j]=dp[i-1][j]
$$
稍微解释一下：如果$j$与$S[i]$相等，则可以分为结尾为$0j$，$1j$和$j$的值

不等则继承上一个部分。



显然上面的式子可以用矩阵优化。

然后就是区间覆盖的问题。

对于这种问题，首先考虑一波线段树，只不过这里的线段树的每个节点维护的是矩阵，维护的信息是一段信息内矩阵的积罢了。

主要是以前认为矩阵的作用仅仅是矩阵快速幂，这题让我长见识了。



#### 代码

```cpp
#include<bits/stdc++.h>
#define M 100005
using namespace std;
const int mod=1e9+7;
//dp[i][j]=dp[i-1][0]+dp[i-1][1]+1;
//dp[i][!j]=dp[i-1][!j];
int T,n,q;
char S[M];
struct Matrix{
    int n,m,a[4][4];
    void clear(){memset(a,0,sizeof(a));}
    void resize(int _n,int _m){n=_n;m=_m;}
    Matrix operator *(const Matrix &_)const{
        Matrix res;res.resize(n,_.m);
        for(int i=1;i<=n;i++){
            for(int j=1;j<=_.m;j++){
                res.a[i][j]=0;
                for(int k=1;k<=m;k++){
                    res.a[i][j]+=1LL*a[i][k]*_.a[k][j]%mod;
                    if(res.a[i][j]>=mod)res.a[i][j]-=mod;    
                }
            }
        }
        return res;
    }
    void flip(){
    	swap(a[1][1],a[2][2]);
		swap(a[1][2],a[2][1]);
		swap(a[3][1],a[3][2]);	
    }
}OP[2];
struct YD_tree{
	struct node{
		int l,r,lzy;
		Matrix sum;
	}tree[M<<2];
	#define fa tree[p]
	#define lson tree[p<<1]
	#define rson tree[p<<1|1]
	void down(int p){
		if(fa.lzy==-1)return;
		lson.sum.flip();rson.sum.flip();
		lson.lzy=-lson.lzy;
		rson.lzy=-rson.lzy;
		fa.lzy=-1;
	}
	void build(int l,int r,int p){
		fa.l=l;fa.r=r;fa.lzy=-1;
		if(l==r){fa.sum=OP[S[l]-'0'];return;}
		int mid=(l+r)>>1;
		build(l,mid,p<<1);
		build(mid+1,r,p<<1|1);
		fa.sum=lson.sum*rson.sum;
	}
	void update(int l,int r,int p){
		if(fa.l==l&&fa.r==r){
			fa.sum.flip();
			fa.lzy=-fa.lzy;
			return;
		}
		down(p);
		int mid=(fa.l+fa.r)>>1;
		if(r<=mid)update(l,r,p<<1);
		else if(l>mid)update(l,r,p<<1|1);
		else {
			update(l,mid,p<<1);
			update(mid+1,r,p<<1|1);
		}
		fa.sum=lson.sum*rson.sum;	
	}
	Matrix query(int l,int r,int p){
		if(fa.l==l&&fa.r==r)return fa.sum;
		down(p);
		int mid=(fa.l+fa.r)>>1;
		if(r<=mid)return query(l,r,p<<1);
		else if(l>mid)return query(l,r,p<<1|1);
		return query(l,mid,p<<1)*query(mid+1,r,p<<1|1);	
	}
	#undef fa
	#undef lson
	#undef rson
}Tr;
int main(){
	scanf("%d",&T);
	OP[0]=(Matrix){
    3,3,
    0,0,0,0,
    0,1,0,0,
    0,1,1,0,
    0,1,0,1};
    OP[1]=(Matrix){
    3,3,
    0,0,0,0,
    0,1,1,0,
    0,0,1,0,
    0,0,1,1};
	Matrix ans;ans.clear();
	ans.resize(1,3);
	ans.a[1][3]=1;
	while(T--){
		scanf("%d%d",&n,&q);
		scanf("%s",S+1);
		Tr.build(1,n,1);
		for(int i=1,op,l,r;i<=q;i++){
			scanf("%d%d%d",&op,&l,&r);
			if(op==1)Tr.update(l,r,1);
			else {
				Matrix res=ans*Tr.query(l,r,1);
				printf("%d\n",(res.a[1][1]+res.a[1][2])%mod);
			}
		}
	}
	return 0;
}
```



