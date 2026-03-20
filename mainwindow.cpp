#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QtConcurrent>
#include <iomanip> // Potrzebne do formatowania liczb
#include <QDesktopServices>
#include <QUrl>

using namespace std;

// Pomocnicza funkcja do usuwania "śmieci" z formatu JSON (\n, \u0026 itp.)
string wyczyscJson(string tekst) {
    size_t p;
    while ((p = tekst.find("\\n")) != string::npos) tekst.replace(p, 2, "\n");
    while ((p = tekst.find("\\\"")) != string::npos) tekst.replace(p, 2, "\"");
    while ((p = tekst.find("\\u0026")) != string::npos) tekst.replace(p, 6, "&");
    return tekst;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Ustawienia początkowe
    ui->lblCena->setText("BTC: czekam...");
    ui->lblCenaPLN->setText("PLN: czekam...");
    ui->lblZloto->setText("Złoto: czekam...");
    ui->progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_btnPokazWykres_clicked()
{
    ui->btnPokazWykres->setEnabled(false);
    ui->btnPokazWykres->setText("Generowanie...");
    QString obecnaCena = ui->lblCena->text();
    if (obecnaCena.contains("...") || obecnaCena.contains("czekam")) {
        QMessageBox::information(this, "Informacja", "Najpierw kliknij 'Analizuj', aby pobrać aktualne dane!");
        return;
    }

    ui->btnPokazWykres->setEnabled(false);
    ui->btnPokazWykres->setText("Generowanie...");
    string cenaStr = ui->lblCena->text().toStdString();
    string tylkoCyfry = "";
    for(char c : cenaStr) if(isdigit(c) || c == '.' || c == ',') tylkoCyfry += (c == ',' ? '.' : c);

    // Pobieramy absolutną ścieżkę do folderu z naszym programem .exe
    QString sciezkaAplikacji = QCoreApplication::applicationDirPath();
    QString pelnaSciezkaHtml = sciezkaAplikacji + "/wykres.html";

    (void)QtConcurrent::run([this, tylkoCyfry, pelnaSciezkaHtml]() {
        // Wywołujemy Pythona
        string cmd = "python generate_chart.py " + tylkoCyfry;
        system(cmd.c_str());

        QMetaObject::invokeMethod(this, [this, pelnaSciezkaHtml]() {
            // Sprawdzamy, czy plik fizycznie istnieje na dysku
            if (QFile::exists(pelnaSciezkaHtml)) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(pelnaSciezkaHtml));
            } else {
                QMessageBox::warning(this, "Błąd", "Nie znaleziono pliku wykresu.\nUpewnij się, że plik 'generate_chart.py' znajduje się w folderze z programem .exe!");
            }

            ui->btnPokazWykres->setEnabled(true);
            ui->btnPokazWykres->setText("Otwórz wykres");
        });
    });
}
// --- ZAKŁADKA 1: ASYSTENT AI ---
void MainWindow::on_btnAnalizuj_clicked()
{
    QString qPrompt = ui->inputSystemPrompt->text();
    if (qPrompt.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Wpisz pytanie do asystenta!");
        return;
    }

    ui->btnAnalizuj->setEnabled(false);
    ui->progressBar->setVisible(true);
    ui->textWynikAI->setPlainText("Analizuję rynki (Llama 3.2 3B)...");

    (void)QtConcurrent::run([this, qPrompt]() {
        double btcUsd = 0.0;
        double zlotoUsd = 0.0;
        double kursPLN = 4.02; // Stały kurs dolara do przeliczeń

        // 1. Pobieranie ceny Bitcoina
        system("curl -s \"https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT\" > btc.txt");
        ifstream f1("btc.txt");
        if (f1.is_open()) {
            string s; getline(f1, s);
            size_t pos = s.find("\"price\":\"");
            if (pos != string::npos) btcUsd = stod(s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9)));
            f1.close();
        }

        // 2. Pobieranie ceny Złota (przez token PAXG, który odzwierciedla cenę złota 1:1)
        system("curl -s \"https://api.binance.com/api/v3/ticker/price?symbol=PAXGUSDT\" > gold.txt");
        ifstream f2("gold.txt");
        if (f2.is_open()) {
            string s; getline(f2, s);
            size_t pos = s.find("\"price\":\"");
            if (pos != string::npos) zlotoUsd = stod(s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9)));
            f2.close();
        }

        double btcPln = btcUsd * kursPLN;

        // 3. Budowanie ukrytego Promptu (System Prompt)
        string rola = "Jesteś ekspertem finansowym w roku 2026. "
                      "Obecna cena BTC: " + to_string(btcUsd) + " USD, Złoto: " + to_string(zlotoUsd) + " USD. "
                                                                                    "Odpowiadaj po polsku, merytorycznie i krótko. ";

        string pelnyPrompt = rola + "Pytanie użytkownika: " + qPrompt.toStdString();

        ofstream pj("zapytanie.json");
        pj << "{\"model\":\"llama3.2\",\"prompt\":\"" << pelnyPrompt << "\",\"stream\":false}";
        pj.close();

        // 4. Rozmowa z Olamą
        system("curl -s -X POST \"http://127.0.0.1:11434/api/generate\" -d @zapytanie.json > odpowiedz.txt");

        string wynikAI = "Błąd połączenia z modelem.";
        ifstream po("odpowiedz.txt");
        if (po.is_open()) {
            string s; getline(po, s);
            size_t start = s.find("\"response\":\"");
            if (start != string::npos) {
                wynikAI = wyczyscJson(s.substr(start + 12, s.find("\",\"done\"", start + 12) - (start + 12)));
            }
            po.close();
        }

        // 5. Aktualizacja UI (formatowanie do 2 miejsc po przecinku)
        QMetaObject::invokeMethod(this, [this, btcUsd, btcPln, zlotoUsd, wynikAI]() {
            ui->lblCena->setText("BTC: " + QString::number(btcUsd, 'f', 2) + " $");
            ui->lblCenaPLN->setText("PLN: " + QString::number(btcPln, 'f', 2) + " zł");
            ui->lblZloto->setText("Złoto: " + QString::number(zlotoUsd, 'f', 2) + " $");
            ui->textWynikAI->setPlainText(QString::fromStdString(wynikAI));
            ui->progressBar->setVisible(false);
            ui->btnAnalizuj->setEnabled(true);
        });
    });
}

