//Utilizamos un archivo binario para almacenar información sobre productos
//Los datos se almacenan de manera compacta, sin espacios adicionales ni caracteres de formato

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>      //libreria para dar formato a la salida de datos en la consola (setw, left)

using namespace std;

class Producto {        
public:
    string nombre;
    double precio;
    int cantidad;

    void mostrar() const {      //Usando funciones de la libreria iomanip
        cout<<left<<setw(20)<<nombre<<setw(10)<<precio<<setw(10)<<cantidad<<endl;
    }

    //reinterpret_cast para convertir datos entre tipos y para escribir y leer en el formato binario

    void escribir(ofstream &ofs) const {        //ofstream se utiliza para escribir elementos en archivos
    size_t len = nombre.size();
    ofs.write(reinterpret_cast<const char*>(&len), sizeof(len)); // Escribir longitud del nombre
    ofs.write(nombre.c_str(), len); // Escribir el nombre
    ofs.write(reinterpret_cast<const char*>(&precio), sizeof(precio)); // Escribir precio
    ofs.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad)); // Escribir cantidad
}
    void leer(ifstream &ifs) {                  //ifstream se utiliza para leer elementos en archivos
    size_t len;
    ifs.read(reinterpret_cast<char*>(&len), sizeof(len)); // Leer longitud del nombre
    nombre.resize(len);
    ifs.read(&nombre[0], len); // Leer el nombre
    ifs.read(reinterpret_cast<char*>(&precio), sizeof(precio)); // Leer precio
    ifs.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad)); // Leer cantidad
}

    ////fstream se utiliza para leer y escribir elementos en archivos
    void leer(fstream &fs) {
        size_t len;
        fs.read(reinterpret_cast<char*>(&len), sizeof(len));
        nombre.resize(len);
        fs.read(&nombre[0], len);
        fs.read(reinterpret_cast<char*>(&precio), sizeof(precio));
        fs.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));
    }

    void escribir(fstream &fs) const {
        size_t len = nombre.size();
        fs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        fs.write(nombre.c_str(), len);
        fs.write(reinterpret_cast<const char*>(&precio), sizeof(precio));
        fs.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));
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
        if (cin.fail()) {       //cin.fail() devuelve True si ocurrio un error en el ingreso de datos (por ejemplo si se espera un int pero se ingresa un string)
            cin.clear(); //Limpiar el estado de error de cin despues de detectar una entrada invalida
            cin.ignore(1000, '\n'); //Ignora hasta 1000 caracteres o hasta el siguiente salto de línea
            throw "Entrada invalida para el precio del producto. Debe ser un numero.";
        }

        cout<<"Ingrese cantidad disponible: ";
        cin>>p.cantidad;
        if (cin.fail()) {   
            cin.clear(); //Limpiar el estado de error de cin
            cin.ignore(1000, '\n'); //Ignora hasta 1000 caracteres o hasta el siguiente salto de línea
            throw "Entrada invalida para la cantidad disponible. Debe ser un numero.";
        }

        ofstream ofs(archivo, ios::app | ios::binary);
        if (!ofs) {
            throw "Error al abrir el archivo para escritura";
        }
        p.escribir(ofs);
        ofs.close();
    } catch (const char* msg) {
        cout<<"Excepcion: "<<msg<<endl;
        // Continua con el flujo del programa permitiendo al usuario reintentar
        registrarProducto(archivo);
    }
}

void actualizarProducto(const string& archivo) {
    try {
        fstream fs(archivo, ios::in | ios::out | ios::binary);
        if (!fs) {
            throw "No se pudo abrir el archivo";
        }

        string nombre;
        cout<<"Ingrese el nombre del producto a actualizar: ";
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
                cout<<"Producto encontrado: "<<endl;
                p.mostrar();
                cout<<"Ingrese nuevo precio: ";
                cin>>p.precio;
                if (cin.fail()) {       //cin.fail() devuelve True si ocurrio un error en el ingreso de datos (por ejemplo si se espera un int pero se ingresa un string)
                    cin.clear(); //Limpiar el estado de error de cin despues de detectar una entrada invalida
                    cin.ignore(1000, '\n'); //Ignora hasta 1000 caracteres o hasta el siguiente salto de línea
                    throw "Entrada invalida para el precio del producto. Debe ser un numero.";
                }
                cout<<"Ingrese nueva cantidad: ";
                cin>>p.cantidad;
                if (cin.fail()) {
                    cin.clear(); //Limpiar el estado de error de cin despues de detectar una entrada invalida
                    cin.ignore(1000, '\n'); //Ignora hasta 1000 caracteres o hasta el siguiente salto de línea
                    throw "Entrada invalida para la cantidad disponible. Debe ser un numero.";
                }

                fs.seekp(pos);
                p.escribir(fs);
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            cout<<"Producto no encontrado"<<endl;
        }

        fs.close();
    } catch (const char* msg) {
        cout<<"Excepcion: "<<msg<<endl;
    }
}

