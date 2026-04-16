#include "tabassistant.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "llmclient.h"
#include "cryptoexchange.h"
#include <QMessageBox>
#include <QtConcurrent>

TabAssistant::TabAssistant(Ui::MainWindow *ui_ptr, MainWindow *parent)
    : QObject(parent), ui(ui_ptr), mainWindow(parent)
{
    // Dowiązanie logiki kliknięcia
    connect(ui->btnAnalizuj, &QPushButton::clicked, this, &TabAssistant::handleAnalysis);

    // Automatyczne pobieranie cen na start
    ui->lblCena->setText("Pobieram...");
    ui->lblCenaPLN->setText("Pobieram...");
    ui->lblZloto->setText("Pobieram...");

    (void)QtConcurrent::run([this]() {
        CryptoExchange exchange;
        std::string btcPln = exchange.fetchCurrentPrice("BTCPLN");
        std::string btcUsd = exchange.fetchCurrentPrice("BTCUSDT");
        std::string goldUsd = exchange.fetchCurrentPrice("PAXGUSDT"); // Krypto-Złoto (Pax Gold)

        QMetaObject::invokeMethod(mainWindow, [this, btcPln, btcUsd, goldUsd]() {
            if (btcPln != "BLAD") ui->lblCenaPLN->setText(QString::fromStdString(btcPln) + " PLN");
            else ui->lblCenaPLN->setText("Błąd giełdy");

            if (btcUsd != "BLAD") ui->lblCena->setText(QString::fromStdString(btcUsd) + " USD");
            else ui->lblCena->setText("Błąd giełdy");

            if (goldUsd != "BLAD") ui->lblZloto->setText(QString::fromStdString(goldUsd) + " USD");
            else ui->lblZloto->setText("Błąd giełdy");
        });
    });
}

void TabAssistant::handleAnalysis()
{
    using namespace std;

    QString qPrompt = ui->inputSystemPrompt->text();
    if (qPrompt.isEmpty()) {
        QMessageBox::warning(mainWindow, "Błąd", "Wpisz pytanie do AI!");
        return;
    }

    bool uzyjGemini = ui->checkUzyjGeminiAsystent->isChecked();
    ui->btnAnalizuj->setEnabled(false);
    ui->progressBar->setVisible(true);

    ui->textWynikAI->setPlainText("KROK 1: Analiza zapytania...");

    (void)QtConcurrent::run([this, qPrompt, uzyjGemini]() {
        try {
            using namespace std;

            // =========================================================
            // ZWYKŁA KONWERSACJA (BEZ POBIERANIA CENY Z GIEŁDY)
            // =========================================================
            LLMClient llm;
            string mAktualny = uzyjGemini ? ("Google (" + llm.getGeminiModelName() + ")") : ("Lokalna Sztuczna Inteligencja (" + llm.getLlamaModelName() + ")");

            QMetaObject::invokeMethod(mainWindow, [this, mAktualny]() {
                ui->textWynikAI->setPlainText(QString::fromStdString("Model " + mAktualny + " analizuje bazę wiedzy i redaguje dla Ciebie odpowiedź..."));
            });

            string promptKoncowy = "Jesteś przyjaznym edukatorem z dziedziny kryptowalut, technologii blockchain i rynków finansowych. "
                                   "Mówisz prostym, zrozumiałym dla początkujących językiem po polsku. "
                                   "Odpowiedz na poniższe pytanie bazując na swojej wiedzy. "
                                   "Pytanie: " + qPrompt.toStdString();

            string odpowiedzKoncowa = "Błąd generowania.";

            if (uzyjGemini) {
                odpowiedzKoncowa = llm.askGemini(promptKoncowy);
            } else {
                odpowiedzKoncowa = llm.askLlama(promptKoncowy, 0.6); // Zwiększyłem temperaturę dla lepszych opisów
            }

            QMetaObject::invokeMethod(mainWindow, [this, odpowiedzKoncowa]() {
                ui->textWynikAI->setPlainText("--- TŁUMACZENIE EKSPERTA ---\n\n" + QString::fromStdString(odpowiedzKoncowa));
                ui->btnAnalizuj->setEnabled(true);
                ui->progressBar->setVisible(false);
            });

        } catch (const std::exception& e) {
            QString errorMsg = QString::fromStdString(e.what());
            QMetaObject::invokeMethod(mainWindow, [this, errorMsg]() {
                ui->textWynikAI->setPlainText("Wystąpił błąd C++:\n" + errorMsg);

                QMessageBox::critical(mainWindow, "Krytyczny Błąd", "Aplikacja natrafiła na problem:\n" + errorMsg);

                ui->btnAnalizuj->setEnabled(true);
                ui->progressBar->setVisible(false);
            });
        }
    });
}
