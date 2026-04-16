#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tabassistant.h"
#include "tabnews.h"
#include "tabcharts.h"
#include "tabtaxes.h"
#include "tabanalysis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Główna klasa okna aplikacji zarządzająca interfejsem i kontrolerami.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor głównego okna.
     * @param parent Wskaźnik na rodzica (opcjonalny).
     */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Destruktor zamykający zasoby.
     */
    ~MainWindow() override;

private:
    Ui::MainWindow *ui; ///< Wskaźnik na interfejs użytkownika wygenerowany z pliku .ui
    
    // Kontrolery logiczne dla poszczególnych modułów aplikacji
    TabAssistant *tabAssistant; ///< Moduł asystenta edukacyjnego
    TabNews *tabNews;           ///< Moduł wiadomości rynkowych
    TabCharts *tabCharts;       ///< Moduł wykresów i analizy trendów
    TabTaxes *tabTaxes;         ///< Moduł kalkulatora podatkowego
    TabAnalysis *tabAnalysis;   ///< Moduł zaawansowanej analizy technicznej
};

#endif // MAINWINDOW_H