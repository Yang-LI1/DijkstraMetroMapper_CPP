#include "MyStationParser.h"
#include <queue>
#include <algorithm>

// Implémentation du constructeur de la classe MyStationParser
travel::MyStationParser::MyStationParser(const std::string& _stations,const std::string& _connections) {
            read_stations(_stations); // Lire les stations depuis le fichier
            read_connections(_connections); // Lire les stations depuis le fichier
        }




 // Lire les stations depuis le fichier
void travel::MyStationParser::read_stations(const std::string& _filename)
{
    // Ouvrir le fichier
    std::ifstream file(_filename);
    std::string line;
    if (file.is_open()) {
        std::getline(file, line); //Ignorer la première ligne
        while (std::getline(file, line)) { //boucle pour lecture de chaque ligne
            std::stringstream ss(line);
            std::string name, station_id,line_id, address, line_name;
            // Extraire les informations de la station en utilisant la virgule comme séparateur
            std::getline(ss, name, ',');//separer avec le virgule
            std::getline(ss, station_id, ',');
            std::getline(ss, line_id, ',');
            std::getline(ss, address, ',');
            std::getline(ss, line_name, ',');
            ss.ignore();
            Station station = { name, line_id, address, line_name };//construire la station
            // Ajouter la station au hashmap avec son identifiant comme clé
            stations_hashmap.emplace(std::stoull(station_id), station);
        
        }
    }
}






// Ajouter la station au hashmap avec son identifiant comme clé
void travel::MyStationParser::read_connections(const std::string& _filename)
{   // Ouvrir le fichier
    std::ifstream file(_filename);
    std::string line;
    if (file.is_open()) {
        std::getline(file, line); //Ignorer la première ligne
        while (std::getline(file, line)) { // boucle pour lecture de chaque ligne
            std::stringstream ss(line);
            uint64_t start_id, end_id, travel_time;
            // Extraire les informations de la connection en utilisant la virgule comme séparateur
            char delimiter = ',';
            // lire les 3 nombres sur chaque ligne
            ss >> start_id >> delimiter >> end_id >> delimiter >> travel_time; 
            connections_hashmap[start_id][end_id] = travel_time; // ajoute la connexion dans la hashmap
            //start stop, end stop comme keys de hashmap
            //transfer_time comme valeur de hashmap
        }
    }
}



class Compare
{
public:
    bool operator()(std::pair<int,int> a,std::pair<int,int> b)
    {
        return a.second>b.second;
    }
};//Classe pour comparer les paires en se basant sur le deuxième élément (coût), utile dans la méthode compute_travel


   
// Méthode pour calculer le chemin le plus rapide entre deux stations
std::vector<std::pair<uint64_t, uint64_t>> travel::MyStationParser::compute_travel(uint64_t _start, uint64_t _end){
    try {
        std::vector<std::pair<uint64_t, uint64_t>> travel_path; // Vecteur pour stocker le chemin et le coût de chaque étape
        std::unordered_map<uint64_t, uint64_t> visited_stations; // HashMap pour stocker les stations visitées et leur coût minimal jusqu'à cette station
        std::unordered_map<uint64_t, uint64_t> prev_stations; // HashMap pour stocker les stations précédentes dans le chemin
        std::priority_queue<std::pair<uint64_t, uint64_t>, std::vector<std::pair<uint64_t, uint64_t>>, Compare> pq;
        // File de priorité pour stocker les stations à visiter

        // Vérifier si les stations de départ et d'arrivée sont valides
        if (stations_hashmap.find(_start) == stations_hashmap.end() || stations_hashmap.find(_end) == stations_hashmap.end()) {
            throw std::runtime_error("La station de départ ou d'arrivée est invalide.");
        }
        
        pq.push(std::make_pair(_start, 0)); // Initialiser la file de priorité avec la station de départ
        visited_stations[_start] = 0; // Initialiser la distance de la station de départ à 0

        // Tant que la file de priorité n'est pas vide
        while (!pq.empty()) {
        uint64_t current_station_id = pq.top().first;
        uint64_t current_station_cost = pq.top().second;
        pq.pop();

        // Si la station actuelle est la station d'arrivée
        if (current_station_id == _end) {
            // Reconstruire le chemin
            uint64_t prev_station_id = prev_stations[current_station_id];
            travel_path.emplace_back(std::make_pair(current_station_id, connections_hashmap[prev_station_id][current_station_id]));
            while (prev_station_id != _start) {
                    prev_station_id = prev_stations[prev_station_id];
                    current_station_id = prev_stations[current_station_id];
                    travel_path.emplace_back(std::make_pair(current_station_id, connections_hashmap[prev_station_id][current_station_id]));
                }
                // Ajouter la station de départ au chemin
                travel_path.emplace_back(std::make_pair(_start, 0));
                break;
            }

        // Parcourir toutes les connexions de la station actuelle
        for (const auto& conn : connections_hashmap[current_station_id]) {
            uint64_t next_station_id = conn.first;
            uint64_t next_station_cost = current_station_cost + conn.second;

            // Si la station suivante n'a pas encore été visitée ou si le nouveau coût est inférieur au coût précédent
            if (visited_stations.find(next_station_id) == visited_stations.end() || next_station_cost < visited_stations[next_station_id]) {
                // Mettre à jour le coût de la station suivante
                visited_stations[next_station_id] = next_station_cost;
                // Mettre à jour la station précédente de la station suivante
                prev_stations[next_station_id] = current_station_id;
                // Ajouter la station suivante dans la file de priorité
                pq.push(std::make_pair(next_station_id, next_station_cost));
            }
        }
        }
            // Inverser l'ordre du chemin pour le rendre correct
            std::reverse(travel_path.begin(), travel_path.end());
            return travel_path;
        } 

        catch (const std::runtime_error &e) {
            std::cerr << "Erreur: " << e.what() << std::endl;
            return std::vector<std::pair<uint64_t, uint64_t>>(); // Retourner un vecteur vide en cas d'erreur
        }
}




