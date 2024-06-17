// Controls currently are:
// Spacebar to accelerate
// Mouse to control direction
// Left mouse button to shoot projectiles
// F11 to toggle fullscreen

#include <iostream>
#include <raylib.h>
#include <vector>
#include <ctime>
#include <string>
#include <cmath>
#include <time.h>


using namespace std;

// ----------------------------- Variables -----------------------------

bool easyMode = false;

// Points for the player shooting an alien
int points = 0;

// Background Colour
Color backgroundColor = { 0, 0, 0, 255 };

// Time Color
Color timeColour = { 255, 255, 255, 100 };

// Screen Size (based on user screen size while keeping the top bar)
float screenWidth = GetScreenWidth();
float screenHeight = GetScreenHeight();

// Player Starting Variables
Vector2 playerPosition = { 400, 400 };
float playerRotation = 0;
Color playerColor = { 0, 255, 0, 255 };
int lives = 1;

// Player Movement Variables
float playerSpeed = 0.0005;

// Player Acceleration Variables
float playerAcceleration = 0.0001;
float speedLimit = 0.75;

// Projectile Variables
float projectileSpeed = 1.5;
float projectileRadius = 5;
Color projectileColor = { 102, 191, 255, 255 };

// Projectile Struct & Vector
struct Projectile
{
	Vector2 position;
	Vector2 direction;
};
vector<Projectile> projectiles;

// Frame Counter
int frameCounter = 0;
int frameCounter2 = 0;

// Shoot Time (this is the time between each shot (In frames))
int shootTime = 300;

// Particle Variables
float particleOpacity = 255;
float particleSpeed = 0.5;
float particleRadius = 3;
Color particleColor = { 150, 50, 180, 255 };
int particleTime = 60;

// Alien Speed
float alienSpeed = 0.1;

struct Particle
{
	Vector2 position;
	Vector2 direction;
	float opacity;
	float lifeTime;
};
vector<Particle> particles;

//----------------------------------------------------------------------



// ----------------------------- Functions -----------------------------

// Function to draw the player as a poly
void drawPlayer(Vector2 position, float rotation, Color color)
{
	// Draw the player
	DrawPoly(position, 3, 20, rotation, color);
}

// Function to move the player towards the mouse position using spacebar
void movePlayer(Vector2& position, float& rotation)
{
	// Get the mouse position
	Vector2 mousePosition = GetMousePosition();

	// Get the direction to the mouse
	Vector2 direction = { mousePosition.x - position.x, mousePosition.y - position.y };

	// Get the angle to the mouse
	float angle = atan2(direction.y, direction.x);

	// Rotate the player towards the mouse
	rotation = angle * RAD2DEG;

	// Move the player towards the mouse
	if (IsKeyDown(KEY_SPACE))
	{
		frameCounter2++;
		// Accelerate the player
		playerSpeed += playerAcceleration;

		// Limit the speed
		if (playerSpeed > speedLimit)
		{
			playerSpeed = speedLimit;
		}

		// Move the player
		position.x += cos(angle) * playerSpeed;
		position.y += sin(angle) * playerSpeed;

		// Create particles at a slower rate
		if (frameCounter2 % particleTime == 0)
		{
			particles.push_back({ position, { -cos(angle) * particleSpeed, -sin(angle) * particleSpeed }, particleOpacity, 255 });
		}
		

	}
	else
	{
		// Decelerate the player
		playerSpeed -= playerAcceleration;

		// Limit the speed
		if (playerSpeed < 0)
		{
			playerSpeed = 0;
		}

		// Move the player
		position.x += cos(angle) * playerSpeed;
		position.y += sin(angle) * playerSpeed;

	}
}

// Function to move player when off screen
void playerWrap(Vector2& position)
{
	if (position.x > GetScreenWidth())
	{
		position.x = 0;
	}
	if (position.x < 0)
	{
		position.x = GetScreenWidth();
	}
	if (position.y > GetScreenHeight())
	{
		position.y = 0;
	}
	if (position.y < 0)
	{
		position.y = GetScreenHeight();
	}
}

// Function to shoot a projectile
void shootProjectile(Vector2 position, float rotation) {
	Vector2 direction = { cos(rotation * DEG2RAD), sin(rotation * DEG2RAD) };
	projectiles.push_back({ position, direction });
}

