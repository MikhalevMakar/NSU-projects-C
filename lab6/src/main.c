#include <stdio.h>
#include <stdlib.h>

typedef struct AVLTree {
    char Height;
    int Value;
    struct AVLTree* Right;
    struct AVLTree* Left;
}AVLTree;

char GetBalance(AVLTree* tree) {
    if (tree != NULL) {
        return tree->Height;
    }
    return 0;
}

char GetHeight(AVLTree* tree) {
    char L = GetBalance(tree->Left);
    char R = GetBalance(tree->Right);
    if (L > R) {
        return L + 1;
    }
    return R + 1;
}

char  BalanceHeightTree(AVLTree* tree) {
    return GetBalance(tree->Right) - GetBalance(tree->Left);
}

AVLTree* LeftRotateTree(AVLTree* tree) {
    AVLTree* curTree = tree->Right;
    tree->Right = curTree->Left;
    curTree->Left = tree;
    tree->Height = GetHeight(tree);
    curTree->Height = GetHeight(curTree);
    return curTree;
}

AVLTree* RigthRotateTree(AVLTree* tree) {
    AVLTree* curTree = tree->Left;
    tree->Left = curTree->Right;
    curTree->Right = tree;
    tree->Height = GetHeight(tree);
    curTree->Height = GetHeight(curTree);
    return curTree;
}

AVLTree* BalanceTree(AVLTree* tree) {
    tree->Height = GetHeight(tree);
    if (BalanceHeightTree(tree) == -2) {
        if (BalanceHeightTree(tree->Left) >= 1) {
            tree->Left = LeftRotateTree(tree->Left);
        }
        return RigthRotateTree(tree);
    }
    else if (BalanceHeightTree(tree) == 2) {
        if (BalanceHeightTree(tree->Right) <= -1) {
            tree->Right = RigthRotateTree(tree->Right);
        }
        return LeftRotateTree(tree);
    }
    return tree;
}

void CreateTree(AVLTree** root, AVLTree* tree, int currentValue, int index) {
    if (*root == NULL) {
        tree[index].Value = currentValue;
        tree[index].Left = NULL;
        tree[index].Right = NULL;
        tree[index].Height = 1;
        *root = &tree[index];
        return;
    }
    else if (currentValue <= (*root)->Value) {
        CreateTree(&(*root)->Right, tree, currentValue, index);
    }
    else {
        CreateTree(&(*root)->Left, tree, currentValue, index);
    }
    *root = BalanceTree(*root);
}

int main() {
    int count = 0, value = 0;
    if (scanf("%d", &count) != 1) {
        return 0;
    }
    if (count == 0) {
        printf("0");
        return 0;
    }

    AVLTree* tree = malloc(count * sizeof(AVLTree));
    if (tree == NULL) {
        return 0;
    }
    AVLTree* root = NULL;
    for (int i = 0; i < count; ++i) {
        if (scanf("%d", &value) != 1) {
            return 0;
        }
        CreateTree(&root, tree, value, i);
    }
    printf("%d", root->Height);
    free(tree);
    return 0;
}

