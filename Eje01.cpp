#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string fileName;
    string fileContent;

    //Solicitar al usuario el nombre del archivo
    cout<<"Ingrese el nombre del archivo de texto: ";
    getline(cin, fileName);

    //Intentar abrir el archivo
    ifstream file(fileName);

    //Verificar si el archivo se abrió correctamente
    if (file.is_open()) {
        //Leer el contenido del archivo
        getline(file, fileContent, '\0');

        //Mostrar el contenido del archivo
        cout<<"Contenido del archivo:\n"<<fileContent<<endl;

        //Cerrar el archivo
        file.close();
    } else {
        //Mostrar un mensaje de error si el archivo no existe
        cerr<<"Error: El archivo '"<<fileName<<"' no existe."<<endl;
    }

    return 0;
}
