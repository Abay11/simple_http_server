#include "pch.h"
#include "CppUnitTest.h"

#include "Request.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			const char* test_request =
				"GET / HTTP/1.1\r\n"
				"Host: localhost:1080\r\n"
				"Connection: keep-alive\r\n"
				"Cache-Control: max-age=0\r\n"
				"DNT: 1\r\n"
				"Upgrade-Insecure-Requests : 1\r\n"
				"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n"
				"Sec-Fetch-Dest: document\r\n"
				"Accept: text/html\r\n"
				"Sec-Fetch-Site: cross-site\r\n"
				"Sec-Fetch-Mode: navigate\r\n"
				"Sec-Fetch-User: ?1\r\n"
				"Accept-Encoding: gzip, deflate, br\r\n"
				"Accept-Language: en-US,en;q=0.9,ru;q=0.8\r\n"
				"\r\n";

			Request reqHolder;
			parse_request(test_request, reqHolder);
			const std::string expected_method = "GET";
			const std::string expected_index = "/";
			const std::string expected_version = "HTTP/1.1";

			Assert::AreEqual(expected_method, reqHolder.method());
			Assert::AreEqual(expected_index, reqHolder.index());
			Assert::AreEqual(expected_version, reqHolder.version());
		}
	};
}
