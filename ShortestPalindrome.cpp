// Problem: https://leetcode.com/problems/shortest-palindrome/
class Solution {
public:
    string rev(string s){
        string res = "";
        for(int i = s.size()-1; i >= 0; i--)
            res += s[i];
        return res;
    }

    string shortestPalindrome(string s) {
        int len = 0;
        int i = 1;
        string cpy = s;
        s = s + '#' + rev(s);

        int lps[s.size()+1] = {0};

        while(i < s.size()){
            if(s[i] == s[len]){
                lps[i] = ++len;
                i++;
            } else {
                if(len != 0) len = lps[len-1];
                else {
                    lps[i] = 0;
                    i++;
                }
            }
        }

        string res = cpy;

        for(int i = lps[s.size()-1]; i < cpy.size(); i++){
            res = cpy[i] + res;
        }
        return res;
    }
};
