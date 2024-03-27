#include <network.h>

Network::Network(std::string ip, std::string port, std::string username): m_ctx(), m_socket(m_ctx), m_username(username) {
	asio::ip::tcp::resolver resolver(m_ctx);
    auto endpoints = resolver.resolve(ip, port);
	connect(endpoints);
	m_ctx_thrd = std::thread([this]() { m_ctx.run();} );
}

Network::~Network() {
	this->close();
}

void Network::connect(const asio::ip::tcp::resolver::results_type& endpoints) {
	asio::async_connect(m_socket, endpoints, [this](std::error_code ec, asio::ip::tcp::endpoint) {
		if (!ec) {
			if (!m_isreg) do_reg();
			std::cout << "Connected to the server\n";
			this->do_read_header();
		}
	});
}

void Network::write(std::vector<uint8_t> data) {
	asio::post(m_ctx, [this, data]() {
		bool write_in_progress = !m_msgq.empty();
		m_msgq.push_back(data);
		if (!write_in_progress)
		{
			do_write();
		}
	});
}

void Network::close()
{
	asio::post(m_ctx, [this]() { m_socket.close(); std::cout << "Closed connection\n"; });
}

void Network::do_write() {
	auto lambda = [this](std::error_code ec, std::size_t /*length*/) {
		if (ec) {
			close();
		} else {
			m_msgq.pop_front();
			if (!m_msgq.empty())
				do_write();
		}
	};
	asio::async_write(m_socket, asio::buffer(m_msgq.front().data(), m_msgq.front().size()), lambda);
}

void Network::do_read_header() {
	m_buffer.clear();
	m_buffer.resize(msg_header_size);
	auto lambda =  [this](std::error_code ec, std::size_t /*length*/) {
		if (!ec) {
			if (m_buffer.size() > 0) {
				msg_header header;
				header.bytes_to_header(m_buffer);
				std::cout << "Got message, type: " << (int)header.type << " size: " << header.body_size << "\n";
				do_read_body(header);
			} else {
				std::cout << "Empty msg header\n";
				do_read_header();
			}
		} else {
			std::cout << "Error: " << ec << "\n";
			close();
		}
	};
	asio::async_read(m_socket, asio::buffer(m_buffer.data(), msg_header_size), lambda);
}

void Network::do_read_body(msg_header header) {
	m_buffer.clear();
	m_buffer.resize(header.body_size);
	auto lambda = [header, this](std::error_code ec, std::size_t /*length*/) {
		if (!ec) {
			if (m_buffer.size() > 0) {
				ChatMessage msg(header, m_buffer);
				std::cout << "Msg: " << msg.get_message() << "\n";
			} else {
				std::cout << "Empty message\n";
			}
			do_read_header();
		} else {
			close();
		}
	};
	asio::async_read(m_socket, asio::buffer(m_buffer.data(), header.body_size), lambda);
}

void Network::do_reg() {
	m_isreg = true;
	LoginMessage login(m_username);
	write(login.get_data());
}
