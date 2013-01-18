#include "GuiComponents.h"

//**************************************************************************//

ElementWithPosition::ElementWithPosition( )
{
	setPosition( 0 , 0, 0, 0 );
}
//////////////////////////////////////////////////////////////////////////////
ElementWithPosition::~ElementWithPosition()
{
}
//////////////////////////////////////////////////////////////////////////////
void ElementWithPosition::setPosition( std::size_t _x ,std::size_t _y ,std::size_t _width , std::size_t _height  )
{
	m_x = _x; m_y =_y; m_width = _width; m_height = _height;
}
//////////////////////////////////////////////////////////////////////////////
void ElementWithPosition::getPosition( std::size_t & _x ,std::size_t & _y ,std::size_t  & _width , std::size_t & _height  )
{
	_x = m_x; _y = m_y, _width = m_width; _height = m_height;
}
//////////////////////////////////////////////////////////////////////////////	


ElementWithText::ElementWithText()
{
	setText( "" );
}
//////////////////////////////////////////////////////////////////////////////
ElementWithText::~ElementWithText()
{
}
//////////////////////////////////////////////////////////////////////////////
void ElementWithText::setText( LPCSTR _txt )
{
	memset(m_txt,0,ELEMENT_TEXT_LENGTH);
	strcpy_s( m_txt, _txt );
}
//////////////////////////////////////////////////////////////////////////////
void ElementWithText::setDouble( double _integer )
{
	char buffer [ELEMENT_TEXT_LENGTH] = {'\0' };
	sprintf_s(buffer,"%4.2f",_integer);
	setText(buffer);
}
//////////////////////////////////////////////////////////////////////////////
void ElementWithText::setString( std::string _txt )
{
	setText( _txt.c_str() );
}
//////////////////////////////////////////////////////////////////////////////
void ElementWithText::getText( char * _buffer, std::size_t _size )
{
	if( _size > ELEMENT_TEXT_LENGTH )
	{
		return;
	}
	memcpy( _buffer , m_txt, _size );
}
//////////////////////////////////////////////////////////////////////////////
double ElementWithText::getDouble( )
{
	double res = atof(m_txt);
	return res;
}

//**************************************************************************//

ControlField::ControlField()
	: m_window(NULL)
	, m_lpWindowName(NULL)
	, m_lpClassName(NULL)
	, m_dwStyle(NULL)
	, m_hWndParent(NULL)
	, m_lpParam(NULL)
{
}
//////////////////////////////////////////////////////////////////////////////
ControlField::~ControlField()
{
}
//////////////////////////////////////////////////////////////////////////////
void ControlField::setWindowAttr( LPCTSTR lpWindowName, DWORD dwStyle, HWND hWndParent, HMENU hMenu, LPVOID lpParam )
{
	m_lpWindowName = lpWindowName; m_dwStyle = dwStyle; m_hWndParent = hWndParent; m_lpParam = lpParam; m_hMenu = hMenu;
}
//////////////////////////////////////////////////////////////////////////////
void ControlField::createWindow( )
{
	m_window = CreateWindow( m_lpClassName, m_lpWindowName, m_dwStyle, m_x, m_y, m_width, m_height, m_hWndParent,  m_hMenu, NULL, m_lpParam );
}

void ControlField::changeColor( DWORD _color )
{
	HBRUSH hbBarva = CreateSolidBrush( _color );
	SetClassLongPtr( m_window, GCLP_HBRBACKGROUND, (LONG_PTR)hbBarva );
	InvalidateRect( m_window, NULL, TRUE );
}

//**************************************************************************//	

ControlFieldStatic::ControlFieldStatic()
	: ControlField()
{
	ControlField::m_lpClassName = WC_STATIC;
}
//////////////////////////////////////////////////////////////////////////////
ControlFieldStatic::~ControlFieldStatic()
{
}
//////////////////////////////////////////////////////////////////////////////
void ControlFieldStatic::setText( LPCSTR _txt )  
{
	ElementWithText::setText( _txt ); 
	SetWindowText( m_window, _txt );
}

//**************************************************************************//

ControlFieldEdit::ControlFieldEdit()
	: ControlField()
{
	ControlField::m_lpClassName = WC_EDIT;
}
//////////////////////////////////////////////////////////////////////////////
ControlFieldEdit::~ControlFieldEdit()
{
}
//////////////////////////////////////////////////////////////////////////////
void ControlFieldEdit::setText( LPCSTR _txt )  
{
	ElementWithText::setText( _txt ); 
	SetWindowText( m_window, _txt );
}
//////////////////////////////////////////////////////////////////////////////
void ControlFieldEdit::catchText()
{
	GetWindowTextA( m_window, m_txt , ELEMENT_TEXT_LENGTH );
}

//**************************************************************************//	

