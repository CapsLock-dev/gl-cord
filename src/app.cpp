#include <app.h>
#include <frame.h>

MyApp::MyApp() {}
MyApp::~MyApp() {}
bool MyApp::OnInit() {
	m_frame1 = new MyFrame();
	m_frame1->Show();

	return true;
}
