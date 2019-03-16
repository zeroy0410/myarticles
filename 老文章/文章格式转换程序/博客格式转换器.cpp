#include<bits/stdc++.h>
using namespace std;
int main(){
	int cnt=0;
	string s="";
	freopen("input.in","r",stdin);
	freopen("output.out","w",stdout);
	char c;
	while(scanf("%c",&c)!=EOF)s+=c;
	for(int i=0;i<(int)s.length();i++){
		if(s[i]=='$'&&s[i-1]!='$'&&s[i+1]!='$'){
			cnt++;
			if(cnt&1){cout<<"\\"<<"\\"<<"(";}
			else cout<<"\\"<<"\\"<<")";	
		}
		else cout<<s[i];
	}
	return 0;	
}
