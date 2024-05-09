#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *map = (TreeMap*)malloc(sizeof(TreeMap));
    
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value)
{
    TreeNode *new_node = createTreeNode(key, value);

    if(tree->root == NULL)
    {
        tree->root = new_node;
        tree->current = new_node;
        return;
    }

    TreeNode *aux = tree->root;

    while (aux != NULL)
    {
        if (tree->lower_than(key, aux->pair->key))
        {
            if (aux->left == NULL)
            {
                aux->left = new_node;
                new_node->parent = aux;
                tree->current = new_node;
                return;
            }

            aux = aux->left;
        }

        else
        {
            if (aux->right == NULL)
            {
                aux->right = new_node;
                new_node->parent = aux;
                tree->current = new_node;
                return;
            }

            aux = aux->right;
        }

        if (is_equal(tree, aux->pair->key, key))
            return;
    }
}

TreeNode * minimum(TreeNode * x)
{
    TreeNode *aux = x;
    
    while (aux->left != NULL)
        aux = aux->left;
    
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
    TreeNode *padre = node->parent;
    
    if (node->left == NULL && node->right == NULL)
    {
        if (padre == NULL)
            tree->root = NULL;
        
        else if (padre->left == node)
            padre->left = NULL;

        else if (padre->right == node)
            padre->right = NULL;

        return;
    }
    
    else if (node->left == NULL || node->right == NULL)
    {
        if (node->left != NULL)
        {
            if (padre == NULL)
            {
                tree->root = node->left;
                node->left->parent = NULL;
            }
            
            else if (padre->left == node)
            {
                padre->left = node->left;
                node->left->parent = padre;
                
            }
            
            else if (padre->right == node)
            {
                padre->right = node->left;
                node->left->parent = padre;
            }

            return;
        }

        if (node->right != NULL)
        {
            if (padre == NULL)
            {
                tree->root = node->right;
                node->right->parent = NULL;
            }

            else if (padre->left == node)
            {
                padre->left = node->right;
                node->right->parent = padre;

            }

            else if (padre->right == node)
            {
                padre->right = node->right;
                node->right->parent = padre;
            }

            return;
        }
    }

    else if (node->left != NULL && node->right != NULL)
    {
        TreeNode *min = minimum(node->right);

        node->pair->key = min->pair->key;
        removeNode(tree, min);

        return;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key)
{
    TreeNode *aux = tree->root;

    while (aux != NULL)
    {
        if (is_equal(tree, aux->pair->key, key))
        {
            tree->current = aux;
            return aux->pair;
        }
        
        else if (tree->lower_than(key, aux->pair->key))
        {
            if (aux->left == NULL)
                return NULL;
            
            aux = aux->left;
        }

        else
        {
            if (aux->right == NULL)
                return NULL;
            
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree)
{
    TreeNode *aux = tree->root;

    while (aux->left != NULL)
        aux = aux->left;

    tree->current = aux;
    
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
    TreeNode *aux = tree->current;

    if (aux->right == NULL)
        return NULL;

    aux = aux->right;

    if (aux->left == NULL)
        return aux->pair;
        
    aux = minimum(aux);

    tree->current = aux;

    return aux->pair;
}