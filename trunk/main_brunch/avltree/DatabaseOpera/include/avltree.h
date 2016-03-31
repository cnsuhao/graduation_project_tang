#ifndef AVLTREE_H
#define AVLTREE_H

struct AvlNode;
typedef struct AvlNode * Position;
typedef struct AvlNode * AvlTree;

struct AvlNode
{
    char element[20];
    AvlTree left;
    AvlTree right;
    int height;
};

/*class AvlNode
{
    public:
        char element[20];
        AvlNode * left;
        AvlNode * right;
        int height;
};

typedef struct AvlNode * Position;
typedef struct AvlNode * AvlTree;*/

class avltree
{
    public:
        avltree();
        virtual ~avltree();

        AvlTree MakeEmpty(AvlTree T);
        Position Find(char * X,AvlTree T);
        AvlTree insert_t(char * X,AvlTree T);
        AvlTree delete_t(char * X,AvlTree T);
        void show_all(AvlTree T);
    private:
        int Height(Position P);
        Position SingleRotateWithLeft(Position K2);
        Position SingleRotateWithRight(Position K2);
        Position DoubleRotateWithLeft(Position K3);
        Position DoubleRotateWithRight(Position K3);
        int Max(int height_l,int height_r);

};

#endif // AVLTREE_H
