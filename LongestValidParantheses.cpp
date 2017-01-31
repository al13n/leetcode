// Problem: https://leetcode.com/problems/longest-valid-parentheses/
class Solution {
public:
    int longestValidParentheses(string s) {
        stack<int> st;
        st.push(0);
        
        int res = 0;
        
        for(int i = 0; i < s.size(); i++){
            if(s[i] == ')'){
                if(st.size()>1) {
                    st.pop();
                    res = max(res, i+1 - st.top());
                    //cout << st.top() << endl;
                } else {
                    st.pop();
                    st.push(i+1);
                }
            } else {
                st.push(i+1);
            }
        }
        return res;
    }
};
