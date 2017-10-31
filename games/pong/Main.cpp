//////////////////////////////////////////////////////////////////////////////////
// Project: Paddle Battle (Pong)
// File:    Main.cpp
// To run: cl /EHsc pong.cpp
//////////////////////////////////////////////////////////////////////////////////

// These three lines link in the required SDL components for our project. //
// Alternatively, we could have linked them in our project settings.      //
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_TTF.lib")

#include <stack>     // We'll use the STL stack to store our function pointers
#include "SDL.h"     // Main SDL header 
#include "SDL_TTF.h" // True Type Font header
#include "math.h"    // We'll be using abs()
#include "time.h"    // For seeding the random generator
#include "Defines.h" // Our defines header
#include "Enums.h"   // Our enums header

using namespace std;   

// The STL stack can't take a function pointer as a type //
// so we encapsulate a function pointer within a struct. //
struct StateStruct 
{
	void (*StatePointer)();
};

// Struct that represents an entity in our game //
struct Entity
{
	SDL_Rect screen_location;  // location on screen
	SDL_Rect bitmap_location;  // location of image in bitmap 

	int x_speed;
	int y_speed; 
};

// Global data //
stack<StateStruct> g_StateStack;     // Our state stack
SDL_Surface*       g_Bitmap = NULL;  // Our background image
SDL_Surface*       g_Window = NULL;  // Our backbuffer
SDL_Event		   g_Event;          // An SDL event structure for input
int				   g_Timer;          // Our timer is just an integer
Entity             g_Computer;       // The computer's paddle
Entity             g_Player;         // The player's paddle
Entity             g_Ball;           // The game ball
int                g_ComputerScore;  // AI's score
int                g_PlayerScore;    // Player's score

// Functions to handle the states of the game //
void Menu();
void Game();
void Exit();
void GameWon();
void GameLost();

// Helper functions for the main game state functions //
void ClearScreen();
void DisplayText(string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB);
void HandleMenuInput();
void HandleGameInput();
void HandleExitInput();
void HandleWinLoseInput();
bool CheckWallCollisions(Entity& entity, Direction dir);
bool CheckBallCollisions(Entity& paddle);
void HandleBall();
void MoveBall();
void HandlePlayerScore();
void HandleComputerScore();
void HandleAI();

// Init and Shutdown functions //
void Init();
void Shutdown();

int main(int argc, char **argv)
{
	Init();
	
	// Our game loop is just a while loop that breaks when our state stack is empty. //
	while (!g_StateStack.empty())
	{
		g_StateStack.top().StatePointer();		
	}

	Shutdown();

	return 0;
}


