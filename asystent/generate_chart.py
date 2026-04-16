import sys
import json
import plotly.graph_objects as go
from datetime import datetime

def main():
    # Pobieramy argumenty z C++ (Symbol, Typ, Supply)
    symbol = sys.argv[1] if len(sys.argv) > 1 else "Nieznany Symbol"
    chart_type = sys.argv[2] if len(sys.argv) > 2 else "Świecowy"
    
    # NOWOŚĆ: Argument 3 to Supply, Argument 4 to Flaga MCAP/PRICE
    supply = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0
    is_mcap = (len(sys.argv) > 4 and sys.argv[4] == "MCAP")

    try:
        with open('hist.txt', 'r') as f:
            data = json.load(f)

        # TWOJE ZABEZPIECZENIE: Sprawdzamy, czy giełda w ogóle coś zwróciła
        if len(data) < 2:
            raise ValueError("Zbyt mały przedział czasu. Giełda zwróciła za mało danych (potrzebne min. 2 punkty).")

        # PRZERABIAMY DANE I WYMNAŻAMY PRZEZ SUPPLY
        # Dzięki temu jeden skrypt rysuje i CENĘ i KAPITALIZACJĘ
        times = [datetime.fromtimestamp(x[0] / 1000.0) for x in data]
        opens = [float(x[1]) * supply for x in data]
        highs = [float(x[2]) * supply for x in data]
        lows = [float(x[3]) * supply for x in data]
        closes = [float(x[4]) * supply for x in data]

        fig = go.Figure()

        # LOGIKA WYBORU WYKRESU
        if "Liniowy" in chart_type or is_mcap:
            # Dla Kapitalizacji ZAWSZE lepszy jest liniowy (is_mcap wymusza linię)
            # Używamy złotego koloru dla kapitalizacji, neonowego dla ceny
            kolor_linii = '#FFD700' if is_mcap else '#00E5FF'
            nazwa_serii = 'Kapitalizacja (USD)' if is_mcap else 'Cena (USD)'
            
            fig.add_trace(go.Scatter(
                x=times, 
                y=closes, 
                mode='lines', 
                name=nazwa_serii,
                line=dict(color=kolor_linii, width=2)
            ))
        else:
            # Twoje standardowe świece japońskie (tylko dla wykresu ceny)
            fig.add_trace(go.Candlestick(
                x=times, 
                open=opens, 
                high=highs, 
                low=lows, 
                close=closes,
                name='Świece rynkowe'
            ))

        # TWOJA STYLIZACJA (Zachowana w całości)
        tytul_wykresu = f'Analiza Kapitalizacji: {symbol} (Rozpoznana Podaż: {supply:,.0f} szt.)' if is_mcap else f'Analiza Giełdowa: {symbol} ({chart_type})'
        y_axis_label = 'Wartość Rynkowa (USD)' if is_mcap else 'Cena (USD)'

        fig.update_layout(
            title=tytul_wykresu,
            template='plotly_dark',
            xaxis_title='Czas / Data',
            yaxis_title=y_axis_label,
            xaxis_rangeslider_visible=False,
            margin=dict(l=40, r=40, t=60, b=40)
        )

        fig.write_html("wykres.html")

    except Exception as e:
        # TWOJA OBSŁUGA BŁĘDÓW (Żeby nie było czarnego ekranu)
        fig = go.Figure()
        fig.update_layout(
            title=f"BŁĄD GENEROWANIA:<br>{str(e)}", 
            template='plotly_dark'
        )
        fig.write_html("wykres.html")

if __name__ == "__main__":
    main()
