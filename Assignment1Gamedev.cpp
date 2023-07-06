// Assessment1.cpp: A program using the TL-Engine
// Author: Ivan Semenov
// EMail: ISemenov@uclan.ac.uk

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include <sstream>
using namespace std;
using namespace tle;


//Initialization of constant variables
const float kSphereSpeed = 50.0;
const float kRotationSpeed = 100.0;
const float KCubeHalfEdge = 2.5;
const int kCubeAmmount = 12;

// Creation of the cube array
IModel* cubeModels[kCubeAmmount];



// All functions are outside of the Main

// This function generates all the cubes at the begining, for every cube generaated it will compare locations
// with all previous cubes spawned and if their coordinates match, then it wil generate new random coordinates 
// until am empty location is found on the field
void Cube_Generate(IMesh* cubeMesh) {
	cubeModels[0] = cubeMesh->CreateModel(rand() % 160 + (-80), KCubeHalfEdge, rand() % 180 + (-90));

	for (int i = 1; i < kCubeAmmount;) {
		float randx = rand() % 160 + (-80);
		float randz = rand() % 180 + (-90);
		cubeModels[i] = cubeMesh->CreateModel(randx, KCubeHalfEdge, randz);
		for (int j = 1; j <= i;) {
			if (cubeModels[i]->GetX() > cubeModels[j - 1]->GetX() + 12.5 || cubeModels[i]->GetX() < cubeModels[j - 1]->GetX() - 12.5 ||
				cubeModels[i]->GetZ() > cubeModels[j - 1]->GetZ() + 12.5 || cubeModels[i]->GetZ() < cubeModels[j - 1]->GetZ() - 12.5) {
				j++;
			}
			else {
				cubeModels[i]->SetPosition(0, KCubeHalfEdge, 0);
				cubeModels[i]->MoveX(rand() % 160 + (-80));
				cubeModels[i]->MoveZ(rand() % 180 + (-90));

			}
		}

		i++;
	}
}



// function wrides an Engame message on screen
void Endgame_Message(IFont* myFont, I3DEngine* myEngine) {
	myFont->Draw("Game-Over", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 - 100, kRed, kCentre);
}

// function wrides a Game won message on screen
void GameWon_Message(IFont* myFont, I3DEngine* myEngine) {
	myFont->Draw("You-Won!!!", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 - 100, kWhite, kCentre);
}

// function wrides an Pause message on screen
void GamePaused_Message(IFont* myFont, I3DEngine* myEngine) {
	myFont->Draw("Paused", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 - 100, kWhite, kCentre);
}

// function has a five second timer
void Timer(float* time2, float* frameTime, I3DEngine* myEngine) {
	*time2 = (*time2 + *frameTime);
	if (*time2 >= 5) {
		myEngine->Stop();
	}
}


// function scales the player sphere by 20%
void Scale(IModel* playersphere, int& counter, float& SphereRadius) {
	playersphere->Scale(1.2);
	SphereRadius = SphereRadius * 1.2;
	counter = 0;
	playersphere->MoveY(2);
}




