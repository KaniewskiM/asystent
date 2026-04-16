#include "utils.h"

std::string wyczyscJson(std::string tekst) {
    size_t p;
    while ((p = tekst.find("\\n"))     != std::string::npos) tekst.replace(p, 2, "\n");
    while ((p = tekst.find("\\\""))    != std::string::npos) tekst.replace(p, 2, "\"");
    while ((p = tekst.find("\\u0026")) != std::string::npos) tekst.replace(p, 6, "&");
    return tekst;
}