// This function initializes our game. //
void Init()
{
	// Initiliaze SDL video and our timer. //
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER);
	// Setup our window's dimensions, bits-per-pixel (0 tells SDL to choose for us), //
	// and video format (SDL_ANYFORMAT leaves the decision to SDL). This function    //
	// returns a pointer to our window which we assign to g_Window.                  //
	g_Window = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_ANYFORMAT);    
	// Set the title of our window. //
	SDL_WM_SetCaption(WINDOW_CAPTION, 0);
	// Get the number of ticks since SDL was initialized. //
	g_Timer = SDL_GetTicks();

	// Seed the random generator //
	srand( time(0) );

	// Initialize the screen locations of two paddles and the ball //
	g_Computer.screen_location.x = (WINDOW_WIDTH / 2) - (PADDLE_WIDTH / 2);  // center screen
	g_Computer.screen_location.y = COMPUTER_Y;
	g_Computer.screen_location.w = PADDLE_WIDTH;
	g_Computer.screen_location.h = PADDLE_HEIGHT;

	g_Player.screen_location.x = (WINDOW_WIDTH / 2) - (PADDLE_WIDTH / 2);   // center screen
	g_Player.screen_location.y = PLAYER_Y;
	g_Player.screen_location.w = PADDLE_WIDTH;
	g_Player.screen_location.h = PADDLE_HEIGHT;

	g_Ball.screen_location.x = (WINDOW_WIDTH / 2) - (BALL_DIAMETER / 2);   // center screen
	g_Ball.screen_location.y = (WINDOW_HEIGHT / 2) - (BALL_DIAMETER / 2);  // center screen
	g_Ball.screen_location.w = BALL_DIAMETER;
	g_Ball.screen_location.h = BALL_DIAMETER;

	// Initialize the image location rects //
	g_Computer.bitmap_location.x = PADDLE_BITMAP_X;
	g_Computer.bitmap_location.y = PADDLE_BITMAP_Y;
	g_Computer.bitmap_location.w = PADDLE_WIDTH;
	g_Computer.bitmap_location.h = PADDLE_HEIGHT;

	g_Player.bitmap_location.x = PADDLE_BITMAP_X;
	g_Player.bitmap_location.y = PADDLE_BITMAP_Y;
	g_Player.bitmap_location.w = PADDLE_WIDTH;
	g_Player.bitmap_location.h = PADDLE_HEIGHT;

	g_Ball.bitmap_location.x = BALL_BITMAP_X;
	g_Ball.bitmap_location.y = BALL_BITMAP_Y;
	g_Ball.bitmap_location.w = BALL_DIAMETER;
	g_Ball.bitmap_location.h = BALL_DIAMETER;

	// Initialize speeds //
	g_Computer.x_speed = COMPUTER_SPEED;
	g_Player.x_speed   = PLAYER_SPEED;
	g_Ball.x_speed     = 0;
	g_Ball.y_speed     = 0;

	// Set scores to zero //
	g_ComputerScore = 0;
	g_PlayerScore   = 0;

	// Fill our bitmap structure with information. //
	g_Bitmap = SDL_LoadBMP("data/PaddleBattle.bmp");	

	// Set our transparent color (magenta) //
	SDL_SetColorKey( g_Bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(g_Bitmap->format, 255, 0, 255) );

	// We start by adding a pointer to our exit state, this way //
	// it will be the last thing the player sees of the game.   //
	StateStruct state;
	state.StatePointer = Exit;
	g_StateStack.push(state);

	// Then we add a pointer to our menu state, this will //
	// be the first thing the player sees of our game.    //
	state.StatePointer = Menu;
	g_StateStack.push(state);

	// Initialize the true type font library. //
	TTF_Init();
}

// This function shuts down our game. //
void Shutdown()
{
	// Shutdown the true type font library. //
	TTF_Quit();

	// Free our surfaces. //
	SDL_FreeSurface(g_Bitmap);
	SDL_FreeSurface(g_Window);

	// Tell SDL to shutdown and free any resources it was using. //
	SDL_Quit();
}

