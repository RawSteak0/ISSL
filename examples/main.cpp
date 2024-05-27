#define SERVER_PERROR
#include "../include/request.hpp"
#include "../include/response.hpp"
#include <cstdio>
#include <iostream>

#define PORT 8080

int main() {
  const char *raw_request =
      "GET / HTTP/1.1\r\n"
      "Host: localhost:8080\r\n"
      "Connection: keep-alive\r\n"
      "Upgrade-Insecure-Requests: 1\r\n"
      "Accept: "
      "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
      "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) "
      "AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 "
      "Safari/604.5.6\r\n"
      "Accept-Language: en-us\r\n"
      "DNT: 1\r\n"
      "Accept-Encoding: gzip, deflate\r\n"
      "\r\n"
      "Usually GET requests don\'t have a body\r\n"
      "But I don\'t care in this case :)";
  
  request::RequestPP request = request::parse_request(raw_request, -1);
  
  if (!request.broken) {
    printf("Method: %d\n", request.method);
    printf("Request-URI: %s\n", request.url.c_str());
    printf("HTTP-Version: %s\n", request.version.c_str());
    puts("Headers:");
    for (request::Header header : request.headers) {
      printf("%32s: %s\n", header.name, header.value);
    }
    puts("message-body:");
    puts(request.body.c_str());
    
    std::cout << "\n================\n\ngenerated response:\n\n" << response::generate_response("HTTP/1.0", {2,0,0}, "OK", {
      {"Server","c++"},
      {"Content-Length","15"},
      {"Content-Type","text/plain"},
    }, "wazzup, Bejing?") << "\n";
    
  }
  return 0;
}
