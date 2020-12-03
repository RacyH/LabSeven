/*Racy Halterman
C++ Fall 2020
Due: December 2nd
Lab 7: Battleship
Make a simple battleship game.*/

#include <iostream>
#include <time.h>
#include <iomanip>

using namespace std;
using std::cout;

void intro();
void game(int& now, int& nol);
void grid(char ps[][10], int& uoc);
void shipplaceuser(char placement[][10]);
void shipplacecomp(int x[], int y[]);
void gameprint();
void computerchoice(int& comp1);
void chooseshot(int& user1, int& comp1);
int lrud();
void clrscr();
void reset();
bool validtest(char valid[][10], int& shipd, int& x, int& y, int shipl);
void endgameprint();

int ships[5] //This is global so that ship lengths are the same for both user and computer 
{
	ships[0] = 5,
	ships[1] = 4,
	ships[2] = 3,
	ships[3] = 3,
	ships[4] = 2
};

char user[10][10], computer[10][10];
char usercompview[10][10], compsideview[10][10]; //These are global so that they can be used without passing values through every single function

int main()
{
	intro();

	int yon, now = 0, nol = 0;
	cin >> yon; //Input from user asks the user to play, and if they want to continue playing.

	while (yon == 1)
	{
		game(now, nol);

		cin >> yon;
	}

	cout << "You won " << now << " times.\nYou lost " << nol << " times.\n";
	if (now > nol)
	{
		cout << "You really showed them who is in charge of these seas.";
	}
	if (nol < now)
	{
		cout << "I think that a demotion may be in your future. Your losses were great.\n";
	}

	return 0;
}

/***********************
* The layout of the game. Either compwin or userwin hits 17 to end the game.
*
*Preconditions - now, or number of wins, and nol, number of losses, must be given names. The other functions included in the function, clrscr(), reset(). grid(), gameprint(),
*chooseshot(), computerchoice(), and endgameprint() must work in order for this function to work. std library must be loaded. iostream loaded.
*Postconditions - Updates the arrays correctly. Outputs the correct messages.
*
***********************/
void game(int& now, int& nol)
{
	clrscr();

	int userwin = 0, compwin = 0, userorcomp = 0; //Resets the numbers that determine if you won or lost

	reset(); //Resets the game boards.
	grid(user, userorcomp); //Sets the ships for the user.
	grid(computer, userorcomp); //Sets the ships for the computer
	gameprint(); //Printst the gameboards on the screen.
	cout << "The user will go first.\n";

 while (userwin < 17 && compwin < 17) //17 is the total number of #'s for each player. One is added to either the computer or the users score until 17 is reached.
	{
		chooseshot(userwin, compwin);//The user chooses their shot.
		if (compwin < 17)//This is in case of surrender. Surrender counts as a victory for the computer, and this makes it so that if the user surrenders, the computer will quit firing.
		{
			computerchoice(compwin); //The computer chooses their shot.

			gameprint(); //Reprints the gameboards
		}
	}//This loop allows the game to continue.
 if (userwin == 17)//If you win, you get this message.
 {
	 cout << "You won! Congratulations!";
	 now++; //Keeps track of the number of your wins.
	 cout << "In total you have won " << now << " times, and the computer has won " << nol << " times.\n";
 }
 else if (compwin == 17)
 {
	 cout << "The computer won.\n";
	 nol++;//Keeps track of your losses.
	 cout << "In total you have won " << now << " times, and the computer has won " << nol << " times.\n";
 }
 endgameprint(); //So you see the most recent game's board
	cout << "Would you like to play again? 1 for yes, 2 for no.\n";
}

/***********************
*Sets the ships in the arrays, and creates the water surrounding the ships in the array.
*
*Preconditions - This requires that the different arrays be called so that they can have data stored in them. It also requires that shipplacecomp() and shipplaceuser() are in
* working condition. uoc, which determines if the user or computer ships are being placed. iostream and std library loaded.
*Postconditions - Updates the arrays user and computer correctly.
*
***********************/
void grid(char ps[][10], int& uoc)
{
	int e[5], f[5]; //These allow the different heads of the ships to be placed properly.
	
	if(uoc != 0) //There were issues with debugging that were fixed by making uoc being equal to two to allow the computer to place their ships.
	{
		shipplacecomp(e, f); //Places ships for the computer.
	}

	if (uoc == 0)
	{
		shipplaceuser(user); //Places the ships for the user.
		uoc++;
		/*for (int i = 0; i < 5; i++) I used this for debugging purposes only. To make sure that the ships were being placed in different positions than the ones
		* placed by the computer.
		{
			cout << s[i] << ' ' << q[i] << ' ' << lrud() << endl;
		}*/
	}

	for (int i = 0; i < 10; i++) //Sets the rest of the gameboard to being ~'s
	{
		for (int j = 0; j < 10; j++)
		{
			if (ps[i][j] != '#')
			{
				ps[i][j] = '~';
			}
		}
	}
}


