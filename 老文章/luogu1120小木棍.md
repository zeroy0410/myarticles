---
title: Luogu1120 小木棍
---

#### 题目

乔治有一些同样长的小木棍，他把这些木棍随意砍成几段，直到每段的长都不超过5050。

现在，他想把小木棍拼接成原来的样子，但是却忘记了自己开始时有多少根木棍和它们的长度。

给出每段小木棍的长度，编程帮他找出原始木棍的最小可能长度。

#### 思路

这题据说有九种剪枝，我只~~香到~~想出了8种。

剪枝1：可行的答案显然应该整除木棒的长度和。（显然）

剪枝2：根据奇偶性 最终要得到奇数长度的木棒，奇数小木棒的条数必须满足每组至少一个（显然）

剪枝3：上下界剪枝（显然）

剪枝4：如果当前木棒作为第一根，找不到解，那么后来的木棒一定也找不到解。（仔细想想）

剪枝5：同剪枝2，在dfs中也可根据奇偶性剪枝（显然）

剪枝6：有很多木棒是重复的，重复的木棒直接跳过（显然）

剪枝7：从大到小枚举木棒，如果当前加上这根能组成一组，但找不到答案，则退出，因为后面小的木棒更灵活一些，用它来代替当前木棒只会更难找到解。

剪枝8：在搜索之前先套一层“找零”找出可行性。（这貌似没用）

#### 代码

```cpp
#include<bits/stdc++.h>
#define clr(x,y) memset(x,y,sizeof(x))
using namespace std;
int n,A[70],rt,mx;
int sum;
bool mark[70];
bool dfs(int x,int len,int cnt,int las){//匹配到哪，当前匹配之长度，奇数个数，还要匹配的根数 
    if(las<=1)return 1;
    else if(len==rt){if(dfs(1,0,cnt,las-1))return 1;}
    else {
        if((rt&1)&&cnt<las-1)return 0;
        if(len+A[n]>rt)return 0;
        for(int i=x;i<=n;i++){
            if(A[i]>50)continue;
            if(!mark[i]){
                mark[i]=1;
                if(dfs(i+1,len+A[i],cnt-(A[i]&1),las))return 1;
                mark[i]=0;	
                if(!len)return 0;
                if(len+A[i]==rt)return 0;
                while(i<=n&&A[i]==A[i+1])i++;
            }
        }
    }
    return 0;
}
bool cmp(int a,int b){return a>b;}
int main(){
    while(1){
        scanf("%d",&n);clr(mark,0);
        int ct=0; 
        if(n==0)break;
        mx=0;sum=0;
        for(int i=1;i<=n;i++){
            scanf("%d",&A[i]);
            if(A[i]>50)continue;
            if(A[i]&1)ct++;
            mx=max(A[i],mx);
            sum+=A[i];
        }
        sort(A+1,A+n+1,cmp);
        int ans=0;
        for(int i=mx;i<=sum;i++){
            if(sum%i!=0)continue;
            rt=i;
            int ned=sum/i;//组数 
            if((i&1)&&(ct<ned||(ct-ned)&1))continue;
            if(dfs(1,0,ct,ned)){ans=i;break;}
        }
        printf("%d\n",ans);
        break;
    }
    return 0;
}
```

