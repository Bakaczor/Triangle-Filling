# Triangle Filling
Triangle Filling to aplikacja pozwalająca na renderowanie animacji krążącego nad powierzchnią Beziera światła, z opcją zmiany tła i tekstur. Program został zbudowany na Qt 6.6.0, w standardzie C++20.

# Klawiszologia
W tej sekcji znajdują się informacje na temat korzystania z aplikacji.

## Tryby wyświetlania
Aplikacja posiada dwa tryby wyświetlania zależne od checkboxa `show`.

1. Tryb wyświetlania i edycji punktów kontrolnych - po wejściu w ten tryb animacja jest zatrzymana. Użytkownik może wybierać punkty kontrolne poprzez kliknięcie (aktywny punkt zaświeci się na czerwono), a następnie zmieniać ich wysokość suwakiem na prawo od checkboxa.
2. Tryb animacji - w tym trybie można kontrolować animację (przyciski `play` i `pause`). Oprócz tego, przy pomocy checkboxa `show grid` można włączyć wyświetlanie siatki trójkątów.

## Ustawienia animacji
Następujące ustawienia zatrzymają animację w momencie ich modyfikacji:

1. Zmiana liczby trójkątów w siatce - obsługiwana przez suwak `triangulation`
2. Zmiana zdjęcia wyświetlanego w tle - pozwala na to przyciski `image`
3. Zmiana tesktury tła (tzw. normal map) - pozwala na to przycisk `map`

Pozostałe ustawienia można zmieniać w trakcie animacji:
- kolor światła - przycisk `light color`
- kolor tła - przycisk `background`
- współczynnik odbicia rozproszonego - suwak `kd`
- współczynnik odbicia zwierciadlanego - suwak `ks`
- współczynnik lustrzaności - suwak `m`

## Kontakt
W razie pytań proszę się ze mną kontaktować pod mailem bartosz.kaczorowski2.stud@pw.edu.pl.