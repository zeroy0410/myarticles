# Hashing & KMP

## 前言

>  进入了字符串专题，总该接触一些字符串的算法了

### 关于Hashing

#### 写法

这个其实没什么好说的，假如是小写字母的话，我们可以搞一个27进制，再取一个大数字作为模数，用数字来表示字符串。

若担心hash冲突，可使用双模数。

#### 用途

快速判断两个字符串是否相同。

### 关于KMP算法

#### 写法

是一个极为神仙的算法了，代码极短，但又并不容易理解。

普通的字符串匹配问题：给定一个字符串$S$，长度为$n$，再给出一个字符串$E$，长度为$m(m<n)$，询问$E$是否为$S$的子串。

考虑朴素的算法，每次枚举一个光标位置，往右扫m个逐一比较，看看是否相同（貌似Hashing可做，不过这里讨论的是KMP算法），最坏情况下，大概是$O(n*m)$（不过假如加一个break的话，对于随机数据这种算法效率意外得高）。

KMP算法就是在这个算法基础之上的优化，当我们匹配完前$i$个字符后，匹配第$i+1$个时，假如失配了，当我们在扫到下一个坐标时，我们再从第一个开始匹配，不是很浪费？——KMP算法因此产生。

前i个字符其实已经知道了，假如是$ABAB$，那么我们下一次其实只用去匹配后面一个$AB$才有效果了，于是我们可以处理出了一个失配数组，表示第$i$位失配时，下一次应匹配哪一位，就如我举的例子，第5位失配了，下次就因匹配第3位，然而假如第三位又失配了，下一次就应匹配第一位了。

那么失配数组又怎么处理呢？我们拿字符串自己与自己匹配即可，二者做法类似。

预处理失配数组代码如下:

```c++
void getf(char *s,int l){
	f[0]=f[1]=0;
	for(int i=1;i<l;i++){
		int j=f[i];
		while(j&&s[i]!=s[j])j=f[j];
		if(s[i]==s[j])j++;
		f[i+1]=j;
	}
}
```

字符串匹配过程代码如下：

```c++
for(int i=0,j=0;i<l1;i++){
    while(j&&s2[j]!=s1[i])j=f[j];
    if(s2[j]==s1[i])j++;
    //现在j就表示下一个要匹配的是什么
}
```

二者极为相似。

KMP的算法是$O(n+m)$的优越复杂度。

#### 用途

字符串匹配以及相关问题。

## 专题例题

### Hashing

#### Aladdin and the Magical Lamp

##### 题意

求三个字符串的最长公共子串。

##### 思路

二分+哈希

单调性是显然的：假如有一个$l$长的公共子串，则$[1,l]$都满足。

二分出了一个长度，先把第一个字符串l长的子串全部取出，丢到一个set里（保佑它不会冲突），再把第二个字符串的长为l的子串取出，若在第一个的set里有，丢到另一个set里，最后把第三个字符串的长为l的子串取出，若在第二个的set里有，就说明找到了。

##### 代码

```c++
#include<bits/stdc++.h>
#define M 5005
#define LL long long
using namespace std;
const int P1=1e9+9;
char s[3][M];
int len[3],mn;
LL base[M],sum[3][M];
set<int>S,S2;
bool can(int l,int id){
	for(int i=l;i<=len[id];i++){
		int pre=i-l;
		LL tmp=(sum[id][i]-sum[id][pre]*base[l])%P1;//每次取就拿前缀和相减
		if(tmp<0)tmp+=P1;
		if(!id)S.insert(tmp);
		else if(id==1){
			if(S.find(tmp)!=S.end())S2.insert(tmp);
		}else if(S2.find(tmp)!=S2.end())return 1;
	}return 0;
}
bool check(int x){
	int c=0;S.clear();S2.clear();
	for(int i=0;i<3;i++)c+=can(x,i);
	return c>0;
}
void Init(){
	base[0]=1;
	for(int i=1;i<M;i++)base[i]=base[i-1]*31%P1;
}
void Work(int x){//hash的前缀和过程
	for(int i=1;i<=len[x];i++)
		sum[x][i]=(sum[x][i-1]*31+(s[x][i]-'a'))%P1;
}
int main(){
	Init();
	for(int _=(scanf("%d",&_),_),kase=0;_;_--){
		scanf("%s %s %s",s[0]+1,s[1]+1,s[2]+1);mn=2e9;
		for(int i=0;i<3;i++)len[i]=strlen(s[i]+1),mn=min(mn,len[i]);
		for(int i=0;i<3;i++)Work(i);
		int l=0,r=mn,res=0;
		while(l<=r){
			int mid=(l+r)>>1;
			if(check(mid))res=mid,l=mid+1;
			else r=mid-1;
		}printf("Case %d: %d\n",++kase,res);
	}
	return 0;
}
```

#### Making Huge Palindromes

##### 题意

给出一个字符串，向它右边添加一些字符，使得其成为一个回文，求最短长度。

##### 思路

分偶数与奇数，枚举一个中点，向两边扩展，保证右边的长度比左边小，假设右边的串能匹配左边等长的串，就可以接上左边剩余的长度的串。

##### 代码

