#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QTcpSocket>

bool checkInternetConnection() {
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53);
    if (socket.waitForConnected(3000)) {
        socket.disconnectFromHost();
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Sprawdzenie dostępności Internetu przed uruchomieniem aplikacji
    if (!checkInternetConnection()) {
        QMessageBox::critical(nullptr, "Brak połączenia z Internetem", 
            "Program Krypto Asystent wymaga aktywnego połączenia z internetem do pobierania danych giełdowych i komunikacji z AI.\n\n"
            "Sprawdź swoje połączenie i uruchom program ponownie.");
        return 0; // Zakończ działanie zanim cokolwiek się załaduje
    }

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
