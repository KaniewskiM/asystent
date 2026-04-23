/**
 * @file cryptoexchange.cpp
 * @brief Implementacja silnika wymiany danych z giełdami i API zewnętrznymi.
 *
 * Klasa odpowiada za pobieranie cen, wskaźników 24h, danych historycznych
 * (świec), podaży monet oraz aktualności rynkowych przy użyciu narzędzia curl.
 */
#include "cryptoexchange.h"
#include <chrono>
#include <cstdlib>
#include <fstream>

CryptoExchange::CryptoExchange() {}

/**
 * @brief Pobiera aktualną cenę wybranego symbolu.
 * @param symbol Kod pary (np. BTCUSDT).
 * @return Cena jako string lub "BLAD" w przypadku niepowodzenia.
 */
std::string CryptoExchange::fetchCurrentPrice(const std::string &symbol) {
  using namespace std;
  string cenaPobrana = "BLAD";

  // Generowanie unikalnej nazwy pliku tymczasowego na podstawie czasu
  auto ms = chrono::duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch())
                .count();
  string file = "price_" + to_string(ms) + ".txt";

  // PRÓBA 1: Binance API
  string cmdB =
      "curl -s \"https://api.binance.com/api/v3/ticker/price?symbol=" + symbol +
      "\" > " + file;
  system(cmdB.c_str());

  ifstream fb(file);
  if (fb.is_open()) {
    string s;
    getline(fb, s);
    fb.close();
    size_t pos = s.find("\"price\":\"");
    if (pos != string::npos && s.find("code") == string::npos) {
      cenaPobrana = s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9));
    }
  }

  // PRÓBA 2: MEXC API (Fallback, jeśli Binance zawiedzie)
  if (cenaPobrana == "BLAD") {
    string cmdM =
        "curl -s \"https://api.mexc.com/api/v3/ticker/price?symbol=" + symbol +
        "\" > " + file;
    system(cmdM.c_str());
    ifstream fm(file);
    if (fm.is_open()) {
      string s;
      getline(fm, s);
      fm.close();
      size_t pos = s.find("\"price\":\"");
      if (pos != string::npos) {
        cenaPobrana = s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9));
      }
    }
  }

  // Formatowanie ceny: usuwanie zbędnych zer na końcu dla lepszej czytelności
  if (cenaPobrana != "BLAD" && symbol != "BTCPLN" && symbol != "BTCUSDT") {
    while (cenaPobrana.back() == '0')
      cenaPobrana.pop_back();
    if (cenaPobrana.back() == '.')
      cenaPobrana.pop_back();
  }

  std::remove(file.c_str()); // Sprzątanie pliku tymczasowego
  return cenaPobrana;
}

/**
 * @brief Pomocnicza metoda do wyciągania wartości tekstowej z surowego formatu
 * JSON.
 */
std::string CryptoExchange::extractJsonValue(const std::string &json,
                                             const std::string &key) {
  using namespace std;
  size_t pos = json.find("\"" + key + "\"");
  if (pos == string::npos)
    return string("0");

  size_t colon = json.find(":", pos);
  size_t startQuote = json.find("\"", colon);

  if (startQuote != string::npos) {
    size_t endQuote = json.find("\"", startQuote + 1);
    if (endQuote != string::npos) {
      return json.substr(startQuote + 1, endQuote - startQuote - 1);
    }
  }
  return string("0");
}

/**
 * @brief Pobiera statystyki rynkowe z ostatnich 24h (wolumen, zmiana %,
 * max/min).
 */