/***********************
* Places the ships where they need to be in the user array.
*
*Preconditions - Requires that the user array is called so that data might be stored within. It also requires that clrscr() and validtest() are in working condition.
* iostream and std library loaded.
*Postconditions - Updates the array user correctly.
*
***********************/
void shipplaceuser(char placement[][10])
{
	int userx, usery, shipdirection, oom = 0;

	for (int d = 0; d < 5; d++)//Loops for each ship.
	{
		bool valid = false; //Resets the validity to be false, just in case.

		while (valid == false)//Checks to see if valid is false
		{
			int shiplength = ships[d]; //Sets an integer to be the length of the current ship.

			if (oom != 0)//Checks to see if this is the first or a different ship, solely for the output message.
			{
				cout << "Place your next ship.\n"; //Placed this first so that oom would be equal to zero
				cout << "Remember that 0 is up, 1 is down, 2 is to the right, and 3 is to the left.\n";
			}

			if (oom == 0)//oom stands for one or more, meaning, the first ship, or the ones after.
			{
				cout << "Insert where you would like to place your ships, given an x-coordinate 0-9, a y-coordinate 0-9, and a number for either up, down, left, or right,\n";
				cout << "which are denominated by a 0, 1, 2, 3, respectively. Remember to put a space between your x, y, and the direction of the ship.\n";
				cout << "Your ship placement will begin with the Carrier, which is 5 units in length, and move down to the Destroyer, which is 2 units in length.\n";
				oom++;//Adds onto oom so it knows which ship is being places, solely for the output message.
			}
			cin >> userx >> usery >> shipdirection; //User inputs their desired location and direction of their ship.

			valid = validtest(placement, shipdirection, userx, usery, shiplength);

			userx = userx - 1; //These allow the coordinates allow the correct coordinates to be placed
			usery = usery - 1;

			if (valid == true && shipdirection == 0 && (usery - shiplength) > 0)//Ship goes up.
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					placement[usery - (shiplength - 1)][userx] = '#';
				}
			}
			if (valid == true && shipdirection == 1 && (usery + (shiplength - 1)) < 10)//Ship goes down.
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					placement[usery + (shiplength - 1)][userx] = '#';
				}
			}
			if (valid == true && shipdirection == 2 && (userx - (shiplength-1)) > 0)//Ship goes left.
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					placement[usery][userx - (shiplength-1)] = '#';
				}
			}
			if (valid == true && shipdirection == 3 && (usery + (shiplength - 1)) < 10)//Ship goes right.
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					placement[usery][userx + (shiplength - 1)] = '#';
				}
			}
			if (valid == false)
			{
				cout << "Invalid placement. Please try placing your ship again.\n";
				d--;
			}
			if (valid == true)
			{
				clrscr();
				gameprint();
			}
			break;
		}//If valid comes back as false, it will ask for them to reenter their ship.
	}//This function will go until all of the users ships are placed.
	clrscr();
}

/***********************
*Places the ships where they need to be in the computer array.
*
*Preconditions - Requires that the computer array is called so that data might be stored within. It also requires an input of two arrays, size 5, be called and passed.
* validtest() and lrud() must be in working condition, and heavily requires the use of <time.h>, and iostream must be loaded.
*Postconditions - Updates the array computer correctly.
*
***********************/
void shipplacecomp(int x[], int y[])
{
	srand(time(NULL)); //Random seed generator

	x[0] = rand() % 10, x[1] = rand() % 10, x[2] = rand() % 10, x[3] = rand() % 10, x[4] = rand() % 10; //Sets the heads of the ships for the computer

	y[0] = rand() % 10, y[1] = rand() % 10, y[2] = rand() % 10, y[3] = rand() % 10, y[4] = rand() % 10;

	for (int compship = 0; compship < 5; compship++) //Pretty much same set up as last function, just a different way to get the x and y coordinates of the head of the ship.
	{
		bool valid;

		int shipdirection = lrud(), shiplength = ships[compship];

		int compx = x[compship], compy = y[compship];

		valid = validtest(computer, shipdirection, compx, compy, shiplength);

		if (valid == true)
		{
			if (shipdirection == 0)//Ship goes up
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					computer[y[compship] - (shiplength - 1)][x[compship]] = '#'; //Places the ship at the coordinate
				}
			}
			if (shipdirection == 1)//Ship goes down
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					computer[y[compship] + (shiplength - 1)][x[compship]] = '#';
				}
			}
			if (shipdirection == 2)//To the left
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					computer[y[compship]][x[compship] - (shiplength - 1)] = '#'; //This warning occurs because of the minus, which would allow the user to input larger numbers, but becuase the computer isn't allowed to, this warning amounts to nothing
				}
			}
			if (shipdirection == 3)//To the right
			{
				for (shiplength; shiplength > 0; shiplength--)
				{
					computer[y[compship]][x[compship] + (shiplength - 1)] = '#';
				}
			}
		}
		else if (valid == false)
		{
			x[compship] = rand() % 10; //This makes the computer rechoose an x coordinate if it's placement is invalid
			compship--;
		}
	}
}