ControlFieldButton::ControlFieldButton()
	: ControlField()
{
	ControlField::m_lpClassName = WC_BUTTON;
}
//////////////////////////////////////////////////////////////////////////////
ControlFieldButton::~ControlFieldButton()
{
}
//**************************************************************************//
ControlFieldCheckBox::ControlFieldCheckBox()
	: m_isChecked(false)
{
}
////////////////////////////////////////////////////////////
ControlFieldCheckBox::~ControlFieldCheckBox()
{
}
////////////////////////////////////////////////////////////
void ControlFieldCheckBox::catchChecked()
{
	//Button_GetCheck
	int id = (int) m_hMenu;
	UINT state = IsDlgButtonChecked(m_hWndParent, id);
	if( state == BST_CHECKED )
	{
		m_isChecked = true;
	}
	else
	{
		m_isChecked = false;
	}
}
////////////////////////////////////////////////////////////
bool ControlFieldCheckBox::isChecked()
{
	return m_isChecked;
}
////////////////////////////////////////////////////////////
void ControlFieldCheckBox::check( bool _check )
{
	int id = (int) m_hMenu;
	UINT state;

	if( _check == true )
	{
		state = BST_CHECKED;
	}
	else
	{
		state = BST_UNCHECKED;
	}

	CheckDlgButton(m_hWndParent, id, state );
}

//**************************************************************************//

void SD_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if(state != SIZE_RESTORED && state != SIZE_MAXIMIZED)
		return;

	SCROLLINFO si = {};
	si.cbSize = sizeof(SCROLLINFO);

	const int bar[] = { SB_HORZ, SB_VERT };
	const int page[] = { cx, cy };

	for(size_t i = 0; i < ARRAYSIZE(bar); ++i)
	{
		si.fMask = SIF_PAGE;
		si.nPage = page[i];
		SetScrollInfo(hwnd, bar[i], &si, TRUE);

		si.fMask = SIF_RANGE | SIF_POS;
		GetScrollInfo(hwnd, bar[i], &si);

		const int maxScrollPos = si.nMax - (page[i] - 1);

		// Scroll client only if scroll bar is visible and window's
		// content is fully scrolled toward right and/or bottom side.
		// Also, update window's content on maximize.
		const bool needToScroll =
			(si.nPos != si.nMin && si.nPos == maxScrollPos) ||
			(state == SIZE_MAXIMIZED);

		if(needToScroll)
		{
			SD_ScrollClient(hwnd, bar[i], si.nPos);
		}
	}
}

void SD_OnHScroll(HWND hwnd, HWND /*hwndCtl*/, UINT code, int /*pos*/)
{
	SD_OnHVScroll(hwnd, SB_HORZ, code);
}

void SD_OnVScroll(HWND hwnd, HWND /*hwndCtl*/, UINT code, int /*pos*/)
{
	SD_OnHVScroll(hwnd, SB_VERT, code);
}

void SD_OnHVScroll(HWND hwnd, int bar, UINT code)
{
	const int scrollPos = SD_GetScrollPos(hwnd, bar, code);

	if(scrollPos == -1)
		return;

	SetScrollPos(hwnd, bar, scrollPos, TRUE);
	SD_ScrollClient(hwnd, bar, scrollPos);
}

void SD_ScrollClient(HWND hwnd, int bar, int pos)
{
	static int s_prevx = 1;
	static int s_prevy = 1;

	int cx = 0;
	int cy = 0;

	int& delta = (bar == SB_HORZ ? cx : cy);
	int& prev = (bar == SB_HORZ ? s_prevx : s_prevy);

	delta = prev - pos;
	prev = pos;

	if(cx || cy)
	{
		ScrollWindow(hwnd, cx, cy, NULL, NULL);
	}
}

int SD_GetScrollPos(HWND hwnd, int bar, UINT code)
{
	SCROLLINFO si = {};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;
	GetScrollInfo(hwnd, bar, &si);

	const int minPos = si.nMin;
	const int maxPos = si.nMax - (si.nPage - 1);

	int result = -1;

	switch(code)
	{
	case SB_LINEUP /*SB_LINELEFT*/:
		result = max(si.nPos - 1, minPos);
		break;

	case SB_LINEDOWN /*SB_LINERIGHT*/:
		result = min(si.nPos + 1, maxPos);
		break;

	case SB_PAGEUP /*SB_PAGELEFT*/:
		result = max(si.nPos - (int)si.nPage, minPos);
		break;

	case SB_PAGEDOWN /*SB_PAGERIGHT*/:
		result = min(si.nPos + (int)si.nPage, maxPos);
		break;

	case SB_THUMBPOSITION:
		// do nothing
		break;

	case SB_THUMBTRACK:
		result = si.nTrackPos;
		break;

	case SB_TOP /*SB_LEFT*/:
		result = minPos;
		break;

	case SB_BOTTOM /*SB_RIGHT*/:
		result = maxPos;
		break;

	case SB_ENDSCROLL:
		// do nothing
		break;
	}

	return result;
}

BOOL SD_OnInitDialog(HWND hwnd, int width , int height, int _npage )
{

	const SIZE sz = { width, height };
	SCROLLINFO si = {};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nPos = si.nMin = 1;

	si.nMax = sz.cx;
	si.nPage = sz.cx;
	SetScrollInfo(hwnd, SB_HORZ, &si, FALSE);

	si.nMax = sz.cy;
	si.nPage = _npage;
	SetScrollInfo(hwnd, SB_VERT, &si, FALSE);

	return FALSE;
}
