// Map.cpp
#include "Map.h"
#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <queue>


using namespace std;

// ==================== Territory Implementation ====================

Territory::Territory() {
    name = new string("");
    owner = nullptr;
    armies = new int(0);
    continent = nullptr;
    adjacentTerritories  = new vector<Territory*>();
}

Territory::Territory(const string& territoryName){
    name = new string(territoryName);
    owner = nullptr;
    armies = new int(0);
    continent = nullptr;
    adjacentTerritories = new vector<Territory*>();
}

Territory::Territory(const Territory& other){
    name = new string(*other.name);
    owner = other.owner;
    armies = new int(*other.armies);
    continent = other.continent;
    adjacentTerritories = new vector<Territory*>(*other.adjacentTerritories);
}

Territory::~Territory(){
    delete name;
    delete armies;
    delete adjacentTerritories;
    name = nullptr;
    armies = nullptr;
    adjacentTerritories = nullptr;

}

// Assignment operator
Territory& Territory::operator=(const Territory& other) {
    if (this != &other) {
        delete name;
        delete armies;
        delete adjacentTerritories;
        
        name = new string(*other.name);
        owner = other.owner;
        armies = new int(*other.armies);
        continent = other.continent;
        adjacentTerritories = new vector<Territory*>(*other.adjacentTerritories);
    }
    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& os, const Territory& territory) {
    os << "Territory: " << *territory.name 
       << ", Armies: " << *territory.armies
       << ", Adjacent to " << territory.adjacentTerritories->size() << " territories";
    return os;
}

// Getters (Territory)
string Territory::getName() const {
    return *name;
}

Player* Territory::getOwner() const {
    return owner;
}

int Territory::getArmies() const {
    return *armies;
}

Continent* Territory::getContinent() const {
    return continent;
}

vector<Territory*>* Territory::getAdjacentTerritories() const{
    return adjacentTerritories;
}

// Setters(Territory)
void Territory::setName(const string& territoryName) {
    *name = territoryName;
}

void Territory::setOwner(Player* newOwner){
    owner = newOwner;
}

void Territory::setArmies(int newArmies){
    *armies = newArmies;
}

void Territory::setContinent(Continent* newContinent){
    continent = newContinent;
}

// Territory operations
void Territory::addAdjacentTerritory(Territory* territory){
    if (territory != nullptr && territory != this) {
        // Checks that the pointer is not nullptr and not the same as current territory (to avoid self-loops)
        // Adds the pointer territory to the end of the container adjacentTerritories
        adjacentTerritories->push_back(territory);
    }
}
bool Territory::isAdjacent(Territory* territory) const {
    if (territory == nullptr) return false;

    for (Territory* t : *adjacentTerritories) {
        if (t == territory) {
            return true;
        }
    }
    return false;
}

// ==================== Continent Implementation ====================

// Constructors(Continent)
Continent::Continent() {
    name = new string("");
    bonusArmies = new int(0);
    territories = new vector<Territory*>();
}

Continent::Continent(const string& continentName, int bonus) {
    name = new string(continentName);
    bonusArmies = new int(bonus);
    territories = new vector<Territory*>();
}

Continent::Continent(const Continent& other) {
    name = new string(*other.name);
    bonusArmies = new int(*other.bonusArmies);
    territories = new vector<Territory*>(*other.territories);
}

// Destructor(Continent)
Continent::~Continent() {
    delete name;
    delete bonusArmies;
    delete territories;
    name = nullptr;
    bonusArmies = nullptr;
    territories = nullptr;
}

// Assignment operator (Continent)
Continent& Continent::operator=(const Continent& other) {
    if (this != &other) {
        delete name;
        delete bonusArmies;
        delete territories;
        
        name = new string(*other.name);
        bonusArmies = new int(*other.bonusArmies);
        territories = new vector<Territory*>(*other.territories);
    }
    return *this;
}

// Stream inserion operator (Continent)
ostream& operator<<(ostream& os, const Continent& continent) {
    os << "Continent: " << *continent.name 
       << ", Bonus: " << *continent.bonusArmies
       << ", Territories: " << continent.territories->size();
    return os;
}

// Getters (Continent)
string Continent::getName() const{
    return *name;
}
int Continent::getBonusArmies() const{
    return *bonusArmies;
}
vector<Territory*>* Continent::getTerritories() const{
    return territories;
}