/***********************
* Prints the data found in the user and usercompview arrays in a grid-like manner, with the correct borders.
*
*Preconditions - Requires that the user and usercompview arrays have data in them. std, iomanip, and iostream libraries must be loaded.
*Postconditions - Outputs the data and values correctly.
*
***********************/
void gameprint()
{
	int n = 1; //Easy way of outputting the correct number, resets every time this function is called.
	cout << setw(15) << "User" << setw(26) <<"Computer\n"; //Header of the game board
	cout << "x  1|2|3|4|5|6|7|8|9|10\tx 1|2|3|4|5|6|7|8|9|10\n"; //X axis for the game board.
	for (int gp = 0; gp < 10; gp++) //gp ~ gameprint
	{
		if (n < 10)
		{
			cout << n << ' ' << ' ';
		}
		else if (n == 10)//This allows the board to print correctly, because of the extra character in 10
		{
			cout << n << ' ';
		}
		for (int gp1 = 0; gp1 < 10; gp1++) //These output the gameboard visible to the user.
		{
			cout <<user[gp][gp1] << ' ';
		}
		if (n < 10)
		{
			cout << '\t' << n << ' ';
		}
		else if (n == 10)
		{
			cout << setw(2) << n << ' ';
		}
		for (int gp2 = 0; gp2 < 10; gp2++)
		{
			cout << usercompview[gp][gp2] << ' ';
		}
		//These two for loops above set the user and compuserview boards next to each other in front of the user.
		cout << endl;
		n++;//Adds one to n so that the y axis is being printed correctly.
	}
}

/***********************
*Chooses the places where the computer will fire and changes the data accordingly, from # to H, from ~ to M, wherever the computer hit or missed.
*
*Preconditions -Requires that the comp1 integer to be called and passed so that it can be modified throughout this function. Also requires the arrays have
* data in them. Uses <time.h> to choose random numbers. std library and iostream must be loaded.
*Postconditions - Updates the arrays correctly.
*
***********************/
void computerchoice(int& comp1)
{
	srand(time(NULL)); //Random seed generator
	int ccx = rand() % 10, ccy = rand() % 10; //Randomizes the computers shot
	while (user[ccy][ccx] == 'M' || user[ccy][ccx] == 'H') //If the computer chooses a place it has already chose, it will reset the computers choice
	{
		ccx = rand() % 10, ccy = rand() % 10;
	}
	if (user[ccy][ccx] != 'M' && user[ccy][ccx] != 'H')//Double verification that the computer did not choose the incorrect input.
	{
		cout << "The computer chose "<< ccx + 1 << ' ' << ccy + 1 << '.' <<endl; //Output of the computers choice

		if (user[ccy][ccx] == '#') //If the computer chose a spot where the ship was
		{
			user[ccy][ccx] = 'H';
			compsideview[ccy][ccx] = 'H';
			comp1++;
			cout << "HIT!" << endl;
		}

		if (user[ccy][ccx] == '~')//If the computer missed.
		{
			user[ccy][ccx] = 'M';
			compsideview[ccy][ccx] = 'M';
			cout << "MISS!" << endl;
		}
	}
}