// Function to update and draw projectiles
void updateProjectiles() {
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		// Move the projectile
		it->position.x += it->direction.x * projectileSpeed;
		it->position.y += it->direction.y * projectileSpeed;

		// Check if the projectile is off screen
		if (it->position.x < 0 || it->position.x > GetScreenWidth() || it->position.y < 0 || it->position.y > GetScreenHeight()) {
			it = projectiles.erase(it);
		}
		else {
			// Draw the projectile
			DrawCircleV(it->position, projectileRadius, projectileColor);
			++it;
		}
	}
}

// Function to make the cursor invisible and replace it with a crosshair
void crosshair()
{
	// Hide the cursor
	HideCursor();

	// Draw the crosshair
	DrawLine(GetMouseX() - 10, GetMouseY(), GetMouseX() + 10, GetMouseY(), SKYBLUE);
	DrawLine(GetMouseX(), GetMouseY() - 10, GetMouseX(), GetMouseY() + 10, SKYBLUE);
}

// Function to draw collide with cursor
void cursorCollisionCheck()
{
	// Check if the cursor is colliding with the player
	if (CheckCollisionPointCircle(GetMousePosition(), playerPosition, 20))
	{
		// Reset the player position
		//playerPosition = { 400, 400 };
		lives--;
	}
}

// Function to make the game fullscreen
void fullscreen()
{
	// Toggle fullscreen
	if (IsKeyPressed(KEY_F11))
	{
		ToggleFullscreen();
	}
}

// Function for when lives reach 0
void gameOver()
{
	if (lives <= 0)
	{
		// Draw red overlay
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 125, 0, 0, 255 });

		// Draw Game Over Text in the center of the screen
		DrawText("Game Over", GetScreenWidth() / 2 - MeasureText("Game Over", 40) / 2, GetScreenHeight() / 2 - 40, 40, WHITE);

		// Button to close the game
		DrawText("Press ESC to close the game", GetScreenWidth() / 2 - MeasureText("Press ESC to close the game", 20) / 2, GetScreenHeight() / 2 + 20, 20, WHITE);

	}
}

// Function to update and draw particles
void updateParticles()
{
	for (auto it = particles.begin(); it != particles.end();)
	{
		// Update particle position
		it->position.x += it->direction.x;
		it->position.y += it->direction.y;

		// Fade out the particle
		it->opacity -= 255.0f / it->lifeTime;

		// Draw the particle
		DrawCircleV(it->position, particleRadius, Fade(particleColor, it->opacity / 255.0f));

		// Check if the particle is no longer visible
		if (it->opacity <= 0)
		{
			it = particles.erase(it);
		}
		else
		{
			++it;
		}
	}
}


// Alien Chaser and Basic Alien Functions
void drawAlien(Vector2 position, float rotation, Color color, int type)
{

	// Draw the alien
	if (type == 1)
	{
		DrawPoly(position, 4, 20, rotation, color);
	}
	else if (type == 2)
	{
		DrawPoly(position, 4, 20, rotation, color);

	}
}

// Move alien by type function
void moveAlien(Vector2& position, float& rotation, int type) {
	if (type == 1) {
		//get the direction to the player
		Vector2 direction = { playerPosition.x - position.x, playerPosition.y - position.y };

		//get the angle to the player
		float angle = atan2(direction.y, direction.x);

		//rotate the alien towards the player
		rotation = angle * RAD2DEG;

		//move the alien towards the player
		position.x += cos(angle) * alienSpeed;
		position.y += sin(angle) * alienSpeed;
	}
}


//------------------- Alien Chaser and Basic Alien Variables ----------------

//Create a struct for the alien chaser and basic alien
struct Alien
{

	Vector2 position;
	Vector2 direction;
	float angle;
	int type;


};

// Function to check collision between player and alien given position and radius
bool checkCollision(Vector2 position, float radius)
{
	if (CheckCollisionPointCircle(playerPosition, position, radius))
	{
		lives--;
	}
	return false;
}


// Function to enable easy mode if F1 is pressed
void enableEasyMode()
{
	if (IsKeyPressed(KEY_F1))
	{
		easyMode = true;
	}
}