// Setters (Continent)
void Continent::setName(const string& continentName){
    *name = continentName;
}
void Continent::setBonusArmies(int bonus){
    *bonusArmies = bonus;
}

// Continent operations
void Continent::addTerritory(Territory* territory){
    if (territory != nullptr) {
        territories->push_back(territory);
        territory->setContinent(this); 
    }
}

// Check if the continent is a connected subgraph (for areContinentsConnected)
bool Continent::isContinentConnected() const{
    if (territories->empty()) return true; // An empty continent is considered connected
    if (territories->size() == 1) return true; // A single territory is considered connected

    // Use Breadth-First Search (BFS) to check connectivity

    set<Territory*> visited; // To track visited territories
    queue<Territory*> IsConnectedQueue; // BFS queue to process territories

    IsConnectedQueue.push(territories->at(0)); // Start BFS from the first territory
    visited.insert(territories->at(0)); // Mark the starting territory as visited

    while (!IsConnectedQueue.empty()) {
        Territory* current = IsConnectedQueue.front();
        IsConnectedQueue.pop();

        for (Territory* adjacent : *current->getAdjacentTerritories()) {
            // Check if adjacent is part of the continent
            bool isInContinent = false;
            // Range-based for loop to check if adjacent territory is in the continent's territories
            for (Territory* adjacentTerritory : *territories) {
                if (adjacentTerritory == adjacent) {
                    isInContinent = true;
                    break;
                }
            }

            // If adjacent is in continent and not visited yet
            // isInContinent is false (means the territory is not in the continent), then not add the adjacent territory to the set (visited).
            if (isInContinent && visited.count(adjacent) == 0) {
                visited.insert(adjacent);
                IsConnectedQueue.push(adjacent);
            }
        }
    }
    
    // Check if the number of visited territories is equal to the total number of territories
    // If so, the continent is connected
    // If not, it is disconnected
    return visited.size() == territories->size();
}
    
// ==================== Map Implementation ====================

// Constructors (Map)
Map::Map(){
    territories = new vector<Territory*>();
    continents = new vector<Continent*>();
}
Map::Map(const Map& other){
    territories = new vector<Territory*>();
    continents = new vector<Continent*>();

    // Deep copy territories
    for (Territory* territory : *other.territories) {
        territories->push_back(new Territory(*territory));
    }
    
    // Deep copy continents
    for (Continent* continent : *other.continents) {
        continents->push_back(new Continent(*continent));
    }
}

//Destructor (Map)
Map::~Map(){
    // Delete all territories
    for (Territory* territory : *territories) {
        delete territory;
    }
    delete territories;
    
    // Delete all continents
    for (Continent* continent : *continents) {
        delete continent;
    }
    delete continents;
    
    territories = nullptr;
    continents = nullptr;
}

// Assignment operator (Map)
Map& Map::operator=(const Map& other){
    if (this != &other) {

            // Clean up existing data of (territories & continents)
            for (Territory* territory : *territories) {
                delete territory;
            }
            territories->clear();
            
            for (Continent* continent : *continents) {
                delete continent;
            }
            continents->clear();
            
            // Deep copy territories
            for (Territory* territory : *other.territories) {
                territories->push_back(new Territory(*territory));
            }
            
            // Deep copy continents
            for (Continent* continent : *other.continents) {
                continents->push_back(new Continent(*continent));
            }
        }
        return *this;
}

// Stream insertion operator (Map)
ostream& operator<<(ostream& os, const Map& map){
    os << "Map with " << map.territories->size() << " territories and " 
       << map.continents->size() << " continents";
    return os;
}

// Getters
vector<Territory*>* Map::getTerritories() const{
    return territories;
}
vector<Continent*>* Map::getContinents() const{
    return continents;
}

bool Map::getisTerritoryIsContinent() const{
    return isTerritoryIsContinent;
}

// Setters
void Map::setisTerritoryIsContinent(bool value){
    isTerritoryIsContinent = value;
}

// Map operations
void Map::addTerritory(Territory* territory){
    if (territory != nullptr) {
            territories->push_back(territory);
    }
}
void Map::addContinent(Continent* continent){
    if (continent != nullptr) {
        continents->push_back(continent);
    }
}
Territory* Map::getTerritory(const string& territoryName) const{
    for (Territory* territory : *territories) {
        if (territory->getName() == territoryName) {
            return territory;
        }
    }
    return nullptr;
}
Continent* Map::getContinent(const string& continentName) const{
    for (Continent* continent : *continents) {
        if (continent->getName() == continentName) {
            return continent;
        }
    }
    return nullptr;
}

