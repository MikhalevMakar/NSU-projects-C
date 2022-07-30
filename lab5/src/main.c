#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
enum { sizeUnsignedChar = 256, countBit = 7, countRead = 1, emptyVert = 1 };

typedef struct TTree {
    unsigned char Symbol;
    int Count;
    struct TTree* Right;
    struct TTree* Left;
}TTree;

typedef struct PriorityQ {
    TTree* Tree;
    struct PriorityQ* Next;
}PriorityQ;

void DestroyTree(TTree* tree);

void FreeMatrixCode(unsigned char** matrixCode);

void FreeEncoder(PriorityQ* root, int* codeChar, unsigned char** matrixCode);

void FreeDecoder(TTree* tree);

void CloseFile(FILE* in, FILE* out);

int* CountingFrequencyOfCharacters(FILE* file);

PriorityQ* CreateRoot();

PriorityQ* AddQ(int* codeChar, int index);

void  Push(PriorityQ* parent, PriorityQ* child);

TTree* Pop(PriorityQ* parent);

PriorityQ* CreateQ(int* codeChar);

PriorityQ* CreateNewTree(TTree* child1, TTree* child2);

void CreateTree(PriorityQ* parent);

void BufferCodeCompleted(bool statusWriteText, unsigned char* leafCode, char* countLeafCode, FILE* fileOut);

void FillMatrix(TTree* tree, unsigned char** matrix, unsigned char* curSequence, int sizeCode);

void CreateSeqToTree(FILE* fileOut, TTree* tree, unsigned char* seqTree, char* sizeSecTree, int sizeSeqCode, unsigned  char** matrixCode, unsigned char* curSeqCode, bool statusWriteTree);

TTree* CreatingEmptyVertex(TTree* root);

unsigned char** CreateTreeToFileOut(bool statusWriteTree, TTree* tree, FILE* fileOut);

void CreateCodeToFileOut(unsigned char** matrixCode, unsigned char indexSymbol, unsigned char* curSymbol, char* curCntBit, FILE* fileOut);

void WriteCodeToFileOut(unsigned char** matrixCode, FILE* fileIn, FILE* fileOut);

void CreateEncoder(FILE* fileIn, FILE* fileOut);

TTree* Node(unsigned char symbol, TTree* Left, TTree* Right);

void BufferSymbol(unsigned char* symbol, int* index, FILE* fileIn);

TTree* CreateDecoderTree(int* index, unsigned char* curSymbol, FILE* fileIn);

void DecodeSequencion(TTree* root, int countNonZero, FILE* fileIn, FILE* fileOut);

void CreateDecoder(FILE* fileIn, FILE* fileOut);

int main() {

    FILE* fileIn = fopen("in.txt", "rb");
    assert(fileIn != NULL);

    FILE* fileOut = fopen("out.txt", "wb");
    assert(fileOut != NULL);

    unsigned char lineOfWork = 0;

    lineOfWork = fgetc(fileIn);

    if (lineOfWork == 'c') {
        CreateEncoder(fileIn, fileOut);
    }

    else {
        CreateDecoder(fileIn, fileOut);
    }

    CloseFile(fileIn, fileOut);
    return 0;
}

void DestroyTree(TTree* tree) {
    TTree* rightChild = tree->Right;
    TTree* leftChild = tree->Left;

    free(tree);

    if (leftChild != NULL) {
        DestroyTree(leftChild);
    }

    if (rightChild != NULL) {
        DestroyTree(rightChild);
    }
}

void FreeMatrixCode(unsigned char** matrixCode) {
    for (int i = 0; i < sizeUnsignedChar; ++i) {
        free(matrixCode[i]);
    }

    free(matrixCode);
}

void FreeEncoder(PriorityQ* root, int* codeChar, unsigned char** matrixCode) {
    free(codeChar);
    FreeMatrixCode(matrixCode);

    DestroyTree(root->Next->Tree);

    free(root->Next);
    DestroyTree(root->Tree);
    free(root);
}

