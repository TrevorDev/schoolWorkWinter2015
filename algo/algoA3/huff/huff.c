#include "dlxlib.h"
#define ASCII_CHARS 128

typedef struct charCount {
   char c;
   int count;
} charCount;

void freeBt(binaryTree * b){
    if(b!=NULL){
        freeBt(b->left);
        freeBt(b->right);
        free(b->data);
        free(b);
    }
}

int getHeapL(binaryTree ** heapAr, int size, int curPos){
    int retPos = curPos*2+1;
    if(retPos>=size){
        return -1;
    }
    return retPos;
}

int getHeapR(binaryTree ** heapAr, int size, int curPos){
    int retPos = curPos*2+2;
    if(retPos>=size){
        return -1;
    }
    return retPos;
}

void printHeap(binaryTree ** heapAr, int size){
    for(int i=0;i<size;i++){
        charCount * cc = heapAr[i]->data;
        print("%c,%d", cc->c, cc->count);
    }
}


int heapify(binaryTree ** heapAr, int size, int pos){
    int l = getHeapL(heapAr, size, pos);
    int r = getHeapR(heapAr, size, pos);
    if(l < 0 && r < 0){
        return 0;
    }
    charCount * cc = heapAr[pos]->data;
    int minPos;
    if(r >= 0){
        charCount * lc = heapAr[l]->data;
        charCount * rc = heapAr[r]->data;
        if(lc->count <= rc->count){
            minPos = l;
        }else{
            minPos = r;
        }
    }else{
        minPos = l;
    }
    

    charCount * minChild = heapAr[minPos]->data;
    if(minChild->count < cc->count){
        binaryTree * tmp = heapAr[pos];
        heapAr[pos] = heapAr[minPos];
        heapAr[minPos] = tmp;
        heapify(heapAr, size, minPos);
        return 1;
    }
    return 0;
    //print("%c %d", cc->c, cc->count);
}

binaryTree * removeMin(binaryTree ** heapAr, int * size){
    binaryTree * ret = heapAr[0];
    heapAr[0] = heapAr[(*size)-1];
    heapAr[(*size)-1] = ret;
    (*size)--;
    heapify(heapAr, *size, 0);
    return ret;
}

void upHeap(binaryTree ** heapAr, int size, int pos){
    int x = heapify(heapAr, size, pos);
    if(x){
        upHeap(heapAr, size, (pos+1)/2 - 1);
    }
}

void insertTree(binaryTree ** heapAr, int * size, binaryTree * add){
    heapAr[*size] = add;
    (*size)++;
    if(*size > 1){
        upHeap(heapAr, *size, (*size)/2 - 1);//upheap the parent
    }
}

binaryTree * combineTrees(binaryTree * a, binaryTree * b){
    charCount * aData = a->data;
    charCount * bData = b->data;
    binaryTree * ret = createBinaryTree();
    ret->left = a;
    ret->right = b;
    charCount * cc = malloc(sizeof(charCount));
    ret->data = cc;
    cc->c = 0;
    cc->count = aData->count + bData->count;
    return ret;
}

int printBtLevel(binaryTree * bt, int lvl){
    if(bt == NULL){
        return 0;
    }else if(lvl == 0){
        charCount * cc = (charCount *)bt->data;
        printf("%c %d -- ", cc->c, cc->count);
        return 1;
    }else{
        lvl--;
        int l = printBtLevel(bt->left, lvl);
        int r = printBtLevel(bt->right, lvl);
        return l+r;
    }
    
}
void printBt(binaryTree * bt){
    int lvl = 0;
    int ret = 1;
    while(ret > 0){
        ret = printBtLevel(bt, lvl);
        printf("\n");
        lvl++;
    }
}

void printBtLeafs(binaryTree * bt, int bitLeng, int bits){
    if(bt->left == NULL && bt->right == NULL){
        charCount * cc = (charCount *)bt->data;
        printf("%c: ", cc->c);
        for(int i=0;i<bitLeng;i++){
             if (bits & 1){
                printf("1");
            }else{
                printf("0");
            }
            bits >>= 1;
        }
        printf("\n");
    }else{
        if(bt->left != NULL){
            printBtLeafs(bt->left, bitLeng+1, bits);
        }
        if(bt->right != NULL){
            printBtLeafs(bt->right, bitLeng+1, bits+(pow(2, bitLeng)));
        }

    }
}

int main(int argc, char **argv)
{
    int countAr[ASCII_CHARS];
    for(int i = 0;i<ASCII_CHARS;i++){
        countAr[i] = 0;
    }

    int heapSizeNeeded = 0;
    for(int c = getchar();;c = getchar()){
        if(c==EOF){
            break;
        }
        if(countAr[c] == 0){
            heapSizeNeeded++;
        }
        countAr[c]++;
        //print("%c", c);
    }

    vector * heap = createVector();
    heap->ar = malloc(heapSizeNeeded*sizeof(binaryTree *));
    binaryTree ** heapAr = (binaryTree **)(heap->ar);
    heap->size = heapSizeNeeded;
    int heapPos = 0;
    for(int i = 0;i<ASCII_CHARS;i++){
        if(countAr[i] > 0){
            charCount * cc = malloc(sizeof(charCount));
            cc->c = i;
            cc->count = countAr[i];

            binaryTree * bt = createBinaryTree();
            bt->data = cc;
            heapAr[heapPos] = bt;
            heapPos++;
        }
    }
    //printHeap(heapAr, heap->size);
    int heapifyStart = heap->size/2 - 1;
    for(int i = heapifyStart;i>=0;i--){
        heapify(heapAr, heap->size, i);
    }
    //print("\n");
    //printHeap(heapAr, heap->size);

    while(heap->size > 1){
        binaryTree *  first = removeMin(heapAr, &(heap->size));
        binaryTree *  second = removeMin(heapAr, &(heap->size));
        binaryTree * com = combineTrees(first, second);
        insertTree(heapAr, &(heap->size), com);
        // print("\n");
        // printHeap(heapAr, heap->size);
    }
    //print("\n");
    //printBt(heapAr[0]);
    printBtLeafs(heapAr[0], 0, 0);
    //print("%d %d", countAr['e'], heapSizeNeeded);
    for(int i = 0;i<heap->size;i++){
        freeBt(heapAr[i]);
    }
    free(heap->ar);
    free(heap);
    return 0; 
}

