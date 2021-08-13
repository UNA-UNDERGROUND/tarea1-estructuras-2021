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
	float salarioBruto;
	float getDeducciones() const {
		float deduccion = salarioBruto * 0.09;
		if (salarioBruto > 950000) {
			deduccion += (salarioBruto - 950000) * 0.05;
		}
		return deduccion;
	}
	float getSalarioNeto() const { return salarioBruto - getDeducciones(); }
	static Empleado fromCSV(std::string linea) {
		std::stringstream s(linea);
		Empleado e;
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

void actualizarDatos(float (&datos)[3], float val) {
	// menor
	if (datos[0] > val) {
		datos[0] = val;
	}
	// mayor
	if (datos[1] < val) {
		datos[1] = val;
	}
	// acumulado (promedio)
	datos[2] += val;
}

int main() {
	try {
		std::ifstream archivo("salarios.txt");
		if (!archivo) {
			throw std::runtime_error("no se pudo abrir el archivo");
		}
		std::vector<Empleado> empleados;
		// min, max y promedio
		float salarioBruto[3] = {0, 0, 0};
		float deducciones[3] = {0, 0, 0};
		float salarioNeto[3] = {0, 0, 0};

		while (!archivo.eof()) {
			std::string linea;
			std::getline(archivo, linea);
			if (linea.empty()) {
				break;
			}
			Empleado e = Empleado::fromCSV(linea);
			actualizarDatos(salarioBruto, e.salarioBruto);
			actualizarDatos(deducciones, e.getDeducciones());
			actualizarDatos(salarioNeto, e.getSalarioNeto());
			empleados.push_back(e);
		}
		if (empleados.size() > 0) {
			salarioBruto[2] = salarioBruto[2] / empleados.size();
			deducciones[2] = deducciones[2] / empleados.size();
			salarioNeto[2] = salarioNeto[2] / empleados.size();
		}

		std::cout
		    << "+-----------+--------------------------+------------------+----------------+----------------+----------------+---+\n"
		       "|        Id |                Apellidos |           Nombre |      Sal.bruto |    Deducciones |       Sal.neto | * |\n"
		       "+-----------+--------------------------+------------------+----------------+----------------+----------------+---+\n";

		for (auto e : empleados) {
			std::cout << "| " << e.id << " | " << e.apellidos[0] << " "
			          << e.apellidos[1] << " | " << e.nombre << " | "
			          << e.salarioBruto << " | " << e.getDeducciones() << " | "
			          << e.getSalarioNeto() << " | "
			          << (e.getSalarioNeto() <= salarioNeto[2] ? "*" : " ")
			          << "|\n";
		}
		std::cout
		    << "+-----------+--------------------------+------------------+----------------+----------------+----------------+---+\n";

		std::cout << empleados.size();
	} catch (std::exception e) {
		std::cerr << "error: " << e.what() << "\n";
		return -1;
	}
}