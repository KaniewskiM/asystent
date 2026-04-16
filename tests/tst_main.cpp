#include <QtTest/QtTest>
#include "taxcalculator.h"
#include "utils.h"

/**
 * @file tst_main.cpp
 * @brief Testy jednostkowe dla TaxCalculator i wyczyscJson.
 */

// =========================================================
// TESTY: TaxCalculator
// =========================================================
class TestTaxCalculator : public QObject {
    Q_OBJECT

private slots:

    // --- Przypadki podstawowe ---

    void testZysk() {
        TaxCalculator calc;
        WynikPodatkowy w = calc.oblicz(1000.0, 1500.0);
        QCOMPARE(w.zysk,     500.0);
        QCOMPARE(w.podatek,   95.0);  // 19% z 500
        QCOMPARE(w.netto,    405.0);
    }

    void testStrata() {
        TaxCalculator calc;
        WynikPodatkowy w = calc.oblicz(2000.0, 1000.0);
        QCOMPARE(w.zysk,    -1000.0);
        QCOMPARE(w.podatek,     0.0); // brak podatku przy stracie
        QCOMPARE(w.netto,   -1000.0);
    }

    void testWyjscieCalkiemNaZero() {
        // Kupno == Sprzedaż → zysk = 0, podatek = 0
        TaxCalculator calc;
        WynikPodatkowy w = calc.oblicz(500.0, 500.0);
        QCOMPARE(w.zysk,    0.0);
        QCOMPARE(w.podatek, 0.0);
        QCOMPARE(w.netto,   0.0);
    }

    void testBardzoMalyZysk() {
        // Sprawdza precyzję dla małych kwot
        TaxCalculator calc;
        WynikPodatkowy w = calc.oblicz(100.0, 100.01);
        QVERIFY(w.podatek > 0.0);
        QVERIFY(w.podatek < 0.01);
    }

    void testDuzeKwoty() {
        TaxCalculator calc;
        WynikPodatkowy w = calc.oblicz(100000.0, 200000.0);
        QCOMPARE(w.zysk,     100000.0);
        QCOMPARE(w.podatek,   19000.0);
        QCOMPARE(w.netto,     81000.0);
    }

    void testStawkaPodatku() {
        TaxCalculator calc;
        QCOMPARE(calc.stawkaPodatku(), 0.19);
    }
};

// =========================================================
// TESTY: wyczyscJson
// =========================================================
class TestWyczyscJson : public QObject {
    Q_OBJECT

private slots:

    void testNowaLinia() {
        std::string wynik = wyczyscJson("linia1\\nlinia2");
        QCOMPARE(wynik, std::string("linia1\nlinia2"));
    }

    void testCudzyslow() {
        std::string wynik = wyczyscJson("powiedz \\\"hej\\\"");
        QCOMPARE(wynik, std::string("powiedz \"hej\""));
    }

    void testAmpersand() {
        std::string wynik = wyczyscJson("AT\\u0026T");
        QCOMPARE(wynik, std::string("AT&T"));
    }

    void testKilkaZamianNaraz() {
        std::string wynik = wyczyscJson("A\\nB\\u0026C\\\"D\\\"");
        QCOMPARE(wynik, std::string("A\nB&C\"D\""));
    }

    void testPustyString() {
        std::string wynik = wyczyscJson("");
        QCOMPARE(wynik, std::string(""));
    }

    void testBrakSekwencji() {
        // Tekst bez żadnych sekwencji ucieczki — powinien wrócić bez zmian
        std::string wynik = wyczyscJson("normalny tekst 123");
        QCOMPARE(wynik, std::string("normalny tekst 123"));
    }

    void testWielokrotneWystapienia() {
        std::string wynik = wyczyscJson("\\n\\n\\n");
        QCOMPARE(wynik, std::string("\n\n\n"));
    }
};

// =========================================================
// Uruchomienie wszystkich testów
// =========================================================
int main(int argc, char *argv[]) {
    int status = 0;
    { TestTaxCalculator t; status |= QTest::qExec(&t, argc, argv); }
    { TestWyczyscJson   t; status |= QTest::qExec(&t, argc, argv); }
    return status;
}

#include "tst_main.moc"
