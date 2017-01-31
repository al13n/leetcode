// Problem: https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        vector<int> dp(prices.size()+1, 0);
        vector<int> mx(prices.size()+1, 0);
        int res = 0;
        int sz = prices.size();
        if(sz == 0) return 0;
        if(sz == 1) return 0;
        if(sz == 2) return (prices[1] > prices[0] ? prices[1]-prices[0]: 0);
        int m = prices[sz-1];
        mx[sz-1] = sz-1;
        for(int i = prices.size()-2; i >= 0; i--){
            mx[i] = mx[i+1];
            m = max(prices[i], m);
            if(m == prices[i]){
                mx[i] = i;
            }
        }
       m = 0;
        for(int i = 0; i < prices.size()-2; i++){
            dp[mx[i+2]] = max(dp[mx[i+2]], prices[mx[i+2]] - prices[i+2] + dp[i]);
            m = max(dp[mx[i+2]], m);
            dp[mx[i+2]] = max(dp[mx[i+2]], m);
            res = max(res, dp[mx[i+2]]);
            cout << i << " " << res << endl;
        }
        
        return res;
    }
};
