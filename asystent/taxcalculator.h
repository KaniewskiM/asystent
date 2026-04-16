#ifndef TAXCALCULATOR_H
#define TAXCALCULATOR_H

/**
 * @file taxcalculator.h
 * @brief Kalkulator podatku od zysków kapitałowych.
 */

/**
 * @brief Struktura przechowująca wynik kalkulacji podatkowej.
 */
struct WynikPodatkowy {
    double zysk;    ///< Różnica między ceną sprzedaży a kupna (może być ujemna).
    double podatek; ///< Należny podatek (19% od zysku, 0 przy stracie).
    double netto;   ///< Kwota do wypłaty po odjęciu podatku.
};

/**
 * @brief Klasa odpowiedzialna za obliczanie podatku od zysków z kryptowalut.
 *
 * Oblicza podatek zgodnie z polskim prawem podatkowym:
 * stawka 19% od zysku kapitałowego. Przy stracie podatek wynosi 0.
 */
class TaxCalculator {
public:
    /**
     * @brief Oblicza wynik finansowy transakcji.
     *
     * @param kwotaKupna   Kwota zapłacona za kryptowalutę (w PLN).
     * @param kwotaSprzedazy Kwota uzyskana ze sprzedaży (w PLN).
     * @return WynikPodatkowy Struktura z zyskiem, podatkiem i kwotą netto.
     */
    WynikPodatkowy oblicz(double kwotaKupna, double kwotaSprzedazy) const;

    /**
     * @brief Zwraca stawkę podatkową stosowaną przy kalkulacjach.
     * @return Stawka podatkowa jako wartość dziesiętna (np. 0.19 dla 19%).
     */
    double stawkaPodatku() const;

private:
    static constexpr double STAWKA = 0.19; ///< Polska stawka podatku od zysków kapitałowych.
};

#endif // TAXCALCULATOR_H
