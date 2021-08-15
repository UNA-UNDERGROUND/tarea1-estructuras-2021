#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Empleado {
	int id = -1;
	std::string apellidos[2];
	std::string nombre;
	std::string fechaNacimiento;
	float salarioBruto = 0;
	float getDeducciones() const {
		float deduccion = float(salarioBruto * 0.09);
		if (salarioBruto > 950000) {
			deduccion += float((salarioBruto - (float)950000) * 0.05);
		}
		return deduccion;
	}
	float getSalarioNeto() const { return salarioBruto - getDeducciones(); }
	static Empleado fromCSV(std::string linea) {
		std::stringstream s(linea);
		Empleado e;
		s >> e.id;
		s >> e.apellidos[0] >> e.apellidos[1];
		s.ignore();
		std::getline(s, e.nombre, '\t');
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
	if (datos[0] > val || datos[0] == -1) {
		datos[0] = val;
	}
	// mayor
	if (datos[1] < val) {
		datos[1] = val;
	}
	// acumulado (promedio)
	datos[2] += val;
}

// setw no maneja las tildes de la forma que necesitamos, por lo que hay que
// agregar espacios adicionales
std::string generarEspacios(std::string str) {
	size_t len = 0;
	size_t bytes = str.length();
	for (char c : str) {
		// https://datatracker.ietf.org/doc/html/rfc2044
		// https://www.ibm.com/docs/en/aix/7.2?topic=8-utf-ucs-transformation-format
		// descartamos todo lo que coincida con la mascara de bytes 10xxxxxx
		// en otras palabras solo contamos caracteres iniciales de unicode
		len += (c & 0xc0) != 0x80;
	}

	return std::string((bytes - len), ' ');
}

int main() {
	try {
		std::ifstream archivo("salarios.txt");
		if (!archivo) {
			throw std::runtime_error("no se pudo abrir el archivo");
		}
		std::vector<Empleado> empleados;
		// min, max y promedio
		float salarioBruto[3] = {-1, 0, 0};
		float deducciones[3]  = {-1, 0, 0};
		float salarioNeto[3]  = {-1, 0, 0};

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
		// sin asignar
		if (salarioBruto[0] == -1) {
			salarioBruto[0] = 0;
			deducciones[0]  = 0;
			salarioNeto[0]  = 0;
		}


		std::sort(empleados.begin(), empleados.end(),
		          [](const Empleado &e1, const Empleado &e2) {
			          return (e1.apellidos[0] != e2.apellidos[0])
			                     ? e1.apellidos[0] < e2.apellidos[0]
			                     : e1.apellidos[1] < e2.apellidos[1];
		          });

		std::cout << std::fixed << std::setprecision(2);
		std::cout
		    << "+-----------+--------------------------+------------------+----------------+----------------+----------------+---+\n"
		       "|        Id |                Apellidos |           Nombre |      Sal.bruto |    Deducciones |       Sal.neto | * |\n"
		       "+-----------+--------------------------+------------------+----------------+----------------+----------------+---+\n";

		// en windows este no alinea correctamente cuando hay tildes
		std::stringstream s;
		s << std::fixed << std::setprecision(2);
		for (auto e : empleados) {
			std::string apellidos = e.apellidos[0] + " " + e.apellidos[1];
			apellidos = apellidos + generarEspacios(apellidos);
			std::string nombre = e.nombre + generarEspacios(e.nombre);
			s 	<< "| " 
				<< std::setfill(' ')
				<< e.id														<< " | " << std::left
			  	<< std::setw(24) << apellidos << generarEspacios(apellidos) << " | "
				<< std::setw(16) << nombre << generarEspacios(nombre) 		<< " | " << std::right
				<< std::setw(14) << e.salarioBruto							<< " | "
			  	<< std::setw(14) << e.getDeducciones()						<< " | " << std::setw(14)
			  	<< e.getSalarioNeto() 										<< " | "
			  	<< (e.getSalarioNeto() <= salarioNeto[2] ? "*" : " ")		<< " |\n";
		}
		std::string val = s.str();
		std::cout << s.str();
		std::cout
		    << "+-----------+--------------------------+------------------+----------------+----------------+----------------+---+\n";
		std::cout
		    << "                                                          | " << std::right
			<< std::setw(14) << salarioBruto[0] 					<< " | "
		    << std::setw(14) << deducciones[0]						<< " | "
			<< std::setw(14) << salarioNeto[0] 						<< " |\n"
		    << "                                                          | "
		    << std::setw(14) << salarioBruto[2] 					<< " | "
			<< std::setw(14) << deducciones[2] 						<< " | "
			<< std::setw(14) << salarioNeto[2] 						<< " |\n"
		    << "                                                          | "
		    << std::setw(14) << salarioBruto[1] 					<< " | "
			<< std::setw(14) << deducciones[1] 						<< " | "
			<< std::setw(14) << salarioNeto[1]						<< " |\n";
		std::cout
			<< "                                                          +----------------+----------------+----------------+\n";

	} catch (const std::exception &e) {
		std::cerr << "error: " << e.what() << "\n";
		return -1;
	}
	//std::cin.ignore();
}