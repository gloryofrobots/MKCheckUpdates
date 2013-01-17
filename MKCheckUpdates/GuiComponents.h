#pragma once;

#include <cstddef>
#include <string>
#include <windows.h>
#include "CommCtrl.h"

#define ELEMENT_TEXT_LENGTH 500

class ElementWithPosition
{
public:
	ElementWithPosition( );

	virtual ~ElementWithPosition();
	virtual void setPosition( std::size_t _x ,std::size_t _y ,std::size_t _width , std::size_t _height  );
	virtual void getPosition( std::size_t & _x ,std::size_t & _y ,std::size_t  & _width , std::size_t & _height  );

protected:
	std::size_t m_x;
	std::size_t m_y;
	std::size_t m_width;
	std::size_t m_height;
};


class ElementWithText
{

public:
	ElementWithText();
	virtual ~ElementWithText();

	virtual void setText( LPCSTR _txt );
	void setDouble( double _integer );
	void setString( std::string _txt );
	virtual void getText( char * _buffer, std::size_t _size );
	virtual double getDouble( );

protected:
	char m_txt[ELEMENT_TEXT_LENGTH];

};

class ControlField
	: public ElementWithText
	, public ElementWithPosition
{
public:
	ControlField();
	virtual ~ControlField();
	virtual void setWindowAttr( LPCTSTR lpWindowName, DWORD dwStyle, HWND hWndParent, HMENU hMenu, LPVOID lpParam );
	virtual void createWindow( );
	virtual void changeColor( DWORD _color );

protected:
	HWND m_window;
	LPCTSTR m_lpWindowName;
	LPCTSTR m_lpClassName;
	DWORD m_dwStyle;
	HWND m_hWndParent;
	LPVOID m_lpParam;
	HMENU m_hMenu;

};


class ControlFieldStatic
	: public ControlField
{
public:
	ControlFieldStatic();
	virtual ~ControlFieldStatic();

	void setText( LPCSTR _txt ) override ;
};

class ControlFieldEdit
	: public ControlField
{
public:
	ControlFieldEdit();
	virtual ~ControlFieldEdit();

	void setText( LPCSTR _txt ) override;
	void catchText();
};

class ControlFieldButton
	: public ControlField
{
public:
	ControlFieldButton();
	virtual ~ControlFieldButton();

};

class ControlFieldCheckBox
	: public ControlFieldButton
{
	bool m_isChecked;
public:
	ControlFieldCheckBox();
	virtual ~ControlFieldCheckBox();
	void check( bool _check );
	void catchChecked();
	bool isChecked();

};

BOOL SD_OnInitDialog(HWND hwnd,  int width , int height, int _npage );
void SD_OnSize(HWND hwnd, UINT state, int cx, int cy);
void SD_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void SD_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void SD_OnHVScroll(HWND hwnd, int bar, UINT code);
void SD_ScrollClient(HWND hwnd, int bar, int pos);
int SD_GetScrollPos(HWND hwnd, int bar, UINT code);