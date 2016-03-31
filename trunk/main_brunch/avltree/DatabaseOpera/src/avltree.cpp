#include "avltree.h"
#include<iostream>
#include<string.h>
#include<malloc.h>

using namespace std;

avltree::avltree()
{
    //ctor
}

avltree::~avltree()
{
    //dtor
}

AvlTree avltree::MakeEmpty(AvlTree T)//delete T->element
{
    if(T!=NULL)
    {
        MakeEmpty(T->left);
        MakeEmpty(T->right);

        delete T;
        T=NULL;
        cout << "work?" << endl;
    }
    return NULL;
}

Position avltree::Find(char * X,AvlTree T)
{
    if(T==NULL)
    {
        return NULL;
    }
    if(strcmp(X,T->element)<0)
    {
        return Find(X,T->left);
    }
    else if(strcmp(X,T->element)>0)
    {
        return Find(X,T->right);
    }
    else
        return T;
}

AvlTree avltree::insert_t(char * X,AvlTree T)
{
    if(NULL==T)
    {
        T=new struct AvlNode;
        if(NULL==T)
        {
            cout << "out of space" << endl;
            return NULL;
        }
        else
        {
            memcpy(T->element,X,20);
            T->left=T->right=NULL;
            T->height=0;
        }
    }
    else if(strcmp(X,T->element)<0)
    {
        T->left=insert_t(X,T->left);
        if(2==Height(T->left)-Height(T->right))
        {
            if(strcmp(X,T->left->element)<0)
            {
                T=SingleRotateWithLeft(T);
            }
            else
                T=DoubleRotateWithLeft(T);
        }
    }
    else
    {
        T->right=insert_t(X,T->right);
        if(2==Height(T->right)-Height(T->left))
        {
            if(strcmp(X,T->right->element)>0)
            {
                T=SingleRotateWithRight(T);
            }
            else
                T=DoubleRotateWithRight(T);
        }
    }

    T->height=Max(Height(T->left),Height(T->right))+1;
    return T;
}

AvlTree avltree::delete_t(char * X,AvlTree T)
{
    if(NULL==T)
    {
        cout << "no online_user can be deleted!" << endl;
        return NULL;
    }
    else if(strcmp(X,T->element)<0)
    {
        delete_t(X,T->left);
        if(2==Height(T->right)-Height(T->left))
        {
            if((NULL!=T->right->left)&&(Height(T->right->left)>Height(T->right->right)))
            {
                DoubleRotateWithRight(T);
            }
            else
            {
                SingleRotateWithRight(T);
            }
        }
    }
    else if(strcmp(X,T->element)>0)
    {
        delete_t(X,T->right);
        if(2==Height(T->left)-Height(T->right))
        {
             if((NULL!=T->left->right)&&(Height(T->left->right)>Height(T->left->left)))
             {
                DoubleRotateWithLeft(T);
             }
             else
             {
                SingleRotateWithLeft(T);
             }
        }
    }
    else
    {
        if((NULL!=T->left)&&(NULL!=T->right))
        {
            Position temp=T->left;
            while(temp->right!=NULL)
            {
                temp=temp->right;
            }
            memcpy(T->element,temp->element,20);
            delete_t(temp->element,T->left);
            if(2==Height(T->right)-Height(T->left))
            {
                if((NULL!=T->right->left)&&(Height(T->right->left)>Height(T->right->right)))
                {
                    DoubleRotateWithRight(T);
                }
                else
                {
                    SingleRotateWithRight(T);
                }
            }
        }
        else
        {
            Position temp=T;
            if(NULL!=T->left)
            {
                T=T->left;
            }
            else if(NULL!=T->right)
            {
                T=T->right;
            }
            T=NULL;
            delete temp;
            temp=NULL;
        }
    }
    if(NULL!=T)
    {
        T->height=Max(Height(T->left),Height(T->right))+1;
    }

    return T;
}

void avltree::show_all(AvlTree T)
{
    if(NULL==T) return;
    show_all(T->left);
    cout << T->element << " ";
    show_all(T->right);
}

int avltree::Height(Position P)//static
{
    if(NULL==P)
    {
        return -1;
    }
    else
    {
        return P->height;
    }
}

Position avltree::SingleRotateWithLeft(Position K2)
{
   Position  K1=NULL;

   K1=K2->left;
   K2->left=K1->right;
   K1->right=K2;

   K2->height=Max(Height(K2->left),Height(K2->right))+1;
   K1->height=Max(Height(K1->left),Height(K1->right))+1;

   return K1;
}

Position avltree::SingleRotateWithRight(Position K2)
{
    Position K1;

    K1=K2->right;
    K2->right=K1->left;
    K1->left=K2;

    K2->height=Max(Height(K2->left),Height(K2->right))+1;
    K1->height=Max(Height(K1->left),Height(K1->right))+1;

    return K1;
}

Position avltree::DoubleRotateWithLeft(Position K3)
{
    K3->left=SingleRotateWithRight(K3->left);
    return SingleRotateWithLeft(K3);
}

Position avltree::DoubleRotateWithRight(Position K3)
{
    K3->right=SingleRotateWithLeft(K3->right);
    return SingleRotateWithRight(K3);
}

int avltree::Max(int height_l,int height_r)
{
    return height_l > height_r ? height_l : height_r;
}












