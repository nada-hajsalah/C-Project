
#ifndef TestClassFille_hpp
#define TestClassFille_hpp

#include <stdio.h>
#pragma once

//#include "Generic_station_parser.hpp"
//#include "Generic_connection_parser.hpp"
#include "Generic_mapper.hpp"

using namespace std;

class Mapper : public travel::Generic_mapper{
    private :
    
    
    protected :
    
    virtual void read_stations(const string& _filename) ;
    virtual void read_connections(const string& _filename) ;
    
    //generic mapper virtual functions
    
    virtual vector<pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end);
    
   virtual vector<pair<uint64_t,uint64_t> > compute_travel(const string&, const string&){
        throw("Nothing here");
    }
   virtual vector<pair<uint64_t,uint64_t> > compute_and_display_travel(const string&, const string&){
        throw("Nothing here");
    }
    

    unsigned int get_levenshtein_distance(const string& string_1, const string& string_2);
    uint64_t max_distance(unordered_map<uint64_t,uint64_t> dist);
    
    pair<uint64_t, uint64_t> station_id_from_name(const string& start_station_name,const string& end_station_name);
    void verify_station(uint64_t _id);
    void verify_connection (uint64_t _id);
    
    public :
    
    void run_with_id (const string& stations_filename,const string& connections_filename,uint64_t _start, uint64_t _end);
    
    void run_with_name_string(const string& stations_filename,const string& connections_filename,string& _start_name, string& _end_name);
    virtual vector<pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end) ;



};

#endif /* TestClassFille_hpp */
