/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

#define DE 1
#define EN 2

#define language DE

#if language == DE
//Start
#define MAINMENU_1 "1. Kraft und Bewegung"
#define MAINMENU_2 "2. Druck und W\x84rme"
#define MAINMENU_3 "3. Licht und Schatten"
#define MAINMENU_4 "4. Strom und Spannung"
#define MAINMENU_5 "5. Schall und Wellen"
#define MAINMENU_6 "6. Sensoren und Aktoren"
#define MAINMENU_7 "7. Dies und Das"
#define MAINMENU_8 "8. Info"

//Allgemein
#define B_SELECT "Auswahl"
#define SAVE "Speichern"
#define RESET "Reset"
#define EDIT "Bearbeiten"
#define SAVED "Gespeichert"
#define NUM "Nr."
#define START "Start"
#define STOP "Stop"
#define DELETE "L\x94schen"
#define PAUSE "Pause"
#define MOD_NOT_CONN "Modul nicht angeschlossen!"

//toolkit
#define IMG_ERR "Fehler beim Laden einer Bilddatei!"

//DUW
#define MENU_DUW_1 "1. Messung einer Temperatur (Wasserdicht)" //Aufzeichnung.. Messung davor Erfassen..
#define MENU_DUW_2 "2. Messung zweier Temperaturen (Wasserdicht)"
#define MENU_DUW_3 "3. Druck und Temperatur"
#define MENU_DUW_4 "4."
#define MENU_DUW_5 "5."
#define TIM_IN_S "Zeit in s"
#define T_IN_C "Temperatur in \xF8\x43"
#define T1_IN_C "T1 in \xF8\x43"
#define T2_IN_C "T2 in \xF8\x43"
#define START_10S "Start (10s)"
#define START_1M "Start (1m)"
#define START_5M "Start (5m)"
#define MEASUREMENT "Messung"
#define START_MEASUREMENT "Messung starten.."

//KBW
#define MENU_KBW_1 "1. Dunkelzeit (1LS)";
#define MENU_KBW_2 "2. Dunkelzeit (2LS)";
#define MENU_KBW_3 "3. Messwagen";
#define MENU_KBW_4 "4. Laufzeit (2LS)";
#define MENU_KBW_5 "5. Durchschnittsgeschwindigkeit (2LS)";
#define MENU_KBW_6 "6. Stroboskopaufnahme des freien Falls";
#define MENU_KBW_7 "7. Dauer des freien Falls";
#define MENU_KBW_8 "8. Schwerelosigkeit im freien Fall";
#define MENU_KBW_9 "9. Energieerhaltung am Fadenpendel";
#define MENU_KBW_10 "10. Periode des Fadenpendels";
#define MENU_KBW_11 "11. Periode des Federpendels";
#define MENU_KBW_12 "12. Radialbeschleunigung (Graph)";
#define MENU_KBW_13 "13. Radialbeschleunigung (Wert)";
#define MENU_KBW_14 "14. Ultraschallmessung am Federpendel";
#define MENU_KBW_15 "15. Synchronisation eines Motors auf ein Pendel";

#define DELTA_H_CM "delta h [cm]"
#define T_IN_MS "t [ms]"
#define T1_IN_MS "t1 [ms]"
#define T2_IN_MS "t2 [ms]"
#define D_IN_M "d [m]"
#define T_IN_S "t [s]"
#define V_IN_MS "v [m/s]"
#define M_PERIOD "Bestimmung der Periodendauer..."
#define U_SONIC "Ultraschall"
#define Z_R_S "0.0 U/s"
#define R_S_FORMAT "%.1f U/s"
#define A_MS2_FORMAT "%.1f m/s\xFD"
#define TRIGGER "Ausl\x94sen"
#define F_FALL "Freier Fall"
#define S_STR "s in Streifen"
#define V_STR_S "v in Str. pro Sek."
#define T1S "1s"
#define T2S "2s"
#define T5S "5s"
#define T10S "10s"
#define T20S "20s"
#define PLOT_S "Plotte s(t)"
#define PLOT_V "Plotte v(t)"
#define VALUES "Werte"
#define REPEAT "Nochmal!"

//KUK
#define MENU_KUK_1 "1. Stoppuhr"
#define MENU_KUK_2 "2. Timer"
#define SPLIT "Zwischenwert"
#define SET "Setzen"
//LUS
// ###

//SUA
#define MENU_SUA_1 "1. Beleuchtungsst\x84rke"
#define MENU_SUA_2 "2. Entfernung (TOF)"
#define MENU_SUA_3 "3. Druck und Temperatur"
#define MENU_SUA_4 "4. Temperatur (Wasserdicht)"
#define MENU_SUA_5 "5. Spannung"
#define MENU_SUA_6 "6. Magnetische Flussdichte"
#define MENU_SUA_7 "7. Kraft"
#define MENU_SUA_8 "8. Funktionsgenerator"
#define SLUX " lux"
#define F_LUX "%d lux"
#define F_TEMP "%.2f %cC"
#define P_TEMP "%.2f Pa"
#define F_MV "%.2f mV"
#define F_GAUSS "%.2f Gauss"
#define F_N "%.2f N"
#define F_HZ "%d Hz"
#define OFFSET "Nullwert"
#define W_FORM "Wellenform"
#define FREQ "Frequenz"
#define OFF "Aus"
#define Z_HZ "0 Hz"
#define WF_SINE "Sinus"
#define WF_TRI "Dreieck"
#define WF_SQUARE "Rechteck"
//SUS

#define MENU_SUS_1 "1. Rotierende Spule im Magnetfeld"
#define MENU_SUS_2 "2. Elektrokardiogramm"
#define MENU_SUS_3 "3. "
#define MENU_SUS_4 "4. "
#define MENU_SUS_5 "5. "
#define X1X10 "X1/X10"

