#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct node{
	char docName[50];
	int priority;
	int index;
};

struct heap{
	struct node *arr;
	int size;
	int count;
};

typedef struct heap heap;
typedef struct heap *heapPtr;

typedef struct node node;
typedef struct node *nodePtr;


int readFile(char *input, char *direc);
char* concat(const char *s1, const char *s2);
char* clearString(char string[]);
char *strRemove(char *str, const char *sub);
int countDoc(char *direc);
heapPtr createHeap(int heapSize);
nodePtr createNode(char *docName, int occurrence);
int parent(int index);
int leftChild(int index);
int rightChild(int index);
void heapifyBottomToTop(heapPtr heap,int index);


heapPtr createHeap(int heapSize){
	heapPtr temp = (heapPtr)malloc(sizeof(heap));
	temp->size = heapSize;
	temp->arr = (nodePtr)malloc((heapSize) * sizeof(node));
	temp->count = 0;
	
	return temp;
}

nodePtr createNode(char *docName, int occurrence){
	nodePtr temp = malloc(sizeof(node));
	temp->priority = occurrence;
	strcpy(temp->docName, docName);
	temp->index = 0;
	
	return temp;
}

void insert(heapPtr heap, nodePtr node){
	if(heap->count <= heap->size){
		heap->arr[count] = node;
		node->index = count;
		heapifyBottomToTop(heap, heap->count);
		heap->count++;
	}
}

int parent(int index){
	if(index == 0){
		return 0;
	}
	return (index - 1) / 2;
}

int leftChild(int index){
	return (2 * index + 1);
}

int rightChild(int index){
	return (2 * index + 2);
}

void heapifyBottomToTop(heapPtr heap,int index){
	int temp, tempIndex;
	
	if(heap->arr[parent(index)]->priority < heap->arr[index]->priority){
		temp = heap->arr[parent(index)];
		tempIndex = heap->arr[parent(index)]->index;
		
		heap->arr[parent(index)] = heap->arr[index];
		heap->arr[parent(index)]->index = heap->arr[index]->index;
		
		heap->arr[index] = temp;
		heap->arr[index]->index = tempIndex;
		heapify(heap, parent(index));
	}
}

void heapifyTopToBottom(heapPtr heap, int index){
	
}

	
int readFile(char *input, char *direc){
	DIR *dp;
	FILE *fp;
	struct dirent *in_file;
	char *fileName, *finalDirec;
	char *temp = malloc(100 * sizeof(char));
	char buffer[100];
	char c;
	int i = 0;
	int docN = countDoc(direc);
	
	
	
	if(!(dp = opendir(direc))){
		printf("Error failed to open input directory\n");
		return 0;
	}

	while(in_file = readdir(dp)){
		if (!strcmp (in_file->d_name, "."))
        	continue;
       	if (!strcmp (in_file->d_name, ".."))    
        	continue;
    
		printf("%s  Input word count: ", in_file->d_name);
		fileName = in_file->d_name;
		finalDirec = concat(direc, fileName);
		
		fp = fopen(finalDirec, "r");
		
		if(!fp){
			printf("Error failed to open file.");
			return 0;
		}
		i = 0;
		
		int count = 0; 
		while((c = fgetc(fp)) != EOF){ 
			if(c == ' ' || c == '\n' || c == '.' || c == '!' || c == ',' || c == '?'){ 
				count = 0;
				strcpy(buffer, clearString(buffer));
				strcpy(temp, buffer);
				if(strcmp(temp, input) == 0){
					i++;
				}
				//Resetting buffer
				memset(buffer, 0, sizeof(buffer));
			} 
			else{ 
				buffer[count] = c;
				count++;
			} 
		} 
			
		printf("%d\n", i);	
		
		fclose(fp);
	}
	printf("NUMBER OF DOCS: %d", docN);
	closedir(dp);

}

int countDoc(char *direc){
	DIR *dp;
	struct dirent *in_file;
	
	int count = 0;
	if(!(dp = opendir(direc))){
		printf("Error failed to open input directory\n");
		return 0;
	}
	
	while(in_file = readdir(dp)){
		if (!strcmp (in_file->d_name, "."))
        	continue;
       	if (!strcmp (in_file->d_name, ".."))    
        	continue;
        	
		count++;
		
	}
		
	closedir(dp);
	return count;
}

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *strRemove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

char* clearString(char string[]){
    char str[100];
    char* temp = malloc(100 * sizeof(char));
    int i, j;

    strcpy(str, string);
    
    /*Converting mixed case string to lower case string*/
    for(i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    
    /*cleaning unwanted characters from string*/
	for(i = 0; str[i] != '\0'; ++i){
		while (!( (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '\0') ){
			for(j = i; str[j] != '\0'; ++j){
				str[j] = str[j+1];
			}
			str[j] = '\0';
		}
	}
	strcpy(temp, str);
   	strcpy(temp, strRemove(temp, "pros"));
   
    
   /*	size_t len = strlen("pros");

    while ((temp = strstr(temp, "pros")) != NULL) {
        memmove(temp, temp + len, strlen(temp + len) + 1);
    }
    //strcpy(str, temp);*/
    
    
    return temp;
}

int main(){
	char input[20];
	char *direc = "C:/Users/oguzh/CLionProjects/project2/files/";
		
	printf("enter input: ");
	
	scanf("%s", &input);
	readFile(input, direc);
}


