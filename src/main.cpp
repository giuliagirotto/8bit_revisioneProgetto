//Gruppo: 8bit

#include <iostream>
#include <vector>
#include "LidarDriver.h"

// da un numero random
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main()
{
    double ris;
    std::cout << "Inserisci risoluzione angolare: ";
    std::cin >> ris;

    LidarDriver laser1(ris);

    std::vector<double> x(100); // Inizializza il vettore con 10 elementi
    laser1.stampaTutto();
    for (int j = 0; j < 13; j++)
    {
        for (int i = 0; i < 100; i++) {
            x[i] = fRand(0,100);
        }
        laser1.new_scan(x);
    }

    laser1.get_scan();
    laser1.get_scan();
    laser1.get_scan();

    laser1.new_scan(x);
    

    laser1.stampaTutto();

    double angle;
    std::cout << "Inserisci un angolo (in gradi): ";
    std::cin >> angle;

    double distance = laser1.get_distance(angle);
    if (distance != -1) {
        std::cout << "Lettura per angolo " << angle << ": " << distance << std::endl;
    }

    // Stampa il buffer tramite overloading <<
    std::cout << laser1 << std::endl;

    // Svuota il buffer
    laser1.clear_buffer();

    // Stampa il buffer dopo averlo svuotato
    std::cout << "\nBuffer dopo clear_buffer:\n";
    laser1.stampaTutto();


    return 0;
}
