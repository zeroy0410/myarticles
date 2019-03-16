#### 概述

KMP算法能够解决字符串匹配问题。即S串在P串中出现了多少次的问题，时间复杂度为$O(n+m)$
设S处的指针为j，P处的指针为i，我们的目的是让P[i-j+1..i]与S[1..j]完全相等。
那么如果使用传统的方法，一旦匹配失败，就需要把i往后移一位，再重新匹配，时间复杂度是$O(n*m)$的很不划算。

#### 尝试优化

很显然，每一次匹配失败获得的信息在上面的朴素算法中没有得到很好的应用。
举个例子：
S="ababacb "
T="abababaabab"
当匹配到第六个字母时，我们发现不匹配了，但同时，这也说明[0,4]这个区间内是匹配的。按照常规的思路，我们应该将i往后移一位再重新匹配。

那么能不能最大限度的利用此时已经得到的j？（此时的j代表S[0..j]与P[i-j+1,i]完全一致了）

KMP的策略是：**在i移动的同时减少j，让j尽量大**

那么新的j'就要满足在S中前j'个字母与后j'个字母是一样的。
于是就可以这么移动：
ababacb
  ababacb
之后再继续按照上述流程匹配。

此时就需要一个数组nxt[x],记录S的一个前缀，表示在[0,x-1]这个范围内，j->j'的映射。

整体的代码雏形也不难写出了：
```cpp
bool KMP(char *S,char *P){
    int n=strlen(P+1),m=strlen(S+1);
	for(int i=1,j=1;i<=n;i++){
		while(j>1&&S[j]!=P[i])j=nxt[j];
		if(S[j]==P[i])j++;
		if(j==m+1)return 1;
	}
	return 0;
}
```

#### 考虑预处理

接下来便是nxt数组的预处理问题。
实际上与上面的代码非常类似（相当于自己跑自己的KMP）

同样是对于上面的S，假设我们已知nxt[0],nxt[1],nxt[2],nxt[3],那么要怎么求出nxt[4]和nxt[5]呢？
先看nxt[4],由nxt[3]=2,又S[2]=S[4]可知，nxt[4]=nxt[3]+1。
但是对于nxt[5]来说，因为S[nxt[4]+1]!=S[5]，所以这里要回退一步，照这样回退下去，我们最终得到nxt[5]=0

代码：
```cpp
void get_nxt(char *S,int *nxt){
	nxt[1]=nxt[2]=1;
	int len=strlen(S+1);
	for(int i=2;i<=len;i++){
		int j=nxt[i];
		while(j>1&&S[i]!=S[j])j=nxt[j];
		if(S[i]==S[j])j++;
		nxt[i+1]=j;	
	}
}
```

大概就是这样。

参考资料：
[KMP算法详解 by Matrix67](http://www.matrix67.com/blog/archives/115)

