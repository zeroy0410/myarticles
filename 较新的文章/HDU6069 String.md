#### 题目

Bob has a dictionary with N words in it. 
Now there is a list of words in which the middle part of the word has continuous letters disappeared. The middle part does not include the first and last character. 
We only know the prefix and suffix of each word, and the number of characters missing is uncertain, it could be 0. But the prefix and suffix of each word can not overlap. 
For each word in the list, Bob wants to determine which word is in the dictionary by prefix and suffix. 
There are probably many answers. You just have to figure out how many words may be the answer.

大意是给你一个前缀一个后缀，问能够同时满足这个前缀和后缀的串有多少个。



#### 思路

这里有一个非常巧妙的处理方法，一般很难想到：**把待匹配的串T转化为T+'{'+T的形式，把前缀s1和后缀s2转化为s2+'{'+s1的形式**。这样就把原问题转化为了一个普通的字符串匹配问题，只要dfs一遍fail树，把子树信息统计上来就行了。

然而，注意题目中还有一个限制条件：**前缀后缀不能重叠**，也就意味着我们还要维护一维信息。

所以只要把这一维用**树状数组**，然后**dfs作差**就好了。



#### 代码

```cpp
#include<bits/stdc++.h>
#define M 1000005
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int T,n,q,ans[M];
char S[M],wk[M<<1];
char S1[M],S2[M];
int h[M],tot;
struct que{int id,l;};
vector<que>Q[M];
int pre[M];
struct edge{
	int nxt,to;
}G[M<<1];
void Add(int a,int b){
	G[++tot]=(edge){h[a],b};
	h[a]=tot;	
}
struct BIT{
	int C[M];
	void clear(){
		clr(C,0);	
	}
	void add(int x,int d){
		x++;
		while(x<M){
			C[x]+=d;
			x+=(x&-x);	
		}
	}
	int sum(int x){
		int res=0;
		x++;
		while(x){
			res+=C[x];
			x-=(x&-x);
		}
		return res;
	}
}TT;
struct AC_automaton{
	int tt;
	int pa[M][27];
	int f[M];
	vector<int>val[M];
	void clear(){
		clr(pa[0],0);
		clr(f,0);
		for(int i=0;i<=tt;i++)val[i].clear();
		tt=0;
	}
	int Insert(char *S,int d){
		int u=0,l=strlen(S);
		for(int i=0;i<l;i++){
			int c=S[i]-'a';
			if(!pa[u][c]){
				pa[u][c]=++tt;
				clr(pa[tt],0);
				Q[tt].clear();
			}
			u=pa[u][c];
			if(d!=-1)val[u].push_back(d);
		}
		return u;
	}
	void get_fail(){
		queue<int>Q;
		for(int i=0;i<=26;i++){
			if(pa[0][i]!=0){
				f[pa[0][i]]=0;
				Q.push(pa[0][i]);	
			}
		}
		while(!Q.empty()){
			int u=Q.front();Q.pop();
			for(int i=0;i<=26;i++){
				if(pa[u][i]!=0){
					f[pa[u][i]]=pa[f[u]][i];
					Q.push(pa[u][i]);
				}
				else pa[u][i]=pa[f[u]][i];
			}
		}
		for(int i=1;i<=tt;i++)if(f[i]!=i)Add(f[i],i);
	}
}Tr;
void jie(char *s1,char *s2,char *s3){
	int l1=strlen(s1);
	int l2=strlen(s2);
	int l3=0;
	for(int i=0;i<l1;i++)s3[l3++]=s1[i];
	s3[l3++]='{';
	for(int i=0;i<l2;i++)s3[l3++]=s2[i];
}
int find(char *s){
	int l=strlen(s),u=0;
	for(int i=0;i<l;i++){
		if(!Tr.pa[u][s[i]-'a'])return -1;
		u=Tr.pa[u][s[i]-'a'];
	}
	return u;
}
void dfs(int x){
	for(int i=0;i<Q[x].size();i++)
		pre[Q[x][i].id]=TT.sum(M-1)-TT.sum(Q[x][i].l-1);
	for(int i=0;i<Tr.val[x].size();i++)
		TT.add(Tr.val[x][i],1);
	for(int i=h[x];i;i=G[i].nxt)dfs(G[i].to);
	for(int i=0;i<Q[x].size();i++)
		ans[Q[x][i].id]=TT.sum(M-1)-TT.sum(Q[x][i].l-1)-pre[Q[x][i].id];
}
int main(){
	cin>>T;
	while(T--){
		Tr.clear();clr(h,0);tot=0;
		TT.clear();clr(ans,0);
		scanf("%d%d",&n,&q);
		for(int i=1;i<=n;i++){
			scanf("%s",S);
			int l=strlen(S);
			jie(S,S,wk);
			wk[l+l+1]=0;
			Tr.Insert(wk,strlen(S));
		}
		for(int i=1;i<=q;i++){
			scanf("%s%s",S1,S2);
			jie(S2,S1,wk);
			wk[strlen(S1)+strlen(S2)+1]=0;
			int k=Tr.Insert(wk,-1);
			if(k==-1)ans[i]=0;
			else Q[k].push_back((que){i,strlen(wk)-1});
		}
		Tr.get_fail();
		dfs(0);
		for(int i=1;i<=q;i++)
			printf("%d\n",ans[i]);
	}
	return 0;
}
```



