#include "stdafx.h"
#include <Windows.h>
#include <cstdlib>
#include <stdlib.h>
#include "UpdatesModel.h"
#include "UpdatesReader.h"
#include "WinApiView.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	PWSTR pCmdLine, int nCmdShow)
{
	UpdatesReader reader;
	CheckUpdatesModel model;

	char * updatesDB = "data.bin"; 
	if ( reader.read( updatesDB, model ) == false )
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
