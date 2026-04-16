#include "tabanalysis.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "llmclient.h"
#include "cryptoexchange.h"
#include <QtConcurrent>
#include <stdexcept>

TabAnalysis::TabAnalysis(Ui::MainWindow *ui_ptr, MainWindow *parent)
    : QObject(parent), ui(ui_ptr), mainWindow(parent)
{
    connect(ui->btnZaawansowanaAnaliza, &QPushButton::clicked, this, &TabAnalysis::performDetailedAnalysis);
}

void TabAnalysis::performDetailedAnalysis()
{
    using namespace std;
    QString wpisanaWaluta = ui->inputWalutaAnaliza->text();
    if (wpisanaWaluta.isEmpty()) return;

    bool uzyjGemini = ui->checkUzyjGeminiAnaliza->isChecked();
    ui->btnZaawansowanaAnaliza->setEnabled(false);
    ui->textWynikAnalizy->setPlainText("KROK 1: Rozpoznawanie symbolu...");

    (void)QtConcurrent::run([this, wpisanaWaluta, uzyjGemini]() {
        try {
            using namespace std;

            // --- KROK 1: Parser symbolu ---
            string walutaStd = wpisanaWaluta.toStdString();
            string promptSymbol = "Zadanie: podaj oficjalny symbol giełdowy Binance dla poniższej nazwy. Zawsze łącz go z USDT. "
                                  "Ogranicz się TYLKO i WYŁĄCZNIE do kodu. BEZ zapytań, BEZ interpunkcji, BEZ zwrotów grzecznościowych. "
                                  "Przykład dla 'Ethereum' -> ETHUSDT. Przykład dla 'solana' -> SOLUSDT. "
                                  "Nazwa: " + walutaStd;
            
            LLMClient llm;
            string surowy = uzyjGemini ? llm.askGemini(promptSymbol) : llm.askLlama(promptSymbol, 0.0);
            string symbol = "";
            
            size_t p1 = surowy.find('[');
            size_t p2 = surowy.find(']');
            if (p1 != string::npos && p2 != string::npos && p2 > p1) {
                string srodek = surowy.substr(p1 + 1, p2 - p1 - 1);
                for (char c : srodek) if (isalnum(c)) symbol += toupper(c);
            } else {
                for (char c : surowy) if (isalnum(c)) symbol += toupper(c);
            }

            // Fallback (mechanizm awaryjny) - gdy Llama odpowie rozwlekle "Nie mogę sprostać..."
            if (symbol.find("MOGE") != string::npos || symbol.find("CANNOT") != string::npos || symbol.length() > 14) {
                if (walutaStd.find(' ') != string::npos) {
                    // Użytkownik wpisał całe zdanie (np. "Jak wygląda sytuacja na TRUMP"), a Llama zablokowała odpowiedź.
                    throw runtime_error("Model AI zablokował analizę całego zdania. W to pole wpisuj ZAWSZE tylko sam skrót krypto (np. TRUMP lub TRUMPUSDT), a nie pytania!");
                } else {
                    symbol = "";
                    for (char c : walutaStd) if (isalnum(c)) symbol += toupper(c);
                    if (!symbol.empty() && symbol.find("USDT") == string::npos && symbol.find("PLN") == string::npos) {
                        symbol += "USDT";
                    }
                }
            }

            // Zabezpieczenie przed ostatecznym błędem w generowaniu symbolu
            if (symbol.empty() || symbol.find("ERROR") != string::npos) {
                throw runtime_error("Nie udało się rozpoznać waluty. Wpisz po prostu krótki skrót, np. 'BTC', 'SOL' lub 'TRUMP'.");
            }

            // --- KROK 2: Pobieranie zaawansowanych danych ---
            QMetaObject::invokeMethod(mainWindow, [this, symbol]() {
                ui->textWynikAnalizy->setPlainText("KROK 2: Pobieranie rozszerzonych wskaźników dla " + QString::fromStdString(symbol) + "...");
            });

            CryptoExchange exchange;
            auto stats = exchange.fetchMarketData24h(symbol);

            if (!stats.success) {
                throw runtime_error("Brak danych wskaźnikowych dla: " + symbol);
            }

            string cena = stats.price;
            string zmiana = stats.changePercent;
            string volUSD = stats.quoteVolume;
            string high = stats.highPrice;
            string low = stats.lowPrice;

            // --- KROK 3: Prawdziwa analiza AI ---
            string mNameAnaliza = uzyjGemini ? ("Google (" + llm.getGeminiModelName() + ")") : ("Agent Ollama (" + llm.getLlamaModelName() + ")");
            QMetaObject::invokeMethod(mainWindow, [this, mNameAnaliza]() {
                ui->textWynikAnalizy->setPlainText(QString::fromStdString("KROK 3: Model " + mNameAnaliza + " przygotowuje szczegółowy raport rynkowy..."));
            });

            string promptAnaliza = "Wciel się w rolę profesjonalnego analityka z Wall Street. Masz dane z ostatnich 24h dla " + symbol + ":\n"
                                   "- Cena obecna: " + cena + " USD\n"
                                   "- Zmiana: " + zmiana + "%\n"
                                   "- Obrót kapitału: " + volUSD + " USD\n"
                                   "- Zakres cenowy (Dołek-Szczyt): " + low + " - " + high + " USD\n\n"
                                   "Napisz WYCZERPUJĄCĄ analizę (minimum 100 słów, około 3-4 akapity). "
                                   "Zinterpretuj co oznacza taki obrót kapitału przy tej zmianie ceny. Oceń rozstrzał cenowy (zmienność) "
                                   "i wypisz możliwe scenariusze na najbliższe dni. Używaj poprawnego języka polskiego i żargonu giełdowego.";

            string raportAI = "Błąd generowania.";

            if (uzyjGemini) {
                raportAI = llm.askGemini(promptAnaliza);
            } else {
                raportAI = llm.askLlama(promptAnaliza, 0.7, 1024);
            }

            // --- KROK 4: Eleganckie wyświetlenie UI ---
            QMetaObject::invokeMethod(mainWindow, [this, symbol, cena, zmiana, volUSD, low, high, raportAI]() {
                QString raport = QString(
                                     "📊 RAPORT RYNKOWY: %1 📊\n"
                                     "=======================================\n"
                                     "💵 Aktualna Cena   : %2 USD\n"
                                     "📈 Zmiana (24h)    : %3 %\n"
                                     "💰 Obrót w USD     : %4 USD\n"
                                     "📉 Najniższa cena  : %5 USD\n"
                                     "🚀 Najwyższa cena  : %6 USD\n"
                                     "=======================================\n\n"
                                     "🧠 ANALIZA EKSPERTA (AI):\n\n"
                                     "%7"
                                     ).arg(
                                         QString::fromStdString(symbol),
                                         QString::fromStdString(cena),
                                         QString::fromStdString(zmiana),
                                         QString::fromStdString(volUSD),
                                         QString::fromStdString(low),
                                         QString::fromStdString(high),
                                         QString::fromStdString(raportAI)
                                         );

                ui->textWynikAnalizy->setPlainText(raport);
                ui->btnZaawansowanaAnaliza->setEnabled(true);
            });

        } catch (const exception& e) {
            QString err = QString::fromStdString(e.what());
            QMetaObject::invokeMethod(mainWindow, [this, err]() {
                ui->textWynikAnalizy->setPlainText("BŁĄD: " + err);
                ui->btnZaawansowanaAnaliza->setEnabled(true);
            });
        }
    });
}
