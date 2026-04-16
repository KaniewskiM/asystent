#ifndef TABANALYSIS_H
#define TABANALYSIS_H

#include <QObject>

namespace Ui { class MainWindow; }
class MainWindow;

/**
 * @brief Kontroler obsługujący zakładkę Zaawansowanej Analizy Technicznej.
 */
class TabAnalysis : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor modułu analizy.
     * @param ui Wskaźnik na interfejs UI.
     * @param parent Wskaźnik na główne okno.
     */
    explicit TabAnalysis(Ui::MainWindow *ui, MainWindow *parent = nullptr);

private slots:
    /**
     * @brief Wykonuje wieloetapową analizę rynkową przy użyciu AI.
     */
    void performDetailedAnalysis();

private:
    Ui::MainWindow *ui;      ///< Interfejs UI
    MainWindow *mainWindow;  ///< Wskaźnik na główne okno
};

#endif // TABANALYSIS_H
