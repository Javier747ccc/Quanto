// Quanto.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <algorithm>  
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include <vector>
#include <fstream> 
#include <string>
#include <math.h>
#include <string>

using namespace std;

#define pi 3.14159265358979323846264

string StringToUpper(string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
	return strToConvert;
}
class point3D
{
private:
	double _x, _y, _z;
public:
	point3D(double x, double y, double z)
	{
		_x = x; _y = y; _z = z;
	}
	point3D()
	{
		_x = _y = 0;
		_z = 1;
	}
	double getX() { return _x; }
	double getY() { return _y; }
	double getZ() { return _z; }
	void setX(double x) { _x = x; }
	void setY(double y) { _y = y; }
	void setZ(double z) { _z = z; }
};

class QuantumReg
{
    private:

		//Angles
        double alpha;
        double beta;
        double gamma;
        point3D blochSpherePoint;
    public:
        QuantumReg()
        {
            alpha = beta = gamma = 0;
		}
        
		void addAlpha(double n) 
        {
            alpha += n;
            if (alpha >= (2 * 180))
				alpha -= (2 * 180);
            if (alpha < 0)
				alpha+= (2 * 180);
			
			rotateX( (alpha*pi)/180.0);

        }
        void addBeta(double n)
        {
            beta += n;
            if (beta >= (2 * 180))
				beta -= (2 * 180);
            if (beta < 0)
				beta += (2 * 180);
			rotateY((beta*pi) / 180.0);
        }
        void addGamma(double n)
        {
            gamma += n;
            if (gamma >= (2 * 180))
				gamma -= (2 * 180);
            if (gamma < 0)
				gamma += (2 * 180);
			rotateZ( (gamma*pi) / 180.0);
        }

		void rotateX(double angle) 
		{
			double cosa = cos(angle);
			double sina = sin(angle);
			double tempy = blochSpherePoint.getY();
			blochSpherePoint.setY( cosa * blochSpherePoint.getY() - sina * blochSpherePoint.getZ()) ;
			blochSpherePoint.setZ(cosa * blochSpherePoint.getZ() + sina * tempy);
		}
		
		void rotateY(double angle) 
		{
			double cosa = cos(angle);
			double sina = sin(angle);
			double tempz = blochSpherePoint.getZ();
			blochSpherePoint.setZ( cosa * blochSpherePoint.getZ() - sina * blochSpherePoint.getX());
			blochSpherePoint.setX( cosa * blochSpherePoint.getX() + sina * tempz);
		}

		void rotateZ(double angle)
		{
			double cosa = cos(angle);
			double sina = sin(angle);
			double tempx = blochSpherePoint.getX();
			blochSpherePoint.setX( cosa * blochSpherePoint.getX() - sina * blochSpherePoint.getY());
			blochSpherePoint.setY( cosa * blochSpherePoint.getY() + sina * tempx);
		}

		point3D getBlochSpherePoint() { return blochSpherePoint; }
        
};
class Quantum
{
    private:
    
