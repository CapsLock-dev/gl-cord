#include <frame.h>
#include <wx/gdicmn.h>

enum {
	ID_SendBtn,
	ID_MsgBox,
	ID_ListBox,
};

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Chat Client"), m_net("188.244.191.140", "1922", "CapsLock") { 
	this->SetBackgroundColour(wxColor(30, 31, 34));

	wxPanel *server_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel *channels_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel *user_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

	wxPanel *chat_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel *msg_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel *send_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	
	wxPanel *top_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	
	m_listbox = new wxListBox(top_panel, ID_ListBox, wxDefaultPosition, wxSize(100,100));
	m_listbox->SetBackgroundColour(wxColor(30,31,34));

	wxPanel *bottom_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 40));
	bottom_panel->SetBackgroundColour(wxColor(148, 156, 247));

	m_msgbox = new wxTextCtrl(bottom_panel, ID_MsgBox, "", wxDefaultPosition, wxSize(100, 40));
	m_sendbtn = new wxButton(bottom_panel, ID_SendBtn, "Send", wxDefaultPosition, wxSize(100, 40));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(top_panel, 3, wxEXPAND);
	sizer->Add(bottom_panel, 0, wxEXPAND);
	
	wxBoxSizer *top_sizer = new wxBoxSizer(wxHORIZONTAL);
	top_sizer->Add(m_listbox, 1, wxEXPAND | wxALL, 10);

	top_panel->SetSizerAndFit(top_sizer);

	wxBoxSizer *bottom_sizer = new wxBoxSizer(wxHORIZONTAL);
	bottom_sizer->Add(m_msgbox, 1, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT, 10);
	bottom_sizer->Add(m_sendbtn, 0, wxEXPAND | wxALL, 10);

	bottom_panel->SetSizerAndFit(bottom_sizer);

	this->SetSizerAndFit(sizer);
	Bind(wxEVT_BUTTON, &MyFrame::OnSendClick, this, ID_SendBtn);
}

MyFrame::~MyFrame() { 
}

void MyFrame::OnSendClick(wxCommandEvent& event) {
	std::string text = m_msgbox->GetValue().ToStdString();
	ChatMessage msg(text);
	m_net.write(msg.get_data());
	m_msgbox->SetValue("");
}
