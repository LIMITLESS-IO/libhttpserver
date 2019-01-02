/*
     This file is part of libhttpserver
     Copyright (C) 2011-2019 Sebastiano Merlino

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
     USA
*/

#if defined(__MINGW32__) || defined(__CYGWIN32__)
#define _WINDOWS
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#include "littletest.hpp"
#include <curl/curl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "httpserver.hpp"

using namespace std;
using namespace httpserver;

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s)
{
    s->append((char*) ptr, size*nmemb);
    return size*nmemb;
}

class ok_resource : public httpserver::http_resource
{
    public:
        const httpserver::http_response render_GET(const httpserver::http_request& req)
        {
            return httpserver::http_response_builder("OK", 200, "text/plain").string_response();
        }
};

LT_BEGIN_SUITE(ws_start_stop_suite)
    void set_up()
    {
    }

    void tear_down()
    {
    }
LT_END_SUITE(ws_start_stop_suite)

LT_BEGIN_AUTO_TEST(ws_start_stop_suite, start_stop)
    {
    webserver ws = create_webserver(8080);
    ok_resource* ok = new ok_resource();
    ws.register_resource("base", ok);
    ws.start(false);

    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    LT_CHECK_EQ(s, "OK");
    curl_easy_cleanup(curl);

    ws.stop();
    }

    {
    webserver ws = create_webserver(8080).start_method(http::http_utils::INTERNAL_SELECT);
    ok_resource* ok = new ok_resource();
    ws.register_resource("base", ok);
    ws.start(false);

    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    LT_CHECK_EQ(s, "OK");
    curl_easy_cleanup(curl);

    ws.stop();
    }

    {
    webserver ws = create_webserver(8080).start_method(http::http_utils::THREAD_PER_CONNECTION);
    ok_resource* ok = new ok_resource();
    ws.register_resource("base", ok);
    ws.start(false);

    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    LT_CHECK_EQ(s, "OK");
    curl_easy_cleanup(curl);

    ws.stop();
    }
LT_END_AUTO_TEST(start_stop)

LT_BEGIN_AUTO_TEST(ws_start_stop_suite, disable_options)
    webserver ws = create_webserver(8080)
        .no_ssl()
        .no_ipv6()
        .no_debug()
        .no_pedantic()
        .no_basic_auth()
        .no_digest_auth()
        .no_comet()
        .no_regex_checking()
        .no_ban_system()
        .no_post_process();
    ok_resource* ok = new ok_resource();
    ws.register_resource("base", ok);
    ws.start(false);

    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    LT_CHECK_EQ(s, "OK");
    curl_easy_cleanup(curl);

    ws.stop();
LT_END_AUTO_TEST(disable_options)

LT_BEGIN_AUTO_TEST(ws_start_stop_suite, enable_options)
    webserver ws = create_webserver(8080)
        .debug()
        .pedantic()
        .comet()
        .regex_checking()
        .ban_system()
        .post_process();
    ok_resource* ok = new ok_resource();
    ws.register_resource("base", ok);
    ws.start(false);

    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    LT_CHECK_EQ(s, "OK");
    curl_easy_cleanup(curl);

    ws.stop();
LT_END_AUTO_TEST(enable_options)

LT_BEGIN_AUTO_TEST(ws_start_stop_suite, custom_socket)
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8181);
    bind(fd, (struct sockaddr*) &address, sizeof(address));
    listen(fd, 10000);

    webserver ws = create_webserver(-1).bind_socket(fd); //whatever port here doesn't matter
    ok_resource* ok = new ok_resource();
    ws.register_resource("base", ok);
    ws.start(false);

    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8181/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    LT_CHECK_EQ(s, "OK");
    curl_easy_cleanup(curl);

    ws.stop();
LT_END_AUTO_TEST(custom_socket)

LT_BEGIN_AUTO_TEST_ENV()
    AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
