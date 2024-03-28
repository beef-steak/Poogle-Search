#ifndef STRING_OPERATION_H
#define STRING_OPERATION_H

#include <all_header.h>

#define MAX_LENGTH_X  35;

static char code[128] = { 0 };

void lower_string(char *s) {
   int c = 0;

   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
}

bool strCheck(char* str){
    for(int i=0;(str[i]!='\0');i++){
        if(!(str[i]<='z'&&str[i]>='a'))return false;
    }
    return true;
}

int isVowl(char c){
	return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int vowlNbr(char c){
	if(c == 'a') return 1;
	if(c == 'e') return 2;
	if(c == 'i') return 3;
	if(c == 'o') return 4;
	if(c == 'u') return 5;
	return 0;
}

int soundex2Score(char *c){
	char cs[7];
	cs[0] = '0'+((int)(c[0]-'a')/10);
	cs[1] = '0'+((int)(c[0]-'a')%10);
	cs[2] = c[1];
	cs[3] = c[2];
	cs[4] = c[3];

	cs[5] = '\0';
	//printf("CS :: %s\n",cs);
	int a = 0;
	
	for(int i=0;i<=4;i++){
		a += (int)(cs[i]-'0') * (int)pow(10,4-i);
	}
	return a;
}

int min(int x,int y){
    return (x>y)?y:x;
}

int min(int x, int y, int z)
{
    return min(min(x, y), z);
}

int editDistDP(char* str1, char* str2, int m, int n){
    int dp[m+1][n+1];

    for (int i=0; i<=m; i++)
    {
        for (int j=0; j<=n; j++)
        {
            if (i==0)
                dp[i][j] = j;  // Min. operations = j

            else if (j==0)
                dp[i][j] = i; // Min. operations = i

            else if (str1[i-1] == str2[j-1])
                dp[i][j] = dp[i-1][j-1];

            else
                dp[i][j] = 1 + min(dp[i][j-1],  // Insert
                                   dp[i-1][j],  // Remove
                                   dp[i-1][j-1]); // Replace
        }
    }

    return dp[m][n];
}

void lex(char str[][35],int len){
    char temp[35];
    for(int i = 0; i < len; ++i) {
        for(int j = i+1; j < len ; ++j) {
            if(strcmp(str[i], str[j]) > 0) {
                strcpy(temp, str[i]);
                strcpy(str[i], str[j]);
                strcpy(str[j], temp);
            }
        }
    }
}

void sortLen(char s[][35], int n) 
{ 
    for (int i=1 ;i<n; i++) 
    { 
        char temp[35];
        strcpy(temp,s[i]); 
        int j = i - 1; 
        int tmpLen = strlen(temp);
        int sLen = strlen(s[j]);
        while (j >= 0 && tmpLen < sLen) 
        { 
            strcpy(s[j+1],s[j]); 
            j--;
            sLen = strlen(s[j]);
        } 
        strcpy(s[j+1] , temp); 
    } 
} 

void printRes(WordX *res,int n){
    char s[35];
    for(int i = 0;i<n ;i++){
        trackRoot(res[i].link,s);
        printf("%s\n",s);
    }
}

void printLink(WordX *res,int n){
    char s[35];
    for(int i = 0;i<n ;i++){
        //trackRoot(res[i].link,s);
        printf("%d\t%p\n",i,res[i].link);
    }
}

char* soundex(const char *s)
{
	static char out[5];
	int c, prev, i;
 
	out[0] = out[4] = 0;
	if (!s || !*s) return out;
 
	out[0] = *s++;
 
	/* first letter, though not coded, can still affect next letter: Pfister */
	prev = code[(int)out[0]];
	for (i = 1; *s && i < 4; s++) {
		if ((c = code[(int)*s]) == prev) continue;
 
		if (c == -1) prev = 0;	/* vowel as separator */
		else if (c > 0) {
			out[i++] = c + '0';
			prev = c;
		}
	}
	while (i < 4) out[i++] = '0';
	return out;
}

int word2Score(char *c){
    return soundex2Score(soundex(c));
}

void add_code(const char *s, int c)
{
	while (*s) {
		code[(int)*s] = code[0x20 ^ (int)*s] = c;
		s++;
	}
}
 
void init()
{
	static const char *cls[] =
		{ "AEIOU", "", "BFPV", "CGJKQSXZ", "DT", "L", "MN", "R", 0};
	int i;
	for (i = 0; cls[i]; i++)
		add_code(cls[i], i - 1);
}

#endif