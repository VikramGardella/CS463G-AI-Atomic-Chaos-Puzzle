#include <iostream>
#include <cstdlib>
#include <string>
#include "Node.cpp"
//#include "Heap.cpp"

using namespace std;


void printState(char** b, char** t);
Node saveState(char** b, char** t, int hValue);
int checkH(char** b, char** t);
void buildHeap(int depth, Node* n);
void flip(char** b, char** t);
void rotateCW(char** b, char** t);
void rotateCCW(char** b, char** t);
void printHeap(Node* n);
char** copy2D(char** b, char *fin[6]);

Node* root;
int shiftInd = 0; //made global variable so I don't have to deal with *'s
int inverted = 0; //0 is not upside down, 1 is upside down.


char** copy2D(char** b, char** fin){

	for(int i=0; i<6; i++){
		for(int j=0;j<(6-i); j++){
			fin[i][j]=b[i][j];
//			cout << fin[i][j];
		}
//		cout << "" << endl;
	}
	return fin;
}





void fallLogic(char** b, char** t){
	int space;
	int topspace;
	int aftShift;
	if(inverted == 0){//t[i-shiftInd] falls into b[i]
		for(int i=0; i<6; i++){
			space = 0;//resetting free space count
			topspace = 0;
			aftShift = shiftInd+i;
			if(aftShift>=6)
				aftShift%=6;
			if(aftShift<0)
				aftShift+=6;
			for(int j=0; j<(6-i); j++){
				if(b[i][j]=='-')
					space++;
			}//end inner


		//PHASE 1: shift/press tubes in original bottom tubes(b) down
			if(b[i][0]=='-'&&space!=0){//downtube shift
				for(int p=0; p<(6-i-space); p++){ //	Problem possibly located here
					b[i][p]=b[i][p+space];
					b[i][p+space]='-';
				}
			}//end if

//			printState(b,t);

		//PHASE 2: shift/press tubes in original top tubes(t) down
			for(int m=0; m<(6-aftShift); m++){
				if(t[aftShift][m]=='-')
					topspace++;
			}
//			cout << "drop " << space << " into bottom tube[" << i << "]" << endl;
			//move balls here

			if(topspace!=0&&t[aftShift][0]=='-'){//uptube shift
				for(int x=0; x<(6-aftShift-topspace); x++){
					t[aftShift][x]=t[aftShift][x+topspace];
					t[aftShift][x+topspace] = '-';
				}
			}

//			printState(b,t);

		//PHASE 3: shift balls from original top tubes(t) into original bottom tubes(t) if there's space
			if(space!=0){
				int topballs = 6-aftShift-topspace;
				for(int l=0; l<(12-i-aftShift-space); l++){//tube transfer shift
					if(l<space && l<topballs){
						b[i][6-i-space+l]=t[aftShift][l];
						t[aftShift][l] = '-';
					}
					if(l>=space && l<topballs){
						t[aftShift][(l-space)]=t[aftShift][l];
						t[aftShift][l] = '-';
					}
				}
			}
//			printState(b,t);
		}//end outer for-loop
	}
	else{//b[i] falls into t[i-shiftInd]
		for(int h=0; h<6; h++){
			space = 0;
			topspace = 0;
			aftShift = h+shiftInd;
			if(aftShift>=6)
				aftShift%=6;
			if(aftShift<0)
				aftShift+=6;
			for(int j=0; j<(6-aftShift); j++){
				if(t[aftShift][j]=='-')
					space++; //counting spaces in orig top tubes(t) which are now on the bottom
			}//end inner

		//PHASE 1: shift/press balls down in original top tubes(t) which is currently on bottom
			if(t[aftShift][6-aftShift-1]=='-'&&space!=0)//downtube shift
				for(int r=0; r<(6-aftShift-space); r++){
					t[aftShift][6-aftShift-1-r]=t[aftShift][6-aftShift-r-1-space];
					t[aftShift][6-aftShift-r-1-space]='-';
				}
//			printState(b,t);

			for(int m=0; m<(6-h); m++){
				if(b[h][m]=='-')
					topspace++;
			}
//			cout << "drop " << space << " into top tube[" << aftShift << "]" << endl;
			//move balls here
			//(12-h-aftShift)-(6-aftShift-space)= combined capacities of alligned tubes minus balls that dont need to move
			//ends up equaling (6-h+space) which is the total number of ball position changes

		//PHASE 2: shift/press up in original bottom tubes(b) which are currently on top
			if(topspace!=0&&b[h][6-h-1]=='-'){//uptube shift
				for(int x=0; x<(6-h-topspace); x++){
					b[h][6-h-1-x]=b[h][6-h-1-x-topspace];
					b[h][6-h-1-x-topspace]='-';
				}

			}

//			printState(b,t);

		//PHASE 3: if there is space current bottom tubes AKA original top tubes(s), then transfer from b to t
			if(space!=0){
				int topballs = 6-h-topspace;
				for(int l=0; l<(12-h-aftShift-space); l++){//tube transfer shift
//					cout << "l = " << l << ", h = " << h << endl;
					if(l<space && l<topballs){
						t[aftShift][space-l-1] = b[h][6-h-1-l];
						b[h][6-h-1-l] = '-';
					}
//					printState(b,t);
					if(l>=space && l<topballs){
						b[h][6-h-1-l+space] = b[h][6-h-l-1];
						b[h][6-h-l-1] = '-';
					}
				}
			}
//			printState(b,t);
		}//end outer for-loop
	}
}