// This function handles the game's main menu. From here //
// the player can select to enter the game, or quit.     //
void Menu()
{
	// Here we compare the difference between the current time and the last time we //
	// handled a frame. If FRAME_RATE amount of time has, it's time for a new frame. //
	if ( (SDL_GetTicks() - g_Timer) >= FRAME_RATE )
	{
		HandleMenuInput();

		// Make sure nothing from the last frame is still drawn. //
		ClearScreen();

		DisplayText("Start (G)ame", 350, 250, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("(Q)uit Game",  350, 270, 12, 255, 255, 255, 0, 0, 0);
			
		// Tell SDL to display our backbuffer. The four 0's will make //
		// SDL display the whole screen. //
		SDL_UpdateRect(g_Window, 0, 0, 0, 0);

		// We've processed a frame so we now need to record the time at which we did it. //
		// This way we can compare this time the next time our function gets called and  //
		// see if enough time has passed between iterations. //
		g_Timer = SDL_GetTicks();
	}	
}

// This function handles the main game. We'll control the   //
// drawing of the game as well as any necessary game logic. //
void Game()
{	
	// Here we compare the difference between the current time and the last time we //
	// handled a frame. If FRAME_RATE amount of time has, it's time for a new frame. //
	if ( (SDL_GetTicks() - g_Timer) >= FRAME_RATE )
	{
		HandleGameInput();

		HandleBall();

		HandleAI();

		// Make sure nothing from the last frame is still drawn. //
		ClearScreen();

		// Draw the two paddles and the ball //
		SDL_BlitSurface(g_Bitmap, &g_Computer.bitmap_location, g_Window, &g_Computer.screen_location);
		SDL_BlitSurface(g_Bitmap, &g_Player.bitmap_location,   g_Window, &g_Player.screen_location);
		SDL_BlitSurface(g_Bitmap, &g_Ball.bitmap_location,     g_Window, &g_Ball.screen_location);

		// Output the computer and player scores //
		char buffer[256];

		string c_score = "Computer Score: ";
		itoa(g_ComputerScore, buffer, 10);
		c_score.append(buffer);

		string p_score = "Player Score: ";
		itoa(g_PlayerScore, buffer, 10);
		p_score.append(buffer);

		DisplayText(c_score, COMPUTER_SCORE_X, COMPUTER_SCORE_Y, 12, 66, 239, 16, 0, 0, 0);
		DisplayText(p_score, PLAYER_SCORE_X,   PLAYER_SCORE_Y,   12, 66, 239, 16, 0, 0, 0);		

		// Tell SDL to display our backbuffer. The four 0's will make //
		// SDL display the whole screen. //
		SDL_UpdateRect(g_Window, 0, 0, 0, 0);

		// We've processed a frame so we now need to record the time at which we did it. //
		// This way we can compare this time the next time our function gets called and  //
		// see if enough time has passed between iterations. //
		g_Timer = SDL_GetTicks();
	}	
}

// This function handles the game's exit screen. It will display //
// a message asking if the player really wants to quit.          //
void Exit()
{	
	// Here we compare the difference between the current time and the last time we //
	// handled a frame. If FRAME_RATE amount of time has, it's time for a new frame. //
	if ( (SDL_GetTicks() - g_Timer) >= FRAME_RATE )
	{
		HandleExitInput();

		// Make sure nothing from the last frame is still drawn. //
		ClearScreen();

		DisplayText("Quit Game (Y or N)?", 350, 260, 12, 255, 255, 255, 0, 0, 0);

		// Tell SDL to display our backbuffer. The four 0's will make //
		// SDL display the whole screen. //
		SDL_UpdateRect(g_Window, 0, 0, 0, 0);

		// We've processed a frame so we now need to record the time at which we did it. //
		// This way we can compare this time the next time our function gets called and  //
		// see if enough time has passed between iterations. //
		g_Timer = SDL_GetTicks();
	}	
}

// Display a victory message. //
void GameWon()
{
	if ( (SDL_GetTicks() - g_Timer) >= FRAME_RATE )
	{
		HandleWinLoseInput();

		ClearScreen();

		DisplayText("You Win!!!", 350, 250, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("Quit Game (Y or N)?", 350, 270, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateRect(g_Window, 0, 0, 0, 0);

		g_Timer = SDL_GetTicks();
	}	
}

// Display a game over message. //
void GameLost()
{	
	if ( (SDL_GetTicks() - g_Timer) >= FRAME_RATE )
	{
		HandleWinLoseInput();

		ClearScreen();

		DisplayText("You Lose.", 350, 250, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("Quit Game (Y or N)?", 350, 270, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateRect(g_Window, 0, 0, 0, 0);

		g_Timer = SDL_GetTicks();
	}	
}

// This function simply clears the back buffer to black. //
void ClearScreen()
{
	// This function just fills a surface with a given color. The //
	// first 0 tells SDL to fill the whole surface. The second 0  //
	// is for black. //
	SDL_FillRect(g_Window, 0, 0);
}

// This function displays text to the screen. It takes the text //
// to be displayed, the location to display it, the size of the //
// text, and the color of the text and background.              //
void DisplayText(string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB) 
{
	// Open our font and set its size to the given parameter. //
    TTF_Font* font = TTF_OpenFont("arial.ttf", size);

	SDL_Color foreground  = { fR, fG, fB};   // Text color. //
	SDL_Color background  = { bR, bG, bB };  // Color of what's behind the text. //

	// This renders our text to a temporary surface. There //
	// are other text functions, but this one looks nice.  //
	SDL_Surface* temp = TTF_RenderText_Shaded(font, text.c_str(), foreground, background);

	// A structure storing the destination of our text. //
	SDL_Rect destination = { x, y, 0, 0 };
	
	// Blit the text surface to our window surface. //
	SDL_BlitSurface(temp, NULL, g_Window, &destination);

	// Always free memory! //
	SDL_FreeSurface(temp);

	// Close the font. //
	TTF_CloseFont(font);
}

// This function receives player input and //
// handles it for the game's menu screen.  //
void HandleMenuInput() 
{
	// Fill our event structure with event information. //
	if ( SDL_PollEvent(&g_Event) )
	{
		// Handle user manually closing game window //
		if (g_Event.type == SDL_QUIT)
		{			
			// While state stack isn't empty, pop //
			while (!g_StateStack.empty())
			{
				g_StateStack.pop();
			}

			return;  // game is over, exit the function
		}

		// Handle keyboard input here //
		if (g_Event.type == SDL_KEYDOWN)
		{
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_StateStack.pop();
				return;  // this state is done, exit the function 
			}
			// Quit //
			if (g_Event.key.keysym.sym == SDLK_q)
			{
				g_StateStack.pop();
				return;  // game is over, exit the function 
			}
			// Start Game //
			if (g_Event.key.keysym.sym == SDLK_g)
			{
				StateStruct temp;
				temp.StatePointer = Game;
				g_StateStack.push(temp);
				return;  // this state is done, exit the function 
			}
		}
	}
}

// This function receives player input and //
// handles it for the main game state.     //
void HandleGameInput() 
{
	static bool left_pressed  = false;
	static bool right_pressed = false;

	// Fill our event structure with event information. //
	if ( SDL_PollEvent(&g_Event) )
	{
		// Handle user manually closing game window //
		if (g_Event.type == SDL_QUIT)
		{			
			// While state stack isn't empty, pop //
			while (!g_StateStack.empty())
			{
				g_StateStack.pop();
			}

			return;  // game is over, exit the function
		}

		// Handle keyboard input here //
		if (g_Event.type == SDL_KEYDOWN)
		{
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_StateStack.pop();
				
				return;  // this state is done, exit the function 
			}	
			if (g_Event.key.keysym.sym == SDLK_SPACE)
			{
				// Player can hit 'space' to make the ball move at start //
				if (g_Ball.y_speed == 0)
					g_Ball.y_speed = BALL_SPEED_Y;
			}
			if (g_Event.key.keysym.sym == SDLK_LEFT)
			{
				left_pressed = true;
			}
			if (g_Event.key.keysym.sym == SDLK_RIGHT)
			{
				right_pressed = true;
			}
		}
		if (g_Event.type == SDL_KEYUP)
		{
			if (g_Event.key.keysym.sym == SDLK_LEFT)
			{
				left_pressed = false;
			}
			if (g_Event.key.keysym.sym == SDLK_RIGHT)
			{
				right_pressed = false;
			}
		}		
	}

	// This is where we actually move the paddle //
	if (left_pressed)
	{
		if ( !CheckWallCollisions(g_Player, LEFT) )
		{
			g_Player.screen_location.x -= PLAYER_SPEED;
		}
	}
	if (right_pressed)
	{
		if ( !CheckWallCollisions(g_Player, RIGHT) )
		{
			g_Player.screen_location.x += PLAYER_SPEED;
		}
	}
}

// This function receives player input and //
// handles it for the game's exit screen.  //
void HandleExitInput() 
{
	// Fill our event structure with event information. //
	if ( SDL_PollEvent(&g_Event) )
	{
		// Handle user manually closing game window //
		if (g_Event.type == SDL_QUIT)
		{			
			// While state stack isn't empty, pop //
			while (!g_StateStack.empty())
			{
				g_StateStack.pop();
			}

			return;  // game is over, exit the function
		}

		// Handle keyboard input here //
		if (g_Event.type == SDL_KEYDOWN)
		{
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_StateStack.pop();
				
				return;  // this state is done, exit the function 
			}
			// Yes //
			if (g_Event.key.keysym.sym == SDLK_y)
			{
				g_StateStack.pop();
				return;  // game is over, exit the function 
			}
			// No //
			if (g_Event.key.keysym.sym == SDLK_n)
			{
				StateStruct temp;
				temp.StatePointer = Menu;
				g_StateStack.push(temp);
				return;  // this state is done, exit the function 
			}
		}
	}
}

// Input handling for win/lose screens. //
void HandleWinLoseInput()
{
	if ( SDL_PollEvent(&g_Event) )
	{
		// Handle user manually closing game window //
		if (g_Event.type == SDL_QUIT)
		{			
			// While state stack isn't empty, pop //
			while (!g_StateStack.empty())
			{
				g_StateStack.pop();
			}

			return;  
		}

		// Handle keyboard input here //
		if (g_Event.type == SDL_KEYDOWN)
		{
			if (g_Event.key.keysym.sym == SDLK_ESCAPE)
			{
				g_StateStack.pop();
				
				return;  
			}
			if (g_Event.key.keysym.sym == SDLK_y)
			{
				g_StateStack.pop();
				return;  
			}
			// If player chooses to continue playing, we pop off    //
			// current state and push exit and menu states back on. //
			if (g_Event.key.keysym.sym == SDLK_n)
			{
				g_StateStack.pop();

				StateStruct temp;
				temp.StatePointer = Exit;
				g_StateStack.push(temp);

				temp.StatePointer = Menu;
				g_StateStack.push(temp);
				return;  
			}
		}
	}
}

// Check to see if a game object is going to hit the side of the screen //
bool CheckWallCollisions(Entity& entity, Direction dir)
{
	int temp_x; // stores the location of the entity after moving
	
	// Get the location of the entity after it moves //
	switch (dir)
	{
		case LEFT:
		{
			temp_x = entity.screen_location.x - entity.x_speed;
		} break;
		case RIGHT:
		{
			// Notice that we have to add the entities width to get its right(->) coordinate //
			temp_x = entity.screen_location.x + entity.screen_location.w + entity.x_speed;
		} break;
	}

	if ( (temp_x <= 0) || (temp_x >= WINDOW_WIDTH) )
	{
		return true;
	}

	return false;
}

// Check to see if the ball is going to hit a paddle //
bool CheckBallCollisions(Entity& paddle)
{
	// Temporary values to keep things tidy //
	int ball_x      = g_Ball.screen_location.x;
	int ball_y      = g_Ball.screen_location.y;
	int ball_width  = g_Ball.screen_location.w;
	int ball_height = g_Ball.screen_location.h;
	int ball_speed  = g_Ball.y_speed;

	int paddle_x      = paddle.screen_location.x;
	int paddle_y      = paddle.screen_location.y;	
	int paddle_width  = paddle.screen_location.w;
	int paddle_height = paddle.screen_location.h;

	// Get which paddle we're checking against //
	if ( paddle.screen_location.y == PLAYER_Y)
	{
		// Check to see if ball is in Y range of the player's paddle. //
		// We check its speed to see if it's even moving towards the player's paddle. //
		if ( (ball_speed > 0) && (ball_y + ball_height >= paddle_y) && 
			 (ball_y + ball_height <= paddle_y + paddle_height) )        // side hit
		{
			// If ball is in the X range of the paddle, return true. //
			if ( (ball_x <= paddle_x + paddle_width) && (ball_x + ball_width >= paddle_x) )
			{
				return true;
			}
		}
	}
	else
	{
		// Check to see if ball is in Y range of the computer's paddle. //
		// We check its speed to see if it's even moving towards the computer's paddle. //
		if ( (ball_speed < 0) && (ball_y >= paddle_y) && 
			 (ball_y <= paddle_y + paddle_height) ) 
		{
			// If ball is in the X range of the paddle, return true. //
			if ( (ball_x <= paddle_x + paddle_width) && (ball_x + ball_width >= paddle_x) )
			{
				return true;
			}
		}
	}

	return false;
}

void HandleBall()
{	
	// Start by moving the ball //
	MoveBall();	

	if ( CheckBallCollisions(g_Player) )	
	{
		// Get center location of paddle //
	    int paddle_center = g_Player.screen_location.x + g_Player.screen_location.w / 2;
		int ball_center = g_Ball.screen_location.x + g_Ball.screen_location.w / 2;

		// Find the location on the paddle that the ball hit //
		int paddle_location = ball_center - paddle_center;

		// Increase X speed according to distance from center of paddle. //
		g_Ball.x_speed = paddle_location / BALL_SPEED_MODIFIER;
		g_Ball.y_speed = -g_Ball.y_speed;
	}

	if ( CheckBallCollisions(g_Computer) )
	{
		// Get center location of paddle //
	    int paddle_center = g_Computer.screen_location.x + g_Computer.screen_location.w / 2;
		int ball_center = g_Ball.screen_location.x + g_Ball.screen_location.w / 2;

		// Find the location on the paddle that the ball hit //
		int paddle_location = ball_center - paddle_center;

		// Increase X speed according to distance from center of paddle. //
		g_Ball.x_speed = paddle_location / BALL_SPEED_MODIFIER;
		g_Ball.y_speed = -g_Ball.y_speed;
	}

	// Check to see if someone has scored //
	if (g_Ball.screen_location.y < 0)
	{
		HandlePlayerScore();
	}
	if (g_Ball.screen_location.y + g_Ball.screen_location.h > WINDOW_HEIGHT)
	{
		HandleComputerScore();
	}
}

void MoveBall()
{
	g_Ball.screen_location.x += g_Ball.x_speed;
	g_Ball.screen_location.y += g_Ball.y_speed;	

	// If the ball is moving left, we see if it hits the wall. If does, //
	// we change its direction. We do the same thing if it's moving right. //
	if ( ( (g_Ball.x_speed < 0) && CheckWallCollisions(g_Ball, LEFT) ) || 
		 ( (g_Ball.x_speed > 0) && CheckWallCollisions(g_Ball, RIGHT) ) )
	{
		g_Ball.x_speed = -g_Ball.x_speed;
	}
}

// Increase the player's score, reset ball,  and see if player has won. //
void HandlePlayerScore()
{
	// Increase score //
	g_PlayerScore++;

	// Reset ball //
	g_Ball.x_speed = 0;
	g_Ball.y_speed = 0;
	g_Ball.screen_location.x = (WINDOW_WIDTH / 2) - (BALL_DIAMETER / 2);
	g_Ball.screen_location.y = (WINDOW_HEIGHT / 2) - (BALL_DIAMETER / 2);
	
	// Check to see if player has won //
	if (g_PlayerScore == 10)
	{
		g_ComputerScore = 0;
		g_PlayerScore   = 0;

		while (!g_StateStack.empty())
		{
			g_StateStack.pop();
		}
			
		StateStruct win;
		win.StatePointer = GameWon;
		g_StateStack.push(win);
	}
}

// Increase computer's score, reset ball, and see it if computer has won. //
void HandleComputerScore()
{
	// Increase score //
	g_ComputerScore++;

	// Reset ball //
	g_Ball.x_speed = 0;
	g_Ball.y_speed = 0;
	g_Ball.screen_location.x = (WINDOW_WIDTH / 2) - (BALL_DIAMETER / 2);
	g_Ball.screen_location.y = (WINDOW_HEIGHT / 2) - (BALL_DIAMETER / 2);
	
	// See if computer has won //
	if (g_ComputerScore == 10)
	{
		g_ComputerScore = 0;
		g_PlayerScore   = 0;
		while (!g_StateStack.empty())
		{
			g_StateStack.pop();
		}
			
		StateStruct lose;
		lose.StatePointer = GameLost;
		g_StateStack.push(lose);
	}
}

// Move the computer's paddle and change its direction if necessary //
void HandleAI()
{
	static int last_speed = g_Ball.x_speed;
	static int decision = rand() % 3 + 1;

	// Keep the code tidy //
	int computer_x;
	int ball_center = g_Ball.screen_location.x + g_Ball.screen_location.w / 2;

	// See if ball has changed direction //
	if (last_speed != g_Ball.x_speed)
	{
		// 1 == left side, 2 == right side, 3 = center //
		decision = rand() % 3 + 1;

		last_speed = g_Ball.x_speed;
	}

	// Determine part of paddle to hit ball with according to decision //
	switch (decision)
	{
		case 1:
		{
			computer_x = g_Computer.screen_location.x;
			
		} break;

		case 2:
		{
			computer_x = g_Computer.screen_location.x + g_Computer.screen_location.w;
			
		} break;

		case 3:
		{
			computer_x = g_Computer.screen_location.x + g_Computer.screen_location.w / 2;
			
		} break;
	}

	// See if ball is near computer's center. Prevents //
	// computer from rapidly moving back and forth.    //
	if ( abs(computer_x - ball_center) < 10 )
	{
		return;
	}

	// Left //
	if (computer_x > ball_center)
	{
		if ( !CheckWallCollisions(g_Computer, LEFT) )
		{
			g_Computer.screen_location.x -= COMPUTER_SPEED;
		}
	}
	// Right //
	else if (computer_x < ball_center)
	{
		if ( !CheckWallCollisions(g_Computer, RIGHT) )
		{
			g_Computer.screen_location.x += COMPUTER_SPEED;
		}
	}
}

//  Aaron Cox, 2004 //