void FreeDecoder(TTree* tree) {
    DestroyTree(tree);
}

void CloseFile(FILE* in, FILE* out) {
    fclose(in);
    fclose(out);
}

int* CountingFrequencyOfCharacters(FILE* file) {
    int* codeChar = calloc(sizeUnsignedChar, sizeof(int));
    assert(codeChar != NULL);

    unsigned char indexSymbol = 0;

    while (fread(&indexSymbol, sizeof(unsigned char), countRead, file) == countRead) {
        codeChar[indexSymbol]++;
    }

    return codeChar;
}

PriorityQ* CreateRoot() {
    PriorityQ* queue = calloc(1, sizeof(PriorityQ));
    assert(queue != NULL);

    queue->Tree = malloc(sizeof(TTree));
    assert(queue->Tree != NULL);

    queue->Tree->Symbol = emptyVert;
    queue->Tree->Right = NULL;
    queue->Tree->Left = NULL;
    queue->Next = NULL;

    return queue;
}


PriorityQ* AddQ(int* codeChar, int index) {
    PriorityQ* child = calloc(1, sizeof(PriorityQ));
    assert(child != NULL);

    child->Tree = calloc(1, sizeof(TTree));
    assert(child->Tree != NULL);

    child->Tree->Count = codeChar[index];

    child->Tree->Symbol = index;

    child->Tree->Right = NULL;
    child->Tree->Left = NULL;
    child->Next = NULL;

    return child;
}

void  Push(PriorityQ* parent, PriorityQ* child) {
    PriorityQ* curPtr = parent;
    while (curPtr->Next != NULL) {
        if (child->Tree->Count < curPtr->Next->Tree->Count) {
            child->Next = curPtr->Next;
            curPtr->Next = child;
            return;
        }

        curPtr = curPtr->Next;
    }

    curPtr->Next = child;
}

TTree* Pop(PriorityQ* parent) {
    PriorityQ* curPtr = parent->Next;
    TTree* root = curPtr->Tree;
    parent->Next = curPtr->Next;

    free(curPtr);
    return root;
}

PriorityQ* CreateQ(int* codeChar) {
    PriorityQ* parent = CreateRoot();

    for (int i = 0; i < sizeUnsignedChar; ++i) {
        if (codeChar[i] != 0) {
            PriorityQ* child = AddQ(codeChar, i);
            Push(parent, child);
        }
    }

    return parent;
}

PriorityQ* CreateNewTree(TTree* child1, TTree* child2) {
    PriorityQ* parent = CreateRoot();
    parent->Tree->Count = child1->Count + child2->Count;

    if (child1->Count < child2->Count) {
        parent->Tree->Left = child1;
        parent->Tree->Right = child2;
    }

    else {
        parent->Tree->Left = child2;
        parent->Tree->Right = child1;
    }

    return parent;
}

void CreateTree(PriorityQ* parent) {
    while (parent->Next && parent->Next->Next) {
        TTree* child1 = Pop(parent);
        TTree* child2 = Pop(parent);
        PriorityQ* tree = CreateNewTree(child1, child2);
        Push(parent, tree);
    }
}

void BufferCodeCompleted(bool statusWriteText, unsigned char* leafCode, char* countLeafCode, FILE* fileOut) {
    if (*countLeafCode == 8) {
        *countLeafCode = 0;

        if (statusWriteText == true) {
            fprintf(fileOut, "%c", *leafCode);
        }
        *leafCode = 0;
    }
}

void FillMatrix(TTree* tree, unsigned char** matrix, unsigned char* curSequence, int sizeCode) {
    matrix[tree->Symbol] = calloc(sizeCode + 1, sizeof(unsigned char));
    assert(matrix[tree->Symbol] != NULL);

    for (int i = 0; i < sizeCode; ++i) {
        matrix[tree->Symbol][i] = curSequence[i];
    }
    matrix[tree->Symbol][sizeCode] = '\0';
}

