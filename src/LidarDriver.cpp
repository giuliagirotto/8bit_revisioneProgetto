//Gruppo: 8bit

#include "LidarDriver.h"
#include <iostream>

// Costruttore
LidarDriver::LidarDriver(double ang) : ris_ang{ang}, v(BUFFER_DIM), numEl{static_cast<int>(180/ang+1)}, numScans{0}, nextInsert{0} {}

void LidarDriver::new_scan(std::vector<double> v2)
{
    // Controllo su ultimo insert
    if (nextInsert >= BUFFER_DIM) {
        nextInsert = 0; // Sovrascrivi le vecchie scansioni
    }

    // Controllo che la dimensione sia corretta
    if (v2.size() < numEl) 
    {  // v2 troppo piccolo --> aggiungo 0
        while (v2.size() != numEl) 
        {
            v2.push_back(0);
        }
        v[nextInsert] = v2;
    } 
    else if (v2.size() > numEl) 
    { 
        // Riduce il vettore alla dimensione desiderata
        v2.resize(numEl);

        // Assegna il vettore modificato al buffer
        v[nextInsert] = v2;
    }

    // Se non abbiamo ancora riempito il buffer, incrementiamo il contatore
    if (numScans < BUFFER_DIM) {
        numScans++;
    }

    nextInsert++; // Aggiorna l'indice per il prossimo inserimento
}

std::vector<double> LidarDriver::get_scan()
{
    if (numScans == 0) {
        return {};  // Buffer vuoto, restituisce un vettore vuoto
    }

    // Calcola l'indice della scansione più vecchia (buffer circolare)
    int oldest_index = (nextInsert == 0) ? BUFFER_DIM - 1 : nextInsert - 1;

    // Prendi la scansione più vecchia
    std::vector<double> oldest_scan = v[oldest_index];

    // "Pulisci" lo slot segnandolo come vuoto
    v[oldest_index].clear();

    // Aggiorna l'indice dell'ultimo inserimento
    if (nextInsert == 0) {
        nextInsert = BUFFER_DIM - 1;
    } else {
        nextInsert--;
    }

    // Decrementa il contatore di scansioni
    numScans--;

    return oldest_scan;
}

void LidarDriver::clear_buffer()
{
    // Ripulisce ogni elemento del buffer
    for (auto& scan : v) {
        scan.clear(); // Svuota il vettore interno
    }

    // Resetta i parametri interni
    nextInsert = 0;
    numScans = 0;
}

// Funzione per convertire angolo in indice del ve ttore
int LidarDriver::angle_to_index(double angle) const
{
    // Limita l'angolo al range [0, 180]
    if (angle < 0.0) angle = 0.0;
    if (angle > 180.0) angle = 180.0;

    // Calcola l'indice corrispondente
    int index = static_cast<int>(angle / ris_ang + 0.5); // Arrotonda all'indice più vicino
    return index;
}

// Funzione per ottenere la lettura di un angolo
double LidarDriver::get_distance(double angle) const
{
    if (numScans == 0) 
    {
        std::cerr << "Errore: il buffer è vuoto. Nessuna scansione disponibile.\n";
        return -1; // Valore di errore
    }

    // Ottieni l'indice dell'angolo richiesto
    int index = angle_to_index(angle);

    // Ottieni la scansione più recente
    int oldest_index = (nextInsert == 0) ? BUFFER_DIM - 1 : nextInsert - 1;
    const std::vector<double>& oldest_scan = v[oldest_index];

    // Verifica che l'indice sia valido
    if (index >= 0 && index < oldest_scan.size())
    {
        return oldest_scan[index];
    }
    else 
    {
        std::cerr << "Errore: indice fuori dai limiti. Nessuna lettura disponibile.\n";
        return -1; // Valore di errore
    }
}

// Overloading dell'operatore <<
std::ostream& operator<<(std::ostream& os, const LidarDriver& lidar)
{
    if (lidar.numScans == 0) {
        os << "Buffer vuoto. Nessuna scansione disponibile.";
        return os;
    }

    // Determina l'indice dell'ultima scansione salvata
    int oldest_index = (lidar.nextInsert == 0) ? BUFFER_DIM - 1 : lidar.nextInsert - 1;
    const std::vector<double>& oldest_scan = lidar.v[oldest_index];

    os << "Ultima scansione salvata: ";
    for (double value : oldest_scan) {
        os << value << " ";
    }

    return os;
}

// Funzione di debug per stampare tutto
void LidarDriver::stampaTutto() const
{
    std::cout << "-----------------------------------------------------------------------" << std::endl;

    std::cout << "BUFFER COMPLETO: "<<std::endl;
    for (const auto& row : v) {
        for (double elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Ultimo insert: " << nextInsert << std::endl;
    std::cout << "Numero di scansioni: " << numScans << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

}
