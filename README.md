# Druud: een nacht- en ontwaaklampje

## Specificatie
Dit project biedt een lampje, om kinderen te helpen bij het niet te vroeg opstaan, dat 3 fases kent:
1. de nacht, rood, waarbij het kind in bed moet blijven;
2. het ontwaken, geel, waarbij het binnenkort tijd is om druud te komen;
3. het opstaan, groen, waarbij het uit bed mag komen;

Het lampje maakt gebruik van WiFi om de tijd op te halen; in de configuratie (`config.h`) moet daarom de naam van het Wifi netwerk (`ssid`) en het bijbehorende wachtwoord (`password`) worden opgegeven.

Daarnaast kunnen de tijden waarop de kleur wijzigt worden ingesteld als `HH:mm`:
- `ontwaken  = "06:30";  // van rood -> geel`
- `druud     = "06:50";  // van geel -> groen`
- `slapen    = "19:30";  // van groen -> rood`

Tenslotte kunnen de helderheid (een getal tussen 1 en 255) en de duur van de verkleuring (in seconden) worden ingesteld:
- `helderheid  = 10;     // tussen 1 en 255`
- `duur        = 30;     // seconden van de overgang van rood -> geel en van geel -> groen`

## Hardware
Het project gaat uit van:
- Wemos LOLIN D1 mini
- Wemos WS2812B LED Shield for D1 Mini

Het Shield moet op de D1 Mini worden gesoldeerd.

## Voeding
De Druud wordt gevoed door een `USB A` naar `USB Micro` kabel en een USB oplader.

## Installatie en configuratie

Om de Druud te configureren moet allereerst de Arduino IDE worden geinstalleerd op een computer: zie https://www.arduino.cc/en/software; voor Windows is deze ook te downloaden van de Windows store. 

Voeg `http://arduino.esp8266.com/stable/package_esp8266com_index.json` toe aan Preferences > `Additional Boards Manager URLs‘`

Voeg de `D1 Mini` familie toe via Tools > Board > Boards Manager, dan zoek op `ESP8266` en installeer `esp8266`.

Sluit de Druud aan via de usb poort

Selecteer via Tools > Board > ESP8266 ... > LOLIN (WEMOS) D1 R2 & Mini

Selecteer via Tools > Port > (voor een Mac:) /dev/usbserial-14310 of (voor Windows:) COM1 of COM5 (weet ik niet zeker ...)

Download de software van https://github.com/rimvanvliet/druud/archive/refs/heads/main.zip en pak t uit.

Open de Druud software via File > Open... en open druud-main/druud/druud.ino

Selecteer `config-template.h` en wijzig de naam naar `config.h` met het drop-down driehoekje, rechts bovenin het Arduino scherm

Wijzig de instellingen in `config.h`

Via Tools > Manage Libraries:
- zoek `NTPClient` van Fabrice Weinberg en installeer die.
- zoek `Timezone` van Jack Christensen en installeer die, en de libraries waar deze van afhankelijk is.
- zoek `Adafruit_NeoPixel` van Adafruit en installeer die (let op, er zijn librarie met bijna dezelfde naam)

Open de monitor via Tools > Serial Monitor en zorg dat je zowel de monitor als het Arduino IDE venster zelf kunt zien.

Laadt de Druud softare via Sketch > Upload 

Als het uploaden gelukt is, ontkoppel dan de Druud van de computer en sluit hem aan op de USB lader
