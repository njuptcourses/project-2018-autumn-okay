
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Board.h"
#include "Snake.h"
#include <random>
#include "SoundEffect.h"
#include "FrameTimer.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	GameSettings settings = "settings.txt";
	Board brd;
	Snake snek;
	Location delta_loc = {1,0};
	std::mt19937 rng;
	FrameTimer ft;
	SoundEffect sfxEat = SoundEffect( { L"Sounds\\Eat.wav" } );
	SoundEffect sfxSlither = SoundEffect( { L"Sounds\\Slither0.wav",L"Sounds\\Slither1.wav",L"Sounds\\Slither2.wav" } );
	Sound sndMusic = Sound( L"Sounds\\Music_Loop.wav",Sound::LoopType::AutoFullSound );
	Sound sndTitle = Sound( L"Sounds\\Title.wav" );
	SoundEffect sndFart = SoundEffect( { L"Sounds\\Fart.wav" } );
	static constexpr float snekMovePeriodMin = 0.040f;
	static constexpr float snekMovePeriodSpeedup = 0.15f;
	int nPoison;
	int nFood;
	float snekMovePeriod = 0.4f;
	float snekMoveCounter = 0.0f;
	float snekSpeedupFactor;
	bool gameIsOver = false;
	bool gameIsStarted = false;
	/********************************/
};