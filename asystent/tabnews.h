#ifndef TABNEWS_H
#define TABNEWS_H

#include <QObject>

namespace Ui { class MainWindow; }
class MainWindow;

/**
 * @brief Kontroler zakładki "Wiadomości rynkowe".
 */
class TabNews : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor modułu wiadomości.
     * @param ui Wskaźnik na interfejs UI.
     * @param parent Wskaźnik na główne okno.
     */
    explicit TabNews(Ui::MainWindow *ui, MainWindow *parent = nullptr);

private slots:
    /**
     * @brief Pobiera najświeższe newsy, tłumaczy je i wyświetla biuletyn.
     */
    void generateNews();

private:
    Ui::MainWindow *ui;      ///< Interfejs UI
    MainWindow *mainWindow;  ///< Wskaźnik na główne okno
};

#endif // TABNEWS_H
