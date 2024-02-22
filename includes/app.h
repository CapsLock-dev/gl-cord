#pragma once
#include <frame.h>
#include <wx/wx.h>

class MyApp : public wxApp 
{
public:
	MyApp();
	~MyApp();
	bool OnInit() override;
private:
	MyFrame* m_frame1;
};
