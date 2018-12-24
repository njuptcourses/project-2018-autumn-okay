
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	brd( settings,gfx ),
	rng( std::random_device()() ),
	snek( {2,2} ),
	nPoison( settings.GetPoisonAmount() ),
	nFood( settings.GetFoodAmount() ),
	snekSpeedupFactor( settings.GetSpeedupRate() )
{
	for( int i = 0; i < nPoison; i++ )
	{
		brd.SpawnContents( rng,snek,Board::CellContents::Poison );
	}
	for( int i = 0; i < nFood; i++ )
	{
		brd.SpawnContents( rng,snek,Board::CellContents::Food );
	}
	sndTitle.Play( 1.0f,1.0f );
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	
	if( gameIsStarted )
	{
		if( !gameIsOver )
		{
			if( wnd.kbd.KeyIsPressed( VK_UP ) )
			{
				const Location new_delta_loc = { 0,-1 };
				if( delta_loc != -new_delta_loc || snek.GetLength() <= 2 )
				{
					delta_loc = new_delta_loc;
				}
			}
			else if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
			{
				const Location new_delta_loc = { 0,1 };
				if( delta_loc != -new_delta_loc || snek.GetLength() <= 2 )
				{
					delta_loc = new_delta_loc;
				}
			}
			else if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
			{
				const Location new_delta_loc = { -1,0 };
				if( delta_loc != -new_delta_loc || snek.GetLength() <= 2 )
				{
					delta_loc = new_delta_loc;
				}
			}
			else if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
			{
				const Location new_delta_loc = { 1,0 };
				if( delta_loc != -new_delta_loc || snek.GetLength() <= 2 )
				{
					delta_loc = new_delta_loc;
				}
			}

			float snekModifiedMovePeriod = snekMovePeriod;
			if( wnd.kbd.KeyIsPressed( VK_CONTROL ) )
			{
				snekModifiedMovePeriod = std::min( snekMovePeriod,snekMovePeriodSpeedup );
			}

			snekMoveCounter += dt;
			if( snekMoveCounter >= snekModifiedMovePeriod )
			{
				snekMoveCounter -= snekModifiedMovePeriod;
				const Location next = snek.GetNextHeadLocation( delta_loc );
				const Board::CellContents contents = brd.IsInsideBoard( next ) ? brd.GetContents( next ) 
					: Board::CellContents::Empty;
				if( !brd.IsInsideBoard( next ) ||
					snek.IsInTileExceptEnd( next ) ||
					contents == Board::CellContents::Obstacle )
				{
					gameIsOver = true;
					sndFart.Play( rng,1.2f );
					sndMusic.StopAll();
				}
				else if( contents == Board::CellContents::Food )
				{
					snek.GrowAndMoveBy( delta_loc );
					brd.ConsumeContents( next );
					brd.SpawnContents( rng,snek,Board::CellContents::Obstacle );
					brd.SpawnContents( rng,snek,Board::CellContents::Food );
					sfxEat.Play( rng,0.8f );
				}
				else if( contents == Board::CellContents::Poison )
				{
					snek.MoveBy( delta_loc );
					brd.ConsumeContents( next );
					snekMovePeriod = std::max( snekMovePeriod * snekSpeedupFactor,snekMovePeriodMin );
					sndFart.Play( rng,0.6f );
				}
				else
				{
					snek.MoveBy( delta_loc );
					sfxSlither.Play( rng,0.08f );
				}
			}
		}
	}
	else
	{
		if( wnd.kbd.KeyIsPressed( VK_RETURN ) )
		{
			sndMusic.Play( 1.0f,0.6f );
			gameIsStarted = true;
		}
	}
}

void Game::ComposeFrame()
{
	if( gameIsStarted )
	{
		snek.Draw( brd );
		brd.DrawCells();
		if( gameIsOver )
		{
			SpriteCodex::DrawGameOver( 350,265,gfx );
		}
		brd.DrawBorder();
	}
	else
	{
		SpriteCodex::DrawTitle( 290,225,gfx );
	}
}
