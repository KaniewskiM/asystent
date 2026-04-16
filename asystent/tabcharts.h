#ifndef TABCHARTS_H
#define TABCHARTS_H

#include <QObject>

namespace Ui { class MainWindow; }
class MainWindow;

/**
 * @brief Kontroler zakładki "Wykresy i trend".
 */
class TabCharts : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor modułu wykresów.
     * @param ui Wskaźnik na interfejs UI.
     * @param parent Wskaźnik na główne okno.
     */
    explicit TabCharts(Ui::MainWindow *ui, MainWindow *parent = nullptr);

private slots:
    /**
     * @brief Pobiera dane i uruchamia zewnętrzny skrypt Python do generowania HTML z wykresem.
     */
    void generateChart();
    
    /**
     * @brief Włącza/wyłącza wybór daty końcowej (tryb "do teraz").
     * @param checked Stan checkboxa.
     */
    void toggleDoTeraz(bool checked);
    
    /** @brief Skrót ustawiający interwał 24h. */
    void setInterwal24h();
    /** @brief Skrót ustawiający interwał 7 dni. */
    void setInterwal7d();
    /** @brief Skrót ustawiający interwał 1 miesiąc. */
    void setInterwal1m();

private:
    Ui::MainWindow *ui;      ///< Interfejs UI
    MainWindow *mainWindow;  ///< Wskaźnik na główne okno
};

#endif // TABCHARTS_H