// --- ZAKŁADKA 2: NEWSY ---
void MainWindow::on_btnSzukajNewsow_clicked()
{
    ui->btnSzukajNewsow->setEnabled(false);
    ui->progressBar->setVisible(true);
    ui->textNewsy->setPlainText("Generuję biuletyn informacyjny na marzec 2026...");

    (void)QtConcurrent::run([this]() {
        string cenaBTC = "Brak";

        // Pobieramy cenę do newsów
        system("curl -s \"https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT\" > n_price.txt");
        ifstream f("n_price.txt");
        if (f.is_open()) {
            string s; getline(f, s);
            size_t pos = s.find("\"price\":\"");
            if (pos != string::npos) cenaBTC = s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9));
            f.close();
        }

        string newsPrompt = "Działaj jako redaktor krypto w roku 2026. Cena BTC to " + cenaBTC +
                            " USD. Stwórz 3 newsy dnia o kryptowalutach w formie listy. "
                            "Pisz po polsku. Nie wspominaj o przeszłości.";

        ofstream pj("n_req.json");
        pj << "{\"model\":\"llama3.2\",\"prompt\":\"" << newsPrompt << "\",\"stream\":false}";
        pj.close();

        system("curl -s -X POST \"http://127.0.0.1:11434/api/generate\" -d @n_req.json > n_out.txt");

        string newsyFinal = "Błąd generowania newsów.";
        ifstream po("n_out.txt");
        if (po.is_open()) {
            string s; getline(po, s);
            size_t start = s.find("\"response\":\"");
            if (start != string::npos) {
                newsyFinal = wyczyscJson(s.substr(start + 12, s.find("\",\"done\"", start + 12) - (start + 12)));
            }
            po.close();
        }

        QMetaObject::invokeMethod(this, [this, newsyFinal]() {
            ui->textNewsy->setPlainText(QString::fromStdString(newsyFinal));
            ui->progressBar->setVisible(false);
            ui->btnSzukajNewsow->setEnabled(true);
        });
    });
}