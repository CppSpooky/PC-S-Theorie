#pragma once
#include <string>
#include <cmath>
#include <mutex>

long double berechnung(long double T_K, long double T_s, long double E_A, long double H_p, long double m, long double c_E){
	// long double T_K ist die Temperatur des gekühlten Eises
	// long double c_E ist die Wärmekapzität des festen Eises in KJ/kgK

	// Gibt die Differenz zwischen der zur Erwärmung auf 37°C benötigten Energie und der des Brennwerts zurück
	return (m * ((c_E*(T_s - T_K)) + E_A - H_p));
}

void Bild_s_t(Image& image, std::vector<std::vector<long double>>& k, int plot_width, int plot_height, long double T_s, long double E_A, long double H_p, long double m){

	//Berechnet Delta E als einzelner Thread, Bild hat keine Farbabstufungen

	long double C = 0; //Die für die Berechnung verwendete Wärmekapazität des Eises
	long double dE = 0; //Energiedifferenz Delta E
	long double T_K = 0; //Die für die Berechnung verwendete Kühlungstemperatur

	for (int t = 0; t < plot_width; t++) {

		T_K = (static_cast<long double>(t) * 260.15) / 2000; //Die für die Berechnung verwendete Kühlungstemperatur

		for (int c = 0; c < plot_height; c++) {

			C = (static_cast<long double>(c) * 100) / 2000;
			dE = berechnung(T_K, T_s, E_A, H_p, m, C);
			k[t][c] = dE;

			if (dE == 0) {
				image.SetColor(Color(0, 0, 0), t, c);
			}

			else if (dE > 0) {
				image.SetColor(Color(0, 0, 1.0f), t, c);
			}

			else if (dE < 0) {
				image.SetColor(Color(1.0f, 0, 0), t, c);
			}
		}
	}
}

void Bild_m_t(Image& image, std::vector<std::vector<long double>>& k, int plot_height, long double T_s, long double E_A, long double H_p, long double m, int t_range_begin, int t_range_end) {

	//Berechnet Delta E als einzelner Thread, ist aber für den einsatz im Multithreading konzipiert, Bild hat keine Farbabstufungen

	long double C = 0; //Die für die Berechnung verwendete Wärmekapazität des Eises
	long double dE = 0; //Energiedifferenz Delta E
	long double T_K = 0; //Die für die Berechnung verwendete Kühlungstemperatur

	for (int t = t_range_begin; t < t_range_end; t++) {

		T_K = (static_cast<long double>(t) * 260.15) / 2000; // Temperaturintervall

		for (int c = 0; c < plot_height; c++) {

			C = (static_cast<long double>(c) * 100) / 2000; // Intervall der Wärmekapazität
			dE = berechnung(T_K, T_s, E_A, H_p, m, C);
			k[t][c] = dE;

			if (dE == 0) {
				image.SetColor(Color(0, 0, 0), t, c);
			}

			else if (dE > 0) {
				image.SetColor(Color(0, 0, 1.0f), t, c);
			}

			else if (dE < 0) {
				image.SetColor(Color(1.0f, 0, 0), t, c);
			}
		}
	}
}

void Bild_m_t_shaded(Image& image, std::vector<std::vector<long double>>& k, int plot_height, long double T_s, long double E_A, long double H_p, long double m, int t_range_begin, int t_range_end) {

	//Wie Bild_m_t aber mit Farbabstufungen

	long double C = 0; //Die für die Berechnung verwendete Wärmekapazität des Eises
	long double dE = 0; //Energiedifferenz Delta E
	long double T_K = 0; //Die für die Berechnung verwendete Kühlungstemperatur

	for (int t = t_range_begin; t < t_range_end; t++) {

		T_K = (static_cast<long double>(t) * 260.15) / 2000; // Temperaturintervall

		for (int c = 0; c < plot_height; c++) {

			C = (static_cast<long double>(c) * 100) / 2000; // Intervall der Wärmekapazität
			dE = berechnung(T_K, T_s, E_A, H_p, m, C);
			k[t][c] = dE;


			if (static_cast<int>(dE) == 0) {
				image.SetColor(Color(0, 0, 0), t, c);
			}

			else if (dE > 0) {
				image.SetColor(Color(0, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 1.0f), t, c);

			}

			else if (dE < 0) {
				image.SetColor(Color(1.0f, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 0), t, c);

			}
		}
	}
}

