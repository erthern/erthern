#include <stdio.h>
#include <stdlib.h>

typedef struct link
{
    int data;
    struct link *next;
}*Link;

Link Create(int n){
    Link head;
    head = NULL;
    int i=n,j[n];
    while(i>0){
        scanf("%d",&j[i-1]);
        i--;
    }
    if(n == 0) return head;
    while(n>0){
        Link s=(Link)malloc(sizeof(Link));
        s->data = j[i];
        s->next = head;
        head = s;
        i++;
        n--;
    }
    return head;
}
void Display(Link head){
    while (head != NULL) {
        printf("%d", head->data);
        if(head->next != NULL) printf("->");
        head = head->next;
    }
    printf("\n");
}

int main()
{
    int   n;
    Link  head;
    scanf("%d",&n);
    head = Create(n);
    Display(head);
    return 0;
}