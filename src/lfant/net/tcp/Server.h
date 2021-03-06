
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/net/User.h>
#include <lfant/net/tcp/Connection.h>

// External
#include <boost/asio.hpp>

namespace lfant {
namespace net {
namespace tcp {

class Server : public net::User
{
public:
	Server();
	virtual ~Server();

	virtual void Init();

	virtual void Host(uint16 port = 22222, string password = "");
	void Finish();
	virtual void OnAccept(const boost::system::error_code &error, net::Connection* con);
	virtual void Accept();
	virtual void Accept(net::tcp::Connection* con);

	virtual uint32 GetConnectionCount() { return User::GetConnectionCount()-1; }

protected:
	ptr<asio::ip::tcp::acceptor> acceptor;
	string password = "";

private:
};

}
}
}
