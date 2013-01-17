#include "UpdatesReader.h"
#include "Windows.h"

/////////////////////////////////////////////////////////////
UpdatesReader::UpdatesReader(void)
{
}
/////////////////////////////////////////////////////////////
UpdatesReader::~UpdatesReader(void)
{
}
/////////////////////////////////////////////////////////////
bool UpdatesReader::read( const String & _path, CheckUpdatesModel & _model )
{
	for( int i = 0; i<=9; i++ )
	{
		bool status;
		status = (i%2 == 0)?true:false;
		UpdatesInfo info("110101011",status );
		_model.addRow(info);
	}

	return true;
}
/////////////////////////////////////////////////////////////
bool UpdatesReader::_isExistFile( const String & _path )
{
	DWORD dwAttrib = GetFileAttributes(_path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
/////////////////////////////////////////////////////////////