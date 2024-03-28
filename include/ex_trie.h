#ifndef EX_TRIE_H
#define EX_TRIE_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/time.h>
#include <all_header.h>

#define alpha_size 26
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

#define ROOT  2
#define LEAF  1
#define EMPTY 0
#define NOT_USED -1

struct ExTrie{
	int state;
	char letter;
	ExTrie *parent;
	ExTrie *children[alpha_size];
};

struct WordX{
	int score;
	ExTrie *link;
};

ExTrie *createNode(int state){
	ExTrie *extrie = (ExTrie*) malloc(sizeof(ExTrie));
	extrie->state = state;
	extrie->parent = NULL;
	if(state == ROOT)
		extrie->letter='\0';
	for(int i=0;i<alpha_size;i++){
		extrie->children[i] = NULL;
		//extrie->children[i]->letter = 'a';
	}
		
	return extrie;
}

ExTrie* insert_word(ExTrie *ext, char *c){
	ExTrie *extP;
	int i = 0;
	for(i = 0;c[i];i++){
		if(ext->children[CHAR_TO_INDEX(c[i])]==NULL)
			ext->children[CHAR_TO_INDEX(c[i])] = createNode(EMPTY);
		ext->parent = (!i)?NULL:extP;
		extP = ext;
		ext = ext->children[CHAR_TO_INDEX(c[i])];
		//printf("%p : %p : %c\n",extP,extP->parent,c[i]);
	}
	ext->state = LEAF;
	ext->parent = extP;
	return ext;
}

bool isLeafNode(struct ExTrie* root)
{
	return (root->state==LEAF)?1:0;
}

int suggestion(ExTrie *ext, char c[], int level, WordX *res, int MaxS ,int *getW){
	if(ext->state == LEAF && level > 2 && *getW<MaxS){
		c[level+1] = '\0';
		//strcpy(res[*getW],c);
		res[*getW].link = ext;
		res[*getW].score = level;
		*getW = *getW + 1;
		//printf("%s\n",c);
		return 1;
	}
	for(int i = 0; c[i] && level <= i ; i++){
		if(ext->children[CHAR_TO_INDEX(c[i])]==NULL)
			return 0;
		ext = ext->children[CHAR_TO_INDEX(c[i])];
		level = i;
		//printf("%d\n",level);
	}
	for(int i=0;i<alpha_size;i++){
		if(ext->children[i]){
			c[level+1] = i+'a';
			//printf("#%s\n",c);
			suggestion(ext->children[i],c,level+1,res,MaxS,getW);
		}
	}
	return 0;
}

/*
void display(TrieNode* root, char str[], int level)
{	
	if (isLeafNode(root))
	{
		str[level] = '\0';
		printf("%s\n",str);
	}
	int i;
	for (i = 0; i < alpha_size; i++)
	{
		if (root->children[i])
		{
			str[level] = i + 'a';
			display(root->children[i], str, level + 1);
		}
	}
}
*/

int search_word(ExTrie *ext, char *c){
	for(int i=0;c[i];i++){
		if(ext->children[CHAR_TO_INDEX(c[i])]==NULL)
			return 0;
		ext = ext->children[CHAR_TO_INDEX(c[i])];
	}
	return (ext->state==LEAF)?1:0;
}

void trackRoot(ExTrie *ext, char *c){
	ExTrie *parent;
	char tmp[35];
	int level = 0;
	while(true){
		if(ext->state == ROOT)break;
		//printf("# %p :: %p\n",ext,ext->parent);
		parent = ext->parent;
		
		for(int i=0;i<alpha_size;i++)
			if(parent->children[i] == ext){
				tmp[level] = 'a'+i;
				level++;
				break;
			}
		
		//tmp[level] =  ext->letter;
		//level++;
		ext = parent;
	}
	tmp[level] = '\0';
	c[level] = '\0';
	for(int i = level-1;i>=0;i--)
		c[i] = tmp[level-1-i];
}

#endif