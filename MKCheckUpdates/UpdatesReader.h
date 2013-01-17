#pragma once
#include "Typedef.h"
#include "UpdatesModel.h"

class UpdatesReader
{
public:
	UpdatesReader(void);
	~UpdatesReader(void);
	bool read( const String & _path,  CheckUpdatesModel & _model );

protected:
	bool _isExistFile( const String & _path );
};

