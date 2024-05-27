// Thank you, random guy on stack overflow. 90% of this code is his
#pragma once
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
namespace request{
  
  typedef enum Method {UNSUPPORTED, GET, HEAD} Method;
  
  typedef struct Header {
    char *name;
    char *value;
    struct Header *next;
  } Header;
  
  typedef struct Request {
    enum Method method;
    char *url;
    char *version;
    struct Header *headers;
    char *body;
  } Request;

  struct RequestPP{
    enum Method method;
    std::string url;
    std::string version;
    std::vector<Header> headers;
    std::string body;
    int socket;
    bool broken = false;
  };
  
  struct Request *parse_request(const char *raw);
  struct RequestPP parse_request(const char* raw, int socket);
  void free_header(struct Header *h);
  void free_request(struct Request *req);

  //IMPLEMENTATION

  struct RequestPP parse_request(const char* raw, int socket){
    Request* request = parse_request(raw);
    RequestPP req;
    if(!request){
      req.broken = true;
      return req;
    }
    req.url = std::string(request->url);
    req.version = std::string(request->version);
    req.body = std::string(request->body);
    for(Header* h = request->headers; h; h = h-> next){
      req.headers.push_back(*h);
    }
    req.broken = false;
    req.socket = socket;
    return req;
  }

  struct Request *parse_request(const char *raw) {
      struct Request *req = NULL;
      req = (Request *)malloc(sizeof(struct Request));
      if (!req) {
          return NULL;
      }
      memset(req, 0, sizeof(struct Request));

      // Method
      size_t meth_len = strcspn(raw, " ");
      if (memcmp(raw, "GET", strlen("GET")) == 0) {
          req->method = GET;
      } else if (memcmp(raw, "HEAD", strlen("HEAD")) == 0) {
          req->method = HEAD;
      } else {
          req->method = UNSUPPORTED;
      }
      raw += meth_len + 1; // move past <SP>

      // Request-URI
      size_t url_len = strcspn(raw, " ");
      req->url = (char*)malloc(url_len + 1);
      if (!req->url) {
          free_request(req);
          return NULL;
      }
      memcpy(req->url, raw, url_len);
      req->url[url_len] = '\0';
      raw += url_len + 1; // move past <SP>

      // HTTP-Version
      size_t ver_len = strcspn(raw, "\r\n");
      req->version = (char*)malloc(ver_len + 1);
      if (!req->version) {
          free_request(req);
          return NULL;
      }
      memcpy(req->version, raw, ver_len);
      req->version[ver_len] = '\0';
      raw += ver_len + 2; // move past <CR><LF>

      struct Header *header = NULL, *last = NULL;
      while (raw[0]!='\r' || raw[1]!='\n') {
          last = header;
          header = (Header*)malloc(sizeof(Header));
          if (!header) {
              free_request(req);
              return NULL;
          }

          // name
          size_t name_len = strcspn(raw, ":");
          header->name = (char*)malloc(name_len + 1);
          if (!header->name) {
              free_request(req);
              return NULL;
          }
          memcpy(header->name, raw, name_len);
          header->name[name_len] = '\0';
          raw += name_len + 1; // move past :
          while (*raw == ' ') {
              raw++;
          }

          // value
          size_t value_len = strcspn(raw, "\r\n");
          header->value = (char*)malloc(value_len + 1);
          if (!header->value) {
              free_request(req);
              return NULL;
          }
          memcpy(header->value, raw, value_len);
          header->value[value_len] = '\0';
          raw += value_len + 2; // move past <CR><LF>

          // next
          header->next = last;
      }
      req->headers = header;
      raw += 2; // move past <CR><LF>

      size_t body_len = strlen(raw);
      req->body = (char*)malloc(body_len + 1);
      if (!req->body) {
          free_request(req);
          return NULL;
      }
      memcpy(req->body, raw, body_len);
      req->body[body_len] = '\0';


      return req;
  }


  void free_header(struct Header *h) {
      if (h) {
          free(h->name);
          free(h->value);
          free_header(h->next);
          free(h);
      }
  }


  void free_request(struct Request *req) {
      free(req->url);
      free(req->version);
      free_header(req->headers);
      free(req->body);
      free(req);
  }


}
