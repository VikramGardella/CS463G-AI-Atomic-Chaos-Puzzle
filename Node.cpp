
class Node
{

private:

char** bot;
char** top;
int hV, fV, gV;
Node* parent;
Node* rCW;
Node* rCCW;
Node* flp;
int shiftI;
int invert;

public:

Node(char** b, char** t, int heur, int shi, int i){
	bot = b;
	top = t;
	hV = heur;
	shiftI = shi;
	invert = i;
}

int getH(){return hV;}
int getG(){return gV;}
int getF(){return fV;}
int getShift(){return shiftI;}
Node* getParent(){return parent;}
char** getBottom(){return bot;}
char** getTop(){return top;}

void setRCW(Node* n){rCW = n;}
void setRCCW(Node* n){rCCW = n;}
void setFlip(Node* n){flp = n;}

Node* getRCW(){return rCW;}
Node* getRCCW(){return rCCW;}
Node* getFlip(){return flp;}

};
