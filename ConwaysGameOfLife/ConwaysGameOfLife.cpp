// ConwaysGameOfLife.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <string>


using namespace std;

struct Position
{
	int x = 0;
	int y = 0;
};
struct Velocity
{
	int x = 0;
	int y = 0;
};
vector<bool> rows(30, 0);
vector<vector<bool>> game;
Position particle;
Velocity velocity;

void makeGame();
void showGame();
void conways_rules();
void fast_forward(int);
int neighborhood(int, int);
//functions to set the initial conditions (you get a choice here!!)
void force_field(int, int);
void inertia(int, int);
void seeder();
//this seems to work better than the Newtonian initializer
void seed_edge();
void initializer(int, int, int, int, int);

int main()
{
	makeGame();
	cout << "Welcome to Conway's game of life\n" << endl;
	cout << "The game of life was invented by John H Conway to understand how simple rules can lead to complex behavior.\n" << endl; 
	cout<<"Rules\n1. Any live cell with fewer than two live neighbours dies, as if by underpopulation." << endl;
	cout << "2. Any live cell with two or three live neighbours lives on to the next generation." << endl;
	cout << "3. Any live cell with more than three live neighbours dies, as if by overpopulation." << endl;
	this_thread::sleep_for(1s);
	cout << "Do you want to seed the edges of the game board?y/n" << endl;
	char yes_or_no = ' ';
	cin >> yes_or_no;
	if(yes_or_no=='y')
		seed_edge();
	else
	//a lot of parameter choices lead to the screen going dark. Analogue of cosmological anthropic principle?
	 initializer(500, 0, 2, -1, 2);
		
	showGame();	
	this_thread::sleep_for(2s);
	cout << "Would you like to run continuously?y/n" << endl;
	cin >> yes_or_no;
	if (yes_or_no == 'y')
	{
		showGame();
		cout << "Here goes" << endl;
		this_thread::sleep_for(1s);
		system("CLS");
		while (true)
		{
			conways_rules();
			showGame();
			system("CLS");
		}
	}
	else {
		int iterations;
		cout << "How many iterations would you like to run for" << endl;
		cin >> iterations;
		fast_forward(abs(iterations));
		showGame();
		
	}
	
}
void makeGame()
{
	for (size_t i = 0; i < 30; i++)
	{
		
			game.push_back(rows);
		
	}
}
void showGame()
{
	for (size_t i = 0; i < game.size(); i++)
	{
		for (size_t j = 0; j < game.size(); j++)
		{
			
			if (game[i][j])
			{
				cout << "*";
			}
			
			else {
				cout << " ";
			}
		}
		cout << endl;

	}
}

void conways_rules()
{
	for (size_t i = 0; i < game.size(); i++)
	{
		for (size_t j = 0; j < game.size(); j++)
		{
			//if a cell has less than 2 and more than three neighbors it dies
			if (game[i][j] && neighborhood(i, j) < 2 || neighborhood(i, j) > 3)
			{
				game[i][j] = false;
			}
			//Reproduction mechanism if a dead cell has exactly 3 live neighbors it comes to life
			//more simply cells in CGL reproduce by having threesomes (:
			if (!game[i][j] && neighborhood(i, j) == 3)
			{
				game[i][j] = true;
			}
		}
	}
}

void fast_forward(int iterations)
{
	for (size_t i = 0; i < iterations; i++)
	{
		conways_rules();
		
	}
	
}

int neighborhood(int i, int j)
{
	int  neighbors= 0;
	
	if (i > 0 && i < game.size() - 1 && j > 0 && j < game.size() - 1)
	{
		for (size_t k = i-1; k <= i+1; k++)
		{
			for (size_t l = j-1; l <= j+1; l++)
			{
				if (game[k][l] && !(k == i && l == j))
				{
					
					neighbors++;
				}
			}
		}
	}
	return neighbors;
}

void force_field(int g_x, int g_y)
{
	velocity.x += g_x;
	velocity.y += g_y;
}

void inertia(int g_x, int g_y)
{
	if (particle.x + velocity.x >= game.size()-1
		|| particle.x + velocity.x<=0)
	{
		velocity.x = -velocity.x -g_x;
	}
	if (particle.y + velocity.y >= game.size()-1
		|| particle.y + velocity.y <= 0)
	{
		velocity.y = -velocity.y -g_y;
	}
	particle.x += velocity.x;
	particle.y += velocity.y;
}

void seeder()
{
	for (size_t i = 0; i < game.size(); i++)
	{
		for (size_t j = 0; j < rows.size(); j++)
		{
			if (i == particle.x && j == particle.y)
			{
				game[i][j] = true;
			}
		}
	}
}

void seed_edge()
{
	for (size_t i = 0; i < game.size(); i++)
	{
		for (size_t j = 0; j < rows.size(); j++)
		{
			
			bool edges = i >= game.size() - 1 && i<game.size()||j<game.size()&& j >= game.size() - 1|| i <= 1 && i>=0 || j>=0 && j <= 1;
			if (edges)
			{
				game[i][j] = true;
			}
		}
	}
}

void initializer(int density, int g_x, int g_y, int init_v_x, int init_v_y)
{
	//this works by "shooting" a virtual projectile into the game space,
	//applying an arbitrary force field to it and taking snapshots as it bounces around
	velocity.x = init_v_x;
	velocity.y = init_v_y;
	for (size_t i = 0; i < density; i++)
	{
		inertia(g_x, g_y);
		force_field(g_x, g_y);
		//this function takes the requisit snapshots, which become live cells
		seeder();
	}
}
