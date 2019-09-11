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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
      ListNode *head1 = l1, *head2 = l2, *sum = NULL, *p;
        bool upFlag = false;
        while (1)
        {
            int persum = 0;
            if(head1)
            {
                persum += head1->val;
                head1 = head1->next;
            }
            if(head2)
            {
                persum += head2->val;
                head2 = head2->next;
            }

            if(upFlag)   persum++;
            if(persum >= 10)
            {
                persum -= 10;
                upFlag = true;
            }
            else
                upFlag = false;

            ListNode *node = new ListNode(persum);
            if(!sum)    p = sum = node;
            else    p->next = node;
            p = node;

            if(!head1 && !head2)
            {
                if(upFlag)
                {
                    ListNode *node = new ListNode(1);
                    p->next = node;
                }
                break;
            }
        }
        return sum;  
    }
};