/***********************
*Allowed the user to choose the places where the user will fire and changes the data accordingly, from # to H, from ~ to M, wherever the user
*hit or missed.
*
*Preconditions - Requires that the comp1 and user1 integers to be called and passed so that they can be modified throughout this function. Also requires the arrays have
* data in them. std library and iostream must be loaded.
*Postconditions - Changes the values in the arrays correctly.
*
***********************/
void chooseshot(int& user1, int&comp1)
{
	int xval, yval, shosu;
	bool hasshot = false; //Resets hasshot to false at the beginning of the function

	while (hasshot == false) //Loop allows for you to shoot
	{
		cout << "Press 1 to shoot, or 2 to surrender.\n";//Allows the user to choose to continue the game or surrender.
		cin >> shosu;//shosu ~ shoot or surrender

		if (shosu == 1)//Checks if the user was to fire or surrender. If the user chose to fire, the following happens.
		{
			cout << "Where would you like to shoot? Put a space, or an enter, between your x and y values.\n";
			cin >> xval >> yval;
			while (0 > yval - 1 || yval - 1 > 10 || 0 > xval - 1 || xval - 1 > 10)//Makes sure that the user inputs correctly
			{
				cout << "I'm sorry, that is out of our range of fire. Please input valid coordinates.\n";
				cin >> xval >> yval;
			}
			while (usercompview[yval - 1][xval - 1] == 'M' || usercompview[yval][xval] == 'H')//If the user chooses a place where they have already chosen, this will ask for new inputs
			{
				cout << "You have already shot there. Choose another spot.\n";
				cin >> xval >> yval;
			}
			if (usercompview[yval - 1][xval - 1] != 'M' && usercompview[yval][xval] != 'H')//If the user has not chosen this location.
			{
				if (computer[yval - 1][xval - 1] != '#')//If the user missed.
				{
					usercompview[yval - 1][xval - 1] = 'M';
					//computer[yval - 1][xval - 1] = 'M'; I chose to not display the where the user hit or missed on the computers screen, so that it makes more sense
					cout << "MISS!\n";
					hasshot = true;
				}
				if (computer[yval - 1][xval - 1] == '#')//If the user hits one of the computers ships.
				{
					usercompview[yval - 1][xval - 1] = 'H';
					//computer[yval - 1][xval - 1] = 'H'; Again, I chose not to display the hits and misses
					user1++;
					cout << "HIT!\n";
					hasshot = true;
				}

			}
		}
		else if (shosu == 2)//If the user decided to surrender.
		{
			cout << "You have chosen to surrender.\nYour superiors will be most displeased.\n";
			comp1 = 17;//The computer wins if the user quits.
			hasshot = true; //This allows the loop to end	
		}
		else if (!(cin >> shosu))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clears and ignores invalid inputs, such as letters or characters of other kinds.
			cout << "If you don't listen to orders, you might just find yourself being demoted.\nNow, 1 to shoot, 2 to surrender.\n";
			cin >> shosu;
		}
	}
}

/***********************
*Returns a random number, 0 to 3, to be used to choose direction of the computers ship.
*
*Preconditions - Requires that <time.h> be included. iostream must be included.
*Postconditions - Returns a random number.
*
***********************/
int lrud()//left, right, up, or down. lrud
{
	int ranno = rand() % 4;
	return ranno;
}

/***********************
* Pushes the output screen down so that it is a clear screen.
*
*Preconditions - Requires the computer to be able to execute this code. Most machines allow this. May be an issue for Apple or Linux, no way to debug.
* std library loaded.
*Postconditions - Pushes the output screen down so that it is a clear screen.
*
***********************/
void clrscr()
{
	cout << "\033[2J\033[1;1H"; //Code I found online to push the output to a blank screen. Cleans up the output a little bit I think.
}

/***********************
*Displays the following messages to the screen.
*
*Preconditions - Requires that std library be loaded. iostream loaded.
*Postconditions - Outputs the following messages.
*
***********************/
void intro() //Introduction screen.
{
	cout << "Welcome to Battleship. This is a game between you and a computer.\n";
	cout << "If you are not familiar with Battleship, the rules are rather simple.\n";
	cout << "You choose an x-value(0-9), and a y-value(0-9) to choose where to fire to shoot the target's ship.\n";
	cout << "There are five ships. You must sink all of your opponents ships to win.\n";
	cout << "The five ships are as follows.\n";
	cout << "Ship\t\tLength\n";
	cout << "Carrier\t\t5\nBattleship\t4\nCruiser\t\t3\nSubmarine\t3\nDestroyer\t2\n";
	cout << "Are you up for this challenge? Press 1 for yes, 2 for no.\n";
}

/***********************
* Fills or resets the arrays with tilda's.
*
*Preconditions - Requires that all of the arrays be initiated to input data in them. Requires iostream.
*Postconditions - Fills or resets the arrays with tilda's.
*
***********************/
void reset() //Resets the gameboards to just open water.
{
	for (int ucv1 = 0; ucv1 < 10; ucv1++)
	{
		for (int ucv2 = 0; ucv2 < 10; ucv2++)
		{
			usercompview[ucv1][ucv2] = '~';
			user[ucv1][ucv2] = '~';
			computer[ucv1][ucv2] = '~';
			compsideview[ucv1][ucv2] = '~';
		}
	}
}

