#include "tabcharts.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "llmclient.h"
#include "cryptoexchange.h"
#include <QtConcurrent>
#include <QDateTime>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <regex>

namespace {
    long long dateToMs(QDate date) {
        return QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch();
    }
}

TabCharts::TabCharts(Ui::MainWindow *ui_ptr, MainWindow *parent)
    : QObject(parent), ui(ui_ptr), mainWindow(parent)
{
    connect(ui->btnGenerujWykres, &QPushButton::clicked, this, &TabCharts::generateChart);
    connect(ui->checkDoTeraz, &QCheckBox::toggled, this, &TabCharts::toggleDoTeraz);
    connect(ui->btnInterwal24h, &QPushButton::clicked, this, &TabCharts::setInterwal24h);
    connect(ui->btnInterwal7d, &QPushButton::clicked, this, &TabCharts::setInterwal7d);
    connect(ui->btnInterwal1m, &QPushButton::clicked, this, &TabCharts::setInterwal1m);
}

void TabCharts::generateChart()
{
    using namespace std;

    QString wpisanaWaluta = ui->inputWaluta->text();
    QString typWykQString = ui->comboTypWykresu->currentText();
    string typWyk = typWykQString.toStdString();

    if (wpisanaWaluta.isEmpty()) return;

    bool uzyjGemini = ui->checkUzyjGeminiWykresy->isChecked();
    bool trybMcap = ui->checkKapitalizacja->isChecked();

    long long startMs = dateToMs(ui->dateOd->date());
    long long endMs = ui->checkDoTeraz->isChecked() ? QDateTime::currentMSecsSinceEpoch() : QDateTime(ui->dateDo->date(), QTime(23, 59, 59)).toMSecsSinceEpoch();

    ui->btnGenerujWykres->setEnabled(false);

    LLMClient llmInfo;
    QString nazwaModelu = uzyjGemini ? ("Google (" + QString::fromStdString(llmInfo.getGeminiModelName()) + ")") : ("Agent Ollama (" + QString::fromStdString(llmInfo.getLlamaModelName()) + ")");
    ui->textAnalizaWykresu->setPlainText("KROK 1: Model " + nazwaModelu + " rozszyfrowuje Twoje zapytanie...");

    (void)QtConcurrent::run([this, wpisanaWaluta, startMs, endMs, typWyk, uzyjGemini, nazwaModelu, trybMcap]() {
        try {
            using namespace std;

            // KROK 1
            string walutaStd = wpisanaWaluta.toStdString();
            string promptKrok1 = "Zadanie: podaj oficjalny symbol giełdowy Binance dla poniższej nazwy powiązany ZAWSZE z USDT. "
                                 "ZABRANIAM używać par z PLN, ponieważ większość krypto (jak np. TRUMP, SHIB) ich nie posiada! Zawsze użyj USDT. "
                                 "Ogranicz się TYLKO i WYŁĄCZNIE do kodu w nawiasach kwadratowych. "
                                 "Przykład dla 'Ethereum' -> [ETHUSDT], dla 'TRUMP' -> [TRUMPUSDT]. "
                                 "Nazwa: " + walutaStd;

            LLMClient llm;
            string surowySzukanie = uzyjGemini ? llm.askGemini(promptKrok1) : llm.askLlama(promptKrok1, 0.0);
            string wykrytySymbol = "";
            
            size_t p1 = surowySzukanie.find('[');
            size_t p2 = surowySzukanie.find(']');
            if (p1 != string::npos && p2 != string::npos && p2 > p1) {
                string srodek = surowySzukanie.substr(p1 + 1, p2 - p1 - 1);
                for (char c : srodek) if (isalnum(c)) wykrytySymbol += toupper(c);
            } else {
                for (char c : surowySzukanie) if (isalnum(c)) wykrytySymbol += toupper(c);
            }

            // Fallback (mechanizm awaryjny odporny na bunty Llama)
            if (wykrytySymbol.find("MOGE") != string::npos || wykrytySymbol.find("CANNOT") != string::npos || wykrytySymbol.length() > 14) {
                if (walutaStd.find(' ') != string::npos) {
                    throw std::runtime_error("AI zablokowało przetworzenie całego zdania. Proszę wpisywać tu samo hasło skrótowe, np. 'TRUMP' lub 'TRUMPUSDT'.");
                } else {
                    wykrytySymbol = "";
                    for (char c : walutaStd) if (isalnum(c)) wykrytySymbol += toupper(c);
                    if (!wykrytySymbol.empty() && wykrytySymbol.find("USDT") == string::npos && wykrytySymbol.find("PLN") == string::npos) {
                        wykrytySymbol += "USDT";
                    }
                }
            }

            if (wykrytySymbol.empty() || wykrytySymbol.find("ERROR") != string::npos) {
                throw std::runtime_error("AI nie odnalazło dopasowanego symbolu giełdowego do tej nazwy.");
            }

            // POBIERANIE LICZBY MONET
            double supply = 1.0;
            if (trybMcap) {
                QMetaObject::invokeMethod(mainWindow, [this]() {
                    ui->textAnalizaWykresu->setPlainText("Sprawdzam liczbę monet w obiegu (API CoinGecko)...");
                });
                
                CryptoExchange exchange;
                supply = exchange.fetchSupply(wykrytySymbol);
                
                // FALLBACK NA LIMITY API: Jeśli CoinGecko zablokował IP (Error 429) lub nie znalazł monety:
                if (supply <= 0) {
                    string s = wykrytySymbol;
                    if (s.find("BTC") != string::npos) supply = 19700000;
                    else if (s.find("ETH") != string::npos) supply = 120000000;
                    else if (s.find("SOL") != string::npos) supply = 460000000;
                    else if (s.find("DOGE") != string::npos) supply = 144000000000.0;
                    else if (s.find("XRP") != string::npos) supply = 55000000000.0;
                    else if (s.find("ADA") != string::npos) supply = 35000000000.0;
                    else if (s.find("BNB") != string::npos) supply = 153000000;
                    else if (s.find("TRX") != string::npos) supply = 86000000000.0;
                    else if (s.find("SHIB") != string::npos) supply = 589000000000000.0;
                    else {
                        throw std::runtime_error("Zewnętrzne API odrzuciło lub nie odnalazło podaży. Wróć do standardowych wykresów Ceny dla waluty: " + wykrytySymbol);
                    }
                }
            }

            // KROK 2: DANE
            QMetaObject::invokeMethod(mainWindow, [this, wykrytySymbol]() {
                ui->textAnalizaWykresu->setPlainText("KROK 2: Szukam danych giełdowych dla: " + QString::fromStdString(wykrytySymbol) + "...");
            });

            long long roznicaCzasu = endMs - startMs;
            string interwal = (roznicaCzasu <= 86400000LL * 2) ? "1h" : "1d";

            CryptoExchange exchange;
            bool sukcesGieldy = exchange.fetchCandles(wykrytySymbol, interwal, startMs, endMs, "hist.txt");

            if (!sukcesGieldy) throw std::runtime_error("Brak pary " + wykrytySymbol + " na giełdach.");

            // KROK 3: ANALIZA TRENDU
            QMetaObject::invokeMethod(mainWindow, [this, nazwaModelu]() {
                ui->textAnalizaWykresu->setPlainText("KROK 3: " + nazwaModelu + " analizuje pobrane dane...");
            });

            string promptTrend = "Jesteś profesjonalnym analitykiem technicznym na rynkach finansowych z wieloletnim stażem. "
                                 "Przeanalizuj szeroki trend giełdowy dla aktywa " + wykrytySymbol + ". "
                                 "Napisz wyczerpujący, profesjonalny raport (podzielony na logiczne akapity). Użyj specjalistycznego "
                                 "żargonu inwestycyjnego (wsparcie, opór, kapitał, wolumen, byki, niedźwiedzie). "
                                 "Musi to być rozbudowana odpowiedź, co najmniej 5-6 zdań wniosków. Odpowiadaj wyłącznie po polsku.";
            string komentarzAI = uzyjGemini ? llm.askGemini(promptTrend) : llm.askLlama(promptTrend, 0.7, 512);

            // KROK 4: RYSOWANIE WYKRESU (PYTHON)
            QMetaObject::invokeMethod(mainWindow, [this, wykrytySymbol, typWyk, komentarzAI, nazwaModelu, supply, trybMcap]() {
                QProcess py;
                QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                QString path = env.value("PATH");
                QStringList paths = path.split(";");
                QStringList cleanPath;
                for(const QString& p : paths) {
                    if(!p.contains("Qt", Qt::CaseInsensitive) && !p.contains("mingw", Qt::CaseInsensitive)) {
                        cleanPath << p;
                    }
                }
                env.insert("PATH", cleanPath.join(";"));
                py.setProcessEnvironment(env);
                
                py.start("python", QStringList() << "generate_chart.py" << QString::fromStdString(wykrytySymbol) << QString::fromStdString(typWyk) << QString::number(supply, 'f', 0) << (trybMcap ? "MCAP" : "PRICE"));
                py.waitForFinished();

                QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/wykres.html"));

                ui->textAnalizaWykresu->setPlainText("ANALIZA " + nazwaModelu + " DLA " + QString::fromStdString(wykrytySymbol) + ":\n\n" + QString::fromStdString(komentarzAI));
                ui->btnGenerujWykres->setEnabled(true);
            });

        } catch (const std::exception& e) {
            QString m = QString::fromStdString(e.what());
            QMetaObject::invokeMethod(mainWindow, [this, m]() {
                ui->textAnalizaWykresu->setPlainText("BŁĄD: " + m);
                ui->btnGenerujWykres->setEnabled(true);
            });
        }
    });
}

void TabCharts::toggleDoTeraz(bool checked) { 
    ui->dateDo->setEnabled(!checked); 
}

void TabCharts::setInterwal24h() { 
    ui->checkDoTeraz->setChecked(false); 
    ui->dateDo->setDate(ui->dateOd->date()); 
    generateChart(); 
}

void TabCharts::setInterwal7d() { 
    ui->checkDoTeraz->setChecked(false); 
    ui->dateDo->setDate(ui->dateOd->date().addDays(6)); 
    generateChart(); 
}

void TabCharts::setInterwal1m() { 
    ui->checkDoTeraz->setChecked(false); 
    ui->dateDo->setDate(ui->dateOd->date().addMonths(1).addDays(-1)); 
    generateChart(); 
}
