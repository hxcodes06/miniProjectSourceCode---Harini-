#include <stdio.h>
int main(){
    int N;
    scanf("%d",&N);
    
    int marks[N][3];
    int total;
    float subjectsum[3]={0,0,0};
    for (int i = 0; i < N; i++){
        for (int j = 0; j < 3; j++ ){
            scanf("%d", &marks[i][j]);
            subjectsum[j] += marks[i][j];
        }
    }
    for(int i=0; i< N; i++){
        total = 0;
        for(int j=0;j<3;j++){
            total += marks[i][j];
        }
        printf("Student %d Total: %d\n",i+1,total);
    }
    printf("\n");
    for(int j=0;j<3;j++){
        printf("Subject %d Average: %.2f\n",j+1,subjectsum[j]/N);
    }
    return 0;
    
}

