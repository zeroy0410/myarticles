$dp[i][S]$表示选到某个男生，女生的选择集合。然后暴力转移，复杂度$O(2^{n*m}*K*m)$

极限数据一亿多，然而卡过去了。

```cpp
#include<bits/stdc++.h>
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,m,ans=0;
bool A[15][15];
vector<int>P; 
int dp[15][1<<10];//选到某个男生，女生的选择情况，方案数 
int calc(int x){
	int ct=0;
	while(x)x-=(x&-x),ct++;
	return ct;	
}
class DancingCouples {
public:
	int countPairs(vector <string> canDance, int K) {
		ans=0;
		n=canDance.size();
		m=canDance[0].size();
		for(int i=0;i<n;i++)
			for(int j=0;j<m;j++)
				A[i][j]=(canDance[i][j]=='Y');
		for(int i=0;i<1<<n;i++){//状压选择的男生 
			if(calc(i)!=K)continue;
			P.clear();
			for(int j=0;j<n;j++)
				if(i&1<<j)P.push_back(j);
			dp[0][0]=1;
			for(int j=0;j<K;j++){
				int x=P[j];
				clr(dp[j+1],0);
				for(int k=0;k<1<<m;k++){
					for(int r=0;r<m;r++){
						if(k&1<<r)continue;
						if(A[x][r])dp[j+1][k|(1<<r)]+=dp[j][k];		
					}
				}
			}
			for(int j=0;j<1<<m;j++)
				ans+=dp[K][j]; 
		}
		return ans;
	}
};
```

