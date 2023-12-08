#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static const int tableSize = 9000;

struct info{
    int numAccesos;
    int numConexiones;
    vector<string> IPs;
    vector<string> causasError;
};

class HashEntry{
public:
    string key;
    info domainInfo;
    HashEntry* next;

    HashEntry(string key, info domainInfo){
        this->key = key;
        this->domainInfo = domainInfo;
        this->next = nullptr;
    }
};

class Hash{
private:
    HashEntry** table;

public:
    Hash(){
        table = new HashEntry * [tableSize];
        for (int i = 0; i < tableSize; i++){
            table[i] = nullptr;
        }
    }

    int HashFunc(string key){
        int sum = 0;
        for (char c : key){
            sum += (int)c;
        }
        return sum % tableSize;
    }

    void insert(string key, info domainInfo) {
        int index = HashFunc(key);
        HashEntry *pointer = table[index];

        while (pointer != nullptr && pointer->key != key) {
            pointer = pointer->next;
        }

        if (pointer == nullptr) {
            HashEntry *n = new HashEntry(key, domainInfo);
            n->next = table[index];
            table[index] = n;
        }
        else {
            pointer->domainInfo.numAccesos++;
            if (find(pointer->domainInfo.IPs.begin(), pointer->domainInfo.IPs.end(), domainInfo.IPs[0]) == pointer->domainInfo.IPs.end()) {
                pointer->domainInfo.IPs.push_back(domainInfo.IPs[0]);
                pointer->domainInfo.numConexiones++;
            }
        }
    }

    HashEntry* search(string key){
        int index = HashFunc(key);
        HashEntry* pointer = table[index];

        while (pointer != nullptr && pointer->key != key){
            pointer = pointer->next;
        }

        if (pointer != nullptr) {
            cout << "Información del dominio " << key << endl;
            cout << "Número de accesos: " << pointer->domainInfo.numAccesos << endl;
            cout << "Número de conexiones únicas: " << pointer->domainInfo.numConexiones << endl;
            cout << "IPs únicas del dominio:" << endl;
            for (const auto& ip : pointer->domainInfo.IPs) {
                cout << ip << "\n";
            }
            cout << "Causas de error:" << endl;
            for (const auto& causa : pointer->domainInfo.causasError) {
                cout << causa << endl;
            }
        }
        else {
            cout << "Dominio no encontrado: " << key << endl;
        }

        return pointer;
    }
};

int main(){
    ifstream file("/Users/armandoestrada/Desktop/DSA Clase/Act 5.2/bitacora.txt");

    if (!file.is_open()){
        cerr << "No se pudo abrir el arhcivo." << endl;
        return 1;
    }

    Hash hashTable;
    string line;

    while (getline(file, line)){
        istringstream iss(line);
        string month, day, time, ip, reason;
        iss >> month >> day >> time >> ip;
        getline(iss, reason);

        int pos = ip.find(":");
        string domain = ip.substr(0, pos);

        info domainInfo;
        domainInfo.numAccesos = 1;
        domainInfo.numConexiones = 1;
        domainInfo.IPs.push_back(ip);
        domainInfo.causasError.push_back(reason);

        hashTable.insert(domain, domainInfo);
    }

    string domain;
    bool flag = true;
    string num;

    while (flag){
        cout << "Ingresa un dominio: "; getline(cin, domain);
        hashTable.search(domain);
        cout << endl;
        cout << "Ingresa 1 para buscar otro dominio, y cualquier otra tecla para salir:  "; getline(cin, num);
        if (num != "1"){
            flag = false;
        }
    }

    return 0;
}