// Validation methods
// Validate the map based on three criteria
// 1) the map is a connected graph, 
// 2) continents are connected subgraphs 
// 3) each country belongs to one and only one continent. 
bool Map::validate() const{
    return isConnectedGraph() && areContinentsConnected() && isEachTerritoryInOneContinent();
}

// 1)Check if the entire map is a connected graph
bool Map::isConnectedGraph() const{
    if (territories->empty()) {
        cout<<"Warning: The map has no territories."<<endl;
        return true; // An empty map is considered connected
    }
    if (territories->size() == 1) {
        cout<<"Warning: The map has only one territory."<<endl;
        return true; // A single territory is considered connected
    }

    set<Territory*> visited; // To track visited territories
    queue<Territory*> IsConnectedGraphQueue; // BFS queue to process territories
    
    // Start BFS from the first territory
    IsConnectedGraphQueue.push(territories->at(0));
    // Mark the starting territory as visited
    visited.insert(territories->at(0));
    
    
    while (!IsConnectedGraphQueue.empty()) {
        // Get the next territory to explore, which is at the front of the queue
        Territory* current = IsConnectedGraphQueue.front();
        IsConnectedGraphQueue.pop();

        // Range-based for loop to iterate through each adjacent territory
        for (Territory* adjacent : *current->getAdjacentTerritories()) {
            // If adjacent territory has not been visited yet
            // 0 means the territory is not in the set (it has not been visited).
            // 1 means the territory is in the set (it has been visited).
            if (visited.count(adjacent) == 0) {
                // Mark it as visited and add to the queue
                visited.insert(adjacent);
                IsConnectedGraphQueue.push(adjacent);
            }
        }
    }
    // If the number of visited territories is equal to the total number of territories, the map is connected
    return visited.size() == territories->size();
}

// 2) Check if each continent is a connected subgraph
bool Map::areContinentsConnected() const{
    for (Continent* continent : *continents) {
        // Use the isContinentConnected function of Continent to check if it's a connected subgraph
        if (!continent->isContinentConnected()) {
            return false;
        }
    }
    return true;
} 
// 3) Check that each territory belongs to exactly one continent
bool Map::isEachTerritoryInOneContinent() const{
    set<Territory*> territorySet; 
    
    // Check that each territory belongs to exactly one continent
    for (Continent* continent : *continents) {
        for (Territory* territory : *continent->getTerritories()) {
            if (territorySet.count(territory) > 0) {
                // Territory found in multiple continents
                return false;
            }
            territorySet.insert(territory);
        }
    }
    
    // Check that all territories belong to a continent 
    // and see whether territory is same name as a continent (Ensure that each territory belongs to exactly one continent)
    // getisTerritoryIsContinent() returns false if a territory has the same name as a continent (Mean territory belongs to more than one continent)
    return territorySet.size() == territories->size() && getisTerritoryIsContinent();
}

// distribute territories to players (for game setup - Ass2)
void Map::distributeTerritories(vector<Player*>& players){
    vector<Territory*> territories = *this->getTerritories();
    if (territories.empty()){
        cout << "No territories found in map." << endl;
        return;
    }

    int numPlayers = players.size();
    int i = 0;
    for (auto* territory : territories){
        Player* currentPlayer = players[i % numPlayers];
        currentPlayer->addToDefend(territory);
        i++;
    }
    cout << "Territories have been distributed to players." << endl;
}


// ==================== MapLoader Implementation ====================