        map<string, QuantumReg> qregs;
        map<string, int> cregs;
		const vector<string> FindRegBracketArgs(string s)
        {
            vector<string> v;
            

            regex exp("((\\w+)\\[(\\d+)\\])");
            smatch res;
            string str = s;
            while (regex_search(str, res, exp))
            {
                //cout << res[0] << endl;
                string reg = res[0];
                smatch match;

                if (regex_search(reg, match, regex("(\\w+)\\[(\\d+)\\]")))
                {
                    if (match.size() > 2)
                    {

                        ostringstream name;
                        name << match.str(1) << match.str(2);
                        v.push_back(name.str());
                    }
                }

                str = res.suffix();
            }

            return v;
        }
    public:
        Quantum() {};
        void executeH(string x);
        void executeQReg(string s);
        void executeCReg(string s);
        void executeXReg(string s);
        void executeYReg(string s);
        void executeZReg(string s);
        void executeCU1(string angle, string s);
		void executeSwap(string s);
		void executeMeasure(string s);
};
std::string trim(const std::string& str,
	const std::string& whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

void Quantum::executeQReg(string s)
{
    std::smatch match;
    if (regex_search(s, match, regex("(\\w+)\\[(\\d+)\\];")))
    {
        if (match.size() > 2)
        {
            int  n = atoi(match.str(2).c_str());
            for (int i = 0; i < n; i++)
            {
                ostringstream name;
                name << match.str(1) << i;
                QuantumReg qreg;
                

                qregs.insert(std::make_pair(name.str(), qreg));
            }
        }
    }
    else
        cout << "QReg Error" << endl;

    
}
void Quantum::executeCReg(string s)
{
    std::smatch match;
    if (regex_search(s, match, regex("(\\w+)\\[(\\d+)\\];")))
    {
        if (match.size() > 2)
        {
            int  n = atoi(match.str(2).c_str());
            for (int i = 0; i < n; i++)
            {
                ostringstream name;
                name << match.str(1) << i;
                
                cregs.insert(std::make_pair(name.str(), 0));
            }
        }
    }
    else
        cout << "CReg Error" << endl;
}
void Quantum::executeXReg(string s)
{
    const vector<string> &v = FindRegBracketArgs(s);
    qregs[v[0]].addAlpha(180);
	point3D p = qregs[v[0]].getBlochSpherePoint();
}
void Quantum::executeYReg(string s)
{
    const vector<string>& v = FindRegBracketArgs(s);
    qregs[v[0]].addBeta(180);
}
void Quantum::executeZReg(string s)
{
    const vector<string>& v = FindRegBracketArgs(s);
    qregs[v[0]].addGamma(180);
}

void Quantum::executeH(string s)
{
    const vector<string>& v = FindRegBracketArgs(s);
    qregs[v[0]].addBeta(-90);
	qregs[v[0]].addGamma(180);
}
void Quantum::executeSwap(string s)
{
	const vector<string>& v = FindRegBracketArgs(s);
	if (v.size() == 2)
	{
		swap(qregs[v[0]], qregs[v[1]]);
		int a = 0;

	}
}
void Quantum::executeMeasure(string s)
{
	const vector<string>& v = FindRegBracketArgs(s);
	if (v.size() == 2)
	{
		//swap(qregs[v[0]], qregs[v[1]]);
		int a = 0;

	}
}
void Quantum::executeCU1(string angle, string s)
{
    const vector<string>& v = FindRegBracketArgs(s);
	try
	{
		if (v.size() > 1)
		{
			if (qregs[v[1]].getBlochSpherePoint().getZ() > 0)
			{

				double a = 0;
				int frac = 1;
				std::regex re("\\((\\w*)+\\/?(\\d*)?\\)");
				std::smatch match;
				angle.erase(remove_if(angle.begin(), angle.end(), ::isspace), angle.end());
				if (std::regex_search(angle, match, re) && match.size() > 1)
				{
					if (match.length() > 2)
					{
						if (match.str(1) == "pi")
						{
							a = 180;
							frac = stod(match.str(2));
						}
					}
					qregs[v[1]].addGamma(a / frac);

					point3D p = qregs[v[1]].getBlochSpherePoint();
					cout << "Poner " << StringToUpper(v[1]) << " en posicion: " << asin( p.getY()) *(180.0/pi)<< endl;
					int a = 0;
				}
				else
				{
					throw;
				}
			}
		}
		else
		{
			throw;
		}
	}
	catch (std::regex_error& e)
	{
		cout << e.what() << endl;
	}
}


void Pruebas()
{
    smatch match;
    string str = "q[1],q[0]";
	
	QuantumReg reg;

	

	reg.rotateY((-90*pi)/180);
	reg.rotateZ((180 * pi) / 180);

	reg.rotateZ((180 * pi) / 180);


	reg.rotateY((-90 * pi) / 180);
	reg.rotateZ((180 * pi) / 180);


    regex exp("((\\w+)\\[(\\d+)\\])");
    smatch res;
    
    while (regex_search(str, res, exp)) 
    {
        cout << res[0] << endl;
        str = res.suffix();
    }


}
int main(int argc, char *argv[])
{

    //Pruebas();
    std::string arg1="";

    if (argc > 1)
        arg1 = std::string(argv[1]);
    std::cout << "Quanto!\n";
    Quantum Q;
    
    ifstream fich(arg1);
    if (!fich.is_open())
    {
        cout << "Error al abrir " << arg1 << "\n";
        exit(EXIT_FAILURE);
    }
    string linea;
    while (getline(fich, linea))
    {
        std::string subject(linea);
        std::string result;
        if (trim(linea).substr(0,2) == "//")
            continue;
        try {
            std::regex re("(\\w*)(\\(.*\\))?\\s(.*,?)");
            std::smatch match;
            if (std::regex_search(subject, match, re) && match.size() > 1)
            {
                if (match.str(1) == "qreg")
                {
                    Q.executeQReg(match.str(3));
                }
                else if (match.str(1) == "creg")
                {
                    Q.executeCReg(match.str(3));
                }
                else if (match.str(1) == "h")
                {
                    Q.executeH(match.str(3));
                }
                else if (match.str(1) == "x")
                {
                    Q.executeXReg(match.str(3));
                }
                else if (match.str(1) == "cu1")
                {
                    Q.executeCU1(match.str(2), match.str(3));
                }
				else if (match.str(1) == "swap")
				{
					Q.executeSwap(match.str(3));
				}
				else if (match.str(1) == "measure")
				{
					Q.executeMeasure(match.str(3));
				}
                else if (match.str(1) == "OPENQASM")
                {
                    string sv = match.str(3);
                    unsigned char v = sv[0];
                    if (v != '2')
                    {
                        cout << "Version incorrecta de Qasm" << endl;
                        exit(0);
                    }
                }
            }
            else
            {
                result = std::string("");
            }
            
        }

        catch (std::regex_error& e)
        {
            cout << e.what() << endl;
        }
    }
    fich.close();
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