CryptoExchange::MarketStats
CryptoExchange::fetchMarketData24h(const std::string &symbol) {
  using namespace std;
  MarketStats stats = {"0", "0", "0", "0", "0", false};

  auto ms = chrono::duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch())
                .count();
  string file = "ind_" + to_string(ms) + ".txt";

  // Pobieranie danych z Binance
  system(("curl -s \"https://api.binance.com/api/v3/ticker/24hr?symbol=" +
          symbol + "\" > " + file)
             .c_str());
  ifstream f1(file);
  string c1((istreambuf_iterator<char>(f1)), istreambuf_iterator<char>());
  f1.close();

  // Fallback na MEXC w przypadku błędu (np. symbolu brak na Binance)
  if (c1.length() < 20 || c1.find("code") != string::npos) {
    system(("curl -s \"https://api.mexc.com/api/v3/ticker/24hr?symbol=" +
            symbol + "\" > " + file)
               .c_str());
    ifstream f2(file);
    c1 = string((istreambuf_iterator<char>(f2)), istreambuf_iterator<char>());
    f2.close();
  }

  // Jeśli dane są poprawne, parsujemy kluczowe pola
  if (c1.length() >= 20 && c1.find("code") == string::npos) {
    stats.success = true;
    stats.price = extractJsonValue(c1, "lastPrice");
    stats.changePercent = extractJsonValue(c1, "priceChangePercent");
    stats.quoteVolume = extractJsonValue(c1, "quoteVolume");
    stats.highPrice = extractJsonValue(c1, "highPrice");
    stats.lowPrice = extractJsonValue(c1, "lowPrice");
  }

  std::remove(file.c_str());
  return stats;
}

/**
 * @brief Pobiera dane o świecach (historyczne ceny) i zapisuje je do pliku dla
 * skryptu Python.
 */
bool CryptoExchange::fetchCandles(const std::string &symbol,
                                  const std::string &interval,
                                  long long startMs, long long endMs,
                                  const std::string &outputFile) {
  using namespace std;

  // Próba pobrania świec z Binance
  string cmdBinance =
      "curl -s \"https://api.binance.com/api/v3/klines?symbol=" + symbol +
      "&interval=" + interval + "&startTime=" + to_string(startMs) +
      "&endTime=" + to_string(endMs) + "\" > " + outputFile;
  system(cmdBinance.c_str());

  ifstream f1(outputFile);
  string c1((istreambuf_iterator<char>(f1)), istreambuf_iterator<char>());
  f1.close();

  if (c1.length() > 20 && c1.find("code") == string::npos) {
    return true;
  }

  // Próba pobrania świec z MEXC
  string cmdMexc =
      "curl -s \"https://api.mexc.com/api/v3/klines?symbol=" + symbol +
      "&interval=" + interval + "&startTime=" + to_string(startMs) +
      "&endTime=" + to_string(endMs) + "\" > " + outputFile;
  system(cmdMexc.c_str());

  ifstream f2(outputFile);
  string c2((istreambuf_iterator<char>(f2)), istreambuf_iterator<char>());
  f2.close();

  if (c2.length() > 20 && c2.find("code") == string::npos) {
    return true;
  }

  return false;
}

/**
 * @brief Pobiera aktualną podaż monety z API CoinGecko (potrzebne do
 * kapitalizacji).
 */
double CryptoExchange::fetchSupply(const std::string &symbol) {
  using namespace std;

  // Normalizacja symbolu (usunięcie końcówek USDT/PLN i zamiana na małe litery
  // dla CoinGecko)
  string baseSymbol = symbol;
  if (baseSymbol.length() > 4 &&
      baseSymbol.substr(baseSymbol.length() - 4) == "USDT") {
    baseSymbol = baseSymbol.substr(0, baseSymbol.length() - 4);
  } else if (baseSymbol.length() > 3 &&
             baseSymbol.substr(baseSymbol.length() - 3) == "PLN") {
    baseSymbol = baseSymbol.substr(0, baseSymbol.length() - 3);
  }
  for (char &c : baseSymbol)
    c = tolower(c);

  auto ms = chrono::duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch())
                .count();
  string file = "supply_" + to_string(ms) + ".txt";

  // Zapytanie do CoinGecko o rynkową podaż obiegu
  string cmd = "curl -s "
               "\"https://api.coingecko.com/api/v3/coins/"
               "markets?vs_currency=usd&symbols=" +
               baseSymbol + "\" > " + file;
  system(cmd.c_str());

  double parsedSupply = 0.0;
  ifstream f(file);
  if (f.is_open()) {
    string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();

    string key = "\"circulating_supply\":";
    size_t pos = s.find(key);
    if (pos != string::npos) {
      size_t start = pos + key.length();
      size_t end = s.find_first_of(",}", start);
      if (end != string::npos) {
        try {
          parsedSupply = stod(s.substr(start, end - start));
        } catch (...) {
        }
      }
    }
  }
  std::remove(file.c_str());
  return parsedSupply;
}