void CreateSeqToTree(FILE* fileOut, TTree* tree, unsigned char* seqTree, char* sizeSecTree, int sizeSeqCode, unsigned  char** matrixCode, unsigned char* curSeqCode, bool statusWriteTree) {

    BufferCodeCompleted(statusWriteTree, seqTree, sizeSecTree, fileOut);

    if (tree->Left == NULL) {
        FillMatrix(tree, matrixCode, curSeqCode, sizeSeqCode);
        *seqTree |= 128 >> (*sizeSecTree);
        (*sizeSecTree)++;

        for (int i = 0; i <= countBit; ++i) {

            BufferCodeCompleted(statusWriteTree, seqTree, sizeSecTree, fileOut);

            if (((tree->Symbol << i) & 128) == 128) {
                *seqTree |= (128 >> *sizeSecTree);
            }

            (*sizeSecTree)++;

        }

        BufferCodeCompleted(statusWriteTree, seqTree, sizeSecTree, fileOut);

        return;
    }

    else {
        curSeqCode[sizeSeqCode] = '0';

        (*sizeSecTree)++;

        CreateSeqToTree(fileOut, tree->Left, seqTree, sizeSecTree, sizeSeqCode + 1, matrixCode, curSeqCode, statusWriteTree);

        curSeqCode[sizeSeqCode] = '1';

        if (tree->Right == NULL) {
            return;
        }

        CreateSeqToTree(fileOut, tree->Right, seqTree, sizeSecTree, sizeSeqCode + 1, matrixCode, curSeqCode, statusWriteTree);
    }
}

TTree* CreatingEmptyVertex(TTree* root) {
    TTree* tree = calloc(1, sizeof(TTree));
    assert(tree != NULL);

    tree->Left = root;
    tree->Right = NULL;
    tree->Symbol = emptyVert;
    return tree;
}

unsigned char** CreateTreeToFileOut(bool statusWriteTree, TTree* tree, FILE* fileOut) {
    unsigned char* curSequenceCode = calloc(sizeUnsignedChar, sizeof(unsigned char));

    assert(curSequenceCode != NULL);

    unsigned char** matrixCode = calloc(sizeUnsignedChar, sizeof(unsigned char*));

    assert(matrixCode != NULL);

    for (int i = 0; i < sizeUnsignedChar; ++i) {
        matrixCode[i] = NULL;
    }

    char countSequenceTree = 0;

    unsigned char sequenceTree = 0;

    CreateSeqToTree(fileOut, tree, &sequenceTree, &countSequenceTree, 0, matrixCode, curSequenceCode, statusWriteTree);

    if (statusWriteTree == false) {
        fprintf(fileOut, "%c%c", 128 | (tree->Left->Symbol >> 1), 128 & (tree->Left->Symbol << 7));
    }

    if (countSequenceTree != 0 && statusWriteTree == true) {
        fprintf(fileOut, "%c", sequenceTree);
    }

    free(curSequenceCode);

    return matrixCode;
}

void CreateCodeToFileOut(unsigned char** matrixCode, unsigned char indexSymbol, unsigned char* curSymbol, char* curCntBit, FILE* fileOut) {
    int index = 0;

    while (matrixCode[indexSymbol][index] != '\0') {

        if (matrixCode[indexSymbol][index] == '1') {
            *curSymbol += (128 >> *curCntBit);
        }

        (*curCntBit)++;
        index++;

        if (*curCntBit == 8) {
            fprintf(fileOut, "%c", *curSymbol);
            *curSymbol = 0;
            *curCntBit = 0;
        }
    }
}

void WriteCodeToFileOut(unsigned char** matrixCode, FILE* fileIn, FILE* fileOut) {
    fseek(fileIn, 1, SEEK_SET);

    unsigned char curSymbol = 0;
    unsigned char symbol = 0;
    char curCntBit = 0;

    while (fread(&curSymbol, sizeof(unsigned char), countRead, fileIn) == countRead) {

        if (matrixCode[curSymbol] != NULL) {
            CreateCodeToFileOut(matrixCode, curSymbol, &symbol, &curCntBit, fileOut);
        }
    }

    if (curCntBit != 0) {
        fprintf(fileOut, "%c", symbol);
        fseek(fileOut, 0, SEEK_SET);
        fprintf(fileOut, "%c", curCntBit);
    }
}

