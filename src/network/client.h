#pragma once
#include <iostream>
#include "asio.hpp"

namespace SAS_3D {
	namespace Network {
		using namespace asio::ip;
		class Client {
		public:
			Client(asio::io_service& io_service,
				tcp::resolver::iterator endpoint_iterator)
				: _io_service(io_service)
				, _socket(io_service)
				, _message_body(1024)
			{
				_connect(endpoint_iterator);
			}

		private:
			void _connect(tcp::resolver::iterator endpoint_iterator) {
				asio::async_connect(_socket, endpoint_iterator,
					[this](std::error_code ec, tcp::resolver::iterator) {
					if (!ec) {
						_read_header();
					}
				});
			}

			void _read_header() {
				asio::async_read(_socket,
					asio::buffer(&_header, 4),
					asio::transfer_exactly(4),
					[this](std::error_code ec, std::size_t /*length*/)
				{
					if (!ec) {
						_body_length = _header;
						_read_body();
					}
					else {
						_socket.close();
					}
				});
			}

			void _read_body() {
				asio::async_read(_socket,
					asio::buffer(_message_body),
					asio::transfer_exactly(_body_length),
					[this](std::error_code ec, size_t bytes_received) {
					if (!ec) {
						std::cout.write(_message_body.data(), _body_length) << std::endl;
						_read_header();
					}
					else {
						_socket.close();
					}
				});
			}

			asio::io_service& _io_service;
			tcp::socket _socket;
			int _header;
			int _body_length;
			std::vector<char> _message_body;
		};
	}
}