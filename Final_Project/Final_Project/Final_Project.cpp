#include <iostream>
#include <raylib.h>
#include <vector>
#include <time.h>
#include <string>
#include <math.h>


using namespace std;


// ----------------------------- Variables -----------------------------

// Background Colour
Color backgroundColor = { 5, 0, 5, 255 };

// Player Starting Variables
Vector2 playerPosition = { 400, 400 };
float playerRotation = 0;
Color playerColor = { 0, 255, 0, 255 };

// Player Movement Variables
float playerSpeed = 0.25;

// Player Acceleration Variables
float playerAcceleration = 0.001;
float speedLimit = 1;

// Projectile Variables
float projectileSpeed = 0.5;
float projectileRadius = 5;
Color projectileColor = { 255, 0, 0, 255 };

// Projectile Vector
vector<Vector2> projectiles;









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

// Function to fire a projectile towards the mouse position of when the player clicks
void fireProjectile(Vector2 position, float rotation)
{
	// Get the mouse position
	Vector2 mousePosition = GetMousePosition();

	// Get the direction to the mouse
	Vector2 direction = { mousePosition.x - position.x, mousePosition.y - position.y };

	// Get the angle to the mouse
	float angle = atan2(direction.y, direction.x);

	// Create a new projectile
	Vector2 projectilePosition = { position.x + cos(angle) * 20, position.y + sin(angle) * 20 };
	projectiles.push_back(projectilePosition);

	// Move the projectile
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].x += cos(angle) * projectileSpeed;
		projectiles[i].y += sin(angle) * projectileSpeed;
	}

	// Draw the projectile
	for (int i = 0; i < projectiles.size(); i++)
	{
		DrawCircle(projectiles[i].x, projectiles[i].y, projectileRadius, projectileColor);

	}
}





// ---------------------------------------------------------------------





int main()
{
    
	// Raylib window setup
	const int windowWidth = 800;
	const int windowHeight = 800;

	InitWindow(windowWidth, windowHeight, "Final Project");
	SetTargetFPS(60);

	// Start drawing
	BeginDrawing();
	
	ClearBackground(backgroundColor);

	// Main game loop
	while (!WindowShouldClose())
	{
		
		// Draw the player
		drawPlayer(playerPosition, playerRotation, playerColor);

		// Movement
		movePlayer(playerPosition, playerRotation);

		// Fire projectile	

		

		



		

		// Clear Background
		ClearBackground(backgroundColor);

		// End drawing
		EndDrawing();
	}




}

