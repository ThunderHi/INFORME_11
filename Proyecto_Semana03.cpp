#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Interfaz para mostrar información de empleados
class Interfaz {
public:
    virtual void mostrarInformacion() const = 0; // Método virtual puro
    virtual double calcularSalario() const = 0;  // Método virtual puro
    virtual void guardar(ofstream& out) const = 0;  // Método virtual puro para guardar
    virtual ~Interfaz() {} // Destructor virtual
};

// Clase Base para empleados
class Empleado : public Interfaz {
protected:
    string nombre;
    double salarioBase;     // salario base acorde a su rol
    string fechaContratacion;
    static double tarifaHora; // Atributo estático
public:
    // Constructor de clase base
    Empleado(string nom, double salario, string fecha) : 
        nombre(nom), salarioBase(salario), fechaContratacion(fecha) {}
    // Método estático para establecer la tarifa por hora
    static void setTarifaHora(double tarifa) {
        tarifaHora = tarifa;
    }
    // Método para mostrar información básica del empleado
    void mostrarInformacion() const override {
        cout << "Nombre: " << nombre << endl;
        cout << "Salario Base: " << salarioBase << endl;
        cout << "Fecha de Contratacion: " << fechaContratacion << endl;
    }
    // Método para guardar información básica del empleado
    void guardar(ofstream& out) const override {
        out << nombre << "," << salarioBase << "," << fechaContratacion << ",";
    }
    virtual ~Empleado() {} // Destructor virtual
};

// Inicialización del atributo estático
double Empleado::tarifaHora = 0.0;

// Clase derivada Gerente
class Gerente : public Empleado {
private:
    double bono; // Adicional a su sueldo
public:
    // Constructor de clase Gerente
    Gerente(string nom, double salario, string fecha, double b) : 
        Empleado(nom, salario, fecha), bono(b) {}
    // Sobreescritura de método
    double calcularSalario() const override {
        return salarioBase + bono;
    }
    // Sobreescritura y refinamiento de método
    void mostrarInformacion() const override {
        Empleado::mostrarInformacion();
        cout << "Bono: " << bono << endl;
    }
    // Método para guardar información
    void guardar(ofstream& out) const override {
        out << "Gerente,";
        Empleado::guardar(out);
        out << bono << endl;
    }
    // Método getNombre() para la búsqueda
    string getNombre() const {
        return nombre;
    }
};

// Clase derivada Desarrollador
class Desarrollador : public Empleado {
private:
    int horasExtras; // Hora fuera de horario normal
public:
    // Constructor de clase Desarrollador
    Desarrollador(string nom, double salario, string fecha, int horas) : 
        Empleado(nom, salario, fecha), horasExtras(horas) {}
    // Sobreescritura de método
    double calcularSalario() const override {
        return salarioBase + (horasExtras * tarifaHora);
    }
    // Sobreescritura y refinamiento de método
    void mostrarInformacion() const override {
        Empleado::mostrarInformacion();
        cout << "Horas Extras: " << horasExtras << endl;
        cout << "Tarifa por Hora Extra: " << tarifaHora << endl;
    }
    // Método para guardar información
    void guardar(ofstream& out) const override {
        out << "Desarrollador,";
        Empleado::guardar(out);
        out << horasExtras << endl;
    }
    // Método getNombre() para la búsqueda
    string getNombre() const {
        return nombre;
    }
};

// Clase derivada Diseñador
class Disenador : public Empleado {
private:
    double porcentajeComision; // Ganancia por ventas
    double ventasTotales; // Ventas realizadas en soles

public:
    // Constructor de Diseñador
    Disenador(string nom, double salario, string fecha, double comision, double ventas) : 
        Empleado(nom, salario, fecha), porcentajeComision(comision), ventasTotales(ventas) {}
    // Sobreescritura de método
    double calcularSalario() const override {
        return salarioBase + (ventasTotales * porcentajeComision);
    }
    // Sobreescritura y refinamiento de método
    void mostrarInformacion() const override {
        Empleado::mostrarInformacion();
        cout << "Porcentaje de Comision: " << porcentajeComision << endl;
        cout << "Ventas Totales: " << ventasTotales << endl;
    }
    // Método para guardar información
    void guardar(ofstream& out) const override {
        out << "Disenador,";
        Empleado::guardar(out);
        out << porcentajeComision << "," << ventasTotales << endl;
    }
    // Método getNombre() para la búsqueda
    string getNombre() const {
        return nombre;
    }
};

