#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // Librería para dar formato a la salida de datos en la consola (setw, left)

using namespace std;

class Producto {
public:
    string nombre;
    double precio;
    int cantidad;

    void mostrar() const {
        cout<<left<<setw(20)<<nombre<<setw(10)<<precio<<setw(10)<<cantidad<<endl;
    }

    void escribir(ofstream &ofs) const {
        size_t len = nombre.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ofs.write(nombre.c_str(), len);
        ofs.write(reinterpret_cast<const char*>(&precio), sizeof(precio));
        ofs.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));
    }

    void leer(ifstream &ifs) {
        size_t len;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        nombre.resize(len);
        ifs.read(&nombre[0], len);
        ifs.read(reinterpret_cast<char*>(&precio), sizeof(precio));
        ifs.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));
    }
};

void registrarProducto(const string& archivo) {
    try {
        Producto p;
        cout<<"Ingrese nombre del producto: ";
        cin.ignore();
        getline(cin, p.nombre);

        cout<<"Ingrese precio del producto: ";
        cin>>p.precio;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            throw "Entrada invalida para el precio del producto. Debe ser un numero.";
        }

        cout<<"Ingrese cantidad disponible: ";
        cin>>p.cantidad;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            throw "Entrada invalida para la cantidad disponible debe ser un numero.";
        }

        ofstream ofs(archivo, ios::app | ios::binary);
        if (!ofs) {
            throw "Error al abrir el archivo para escritura";
        }
        p.escribir(ofs);
        ofs.close();
    } catch (const char* msg) {
        cout<<"Excepcion: "<<msg<<endl;
        registrarProducto(archivo);
    }
}

void generarInforme(const string& archivo) {
    try {
        ifstream ifs(archivo, ios::binary);
        if (!ifs) {
            throw "No se pudo abrir el archivo.";
        }

        cout<<left<<setw(20)<<"Nombre"<<setw(10)<<"Precio"<<setw(10)<<"Cantidad"<<endl;
        cout<< "-------------------------------------------------"<<endl;

        Producto p;
        while (true) {
            p.leer(ifs);
            if (ifs.eof()) break;
            p.mostrar();
        }

        ifs.close();
    } catch (const char* msg) {
        cout<<"Excepcion: "<<msg<<endl;
    }
}

int main() {
    string archivo = "inventario.dat";
    int opcion;

    do {
        cout<<"1) Registrar producto"<<endl;
        cout<<"2) Generar informe"<<endl;
        cout<<"3) Salir"<<endl;
        cout<<"Seleccione una opcion: ";
        cin>> opcion;

        try {
            switch (opcion) {
                case 1:
                    registrarProducto(archivo);
                    break;
                case 2:
                    generarInforme(archivo);
                    break;
                case 3:
                    cout<<"Hasta luego"<<endl;
                    break;
                default:
                    cout<<"Opcion no valida"<<endl;
            }
        } catch (const char* msg) {
            cout<<"Excepcion: "<<msg<<endl;
        }
    } while (opcion != 3);

    return 0;
}
