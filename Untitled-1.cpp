#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// FUNCION PARA LEER DATOS DESDE ARCHIVO
map<string, vector<int>> leerDatos(string nombreArchivo) {

    map<string, vector<int>> pacientes;

    ifstream archivo(nombreArchivo);

    string linea;

    while(getline(archivo, linea)) {

        stringstream ss(linea);

        string nombre;
        getline(ss, nombre, ',');

        vector<int> frecuencias;

        string dato;

        while(getline(ss, dato, ',')) {

            frecuencias.push_back(stoi(dato));
        }

        pacientes[nombre] = frecuencias;
    }

    archivo.close();

    return pacientes;
}


// FUNCION PARA DETECTAR TAQUICARDIA
bool detectarTaquicardia(vector<int> fc,
                         int &inicio,
                         int &fin) {

    int contador = 0;

    for(int i = 0; i < fc.size(); i++) {

        if(fc[i] > 100) {

            contador++;

            // Más de 5 consecutivas
            if(contador > 5) {

                inicio = i - 4;
                fin = i + 1;

                return true;
            }

        } else {

            contador = 0;
        }
    }

    return false;
}


// FUNCION PARA DETECTAR FATIGA
bool detectarFatiga(vector<int> fc,
                    double &promedioInicial,
                    double &promedioFinal) {

    promedioInicial = 0;
    promedioFinal = 0;

    // Primeros 10 minutos
    for(int i = 0; i < 10; i++) {

        promedioInicial += fc[i];
    }

    promedioInicial /= 10;

    // Últimos 10 minutos
    for(int i = fc.size() - 10; i < fc.size(); i++) {

        promedioFinal += fc[i];
    }

    promedioFinal /= 10;

    double incremento =
        ((promedioFinal - promedioInicial)
         / promedioInicial) * 100;

    return incremento > 20;
}


// ---------------- PROGRAMA PRINCIPAL ----------------

int main() {

    map<string, vector<int>> pacientes;

    pacientes = leerDatos("pacientes.txt");

    string nombrePaciente;

    char opcion;

    do {

        cout << "\n===== LISTA DE PACIENTES =====\n";

        for(auto paciente : pacientes) {

            cout << "- " << paciente.first << endl;
        }

        cout << "\nIngrese paciente: ";
        getline(cin, nombrePaciente);

        if(pacientes.find(nombrePaciente)
           != pacientes.end()) {

            vector<int> fc =
                pacientes[nombrePaciente];

            int inicio, fin;

            bool taquicardia =
                detectarTaquicardia(fc,
                                    inicio,
                                    fin);

            double prom1, prom2;

            bool fatiga =
                detectarFatiga(fc,
                                prom1,
                                prom2);

            cout << "\n===== RESULTADOS =====\n";

            if(taquicardia) {

                cout << "Taquicardia Sostenida detectada\n";

                cout << "Desde el minuto "
                     << inicio
                     << " hasta el minuto "
                     << fin << endl;

            } else {

                cout << "No presenta Taquicardia Sostenida\n";
            }

            if(fatiga) {

                cout << "\nFatiga por Esfuerzo detectada\n";

                cout << "Promedio primeros 10 min: "
                     << prom1 << endl;

                cout << "Promedio ultimos 10 min: "
                     << prom2 << endl;

            } else {

                cout << "\nNo presenta Fatiga por Esfuerzo\n";
            }

            if(!taquicardia && !fatiga) {

                cout << "\nPaciente sin complicaciones cardiacas\n";
            }

        } else {

            cout << "\nPaciente no encontrado\n";
        }

        cout << "\n¿Desea evaluar otro paciente? (s/n): ";
        cin >> opcion;

        cin.ignore();

    } while(opcion == 's' || opcion == 'S');

    return 0;
}