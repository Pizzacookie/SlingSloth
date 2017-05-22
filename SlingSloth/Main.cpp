#include <iostream>
#include "SDL2\SDL.h"
#include "SDL_Image.h"
#include <string>
#define FPS 60

//Initialize global labels
unsigned int timeStart;
unsigned int timeNow;

bool quit = false;

float movementSpeed = 3;
float playerDeathX = 0;
float playerDeathY = 0;
float playerX = 0;
float playerY = 0;
float playerW = 70;
float playerH = 90;
float playerXW = playerX + playerW;
float playerYH = playerY + playerH;

SDL_Rect playerHitbox;

SDL_Rect instaRect(int x, int y, int w, int h)
{
	SDL_Rect temp;
	temp.x = x;
	temp.y = y;
	temp.w = w;
	temp.h = h;
	return temp;
}

//using namespace std;
int wmain(int argv, char* argc)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "Failed to initialize" << std::endl;
	}
	else
	{
		timeStart = 0;

		SDL_Window* window = SDL_CreateWindow("TestWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		//Initialize Local Variables
		bool key_left = false;
		bool key_right = false;
		bool key_down = false;
		bool key_up = false;
		int a = 0; //Debugging variable

		//Platform Position
		float platformX = 33;
		float platformY = 203;
		float platformW = 100;
		float platformH = 20;
		float platformXW = platformX + platformW;
		float platformYH = platformY + platformH;

		//Standard Movement Collision
		bool collision = false;
		float tempCollisionX = 0;
		float tempCollisionY = 0;
		float collisionX = 0;
		float collisionY = 0;

		//Pixel counters
		int i = 1;
		int pixelHor = 0;
		int pixelVer = 0;
		int pixelsUntilCollision = 0;

		//Gravity variables
		bool gravityActive = true;
		bool gravCollision = false;
		float gravity = 0;
		float gravityMin = 0.01;
		float gravityMax = 4;
		float gravityAcc = 0.1;
		bool onLand = false;

		//Gravity distance counters
		float gravPixelHor = 0;
		float gravPixelVer = 0;
		float gravPixelsUntilCollision = 0;

		//Potential variable for player death
		bool playerExploded = false;




		if (window == 0)
		{
			std::cerr << "Failed to create window" << std::endl;
		}
		else
		{
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == 0)
			{
				std::cerr << "Failed to create renderer" << std::endl;
			}

			//Player
			SDL_Surface* playerImage = IMG_Load("Thomas.jpg");

			SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerImage);

			//Explosion
			SDL_Surface* explosionImage = IMG_Load("explosion.png");

			SDL_Texture* explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionImage);

			//Platform
			SDL_Surface* platformImage = IMG_Load("morals.png");

			SDL_Texture* platformTexture = SDL_CreateTextureFromSurface(renderer, platformImage);

			while (!quit)
			{				
				SDL_Event event;

				SDL_PollEvent(&event);

				switch (event.type)
				{
					case(SDL_QUIT):
					{
						quit = true;
						break;
					}

					//Check when a key is pressed
					case(SDL_KEYDOWN):
					{
						switch (event.key.keysym.sym)
						{
							case(SDLK_w):
							{
								std::cerr << "Pressed W" << std::endl;

								playerY -= 20;

								break;
							}

							case(SDLK_s):
							{
								std::cerr << "Pressed S" << std::endl;	

								playerY += 20;

								break;
							}

							case(SDLK_a):
							{

								if (key_left == false)
								{
									key_left = true;
								}

								std::cerr << "Pressed A" << std::endl;								
								break;
							}

							case(SDLK_d):
							{
								if (key_left == false)
								{
									key_right = true;
								}
								std::cerr << "Pressed D" << std::endl;								
								break;
							}

							case(SDLK_l):
							{
								if (playerExploded == false)
								{
									std::cerr << "Pressed L" << std::endl;
									playerExploded = true;
									playerDeathX = playerX;
									playerDeathY = playerY;
								}
								break;
								
							}

							case(SDLK_g):
							{
								std::cerr << "pressed G" << std::endl;								
								if (gravityActive == true)
								{
									gravityActive = false;
								}
								else
								{
									gravityActive = true;
								}
								break;

							}

							case(SDLK_ESCAPE):
							{
								quit = true;
								break;
							}
						} 
						break;
					}

					//When key isn't pressed
					case(SDL_KEYUP):
					{
						switch (event.key.keysym.sym)
						{
							case(SDLK_w):
							{
								break;
							}

							case(SDLK_s):
							{
								break;
							}

							case(SDLK_a):
							{

								if (key_left == true)
								{
									key_left = false;
								}

								break;
							}

							case(SDLK_d):
							{
							
								if (key_right == true)
								{
									key_right = false;
								}

								break;
							}
						}
						break;
					}		

				}				

				timeNow = SDL_GetTicks();

				playerHitbox.x = playerX;
				playerHitbox.y = playerY;
				playerHitbox.w = playerW;
				playerHitbox.h = playerH;
				playerXW = playerX + playerW;
				playerYH = playerY + playerH;

				if ((timeNow - timeStart) >= (1000 / 60))
				{
					//Movement

					//Key left
					if (key_left == true)
					{						
						for (pixelHor = playerX; pixelHor <= playerXW; pixelHor++)
						{
							for (pixelVer = playerY; pixelVer <= playerYH; pixelVer++)
							{
								tempCollisionX = pixelHor - movementSpeed; //Calculates the collision if the player moves on the next tick
								tempCollisionY = pixelVer;	//Calculates the collision if the player moves on the next tick
								if (((tempCollisionX > platformX) && (tempCollisionY > platformY)) && ((tempCollisionX < platformXW) && (tempCollisionY < platformYH)))
								{
									collision = true;
									collisionX = tempCollisionX;
									collisionY = tempCollisionY;
								}
							}
						}
						if (collision == false)
						{
							playerX -= movementSpeed;
						}
						else
						{
							for (pixelsUntilCollision = movementSpeed; pixelsUntilCollision >= 1; pixelsUntilCollision--)
							{
								std::cerr << pixelsUntilCollision << std::endl;
								collision = false;
								for (pixelHor = playerX; pixelHor <= playerXW; pixelHor++)
								{
									for (pixelVer = playerY; pixelVer <= playerYH; pixelVer++)
									{
										tempCollisionX = pixelHor - pixelsUntilCollision;
										tempCollisionY = pixelVer;
										if (((tempCollisionX >= platformX) && (tempCollisionY >= platformY)) && ((tempCollisionX < platformXW) && (tempCollisionY <= platformYH)))
										{
											collision = true;
										}
									}
								}
								if (collision == false)
								{
									playerX -= pixelsUntilCollision;
									break;
								}

							}
						}
						collision = false;
					}

					//Key right
					if (key_right == true)
					{
						for (pixelHor = playerX; pixelHor <= playerXW; pixelHor++)
						{
							for (pixelVer = playerY; pixelVer <= playerYH; pixelVer++)
							{
								tempCollisionX = pixelHor + movementSpeed;
								tempCollisionY = pixelVer;
								if (((tempCollisionX > platformX) && (tempCollisionY > platformY)) && ((tempCollisionX < platformXW) && (tempCollisionY < platformYH)))
								{
									collision = true;
									collisionX = tempCollisionX;
									collisionY = tempCollisionY;
								}
							}
						}
						if (collision == false)
						{
							playerX += movementSpeed;
						}
						else
						{
							for (pixelsUntilCollision = movementSpeed; pixelsUntilCollision >= 1; pixelsUntilCollision--)
							{
								std::cerr << pixelsUntilCollision << std::endl;
								collision = false;
								for (pixelHor = playerX; pixelHor <= playerXW; pixelHor++)
								{
									for (pixelVer = playerY; pixelVer <= playerYH; pixelVer++)
									{
										tempCollisionX = pixelHor + pixelsUntilCollision;
										tempCollisionY = pixelVer;
										if (((tempCollisionX > platformX) && (tempCollisionY > platformY)) && ((tempCollisionX < platformXW) && (tempCollisionY < platformYH)))
										{
											collision = true;
										}
									}
								}
								if (collision == false)
								{
									playerX += pixelsUntilCollision;
									break;
								}

							}
						}
						collision = false;
					}
					//Movement end


					//Rendering
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderClear(renderer);
					if (playerExploded == false)
					{
						SDL_RenderCopy(renderer, playerTexture, &instaRect(0, 0, playerImage->w, playerImage->h), &instaRect(playerX, playerY, playerW, playerH));

					}

					//Turns player into explosion upon death
					if (playerExploded == true)
					{
						SDL_RenderCopy(renderer, explosionTexture, &instaRect(0, 0, explosionImage->w, explosionImage->h), &instaRect(playerDeathX, playerDeathY, 70, 90));
					}

					//Rendering Platform
					SDL_RenderCopy(renderer, platformTexture, &instaRect(0, 0, platformImage->w, platformImage->h), &instaRect(platformX, platformY, platformW, platformH));

					timeStart = timeNow;


					

					onLand = false;
					gravCollision = false;

					//Gravity
					
					if (gravityActive == true)
					{
						//Goes through all the pixels to see if gravity will make the player collide on the next tick
						for (gravPixelHor = playerX; gravPixelHor <= playerXW; gravPixelHor++)
						{
							for (gravPixelVer = playerY; gravPixelVer <= playerYH; gravPixelVer++)
							{
								tempCollisionX = gravPixelHor;
								tempCollisionY = playerYH + gravity;

								//If the player will collide, gravCollision is set to true
								if (tempCollisionX >= platformX && tempCollisionX <= platformXW && tempCollisionY >= platformY && tempCollisionY <= platformYH)
								{
									gravCollision = true;
								}
							}
						}



						//If the player wont collide, gravity works as normal
						if (!gravCollision)
						{
							//Makes the gravity accelerate over time
							if (gravity < gravityMax)
							{
								gravity += gravityAcc;
							}

							//If the gravity is equal to or greater than the maximum, the gravity set to the maximum.
							if (gravity >= gravityMax)
							{
								gravity = gravityMax;
							}

							//Makes the player fall
							playerY += gravity;
						}

						//While the player is about to collide and is not already on the ground, the player will be moved as close as possible to the collision object
						while (gravCollision && !onLand)
						{
							playerY += gravityAcc;
							tempCollisionY = playerY + gravityAcc;
							//If the player is right above the collision object, the player is moved 1 pixel above the object and countet as on the ground
							if (tempCollisionY >= platformY && tempCollisionY <= platformYH)
							{
								playerY = platformY - playerH;
								onLand = true;
							}
						}
					}
				}

					
				
				//Updates window with renders
				SDL_RenderPresent(renderer);
			}
		}
	}

	return 0;

	if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		std::cerr << "Failed to initialize SDL_playerImage.n" << std::endl;
	}

}

