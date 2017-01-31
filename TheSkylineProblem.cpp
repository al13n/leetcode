//Problem: https://leetcode.com/problems/the-skyline-problem/
class Solution {
public:
    vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int, int>> res;
        vector< pair<int, int> > points;
        for(int i = 0; i < buildings.size(); i++){
            points.push_back(make_pair(buildings[i][0], -(i+1)));
            points.push_back(make_pair(buildings[i][1], (i+1)));
        }
        sort(points.begin(), points.end());
        if(points.empty()) return res;
        
        int currentmax = buildings[-points[0].second-1][2];
        int currentx = points[0].first;
        multiset<int> current;
        current.insert(currentmax);
        
        for(int i = 1; i < points.size(); i++){
            //cout << "POINTS: " << points[i].first << " " << points[i].second << endl;
            //cout << "CXCM: " << currentx << " " << currentmax << endl;
            //for(auto it: res)
                //cout << "res: " << it.first << " " << it.second << endl;
            if(points[i].second < 0){
                int ht = buildings[-points[i].second-1][2];
                if(currentmax < ht){
                    if(points[i].first > currentx && (res.empty() || res.back().second != currentmax)) res.push_back(make_pair(currentx, currentmax));
                    currentmax = ht;
                    currentx = points[i].first;
                }
                current.insert(ht);
            } else {
                int ht = buildings[points[i].second-1][2];
                //cout << ht << " " << points[i].first << endl;
                
                auto itr = current.find(ht);
                if(itr!=current.end()){
                    current.erase(itr);
                }
                
                if(current.empty()){
                    //if(!res.empty()) cout << "Here: " << currentx << " " << currentmax << " " << res.back().second << endl;
                    if(points[i].first > currentx && (res.empty() || res.back().second != currentmax)) res.push_back(make_pair(currentx, currentmax));
                    currentx = points[i].first;
                    currentmax = 0;
                } else {
                    if(*(current.rbegin()) != currentmax){
                        if(points[i].first > currentx && (res.empty() || res.back().second != currentmax)) res.push_back(make_pair(currentx, currentmax));
                        currentx = points[i].first;
                        currentmax = *(current.rbegin());
                    }
                }
            }
        }
        
        if(res.empty() || res.back().second != currentmax) res.push_back(make_pair(currentx, currentmax));
        return res;
    }
};
