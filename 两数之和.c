/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
 #include<stdlib.h>
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int i,j;
    int* s=(int*)malloc(sizeof(int)*(*returnSize));
    for(i=0;i<numsSize;i++){
        for(j=0;j<numsSize;j++){
            if(nums[i]+nums[j]==target) {
                s[0]=i;
            s[1]=j;
            return s;
            }
        }
    }
    return NULL;
}
