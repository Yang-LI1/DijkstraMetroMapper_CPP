#include "MyStationParser.h"
#include "Grade.hpp"



int main() {
    travel::MyStationParser parser("./data/s.csv","./data/c.csv");//instancier mon class
    travel::evaluate_small.stations(parser);//utliser l’objet statique evaluate_small et appeler la méthode stations
    travel::evaluate_small.connections(parser);//utliser l’objet statique evaluate_small et appeler la méthode connections
    std::cout <<"Test 1 : compute_and_display_travel avec station id"<< std::endl;
    parser.compute_and_display_travel(1722,2062);
    std::cout<< std::endl;

    std::cout <<"Test 2 : gestion d'exception pour l'entrée identifiant de la station"<< std::endl;
    parser.compute_and_display_travel(17,2062);
    std::cout<< std::endl;
    

    std::cout <<"Test 3 : compute_and_display_travel avec nom de la station"<< std::endl;
    parser.compute_and_display_travel("Saint-Lazare,3","Bastille,1");
    std::cout<< std::endl;

    std::cout <<"Test 4 : gestion d'exception pour l'entrée nom de la station"<< std::endl;
    parser.compute_and_display_travel("Saint-Laz","Bastille");
    return 0;
}
