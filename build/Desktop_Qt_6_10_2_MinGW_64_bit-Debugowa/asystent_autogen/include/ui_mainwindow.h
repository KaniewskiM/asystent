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
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_3;
    QLabel *label;
    QLineEdit *inputSystemPrompt;
    QPushButton *btnAnalizuj;
    QLabel *label_2;
    QLabel *lblCena;
    QTextEdit *textWynikAI;
    QWidget *tab_2;
    QPushButton *btnSzukajNewsow;
    QPlainTextEdit *textNewsy;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(797, 527);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 0, 831, 501));
        tab = new QWidget();
        tab->setObjectName("tab");
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(280, -10, 261, 91));
        QFont font;
        font.setFamilies({QString::fromUtf8("SimSun-ExtB")});
        font.setPointSize(16);
        label_3->setFont(font);
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 60, 81, 20));
        inputSystemPrompt = new QLineEdit(tab);
        inputSystemPrompt->setObjectName("inputSystemPrompt");
        inputSystemPrompt->setGeometry(QRect(40, 90, 391, 81));
        btnAnalizuj = new QPushButton(tab);
        btnAnalizuj->setObjectName("btnAnalizuj");
        btnAnalizuj->setGeometry(QRect(540, 140, 90, 29));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("SimSun-ExtG")});
        font1.setPointSize(12);
        btnAnalizuj->setFont(font1);
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(490, 90, 101, 20));
        lblCena = new QLabel(tab);
        lblCena->setObjectName("lblCena");
        lblCena->setGeometry(QRect(620, 90, 91, 20));
        textWynikAI = new QTextEdit(tab);
        textWynikAI->setObjectName("textWynikAI");
        textWynikAI->setGeometry(QRect(30, 200, 731, 231));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        btnSzukajNewsow = new QPushButton(tab_2);
        btnSzukajNewsow->setObjectName("btnSzukajNewsow");
        btnSzukajNewsow->setGeometry(QRect(240, 230, 90, 29));
        textNewsy = new QPlainTextEdit(tab_2);
        textNewsy->setObjectName("textNewsy");
        textNewsy->setGeometry(QRect(90, 50, 421, 161));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 797, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Krypto Ai Asystent", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Kontekst AI", nullptr));
        btnAnalizuj->setText(QCoreApplication::translate("MainWindow", "Analizuj", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Aktualna Cena", nullptr));
        lblCena->setText(QCoreApplication::translate("MainWindow", "Cyferki", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        btnSzukajNewsow->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
