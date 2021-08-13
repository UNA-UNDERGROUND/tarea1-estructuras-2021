#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Empleado {
	int id;
	std::string apellidos[2];
	std::string nombre;
	std::string fechaNacimiento;
	int salarioBruto;
	static Empleado fromCSV(std::string linea) {
		Empleado e;
		std::stringstream s;
		s >> e.id;
		s >> e.apellidos[0] >> e.apellidos[1];
		s >> e.nombre;
		s >> e.fechaNacimiento;
		s >> e.salarioBruto;
		if (!s) {
			throw std::runtime_error("formato invalido: \"" + linea + "\"");
		}
		return e;
	};
};

int main() {
	std::ifstream archivo("salarios.txt");
	if (archivo) {
		std::vector<Empleado> empleados;
		while (archivo.eof()) {
			std::string linea;
			std::getline(archivo, linea);
			empleados.push_back(Empleado::fromCSV(linea));
		}
		std::cout << empleados.size();
	} else {
		std::cerr << "error: no se pudo abrir el archivo\n";
		return -1;
	}
}