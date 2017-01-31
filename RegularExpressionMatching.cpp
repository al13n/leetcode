//Problem: https://leetcode.com/problems/regular-expression-matching/
class Solution {
public:
    bool matches(int sl, int pl){
        //cout << sl << " " << pl << endl;
        if(pl == pp.size()){
            return sl == ss.size();
        }
        if(pl != pp.size()-1 && pp[pl+1] == '*'){
            do{
                if(matches(sl, pl+2)) { return true; }
            }while(sl < ss.size() && (ss[sl++] == pp[pl] || pp[pl] == '.'));
        } else {
            return (pp[pl] == '.' || (sl < ss.size() && ss[sl] == pp[pl])) && matches(sl+1, pl+1);
        }
        
        return false;
    }
    
    bool isMatch(string& s, string& p) {
        ss = s;
        pp = p;
        return matches(0, 0);
    }
    
private:
    string ss;
    string pp;
};
