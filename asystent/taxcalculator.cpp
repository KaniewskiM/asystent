#include "taxcalculator.h"

WynikPodatkowy TaxCalculator::oblicz(double kwotaKupna, double kwotaSprzedazy) const {
    WynikPodatkowy wynik;
    wynik.zysk    = kwotaSprzedazy - kwotaKupna;
    wynik.podatek = (wynik.zysk > 0) ? wynik.zysk * STAWKA : 0.0;
    wynik.netto   = wynik.zysk - wynik.podatek;
    return wynik;
}

double TaxCalculator::stawkaPodatku() const {
    return STAWKA;
}
