#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>
#include <deque>
#include <thread>
#include <message.h>

class Network {
public:
	Network(std::string ip, std::string port, std::string username);
	void write(std::vector<uint8_t> data);
	void close();
	~Network();
private:
	void connect(const asio::ip::tcp::resolver::results_type& endpoints);
	void do_write();
	void do_read_header();
	void do_read_body(msg_header header);
	bool is_connected();
	bool reconnect();
	void do_reg();
	asio::io_context m_ctx;
	asio::ip::tcp::socket m_socket;
	std::deque<std::vector<uint8_t>> m_msgq;
	std::thread m_ctx_thrd;
	std::string m_username;
	bool m_isreg = false;
	std::vector<uint8_t> m_buffer = {};
};

