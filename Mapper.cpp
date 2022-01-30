#include "Mapper.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <regex>
#include <sstream>
#include <vector>
#include <queue>
#include <climits>


using namespace std;
//Permet de faire l'ordre du queue, pour que le valeur du cout minimale soit toujours au top 
class Cost_Order{
    public:
        bool operator()(std::pair < uint64_t, uint64_t > s1, std::pair < uint64_t, uint64_t > s2) {
            return s1.second > s2.second;
        }
};

void Mapper::read_stations(const std::string& _filename){
    
    travel::Station station;
    string temp;
    int station_id_int;
    string station_id; 
    ifstream file(_filename);
  
    if(file.good()){

    //ON ne prend pas première ligne qui contient headers 
    getline(file, temp, '\n'); 
    while (getline(file, temp, ',')){
        station.name = temp;
        getline(file, temp, ',');
        station_id = temp; 
        station_id_int = atoi(station_id.c_str());
        getline(file, temp, ',');
        station.line_id = temp;
        getline(file, temp, ',');
        station.address = temp;
        getline(file, temp);
        station.line_name = temp;
        stations_hashmap.insert(make_pair(station_id_int,station));
    }	}
  else{
       throw string("Fichier des stations n'est pas trouvé ");
    }
}


void Mapper::read_connections(const std::string& _filename)
{
    string headers,line;
    int from, to, time;
    ifstream file(_filename);

    string read_line;
    travel::Station connection;



    // read the first line
    if(file.good()){
    getline(file, read_line, '\n');

    //read all lines until end of file
    while (getline(file, read_line, '\n')) {

        std::istringstream temp(read_line);


        getline(temp, line, ',');
        from = std::strtoul(line.c_str(), NULL, 10);


        getline(temp, line, ',');
        to = std::strtoul(line.c_str(), NULL, 10);


        getline(temp, line, '\n');




       time = std::strtoul(line.c_str(), NULL, 10);

        // insert connection sin unordered_map
        connections_hashmap[from][to] = time;
    }
        file.close();
    }

    else{
        throw string("Fichier des connexions n'est pas trouvé ");
    }
}




vector< pair<uint64_t,uint64_t> > Mapper::compute_travel(uint64_t _start, uint64_t _end){
    
   
    unordered_map<uint64_t,uint64_t> dist; //contient l'id de station et le cout du chemin
    std::priority_queue < std::pair < uint64_t, uint64_t > , std::vector < std::pair < uint64_t, uint64_t > > , Cost_Order > prio_queue; // pour stocker les couts des adjacents
    vector< pair<uint64_t,uint64_t> >chemin_vector;  // on va retourner à la fin ce vecteur qui contient le plus cout chemin trouvé 
    unordered_map<uint64_t, uint64_t> path_cost; //contient l'id de station et le cout du chemin
    

    unordered_map<uint64_t, uint64_t> prev_station_path;
 // 1ere étape: initialiser les couts à l'infini et mettre le cout du station de départ à 0 
    for (unordered_map<uint64_t, travel::Station > :: iterator i=stations_hashmap.begin() ; i != stations_hashmap.end() ; ++i){

        path_cost.insert(make_pair(i->first,  INT_MAX));
        dist.insert(make_pair(i->first,  INT_MAX));
    }



    path_cost.at(_start) = 0;
    dist[_start] = 0;
    prio_queue.push(make_pair(_start,0));
    while (!path_cost.empty() ) {

        pair < uint64_t, uint64_t > now= prio_queue.top();
        prio_queue.pop();

       // while (!prio_queue.empty())
    //{
        //prio_queue.pop();
    //}
        

        uint64_t v = now.second; // cost

        uint64_t w = now.first; //  id of station 
        if (w == _end) {
           
            break;
        }

        path_cost.erase(w);

        unordered_map<uint64_t,uint64_t> current_station_connections = connections_hashmap.at(w);
       

         for (unordered_map<uint64_t,uint64_t> :: iterator j=current_station_connections.begin() ; j != current_station_connections.end() ; ++j){
             
            
             if (v + j->second < dist[j->first] && path_cost.count(j->first) != 0)
{
                
                dist[j->first] = v + j->second;
                path_cost.at(j->first)=dist[j->first];
                prev_station_path[j->first] = now.first;
                prio_queue.push(make_pair( j->first, dist[j->first]));
                
                
            }
        }
    }
    uint64_t current_node_in_path = _end;
    
    uint64_t previous_station_in_path = prev_station_path.at(current_node_in_path);
    while (previous_station_in_path != _start) {
        chemin_vector.insert(chemin_vector.begin(), make_pair(current_node_in_path, dist.at(current_node_in_path)));
        current_node_in_path = previous_station_in_path;
        previous_station_in_path = prev_station_path.at(current_node_in_path);
    }
    
    chemin_vector.insert(chemin_vector.begin(), make_pair(current_node_in_path, dist.at(current_node_in_path)));
    chemin_vector.insert(chemin_vector.begin(), make_pair(_start, dist.at(_start)));
    
    return chemin_vector;
}