```c++
#include<bits/stdc++.h>
#define M 1000005
#define LL long long
using namespace std;
bool ___1;
char s[M];
int len;
struct HASH{
	int base[M],sum[2][M];//一个前缀，一个后缀
	LL P;
	void Init(LL mo){
		base[0]=1;P=mo;
		for(int i=1;i<=len;i++)base[i]=27LL*base[i-1]%mo;
		for(int i=1;i<=len;i++)sum[0][i]=(LL)(1LL*sum[0][i-1]*27+(s[i]-'a'+1))%mo;
		sum[1][len+1]=0;
		for(int i=len;i>=1;i--)sum[1][i]=(LL)(1LL*sum[1][i+1]*27+(s[i]-'a'+1))%mo;
	}
	int Get(int l,int r,bool f){
		if(!f)return (((LL)sum[0][r]-1LL*sum[0][l-1]*base[r-l+1])%P+P)%P;
		else return (((LL)sum[1][l]-1LL*sum[1][r+1]*base[r-l+1])%P+P)%P;
	}
}ha1,ha2;
bool ___2;
int main(){
	for(int _=(scanf("%d",&_),_),kase=0;_;_--){
		scanf("%s",s+1);
		len=strlen(s+1);
		ha1.Init(1e9+7),ha2.Init(1e9+9);
		int ans=2e9;
		for(int i=1;i<=len;i++){//奇数 
			int l=i-1,r=len-i;
			if(l>=r){
				if(!r)ans=min(ans,len*2-1);
				else{
					LL ar=ha1.Get(i+1,len,0),br=ha2.Get(i+1,len,0);
					LL al=ha1.Get(i-r,i-1,1),bl=ha2.Get(i-r,i-1,1);
					if(ar==al&&br==bl)ans=min(ans,l-r+len);
				}
			}
		}
		for(int i=1;i<len;i++){//偶数 
			int l=i,r=len-i;
			if(l>=r){
				LL ar=ha1.Get(i+1,len,0),br=ha2.Get(i+1,len,0);
				LL al=ha1.Get(i-r+1,i,1),bl=ha2.Get(i-r+1,i,1);
				if(ar==al&&br==bl)ans=min(ans,l-r+len);
			}
		}printf("Case %d: %d\n",++kase,ans);
	}
	return 0;
}
```

### KMP

#### Lucky Common Subsequence

##### 题意

给出三个字符串s1,s2,virus，找出s1,s2最长公共子序列，使得其子串不包括virus。

##### 思路

考虑dp。

普通的lcs模板是这样的:

```c++
for(int i=1;i<=l1;i++)
    for(int j=1;j<=l2;j++)
        if(s1[i]==s2[j])dp[i][j]=dp[i-1][j-1]+1;
        else dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
```

现在只需再加上一维，表示对于virus串已经匹配的长度为k。

转移时套用kmp算法，假如$s1[i]==s2[j]$就拿去匹配，若不能完全匹配，则转移。

转移时多了一维，复杂度$O(n^3)$

具体见代码

##### 代码

```c++
#include<bits/stdc++.h>
#define M 105
using namespace std;
char s1[M],s2[M],viru[M];
int l1,l2,l3,f[M];
void Getf(char *s,int len){
	f[0]=f[1]=0;
	for(int i=1;i<len;i++){
		int j=f[i];
		while(j&&s[j]!=s[i])j=f[j];
		if(s[j]==s[i])j++;
		f[i+1]=j;
	}
}
struct node{
	int lst1,lst2,lst3,v;char c;//还需记录方案
	bool operator <(const node &_)const{
		return v<_.v;
	}	
}dp[M][M][M];
void print(node x){
	if(!x.c)return;
	print(dp[x.lst1][x.lst2][x.lst3]);
	printf("%c",x.c);
}
int main(){
	scanf("%s %s %s",s1+1,s2+1,viru);
	l1=strlen(s1+1);
	l2=strlen(s2+1);
	l3=strlen(viru);
	Getf(viru,l3);
	for(int i=0;i<=l1;i++)	
		for(int j=0;j<=l2;j++)
			for(int q=0;q<l3;q++)
				dp[i][j][q].v=-2e9;
	for(int i=0;i<=l1;i++)dp[i][0][0].v=0;
	for(int i=1;i<=l2;i++)dp[0][i][0].v=0;
	for(int i=1;i<=l1;i++)
		for(int j=1;j<=l2;j++){
			if(s1[i]==s2[j])//若相同则取这个字符去匹配第q位
				for(int q=0;q<l3;q++){
					int t=q;
					while(t&&s1[i]!=viru[t])t=f[t];
					if(s1[i]==viru[t])t++;
					node tmp=dp[i-1][j-1][q];
					tmp.v++,tmp.lst1=i-1,tmp.lst2=j-1,tmp.lst3=q,tmp.c=s1[i];
                    //记录上一层的方案
					if(t<l3)dp[i][j][t]=max(dp[i][j][t],tmp);
				}
			for(int q=0;q<l3;q++)//不取直接转移
				dp[i][j][q]=max(dp[i][j][q],max(dp[i-1][j][q],dp[i][j-1][q]));
		}
	node tmp;tmp.v=0;
	for(int i=1;i<=l1;i++)
		for(int j=1;j<=l2;j++)
			for(int q=0;q<l3;q++)
				tmp=max(tmp,dp[i][j][q]);
	if(tmp.v>0)print(tmp);	
	else puts("0");
	return 0;
}
```

