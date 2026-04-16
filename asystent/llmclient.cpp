#include "llmclient.h"
#include "utils.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <chrono>

LLMClient::LLMClient() {
    using namespace std;
    
    ifstream keyFile("api_key.txt");
    if (keyFile.is_open()) {
        string rawKey;
        getline(keyFile, rawKey);
        apiKey = "";
        // Przepisujemy tylko poprawne znaki (omijamy ukryte znaki, entery, spacje wpadające przy kopiuj-wklej)
        for (char c : rawKey) {
            if (c > 32) apiKey += c;
        }
        keyFile.close();
    } else {
        // Fallback oraz stworzenie pliku przy pierwszym odpaleniu
        apiKey = "AIzaSyCDUqpa5-e-OmyFAaSWgTXIJzUQz609xFw";
        ofstream outKey("api_key.txt");
        if (outKey.is_open()) {
            outKey << apiKey;
            outKey.close();
        }
    }
    
    // Konfiguracja nazwy lokalnego modelu AI
    ifstream modelFile("llama_model.txt");
    if (modelFile.is_open()) {
        string rawModel;
        getline(modelFile, rawModel);
        llamaModel = "";
        for (char c : rawModel) {
            if (c > 32) llamaModel += c;
        }
        modelFile.close();
        if(llamaModel.empty()) llamaModel = "llama3.1"; // zabezpieczenie przed pustym plikiem
    } else {
        llamaModel = "llama3.1"; // domyślny na ten moment
        ofstream outModel("llama_model.txt");
        if (outModel.is_open()) {
            outModel << llamaModel;
            outModel.close();
        }
    }

    geminiModel = "gemini-flash-latest";
    llamaEndpoint = "http://127.0.0.1:11434/api/generate";
}

LLMClient::~LLMClient() {
}

std::string LLMClient::askGemini(const std::string& prompt) {
    using namespace std;
    
    // Używamy timestampu dla unikalności plików w QtConcurrent
    auto ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    string reqFile = "gemini_req_" + to_string(ms) + ".json";
    string resFile = "gemini_res_" + to_string(ms) + ".txt";

    ofstream pj(reqFile);
    pj << "{\"contents\":[{\"parts\":[{\"text\":\"" << wyczyscJson(prompt) << "\"}]}]}";
    pj.close();

    string url = "https://generativelanguage.googleapis.com/v1beta/models/" + geminiModel + ":generateContent?key=" + apiKey;
    string cmd = "curl -s -X POST \"" + url + "\" -H \"Content-Type: application/json\" -d @" + reqFile + " > " + resFile;
    system(cmd.c_str());

    ifstream po(resFile);
    string odpowiedz = "Błąd komunikacji z Gemini.";

    if (po.is_open()) {
        string s((istreambuf_iterator<char>(po)), istreambuf_iterator<char>());
        po.close();

        size_t textKeyPos = s.find("\"text\"");
        if (textKeyPos != string::npos) {
            size_t startQuote = s.find("\"", s.find(":", textKeyPos));
            size_t endQuote = s.find("\"", startQuote + 1);

            while (endQuote != string::npos && s[endQuote-1] == '\\') {
                endQuote = s.find("\"", endQuote + 1);
            }

            if (startQuote != string::npos && endQuote != string::npos) {
                odpowiedz = wyczyscJson(s.substr(startQuote + 1, endQuote - startQuote - 1));
            }
        } else if (s.find("\"error\"") != string::npos) {
            if (s.find("429") != string::npos) return "BŁĄD: Przekroczono limit zapytań (Error 429). Odczekaj minutę.";
            if (s.find("403") != string::npos) return "BŁĄD: Zły klucz API lub brak uprawnień (Error 403).";
            if (s.find("404") != string::npos) return "BŁĄD: Model nieodnaleziony (Error 404). Sprawdź nazwę modelu!";
            odpowiedz = "Błąd serwera Google.";
        }
    }
    
    // Cleanup temporary files
    remove(reqFile.c_str());
    remove(resFile.c_str());
    return odpowiedz;
}

std::string LLMClient::askLlama(const std::string& prompt, double temperature, int numPredict) {
    using namespace std;
    
    auto ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    string reqFile = "llama_req_" + to_string(ms) + ".json";
    string resFile = "llama_res_" + to_string(ms) + ".txt";

    ofstream pj(reqFile);
    pj << "{\"model\":\"" << llamaModel << "\",\"prompt\":\"" << wyczyscJson(prompt) 
       << "\",\"stream\":false,\"options\":{\"temperature\":" << temperature << ", \"num_predict\": " << numPredict << "}}";
    pj.close();

    string cmd = "curl -s -X POST \"" + llamaEndpoint + "\" -d @" + reqFile + " > " + resFile;
    system(cmd.c_str());

    string odpowiedz = "Błąd statusu: (Model Llama nie odpowiedział)";
    ifstream po(resFile);
    if (po.is_open()) {
        string s((istreambuf_iterator<char>(po)), istreambuf_iterator<char>());
        po.close();
        
        size_t start = s.find("\"response\":\"");
        if (start != string::npos) {
            size_t endQuote = s.find("\"", start + 12);
            while (endQuote != string::npos && s[endQuote-1] == '\\') {
                endQuote = s.find("\"", endQuote + 1);
            }
            if (endQuote != string::npos) {
                odpowiedz = wyczyscJson(s.substr(start + 12, endQuote - (start + 12)));
            }
        }
    }

    remove(reqFile.c_str());
    remove(resFile.c_str());
    return odpowiedz;
}