/***********************
* Tests the placement of the ship.
*
*Preconditions - Requires the different arrays, namely the user and computer arrays, to have data in them. Also requires that the different data be filled according to
* whichever array is being tested. iostream loaded.
*Postconditions - Returns as false if the ship is not placed correctly on the game board, or true if it is placed correctly.
*
***********************/
bool validtest(char valid[][10], int& shipd, int& x, int& y, int shipl)
{
	bool validity = false;

	if (shipd == 0 && (y - shipl) > 0)//Ship goes down.
	{
		for (shipl; shipl > 0; shipl--)
		{
			if (valid[y - (shipl - 1)][x] != '#')
			{
				validity = true;
			}
			else
			{
				validity = false;
				return validity;
			}
		}
	}
	else if (shipd == 1 && (y + (shipl - 1)) < 10)//Ship goes up.
	{
		for (shipl; shipl > 0; shipl--)
		{
			if (valid[y + (shipl - 1)][x] != '#')
			{
				validity = true;
			}
			else
			{
				validity = false;
				return validity;
			}
		}
	}
	else if (shipd== 2 && (x - (shipl - 1)) > 0)//Ship goes left.
	{
		for (shipl; shipl > 0; shipl--)
		{
			if (valid[y][x - (shipl - 1)] != '#')
			{
				validity = true;
			}
			else
			{
				validity = false;
				return validity;
			}
		}
	}
	else if (shipd == 3 && (x + (shipl - 1)) < 10)//Ship goes right.
	{
		for (shipl; shipl > 0; shipl--)
		{
			if (valid[y][x + (shipl - 1)] != '#')
			{
				validity = true;
			}
			else
			{
				validity = false;
				return validity;
			}
		}
	}

	return validity;
}

/***********************
* Outputs the data found in the user, computer, usercompview, and compsideview arrays in a grid like manner, with the correct borders.
*
*Preconditions - Requires endgameprint() be in working order. It also requires that the user, computer, usercompview, and compsideview arrays are called
*and have data in them. Also requires that clrscr() is in working condition.  std, iomanip, and iostream libraries must be loaded.
*Postconditions - Outputs the data and values correctly.
*
***********************/
void endgameprint()
{
	clrscr();

	int n = 1; //Easy way of outputting the correct number, resets every time this function is called.
	cout << setw(15) << "User" << setw(26) << "Computer\n"; //Header of the game board
	cout << "x  1|2|3|4|5|6|7|8|9|10\tx 1|2|3|4|5|6|7|8|9|10\n"; //X axis for the game board.
	for (int egp = 0; egp < 10; egp++) //gp ~ gameprint
	{
		if (n < 10)
		{
			cout << n << ' ' << ' ';
		}
		else if (n == 10)//This allows the board to print correctly, because of the extra character in 10
		{
			cout << n << ' ';
		}
		for (int egp1 = 0; egp1 < 10; egp1++) //These output the gameboard visible to the user.
		{
			cout << user[egp][egp1] << ' ';
		}
		if (n < 10)
		{
			cout << '\t' << n << ' ';
		}
		else if (n == 10)
		{
			cout << setw(2) << n << ' ';
		}
		for (int egp2 = 0; egp2 < 10; egp2++)
		{
			cout << usercompview[egp][egp2] << ' ';
		}
		cout << endl;
		n++;
	}
	cout << endl;
	n = 1;
	cout << setw(16) << "Computer" << setw(22) << "User\n";
	cout << "x  1|2|3|4|5|6|7|8|9|10\tx 1|2|3|4|5|6|7|8|9|10\n";
	for (int endgp = 0; endgp < 10; endgp++) //gp ~ gameprint
	{
		if (n < 10)
		{
			cout << n << ' ' << ' ';
		}
		else if (n == 10)//This allows the board to print correctly, because of the extra character in 10
		{
			cout << n << ' ';
		}
		for (int endgp1 = 0; endgp1 < 10; endgp1++) //These output the gameboard visible to the user.
		{
			cout << computer[endgp][endgp1] << ' ';
		}
		if (n < 10)
		{
			cout << '\t' << n << ' ';
		}
		else if (n == 10)
		{
			cout << setw(2) << n << ' ';
		}
		for (int endgp2 = 0; endgp2 < 10; endgp2++)
		{
			cout << compsideview[endgp][endgp2] << ' ';
		}
		cout << endl;
		n++;
	}
	cout << endl << endl;
}
