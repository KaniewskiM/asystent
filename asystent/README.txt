Krypto Asystent AI
=================================================
Aplikacja desktopowa C++ (Qt6) współpracująca z lokalnymi modelami językowymi (Ollama) oraz zewnętrznym API Google Gemini. Program służy do nauki i analizy danych rynkowych kryptowalut.
Autor: Maksym Kaniewski FH TEAM

WYMAGANIA SYSTEMOWE
-------------------------------------------------
- System operacyjny: Windows / Linux
- Python 3.10+ (aby działały wykresy)

INSTALACJA ZALEZNOŚCI (Python)
-------------------------------------------------
Aby moduł wykresów działał poprawnie, musisz doinstalować dwie biblioteki do Pythona.
W wierszu poleceń (CMD/PowerShell) wpisz:
pip install -r requirements.txt

URUCHOMIENIE PROGRAMU
-------------------------------------------------
1. Upewnij się, że masz włączonego lokalnego serwera z aplikacją Ollama. Najlepiej, aby pobrany był model Llama 3 (komenda w terminalu `ollama run llama3.1`).
2. Główny plik `api_key.txt` służący do obsługi Google Gemini otworzy się / wygeneruje automatycznie w folderze.
3. Jeśli kompilujesz program, użyj środowiska Qt Creator z najnowszym zestawem narzędzi Desktop Qt 6.10+ MinGW.

URUCHAMIANIE TESTÓW JEDNOSTKOWYCH
-------------------------------------------------
W ramach projektu zaimplementowano testy jednostkowe oparte o wbudowaną bibliotekę QtTest.
Aby uruchomić testy, skompiluj oddzielny cel (target) z pliku CMakeLists o nazwie `asystent_tests` lub uruchom go z terminala po skompilowaniu używając komendy:
.\build\asystent_tests.exe
Moduł ten potwierdzi pełną stabilność silnika podatkowego oraz bibliotek formatowania tekstu w aplikacji.
