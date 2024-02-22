#include <frame.h>

enum {
	ID_SendBtn,
	ID_MsgBox
};

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Chat Client") { 
	m_sendbtn = new wxButton(this, ID_SendBtn, "Send", wxPoint(300, 100), wxSize(100, 50));
	m_msgbox = new wxTextCtrl(this, ID_MsgBox, "", wxPoint(100, 100), wxSize(200, 50));
	Bind(wxEVT_BUTTON, &MyFrame::OnSendClick, this, ID_SendBtn);
}

MyFrame::~MyFrame() { 
}

void MyFrame::OnSendClick(wxCommandEvent& event)
{
	std::cout << m_msgbox->GetValue() << "\n";
	m_msgbox->SetValue("");
}