// Plantilla de clase Departamento
template <typename T>
class Departamento {
private:    //Atributos de clase
    T** empleados;  //Se agregaran de tipo empleados
    int capacidad;
    int contador;

public:
    //Constructor de clase
    Departamento(int capInicial = 10) : capacidad(capInicial), contador(0) {
        empleados = new T*[capacidad];
    }

    //Destructor de clase
    ~Departamento() {
        for (int i = 0; i < contador; ++i) {
            delete empleados[i];
        }
        delete[] empleados;
    }

    //Método para agregar un empleado
    void agregarEmpleado(T* empleado) {
        if (contador >= capacidad) {
            capacidad *= 2;
            T** nuevosEmpleados = new T*[capacidad];
            for (int i = 0; i < contador; ++i) {
                nuevosEmpleados[i] = empleados[i];
            }
            delete[] empleados;
            empleados = nuevosEmpleados;
        }
        empleados[contador++] = empleado;
    }

    //Método para eliminar un empleado por índice
    void eliminarEmpleado(int indice) {
        //en caso de encontrar fuera de indice
        if (indice < 0 or indice >= contador) {
            cout << "Indice fuera de rango" << endl;
            return;
        }
        //elimina empleado por indice
        delete empleados[indice];

        for (int i = indice; i < contador - 1; ++i) {
            empleados[i] = empleados[i + 1];
        }
        //se actualiza contador
        --contador;
    }

    //Método para buscar un empleado por nombre
    T* buscarEmpleado(const string& nombre) const {
        for (int i = 0; i < contador; ++i) {
            if (empleados[i]->getNombre() == nombre) {
                //retorna al empleado
                return empleados[i];
            }
        }
        //en caso de que no encuentre empleado buscado
        return nullptr;
    }

    //Método para listar todos los empleados
    void listarEmpleados() const {
        for (int i = 0; i < contador; ++i) {
            cout << "Indice: " << i + 1 << endl;
            //Polimorfismo, muestra información de empleados
            empleados[i]->mostrarInformacion();
            //Polimorfismo, calcula salario de empleados
            cout << "Salario Calculado: " << empleados[i]->calcularSalario() << endl << endl;
        }
    }

    // Método para guardar todos los empleados a un archivo
    void guardar(ofstream& out) const {
        out << contador << endl;  // Guardar el número de empleados
        for (int i = 0; i < contador; ++i) {
            empleados[i]->guardar(out);
        }
    }
};