void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();
	myEngine->Timer();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:./Media");

	/**** Set up your scene here ****/

	// initialize the sphere radius
	float SphereRadius = 10;

	// creating all models amd meshes

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetLocalPosition(0, 200, 0);
	myCamera->RotateX(90);

	IMesh* waterMesh;
	IModel* water;
	waterMesh = myEngine->LoadMesh("water.x");
	water = waterMesh->CreateModel(0, -5, 0);

	IMesh* islandMesh;
	IModel* island;
	islandMesh = myEngine->LoadMesh("island.x");
	island = islandMesh->CreateModel(0, -5, 0);

	IMesh* skyMesh;
	IModel* sky;
	skyMesh = myEngine->LoadMesh("sky.x");
	sky = skyMesh->CreateModel(0, -960, 0);

	IMesh* sphereMesh;
	IModel* playersphere;
	sphereMesh = myEngine->LoadMesh("spheremesh.x");
	playersphere = sphereMesh->CreateModel(0, SphereRadius, 0);

	IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");



	srand(time(nullptr));

	// calls the cube generator fnction
	Cube_Generate(cubeMesh);



	// iinitialize all the cube coordinate arrays
	float CubeMinX[kCubeAmmount];
	float CubeMaxX[kCubeAmmount];
	float CubeMinY[kCubeAmmount];
	float CubeMaxY[kCubeAmmount];
	float CubeMinZ[kCubeAmmount];
	float CubeMaxZ[kCubeAmmount];


	// initializtion of counters, timers and points
	float time2 = 0;
	int playerPoints = 0;
	int counter = 0;

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);

	// creating all enumirators
	enum EGameState { Running, Paused, GameOver, GameWon };
	EGameState GameState = Running;

	enum ECameraState { View1, View2 };
	ECameraState CameraState = View1;






	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// Starts the Frametime timer so as to know time per frame
		float frameTime = myEngine->Timer();




		// for loop that every frame saves each cubes side coordinates in arrays and then chacks the sphere to box colision
		for (int i = 0; i < kCubeAmmount; i++) {
			CubeMinX[i] = cubeModels[i]->GetLocalX() - KCubeHalfEdge;
			CubeMaxX[i] = cubeModels[i]->GetLocalX() + KCubeHalfEdge;
			CubeMinY[i] = cubeModels[i]->GetLocalY() - KCubeHalfEdge;
			CubeMaxY[i] = cubeModels[i]->GetLocalY() + KCubeHalfEdge;
			CubeMinZ[i] = cubeModels[i]->GetLocalZ() - KCubeHalfEdge;
			CubeMaxZ[i] = cubeModels[i]->GetLocalZ() + KCubeHalfEdge;



			if (playersphere->GetX() > CubeMinX[i] - SphereRadius && playersphere->GetX() < CubeMaxX[i] + SphereRadius &&
				playersphere->GetY() > CubeMinY[i] - SphereRadius && playersphere->GetY() < CubeMaxY[i] + SphereRadius &&
				playersphere->GetZ() > CubeMinZ[i] - SphereRadius && playersphere->GetZ() < CubeMaxZ[i] + SphereRadius) {
				cubeModels[i]->SetPosition(0, KCubeHalfEdge, 0);
				cubeModels[i]->MoveX(rand() % 160 + (-80));
				cubeModels[i]->MoveZ(rand() % 180 + (-90));
				playerPoints = playerPoints + 10;
				counter++;

				// after colision, respawns the cubes in a different location

				for (int j = 1; j <= i;) {
					if (cubeModels[i]->GetX() > cubeModels[j - 1]->GetX() + 12.5 || cubeModels[i]->GetX() < cubeModels[j - 1]->GetX() - 12.5 ||
						cubeModels[i]->GetZ() > cubeModels[j - 1]->GetZ() + 12.5 || cubeModels[i]->GetZ() < cubeModels[j - 1]->GetZ() - 12.5) {
						j++;
					}
					else {
						cubeModels[i]->SetPosition(0, KCubeHalfEdge, 0);
						cubeModels[i]->MoveX(rand() % 160 + (-80));
						cubeModels[i]->MoveZ(rand() % 180 + (-90));

					}
				}
			}

		}


		// counter so as to increase the scale of sphere every 40 points
		if (counter == 4) {
			Scale(playersphere, counter, SphereRadius);
		}

		// if player reaches 120 points then it turns to the gamewon gamestate
		if (playerPoints == 120) {
			GameState = GameWon;
		}


		// initialize the outtext 
		stringstream outText;
		outText << "Points: " << playerPoints;
		myFont->Draw(outText.str(), myEngine->GetWidth() - 160, myEngine->GetHeight() - 700);

		outText.str(""); // Clear myStream


		// if player leaves the island borders then gamestate changes to game over
		if (playersphere->GetX() >= 100 || playersphere->GetX() <= -100 || playersphere->GetZ() >= 100 || playersphere->GetZ() <= -100) {
			GameState = GameOver;
		}


		// if statements that connect the key presses to imput into the program
		// move the player sphere on screen
		if (myEngine->KeyHeld(Key_W) && GameState == Running) {
			playersphere->MoveLocalZ(kSphereSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_S) && GameState == Running) {
			playersphere->MoveLocalZ(-kSphereSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_A) && GameState == Running) {
			playersphere->RotateY(-kRotationSpeed * frameTime);
		}
		else if (myEngine->KeyHeld(Key_D) && GameState == Running) {
			playersphere->RotateY(kRotationSpeed * frameTime);
		}

		// move the camera in game
		if (myEngine->KeyHeld(Key_Up) && GameState == Running) {
			myCamera->MoveZ(kSphereSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Down) && GameState == Running) {
			myCamera->MoveZ(-kSphereSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Left) && GameState == Running) {
			myCamera->MoveX(-kSphereSpeed * frameTime);
		}
		else if (myEngine->KeyHeld(Key_Right) && GameState == Running) {
			myCamera->MoveX(kSphereSpeed * frameTime);
		}

		// pause game and unpause game
		if (myEngine->KeyHit(Key_P)) {
			if (GameState == Running) {
				GameState = Paused;
			}
			else {
				GameState = Running;
			}
		}


		// change location of camera to position 2
		if (myEngine->KeyHit(Key_2) && CameraState == View1 && GameState == Running) {
			myCamera->SetLocalPosition(150, 150, -150);
			myCamera->RotateX(-45);
			myCamera->RotateY(-45);
			CameraState = View2;
		}

		// change the position back to original
		if (myEngine->KeyHit(Key_1) && CameraState == View2 && GameState == Running) {
			myCamera->SetLocalPosition(0, 200, 0);
			myCamera->RotateY(45);
			myCamera->RotateX(45);
			CameraState = View1;
		}

		// if gamestate is game over then output a gameover message on screen and close programm after 5 seconds
		if (GameState == GameOver) {
			Endgame_Message(myFont, myEngine);
			Timer(&time2, &frameTime, myEngine);
		}

		// if gamestate is game won then output a gamewon message on screen
		if (GameState == GameWon) {
			GameWon_Message(myFont, myEngine);
		}

		// if gamestate is paused then display a paused message on screen
		if (GameState == Paused) {
			GamePaused_Message(myFont, myEngine);
		}

		// if press escape then close program
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();

		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