//SUW
#define MENU_SUW_1 "1. Schallgeschwindigkeit";
#define MENU_SUW_2 "2. ";
#define HIT_IT "Hau drauf!"
#endif


//Titel neu sortieren
#if language == EN
#define MAINMENU_1 "1. Force and motion"
#define MAINMENU_2 "2. Heat and pressure"
#define MAINMENU_3 "3. Light and Shadow"
#define MAINMENU_4 "4. Volage und current"
#define MAINMENU_5 "5. Sound und waves"
#define MAINMENU_6 "6. Sensors und actuators"
#define MAINMENU_7 "7. Miscellaneous"
#define MAINMENU_8 "8. Information"

//Allgemein
#define B_SELECT "Select"
#define SAVE "Save"
#define RESET "Reset"
#define EDIT "Edit"
#define SAVED "Saved"
#define NUM "No."
#define START "Start"
#define STOP "Stop"
#define DELETE "Delete"
#define PAUSE "Pause"
#define MOD_NOT_CONN "Module not connected!"

//toolkit
#define IMG_ERR "Error loading Image!"

//DUW
#define MENU_DUW_1 "1. Measure one temperature value (waterproof)"
#define MENU_DUW_2 "2. Measure two temperature values (waterproof)"
#define MENU_DUW_3 "3. Pressure and temperature"
#define MENU_DUW_4 "4. Pressure in heights"
#define MENU_DUW_5 "5."
#define TIM_IN_S "Time in s"
#define T_IN_C "Temperature in \xF8\x43"
#define T1_IN_C "T1 in \xF8\x43"
#define T2_IN_C "T2 in \xF8\x43"
#define START_10S "Start (10s)"
#define START_1M "Start (1min)"
#define START_5M "Start (5min)"
#define MEASUREMENT "Measurement"
#define START_MEASUREMENT "Start measurement.."

//KBW
#define MENU_KBW_1 "1. Dark time (1LB)";
#define MENU_KBW_2 "2. Dark time (2LB)";
#define MENU_KBW_3 "3. Measuring vehicle";
#define MENU_KBW_4 "4. Timespan (2LB)";
#define MENU_KBW_5 "5. Average speed (2LB)";
#define MENU_KBW_6 "6. Stroboscope photo of a free fall";
#define MENU_KBW_7 "7. Fall time";
#define MENU_KBW_8 "8. Zero gravity during free fall";
#define MENU_KBW_9 "9. Energy conservation in a thread pendulum";
#define MENU_KBW_10 "10. Period of a thread pendulum";
#define MENU_KBW_11 "11. Period of a spring pendulum";
#define MENU_KBW_12 "12. Radial force (graph)";
#define MENU_KBW_13 "13. Radial force (value)";
#define MENU_KBW_14 "14. Ultrasonic measurement of spring pendulum";
#define MENU_KBW_15 "15. Motor synchronizes on spring pendulum";
#define DELTA_H_CM "delta h [cm]"
#define T_IN_MS "t [ms]"
#define T1_IN_MS "t1 [ms]"
#define T2_IN_MS "t2 [ms]"
#define D_IN_M "d [m]"
#define T_IN_S "t [s]"
#define V_IN_MS "v [m/s]"
#define M_PERIOD "Measuring period of pendulum..."
#define U_SONIC "Ultrasonic"
#define Z_R_S "0.0 r/s"
#define R_S_FORMAT "%.1f r/s"
#define A_MS2_FORMAT "%.1f m/s\xFD"
#define TRIGGER "Trigger"
#define F_FALL "free fall"
#define S_STR "s in stripes"
#define V_STR_S "v in str. per sec."
#define T1S "1s"
#define T2S "2s"
#define T5S "5s"
#define T10S "10s"
#define T20S "20s"
#define PLOT_S "Plot s(t)"
#define PLOT_V "Plot v(t)"
#define VALUES "Values"
#define REPEAT "Repeat!"

//KUK
#define MENU_KUK_1 "1. Stop watch"
#define MENU_KUK_2 "2. Timer"
#define SPLIT "Split"
#define SET "Set"

//LUS
// ###

//SUA
#define MENU_SUA_1 "1. Illuminance"
#define MENU_SUA_2 "2. Distance (TOF)"
#define MENU_SUA_3 "3. Temperature and pressure"
#define MENU_SUA_4 "4. Temperature (waterproof)"
#define MENU_SUA_5 "5. Voltage"
#define MENU_SUA_6 "6. Magnetic flux"
#define MENU_SUA_7 "7. Force"
#define MENU_SUA_8 "8. Function generator"
#define SLUX " lux"
#define F_LUX "%d lux"
#define F_TEMP "%.2f %cC"
#define P_TEMP "%.2f Pa"
#define F_MV "%.2f mV"
#define F_GAUSS "%.2f Gauss"
#define F_N "%.2f N"
#define F_HZ "%d Hz"
#define OFFSET "Offset"
#define W_FORM "Waveform"
#define FREQ "Frequency"
#define OFF "Off"
#define Z_HZ "0 Hz"
#define WF_SINE "Sine"
#define WF_TRI "Triangle"
#define WF_SQUARE "Square"
//SUS

#define MENU_SUS_1 "1. Multimeter (coming soon)"
#define MENU_SUS_2 "2. Induction"
#define MENU_SUS_3 "3. "
#define MENU_SUS_4 "4. "
#define MENU_SUS_5 "5. "
//SUW
#define MENU_SUW_1 "1. Speed of sound";
#define MENU_SUW_2 "2. ";
#define HIT_IT "Hit it!"

#endif

#if language == FR
#endif

#if language == ES
#endif