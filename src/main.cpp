#include <all_header.h>

#define MAX_RES 500

WordX word[MAX_WORD];
ExTrie* root = createNode(ROOT);
int NumOfWord;

struct timeval t0;
struct timeval t1;
float elapsed;

char p[200],t[200];
//char result[1000][MAXW_LENGTH];
int nOresS = 0;
int nOresP = 0;
WordX resultS[MAX_RES],resultP[MAX_RES];

int main(){
    printf("\033c");
    load_word("../asset/preProcess.txt", root , word , &NumOfWord);
    //knnWordSearch(word,NumOfWord-1,12.409);
    //display(root,p,0);
    while(1){
        scanf("%s",p);
        strcpy(t,p);
        if(search_word(root,p))
            printf("\e[32mWord present in dictionary\n\e[39m");
        else 
            printf("\e[31mWord is not present in dictionary\n\e[39m");
        printf("Auto complete suggestion: \n\e[36m");
        gettimeofday(&t0, 0);
        nOresS = 0;
        nOresP = 0;
        suggestion(root,p,0,resultS,MAX_RES,&nOresS);
        sortWord(resultS,nOresS,NO_STATUS);
        printRes(resultS,(nOresS>=6)?6:nOresS);
        gettimeofday(&t1, 0);
        elapsed = timedifference_msec(t0, t1);
        printf("\e[33m%d Suggestion result in %lf seconds...\n",nOresS,(double)elapsed/1000.0);
        printf("\e[39m");

        gettimeofday(&t0, 0);
        int ip = knnWordSearch(word,NumOfWord-1,word2Score(t));
        searchNear(word,resultP,t,NumOfWord-1,ip,25,1,2,5,0,&nOresP);
        sortWord(resultP,nOresP,NO_STATUS);
        gettimeofday(&t1, 0);
        elapsed = timedifference_msec(t0, t1);

        printf("Did you mean : \n\e[92m");
        printRes(resultP,(nOresP>=5)?5:nOresP);
        //printLink(resultP,nOresP);
        //trackRoot(resultP[2].link,t);
        //printf("%s\n",t);
        //printf("PRINT_FAILL");
        printf("\e[33m%d prediction result in %lf seconds...\n\n\n\e[39m",nOresP,(double)elapsed/1000.0);
    }
}