// Problem: https://leetcode.com/problems/course-schedule-ii/
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<pair<int, int>>& prereq) {
        vector< vector<int> > graph (numCourses, vector<int> ());
        vector<int> indegree(numCourses, 0);
        for(auto it: prereq){
            graph[it.second].push_back(it.first);
            indegree[it.first]++;
        }
        queue<int> q;
        for(auto& it: indegree){
            auto index = &it - &indegree[0];
            //cout << it << " " << index;
            if(!it) q.push(index);
        }
        vector<int> res;
        while(!q.empty()){
            int t = q.front();
            q.pop();
            res.push_back(t);
            for(auto it: graph[t]){
                if(--indegree[it] == 0)
                    q.push(it);
            }
        }
        if(res.size() != numCourses)
            res.clear();
        return res;
    }
};
