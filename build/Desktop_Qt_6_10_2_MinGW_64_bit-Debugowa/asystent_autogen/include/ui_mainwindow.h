/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QPushButton *btnPokazWykres;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *lblCenaPLN;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *lblZloto;
    QLabel *label;
    QLabel *lblCena;
    QLabel *label_4;
    QLineEdit *inputSystemPrompt;
    QPushButton *btnAnalizuj;
    QProgressBar *progressBar;
    QTextEdit *textWynikAI;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *textNewsy;
    QPushButton *btnSzukajNewsow;
    QMenuBar *menubar;
    QMenu *menuKrypto_Asystent;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1011, 731);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setAutoFillBackground(false);
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        btnPokazWykres = new QPushButton(tab_3);
        btnPokazWykres->setObjectName("btnPokazWykres");
        btnPokazWykres->setGeometry(QRect(280, 350, 421, 81));
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName("verticalLayout");
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        QFont font;
        font.setFamilies({QString::fromUtf8("SimSun-ExtB")});
        font.setPointSize(16);
        label_3->setFont(font);

        verticalLayout->addWidget(label_3);

        lblCenaPLN = new QLabel(tab);
        lblCenaPLN->setObjectName("lblCenaPLN");

        verticalLayout->addWidget(lblCenaPLN);

        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");

        verticalLayout->addWidget(label_5);

        lblZloto = new QLabel(tab);
        lblZloto->setObjectName("lblZloto");

        verticalLayout->addWidget(lblZloto);

        label = new QLabel(tab);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        lblCena = new QLabel(tab);
        lblCena->setObjectName("lblCena");

        verticalLayout->addWidget(lblCena);

        label_4 = new QLabel(tab);
        label_4->setObjectName("label_4");

        verticalLayout->addWidget(label_4);

        inputSystemPrompt = new QLineEdit(tab);
        inputSystemPrompt->setObjectName("inputSystemPrompt");

        verticalLayout->addWidget(inputSystemPrompt);

        btnAnalizuj = new QPushButton(tab);
        btnAnalizuj->setObjectName("btnAnalizuj");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("SimSun-ExtG")});
        font1.setPointSize(12);
        btnAnalizuj->setFont(font1);

        verticalLayout->addWidget(btnAnalizuj);

        progressBar = new QProgressBar(tab);
        progressBar->setObjectName("progressBar");
        progressBar->setMaximum(0);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        verticalLayout->addWidget(progressBar);

        textWynikAI = new QTextEdit(tab);
        textWynikAI->setObjectName("textWynikAI");

        verticalLayout->addWidget(textWynikAI);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        textNewsy = new QPlainTextEdit(tab_2);
        textNewsy->setObjectName("textNewsy");

        verticalLayout_2->addWidget(textNewsy);

        btnSzukajNewsow = new QPushButton(tab_2);
        btnSzukajNewsow->setObjectName("btnSzukajNewsow");

        verticalLayout_2->addWidget(btnSzukajNewsow);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1011, 25));
        menuKrypto_Asystent = new QMenu(menubar);
        menuKrypto_Asystent->setObjectName("menuKrypto_Asystent");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuKrypto_Asystent->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnPokazWykres->setText(QCoreApplication::translate("MainWindow", "Generuj i otw\303\263rz interaktywny wykres BTC\"", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Wykresy", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Krypto Ai Asystent", nullptr));
        lblCenaPLN->setText(QCoreApplication::translate("MainWindow", "Cyferki", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Cena BTC w PLN", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Cena z\305\202ota w Dolarach", nullptr));
        lblZloto->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Kontekst AI", nullptr));
        lblCena->setText(QCoreApplication::translate("MainWindow", "Cyferki", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Cena BTC w DOL", nullptr));
        btnAnalizuj->setText(QCoreApplication::translate("MainWindow", "Analizuj", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        btnSzukajNewsow->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        menuKrypto_Asystent->setTitle(QCoreApplication::translate("MainWindow", "Krypto Asystent", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