vector<pair<uint64_t,uint64_t> > Mapper::compute_and_display_travel(uint64_t _start, uint64_t _end){



    bool same_line = false; // True si on est dans le meme ligne
    int time_change = 0; //temps de changement
    int line_time= 0; //temps de voyage sur une ligne
    int path_time = 0;//temps cumulé
    int start_line = 0;// temps de debut ligne
    travel::Station current_station;
    travel::Station previous_station =stations_hashmap.at(_start);

    std::vector<std::pair<uint64_t,uint64_t> > res_vector= compute_travel(_start,_end);
    cout<<"\nLa meilleure façon "<< stations_hashmap.at(_start).name << " (ligne " << stations_hashmap.at(_start).line_id<<")"<<" to "<<stations_hashmap.at(_end).name<<
        " (ligne " << stations_hashmap.at(_end).line_id<<") : \n"<<endl;

    for (vector<pair<uint64_t,uint64_t> > :: const_iterator i = res_vector.begin() ; i != res_vector.end() ; ++i){

        current_station = stations_hashmap.at(i->first);
        if(!(current_station.line_id ==previous_station.line_id))
        {
            time_change = (int)(i->second) - path_time;
            if(same_line){
                line_time= path_time- start_line ;
                cout<<" d'aller à "<< current_station.name;
                cout << "en "<<line_time<<" s"<<endl;
                same_line = false;
            }
            cout<<"Va à ";
            cout<<current_station.name<< ", ligne " << current_station.line_id;
            cout << " avec  temps du trajet :"<< time_change <<" s" <<endl;
        }

        //condition sur le temps permet de ne pas afficher les correpondances à 0, sur le nom de la station d'arrivée de ne pas afficher un dernier "faux" voyage
        if(!( same_line
              ||(i->second - path_time) == 0
              || current_station.name.compare(stations_hashmap.at(_end).name) ==0))
        {
            cout<<"Prends la ligne "<< current_station.line_id<<" "<< current_station.line_name<<endl;
            cout<<"A partir de "<<previous_station.name;
            same_line=true;
            start_line = path_time + time_change;
        }

        path_time = (int) i->second;
        previous_station= current_station;
    }

    if(same_line){
        cout<<" to "<< current_station.name;
        line_time= path_time - start_line;
        cout << " avec temps du trajet:"<<line_time<<" s"<<endl;
    }
    cout<<"******Temps pour arriver au destination ****** \n"<<endl;
    cout <<"\n Temps: "<<path_time<<" secondes" <<endl;
    return res_vector;
}

