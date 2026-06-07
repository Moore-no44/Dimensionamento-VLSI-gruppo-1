// Dimensionamento VLSI.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>

#include <math.h>

double V_DD = 0.3; // Tensione di alimentazione

double N_inv = 5; // Numero di stadi dell'inverter

double C_L = 0.5e-12; // Capacità di carico

double L_min = 0.18e-6; // Lunghezza minima del canale

double W_min = 0.22e-6; // Larghezza minima del canale

double C_ox = 7e-3; // Capacità di ossido per unità di area

double mu_np = 3; // rapporto tra mobilità elettronica e mobilità di lacune

double mu_n_C_ox = 320e-6; // prodotto tra mobilità elettronica e capacità di ossido per unità di area

double V_th = 0.45; // Tensione di soglia

// double Frequency_target = 1e5; // Frequenza di oscillazione target

int main()
{
	// parametri da dimensionare

	double W_nand_p, W_nand_n, L_nand_p, L_nand_n;

	double W_tbuff_1_p, W_tbuff_1_n, L_tbuff_1_p, L_tbuff_1_n;

	double C_GS_1_tb_n, C_GS_1_tb_p;

	double C_GD_1_tb_n, C_GD_1_tb_p;

	double C_in_tbuff;

	double K_alla_N;

	double N = 0;

	bool N_choosing;

	double K;

	double a;

	double C_DB_inv_n_prev, C_DB_inv_p_prev;
	
	double C_GD_inv_n_prev, C_GD_inv_p_prev;

	double C_GS_inv_n_next, C_GS_inv_p_next;

	double C_line;

	double W_inv_p, W_inv_n, L_inv_p, L_inv_n;

	double C_tot_inv;

	double tao_inv;

	double oscillation_frequency; 

	// double C_tot_new;

	// Calcoli

	W_nand_n = W_min;

	W_nand_p = 2 * W_min / 3;

	L_nand_p = L_nand_n = L_min;

	W_tbuff_1_n = W_min;

	W_tbuff_1_p = 2 * W_min;

	L_tbuff_1_p = L_tbuff_1_n = L_min;

	C_GS_1_tb_p = 2 * C_ox * W_tbuff_1_p * L_tbuff_1_p / 3;

	C_GS_1_tb_n = 2 * C_ox * W_tbuff_1_n * L_tbuff_1_n / 3;

	C_GD_1_tb_p = C_ox * W_tbuff_1_p * L_tbuff_1_p;

	C_GD_1_tb_n = C_ox * W_tbuff_1_n * L_tbuff_1_n;

	C_in_tbuff = C_GS_1_tb_p + C_GS_1_tb_n + 2 * C_GD_1_tb_p + 2 * C_GD_1_tb_n;

	K_alla_N = C_L / C_in_tbuff;

	std::cout << "Scegliere N (0 per N=3, 1 per N=2 se 10 <= K^N <= 100, 0 per N=4, 1 per N=3 se 100 <= K^N <= 1000, 0 per N=5, 1 per N=4 se 1000 <= K^N <= 10000)\n";

	std::cout << "K^N = " << K_alla_N << "\n";

	if (10 <= K_alla_N && K_alla_N <= 100) {
		std::cout << "1 => " << "2," << " 0 => " << "3" << "\n";
		std::cin >> N_choosing;
		if (N_choosing) {
			N = 2;
		}
		else {
			N = 3;
		}
	}
	else if (100 <= K_alla_N && K_alla_N <= 1000) {
		std::cout << "1 => " << "3," << " 0 => " << "4" << "\n";
		std::cin >> N_choosing;
		if (N_choosing) {
			N = 3;
		}
		else {
			N = 4;
		}
	}
	else if (1000 <= K_alla_N && K_alla_N <= 10000) {
		std::cout << "1 => " << "4," << " 0 => " << "5" << "\n";
		std::cin >> N_choosing;
		if (N_choosing) {
			N = 4;
		}
		else {
			N = 5;
		}
	}

	if (N == 0) {

		std::cout << "K^N non è compreso in nessuno degli intervalli, scegliere un altro valore per K^N\n";
		return 0;
	}

	if (pow(K_alla_N, 1 / N) - floor(pow(K_alla_N, 1 / N)) >= 0.5) {
		K = ceil(pow(K_alla_N, 1 / N))-2;
	}
	else {
		K = floor(pow(K_alla_N, 1 / N))-2;
	}

	std::cout << "N scelto: " << N << "\n";
	std::cout << "K imposto: " << K << "\n";

	double Dimensioni[10] = {0};

	for (int i = 0; i < N; i++) {

		Dimensioni[i] = 220e-9 * pow(K, i);
	}

	L_inv_n = L_inv_p = L_min;

	W_inv_n = W_min;

	W_inv_p = 2 * W_min;

	C_GS_inv_n_next = 2 * C_ox * W_inv_n * L_inv_n / 3;

	C_GS_inv_p_next = 2 * C_ox * W_inv_p * L_inv_p / 3;

	C_GD_inv_n_prev = C_ox * W_inv_n * L_inv_n;

	C_GD_inv_p_prev = C_ox * W_inv_p * L_inv_p;

	C_DB_inv_n_prev = 5e-4 * W_inv_n * L_inv_n;

	C_DB_inv_p_prev = 5e-4 * W_inv_p * L_inv_p;

	C_line = 0;

	C_tot_inv = C_DB_inv_n_prev + C_DB_inv_p_prev + 4 * (C_GD_inv_n_prev + C_GD_inv_p_prev) + C_GS_inv_n_next + C_GS_inv_p_next + C_line;

	tao_inv = C_tot_inv * V_DD * L_inv_n / (W_inv_n * mu_n_C_ox * (V_DD - V_th) * (V_DD - V_th));

	oscillation_frequency = 1 / (2 * tao_inv * N_inv);



	// C_tot_new = mu_n_C_ox * W_inv_n * (V_DD - V_th) * (V_DD - V_th) / 2 * N_inv * Frequency_target * V_DD * L_inv_n;

	// C_line = C_tot_new - C_tot_inv;

    // Valori ottenuti

	std::cout << "NAND: W_p = " << W_nand_p << " m, L_p = " << L_nand_p << " m\n";

	std::cout << "C_GS_1_tb_p: " << C_GS_1_tb_p << " F\n";

	std::cout << "C_GS_1_tb_n: " << C_GS_1_tb_n << " F\n";

	std::cout << "C_GD_1_tb_p: " << C_GD_1_tb_p << " F\n";

	std::cout << "C_GD_1_tb_n: " << C_GD_1_tb_n << " F\n";

	std::cout << "C_in_tbuff: " << C_in_tbuff << " F\n";

	std::cout << "K^N: " << K_alla_N << "\n";

	std::cout << "N: " << N << "\n";

	std::cout << "K: " << K << "\n";

	std::cout << "Dimensioni dei transistor del buffer di dimensionamento: \n";
	for (int i = 0; i < N; i++) {
		std::cout << "p: " << i+1 << " => " << Dimensioni[i] * 2.5 << " m\n";
		std::cout << "n: " << i+1 << " => " << Dimensioni[i] << " m\n\n";
	}

	std::cout << "C_tot_inv: " << C_tot_inv << " F\n";

	std::cout << "tao_inv: " << tao_inv << " s\n";

	std::cout << "Oscillation Frequency: " << oscillation_frequency << " Hz\n";

	std::cout << "\n";

	std::cout << "C_GS_inv_p_next: " << C_GS_inv_p_next << " F\n";
	std::cout << "C_GS_inv_n_next: " << C_GS_inv_n_next << " F\n";
	std::cout << "C_GD_inv_p_prev: " << C_GD_inv_p_prev << " F\n";
	std::cout << "C_GD_inv_n_prev: " << C_GD_inv_n_prev << " F\n";
	std::cout << "C_DB_inv_p_prev: " << C_DB_inv_p_prev << " F\n";
	std::cout << "C_DB_inv_n_prev: " << C_DB_inv_n_prev << " F\n";

	// std::cout << "C_line: " << C_line << " F\n";

	// std::cout << "C_tot_new: " << C_tot_new << " F\n";

    std::cout << "Hello World!\n";

}

// Per eseguire il programma: CTRL+F5 oppure Debug > Avvia senza eseguire debug
// Per eseguire il debug del programma: F5 oppure Debug > Avvia debug

// Suggerimenti per iniziare: 
//   1. Usare la finestra Esplora soluzioni per aggiungere/gestire i file
//   2. Usare la finestra Team Explorer per connettersi al controllo del codice sorgente
//   3. Usare la finestra di output per visualizzare l'output di compilazione e altri messaggi
//   4. Usare la finestra Elenco errori per visualizzare gli errori
//   5. Passare a Progetto > Aggiungi nuovo elemento per creare nuovi file di codice oppure a Progetto > Aggiungi elemento esistente per aggiungere file di codice esistenti al progetto
//   6. Per aprire di nuovo questo progetto in futuro, passare a File > Apri > Progetto e selezionare il file con estensione sln
