这里主要讲一讲Splay的基础操作（插入，删除）。



#### 前言

Splay本质上是一棵二叉查找树。

![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/c0%3Dbaike80%2C5%2C5%2C80%2C26/sign=79dcefded70735fa85fd46ebff3864d6/8644ebf81a4c510f0b3dafdf6359252dd52aa57e.jpg)

这种树有许多美妙的性质：

若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值； 

若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值； 

它的左、右子树也分别为二叉查找树。



但是它也有缺点：可能退化成一条链。



所以Splay就是通过不断地对树中的点进行旋转和变换，让树的高度保持在logn。



#### 旋转

[yyb的描述](https://www.cnblogs.com/cjyyb/p/7499020.html)

```c++
void rotate(int x){//将x旋转到它的父节点的位置
	int y=fa[x],z=fa[y],k=(x==ch[y][1]);
	Down(y);Down(x);
	if(z)ch[z][y==ch[z][1]]=x;fa[x]=z;
	ch[y][k]=ch[x][!k];if(ch[x][!k])fa[ch[x][!k]]=y;
	ch[x][!k]=y;fa[y]=x;
	Up(y);Up(x);
}
```

这个多画图感受一下就好了。。。



#### Splay操作

这是为了防止Splay在不断旋转的过程中仍有一条链永远存在的问题，所以要防止这一点，yyb的博客中也有描述，这里不再重复造轮子。

```c++
void splay(int x,int w){
    while(fa[x]!=w){
        int y=fa[x],z=fa[y];
        if(z!=w)(x==ch[y][1])==(y==ch[z][1])?rotate(y):rotate(x);
        rotate(x);
    }
    if(!w)rt=x;
}

```

#### 插入，查找

这个都很简单，按照定义来就好了。

```c++
void Insert(int x){
    int u=rt,ff=0;
    while(u&&key[u]!=x){
        ff=u;
        u=ch[u][x>key[u]];
    }
    u=++tot;
    if(ff)ch[ff][x>key[u]]=u;
    ch[u][0]=ch[u][1]=0;
    fa[u]=ff;key[u]=x;
    splay(u,0);
}
int find(int x){
    int u=rt;
    while(ch[u][x>key[u]]&&key[u]!=x){
        Down(u);
        u=ch[u][x>key[u]];	
    }
    return u;
}

```

这样这个板子就讲完了

~~原谅我的懒惰~~







