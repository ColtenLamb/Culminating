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


using namespace std;

// ----------------------------- Variables -----------------------------

// Paused
bool paused = false;

// Background Colour
Color backgroundColor = { 0, 0, 0, 255 };

// Screen Size (based on user screen size while keeping the top bar)
float screenWidth = GetScreenWidth();
float screenHeight = GetScreenHeight();

// Player Starting Variables
Vector2 playerPosition = { 400, 400 };
float playerRotation = 0;
Color playerColor = { 0, 255, 0, 255 };

// Player Movement Variables
float playerSpeed = 0.0005;

// Player Acceleration Variables
float playerAcceleration = 0.00001;
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
int shootTime = 600;

// Particle Variables
float particleOpacity = 255;
float particleSpeed = 0.5;
float particleRadius = 2;
Color particleColor = { 255, 161, 0, 255 };
int particleTime = 420;

struct Particle
{
	Vector2 position;
	Vector2 direction;
};
vector<Particle> particle;

//----------------------------------------------------------------------

//-------------------------- Put UI Function Here ----------------------


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
		playerPosition = { 400, 400 };
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







// ---------------------------------------------------------------------

// ---------------------------------- Temporary Object Functions, variables and testing ----------------------------------

// Temporary Object Variables
Vector2 tempObjectPosition = { 200, 200 };
float tempObjectRotation = 0;
Color tempObjectColor = { 255, 0, 0, 255 };

// Temporary Object Functions
void drawTempObject(Vector2 position, float rotation, Color color)
{
	// Draw the Object
	DrawPoly(position, 40, 20, rotation, color);
}

// Temporary Object Movement Variables
float tempObjectSpeed = 0.05;

// Move towards player function
void moveTowardsPlayer(Vector2& position, float& rotation)
{
	// Get the direction to the player
	Vector2 direction = { playerPosition.x - position.x, playerPosition.y - position.y };

	// Get the angle to the player
	float angle = atan2(direction.y, direction.x);

	// Rotate the object towards the player
	rotation = angle * RAD2DEG;

	// Move the object towards the player
	position.x += cos(angle) * tempObjectSpeed;
	position.y += sin(angle) * tempObjectSpeed;


}

// Projectile collision with object
void collisionCheck()
{
	for (auto it = projectiles.begin(); it != projectiles.end();)
	{
		// Check if the projectile is colliding with the object
		if (CheckCollisionPointCircle(it->position, tempObjectPosition, 20))
		{
			it = projectiles.erase(it);

			// Reset the object position
			tempObjectPosition = { 200, 200 };
		}
		else
		{
			++it;
		}
	}
}

// Player collision with object
void playerCollisionCheck()
{
	// Check if the player is colliding with the object
	if (CheckCollisionPointCircle(playerPosition, tempObjectPosition, 20))
	{
		// Reset the object position
		playerPosition = { 1000, 1000 };
	}
}

// ------------------------------------------------------------------------------------------------------------------------------


int main()
{
    
	// Raylib window setup
	const int windowWidth = screenWidth;
	const int windowHeight = screenHeight;

	InitWindow(windowWidth, windowHeight, "Final Project");
	SetTargetFPS(60);

	// Start drawing
	BeginDrawing();
	ClearBackground(backgroundColor);


	
	// Main game loop
	while (!WindowShouldClose())
	{
		
		// Check if the game is paused using the Enter key
		if (IsKeyPressed(KEY_P))
		{
			paused = !paused;
		}
		




			// Fullscreen off
			fullscreen();

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

			// Collision Check with cursor
			cursorCollisionCheck();



			// ---------------------------------- Temp Functions ----------------------------------

				// Draw the object
			drawTempObject(tempObjectPosition, tempObjectRotation, tempObjectColor);

			// Move the object towards the player
			moveTowardsPlayer(tempObjectPosition, tempObjectRotation);

			// Collision Check with projectiles
			collisionCheck();

			// Collision Check with player
			playerCollisionCheck();

			// ---------------------------------------------------------------------------

			



			// Clear Background
			ClearBackground(backgroundColor);


		
		// End drawing
		EndDrawing();
	}




}

