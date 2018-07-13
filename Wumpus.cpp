#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
int **map = NULL;
/* logica del mapa
ground = 0

Wumpus = 5
smell wumpus = -4

abyss = 3
wind = -2

wind and smell = -6

player = 1

player in -6 = -5 wind and smell
player in -4 = -3 smell
player in -2 = -1 wind
*/
struct Enemy{
	int positionX;
	int positionY;
	bool live;
};

struct Player{
	int positionX;
	int positionY;
	int move;
	int usedArrows;
	bool live;
};
void printMap(int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cout << map[i][j] << "\t";
		}
		cout << "\n";
	}
}
void particles(int x1, int y1, int type){
	if(type == 1 || type == -1){
		map[x1][y1] = map[x1][y1] + type;
	}else if(map[x1][y1] == -2){
		if (type == -4)
			map[x1][y1] = map[x1][y1] + type;
	} else if(map[x1][y1] == -4){
		if(type == -2)
			map[x1][y1] = map[x1][y1] + type;
	} else if (map[x1][y1] != 3){
		map[x1][y1] = map[x1][y1] + type;
	}
}
void smellWind(int x1, int y1, int type, int dimension){
	if(x1 != 0)
		particles(x1-1,y1,type);
	if(y1 != 0)
		particles(x1,y1-1,type);
	if(y1 != dimension)
		particles(x1,y1+1,type);
	if(x1 != 0 && y1 !=0)
		particles(x1-1,y1-1,type);
	if(x1 != 0 && y1 != dimension)
		particles(x1-1,y1+1,type);
	
	if(x1 == (dimension-1)){
		if(x1 == dimension)
			particles(x1+1,y1,type);
		if(x1 == dimension && y1 != 0)
			particles(x1+1,y1-1,type);
		if(x1 == dimension && y1 != dimension)
			particles(x1+1,y1+1,type);
	}else{
		if(x1 != dimension)
			particles(x1+1,y1,type);
		if(x1 != dimension && y1 != 0)
			particles(x1+1,y1-1,type);
		if(x1 != dimension && y1 != dimension)
			particles(x1+1,y1+1,type);
	}
}

void abysses(int cant){
	int dimension = cant;
	int a = dimension - ((double) (dimension*30)/100); //Max surrounding abysses
	cant = cant / 2; //Max principal abysses
	
	while(cant != 0){
		//Principal abyss
		int x = rand()%dimension;
		int y = rand()%dimension;
		map[y][x] = 3;
		
		//Put wind surrounding the principal abyss
		smellWind(y,x,-2,dimension);
		
		
		//Same logic for surrounding abysses
		int n = 2 + rand()%a; //Amount of surrounding abysses
		if (x != (dimension-1) && x !=0 && y != (dimension-1) && y !=0){
			while(n != 0){
				int x1 = (rand()%4) - 2 ;
				int y1 = (rand()%4) - 2;
				if(map[abs(y+y1)][abs(x+x1)] != 3){
					map[abs(y+y1)][abs(x+x1)] = 3;
					smellWind(abs(y+y1),abs(x+x1),-2,dimension);
				}
				n--;
			}
		}		
		cant--;
	}
}

Enemy DefWumpus(int dimension){
	//Define wumpus like Enemy
	Enemy Wumpus;
	
	Wumpus.live = true;
	do{
		Wumpus.positionX = rand()%dimension;
		Wumpus.positionY = rand()%dimension;
	}while(map[Wumpus.positionY][Wumpus.positionX] == 3);
	map[Wumpus.positionY][Wumpus.positionX] = 5;
	
	//Put smell around the wumpus
	smellWind(Wumpus.positionY,Wumpus.positionX,-4,dimension);
	
	return Wumpus;
}

Player DefWarrior(int dimension){
	//Define warrior like Player
	Player Warrior;
	
	Warrior.live = true;
	do{
		Warrior.positionX = rand()%dimension;
		Warrior.positionY = rand()%dimension;
	} while(map[Warrior.positionY][Warrior.positionX] == 3 || map[Warrior.positionY][Warrior.positionX] == 5);
	
	particles(Warrior.positionY,Warrior.positionX,1);
	
	return Warrior;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int n = 0; //Dimension board
	int state = 0; //State of the warrio
	int option = 0; //Move or shoot
	int optShoot = 0; //Place where shoot
	
	cout << "Please enter the game board dimension(10-20): ";
	cin >> n;
	while (n < 10 || n > 20){
		system("CLS");
		cout << "The dimension should be between 10 and 20" << "\n";
		cout << "Please enter the game board dimension(10-20): ";
		cin >> n;
	}
	
	//{Define game board
	system("CLS");
	map = new int*[n];
	for(int i = 0; i < n; i++){
		map[i] =  new int[n];
	}
	
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			map[i][j] = 0;
		}
	}
	
	//Define characters
	Enemy wumpus;
	Player warrior;
	
	//Put abysses, wumpus and player
	abysses(n);
	wumpus = DefWumpus(n);
	warrior = DefWarrior(n);
	
	printMap(n);
	//}
	
	//{Gameplay
	while(warrior.live && wumpus.live){
		cout << "Warrior say:\n";
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if (map[i][j] == -5 || map[i][j] == -3 || map[i][j] == -1)
					state = map[i][j];
			}
		}
		switch (state){
		case -5:
			cout << "player in -6 wind and smell\n";
			break;
		case -3:
			cout << "player in -4 smell\n";
			break;
		case -1:
			cout << "player in -2 wind\n";
			break;
		default:
			cout << "Nothing\n";
		}
		
		do{
			cout << "\nChoose your turn accion: \n1 = Move\n2 = shoot\n";
			cin >> option;		}while(option != 1 && option != 2);
		
		if(option == 1){//Warrior'll move
			
		} else if (option == 2){//warrior'll shoot
			system("CLS");
			cout << "Warrior X= "<<warrior.positionX<< " Y= "<<warrior.positionY;
			cout << " wumpus X= "<<wumpus.positionX<< " Y= "<<wumpus.positionY;
			
			
			cout << "Choose where atack:\n1\t2\t3\n4\tU\t5\n6\t7\t8\n";
			cin >> optShoot;
			switch (optShoot){
			case 1:
				break;
			case 2:
				if(warrior.positionX == wumpus.positionX && warrior.positionY > wumpus.positionY)
					warrior.live = false;
				break;
			case 3:
				break;
			case 4:
				if(warrior.positionX > wumpus.positionX && warrior.positionY == wumpus.positionY)
					warrior.live = false;
				break;
			case 5:
				if(warrior.positionX < wumpus.positionX && warrior.positionY == wumpus.positionY)
					warrior.live = false;
				break;
			case 6:
				break;
			case 7:
				if(warrior.positionX == wumpus.positionX && warrior.positionY < wumpus.positionY)
					warrior.live = false;
				break;
			case 8:
				break;
			}
		}
		
		
		
	}
	
	if(warrior.live){
		cout << "You win" << "\n" << "CONGRATULATIONS!";
	} else{
		cout << "You dead";
	}
	
	return 0;
}
