#pragma once

#include "Typedef.h"

struct UpdatesInfo
{
	UpdatesInfo( const String & _name, bool _exist )
		: name( _name )
		, exist( _exist )
	{
	}

	String name;
	bool exist;
};

typedef std::vector<UpdatesInfo> TVectorUpdatesInfo;

class CheckUpdatesModel
{
public:
	CheckUpdatesModel(void);
	~CheckUpdatesModel(void);

	const TVectorUpdatesInfo & getRows() const;
	void addRow( const UpdatesInfo & _row );
	size_t count() const;
protected:
	TVectorUpdatesInfo m_rows;
};

