#include "utils.h"

std::string wyczyscJson(std::string tekst) {
  size_t p;
  while ((p = tekst.find("\\n")) != std::string::npos)
    tekst.replace(p, 2, "\n");
  while ((p = tekst.find("\\\"")) != std::string::npos)
    tekst.replace(p, 2, "\"");
  while ((p = tekst.find("\\u0026")) != std::string::npos)
    tekst.replace(p, 6, "&");
  return tekst;
}

std::string zabezpieczJson(std::string tekst) {
  std::string wynik;
  wynik.reserve(tekst.length() * 1.1); // mała optymalizacja
  for (char c : tekst) {
    if (c == '\n') {
      wynik += "\\n";
    } else if (c == '\r') {
      wynik += "\\r";
    } else if (c == '\"') {
      wynik += "\\\"";
    } else if (c == '\\') {
      wynik += "\\\\";
    } else if (c == '\t') {
      wynik += "\\t";
    } else {
      wynik += c;
    }
  }
  return wynik;
}
