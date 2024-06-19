#pragma once

#include "Generic_mapper.hpp"
#include <fstream>
#include <sstream>//lecture de fichier csv

namespace travel {
    class MyStationParser : public Generic_mapper{
    
    public:
        MyStationParser(const std::string& _stations,const std::string& _connections);
        ~MyStationParser() {}
        std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end);
        std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end);
        std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string&, const std::string&);
        std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(const std::string&, const std::string&);
        uint64_t find_station_id_by_name(const std::string& station_name);
    protected:
        void read_stations(const std::string& _filename) override;
        void read_connections(const std::string& _filename) override;

    };
};