// Méthode pour calculer et afficher le chemin le plus rapide entre deux stations et afficher
std::vector<std::pair<uint64_t, uint64_t>> travel::MyStationParser::compute_and_display_travel(uint64_t _start, uint64_t _end)  {
    // Calculer le chemin le plus rapide
    std::vector<std::pair<uint64_t, uint64_t>> travel_path = compute_travel(_start, _end);
    // Initialiser le temps total pour la trajectoire
    uint64_t total_time = 0;
    // Si le chemin n'est pas vide
    if (!travel_path.empty()) {
        // Afficher les informations de départ et d'arrivée
        std::cout << "Best way from " << stations_hashmap[_start].name << " (line " << stations_hashmap[_start].line_id << ") to "
                  << stations_hashmap[_end].name << " (line " << stations_hashmap[_end].line_id << ") is:" << std::endl;

        uint64_t previous_station_id = _start;
        uint64_t line_start_id = _start;
        uint64_t line_start_time = 0;
        
        // Parcourir le chemin
        for (size_t i = 1; i < travel_path.size(); ++i) {
            uint64_t current_station_id = travel_path[i].first;
            uint64_t current_travel_time = travel_path[i].second;
            total_time += current_travel_time;

            // Si la ligne de la station actuelle est différente de la ligne de la station précédente
            // changer la ligne
            if (stations_hashmap[current_station_id].line_id != stations_hashmap[previous_station_id].line_id) {
                if (i != 1) {
                    //calculer le temps sur cette ligne
                    uint64_t travel_time_on_line = total_time - line_start_time - current_travel_time;
                    //si travel_time_on_line = 0, c'est changer la ligne dans la meme station
                    //donc il n'y a pas besoin de dire 'from ... to' 
                    if (travel_time_on_line > 0) {
                        std::cout << "From" << stations_hashmap[line_start_id].name << " to " << stations_hashmap[previous_station_id].name << " (" << travel_time_on_line << " s)" << std::endl;
                    }
                }
                if (current_travel_time > 0) {
                    std::cout << "Walk to " << stations_hashmap[current_station_id].name << ", line " << stations_hashmap[current_station_id].line_id << " (" << current_travel_time << " s)" << std::endl;
                    std::cout << "Take line " << stations_hashmap[current_station_id].line_id << stations_hashmap[current_station_id].line_name << std::endl;
                }

                // Mettre à jour les informations de la ligne
                line_start_id = current_station_id;
                line_start_time = total_time;
            }
            // Mettre à jour la station précédente
            previous_station_id = current_station_id;
        }
        // Afficher le dernier trajet sur la ligne
        uint64_t last_line_travel_time = total_time - line_start_time;
        if (last_line_travel_time > 0) {
            std::cout << "From " << stations_hashmap[line_start_id].name << " to " << stations_hashmap[_end].name << " (" << last_line_travel_time << " s)" << std::endl;
        }
        
        //Afficher le temps total pour atteindre la destination
        std::cout << std::endl << "After " << total_time << " secs, you have reached your destination!" << std::endl;
    } else {
        std::cout << "No valid path found." << std::endl;
    }
    return travel_path;
}





// Méthode pour trouver l'ID d'une station à partir de son nom et ligne (nom,ligne)
uint64_t travel::MyStationParser::find_station_id_by_name(const std::string& station_name_and_line) {
    //Diviser la chaîne d'entrée en noms de stations et en lignes de métro en utilisant des virgules.
    size_t pos = station_name_and_line.find(",");
    std::string station_name = station_name_and_line.substr(0, pos);
    std::string line_id = station_name_and_line.substr(pos + 1);

    for (const auto& station : stations_hashmap) {
        if (station.second.name == station_name && station.second.line_id == line_id) {
            return station.first;
        }
    }
    // Si la station n'est pas trouvée, lancer une exception
    throw std::runtime_error("Station name or line not found.");
}



// Surcharge de la méthode compute_travel pour accepter les noms des stations
std::vector<std::pair<uint64_t, uint64_t>> travel::MyStationParser::compute_travel(const std::string& start_name, const std::string& end_name) {
    try {
        uint64_t start_id = find_station_id_by_name(start_name);
        uint64_t end_id = find_station_id_by_name(end_name);
        return compute_travel(start_id, end_id);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return std::vector<std::pair<uint64_t, uint64_t>>();
    }
}





// Surcharge de la méthode compute_and_display_travel pour accepter les noms des stations
std::vector<std::pair<uint64_t, uint64_t>> travel::MyStationParser::compute_and_display_travel(const std::string& start_name, const std::string& end_name) {
    try {
        uint64_t start_id = find_station_id_by_name(start_name);
        uint64_t end_id = find_station_id_by_name(end_name);
        return compute_and_display_travel(start_id, end_id);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return std::vector<std::pair<uint64_t, uint64_t>>();
    }
}
