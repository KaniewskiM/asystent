# 🚀 Krypto Asystent AI

Profesjonalna aplikacja desktopowa zbudowana w **Qt 6**, służąca do edukacji, analizy rynkowej i śledzenia nowości ze świata kryptowalut. Projekt integruje sztuczną inteligencję (Ollama/Llama 3.1 & Google Gemini) z danymi giełdowymi w czasie rzeczywistym.

## ✨ Kluczowe Funkcje
- **Asystent Edukacyjny:** Zadawaj pytania o blockchain i krypto (Powered by AI).
- **Analiza Techniczna:** Zaawansowane raporty rynkowe oparte na danych z 24h.
- **Wykresy & Trend:** Generowanie interaktywnych wykresów świecowych (Python/Plotly).
- **Biuletyn News:** Tłumaczenie i streszczanie najświeższych wiadomości z CoinTelegraph.
- **Podaż Rynkowa:** Automatyczne pobieranie circulating supply z API CoinGecko.
- **Kalkulator Podatkowy:** Obliczanie należnego podatku od zysków kapitałowych.

## 🛠 Technologia
- **Frontend:** Qt 6 (Widgets, CMake)
- **Backend:** C++ 17, QtConcurrent, QtNetwork
- **AI:** Google Gemini API v1.5 / Llama 3.1 (Ollama)
- **Data Science:** Python 3 (Pandas, Plotly)
- **API:** Binance, CoinGecko, RSS CoinTelegraph

## 📖 Dokumentacja
Projekt posiada pełną dokumentację technologiczną w formacie **Doxygen**. Aby ją wygenerować, użyj polecenia:
```bash
doxygen Doxyfile
```

## 🔐 Konfiguracja
Wymagane jest utworzenie pliku `api_key.txt` w głównym katalogu i wklejenie tam swojego klucza Gemini API. Dla lokalnej analizy wymagane jest uruchomienie serwera Ollama.

---
*Projekt stworzony w celach edukacyjnych i analitycznych.*
