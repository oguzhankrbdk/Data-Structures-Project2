/*
 * Oguzhan KARABUDAK
 * Purpose of the program: Implementing a priority queue for ranking documents according to relevance of the given keyword.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

struct node{
    char docName[50];
    int priority;
    int index;
};

struct heap{
    struct node **arr;
    int count;
    int size;
};

typedef struct heap heap;
typedef struct heap *heapPtr;
typedef struct node node;
typedef struct node *nodePtr;
typedef struct node **nodePtrPtr;

int readFile(heapPtr heap, char *input, char *direc);
char* concat(const char *s1, const char *s2);
char* clearString(char string[]);
char *strRemove(char *str, const char *sub);
int countDoc(char *direc);
void insert(heapPtr heap, char *docName, int occurrence);
void initHeap(int heapSize, heapPtr heap);
nodePtr popMax(heapPtr heap);
int parent(int index);
int leftChild(int index);
int rightChild(int index);
void heapifyBottomToTop(heapPtr heap,int index);
void heapifyTopToBottom(heapPtr heap, int index);
void display(heapPtr heap);

//heap initialization
void initHeap(int heapSize, heapPtr heap){
	int i;
    heap->count = 0;
    heap->size = heapSize;
    heap->arr = (nodePtrPtr)malloc(sizeof(nodePtr) * heapSize);
    for(i = 0; i < heapSize; i++){
        heap->arr[i] = malloc(sizeof(node));
    }
}

//enqueue operation
void insert(heapPtr heap, char *docName, int occurrence){

    //count field holding first empty index after the last element
    if(heap->count < heap->size){
        heap->arr[heap->count]->priority = occurrence;
        strcpy(heap->arr[heap->count]->docName, docName);
        heap->arr[heap->count]->index = heap->count;

        //we heapified the heap from bottom to top for make sure it does not violate max heap property
        heapifyBottomToTop(heap, heap->count);
        heap->count++;
    }
}

//it gives parent of the given index
int parent(int index){
    return (index - 1) / 2;
}

//it gives left child of the given index
int leftChild(int index){
    return (2 * index + 1);
}

//it gives right child of the given index
int rightChild(int index){
    return (2 * index + 2);
}

void heapifyBottomToTop(heapPtr heap,int index){
    nodePtr temp;
    int tempIndex;

    //if parent node's priority less than child than swap recursively
    if(heap->arr[parent(index)]->priority < heap->arr[index]->priority){
        temp = heap->arr[parent(index)];
        tempIndex = heap->arr[parent(index)]->index;

        heap->arr[parent(index)] = heap->arr[index];
        heap->arr[tempIndex]->index = tempIndex;

        heap->arr[index] = temp;
        heap->arr[index]->index = index;
        heapifyBottomToTop(heap, parent(index));
    }
}

void heapifyTopToBottom(heapPtr heap, int index){
    nodePtr temp, max;
    int tempIndex, maxIndex, left = leftChild(index), right = rightChild(index);
    max = heap->arr[index];
    maxIndex = heap->arr[index]->index;

    //finding largest value if its exist
    if(left <= heap->count && heap->arr[left]->priority > max->priority){
        max = heap->arr[left];
        maxIndex = heap->arr[left]->index;
    }

    if(right <= heap->count && heap->arr[right]->priority > max->priority){
        max = heap->arr[right];
        maxIndex = heap->arr[right]->index;
    }

    //if its exist then we swap with parent node
    if(maxIndex != index){
        temp = heap->arr[index];
        tempIndex = heap->arr[index]->index;

        heap->arr[index] = max;
        heap->arr[index]->index = index;
        heap->arr[maxIndex] = temp;
        heap->arr[maxIndex]->index = tempIndex;

        //its continue recursively until heap property provided
        heapifyTopToBottom(heap, maxIndex);
    }
}

//dequeue operation
nodePtr popMax(heapPtr heap){
    heap->count--;
    nodePtr removedNode;
    nodePtr temp = heap->arr[heap->count];

    //decreasing heap size by one
    if(heap->count <= (heap->size + 2)){
        heap->size -= 1;

        //reallocating memory
        heap->arr = (nodePtrPtr)realloc(heap->arr, sizeof(nodePtr) * heap->size);

    }

    removedNode = heap->arr[0];
    heap->arr[0] = temp;

    //heapify to maintain heap property
    heapifyTopToBottom(heap, 0);

    //releasing unused memory
    return removedNode;
}

//reading file and constructing heap
int readFile(heapPtr heap, char *input, char *direc){
    DIR *dp;
    FILE *fp;
    struct dirent *in_file;
    char *fileName, *finalDirec;
    char *temp = malloc(100 * sizeof(char));
    char buffer[100];
    char c;
    int occurrence = 0, count = 0, relevanceFiles = 0;


    if(!(dp = opendir(direc))){
        printf("Error failed to open input directory\n");
        return 0;
    }

    while(in_file = readdir(dp)){
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))
            continue;

        fileName = in_file->d_name;
        finalDirec = concat(direc, fileName);
        occurrence = 0;
        count = 0;
        fp = fopen(finalDirec, "r");

        if(!fp){
            printf("Error failed to open file.");
            return 0;
        }

        while((c = fgetc(fp)) != EOF){
            if(c == ' ' || c == '\n' || c == '.' || c == '!' || c == ',' || c == '?'){
                count = 0;
                strcpy(buffer, clearString(buffer));
                strcpy(temp, buffer);

                if(strcmp(temp, input) == 0){
                    occurrence++;
                }

                //Resetting buffer
                memset(buffer, 0, sizeof(buffer));
            }
            else{
                buffer[count] = c;
                count++;
            }
        }

        //insertion
        insert(heap, strRemove(fileName, ".txt"), occurrence);
        if(occurrence != 0){
            relevanceFiles++;
        }
        fclose(fp);
    }

    closedir(dp);
    return  relevanceFiles;

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

    return temp;
}

void display(heapPtr heap){
    int i;
    for(i = 0; i < heap->size; i++){
        printf("Doc: %s -> %d\n", heap->arr[i]->docName, heap->arr[i]->priority);
    }
}

int main(){
    char input[20];
    char *direc = "C:/Users/oguzh/CLionProjects/projectFinal/files/";
    int i, relevantDocs;
    int docN = countDoc(direc);

    heap heap;
    nodePtr temp;

    //taking input from user
    printf("Enter input:");
    scanf("%s", &input);

    //heap initialization
    initHeap(docN, &heap);

    //constructing heap
    relevantDocs = readFile(&heap, input, direc);

    printf("\nNumber of relevant documents: %d", relevantDocs);
    printf("\nThe relevance order is: \n");

    //dequeuing top 5 element with highest priority
    for(i = 0; i < 5; i++){
        temp = popMax(&heap);
        if(temp->priority != 0){
            printf("\t%d. %s %d\n", i+1, temp->docName, temp->priority);
        }
    }

}
