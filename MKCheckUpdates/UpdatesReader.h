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
	bool _getUpdateStatus( const String & _id1, const String & _id2 );
	void _createUpdatePath( const String & _id, String& _dest );
	bool _isExistFile( const String & _path );
};

