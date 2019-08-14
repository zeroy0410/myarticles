#### 题目

The repetition number of a string is defined as the maximum number *R* such that the string can be partitioned into *R* same consecutive substrings. For example, the repetition number of "ababab" is 3 and "ababa" is 1.

Given a string containing lowercase letters, you are to find a substring of it with maximum repetition number.

大意是，给你一个字符串，让你输出其中重复次数最多的重复连续字串，如果次数相同就输出字典序最小的。

#### 思路

一开始不好想。

首先，枚举串的长度$L$，然后枚举区间$[1,L]$，$[L+1,2*L]$，会发现，一个连续重复子串总会被截头去尾的被枚举到。

比如`cabababc`，在枚举到长度$L=2$的时候,中间的那段`baba`就会被枚举到，然而实际上可能是去了头尾的，注意到LCP求出后后面会多出一段，定义$R=LCP$，那么，R%L就是多出来的一段，可以看成是前面少取了L-R%L这一段，然后判断是不是可以把这一段补上，就完成了第一步:找到一段区间内的最长重复连续子串。

然后考虑第二步：要求字典序最小。

我们把这些可能成为答案的串先存起来，然后按照的顺序一遍扫描，因为sa天然有序，所以就可以得到最终的答案了。



#### 代码

```c++
#include<cstdio>
#include<string.h>
#include<algorithm> 
#include<iostream>
#define M 100005
using namespace std;
int sa[M],rk[M],t1[M],t2[M],tmp[M],cnt1[M],cnt2[M],H[M];
struct node{
	int x,id;
	bool operator < (const node&  res)const{
		if(x!=res.x)return x<res.x;
		return id<res.id;
	}
}A[M];
void Init(char *s,int n){
	for(int i=1;i<=n;i++)A[i]=(node){s[i],i};
	sort(A+1,A+n+1);
	for(int i=1;i<=n;i++)sa[i]=A[i].id; 
	rk[sa[1]]=1;
	for(int i=2;i<=n;i++){
		rk[sa[i]]=rk[sa[i-1]];
		if(s[sa[i]]!=s[sa[i-1]])rk[sa[i]]++; 
	}
	for(int l=1;rk[sa[n]]<n;l<<=1){
		for(int i=0;i<=n;i++)cnt1[i]=cnt2[i]=0;
		for(int i=1;i<=n;i++)cnt1[t1[i]=rk[i]]++,cnt2[t2[i]=(l+i<=n)?rk[i+l]:0]++;
		for(int i=1;i<=n;i++)cnt1[i]+=cnt1[i-1],cnt2[i]+=cnt2[i-1];
		for(int i=n;i>=1;i--)tmp[cnt2[t2[i]]--]=i;
		for(int i=n;i>=1;i--)sa[cnt1[t1[tmp[i]]]--]=tmp[i];
		rk[sa[1]]=1;
		for(int i=2;i<=n;i++){
			rk[sa[i]]=rk[sa[i-1]];
			if(t1[sa[i]]!=t1[sa[i-1]]||t2[sa[i]]!=t2[sa[i-1]])rk[sa[i]]++;
		}
	}
	for(int i=1,j=0;i<=n;i++){
		j-=j>0;
		while(s[i+j]==s[sa[rk[i]-1]+j])j++;
		H[rk[i]]=j;
	}
}
struct Stable{
	int mn[M][21],Log[M];
	void Init(int n){
		for(int i=1;i<=n;i++){
			mn[i][0]=H[i];
			if(i>1)Log[i]=Log[i>>1]+1;
		}
		for(int j=1;j<21;j++)
			for(int i=1;i+(1<<j-1)<=n;i++)
				mn[i][j]=min(mn[i][j-1],mn[i+(1<<j-1)][j-1]);
	}
	int Query(int l,int r){
		int t=Log[r-l+1];
		return min(mn[l][t],mn[r-(1<<t)+1][t]);
	}
}st;
int LCP(int l,int r){
	l=rk[l],r=rk[r];
	if(l>r)swap(l,r);
	return st.Query(l+1,r);
}
char S[M];
int n,cas=0,a[M],cnt;
int main(){
	while(scanf("%s",S+1)&&S[1]!='#'){
		n=strlen(S+1);
		Init(S,n);
		st.Init(n);
		int ans=0;
		for(int L=1;L<=n;L++){
			for(int i=1;i<=n;i+=L){
				int R=LCP(i,i+L),step=R/L+1,k=i-(L-R%L);
				if(k>=0&&R%L)if(LCP(k,k+L)>=R)step++;
				if(step>ans){ans=step;cnt=0;a[cnt++]=L;}
				else if(step==ans)a[cnt++]=L;
			}
		}
		int len=-1,st;
		for(int i=1;i<=n&&len==-1;i++){
			for(int j=0;j<cnt;j++){
				int L=a[j];
				if(LCP(sa[i],sa[i]+L)>=(ans-1)*L){
					len=L;st=sa[i];break;
				}
			}
		}		
		printf("Case %d: ",++cas);
		for(int i=st;i<st+len*ans;i++)printf("%c",S[i]);
		putchar('\n');
		for(int i=1;i<=n;i++)S[i]=0;
	}
	return 0;
}
```

