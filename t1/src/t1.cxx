#include <wx/wx.h>
#include <cassert>
#include "t1.h"

IMPLEMENT_APP(BasicApplication)

bool BasicApplication::OnInit() {
    BasicFrame *frame = new BasicFrame("Basic", 50, 50, 450, 300);
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

BasicFrame::BasicFrame(const wxChar *title, int xpos, int ypos,
    int width, int height) : wxFrame((wxFrame *) NULL, -1, title, wxPoint(xpos, ypos),
                                     wxSize(width, height))
{
	theText = (wxTextCtrl *) NULL;
	menuBar  = (wxMenuBar *) NULL;
	fileMenu = (wxMenu *) NULL;
    theText = new wxTextCtrl(this, -1, wxString("Hello, world!"), wxDefaultPosition,
                wxDefaultSize, wxTE_MULTILINE);
  fileMenu = new wxMenu;
  fileMenu->Append(BASIC_OPEN,  "&Open file");
  fileMenu->Append(BASIC_ABOUT, "&About");
  fileMenu->AppendSeparator();
  fileMenu->Append(BASIC_EXIT,  "E&xit");

  menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, "&File");
  SetMenuBar(menuBar);
}

BasicFrame::~BasicFrame()
{
}

