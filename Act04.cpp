#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>  // Para std::runtime_error
using namespace std;

class Producto {
public:
    string nombre;
    double precio;
    int cantidad;

    void mostrar() const {
        cout << left << setw(20) << nombre << setw(10) << precio << setw(10) << cantidad << endl;
    }

    void escribir(ofstream& ofs) const {
        size_t len = nombre.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ofs.write(nombre.c_str(), len);
        ofs.write(reinterpret_cast<const char*>(&precio), sizeof(precio));
        ofs.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));
    }

    void leer(ifstream& ifs) {
        size_t len;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        nombre.resize(len);
        ifs.read(&nombre[0], len);
        ifs.read(reinterpret_cast<char*>(&precio), sizeof(precio));
        ifs.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));
    }

    void leer(fstream& fs) {
        size_t len;
        fs.read(reinterpret_cast<char*>(&len), sizeof(len));
        nombre.resize(len);
        fs.read(&nombre[0], len);
        fs.read(reinterpret_cast<char*>(&precio), sizeof(precio));
        fs.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));
    }

    void escribir(fstream& fs) const {
        size_t len = nombre.size();
        fs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        fs.write(nombre.c_str(), len);
        fs.write(reinterpret_cast<const char*>(&precio), sizeof(precio));
        fs.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));
    }
};

void registrarProducto(const string& archivo) {
    Producto p;
    cout << "Ingrese nombre del producto: ";
    cin.ignore();
    getline(cin, p.nombre);
    cout << "Ingrese precio del producto: ";
    cin >> p.precio;
    cout << "Ingrese cantidad disponible: ";
    cin >> p.cantidad;

    ofstream ofs(archivo, ios::app | ios::binary);
    if (!ofs) {
        throw std::runtime_error("Error al abrir el archivo para escritura");
    }
    p.escribir(ofs);
    ofs.close();
}

void actualizarProducto(const string& archivo) {
    fstream fs(archivo, ios::in | ios::out | ios::binary);
    if (!fs) {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    string nombre;
    cout << "Ingrese el nombre del producto a actualizar: ";
    cin.ignore();
    getline(cin, nombre);

    Producto p;
    long pos;
    bool encontrado = false;

    while (fs) {
        pos = fs.tellg();
        p.leer(fs);
        if (fs.eof()) break;
        if (p.nombre == nombre) {
            cout << "Producto encontrado: " << endl;
            p.mostrar();
            cout << "Ingrese nuevo precio: ";
            cin >> p.precio;
            cout << "Ingrese nueva cantidad: ";
            cin >> p.cantidad;

            fs.seekp(pos);
            p.escribir(fs);
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        throw std::runtime_error("Producto no encontrado.");
    }

    fs.close();
}

void realizarVenta(const string& archivo) {
    fstream fs(archivo, ios::in | ios::out | ios::binary);
    if (!fs) {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    string nombre;
    cout << "Ingrese el nombre del producto a vender: ";
    cin.ignore();
    getline(cin, nombre);

    Producto p;
    long pos;
    bool encontrado = false;

    while (fs) {
        pos = fs.tellg();
        p.leer(fs);
        if (fs.eof()) break;
        if (p.nombre == nombre) {
            cout << "Producto encontrado: " << endl;
            p.mostrar();
            int cantidad;
            cout << "Ingrese cantidad a vender: ";
            cin >> cantidad;

            if (cantidad > p.cantidad) {
                throw std::runtime_error("Cantidad insuficiente en inventario");
            } else {
                p.cantidad -= cantidad;
                fs.seekp(pos);
                p.escribir(fs);
                cout << "Venta realizada con exito" << endl;
            }
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        throw std::runtime_error("Producto no encontrado.");
    }

    fs.close();
}

void generarInforme(const string& archivo) {
    ifstream ifs(archivo, ios::binary);
    if (!ifs) {
        throw std::runtime_error("No se pudo abrir el archivo.");
    }

    cout << left << setw(20) << "Nombre" << setw(10) << "Precio" << setw(10) << "Cantidad" << endl;
    cout << "-------------------------------------------------" << endl;

    Producto p;
    while (true) {
        p.leer(ifs);
        if (ifs.eof()) break;
        p.mostrar();
    }

    ifs.close();
}

int main() {
    string archivo = "inventario.dat";
    int opcion;

    do {
        try {
            cout << "1) Registrar producto" << endl;
            cout << "2) Actualizar producto" << endl;
            cout << "3) Realizar venta" << endl;
            cout << "4) Generar informe" << endl;
            cout << "5) Salir" << endl;
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                registrarProducto(archivo);
                break;
            case 2:
                actualizarProducto(archivo);
                break;
            case 3:
                realizarVenta(archivo);
                break;
            case 4:
                generarInforme(archivo);
                break;
            case 5:
                cout << "Hasta luego" << endl;
                break;
            default:
                cout << "Opcion no valida" << endl;
            }
        } catch (const std::runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        } catch (const std::exception& e) {
            cerr << "Error inesperado: " << e.what() << endl;
        } catch (...) {
            cerr << "Error desconocido" << endl;
        }

        cin.clear(); // Limpiar el estado de cin en caso de error de entrada
    } while (opcion != 5);

    return 0;
}