// Prototipo del menú
void menu() {
    cout << "1. Agregar Empleado\n";
    cout << "2. Eliminar Empleado\n";
    cout << "3. Buscar Empleado\n";
    cout << "4. Listar Empleados\n";
    cout << "5. Guardar Empleados a Archivo\n";
    cout << "6. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    double Thora;
    try {   //Empieza bloque Try
        // Lectura de la tarifa por hora
        cout << "Ingrese tarifa en soles de hora: ";
        cin >> Thora;
        if (cin.fail()) {   //Si el ingreso fue de algun otro tipo que no sea double
            throw runtime_error("Error: entrada no válida para tarifa por hora.");
        }
        cin.ignore();  // Limpiar el buffer de entrada

        Empleado::setTarifaHora(Thora);

        // Creación de departamentos
        Departamento<Gerente> deptoGerentes;
        Departamento<Desarrollador> deptoDesarrolladores;
        Departamento<Disenador> deptoDisenadores;

        int opcion;
        do {
            menu();
            cin >> opcion;
            cin.ignore(); // Limpiar el buffer de entrada
            switch (opcion) {
                case 1: {
                    cout << "Tipo de Empleado (1. Gerente, 2. Desarrollador, 3. Disenador): ";
                    int tipo;
                    cin >> tipo;
                    cin.ignore();

                    string nombre, fecha;
                    double salario;

                    cout << "Nombre: ";
                    getline(cin, nombre);
                    cout << "Salario Base: ";
                    cin >> salario;
                    cin.ignore();
                    cout << "Fecha de Contratacion (YYYY-MM-DD): ";
                    getline(cin, fecha);

                    if (tipo == 1) {
                        double bono;
                        cout << "Bono: ";
                        cin >> bono;
                        //creación dinámica de objeto Gerente
                        deptoGerentes.agregarEmpleado(new Gerente(nombre, salario, fecha, bono));
                    } else if (tipo == 2) {
                        int horasExtras;
                        cout << "Horas Extras: ";
                        cin >> horasExtras;
                        //creación dinámica de objeto Desarrollador
                        deptoDesarrolladores.agregarEmpleado(new Desarrollador(nombre, salario, fecha, horasExtras));
                    } else if (tipo == 3) {
                        double comision, ventas;
                        cout << "Porcentaje de Comision: ";
                        cin >> comision;
                        cout << "Ventas Totales: ";
                        cin >> ventas;
                        //creación dinámica de objeto Disenador
                        deptoDisenadores.agregarEmpleado(new Disenador(nombre, salario, fecha, comision, ventas));
                    } else {
                        cout << "Tipo de empleado no valido." << endl;
                    }
                    break;
                }
                case 2: {
                    cout << "Departamento (1. Gerentes, 2. Desarrolladores, 3. Disenadores): ";
                    int depto;
                    cin >> depto;
                    int indice;
                    cout << "Indice del empleado a eliminar: ";
                    cin >> indice;

                    if (depto == 1) {
                        deptoGerentes.eliminarEmpleado(indice - 1);
                    } else if (depto == 2) {
                        deptoDesarrolladores.eliminarEmpleado(indice - 1);
                    } else if (depto == 3) {
                        deptoDisenadores.eliminarEmpleado(indice - 1);
                    } else {
                        cout << "Departamento no valido." << endl;
                    }
                    break;
                }
                case 3: {
                    cout << "Departamento (1. Gerentes, 2. Desarrolladores, 3. Disenadores): ";
                    int depto;
                    cin >> depto;
                    cin.ignore();
                    string nombre;
                    cout << "Nombre del empleado a buscar: ";
                    getline(cin, nombre);

                    if (depto == 1) {
                        Gerente* g = deptoGerentes.buscarEmpleado(nombre);
                        if (g) {
                            g->mostrarInformacion();
                            cout << "Salario Calculado: " << g->calcularSalario() << endl;
                        } else {
                            cout << "Gerente no encontrado." << endl;
                        }
                    } else if (depto == 2) {
                        Desarrollador* d = deptoDesarrolladores.buscarEmpleado(nombre);
                        if (d) {
                            d->mostrarInformacion();
                            cout << "Salario Calculado: " << d->calcularSalario() << endl;
                        } else {
                            cout << "Desarrollador no encontrado." << endl;
                        }
                    } else if (depto == 3) {
                        Disenador* d = deptoDisenadores.buscarEmpleado(nombre);
                        if (d) {
                            d->mostrarInformacion();
                            cout << "Salario Calculado: " << d->calcularSalario() << endl;
                        } else {
                            cout << "Disenador no encontrado." << endl;
                        }
                    } else {
                        cout << "Departamento no valido." << endl;
                    }
                    break;
                }
                case 4: {
                    cout << "Departamento (1. Gerentes, 2. Desarrolladores, 3. Disenadores): ";
                    int depto;
                    cin >> depto;

                    if (depto == 1) {
                        deptoGerentes.listarEmpleados();
                    } else if (depto == 2) {
                        deptoDesarrolladores.listarEmpleados();
                    } else if (depto == 3) {
                        deptoDisenadores.listarEmpleados();
                    } else {
                        cout << "Departamento no valido." << endl;
                    }
                    break;
                }
                case 5: {
                    cout << "Guardando información de empleados a archivo..." << endl;
                    ofstream outFile("empleados.txt");
                    if (!outFile) {
                        throw runtime_error("Error al abrir el archivo de salida.");
                    }
                    deptoGerentes.guardar(outFile);
                    deptoDesarrolladores.guardar(outFile);
                    deptoDisenadores.guardar(outFile);
                    outFile.close();
                    cout << "Informacion guardada correctamente." << endl;
                    break;
                }
                case 6:
                    cout << "Saliendo..." << endl;
                    break;
                default:
                    cout << "Opción no valida." << endl;
            }
        } while (opcion != 6);
    } catch (const exception& e) {
        cerr << "Excepción capturada: " << e.what() << endl;
    }

    return 0;
}
