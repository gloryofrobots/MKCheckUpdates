#include "UpdatesModel.h"


CheckUpdatesModel::CheckUpdatesModel(void)
{
}
///////////////////////////////////////////
CheckUpdatesModel::~CheckUpdatesModel(void)
{
}
///////////////////////////////////////////
const TVectorUpdatesInfo & CheckUpdatesModel::getRows() const
{
	return m_rows;
}
///////////////////////////////////////////
void CheckUpdatesModel::addRow( const UpdatesInfo & _row )
{
	m_rows.push_back( _row );
}
///////////////////////////////////////////
size_t CheckUpdatesModel::count() const
{
	return m_rows.size();
}
///////////////////////////////////////////