void gif_m_t(int start, int end, long double* werte_H_p, long double T_s, long double E_A, long double m) {

	// Wird von gif_main aufgerufen, um viele Bilder mit unterschiedlichen Delta H_p zu erstellen, Version für Multithreading

	Image image(2000, 2000);

	//Dieser Teil ist im wesentlichen Bild_s_t ohne dass die Daten zusätzlich in std::vector<std::vector<long double>>& k geschrieben werden, aber mit Farbe aus Bild_m_t_shaded

	std::string path = "";
	long double C = 0; //Die für die Berechnung verwendete Wärmekapazität
	long double dE = 0; //Energiedifferenz Delta E
	long double T_K = 0; //Die für die Berechnung verwendete Kühlungstemperatur

	for (int i = start; i < end; i++) {

		for (int t = 0; t < 2000; t++) {

			T_K = (static_cast<long double>(t) * 260.15) / 2000;

			for (int c = 0; c < 2000; c++) {

				C = (static_cast<long double>(c) * 100) / 2000;
				dE = berechnung(T_K, T_s, E_A, werte_H_p[i], m, C); 

				if (static_cast<int>(dE) == 0) {
					image.SetColor(Color(0, 0, 0), t, c);
				}

				else if (dE > 0) {
					image.SetColor(Color(0, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 1.0f), t, c);

				}

				else if (dE < 0) {
					image.SetColor(Color(1.0f, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 0), t, c);

				}
			}
		}

		path = "m" + std::to_string(i) + ".bmp";
		image.Export(path.c_str()); //Kovertiere path in C-String / const char *
		std::cout << i << std::endl;

	}
}

void gif_s_t(int lenght, long double* werte_H_p, long double T_s, long double E_A, long double m) {

	// Wird von gif_main aufgerufen, um viele Bilder mit unterschiedlichen Delta H_p zu erstellen, Single-Threaded Version

	Image image(2000, 2000);

	//Dieser Teil ist im wesentlichen Bild_s_t ohne dass die Daten zusätzlich in std::vector<std::vector<long double>>& k geschrieben werden, aber mit Farbe aus Bild_m_t_shaded

	std::string path = "";
	long double C = 0;
	long double dE = 0;
	long double T_K = 0; //Kühltemperartur des Eises in Kelvin

	for (int i = 0; i < lenght; i++) {

		for (int t = 0; t < 2000; t++) {

			T_K = (static_cast<long double>(t) * 260.15) / 2000; //Die für die Berechnung verwendete Kühlungstemperatur

			for (int c = 0; c < 2000; c++) {

				C = (static_cast<long double>(c) * 100) / 2000; //Die für die Berechnung verwendete Wärmekapazität
				dE = berechnung(T_K, T_s, E_A, werte_H_p[i], m, C); //Energiedifferenz Delta E

				if (static_cast<int>(dE) == 0) {
					image.SetColor(Color(0, 0, 0), t, c);
				}

				else if (dE > 0) {
					image.SetColor(Color(0, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 1.0f), t, c);

				}

				else if (dE < 0) {
					image.SetColor(Color(1.0f, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 0), t, c);

				}
			}
		}

		path = "s" + std::to_string(i) + ".bmp";
		image.Export(path.c_str()); //Kovertiere path in C-String / const char*
		std::cout << i << std::endl;

	}
}