#### Anthem of Berland

##### 题意

给出两个字符串s，t，s字符串中包括一些未知的位，用"?"表示，可表示任意字符，求t可能在s中出现的最多次数。

##### 思路

还是dp。

再预处理t的失配数组时，我们可以再预处理出另一个数组$nxt[i][j]$表示已经匹配的长度为$i$时，下一个字符的为$j$时，可以匹配（失配）到哪个长度。

再进行dp转移，$dp[i][j]$即表示第i个坐标，匹配到第j个时t出现的最多次数。假如这个点为"?"，就枚举26个字母，用nxt数组O(1)转移。

细节见代码。

##### 代码

```c++
#include<bits/stdc++.h>
#define M 100005
using namespace std;
char s1[M],s2[M];
int **dp;
int nxt[M][26];
int f[M];
void Getf(char *s,int len) {
	f[1]=f[2]=1;
	for(int i=2; i<=len; i++) {
		int j=f[i];
		while(j>1&&s[j]!=s[i])j=f[j];
		if(s[j]==s[i])j++;
		f[i+1]=j;
	}
	for(int i=0; i<=len; i++) {//预处理nxt数组
		for(int j=0; j<26; j++) {
			int t=i+1;
			while(t>1&&s[t]!=j+'a')t=f[t];
			if(s[t]==j+'a')t++;
			nxt[i][j]=t-1;
		}
	}
}
void Max(int &x,int y) {
	if(y>x)x=y;
}
int main() {
	scanf("%s %s",s1+1,s2+1);
	int l1=strlen(s1+1),l2=strlen(s2+1);
	if(l2>l1)return puts("0"),0;
	Getf(s2,l2);
	dp=new int*[l1+1];
	for(int i=0; i<=l1; i++) {
		dp[i]=new int[l2+1];
		for(int j=0; j<=l2; j++)dp[i][j]=-2e9;
	}
	dp[0][0]=0;
	for(int i=1; i<=l1; i++)//dp转移过程
		for(int j=0; j<=l2; j++)
			if(dp[i-1][j]>=0) {
				if(s1[i]=='?') {
					for(int q=0; q<26; q++) {
						int nx=nxt[j][q];
						Max(dp[i][nx],dp[i-1][j]+(nx==l2));
					}
				} else {
					int nx=nxt[j][s1[i]-'a'];
					Max(dp[i][nx],dp[i-1][j]+(nx==l2));
				}
			}
	int ans=0;
	for(int j=0;j<=l2;j++)Max(ans,dp[l1][j]);
	printf("%d\n",ans);
	return 0;
}
```

#### String Compression

##### 题意

给出一个字符串，进行字符串压缩（如aaa变成3a,abcabc变成2abc），求字符串压缩后的最短长度。

##### 思路

考虑朴素的dp

```c++
for(int i=1;i<=n;i++)
    for(int j=0;j<i;j++)
        dp[i]=min(dp[i]，dp[j]+/*区间[j+1,i]压缩后的最短长度*/)
```

为了求注释里的东西，我们再看一道题[Power String](http://poj.org/problem?id=2406)，题目就是问你用最短的前缀压缩一整个字符串。

这里可以用到KMP的失配数组，不难想到如下式子（下标从1开始）：

```c++
if(len%(len-f[len+1]+1)==0)
		ans=len/(len-f[len+1]+1);
```

同理，这道题我们也可以用到这个结论，先枚举一个起始坐标，用KMP算法一个个往后扫求出失配数组以及最短压缩前缀，再加上数字的长度即可在$O(n^2)$里处理出转移数组。

##### 代码

```c++
#include<bits/stdc++.h>
#define M 8005
using namespace std;
char s[M];
int len,f[M];
int dp[M],cnt[M][M];
int WEI(int x){
	int d=0;
	while(x)d++,x/=10;
	return d;
}
int main(){
	scanf("%s",s+1);
	len=strlen(s+1);
	for(int i=1;i<=len;i++){
		memset(f,0,sizeof(f));
		f[i]=f[i+1]=i;
		cnt[i][i]=2;
		for(int j=i+1;j<=len;j++){
			int t=f[j];
			while(t>i&&s[t]!=s[j])t=f[t];
			if(s[t]==s[j])t++;
			f[j+1]=t;
			int l=j-i+1;
			cnt[i][j]=l%(j-f[j+1]+1)==0?(j-f[j+1]+1)+WEI(l/(j-f[j+1]+1)):l+1;
            //预处理[i,j]区间可以被压缩成的最短长度
		}
	}
	memset(dp,127,sizeof(dp));
	dp[0]=0;
	for(int i=1;i<=len;i++)
		for(int j=0;j<i;j++)
			dp[i]=min(dp[i],dp[j]+cnt[j+1][i]);
	printf("%d\n",dp[len]);
	return 0;
}
```

