#include "BatWinAPI.h"
#include "Window.h"
#include "Graphics.h"
#include "Game.h"
#include <string>

using namespace Bat;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow )
{
	try
	{
		Window wnd( { 50, 50 }, Graphics::ScreenWidth, Graphics::ScreenHeight, "Slidy Engine", Graphics::FullScreen );

		try
		{
			Game game( wnd );
			while( wnd.ProcessMessage() )
			{
				game.Run();
			}
		}
		catch( const std::exception& e )
		{
			wnd.ShowMessageBox( "Error", e.what(), MB_ICONWARNING );
		}
	}
	catch( const std::exception& e )
	{
		MessageBox( NULL, e.what(), "Error in window creation", MB_ICONWARNING );
	}

	return 0;
}