#### 其实这是一道简单题不知道为什么1AC

定义$dp[i][S]$表示前$i$的长度，取了$S$这个集合，这样就可以用$dp[i][S]$更新$dp[i+1][S&v]​$。
最后统计一下答案就好了。

#### 代码
```cpp
#include<bits/stdc++.h>
using namespace std;
const int M=1000003;
int st,cnt,dp[52][1<<15],len,ans,n;//f[x][y]表示到第x位已经考虑了y（二进制表示） 
char c;
class SetOfPatterns{
	public:
		int howMany(vector<string>s,int k){
			n=s.size();
			len=s[0].size();
			for(int i=0;i<len;i++)//考虑每一位 
				for(c='a';c<='z';c++){//考虑这一位上放什么 
					st=0;//st表示这一位上可以成为c的方案（二进制表示） 
					for(int j=0;j<n;j++)
						if(s[j][i]=='?'||s[j][i]==c)st|=1<<j;
					if(!i)dp[i][st]++;
					else for(int j=0;j<1<<n;j++)(dp[i][st&j]+=dp[i-1][j])%=M;
				}
			for(int i=0;i<1<<n;i++){
				cnt=0;
				for(int j=0;j<n;j++)
					if(i&(1<<j))cnt++;
				if(cnt==k)(ans+=dp[len-1][i])%=M;
			}
			return ans;
		}
};
```