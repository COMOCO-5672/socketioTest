// socketIOTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <memory>
#include <sio_client.h>
#include <sio_message.h>
#include <thread>

std::shared_ptr<sio::client> m_client;
bool opening = false;

int main()
{
	while (getchar())
	{
		if (m_client) {
			if (m_client->state() == 1)
			{
				std::cout << "close state:" << m_client->state() << std::endl;
				m_client->sync_close();
			}
		}
		if(!m_client)
		{
			m_client = std::make_shared<sio::client>();
		}

		if (m_client->state() != 3)
		{
			std::cout << "state:" << m_client->state() << std::endl;
			continue;
		}

		m_client->set_reconnect_delay_max(2000);
		m_client->set_reconnect_attempts(1);
		m_client->set_reconnect_delay(1000);
		auto socket = m_client->socket();
		m_client->set_close_listener([](sio::client::close_reason reason)
		{
				std::cout << "closed****" << std::endl; opening = false;
		});
		m_client->set_open_listener([]() { std::cout << "opened****" << std::endl;
		opening = false;
			/*m_client->socket()->on("text", [](const std::string &name, sio::message::ptr const &message, bool need_ack, sio::message::list &ack_message)
			{

			});*/
			/*m_client->socket()->emit("text", sio::message::list("hello"), [](const sio::message::list &)
			{
					std::cout << "will callback this ack" << std::endl;
			});*/
			});
		m_client->set_socket_close_listener([](std::string const &nsp)
		{
			std::cout << "socket_close****," << nsp << std::endl;
			m_client->socket()->emit("text", sio::message::list("hello"), [](const sio::message::list &)
			{
					std::cout << "will callback this ack" << std::endl;
			});
		});
		m_client->set_socket_open_listener([](std::string const &nsp) { std::cout << "socket_open****," << nsp << std::endl; });
		m_client->set_fail_listener([]() { std::cout << "failed*****" << std::endl; });
		m_client->set_reconnect_listener([](unsigned i, unsigned j) {std::cout << "failed***" << std::endl; });
		m_client->set_reconnecting_listener([]() { std::cout << "reconnecting****" << std::endl; });
		opening = true;
		m_client->connect("wss://localhost:8888");

		std::cout << std::this_thread::get_id << std::endl;
	}

	std::cout << "Hello World!\n";
	return 1;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