void realizarVenta(const string& archivo) {
    try {
        fstream fs(archivo, ios::in | ios::out | ios::binary);
        if (!fs) {
            throw "No se pudo abrir el archivo";
        }

        string nombre;
        cout<<"Ingrese el nombre del producto a vender: ";
        cin.ignore();
        getline(cin, nombre);

        Producto p;
        long pos;       //Almacena la posicion en el archivo 
        bool encontrado = false;

        //Se recorre el archivo leyendo productos hasta encontrar el producto con el nombre especificado o alcanzar el final del archivo
        while (fs) {
            pos = fs.tellg();       //Obtiene la posicion actual del cursor de lectura en el archivo
            p.leer(fs);
            if (fs.eof()) break;
            if (p.nombre == nombre) {
                cout<<"Producto encontrado: "<<endl;
                p.mostrar();
                int cantidad;
                cout<<"Ingrese cantidad a vender: ";
                cin>>cantidad;
                if (cin.fail()) {       //Devuelve True si ocurrio un error en el ingreso de datos
                    cin.clear(); //Limpiar el estado de error de cin
                    cin.ignore(1000, '\n'); //Ignora hasta 1000 caracteres o hasta el siguiente salto de línea
                    throw "Entrada invalida para la cantidad a vender. Debe ser un numero.";
                }
                //si la cantidad a vender es mayor que la disponible, se muestra un mensaje de error
                if (cantidad > p.cantidad) {
                    cout<<"Cantidad insuficiente en inventario"<<endl;
                } else {
                    p.cantidad -= cantidad;
                    fs.seekp(pos);      //Mover la posición de escritura a la posición del producto encontrado
                    p.escribir(fs);
                    cout<<"Venta realizada con exito"<<endl;
                }
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            cout<<"Producto no encontrado."<<endl;
        }

        fs.close();
    } catch (const char* msg) {
        cout<<"Excepcion: "<<msg<<endl;
    }
}

void generarInforme(const string& archivo) {    //Leer y mostrar todos los productos almacenados en el archivo binario
    try {   
        ifstream ifs(archivo, ios::binary);
        if (!ifs) {
            throw "No se pudo abrir el archivo.";
        }

        cout<<left<<setw(20)<<"Nombre"<<setw(10)<<"Precio"<<setw(10)<<"Cantidad"<<endl; //Formato de salida de los datos de los productos
        cout<<"-------------------------------------------------"<<endl;

        Producto p;
        while (true) {
            p.leer(ifs);
            if (ifs.eof()) break; //Comprueba si se ha llegado al final del archivo
            p.mostrar();
        }

        ifs.close();
    } catch (const char* msg) {
        cout<<"Excepcion: "<<msg<<endl;
    }
}

int main() {    
    string archivo = "inventario.dat";   //Se define el archivo
    int opcion;

    do {
        cout<<"1) Registrar producto"<<endl;
        cout<<"2) Actualizar producto"<<endl;
        cout<<"3) Realizar venta"<<endl;
        cout<<"4) Generar informe"<<endl;
        cout<<"5) Salir"<<endl;
        cout<<"Seleccione una opcion: ";
        cin>>opcion;

        try {
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
                    cout<<"Hasta luego"<<endl;
                    break;
                default:
                    cout<<"Opcion no valida" << endl;
            }
        } catch (const char* msg) {     //En caso presentar un error se muestra por pantalla
            cout<<"Excepcion: "<<msg<<endl;
        }
    } while (opcion != 5);

    return 0;
}

