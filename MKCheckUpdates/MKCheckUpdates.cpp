#include "stdafx.h"
#include <Windows.h>
#include <cstdlib>
#include <stdlib.h>
#include "UpdatesModel.h"
#include "UpdatesReader.h"
#include "WinApiView.h"
#include "Config.h"


void HelloWorld()
{
	 MessageBox(NULL, "Добрый день!", "Приветствие!",	MB_OK);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	PWSTR pCmdLine, int nCmdShow)
{
#ifdef JOKE_MODE
	HelloWorld();
#endif
	
	UpdatesReader reader;
	CheckUpdatesModel model;

	if ( reader.read( DB_PATH , model ) == false )
	{
		return -1;
	}

	WinApiView view(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	if( view.init() == false )
	{
		return -1;
	}

	if ( view.show( &model ) == false )
	{
		return -1;
	}

	return 0;
}
