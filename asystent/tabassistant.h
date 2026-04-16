#ifndef TABASSISTANT_H
#define TABASSISTANT_H

#include <QObject>

namespace Ui { class MainWindow; }
class MainWindow;

/**
 * @brief Kontroler zakładki "Zadaj Pytanie do Asystenta".
 */
class TabAssistant : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor modułu asystenta.
     * @param ui Wskaźnik na interfejs UI.
     * @param parent Wskaźnik na główne okno.
     */
    explicit TabAssistant(Ui::MainWindow *ui, MainWindow *parent = nullptr);

private slots:
    /**
     * @brief Obsługuje przesyłanie zapytania tekstowego do AI i wyświetlanie odpowiedzi.
     */
    void handleAnalysis();

private:
    Ui::MainWindow *ui;      ///< Interfejs UI
    MainWindow *mainWindow;  ///< Wskaźnik na główne okno
};

#endif // TABASSISTANT_H