// Function to tell player how to enable easy mode and if it is enabled
void easyModeText()
{
	if (easyMode == false)
	{
		// in top right corner
		DrawText("Press F1 to Disable Easy Mode", GetScreenWidth() - MeasureText("Press F1 to Disable Easy Mode", 20) - 10, 10, 20, timeColour);
	}
	else
	{
		// in the top right corner
		DrawText("Easy Mode Disabled", GetScreenWidth() - MeasureText("Easy Mode Disabled", 20) - 10, 10, 20, timeColour);
		
	}
}

// Function to draw white specs in the background randomly
void drawBackground()
{
	for (int i = 0; i < 100; i++)
	{
		DrawCircle(GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()), 1, timeColour);
	}
}


// -----------------------------------------------------------------------


int main()
{

	// Alien Chaser and Basic Alien Variables
	vector<Alien> chaser;
	vector<Alien> basic;

	// Start Clock
	int time = clock();

	// Raylib window setup
	const int windowWidth = screenWidth;
	const int windowHeight = screenHeight;

	InitWindow(windowWidth, windowHeight, "Final Project");
	SetTargetFPS(60);

	// Start drawing
	BeginDrawing();
	ClearBackground(backgroundColor);

	// Loading the image
	Image playerImage = LoadImage("player.png");
	ImageResize(&playerImage, 50, 50);
	Texture2D playerTexture = LoadTextureFromImage(playerImage);
	
	

	// Main game loop
	while (!WindowShouldClose())
	{
		// Timer for seconds
		int seconds = (clock() - time) / CLOCKS_PER_SEC;

			// Fullscreen 
			fullscreen();

			// Draw Specs
			drawBackground();

			// Draw the player
			drawPlayer(playerPosition, playerRotation, playerColor);

			// Movement
			movePlayer(playerPosition, playerRotation);

			// Wrap player
			playerWrap(playerPosition);

			// Shoot Projectile every 1 second
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				frameCounter++;
				if (frameCounter % shootTime == 0)
				{
					shootProjectile(playerPosition, playerRotation);
				}
			}

			// Update and draw projectiles
			updateProjectiles();

			// Draw the crosshair
			crosshair();

			// Easy Mode
			enableEasyMode();

			if (easyMode == true)
			{
				cursorCollisionCheck();
			}
		
			// Easy Mode Text
			easyModeText();

			// Update and draw particles
			updateParticles();
		
			// Draw the texture locked to the player and player rotation
			DrawTexturePro(playerTexture, { 0, 0, (float)playerTexture.width, (float)playerTexture.height }, { playerPosition.x, playerPosition.y, (float)playerTexture.width, (float)playerTexture.height }, { (float)playerTexture.width / 2, (float)playerTexture.height / 2 }, playerRotation, WHITE);

			
			// Alien Chaser
			int screenWidth = GetScreenWidth();
			int screenHeight = GetScreenHeight();
			frameCounter2++;
			if (frameCounter2 % 1880 == 0)
			{
				// Create a new alien that is not too close to the player
				Alien newAlien;
				newAlien.position = { (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) };
				// Check if the alien position is too close to the player
				while (CheckCollisionPointCircle(newAlien.position, playerPosition, 200))
				{
					newAlien.position = { (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) };
				}
				newAlien.angle = 0;
				newAlien.type = 1;
				chaser.push_back(newAlien);




			}

			for (auto& alien : chaser)
			{
				drawAlien(alien.position, alien.angle, playerColor, alien.type);
				moveAlien(alien.position, alien.angle, alien.type);
			}


			// Collision Check 
			for (auto& alien : chaser)
			{
				if (checkCollision(alien.position, 20))
				{
					chaser.erase(chaser.begin());
				}

				// Collision with projectiles
				if (projectiles.size() > 0)
				{
					for (auto it = projectiles.begin(); it != projectiles.end();)
					{
						if (CheckCollisionPointCircle(it->position, alien.position, 20))
						{
							it = projectiles.erase(it);
							chaser.erase(chaser.begin());
							points++;
						}
						else
						{
							++it;
						}
					}
				}
			}

			// Draw the time alive in the top left corner
			DrawTextEx(GetFontDefault(), TextFormat("TIME ALIVE: %i", seconds), { 10, 30 }, 20, 0, timeColour);

			// Draw the points in the top left corner
			DrawTextEx(GetFontDefault(), TextFormat("POINTS: %i", points), { 10, 10 }, 20, 0, timeColour);
			
			// Game Over
			gameOver();

			// Clear Background
			ClearBackground(backgroundColor);

		

		// End drawing
		EndDrawing();
	}


}

