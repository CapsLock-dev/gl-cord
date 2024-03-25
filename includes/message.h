#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>

enum class MessageType {
	Undefined,
	ChatMessage,
	LoginMessage,
	_Count,
};

struct msg_header {
	uint32_t body_size=0;
	MessageType type=MessageType::Undefined;
	void bytes_to_header(std::vector<uint8_t> bytes) {
		type=(MessageType)bytes[0];
		std::memcpy(&body_size, bytes.data()+1, bytes.size()-1);
	}
	std::vector<uint8_t> header_to_bytes() const {	
		char size_data[sizeof(msg_header::body_size)] = {0};
		std::copy((uint8_t*)((void*)&body_size), (uint8_t*)((void*)&body_size) + sizeof(msg_header::body_size), size_data);
		std::vector<uint8_t> buffer;
		buffer.push_back((uint8_t)type);
		std::copy(std::begin(size_data), std::end(size_data), std::back_inserter(buffer));
		return buffer;
	};
};
inline constexpr const size_t max_body_size = 1024;
inline constexpr const size_t msg_header_size = sizeof(msg_header::body_size) + 1;

class IMessage {
public:
	IMessage() {};
	virtual ~IMessage() {};
	virtual std::vector<uint8_t> get_data() const = 0;
	virtual uint32_t get_size() const = 0;
	virtual msg_header get_header() const = 0;
};
// первый байт - тип
// следующие 4 - размер
// далее 25 байтов имени пользователя
struct loginmsg_data {
	std::string m_username;
	void bytes_to_logindata(std::vector<uint8_t> bytes) {
		m_username = std::string(bytes.begin(), bytes.end());
	}
	std::vector<uint8_t> logindata_to_bytes() const {
		std::vector<uint8_t> bytes;
		bytes.reserve(m_username.size());
		std::copy(m_username.begin(), m_username.end(), std::back_inserter(bytes));
		return bytes;
	}	
};
class LoginMessage : public IMessage {
public:
	LoginMessage(std::string username) {
		if (username.size() > 25) {
			std::cout << "Wrong username length\n";
			exit(1);
		}
		m_data.m_username = username;
		m_header.body_size = username.length();
	}
	LoginMessage(std::vector<uint8_t> data) {
		std::vector<uint8_t> bytes(data.begin(), data.begin()+msg_header_size);
		m_header.bytes_to_header(bytes);
		std::vector<uint8_t> logindata(data.begin()+msg_header_size, data.end());
		m_data.bytes_to_logindata(logindata);
	}
	LoginMessage(msg_header header, std::vector<uint8_t> data) {
		m_header.body_size = header.body_size;
		m_data.bytes_to_logindata(data);
	}
	~LoginMessage() {
		IMessage::~IMessage();
	}	
	uint32_t get_size() const override {
		return m_header.body_size;
	}
	msg_header get_header() const override {
		return m_header;
	}
	std::string get_name() {
		return m_data.m_username;
	}
	std::vector<uint8_t> get_data() const override {
		std::vector<uint8_t> buff_header = m_header.header_to_bytes();
		std::vector<uint8_t> buff_data = m_data.logindata_to_bytes();
		buff_header.insert(buff_header.end(), buff_data.begin(), buff_data.end());
		return buff_header;
	}
private:
	msg_header m_header={.type=MessageType::LoginMessage};
	loginmsg_data m_data={};
};

// первый байт - тип
// следующие 4 байта - размер
// далее 4 байта айди сервера
// далее тело с заданным размером
class ChatMessage : public IMessage {
public:
	ChatMessage(std::string data) {	
		m_body.reserve(data.length());
		for (char c : data) {
			m_body.push_back(c);
		}
		m_header.body_size = data.size();
	}
	ChatMessage(std::vector<uint8_t> data) {
		std::vector<uint8_t> bytes(data.begin(), data.begin()+msg_header_size);
		m_header.bytes_to_header(bytes);
		m_body.reserve(data.size()-msg_header_size);
		std::copy(data.begin()+msg_header_size, data.end(), std::back_inserter(m_body));
	}
	ChatMessage(msg_header header, std::vector<uint8_t> body_buffer) {
		m_header.body_size = header.body_size;
		m_body.reserve(header.body_size);
		m_body = body_buffer;
	}
	~ChatMessage() {
		IMessage::~IMessage();
	}
	uint32_t get_size() const override {
		return m_header.body_size;
	}
	msg_header get_header() const override {
		return m_header;
	}
	std::vector<uint8_t> get_data() const override {
		std::vector<uint8_t> header_data = m_header.header_to_bytes();
		header_data.reserve(m_header.body_size+msg_header_size);
		std::copy(m_body.begin(), m_body.end(), std::back_inserter(header_data));
		return header_data;
	}
	std::string get_message() {
		return std::string(m_body.begin(), m_body.end());
	}
private:
	msg_header m_header={.type=MessageType::ChatMessage};
	std::vector<uint8_t> m_body = {};
};

class MessageTests {
public:
	MessageTests() {};
	~MessageTests() {};
	static void start() {
		chatmsg_test();
		loginmsg_test();
		msgheader_test();
		logindata_test();
	}
private:
	static void msgheader_test() {
		msg_header header1 = { .body_size = 12, .type = MessageType::LoginMessage };
		msg_header header2;
		header2.bytes_to_header(header1.header_to_bytes());
		if (header1.body_size == header2.body_size && header1.type == header2.type) {
			std::cout << "[Test][+] msg_header test passed\n";
		} else {
			std::cout << "[Test][-] msg_header test failed\n";
		}
	}
	static void logindata_test() {
		loginmsg_data data1 = {.m_username = "CapsLock"};
		loginmsg_data data2;
		data2.bytes_to_logindata(data1.logindata_to_bytes());
		if (data1.m_username == data2.m_username) {
			std::cout << "[Test][+] logindata test passed\n";
		} else {
			std::cout << "[Test][-] logindata test failed\n";
		}
	}
	static void chatmsg_test() {
		ChatMessage msg1("Hello world"); 
		ChatMessage msg2(msg1.get_data());
		std::vector<uint8_t> a = msg1.get_data();
		std::vector<uint8_t> body(a.begin()+msg_header_size, a.end());
		ChatMessage msg3(msg1.get_header(), body);
		if (msg2.get_data() == msg1.get_data() && msg2.get_message() == msg1.get_message() && msg3.get_data() == msg1.get_data() && msg3.get_message() == msg1.get_message()) {
			std::cout << "[Test][+] ChatMessage test passed. Message: " << msg2.get_message() << "\n";	
		} else {
			std::cout << "[Test][-] ChatMessage test failed. Message: " << msg3.get_message() << "\n";
		}
	}
	static void loginmsg_test() {
		LoginMessage msg1("CapsLock");
		LoginMessage msg2(msg1.get_data());
		if (msg1.get_data() == msg2.get_data()) {
			std::cout << "[Test][+] LoginMessage test passed\n";	
		} else {
			std::cout << "[Test][-] LoginMessage test failed\n";
		}
	}
};
