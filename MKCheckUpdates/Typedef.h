#	pragma once

#	ifdef _MSC_VER
#	pragma warning(disable:4250) // remove inherits dominance
#	endif

#	include <string>
#	include <vector>
#	include <list>
#	include <iostream>
#	include <cstring>
#	include <cstdlib>
#	include <sstream>
#	include <fstream>

typedef wchar_t TCharW;
typedef char TCharA;

typedef std::wstring WString;
typedef std::string String;

typedef std::wstringstream WStringstream;
typedef std::stringstream Stringstream;

typedef std::ifstream InputFileStream;

typedef void* WindowHandle;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;

typedef std::vector<char> TBlobject;
typedef std::vector<String> TVectorString;
typedef std::vector<WString> TVectorWString;


