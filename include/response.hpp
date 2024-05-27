
#pragma once
#include <array>

namespace response{
  std::string generate_response(std::string httpversion,
                  std::array<char,3> responsecode,
                         std::string responsename, std::vector<std::pair<std::string,std::string>> headers,
                         std::string body){
    std::vector<char> code = {(char)(48 + responsecode[0]), (char)(48 + responsecode[1]), (char)(48 + responsecode[2])};
    std::string response = httpversion + " ";
    response += code.data(); response += " ";
    response += responsename + "\n";
    for(std::pair<std::string,std::string> pair: headers){
      response += pair.first + ": " + pair.second + "\n";
    }
    response += "\n";
    response += body;
    return response;
  }
}
