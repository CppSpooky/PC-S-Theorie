#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "graphics.hpp"
#include "math.hpp"

int main() {
	
	long double T_s = 260.15; // -13°C in Kelvin
	long double T_K = 0; //Kühltemperartur des Eises in Kelvin
	long double E_A = 372; // Aufwärmenergie KJ/Kg
	long double H_p = 8360; // Brennwert in KJ/kg
	long double m = 0.2; // Masse des Eises in kg

	std::vector<std::vector<long double>> k(2000, std::vector<long double>(2000, 0)); //Erstelle 2D-Vector

	int plot_width = 2000; // Sollte durch Threadzahl teilbar sein
	int plot_height = 2000;

	//Beginne Zeitmessung
	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	Image image(plot_width, plot_height);

	int threads = std::thread::hardware_concurrency();
	std::vector<std::thread> v;

	//Starte alle Threads
	for (int i = 0; i < threads; i++) {
		v.emplace_back(Bild_m_t_shaded, std::ref(image), std::ref(k), plot_height, T_s, E_A, H_p, m, (i*plot_width) / threads, ((i + 1)*plot_width) / threads);
		std::cout << "Thread "<<v[i].get_id() << "\t Intervall " << (i*plot_width) / threads << " - " << ((i + 1)*plot_width) / threads << std::endl;
	}

	//Warte auf die Threads
	for (auto& t : v) {
		t.join();
	}
	
	auto t1 = std::chrono::high_resolution_clock::now();
	auto diff = t1 - t0;

	std::cout << "Multithreading nach " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " ms abgeschlossen\n" << std::endl;

	image.Export("plot_mt_shaded.bmp");	
	
	//#error Warnung: Der folgende Teil kann sehr große Datenmengen generieren. Präprozessoranweisung entfernen um fortzufahren!

	long double range[2] = {0, 10000}; // Intervall von H_p für die Animation
	int lenght = 8; //Anzahl der erzeugten Bilder, muss durch Threadzahl teilbar sein
	gif_main(range, lenght, T_s, E_A, m);

	Bild_s_t(image, k, plot_width, plot_height, T_s, E_A, H_p, m);
	image.Export("plot_s_t.bmp");

	return 0;
}