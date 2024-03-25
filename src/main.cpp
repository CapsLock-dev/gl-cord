#include <iostream>
#include <app.h>

//#define cl_debug

#ifndef cl_debug
wxIMPLEMENT_APP(MyApp);
#endif

#ifdef cl_debug
#include <network.h>
int main() {
//	Network net("127.0.0.1", "1922");
//	using namespace std::chrono_literals;
//	std::this_thread::sleep_for(2000ms);
//	net.write("test msg");
//	using namespace std::chrono_literals;
//	std::this_thread::sleep_for(2000ms);
	MessageTests::start();
	return 0;
}
#endif
