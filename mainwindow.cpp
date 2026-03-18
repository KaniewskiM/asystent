#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QtConcurrent> // Biblioteka do wielowatkowosci

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblCena->setText("BTC: Oczekiwanie...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- ZAKŁADKA 1: ASYSTENT (Analiza własna) ---
void MainWindow::on_btnAnalizuj_clicked()
{
    QString qPrompt = ui->inputSystemPrompt->text();

    if (qPrompt.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wpisać instrukcję dla AI!");
        return;
    }

    ui->btnAnalizuj->setEnabled(false);
    ui->textWynikAI->setPlainText("Asystent (Llama 3.2 3B) przygotowuje odpowiedź...");

    (void)QtConcurrent::run([this, qPrompt]() {
        string cenaBTC = "Brak danych";
        string komentarzAI = "Błąd połączenia z modelem.";

        // Czyszczenie starych plików przed startem
        std::remove("btc_cena.txt");
        std::remove("zapytanie_ai.json");
        std::remove("odpowiedz_ai.txt");

        try {
            system("curl -s \"https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT\" > btc_cena.txt");
            ifstream pCena("btc_cena.txt");
            if (pCena.is_open()) {
                string s; getline(pCena, s);
                size_t pos = s.find("\"price\":\"");
                if (pos != string::npos) {
                    cenaBTC = s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9));
                }
                pCena.close();
            }

            string pelnyPrompt = "Cena BTC to " + cenaBTC + " USD. " + qPrompt.toStdString();
            ofstream pj("zapytanie_ai.json");
            pj << "{\"model\":\"llama3.2\",\"prompt\":\"" << pelnyPrompt << "\",\"stream\":false}";
            pj.close();

            system("curl -s -X POST \"http://127.0.0.1:11434/api/generate\" -d @zapytanie_ai.json > odpowiedz_ai.txt");

            ifstream po("odpowiedz_ai.txt");
            if (po.is_open()) {
                string s; getline(po, s);
                size_t start = s.find("\"response\":\"");
                if (start != string::npos) {
                    komentarzAI = s.substr(start + 12, s.find("\",\"done\"", start + 12) - (start + 12));
                }
                po.close();
            }
        } catch (...) {
            komentarzAI = "Błąd krytyczny wątku.";
        }

        QMetaObject::invokeMethod(this, [this, cenaBTC, komentarzAI]() {
            ui->lblCena->setText("BTC: " + QString::fromStdString(cenaBTC) + " USD");
            ui->textWynikAI->setPlainText(QString::fromStdString(komentarzAI));
            ui->btnAnalizuj->setEnabled(true);
        });
    });
}

// --- ZAKŁADKA 2: NEWSY (Generowanie automatyczne) ---
void MainWindow::on_btnSzukajNewsow_clicked()
{
    ui->btnSzukajNewsow->setEnabled(false);
    ui->textNewsy->setPlainText("Generuję najświeższe newsy rynkowe (Llama 3.2 3B)...");

    (void)QtConcurrent::run([this]() {
        string cenaBTC = "Brak danych";
        string newsyAI = "Błąd podczas generowania wiadomości.";

        // Czyszczenie plików newsów
        std::remove("news_price.txt");
        std::remove("news_request.json");
        std::remove("news_output.txt");

        try {
            system("curl -s \"https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT\" > news_price.txt");
            ifstream pCena("news_price.txt");
            if (pCena.is_open()) {
                string s; getline(pCena, s);
                size_t pos = s.find("\"price\":\"");
                if (pos != string::npos) {
                    cenaBTC = s.substr(pos + 9, s.find("\"", pos + 9) - (pos + 9));
                }
                pCena.close();
            }

            string automatycznyPrompt = "Jesteś redaktorem serwisu finansowego. Cena Bitcoina wynosi teraz " + cenaBTC +
                                        " USD. Na tej podstawie wygeneruj 3 krótkie newsy ze świata krypto w formie listy. " +
                                        "Pisz po polsku, profesjonalnie.";

            ofstream pj("news_request.json");
            pj << "{\"model\":\"llama3.2\",\"prompt\":\"" << automatycznyPrompt << "\",\"stream\":false}";
            pj.close();

            system("curl -s -X POST \"http://127.0.0.1:11434/api/generate\" -d @news_request.json > news_output.txt");

            ifstream po("news_output.txt");
            if (po.is_open()) {
                string s; getline(po, s);
                size_t start = s.find("\"response\":\"");
                if (start != string::npos) {
                    newsyAI = s.substr(start + 12, s.find("\",\"done\"", start + 12) - (start + 12));
                }
                po.close();
            }
        } catch (...) {
            newsyAI = "Błąd generowania newsów.";
        }

        QMetaObject::invokeMethod(this, [this, newsyAI]() {
            ui->textNewsy->setPlainText(QString::fromStdString(newsyAI));
            ui->btnSzukajNewsow->setEnabled(true);
        });
    });
}