#pragma once
#include <wx/wx.h>
#include <network.h>

class MyFrame : public wxFrame {
public:
	MyFrame();
	~MyFrame();
private:
	wxButton *m_sendbtn;
	wxTextCtrl *m_msgbox;
	wxListBox *m_listbox;
	Network m_net;	
    void OnSendClick(wxCommandEvent& event);
};
