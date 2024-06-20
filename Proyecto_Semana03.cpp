#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>

using namespace std;

class Empleado {
protected:
    string nombre;
    int edad;
    double salario;
public:
    Empleado() {}
    Empleado(string nom, int age, double sal) : nombre(nom), edad(age), salario(sal) {}

    virtual void mostrarInformacion() const {
        cout << "Nombre: " << nombre << endl;
        cout << "Edad: " << edad << endl;
        cout << "Salario: " << salario << endl;
    }

    virtual void guardar(ofstream& out) const {
        out << nombre << endl;
        out << edad << endl;
        out << salario << endl;
    }

    virtual void cargar(ifstream& in) {
        getline(in, nombre);
        in >> edad;
        in >> salario;
        in.ignore(); // Ignorar el salto de línea después del salario
    }

    virtual ~Empleado() {}
};

class Gerente : public Empleado {
public:
    Gerente() {}
    Gerente(string nom, int age, double sal) : Empleado(nom, age, sal) {}

    void mostrarInformacion() const override {
        cout << "Gerente: " << endl;
        Empleado::mostrarInformacion();
    }

    void guardar(ofstream& out) const override {
        out << "Gerente" << endl;
        Empleado::guardar(out);
    }

    void cargar(ifstream& in) override {
        Empleado::cargar(in);
    }
};

class Desarrollador : public Empleado {
public:
    Desarrollador() {}
    Desarrollador(string nom, int age, double sal) : Empleado(nom, age, sal) {}

    void mostrarInformacion() const override {
        cout << "Desarrollador: " << endl;
        Empleado::mostrarInformacion();
    }

    void guardar(ofstream& out) const override {
        out << "Desarrollador" << endl;
        Empleado::guardar(out);
    }

    void cargar(ifstream& in) override {
        Empleado::cargar(in);
    }
};

class Disenador : public Empleado {
public:
    Disenador() {}
    Disenador(string nom, int age, double sal) : Empleado(nom, age, sal) {}

    void mostrarInformacion() const override {
        cout << "Disenador: " << endl;
        Empleado::mostrarInformacion();
    }

    void guardar(ofstream& out) const override {
        out << "Disenador" << endl;
        Empleado::guardar(out);
    }

    void cargar(ifstream& in) override {
        Empleado::cargar(in);
    }
};

template <typename T>
void guardarEmpleado(const T& empleado, const string& archivo) {
    ofstream out(archivo, ios::app);
    if (out.is_open()) {
        empleado.guardar(out);
        out.close();
    } else {
        cout << "No se pudo abrir el archivo para escritura." << endl;
    }
}

template <typename T>
void cargarEmpleados(const string& archivo) {
    ifstream in(archivo);
    if (in.is_open()) {
        string tipo;
        while (getline(in, tipo)) {
            T empleado;
            empleado.cargar(in);
            empleado.mostrarInformacion();
            cout << endl;
        }
        in.close();
    } else {
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
}

bool validarNombre(const string& nombre) {
    for (char c : nombre) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool validarEdad(const string& edadStr, int& edad) {
    try {
        edad = stoi(edadStr);
        return true;
    } catch (...) {
        return false;
    }
}

bool validarSalario(const string& salarioStr, double& salario) {
    try {
        salario = stod(salarioStr);
        return true;
    } catch (...) {
        return false;
    }
}

void ingresarDatos() {
    string nombre, edadStr, salarioStr;
    int edad;
    double salario;

    cout << "Ingrese el nombre: ";
    getline(cin, nombre);
    try {
        if (!validarNombre(nombre)) {
            throw invalid_argument("El nombre solo puede contener letras.");
        }

        cout << "Ingrese la edad: ";
        getline(cin, edadStr);
        if (!validarEdad(edadStr, edad)) {
            throw invalid_argument("La edad debe ser un número entero.");
        }

        cout << "Ingrese el salario: ";
        getline(cin, salarioStr);
        if (!validarSalario(salarioStr, salario)) {
            throw invalid_argument("El salario debe ser un número decimal.");
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    char tipo;
    cout << "Ingrese el tipo de empleado (G: Gerente, D: Desarrollador, S: Disenador): ";
    cin >> tipo;
    cin.ignore(); // Ignorar el salto de línea

    switch (tipo) {
    case 'G':
    case 'g':
        guardarEmpleado(Gerente(nombre, edad, salario), "empleados.txt");
        break;
    case 'D':
    case 'd':
        guardarEmpleado(Desarrollador(nombre, edad, salario), "empleados.txt");
        break;
    case 'S':
    case 's':
        guardarEmpleado(Disenador(nombre, edad, salario), "empleados.txt");
        break;
    default:
        cout << "Tipo de empleado no válido." << endl;
        break;
    }
}

int main() {
    int opcion;
    do {
        cout << "1. Ingresar datos de empleado" << endl;
        cout << "2. Cargar y mostrar empleados" << endl;
        cout << "3. Salir" << endl;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        cin.ignore(); // Ignorar el salto de línea

        switch (opcion) {
        case 1:
            ingresarDatos();
            break;
        case 2:
            cargarEmpleados<Empleado>("empleados.txt");
            break;
        case 3:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opción no válida." << endl;
            break;
        }
    } while (opcion != 3);

    return 0;
}