uint64_t Mapper::max_distance(unordered_map<uint64_t,uint64_t> dist) {

    // Initialiser value

    


    uint64_t distance;
    unordered_map<uint64_t, uint64_t> :: iterator d=dist.begin();
    uint64_t  dist_max=d->second;
    uint64_t max_index;



    for (unordered_map<uint64_t, uint64_t> :: iterator i=dist.begin() ; i != dist.end() ; ++i){
        distance=i->second;





        if(distance<dist_max){
            dist_max=distance;
            max_index = i->first;



        }
    }
// fixer un seuil , pour lequel on rejette le matching des noms
        if (dist_max>10){max_index=-1; }





//

    return max_index;

}


    pair<uint64_t, uint64_t> Mapper::station_id_from_name(const string& start_station_name,const string& end_station_name){


        unordered_map<uint64_t,uint64_t> dist_start;
        unordered_map<uint64_t,uint64_t> dist_end;

        pair<uint64_t, uint64_t> id_stat;

        travel::Station current_station;
        uint64_t id_station_end_max;

        uint64_t  id_station_start_max;

        uint64_t distance;

        string start_station_name_copy = start_station_name;

        string end_station_name_copy = end_station_name;




        //passe toutes les lettres en minuscule

        transform(start_station_name_copy.begin(), start_station_name_copy.end(), start_station_name_copy.begin(), ::tolower);

        transform(end_station_name_copy.begin(), end_station_name_copy.end(), end_station_name_copy.begin(), ::tolower);



        for (unordered_map<uint64_t, travel::Station > :: iterator i=stations_hashmap.begin() ; i != stations_hashmap.end() ; ++i){

            current_station = i->second;



            string current_station_name_copy = current_station.name;

            transform(current_station_name_copy.begin(), current_station_name_copy.end(), current_station_name_copy.begin(), ::tolower);

            int pos = (int)current_station_name_copy.find('(');
            string current_station_name = current_station_name_copy.substr(0,pos);
            distance = get_levenshtein_distance(current_station_name, start_station_name_copy);
            dist_start.insert(make_pair(i->first,distance));

            distance = get_levenshtein_distance(current_station_name, end_station_name_copy);
            dist_end.insert(make_pair(i->first,distance));
            id_station_start_max=max_distance(dist_start);
            id_station_end_max=max_distance(dist_end);

        }
            // si on ne trouve pas les noms des stations, on afficher un erreur
            if((int) id_station_start_max == -1){

                throw string(" Matching de première station non trouvée, Veuillez réessayer s’il vous plaît  ");

            }else {  cout<<" Meilleur matching de  "<< start_station_name<<" est "<<stations_hashmap.at(id_station_start_max).name<<endl;
            }

            if((int)id_station_end_max == -1){

                throw string(" Matching de  station finale  non trouvée, Veuillez réessayer s’il vous plaît ");  } else {
                    cout <<" Meilleur matching de  "<< end_station_name <<" est "<< stations_hashmap.at(id_station_end_max).name<<endl;

                }



                id_stat.first = id_station_start_max;

                id_stat.second = id_station_end_max;



                return id_stat;

            }

unsigned int Mapper::get_levenshtein_distance(const string& string_1, const string& string_2){
    //cette fonction permet de calculer la distance de Levenshtein
     const size_t size_1 = string_1.size();
    const size_t size_2= string_2.size();

      if (size_1 == 0) return size_2;
       if (size_2 == 0) return size_1;
    
    vector<vector<int> > distance(size_1 + 1, vector<int>(size_2 + 1));
    
    distance[0][0] = 0;
    for(int i = 1; i <= (int)size_1; ++i) 
       {distance[i][0] = i;}
    for(int i = 1; i <= (int)size_2; ++i)
        {distance[0][i] = i;}
    
    for(int i = 1; i <= (int)size_1; ++i)
        for(int j = 1; j <= (int)size_2; ++j)
            {distance[i][j] =  min( min(distance[i - 1][j] + 1, distance[i][j - 1] + 1), distance[i - 1][j - 1] + (string_1[i - 1] == string_2[j - 1] ? 0 : 1)) ;}
    
    return distance[size_1][size_2];
}





void Mapper::run_with_name_string(const string& stations_filename,const string& connections_filename,string& _start, string& _end){
    
    vector<pair<uint64_t,uint64_t> > res_vector;
    pair<uint64_t, std::uint64_t> id_stations;
    
    read_stations(stations_filename);
    
    read_connections(connections_filename);

    
    id_stations = station_id_from_name(_start, _end);
    
    res_vector = compute_and_display_travel(id_stations.first, id_stations.second);

}
void Mapper::verify_station(uint64_t _id){ if(stations_hashmap.find(_id) == stations_hashmap.end() ){
        throw string("station not found in stations file.");
    } }
void Mapper::verify_connection(uint64_t _id){ if(connections_hashmap.find(_id) == connections_hashmap.end() ){
            throw string("Start station not found in stations file.");
        }  }
        void Mapper::run_with_id(const string& stations_filename, const string& connections_filename, uint64_t _start, uint64_t _end){
    
    vector<pair<uint64_t,uint64_t> > res_vector;
    
    read_stations(stations_filename);
    verify_station(_start);
    verify_station(_end);

    read_connections(connections_filename);

            verify_connection(_start);
            verify_connection(_end);

    res_vector = compute_and_display_travel(_start, _end);
}