// Helper methods for parsing
vector<string> MapLoader::split(const string& str, char delimiter) const {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

string MapLoader::trim(const string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}


// Constructors (mapLoader)
MapLoader::MapLoader() {
    fileName = new string("");
}

MapLoader::MapLoader(const string& file) {
    fileName = new string(file);
}

MapLoader::MapLoader(const MapLoader& other) {
    fileName = new string(*other.fileName);
}

//Destructor (MapLoader)
MapLoader::~MapLoader() {
    delete fileName;
    fileName = nullptr;
}

//Assignment operator (MapLoader)
MapLoader& MapLoader::operator=(const MapLoader& other) {
    if (this != &other) {
        delete fileName;
        fileName = new string(*other.fileName);
    }
    return *this;
}

// Stream insertion operator (MapLoader)
ostream& operator<<(ostream& os, const MapLoader& loader) {
    os << "MapLoader for file: " << *loader.fileName;
    return os;
}

// Getters (MapLoader)
string MapLoader::getFileName() const{
    return *fileName;
}

// Setters (MapLoader)
void MapLoader::setFileName(const string& file){
    *fileName = file;
}

// Map loading operations
Map* MapLoader::loadMap() const{
    return loadMap(*fileName);
}
Map* MapLoader::loadMap(const string& file) const{
    ifstream inputFile(file);
    if (!inputFile.is_open()) {
        cout << "Error: Cannot open file " << file << endl;
        return nullptr;
    }
    
    Map* map = new Map();
    string line;
    string currentSection = "";
    // For two-pass adjacency setup
    std::map<std::string, std::vector<std::string>> adjacencyMap;
    
    try {
        while (getline(inputFile, line)) {
            line = trim(line);
            // Skip empty lines and comments (starts with a semicolon (;))
            if (line.empty() || line[0] == ';') continue;
            
            // Check for section headers
            // E.g. [Map] , [Continents], [Territories]
            else if (line[0] == '[' && line[line.length()-1] == ']') {
                currentSection = line;
                continue;
            }

            // Parse [Map] Section
            else if (currentSection == "[Map]") {
                // Print a header before printing the Map info
                cout << "Map Info:" << endl;

                // Print all the lines in the [Map] section until a blank line is found
                while (getline(inputFile, line)) {
                    line = trim(line); // Trim whitespace

                    // If encounter a blank line, stop processing the map section
                    if (line.empty()) {
                        break;
                    }
                    // Print the line to the console (or log it as needed)
                    cout << line << endl;
                }
            }

            // Parse [Continents] Section
            else if (currentSection == "[Continents]") {
                vector<string> tokens = split(line, '=');
                if (tokens.size() >= 2) {
                    string continentName = tokens[0];
                    int bonus = stoi(tokens[1]); // stoi: converts string to int
                    Continent* continent = new Continent(continentName, bonus);
                    map->addContinent(continent);  // Add the continent to the map
                }
            }
            
            // Parse [Territories] Section
            else if (currentSection == "[Territories]") {
                vector<string> tokens = split(line, ',');
                if (tokens.size() >= 4) {
                    string territoryName = tokens[0];
                    int x = stoi(tokens[1]);  // Coordinates
                    int y = stoi(tokens[2]);  // Coordinates
                    string continentName = tokens[3];

                    Territory* territory = new Territory(territoryName);
                    Continent* continent = map->getContinent(continentName);

                    if (continent != nullptr) {
                        continent->addTerritory(territory);  // Add territory to its continent
                    }

                    map->addTerritory(territory);  // Add territory to the map

                    // Store adjacency info for second pass
                    vector<string> adjacents;

                    for (size_t i = 4; i < tokens.size(); i++) {
                        for(Continent* continentMarked : *map->getContinents()){
                            if (tokens[i] == continentMarked->getName()){
                                map->setisTerritoryIsContinent(false);
                            }
                        }
                        // Add adjacent territory names to the list
                        adjacents.push_back(tokens[i]);
                    }
                    // Map territory name to its list of adjacent territory names
                    adjacencyMap[territoryName] = adjacents;
                    
                }
            
            } 
        }
        
        inputFile.close();
        
        // Second pass: set up adjacencies
        for (const auto& pair : adjacencyMap) {
            // Get the territory object by name
            Territory* territory = map->getTerritory(pair.first);

            // Range-based for loop to iterate through each adjacent territory name
            for (const string& adjName : pair.second) {
                Territory* adjacent = map->getTerritory(adjName);
                if (adjacent && adjacent != territory) {
                    territory->addAdjacentTerritory(adjacent);
                }
            }
        }

        
        return map;
        
    } catch (const exception& e) {
        cout << "Error parsing file " << file << ": " << e.what() << endl;
        inputFile.close();
        delete map;
        return nullptr;
    }
}

bool MapLoader::isValidMapFile(const string& fileName) const{
    Map* testMap = loadMap(fileName);
    bool isValid = (testMap != nullptr && testMap->validate());
    delete testMap;
    return isValid;
}