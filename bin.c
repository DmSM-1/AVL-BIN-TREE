#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAIN_TXT  "ns.txt"
#define DATA_TXT  MAIN_TXT
#define TIME_MOD

#ifdef TIME_MOD
#define DATA_TXT "delited.txt"
#endif

#define STEP 500000
#define STRING_SIZE 1024

typedef struct Bin
{
    char* el;
    struct Bin* l;
    struct Bin* r;
}Tree;

int pr(Tree* b){
    if (!b) return 0;
    putchar('(');
    int el = pr(b->l);
    printf(" %s ", b->el);
    el+=pr(b->r)+1;
    putchar(')');
    return el;
}

int printTree(Tree* t) {
    int n = 0;
	if ( t ) {
		//printf("(");
		n+=printTree(t->l);
		printf("%s\n", t->el);
		n+=printTree(t->r);
		//printf(")");
        n++;
	}
    return n;
}


char prefixString(char* prefix, char* string){
    while (*prefix==*string && *prefix!=0){
        prefix++;
        string++;
    }
    return (*prefix==0);
}

int prefixSearch(Tree* t, char* prefix, char wasFound){
    if (t==0) return 0;

    if (prefixString(prefix, t->el)){
        prefixSearch(t->l, prefix, 1);
        for(char* i = t->el; *i!=0; i++){
            if (*i=='\n') break;
            putchar(*i);
        }
        putchar('\n');
        prefixSearch(t->r,prefix, 1);
    }else if (strcmp(prefix,t->el)<=0){
        if (t->l==NULL){
            if (!wasFound)
            printf("not found\n");
        }
        else
            prefixSearch(t->l, prefix, wasFound);
    }else{
        if (t->r==NULL){
            if (!wasFound)
            printf("not found\n");
        }
        else
            prefixSearch(t->r, prefix, wasFound);
    }
}

void prefix(Tree* root){
    char prefix[STRING_SIZE];
    while(1){
        system("clear");
        printf("Enter prefix: ");    
        scanf("%s", prefix);
        printf("Searching results:\n");
        prefixSearch(root, prefix, 0);
        printf("continue? [y/n]\n");
        scanf("%s", prefix);
        if (*prefix == 'n') break;
    }
}

void* ft(Tree* b){
    if(!b) return NULL;
    ft(b->l);
    ft(b->r);
    free(b->el);
    free(b);
}


int bin(){
    FILE* f = fopen(DATA_TXT, "r");
    char b[1024];

    fscanf(f,"%s", b);
    Tree* t = malloc(sizeof(Tree));
    *t = (Tree){strdup(b), NULL, NULL};

    Tree* s = t;
    Tree* s1 = NULL;
    
    while (fscanf(f,"%s", b)==1){
        s = t;
        s1 = t;
        while(s){
             s1 = s;
             s = strcmp(s->el, b)>0 ? s->l : s->r;
        }
    s = malloc(sizeof(Tree));
    *s = (Tree){strdup(b), NULL, NULL};    

    if (strcmp(s1->el, b)>0) s1->l=s;
    else s1->r=s;
    } 
    #ifndef TIME_MOD
    prefix(t);
    #endif

    ft(t);
    return 0;
}

int main(){
    #ifdef TIME_MOD
    struct timespec start, end;
    FILE* result = fopen("result_bin.txt","w");

    FILE* input = fopen(MAIN_TXT, "r");
    char str[STRING_SIZE];
    int breakPoint = 0;
    int lens = 0;

    while(!breakPoint){
        FILE* output = fopen(DATA_TXT, "a");
        for(int i = 0;i<STEP;i++){
            if (fscanf(input,"%s", str)==0){
                breakPoint=1;
                break;
            }
            fprintf(output,"%s\n",str);
        }
        fclose(output);

        double t = 0;
        for (int k = 0; k<5; k++){
        clock_gettime(CLOCK_REALTIME, &start);
        #endif
        bin();
        #ifdef TIME_MOD
        clock_gettime(CLOCK_REALTIME, &end);
        t += (end.tv_sec - start.tv_sec)+(double)(end.tv_nsec - start.tv_nsec)/1e9;
        }
        lens+=STEP;

        fprintf(result,"%d %lf\n", lens, t/5);
        printf("%d %lf\n", lens, t/5);
    }
    fclose(input);
    fclose(result);
    #endif

    return 0;
}