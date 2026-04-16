#include "tabnews.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "llmclient.h"
#include "cryptoexchange.h"
#include <QtConcurrent>
#include <QDateTime>

TabNews::TabNews(Ui::MainWindow *ui_ptr, MainWindow *parent)
    : QObject(parent), ui(ui_ptr), mainWindow(parent)
{
    connect(ui->btnSzukajNewsow, &QPushButton::clicked, this, &TabNews::generateNews);
}

void TabNews::generateNews()
{
    using namespace std;

    ui->btnSzukajNewsow->setEnabled(false);
    ui->progressBar->setVisible(true);

    QString wybranyCzas = ui->comboCzasNewsow->currentText();
    bool uzyjGemini = ui->checkUzyjGemini->isChecked();

    QDateTime teraz = QDateTime::currentDateTime();
    QDateTime startCzas = (wybranyCzas == "Ostatnie 24h") ? teraz.addDays(-1) : teraz.addDays(-7);
    QString zakresTekst = "od " + startCzas.toString("dd.MM.yyyy HH:mm") + " do " + teraz.toString("dd.MM.yyyy HH:mm");

    ui->textNewsy->setPlainText("Generowanie biuletynu...");

    (void)QtConcurrent::run([this, zakresTekst, uzyjGemini, wybranyCzas, teraz]() {
        using namespace std;

        CryptoExchange exchange;
        
        QMetaObject::invokeMethod(mainWindow, [this]() {
            ui->textNewsy->setPlainText("Połączenie z portalem CoinTelegraph (Pobieranie RSS)...");
        });
        
        string rssHeadlines = exchange.fetchLatestNews();

        string dzisiejszaData = teraz.toString("dd.MM.yyyy HH:mm").toStdString();
        string zakresTekstStd = zakresTekst.toStdString();

        string newsPrompt = "Jesteś wybitnym redaktorem polskiego portalu kryptowalutowego.\n"
                            "Oto dzisiejsza bieżąca data i godzina: " + dzisiejszaData + "\n\n"
                            "Oto lista najnowszych wiadomości ze świata (zawiera surową datę, tytuł i krótki OPIS):\n" + rssHeadlines + "\n\n"
                            "ZADANIE:\n"
                            "1. Wybierz 5 najciekawszych spraw z okresu: " + wybranyCzas.toStdString() + " (" + zakresTekstStd + ").\n"
                            "2. Przetłumacz angielskie tytuły i OPISY na naturalnie brzmiący język polski.\n"
                            "3. Skróć zagraniczną datę publikacji do polskiego formatu czasu (np. 3 h temu).\n"
                            "4. W raporcie, POD tytułem każdego newsa, koniecznie napisz rozbudowane podsumowanie (2-3 merytoryczne zdania w punktach lub pełnym zdaniem), bazując wyłącznie na jego OPISIE z danych.\n\n"
                            "ZABRANIAM PISANIA WSTĘPÓW, ZAKOŃCZEŃ I ZDAŃ TYPU: 'Oto moja odpowiedź'.\n"
                            "Wygeneruj SAM CZYSTY RZETELNY BIULETYN według poniższego wzoru:\n"
                            "- [Wczoraj o 15:30] **Dobrze przetłumaczony tytuł pierwszego artykułu**\n"
                            "Tutaj znajduje się rozbudowany, przetłumaczony tekst na 2-3 zdania podsumowujący opisaną sprawę z zachowaniem profesjonalnego żargonu z Wall Street.\n\n"
                            "- [2 dni temu] **Kolejny polski tytuł...**\n"
                            "Tłumaczenie dłuższego tekstu z opisu...\n";
        
        LLMClient llm;
        string mName = uzyjGemini ? ("Google (" + llm.getGeminiModelName() + ")") : ("Agent Ollama (" + llm.getLlamaModelName() + ")");

        QMetaObject::invokeMethod(mainWindow, [this, mName]() {
            ui->textNewsy->setPlainText(QString::fromStdString("Model " + mName + " pobiera logi RSS i tłumaczy wiadomości w tle..."));
        });
        
        string newsyFinal = "Błąd generowania newsów.";

        if (uzyjGemini) {
            newsyFinal = llm.askGemini(newsPrompt);
        } else {
            // Zwiększamy liczbę dopuszczalnych tokenów, bo długi opis zajmuje więcej miejsca
            newsyFinal = llm.askLlama(newsPrompt, 0.7, 1024);
        }

        QMetaObject::invokeMethod(mainWindow, [this, newsyFinal, wybranyCzas]() {
            ui->textNewsy->setPlainText("BIULETYN (" + wybranyCzas + ")\n\n" + QString::fromStdString(newsyFinal));
            ui->progressBar->setVisible(false);
            ui->btnSzukajNewsow->setEnabled(true);
        });
    });
}
