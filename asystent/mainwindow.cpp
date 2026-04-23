#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "llmclient.h"
#include <QProcess>
#include <fstream>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Domyślny stan kontrolek interfejsu wizualnego
    ui->lblCena->setText("Czekam na zapytanie...");
    ui->progressBar->setVisible(false);
    ui->dateDo->setDate(QDate::currentDate());
    ui->dateOd->setDate(QDate::currentDate().addMonths(-1));
    ui->dateDo->setEnabled(false);
    
    // Inicjalizacja profesjonalnych kontrolerów dla poszczególnych zakładek (MVC)
    tabAssistant = new TabAssistant(ui, this);
    tabNews = new TabNews(ui, this);
    tabCharts = new TabCharts(ui, this);
    tabTaxes = new TabTaxes(ui, this);
    tabAnalysis = new TabAnalysis(ui, this);
    
    // Podłączenie nowej zakładki Ustawienia
    connect(ui->btnOdswiezModele, &QPushButton::clicked, this, &MainWindow::odswiezModeleOllama);
    connect(ui->btnZapiszUstawienia, &QPushButton::clicked, this, &MainWindow::zapiszUstawieniaAI);
    
    odswiezModeleOllama();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::odswiezModeleOllama() {
    ui->comboLlamaModel->clear();
    
    QProcess process;
    process.start("ollama", QStringList() << "list");
    process.waitForFinished(2000); // Czekamy max 2 sekundy na odpowiedź od Ollamy
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    bool firstLine = true;
    for (const QString& line : lines) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (!parts.isEmpty()) {
            ui->comboLlamaModel->addItem(parts[0]);
        }
    }
    
    // Odczytaj aktualny model i zaznacz go na liście
    LLMClient llm;
    QString currentModel = QString::fromStdString(llm.getLlamaModelName());
    
    int index = ui->comboLlamaModel->findText(currentModel);
    if (index != -1) {
        ui->comboLlamaModel->setCurrentIndex(index);
    } else if (!currentModel.isEmpty()) {
        ui->comboLlamaModel->addItem(currentModel);
        ui->comboLlamaModel->setCurrentIndex(ui->comboLlamaModel->count() - 1);
    }
    
    // Załaduj aktualny klucz API
    ui->inputGeminiKey->setText(QString::fromStdString(llm.getApiKey()));
    
    ui->lblUstawieniaStatus->setText("Lista modeli załadowana.");
}

void MainWindow::zapiszUstawieniaAI() {
    QString selectedModel = ui->comboLlamaModel->currentText();
    QString geminiKey = ui->inputGeminiKey->text().trimmed();
    
    if (selectedModel.isEmpty()) {
        ui->lblUstawieniaStatus->setText("Błąd: Wybierz model!");
        return;
    }
    
    bool zapisOllama = false;
    std::ofstream out("llama_model.txt");
    if (out.is_open()) {
        out << selectedModel.toStdString();
        out.close();
        zapisOllama = true;
    }
    
    bool zapisGemini = false;
    if (!geminiKey.isEmpty()) {
        std::ofstream outKey("api_key.txt");
        if (outKey.is_open()) {
            outKey << geminiKey.toStdString();
            outKey.close();
            zapisGemini = true;
        }
    } else {
        zapisGemini = true; // Zignoruj jeśli puste
    }
    
    if (zapisOllama && zapisGemini) {
        ui->lblUstawieniaStatus->setText("Pomyślnie zapisano ustawienia AI!");
    } else {
        ui->lblUstawieniaStatus->setText("Błąd: Nie można zapisać ustawień do pliku!");
    }
}