void gif_main(long double range[2], int lenght, long double T_s, long double E_A, long double m) {

	// Funktion zur generierung vieler Bilder mit unterschiedlichen Delta H_p

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now(); //Beginne Zeitmessung

	long double* werte_H_p = new long double[lenght]; // Array der Werte von H_p 

	//Generiere Werte für H_p
	for (int i = 0; i < lenght; i++) {
		werte_H_p[i] = (i*(range[1] - range[0])/lenght) + range[0];
	}

	//Multithreading für die Animation

	std::vector<std::thread> t;
	int threads = std::thread::hardware_concurrency();

	//Starte alle Threads, Singlethreading wahrscheinlich sinnvoller bei langsamer Speicherung, da der Bildexport dann deutlich länger dauert als die Rechenzeit
	for (int i = 0; i < threads; i++) {
		t.emplace_back(gif_m_t, (i*lenght) / threads, ((i + 1)*lenght) / threads, werte_H_p, T_s, E_A, m);
	}

	//Joine alle Threads
	for (auto &x : t) {
		x.join();
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto diff = t1 - t0;
	std::cout << "Animation nach " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " ms abgeschlossen\n" << std::endl;

	t0 = std::chrono::high_resolution_clock::now();

	gif_s_t(lenght, werte_H_p, T_s, E_A, m); // Starte Singlethread-Version

	delete[] werte_H_p;

	t1 = std::chrono::high_resolution_clock::now();
	diff = t1 - t0;

	std::cout << "Animation nach " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " ms abgeschlossen\n" << std::endl;
}

bool is_H_empty(std::mutex& mutex_w_H_p, std::vector<long double>& werte_H_p, long double& Wert) {

	std::lock_guard<std::mutex> guard(mutex_w_H_p); // Zugriff auf werte_H_p durch andere Threads einschränken

	if (werte_H_p.empty()) {
		return true;
	}
	else {
		Wert = werte_H_p[0]; // Entnehme Element aus Warteschlange
		werte_H_p.erase(werte_H_p.begin()); // Entferne das entnommene Element aus Warteschlange
		return false;
	}
	
}

void gif_m_t_queue(std::vector<long double>& werte_H_p, int& status, long double T_s, long double E_A, long double m, std::mutex& mutex_status, std::mutex& mutex_w_H_p) {

	// Wird von gif_main_queue aufgerufen, um viele Bilder mit unterschiedlichen Delta H_p zu erstellen, Version für Multithreading

	Image image(2000, 2000);

	//Dieser Teil ist im wesentlichen Bild_s_t ohne dass die Daten zusätzlich in std::vector<std::vector<long double>>& k geschrieben werden, aber mit Farbe aus Bild_m_t_shaded

	std::string path = "";
	long double C = 0; //Die für die Berechnung verwendete Wärmekapazität
	long double dE = 0; //Energiedifferenz Delta E
	long double T_K = 0; //Die für die Berechnung verwendete Kühlungstemperatur
	int i = 0; //Die Anzahl an Bildern, die dieser Tread bereits fertiggestellt hat
	long double Wert = 0; // Wert von H, der für die einzelne Berechnung angenommen wird

	while (!is_H_empty(mutex_w_H_p, werte_H_p, Wert)) {

		for (int t = 0; t < 2000; t++) {

			T_K = (static_cast<long double>(t) * 260.15) / 2000;

			for (int c = 0; c < 2000; c++) {

				C = (static_cast<long double>(c) * 100) / 2000;
				dE = berechnung(T_K, T_s, E_A, Wert, m, C);

				if (static_cast<int>(dE) == 0) {
					image.SetColor(Color(0, 0, 0), t, c);
				}

				else if (dE > 0) {
					image.SetColor(Color(0, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 1.0f), t, c);

				}

				else if (dE < 0) {
					image.SetColor(Color(1.0f, 1.0f - (atanf(static_cast<float>(abs(dE)) / 400.0f) / 1.5f), 0), t, c);

				}
			}
		}

		std::lock_guard<std::mutex> guard(mutex_status);

		path = "qm" + std::to_string(status) + ".bmp";
		image.Export(path.c_str()); //Konvertiere path in C-String / const char *
		i++;
		status++;
		std::cout << "Thread " << std::this_thread::get_id() << ":  Total " << status << "/" << i << " here\n";
	}
}

void gif_main_queue(long double range[2], int lenght, long double T_s, long double E_A, long double m) {

	// analog zu gif_main, aber mit Warteschlange anstatt fest zugeschriebener Intervalle pro Thread

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now(); //Beginne Zeitmessung

	std::vector<long double> werte_H_p;// Array der Werte von H_p 
	std::mutex mutex_w_H_p;
	std::mutex mutex_status;

	//Generiere Werte für H_p
	for (int i = 0; i < lenght; i++) {
		werte_H_p.emplace_back((i*(range[1] - range[0]) / lenght) + range[0]);
	}

	//Multithreading für die Animation

	std::vector<std::thread> t;
	int threads = std::thread::hardware_concurrency();
	int status = 0;

	for (int i = 0; i < threads; i++) {
		t.emplace_back(gif_m_t_queue, std::ref(werte_H_p), std::ref(status), T_s, E_A, m, std::ref(mutex_status), std::ref(mutex_w_H_p));
	}

	//Joine alle Threads
	for (auto &x : t) {
		x.join();
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto diff = t1 - t0;
	std::cout << "Animation nach " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " ms abgeschlossen\n Status: " << status <<  std::endl;
}