void printState(char** b, char** t){
	int aftShift;
	char colors[6] = {'R', 'G', 'Y', 'B', 'W', 'P'};
	cout << endl << "Current state: " << endl;
	for(int i=0; i<6; i++){
		aftShift = i+shiftInd;
		if(aftShift>=6)
			aftShift%=6;
		if(aftShift<0)
			aftShift+=6;
		for(int l=0; l<i; l++)
			cout << " ";
		for(int j=0; j<(6-i); j++)
			cout << b[i][j];
		cout << "|";
		for(int k=0; k<(6-aftShift); k++)
			cout << t[aftShift][k];
		cout << "" << endl;
	}
//	cout << "shiftind = " << shiftInd << ", aftShift = " << aftShift << endl;
	if(inverted==0)
		cout << "This way is up ->" << endl;
	else
		cout << "<- This way is up" << endl;
	int hVal = checkH(b,t);
	cout << "Variable 'inverted' = " << inverted << endl;
	cout << "Heuristic Value is: " << hVal << " (0 is solved)" << endl;
	cout << "" << endl;
}

int checkH(char** b, char** t){
	int total = 0;
	bool stuckUp = false;
	char col[6] = {'R', 'G', 'Y', 'B', 'W', 'P'};
	for(int i=0; i<6; i++)
		for(int j=0; j<(6-i-1); j++)
			if(b[i][j]!=b[i][j+1]&&b[i][j]!='-'&&b[i][j+1]!='-'){
				total+=3;
				if(inverted==1)
					stuckUp = true;
		}
	for(int i=0; i<6; i++)
		for(int j=0; j<(6-i-1); j++)
			if(b[i][j]!=col[i])
				total++;
		
	for(int i=0; i<6; i++)
		for(int j=0; j<(6-i-1); j++){
			if(t[i][j]!=t[i][j+1]&&t[i][j]!='-'&&t[i][j+1]!='-'){
				total+=3;
				if(inverted==0)
					stuckUp = true;
			}
			if(t[i][j]!=col[j]&&t[i][j]!='-')
				total++;
		}
	if(stuckUp == true)//if one or more balls are stuck in top tubes
		total++;//the puzzle will require at least one more move
	int twistEquil;
	if(shiftInd>=3)
		twistEquil = 6-shiftInd;
	else
		twistEquil = shiftInd;
	total += twistEquil;
	return total;
//for every neighbor that is a different color, 1 is added to the hValue
//because it will take at least one move to change the order. Only checking
//one neighbor for each ball

/*	return (21-total);*/
}

