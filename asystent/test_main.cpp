#include <QtTest>
#include "taxcalculator.h"
#include "utils.h"
#include "cryptoexchange.h"

// @brief Rozbudowana klasa testów jednostkowych (Unit Tests) dla logiki aplikacji Krypto Asystent.
class TestKryptoAsystent : public QObject
{
    Q_OBJECT

private slots:
    // --- TESTY MODUŁU PODATKOWEGO ---
    void testTaxCalculator_Zysk() {
        TaxCalculator calc;
        // Standardowy zysk rynkowy
        WynikPodatkowy wynik = calc.oblicz(1000.0, 2000.0);
        QCOMPARE(wynik.zysk, 1000.0);
        QCOMPARE(wynik.podatek, 190.0);
        QCOMPARE(wynik.netto, 810.0);
    }

    void testTaxCalculator_Strata() {
        TaxCalculator calc;
        // Założenie: kupujemy za 5000 PLN, sprzedajemy w dołku (ujemny zysk = zero podatku)
        WynikPodatkowy wynik = calc.oblicz(5000.0, 2000.0);
        QCOMPARE(wynik.zysk, -3000.0);
        QCOMPARE(wynik.podatek, 0.0);
        QCOMPARE(wynik.netto, -3000.0);
    }

    void testTaxCalculator_ZarobekNaZero() {
        TaxCalculator calc;
        // Kupno i Sprzedaż za tą samą kwotę (tzw. Break Even)
        WynikPodatkowy wynik = calc.oblicz(3000.0, 3000.0);
        QCOMPARE(wynik.zysk, 0.0);
        QCOMPARE(wynik.podatek, 0.0);
        QCOMPARE(wynik.netto, 0.0);
    }
    
    void testTaxCalculator_StawkaPrawna() {
        TaxCalculator calc;
        QCOMPARE(calc.stawkaPodatku(), 0.19); // Polskie prawo nakazuje sztywno 19%
    }

    // --- TESTY MODUŁU UTILS I BEZPIECZEŃSTWA TEKSTU ---
    void testUtils_WyczyscJson() {
        // Testowanie usuwania zbędnego kodu z JSONa (czyszczenie stringów)
        std::string input = "Oto tekst\\nNowa linia i cudzys\u0142\u00f3w \\\"Gie\u0142da\\\"";
        std::string expected = "Oto tekst\nNowa linia i cudzysł\xf3w \"Giełda\""; 
        
        std::string basicInput = "Linia 1\\nLinia 2\\\"Koniec\\\"";
        std::string basicExpected = "Linia 1\nLinia 2\"Koniec\"";
        
        QCOMPARE(QString::fromStdString(wyczyscJson(basicInput)), QString::fromStdString(basicExpected));
    }

    // --- TESTY ODPORNOŚCI GIEŁDY (ZABEZPIECZENIE PRZED AWARIAMI) ---
    void testCryptoExchange_NiepoprawnySymbolCeny() {
        // Zgodnie z wytycznymi na 5.0 program musi trzymać stabilność w razie awarii.
        // Podanie giełdzie waluty, która nie istnieje:
        CryptoExchange exchange;
        std::string fakePrice = exchange.fetchCurrentPrice("WKURZAJACY_KOT_TOKEN");
        
        // Funkcja powinna złapać brak JSON'a i zablokować awarię zawracając string "BLAD"
        QCOMPARE(QString::fromStdString(fakePrice), QString("BLAD"));
    }

    void testCryptoExchange_NiepoprawnySymbolPodaży() {
        // Sprawdzenie czy inteligentny Fallback w fetchSupply broni nas przed wyciekiem pamięci
        CryptoExchange exchange;
        double fakeSupply = exchange.fetchSupply("JAKAS_LOSOWA_WIEZA");
        
        // Podaż dla nieistniejącej w słowniku waluty musi zabezpieczyć matematykę i zwrócić 0.0
        QCOMPARE(fakeSupply, 0.0);
    }
};

QTEST_MAIN(TestKryptoAsystent)
#include "test_main.moc"
