/* 
 * t1.h 
 * 
 * header file for defining BasicApplication and BasicFrame
 */

#ifndef T1_H
#define T1_H

enum
{ BASIC_EXIT    =   1,
  BASIC_OPEN	= 100,
  BASIC_ABOUT	= 200
};

class BasicApplication : public wxApp
{
    public:
        virtual bool OnInit();
};

class BasicFrame : public wxFrame
{
    public:
        BasicFrame( const wxChar *title, int xpos, int ypos, int width, int height);
        ~BasicFrame();
        wxTextCtrl *theText;
    	wxMenuBar *menuBar;
		wxMenu *fileMenu;
};

#endif

