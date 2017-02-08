// Problem: https://leetcode.com/problems/distinct-subsequences/
class Solution {
public:
    int numDistinct(string s, string t) {
        int ssize = s.size();
        int tsize = t.size();
        
        if(!ssize || !tsize) return 0;
        
        int dp[tsize][ssize];
        for(int i = 0; i < tsize; i++){
            for(int j = 0; j < ssize; j++)
                dp[i][j] = 0;
        }
        
        //dp[i][j] = num of subsequences in t[0...i], and ending at s[j]
        
        for(int i = 0; i < tsize; i++){
            for(int j = 0; j < ssize; j++){
                if(t[i] == s[j]){
                    dp[i][j] = 1 + (i > 0 && j > 0 ? dp[i-1][j-1]: 0) + (i > 0 ? dp[i-1][j]: 0);
                }
                dp[i][j] = max(dp[i][j], (i> 0 ? dp[i-1][j]: 0));
            }
        }
        
        int res = 0;
        for(int i = 0; i < ssize; i++){
            res += dp[tsize-1][i];
        }
        
        return res;
    }
};
