#pragma once
#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
	MyFrame();
	~MyFrame();
private:
	wxButton *m_sendbtn;
	wxTextCtrl *m_msgbox;
    void OnSendClick(wxCommandEvent& event);
};
