#include <iostream>
#include <string>

using namespace std;

//Función que divide un número por otro y lanza una excepción si se intenta dividir por cero
int divide(int num1, int num2) {
    if (num2 == 0) {
//runtime_error es una clase de excepción predefinida en C++ que se utiliza para indicar errores en tiempo de ejecución
        throw runtime_error("No se puede dividir por cero");
    }
    return num1 / num2;
}

int main() {
    try {
        //Llamar a la función divide con diferentes valores
        int resultado1 = divide(10, 2);
        cout<<"10 / 2 = "<<resultado1<<endl;

        int resultado2 = divide(10, 0);
        cout<<"10 / 5 = "<<resultado2<<endl;  // No se imprimirá debido a la excepción

    } catch (const runtime_error&) {
        //Manejar la excepción
        cerr<<"Error: No se puede dividir por cero"<<endl;
    }

    return 0;
}
