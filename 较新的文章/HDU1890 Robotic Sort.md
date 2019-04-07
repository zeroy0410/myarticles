#### 题目

[题目](http://acm.hdu.edu.cn/showproblem.php?pid=1890)

大意是让你找在一个区间内找一个最小权值的点，让它通过旋转的方式换到最前面。



#### 思路

注意Splay的性质：中序遍历不变。

所以这里就可以把Splay看作一个**超级数组**，而不是通常定义的二叉查找树（左边的子树权值都小于它，右边的子树权值都大于它）。

然后它就可以通过打标记的方式实现区间的翻转操作。

具体方法是：

如果要对一段区间进行操作的话，只要：

```c++
int L=find(l-1),R=find(r+1);
splay(L,0);
splay(R,rt);
```

那么这个时候，$ch[R][0]$就是要操作区间了。

注意这里的find操作不是基于“key”的，而是基于"sz"的，它的含义是查找Splay中的中序遍历第k个的标号。

另外，数据范围有锅，输入数据可能会>n。



#### 代码

```c++
#include<cstdio>
#include<string.h>
#include<algorithm>
#define M 100005
using namespace std;
int stk[M],top;
int mp[M];
struct Splay{
	int fa[M],ch[M][2],rt,tot,val[M],sz[M];
	bool mark[M];
	void Up(int p){sz[p]=sz[ch[p][0]]+sz[ch[p][1]]+1;}
	void Down(int p){
		if(mark[p]){
			swap(ch[p][0],ch[p][1]);
			mark[ch[p][0]]^=1;
			mark[ch[p][1]]^=1;
			mark[p]=0;
		}
	}
	void build(int &x,int f,int *arr,int L,int R){
		if(L>R)return;
		int mid=(L+R)>>1;
		x=++tot;
		ch[x][0]=ch[x][1]=0;
		fa[x]=f;
		val[x]=arr[mid];
		mp[arr[mid]]=x;
		sz[x]=1;mark[x]=0;
		build(ch[x][0],x,arr,L,mid-1);
		build(ch[x][1],x,arr,mid+1,R);
		Up(x);
	}
	void clear(){rt=tot=0;}
	void rotate(int x){
		int y=fa[x],z=fa[y],k=x==ch[y][1];
		Down(y);Down(x);
		if(z)ch[z][y==ch[z][1]]=x;fa[x]=z;
		ch[y][k]=ch[x][!k];if(ch[x][!k])fa[ch[x][!k]]=y;
		ch[x][!k]=y,fa[y]=x;
		Up(y);Up(x);
	}
	void splay(int x,int w){
		for(int y=x;;y=fa[y]){
			stk[++top]=y;
			if(y==w)break;
		}while(top)Down(stk[top--]);
		while(fa[x]!=w){
			int y=fa[x],z=fa[y];
			if(z!=w)(y==ch[z][1]&&x==ch[y][1])?rotate(y):rotate(x);
			rotate(x);
		}
		if(!w)rt=x;
	}
	int find(int x){//找到第x大的元素
		x++;
		int u=rt;
		while(1){
			Down(u);
			if(sz[ch[u][0]]+1==x)return u;
			else if(sz[ch[u][0]]>=x)u=ch[u][0];
			else x-=sz[ch[u][0]]+1,u=ch[u][1];
		}
		return u;
	}
	int query(int x){
		splay(mp[x],0);
		return sz[ch[mp[x]][0]];
	}
	void reserve(int l,int r){
		int L=find(l-1),R=find(r+1);
		splay(L,0);
		splay(R,rt);
		mark[ch[R][0]]^=1;
	}
}Tr;
int n;
int A[M],B[M];
int cnt[M];
int main(){
	 // freopen("data.in","r",stdin);
	 // freopen("data.out","w",stdout);
	while(scanf("%d",&n)&&n){
		Tr.clear();
		memset(cnt,0,sizeof(cnt));
		for(int i=1;i<=n;i++)
			scanf("%d",&A[i]),B[i]=A[i];
		sort(B+1,B+n+1);
		for(int i=1;i<=n;i++){
			A[i]=lower_bound(B+1,B+n+1,A[i])-B;	
			cnt[A[i]]++;
			A[i]+=cnt[A[i]]-1;
		}
		A[0]=0;A[n+1]=0;
		Tr.build(Tr.rt,0,A,0,n+1);
		for(int i=1;i<=n;i++){
			int u=Tr.query(i);
	  		printf("%d%c",u," \n"[i==n]);
			Tr.reserve(i,u);
		}
	}		
	return 0;
}
```

