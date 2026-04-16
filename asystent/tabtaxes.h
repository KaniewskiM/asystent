#ifndef TABTAXES_H
#define TABTAXES_H

#include <QObject>

namespace Ui { class MainWindow; }
class MainWindow;

/**
 * @brief Kontroler zakładki "Kalkulator Podatkowy".
 */
class TabTaxes : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor modułu podatkowego.
     * @param ui Wskaźnik na interfejs UI.
     * @param parent Wskaźnik na główne okno.
     */
    explicit TabTaxes(Ui::MainWindow *ui, MainWindow *parent = nullptr);

private slots:
    /**
     * @brief Pobiera dane z UI i oblicza podatek korzystając z klasy TaxCalculator.
     */
    void calculateTax();

private:
    Ui::MainWindow *ui;      ///< Interfejs UI
    MainWindow *mainWindow;  ///< Wskaźnik na główne okno
};

#endif // TABTAXES_H
