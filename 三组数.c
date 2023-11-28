#include<stdio.h>
double ave1(double *a){
    double ave1;
    int i;
    for ( i = 0; i < 5; i++)
    {
        ave1+=a[i];
    }
    ave1/=5;
    return ave1;
}
double ave2(double *a){
    double ave2=0;
    int i;
    for ( i = 0; i < 15; i++)
    {
        ave2+=a[i];
    }
    ave2/=15;
    return ave2;
}
double max(double *a){
    double max=a[0];
    int i;
    for ( i = 0; i < 15; i++)
    {
        if(a[i]>max){
            max=a[i];
        }
    }
    return max;
}
int main(){
    double arr[3][5];
    int i,t;
    for(i=0;i<3;i++){
            for(t=0;t<5;t++){
                scanf("%lf",&arr[i][t]);
            }
    }
    for(i=0;i<3;i++){
        printf("%lf\n",ave1(arr[i]));
    }
    printf("%lf,%lf\n",ave2(arr),max(arr));
}