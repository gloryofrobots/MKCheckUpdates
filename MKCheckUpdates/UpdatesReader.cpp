#include "UpdatesReader.h"
#include "Config.h"
#include "Typedef.h"

#include "Windows.h"
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>


// trim from start
static String &ltrim( String &s )
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static String &rtrim( String &s )
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static String &trim( String &s )
{
	return ltrim(rtrim(s));
}

void split( const String& s, const String& delim, TVectorString & result, const bool keep_empty ) 
{
	if (delim.empty())
	{
		result.push_back(s);
		return;
	}
	String::const_iterator substart = s.begin(), subend;
	while (true)
	{
		subend = std::search(substart, s.end(), delim.begin(), delim.end());
		String temp(substart, subend);
		if (keep_empty || !temp.empty())
		{
			result.push_back(temp);
		}
		if (subend == s.end())
		{
			break;
		}
		substart = subend + delim.size();
	}
}

/////////////////////////////////////////////////////////////
UpdatesReader::UpdatesReader(void)
{
}
/////////////////////////////////////////////////////////////
UpdatesReader::~UpdatesReader(void)
{
}
/////////////////////////////////////////////////////////////
void UpdatesReader::_createUpdatePath( const String & _id, String& _dest )
{
	char buffer[256];
	sprintf_s( buffer, "%s\\%s.%s", EVOLUTION_UPDATE_DIR, _id.c_str(), EVOLUTION_UPDATE_EXTENSION );
	_dest = String(buffer);
}
/////////////////////////////////////////////////////////////
bool UpdatesReader::_getUpdateStatus( const String & _id1, const String & _id2 )
{
	String _path1;
	this->_createUpdatePath( _id1, _path1 );
	if( this->_isExistFile( _path1 ) == false )
	{
		return false;
	}

	String _path2;
	this->_createUpdatePath( _id2, _path2 );
	if( this->_isExistFile( _path2 ) == false )
	{
		return false;
	}

	return true;
}
/////////////////////////////////////////////////////////////
bool UpdatesReader::read( const String & _path, CheckUpdatesModel & _model )
{
	InputFileStream myfile;
	myfile.open( _path, std::ios::in );
	if ( myfile.is_open() == false )
	{
		return false;
	}
	
	String line;
	TVectorString tokens;
	while ( myfile.good() )
	{
		getline (myfile,line);
		split(line, ":", tokens, false);
	}

	myfile.close();
	
	int count = tokens.size();

	if( count == 0  || ( count %3 ) != 0 )
	{
		return false;
	}

	for( size_t i = 0; i < count; i += 3 )
	{

		char buff[255] = {'\0'};
		sprintf(buff, "%i", i);

		String name =tokens[i];
		String id1 = tokens[i + 1];
		String id2 = tokens[i + 2];

		bool status = this->_getUpdateStatus( id1, id2 );

		UpdatesInfo info( name, status );
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