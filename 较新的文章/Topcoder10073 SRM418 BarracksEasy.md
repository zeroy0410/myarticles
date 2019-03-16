这题最困难的方面是战斗的策略。

假设我们有$i$个士兵，兵营的血量为$z$,我们的对手有$j$个士兵，设$dp[i][j][k]$为我们赢得战斗的最小回合数（如果不可能，则为无穷）。计算时，按照题目的步骤一步步模拟递推就行了。

但是如果只是这样的话，可能会出现环，或者无穷的状态（比如$j$不断往上增长），所以为了解决这一点，需要将$j>100$的情况全部舍弃。

具体细节请看代码

```cpp
#include<bits/stdc++.h>
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int dp[55][2005][55];
class BarracksEasy {
public:
	int attack(int myUnits, int barHp, int unitsPerRound) {
		clr(dp,60);
		for(int i=1;i<=myUnits;i++)dp[i][0][0]=0;
		for(int i=1;i<=myUnits;i++)
			for(int z=0;z<=barHp;z++)
				for(int j=0;j<=100;j++){
					for(int k=0;k<=i;k++){//多少士兵去攻击对面的大本营 
				    	int bH=max(0,z-k);
				    	int op=max(0,j-i+k);
				    	int mine=max(0,i-op);
				    	if(bH>0)op+=unitsPerRound;
				    	if(k==0 && bH>0)continue;//如果没有士兵去攻击大本营，那么状态转移会出现环，直接跳过 
				    	if(op<=100)dp[i][j][z]=min(dp[i][j][z],dp[mine][op][bH]+1);
			        }
				}
		if(dp[myUnits][0][barHp]>1e9)return -1;
		return dp[myUnits][0][barHp];
	}
};
```

