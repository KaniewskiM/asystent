#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @file utils.h
 * @brief Funkcje pomocnicze używane w całej aplikacji.
 */

/**
 * @brief Czyści tekst z technicznych sekwencji ucieczki JSON.
 *
 * Zamienia sekwencje takie jak \\n, \\" czy \\u0026
 * na odpowiadające im znaki (np. nową linię, cudzysłów, ampersand).
 *
 * @param tekst Surowy tekst zawierający sekwencje ucieczki JSON.
 * @return Oczyszczony tekst gotowy do wyświetlenia użytkownikowi.
 */
std::string wyczyscJson(std::string tekst);

/**
 * @brief Zabezpiecza tekst do wstawienia do zapytania JSON.
 *
 * Zamienia znaki takie jak nowa linia czy cudzysłów
 * na ich bezpieczne odpowiedniki (\\n, \\"), aby zapytanie JSON było poprawne składniowo.
 *
 * @param tekst Tekst wejściowy (np. prompt)
 * @return Bezpieczny tekst JSON
 */
std::string zabezpieczJson(std::string tekst);

#endif // UTILS_H