/**
 * @brief Pobiera biuletyn aktualności (RSS) z portalu CoinTelegraph i parsuje
 * go do czystego tekstu.
 */
std::string CryptoExchange::fetchLatestNews() {
  using namespace std;

  auto ms = chrono::duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch())
                .count();
  string file = "newsrss_" + to_string(ms) + ".txt";

  // Pobranie surowego pliku XML (RSS feed)
  string cmd = "curl -s \"https://cointelegraph.com/rss\" > " + file;
  system(cmd.c_str());

  string titles = "";
  ifstream f(file);
  if (f.is_open()) {
    string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();

    // Parsowanie bloków <item> z RSS
    size_t pos = 0;
    int count = 0;
    while ((pos = s.find("<item>", pos)) != string::npos && count < 30) {
      size_t itemEnd = s.find("</item>", pos);
      if (itemEnd == string::npos)
        break;

      // Wyciąganie Tytułu
      size_t titleStart = s.find("<title>", pos);
      size_t titleEnd = s.find("</title>", titleStart);
      // Wyciąganie Daty Publikacji
      size_t pubStart = s.find("<pubDate>", pos);
      size_t pubEnd = s.find("</pubDate>", pubStart);
      // Wyciąganie Opisu (Skrótu artykułu)
      size_t descStart = s.find("<description>", pos);
      size_t descEnd = s.find("</description>", descStart);

      if (titleStart != string::npos && titleEnd != string::npos &&
          titleStart < itemEnd) {
        // Czyszczenie Tytułu (obsługa CDATA)
        string rawTitle = s.substr(titleStart + 7, titleEnd - titleStart - 7);
        size_t cdataStart = rawTitle.find("<![CDATA[");
        if (cdataStart != string::npos) {
          size_t cdataEnd = rawTitle.find("]]>");
          if (cdataEnd != string::npos) {
            rawTitle =
                rawTitle.substr(cdataStart + 9, cdataEnd - cdataStart - 9);
          }
        }

        string pd = "Brak daty";
        if (pubStart != string::npos && pubEnd != string::npos &&
            pubStart < itemEnd) {
          pd = s.substr(pubStart + 9, pubEnd - pubStart - 9);
        }

        string desc = "";
        if (descStart != string::npos && descEnd != string::npos &&
            descStart < itemEnd) {
          desc = s.substr(descStart + 13, descEnd - descStart - 13);
          size_t cdataS = desc.find("<![CDATA[");
          if (cdataS != string::npos) {
            size_t cdataE = desc.find("]]>");
            if (cdataE != string::npos)
              desc = desc.substr(cdataS + 9, cdataE - cdataS - 9);
          }

          // RĘCZNY AUTOMAT DO USUWANIA TAGÓW HTML (Wyciskanie czystego tekstu
          // dla AI)
          string cleanDesc = "";
          bool inHtml = false;
          for (char c : desc) {
            if (c == '<')
              inHtml = true;
            else if (c == '>')
              inHtml = false;
            else if (!inHtml)
              cleanDesc += c;
          }
          desc = cleanDesc;
        }

        // Formowanie gotowej paczki tekstowej dla AI
        titles +=
            "- [" + pd + "] Tytuł: " + rawTitle + "\n  Opis: " + desc + "\n\n";
        count++;
      }
      pos = itemEnd;
    }
  }
  std::remove(file.c_str());
  return titles;
}