void CreateEncoder(FILE* fileIn, FILE* fileOut) {
    char nonZero = 8;
    fprintf(fileOut, "%c", nonZero);

    int* codeChar = CountingFrequencyOfCharacters(fileIn);

    PriorityQ* root = CreateQ(codeChar);
    CreateTree(root);

    if (root->Next == NULL) {
        DestroyTree(root->Tree);
        free(root);
        free(codeChar);
        return;
    }

    bool statusWriteTree = true;

    if (root->Next->Tree->Symbol != emptyVert) {
        root->Next->Tree = CreatingEmptyVertex(root->Next->Tree);
        statusWriteTree = false;
    }

    unsigned char** matrixCode = CreateTreeToFileOut(statusWriteTree, root->Next->Tree, fileOut);

    WriteCodeToFileOut(matrixCode, fileIn, fileOut);

    FreeEncoder(root, codeChar, matrixCode);
}

TTree* Node(unsigned char symbol, TTree* Left, TTree* Right) {
    TTree* root = calloc(1, sizeof(TTree));
    assert(root != NULL);

    root->Symbol = symbol;
    root->Count = 0;
    root->Right = Right;
    root->Left = Left;
    return root;
}

void BufferSymbol(unsigned char* symbol, int* index, FILE* fileIn) {
    if ((*index) >= 8) {
        (*index) = (*index) - 8;
        *symbol = fgetc(fileIn);
    }
}

TTree* CreateDecoderTree(int* index, unsigned char* curSymbol, FILE* fileIn) {
    if ((((*curSymbol) << (*index)) & 128) == 128) {
        (*index)++;
        BufferSymbol(curSymbol, index, fileIn);

        unsigned char symbol = 0;

        symbol |= ((*curSymbol) << (*index));
        (*index) += 8;

        BufferSymbol(curSymbol, index, fileIn);

        symbol |= ((*curSymbol) >> (8 - (*index)));

        TTree* root = Node(symbol, NULL, NULL);

        return root;
    }

    else {
        (*index)++;

        BufferSymbol(curSymbol, index, fileIn);

        TTree* leftChild = CreateDecoderTree(index, curSymbol, fileIn);

        TTree* rightChild = CreateDecoderTree(index, curSymbol, fileIn);

        TTree* tree = Node(emptyVert, leftChild, rightChild);

        return tree;
    }
}

void DecodeSequencion(TTree* root, int countNonZero, FILE* fileIn, FILE* fileOut) {
    bool statusReading = true;
    TTree* tree = root;
    int count = 8;
    
    unsigned char firstReadByte = fgetc(fileIn);
    unsigned char secondReadByte;
    
    while (statusReading == 1) {
        if (fread(&secondReadByte, sizeof(unsigned char), countRead, fileIn) != countRead) {
            statusReading = false;
            count = countNonZero;
        }

        for (int i = 0; i < count; ++i) {

            if (((firstReadByte << i) & 128) == 128) {
                tree = tree->Right;
            }

            else {
                tree = tree->Left;
            }

            if (tree == NULL) {
                return;
            }

            if (tree->Left == NULL && tree->Right == NULL) {
                fprintf(fileOut, "%c", tree->Symbol);
                tree = root;
            }
        }
        firstReadByte = secondReadByte;
    }
}

void CreateDecoder(FILE* fileIn, FILE* fileOut) {
    char countByte = 0;
    unsigned char symbol = 0;
    countByte = fgetc(fileIn);

    if (fscanf(fileIn, "%c", &symbol) != 1) {
        return;
    }

    int index = 0;

    TTree* root = CreateDecoderTree(&index, &symbol, fileIn);

    if (root->Symbol != emptyVert) {
        root = CreatingEmptyVertex(root);
    }

    DecodeSequencion(root, countByte, fileIn, fileOut);
    FreeDecoder(root);
}
