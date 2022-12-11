#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define hTree(a) ((a!=NULL)?(a->h):(-1))

#define MAIN_TXT  "ns.txt"
#define DATA_TXT  MAIN_TXT
//#define TIME_MOD

#ifdef TIME_MOD
#define DATA_TXT "delited.txt"
#endif

#define STEP 500000
#define STRING_SIZE 1024

typedef struct _tree {
	struct _tree* l;
	char* el;
	struct _tree* r;
	int h;
} Tree;

void heightTree(Tree* t){
    if (t->l==0 && t->r==0){
        t->h=0;
    }else{
        int leftHeight  = (t->l!=NULL)?(t->l->h):0;
        int rightHeight = (t->r!=NULL)?(t->r->h):0;
        t->h = 1+((leftHeight>rightHeight)?leftHeight:rightHeight);
    }
}

void rotateLeft(Tree** t){
    Tree* newLeft = *t;
    Tree* newLeftRight = (*t)->r->l;
    *t=(*t)->r;
    (*t)->l=newLeft;
    (*t)->l->r=newLeftRight;
    heightTree((*t)->l);
    heightTree((*t));
}


void rotateRight(Tree** t){
    Tree* newRight = *t;
    Tree* newRightLeft = (*t)->l->r;
    *t=(*t)->l;
    (*t)->r=newRight;
    (*t)->r->l=newRightLeft;
    heightTree((*t)->r);
    heightTree(*t);
}


int diffHeights(Tree* t){
    if (t==NULL) return 0;
    int leftHeight  = (t->l!=NULL)?((t->l->h)+1):0;
    int rightHeight = (t->r!=NULL)?((t->r->h)+1):0;
    return leftHeight - rightHeight;
}

Tree* balaceTree(Tree* t){
    int diffRoot = diffHeights(t);
    int diffLeft = diffHeights((t)->l);
    int diffRight = diffHeights((t)->r);
    Tree** tr = &t;

    if (diffRoot == -2 && diffRight<=0)
        rotateLeft(tr);
    if (diffRoot == 2 && diffLeft>=0)
        rotateRight(tr);
    if (diffRoot == -2 && diffRight == 1){
        rotateRight((&(t)->r));
        rotateLeft(tr);
    }
    if (diffRoot == 2 && diffLeft == -1){
        rotateLeft((&(t)->l));
        rotateRight(tr);
    }
    return t;
}


Tree* initTree(Tree* t, char* str){
    if (!t){
        t = malloc(sizeof(Tree));
        *t = (Tree){NULL,strdup(str),NULL,0};
        return t;
    }else{
        if ((strcmp((t)->el,str)>=0))
            (t)->l = initTree(((t)->l), str);
        else if ((strcmp((t)->el,str)<0))
            (t)->r = initTree(((t)->r), str);

        heightTree(t);
        t=balaceTree(t);
        return t;
    }
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

void freeTree(Tree* t){
    if (t->l!=NULL)
        freeTree(t->l);
    if (t->r!=NULL)
        freeTree(t->r);

    free(t->el);
    free(t);
}


double avl(){
    FILE* input = fopen(DATA_TXT, "r");
    char str[STRING_SIZE];
    Tree* root = NULL;
    while( (fscanf(input,"%s", str) == 1)){
        root = initTree(root, str);
    }
    #ifndef TIME_MOD
    prefix(root);
    #endif

    freeTree(root);
    return 0;
}

int main(){
    #ifdef TIME_MOD
    struct timespec start, end;
    FILE* result = fopen("result_avl.txt","w");

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
        avl();
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