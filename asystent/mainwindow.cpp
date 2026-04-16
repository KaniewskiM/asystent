#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

MainWindow::~MainWindow() {
    delete ui;
}