//Problem: https://leetcode.com/problems/reverse-nodes-in-k-group/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* reverse(ListNode* start, ListNode* end){
        //if(end != NULL) cout << start->val << " " << end->val << endl;
        //else cout << start->val << endl;
        ListNode* next = start->next;
        ListNode*prev = start;
        
        while(next != end){
            ListNode* tmpnext = next->next;
            next->next = prev;
            prev = next;
            next = tmpnext;
        }
        start->next = end;
        return prev;
    }
    
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* start = head;
        ListNode* prev = start;
        
        bool flag = true;
        while(start != NULL){
            int cnt = 0;
            ListNode* end = start;
            
            while(cnt < k && end != NULL) end = end->next, cnt++;
            if(cnt != k) break;
            
            start = reverse(start, end);
            
            if(flag) head = start, flag = false;
            else prev->next = start;
            
            cnt = 0;
            
            while(cnt < k-1 && start != NULL) start = start->next, cnt++;
            prev = start;
            if(start != NULL) start = start->next;
        }
        
        return head;
    }
};