Node saveState(char** b, char** t, int hValue){
	Node n = Node(b, t, hValue, shiftInd, inverted);
	return n;
}

void buildHeap(int depth, Node* n){
	if(depth>0){
		char* ab[6];
		char* at[6];
		char asixBot[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
		char afiveBot[5] = {'G', 'G', 'G', 'G', 'G'};
		char afourBot[4] = {'Y', 'Y', 'Y', 'Y'};
		char athreeBot[3] = {'B', 'B', 'B'};
		char atwoBot[2] = {'W', 'W'};
		char aoneBot[1] = {'P'};
		ab[0] = asixBot;
		ab[1] = afiveBot;
		ab[2] = afourBot;
		ab[3] = athreeBot;
		ab[4] = atwoBot;
		ab[5] = aoneBot;
		char asixTop[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
		char afiveTop[5] = {'G', 'G', 'G', 'G', 'G'};
		char afourTop[4] = {'Y', 'Y', 'Y', 'Y'};
		char athreeTop[3] = {'B', 'B', 'B'};
		char atwoTop[2] = {'W', 'W'};
		char aoneTop[1] = {'P'};
		at[0] = asixTop;
		at[1] = afiveTop;
		at[2] = afourTop;
		at[3] = athreeTop;
		at[4] = atwoTop;
		at[5] = aoneTop;
		copy2D(n->getBottom(),ab);
		copy2D(n->getTop(),at);
//		printState(ab, at);
		rotateCCW(ab, at);
		cout << "IF YOU ROTATE CCW:" << endl;
		printState(ab, at);
		Node aaa = Node(ab, at, checkH(ab, at), shiftInd, inverted);
		Node* aaaa = &aaa;
		n->setRCCW(aaaa);
		shiftInd++;
		if(shiftInd>=6)
			shiftInd%=6;
		if(shiftInd<0)
			shiftInd+=6;
	}

	if(depth>0){
		char* bb[6];
		char* bt[6];
		char bsixBot[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
		char bfiveBot[5] = {'G', 'G', 'G', 'G', 'G'};
		char bfourBot[4] = {'Y', 'Y', 'Y', 'Y'};
		char bthreeBot[3] = {'B', 'B', 'B'};
		char btwoBot[2] = {'W', 'W'};
		char boneBot[1] = {'P'};
		bb[0] = bsixBot;
		bb[1] = bfiveBot;
		bb[2] = bfourBot;
		bb[3] = bthreeBot;
		bb[4] = btwoBot;
		bb[5] = boneBot;
		char bsixTop[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
		char bfiveTop[5] = {'G', 'G', 'G', 'G', 'G'};
		char bfourTop[4] = {'Y', 'Y', 'Y', 'Y'};
		char bthreeTop[3] = {'B', 'B', 'B'};
		char btwoTop[2] = {'W', 'W'};
		char boneTop[1] = {'P'};
		bt[0] = bsixTop;
		bt[1] = bfiveTop;
		bt[2] = bfourTop;
		bt[3] = bthreeTop;
		bt[4] = btwoTop;
		bt[5] = boneTop;
		copy2D(n->getBottom(),bb);
		copy2D(n->getTop(),bt);
//		printState(bb, bt);
		rotateCW(bb, bt);
		cout << "IF YOU ROTATE CW:" << endl;
		printState(bb, bt);
		Node bbb = Node(bb, bt, checkH(bb, bt), shiftInd, inverted);
		Node* bbbb = &bbb;
		n->setRCW(bbbb);
		shiftInd--;
		if(shiftInd<0)
			shiftInd+=6;
		if(shiftInd>=6)
			shiftInd%=6;
	}


	if(depth>0){
		char* cb[6];
		char* ct[6];
		char csixBot[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
		char cfiveBot[5] = {'G', 'G', 'G', 'G', 'G'};
		char cfourBot[4] = {'Y', 'Y', 'Y', 'Y'};
		char cthreeBot[3] = {'B', 'B', 'B'};
		char ctwoBot[2] = {'W', 'W'};
		char coneBot[1] = {'P'};
		cb[0] = csixBot;
		cb[1] = cfiveBot;
		cb[2] = cfourBot;
		cb[3] = cthreeBot;
		cb[4] = ctwoBot;
		cb[5] = coneBot;
		char csixTop[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
		char cfiveTop[5] = {'G', 'G', 'G', 'G', 'G'};
		char cfourTop[4] = {'Y', 'Y', 'Y', 'Y'};
		char cthreeTop[3] = {'B', 'B', 'B'};
		char ctwoTop[2] = {'W', 'W'};
		char coneTop[1] = {'P'};
		ct[0] = csixTop;
		ct[1] = cfiveTop;
		ct[2] = cfourTop;
		ct[3] = cthreeTop;
		ct[4] = ctwoTop;
		ct[5] = coneTop;
		copy2D(n->getBottom(),cb);
		copy2D(n->getTop(),ct);
//		printState(ab, at);
		flip(cb, ct);
		cout << "IF YOU FLIP:" << endl;
		printState(cb, ct);
		Node ccc = Node(cb, ct, checkH(cb, ct), shiftInd, inverted);
		Node* cccc = &ccc;
		n->setFlip(cccc);
		if(inverted==0)
			inverted = 1;
		else
			inverted = 0;

	}
	int ahval, bhval, chval;
	ahval = n->getRCCW()->getH();
	bhval = n->getRCW()->getH();
	chval = n->getFlip()->getH();
	Node* next;
	if(ahval<bhval){
		if(ahval<chval){
			next = n->getRCCW();//pick a
//			cout << ahval <<" is less than " << chval << " and " << bhval << endl;
		}
		else if(chval<bhval){
			next = n->getFlip();//pick c
//			cout << chval << " is less than " << bhval << " and " << ahval << endl;
		}
	}
	else{
		if(bhval<chval)
			next = n->getRCW();//pick b
		else{
			next = n->getFlip();//pick c
//			cout << chval << " is less than " << bhval << " and " << ahval << endl;
		}
	}
//	cout << "chose: " << next->getH() <<  endl;
	printState(next->getBottom(),next->getTop());
	if(next->getH()>0)
		buildHeap(depth-1, next);

}
void printHeap(Node* n){

	cout << n->getH() << endl;
	cout << n->getRCW()->getH() << "-" << n->getRCCW()->getH() << "-" << n->getFlip()->getH() << endl;

/*	char** bee = n->getBottom();
	char** tee = n->getTop();
	printState(bee,tee);
	cout << endl << endl << endl;*/
//	if(n->getRCW()!=NULL){
//		bee = n->getRCW()->getBottom();
//		tee = n->getRCW()->getTop();
//		printState(bee, tee);
//	}
//	if(n->getRCCW()!=NULL){
/*		bee = n->getRCCW()->getBottom();
		tee = n->getRCCW()->getTop();
		printState(bee, tee);
//	}
//	if(n->getFlip()!=NULL){
		bee = n->getFlip()->getBottom();
		tee = n->getFlip()->getTop();
		printState(bee, tee);
//	}
	printHeap(n->getRCW());
	printHeap(n->getRCCW());
	printHeap(n->getFlip());
*/
}


void rotateCCW(char** b, char** t){
	shiftInd--;
	if(shiftInd<0)
		shiftInd+=6;
	if(shiftInd>=6)
		shiftInd%=6;
//	cout << "Rotated counter-clockwise." << endl << endl;
	fallLogic(b, t);

}

void rotateCW(char** b, char** t){
	shiftInd++;
	if(shiftInd>=6)
		shiftInd%=6;
	if(shiftInd<0)
		shiftInd+=6;
//	cout << "Rotated clockwise." << endl << endl;
	fallLogic(b, t);
}

void flip(char** b, char** t){
	if(inverted==1)
		inverted = 0;
	else
		inverted = 1;
	fallLogic(b, t);
	//make changes to ball position based on logic here^
}

void randomize(char** b, char** t, int in){
	int r;
	for(int i=0; i<in; i++){//will perform 1000 random operations
		r = rand()%3;
		if(r==0)
			rotateCCW(b, t);
		if(r==1)
			rotateCW(b, t);
		if(r==2)
			flip(b, t);
	}

	Node apple = saveState(b, t, checkH(b,t));
	root = &apple;
	buildHeap(3, root);
//	printHeap(root);
}

int main(){
/*
to begin the structure of this simulated atomic puzzle structure,
I will create 12 arrays, each with a defined capacity depending on which tube
they represent: 6 bottom tubes, 6 corresponding top tubes as well as "shift index"
to help keep track of which tubes are over which. The "shift index" will be 0 when
all the tubes are over the tube with the same capacity as them (their twin on the other side)
The balls will be represented with characters; to avoid confusion with blue and black, black is
replaced with Purple (P). There are 6 red balls (R), 5 green (G), 4 Yellow (Y), 3 blue (B),
2 white (W), and 1 purple (P). Void spaces are represented with "-".
*/


//Original bottom tubes
	char *bottom[6];//making 2D with pointer to char arrays so they
//each can have different capacities

	char sixBot[6] = {'R', 'R', 'R', 'R', 'R', 'R'};
	char fiveBot[5] = {'G', 'G', 'G', 'G', 'G'};
	char fourBot[4] = {'Y', 'Y', 'Y', 'Y'};
	char threeBot[3] = {'B', 'B', 'B'};
	char twoBot[2] = {'W', 'W'};
	char oneBot[1] = {'P'};

	bottom[0] = sixBot;
	bottom[1] = fiveBot;
	bottom[2] = fourBot;
	bottom[3] = threeBot;
	bottom[4] = twoBot;
	bottom[5] = oneBot;

//Original top tubes
	char *top[6]; //making them into a 2D array like up there^

	char sixTop[6] = {'-', '-', '-', '-', '-', '-'};
	char fiveTop[5] = {'-', '-', '-', '-', '-'};
	char fourTop[4] = {'-', '-', '-', '-'};
	char threeTop[3] = {'-', '-', '-'};
	char twoTop[2] = {'-', '-'};
	char oneTop[1] = {'-'};

	top[0] = sixTop;
	top[1] = fiveTop;
	top[2] = fourTop;
	top[3] = threeTop;
	top[4] = twoTop;
	top[5] = oneTop;

	string inp = "";
	int input = 0;
	printState(bottom, top);
	while(inp!="6"){
		cout << "Please enter the number corresponding with what you wish to do." << endl;
		cout << "1 - Print current state of puzzle." << endl;
		cout << "2 - Rotate top half counter-clockwise." << endl;
		cout << "3 - Rotate top half clockwise." << endl;
		cout << "4 - Flip puzzle." << endl;
		cout << "5 - Randomize puzzle" << endl;
		cout << "6 - Exit puzzle simulator." << endl << endl;
		cin >> inp;

		if(inp == "1")
			printState(bottom, top);
		else if(inp == "2")
			rotateCCW(bottom, top);
		else if(inp == "3")
			rotateCW(bottom, top);
		else if(inp == "4")
			flip(bottom, top);
		else if(inp == "5"){
			cout << "How many random operations would you like to perform?" << endl;
			cin >> input;
			randomize(bottom, top, input);
		}
		else if(inp!= "6")
			cout << "Invalid input, please try again." << endl;
	}

}
