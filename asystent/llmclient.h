#ifndef LLMCLIENT_H
#define LLMCLIENT_H

#include <string>

/**
 * @brief Klasa odpowiedzialna za komunikację ze sztuczną inteligencją.
 */
class LLMClient {
public:
    LLMClient();
    ~LLMClient();

    /**
     * @brief Wysyła zapytanie do modelu Gemini (Google).
     * @param prompt Treść pytania.
     * @return Odpowiedź modelu jako string.
     */
    std::string askGemini(const std::string& prompt);

    /**
     * @brief Wysyła zapytanie do lokalnego modelu Llama (via Ollama).
     * @param prompt Treść pytania.
     * @param temperature Parametr kreatywności (0.0 - 1.0).
     * @param numPredict Maksymalna liczba generowanych tokenów.
     * @return Odpowiedź modelu jako string.
     */
    std::string askLlama(const std::string& prompt, double temperature = 0.5, int numPredict = 800);

private:
    std::string apiKey;       ///< Klucz API wczytany z pliku
    std::string geminiModel;  ///< Nazwa używanego modelu Gemini
    std::string llamaEndpoint;///< Adres URL serwera Ollama
};

#endif // LLMCLIENT_H
