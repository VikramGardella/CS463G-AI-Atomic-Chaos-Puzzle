#ifndef NODE_H
#define NODE_H

Class Node
{

private:

char** bot;
char** top;
int hV, fV, gV;
Node* parent;

public:

Node(char** b, char** t, int heur, Node* par);

int getH();
int getG();
int getF();
Node* getParent();

}

#endif
