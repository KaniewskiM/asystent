#ifndef CRYPTOEXCHANGE_H
#define CRYPTOEXCHANGE_H

#include <string>

/**
 * @brief Klasa do pobierania danych z giełd kryptowalut.
 */
class CryptoExchange {
public:
    CryptoExchange();

    /**
     * @brief Pobiera aktualną, uśrednioną cenę kryptowaluty.
     * @param symbol Symbol pary walutowej, np. "BTCUSDT".
     * @return Zwraca uśrednioną cenę jako std::string (np. "95420.50"). Zwraca "BLAD" przy braku danych.
     */
    std::string fetchCurrentPrice(const std::string& symbol);
    
    /**
     * @brief Struktura przechowująca zaawansowane 24-godzinne dane giełdowe.
     */
    struct MarketStats {
        std::string price;         ///< Aktualna cena
        std::string changePercent; ///< Procentowa zmiana ceny w ciągu 24 godzin (np. "+5.2" lub "-1.3")
        std::string quoteVolume;   ///< Obrót wolumenu w walucie bazowej (np. USDT)
        std::string highPrice;     ///< Najwyższa odnotowana cena z 24 godzin
        std::string lowPrice;      ///< Najniższa odnotowana cena z 24 godzin
        bool success;              ///< Flaga logiczna potwierdzająca lub zaprzeczająca udanemu pobraniu zapytania
    };
    
    /**
     * @brief Pobiera pełne statystyki rynkowe z ostatnich 24h dla wybranej kryptowaluty.
     * @param symbol Symbol pary walutowej giełdy Binance (np. BTCUSDT).
     * @return Wypełniony obiekt struktury \ref MarketStats.
     */
    MarketStats fetchMarketData24h(const std::string& symbol);
    
    /**
     * @brief Pobiera historyczną bądź całkowitą wielkość podaży monet w obiegu korzystając z darmowego API CoinGecko.
     * @param symbol Złożony symbol rynkowy z pary Binance (np. "SOLUSDT"), fukcja samodzielnie go filtruje i uodparnia.
     * @return Zwraca podaż np. "19700000" jako zmienną zmiennoprzecinkową typu double lub "0.0" w razie awarii API 429.
     */
    double fetchSupply(const std::string& symbol);
    
    /**
     * @brief Bezpośrednio zczytuje publiczny kanał RSS światowych informacji kryptowalutowych celem parsowania do AI.
     * @return Zwraca wylistowany, skompresowany i wyczyszczony z flag HTML ciąg 30 nagłówków jako string.
     */
    std::string fetchLatestNews();

    /**
     * @brief Pobiera surowe dane techniczne giełdy (Świece Heikin Ashi/OHLC) dla celów rysowania i eksportuje je do pliku tekstowego.
     * @param symbol Symbol rynkowy (np. BTCUSDT) do wykresu.
     * @param interval Ścisły Interwał rysowania (np. "1h", "1d" - 1 godzina/1 dzień).
     * @param startMs Początek interwału (timestamp w milisekundach).
     * @param endMs Koniec interwału (timestamp w milisekundach).
     * @param outputFile Względna ścieżka do docelowego pliku TXT dla danych z giełdy.
     * @return Informacja true/false dotycząca powodzenia zapisu na dysku.
     */
    bool fetchCandles(const std::string& symbol, const std::string& interval, long long startMs, long long endMs, const std::string& outputFile);

private:
    std::string extractJsonValue(const std::string& json, const std::string& key);
};

#endif // CRYPTOEXCHANGE_H
