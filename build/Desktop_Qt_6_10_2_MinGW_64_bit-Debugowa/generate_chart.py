import plotly.graph_objects as go
import pandas as pd
import sys
import requests
import os

# To jest ten "silnik graficzny", który C++ wywołuje, żeby narysować wykres.

# --- BEZPIECZNE ODBIERANIE ARGUMENTÓW (Logiczna poduszka powietrzna) ---
# Jeśli C++ wyśle nam śmieci (np. '...'), skrypt ich nie zje i użyje wartości domyślnej.
try:
    # 1. Argument: Cena Bitcoina (BTC)
    # Sprawdzamy czy argument istnieje I czy jest liczbą
    if len(sys.argv) > 1 and sys.argv[1].replace('.','',1).isdigit():
        cena_btc_usd = float(sys.argv[1])
        print(f"Używam ceny BTC z argumentu: {cena_btc_usd}")
    else:
        # Jeśli dostajemy "...", "czekam" albo brak argumentu
        cena_btc_usd = 95000.0 # Domyślna wartość "bezpieczna"
        print(f"Brak poprawnego argumentu dla BTC. Używam ceny domyślnej: {cena_btc_usd}")

    # 2. Argument: Cena Złota (GOLD)
    if len(sys.argv) > 2 and sys.argv[2].replace('.','',1).isdigit():
        cena_gold_usd = float(sys.argv[2])
        print(f"Używam ceny Złota z argumentu: {cena_gold_usd}")
    else:
        cena_gold_usd = 2100.0 # Domyślna wartość
        print(f"Brak poprawnego argumentu dla Złota. Używam ceny domyślnej: {cena_gold_usd}")

except Exception as e:
    # Ostateczna poduszka powietrzna na wypadek totalnego błędu logicznego
    cena_btc_usd = 95000.0
    cena_gold_usd = 2100.0
    print(f"Wystąpił nieoczekiwany błąd argumentów: {e}. Używam wartości domyślnych.")

# --- POBIERANIE PRAWDZIWYCH DANYCH historycznych z Binance API ---
def pobierz_historie(symbol, interval='1d', limit=1000):
    try:
        url = f"https://api.binance.com/api/v3/klines"
        params = {
            'symbol': symbol,
            'interval': interval, # '1h', '1d', '1w'
            'limit': limit # Maksymalnie 1000 świeczek
        }
        data = requests.get(url, params=params).json()
        
        # Przerabiamy dane z API na ładną tabelkę Pandas
        df = pd.DataFrame(data, columns=['Open time', 'Open', 'High', 'Low', 'Close', 'Volume', 'Close time', 'Quote asset volume', 'Number of trades', 'Taker buy base asset volume', 'Taker buy quote asset volume', 'Ignore'])
        
        # Konwertujemy czas na czytelny format
        df['Open time'] = pd.to_datetime(df['Open time'], unit='ms')
        # Konwertujemy ceny na liczby zmiennoprzecinkowe
        df['Close'] = df['Close'].astype(float)
        
        return df[['Open time', 'Close']]
    except Exception as e:
        print(f"Błąd API: {e}")
        return None

# --- Main Script ---
# Pobieramy historię dla BTCUSDT (1 dzień, 1000 świeczek to prawie 3 lata historii!)
print("Pobieram historię BTC z Binance API...")
df_btc = pobierz_historie("BTCUSDT", interval='1d', limit=1000)

if df_btc is None:
    # Jeśli API Binance leży, nie pękaj, po prostu zrób prosty wykres z jednej ceny
    print("Błąd API Binance. Generuję prosty wykres punktowy.")
    fig = go.Figure(go.Scatter(x=[datetime.datetime.now()], y=[cena_btc_usd], mode='markers', name='BTC (Punkt)'))
    fig.update_layout(title="Błąd API. Pokazuję tylko obecną cenę.", template='plotly_dark')
else:
    # Mamy dane z API! Tworzymy interaktywny wykres liniowy.
    fig = go.Figure()
    fig.add_trace(go.Scatter(x=df_btc['Open time'], y=df_btc['Close'], name='Cena BTC (USD)', line=dict(color='#0078d7', width=2)))

    # --- Super-Interaktywność (Przycisk zmiany okresu, suwak) ---
    fig.update_layout(
        title='<b>Interaktywna Historia Bitcoina (BTC/USD) - Dane Real-Time z API</b>',
        title_x=0.5,
        template='plotly_dark',
        hovermode='x unified',
        xaxis=dict(
            rangeselector=dict( # Przyciski zmiany okresu
                buttons=list([
                    dict(count=7, label="1w", step="day", stepmode="backward"),
                    dict(count=1, label="1m", step="month", stepmode="backward"),
                    dict(count=1, label="1r", step="year", stepmode="backward"),
                    dict(step="all", label="Wszystko")
                ]),
                activecolor="#0078d7", bgcolor="#2b2b2b"
            ),
            rangeslider=dict(visible=True, thickness=0.05), # Suwak na dole
            type="date"
        ),
        yaxis=dict(tickformat='$,.2f') # Formatowanie osi jako waluta
    )

# --- Zapisywanie do pliku HTML ---
# C++ to wczyta z folderu 'debug'
sciezka = os.path.abspath("wykres.html")
fig.write_html(sciezka, include_plotlyjs="cdn")
print(f"Udało się! Interaktywny wykres zapisany w: {sciezka}")