# 3D_Seminar_PostProcessing

## Name
Postprocessing Demo in OpenGL

## Description
Seminar Anwendung für Postprocessing.
Der Nutzer kann durch das User Interface Szenen, Clear color und Postprocessing Effekt Manipulieren und Parameter testen.

Die Anwendung wurde auf Windows(x64) Programmiert und nicht auf anderen Betriebssystemen getestet. Wir empfehlen daher ein Windows Gerät zu verwenden.


## Visuals

![Raw](example/exampleImage.png?raw=true "Pixel Effect")
![Raw](example/exampleImage2.png?raw=true "Lethal Effect")


Mögliche Bugs (Nur auf Intel integrated Graphics festgestellt) : 
1. SSAO modus ist sehr Dunkel und parameter Slider ändern nichts -> Programm neu starten 
2. Bloom lässt sich nicht ändern -> Programm neu starten
3.Sollte das Auswahl Fenster in der Anwendung im Hintergrund selber sein, und sie können es nicht Rausziehen, können sie die Imgui.ini Datei öffnen und den [Post Processing] Teil mit diesem hier ersetzen:

[Window][Post Processing]
ViewportPos=1200,150
ViewportId=0x01563991
Size=500,600
Collapsed=0

Dieser Bug sollte aber gefixed sein.

## Installation

Im Ordner "Setup1/Debug/Setup1.msi" finden sie ein Setup.
Dieses Setup ausführen und als Zielordner den Ordner:
"Setup1/Release" auswählen. Es wird empfohlen den Pfad aus dem Windows Explorer zu kopieren und einzusetzen.
"Setup1/Release" beinhaltet alle models, shaders, dll's und config Dateien die das Programm benötigt.

Nach der Installation die Anwendung OpenGL.exe im Release Ordner öffnen.

Start der Anwendung könnte etwas dauern, ein Whitescreen ist für paar Sekunden zu erwarten.

ODER:
Sollte diese Methode nicht funktionieren oder sie wollen das Programm selber bauen(nicht Empfohlen, dauert etwas lange):

Wenn sie Visual Studio auf ihrem Gerät haben, können sie im Ordner "OpenGL" eine "OpenGL.sln" datei finden. Öffnen sie diese in Visual Studio und bauen sie das Programm selber.


Nach Verwendung und Evaluierung(oder fehlgeschlagenem Setup) der Anwendung, wird empfohlen das Programm unter dem Windows Reiter "Installierte Apps" zu deinstallieren mit dem Namen "Setup1".


note: In einem Einzelfall kam es dazu das der Windows Antivirus eine Meldung zum Programm gegeben hat. Wenn das Vorkommen sollte bitte das Programm neu installieren.

Bei Fragen zur Anwendung bitte nicht zögern mir eine E-Mail zu schicken an "dobah100@uni-duesseldorf.de" .

## Model Creator

"Low Poly Winter Scene" (https://skfb.ly/6R6MM) by EdwiixGG is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"de_dust2 - CS map" (https://skfb.ly/6ACOH) by vrchris is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Medieval Fantasy Book" (https://skfb.ly/69Qty) by Pixel is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"(FREE) Low Poly Game Assets" (https://skfb.ly/oDSIM) by LowPolyBoy is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Fantasy Game Inn" (https://skfb.ly/JDD6) by sirsaugsage is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).