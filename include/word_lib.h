#ifndef WORD_LIB_H
#define WORD_LIB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <ex_trie.h>
#include <string_operation.h>

#define MAX_WORD 1000000

#define PRE_PROCESS 1
#define NO_STATUS 0
#define WITH_STATUS 1

const size_t MAXW_LENGTH = 35;

char fnx[50];

FILE *wordF;
FILE *preProcess;

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void addWord2lib(WordX *wx,WordX wrd,int *n){
    WordX swap;
    int pos = 0;
    //*n++;
    for (int i=0 ; i < *n-1; i++){
        if(wrd.score<=wx[i].score){
            if(!pos)pos = i;
            swap = wx[i+1];
            wx[i+1] = wx[i];
            wx[i+2] = swap;
            break;
        }
	}
    wx[pos] = wrd;
    *n++;
}

void sortWord(WordX *wx, int n, int Status){
	WordX swap;
	int swapped;
	int cmp = 0;
	int div = 1000;
	int rang = n/div;
	for (int i=0 ; i < n-1; i++){
		if(i%div==0){
			cmp++;
			//printf("\033c%d percent done\n",(int)(((double)cmp/(double)rang)*100.00));
            if(Status){
                printf("Wait... %d%% done\n",(int)(((double)cmp/(double)rang)*100.00));
                printf("\033[1A");
            }
		}
		swapped = false;
		for (int j = 0 ; j < n - i - 1; j++){
		  if (wx[j].score > wx[j+1].score){
				swap       = wx[j];
				wx[j]   = wx[j+1];
				wx[j+1] = swap;
				swapped = true;
		  }
		}
		if (swapped == false)
        break;
	  }
    if(Status)
	    printf("Need to process %d%% data of 100%%\n",(int)(((double)cmp/(double)rang)*100.0));
}

int knnWordSearch(WordX *wx, int r, int scr){
    int l = 0;
    int m , cnt = 0;
    while (l <= r) { 
        m = l + (r - l) / 2;  

        if(wx[m].score == scr) break;
        if (wx[m].score < scr) 
            l = m + 1;  
        else
            r = m - 1; 
        cnt++;
    } 
    char s[35];
    trackRoot(wx[m].link,s);
    printf("Target : %d\t,",scr);
    printf("Point of interest %d , %d , %s\n",m,wx[m].score,s);
    printf("Searched in O(%d)\n",cnt);
    return m;
}

void searchNear(WordX *wx,WordX *res, char c[] , int n, int m, int dept, int minReq, int Lcompromise, int Hcompromise , int layer , int *no){////
    int cnt = 0;
    int ed;
    char s[35];
    int scr = wx[m].score;
    for(int i=m; (scr==wx[i].score||i<m+dept) && i<n-1;i++){
        trackRoot(wx[i].link,s);
        ed = editDistDP(c,s,strlen(c),strlen(s));
        if(ed<=Lcompromise){
            //strcpy(res[cnt],s);
            res[cnt].link = wx[i].link;
            //trackRoot(res[*no].link,s);
            res[cnt].score = ed;
            //printf("%s\n",s);
            cnt++;
        }
    }

    //printf("FIRST_HALF_DONE!\n");
    for(int i=m-1; (scr==wx[i].score||i>m-dept) && i>=0;i--){
        trackRoot(wx[i].link,s);
        ed = editDistDP(c,s,strlen(c),strlen(s));
        if(ed<=Lcompromise){
            //strcpy(res[cnt],s);
            res[cnt].link=wx[i].link;
            //trackRoot(res[*no].link,s);
            res[cnt].score = ed;
            //printf("%s\n",s);
            cnt++;
        }
    }
    *no = cnt;
    if(!layer)printf("\n");
    if(cnt<=minReq){
        if(Lcompromise<=Hcompromise){
            printf("\e[94mGoing for Deep Search. Pointer at Level #%d \n",Lcompromise+1);
            printf("\033[1A");
            searchNear(wx,res,c,n,m,dept,minReq,Lcompromise+1,Hcompromise,layer+1,no);
        }
    }
    //printf("Complete..\n");
    if(layer) return;
    printf("\e[39m\n\n");
}

void saveAll(WordX *wx,int n){
    wordF = fopen(fnx,"w");
    char c[50];
    for(int i=0;i<n;i++){
        trackRoot(wx[i].link,c);
        fprintf(wordF,"%s\t%d\n",c,wx[i].score);
    }
}

int load_word(const char fn[50], ExTrie* root, WordX *wx, int *n){
    init();
    strcpy(fnx,fn);
	const size_t len = 50;
    char line[len];

	struct timeval t0;
    struct timeval t1;
    float elapsed;

	//root = createNode();
    int score;

	wordF = fopen(fn,"r");
	if (fn == NULL){
        printf("File open error!!!\n");
        return -1;
    }
    int cnt = 0;
	gettimeofday(&t0, 0);
    if(!PRE_PROCESS)
        while (fgets(line, len, wordF) != NULL) {
            line[strlen(line)-1] = '\0';
            lower_string(line);
            if(strCheck(line)){
                wx[cnt].link = insert_word(root, line);
                wx[cnt].score = word2Score(line);
                cnt++;
            }
        }
    else{
        while(fscanf(wordF,"%s\t%d",line,&score)==2){
            //printf("%s\t%d\n",line,score);
            wx[cnt].link = insert_word(root, line);
            wx[cnt].score = score;
            cnt++;
        }
    }
    *n = cnt;
	gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);

	printf("\e[33mTime need to load %d words : %f milliseconds... \n\e[39m",cnt,elapsed);

	fclose(wordF);

    gettimeofday(&t0, 0);
	//sortWord(wx,cnt,1);
	gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);

    if(!PRE_PROCESS){
        preProcess = fopen("preProcess.txt","a");
        for (int c = 0; c < cnt; c++){
		    //printf("%p :: %lf\n", wx[c].link, word[c].score);
		    trackRoot(wx[c].link,line);
		    fprintf(preProcess,"%s%d\n",line,wx[c].score);
	    }
        fclose(preProcess);
    }

    printf("\e[33mTime need to process words %f milliseconds... \n\e[39m",elapsed);
}

#endif