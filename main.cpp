
#include <iostream>
#include <fstream>
#include "Mapper.hpp"
#include <regex>

using namespace std;

int main(int argc, const char * argv[]) {

    int start_station_id, end_station_id;
    string stations_file, connections_file;
    string exec;
try{

    string start_station_string, end_station_string;
    std::cout << "**** Meuilleur chemin  **** \n" << std::endl;



        if ( argc != 6 ) {
        std::cerr<<"Ressayer svp : Stations.csv Connections.csv Id_Start Id_End "<<std::endl;
        exit(1);
    }else{
        Mapper network;

        stations_file = argv[1];

        connections_file= argv[2];
        
        exec= argv[3];
        
        if(exec.compare("By_ID")==0){
            start_station_id  = std::strtoul(argv[4], NULL, 10); 	 
            end_station_id= std::strtoul(argv[5], NULL, 10);
           
             network.run_with_id(stations_file, connections_file, start_station_id, end_station_id);
        }else{
            if(exec.compare("By_Name")==0){
                start_station_string = argv[4];
                end_station_string = argv[5];
                network.run_with_name_string(stations_file, connections_file, start_station_string, end_station_string);
            }else{
                cerr<<"Ressayer svp, Parametre non valide : By_ID ou By_Name"<<endl;
                exit(1);
            }
        }
    }


 }
    catch(string const& erreur) {
        cerr << "Error : "  <<erreur<< endl;
        exit(1);
    }
    catch(out_of_range const&) {
        cerr << "Error : out of index"  << endl ;
        exit(1);
    }


    return 0;
}
