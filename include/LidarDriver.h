//Gruppo: 8bit

#ifndef LIDARDRIVER_H
#define LIDARDRIVER_H

#include <iostream>
#include <vector>

const int BUFFER_DIM = 10;

class LidarDriver
{
public:
    LidarDriver(double ris_ang);
    void new_scan(std::vector<double>);
    std::vector<double> get_scan();
    void clear_buffer();
    double get_distance(double angle) const; 
    void stampaTutto() const; //per il debug

	
    friend std::ostream& operator<<(std::ostream& os, const LidarDriver& lidar);

private:
    std::vector<std::vector<double>> v; // Buffer circolare di vettori
    const double ris_ang;
    int nextInsert; // Posizione del prossimo elemento da inserire
    int numScans; // Numero di scansioni effettivamente inserite
    int numEl; // Numero di elementi pSer ciascuna scansione (derivato da ris_ang)
    int angle_to_index(double angle) const; // Funzione ausiliaria per convertire angolo in indice
};

#endif // LIDARDRIVER_H
