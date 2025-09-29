//Alona Nicolau 249664

#include <iostream>
#include <vector>
#include <fstream>

//biblioteca baixada de nlohmann https://github.com/nlohmann/json.git
#include "nlohmann/json.hpp"

//troquei as strings por char para conseguir salvar e resgatar de forma simples as informacoes
//se não for pod fica mais complexo e pra esse exercicio não julguei necessario
#include "Student.hpp"
#include "CourseRegistration.hpp"


//template pra funcao aceitar todo objeto(nesse caso vai funcionar com atributos simples[pod],
//se fosse salvar string e outras coisas faria completamente de outra forma sobrecarregando operadores)
template <typename academicInfo>

void jsonSaveInformation(std::vector<academicInfo> Info, std::string filename,  std::string type)
{
	
	nlohmann::json data;
	//verifica se ja existe
	std::ifstream inFile(filename);
	if (inFile.is_open()) {
		inFile >> data;    
		inFile.close();
	}
	//verifica se tem o tipo
	if (!data.contains(type)) {
		data[type] = nlohmann::json::array();
	}

	//entrega os json conforme ambas as classes tem getJson
	for(int index = 0;index<Info.size();index++)
	{
		data[type].push_back(Info[index].getJson());
	}

	std::ofstream arquivo(filename);
	arquivo << data.dump(3);
	arquivo.close();
}


nlohmann::json jsonReadInformation(std::string filename)
{
	std::ifstream arquivo(filename);
	nlohmann::json data;
	arquivo >> data;

	return data;
}


/*//pega o vetor de objetos e o nome do arquivo em string
void crudeSaveInformation(std::vector<academicInfo> Info, std::string filename)
{
    //abre o arquivo pra impressao binaria, adicionando no final do arquivo(flags binary e append)
	std::ofstream out(filename.c_str(), std::ios::binary | std::ios::app);
	
	for(int index = 0;index<Info.size();index++)
	{
	    //reinterpret pro compilador considerar o objeto em bytes no tamanho do objeto, assim quando ler
	    //do mesmo tamanho vai pegar exatamente o objeto
		out.write(reinterpret_cast<const char*>(&Info[index]), sizeof(academicInfo));
	}
	
	out.close();
	
	return;
}

//mesma coisa o template
template <typename academicInfo>

//retorna o vetor com as informacoes lida do arquivo, pega o nome do arquivo
std::vector<academicInfo> crudeReadInformation(std::string filename)
{
    //ifstream pra ler, só precisa da flag binary pra ler o arquivo binario
	std::ifstream in(filename.c_str(), std::ios::binary);
	
	//string pra guardar o que for lido do arquivo
	std::vector<academicInfo> Infos;
	
	//objeto pra ler objeto por objeto
	academicInfo Info;
	
	//while para ler todos os objetos em bin e inserir no vetor, pelo tamanho do objeto(sizeof), dentro de objeto Info
	while(in.read(reinterpret_cast<char*>(&Info), sizeof(academicInfo)))
	{
		Infos.push_back(Info);
	}
	
	in.close();
    
    //retorna o vetor
	return Infos;
}
*/


int main() {
    
    //vetores para guardar em memoria estudantes e registros de curso
    std::vector<Student> students;
    std::vector<CourseRegistration> registeredCourses;

    students.push_back(Student("532412", "1", "Alona", "Rua 16", 16, 5, 2004));
    students.push_back(Student("123124", "2", "Joao", "Rua 10", 5, 12, 2006));
    students.push_back(Student("123422", "3", "Pedro", "Rua 3", 20, 4, 2002));

    registeredCourses.push_back(CourseRegistration("1", "1", 3, 0));
    registeredCourses.push_back(CourseRegistration("2", "1", 6, 0));
    registeredCourses.push_back(CourseRegistration("1", "2", 3, 0));
    registeredCourses.push_back(CourseRegistration("2", "2", 6, 0));
    registeredCourses.push_back(CourseRegistration("1", "3", 3, 0));
    registeredCourses.push_back(CourseRegistration("2", "3", 6, 0));
    
    //funcao crudeSaveInformation para salvar nos arquivos binarios os objetos guardados nos vetores em memoria
    jsonSaveInformation(students, "general.json", "students");
    jsonSaveInformation(registeredCourses, "general.json", "courses");
    
    std::cout << "Dados salvos com sucesso." << std::endl;
    
    //vetores para resgatar os objetos salvos nos arquivos binarios, utilizando a funcao crudeReadInformation
	nlohmann::json retrievedData = jsonReadInformation("general.json");
    
    std::cout << "Dados carregados na memoria com sucesso.\n\n" << std::endl;
    std::cout << "Dados:\n" << std::endl;
    
    //teste de impressao 
    for(auto& student : retrievedData["students"])
	{
		std::cout
		<<"CPF: "<<student["CPF"]
		<<"\nId: "<<student["id"]
		<<"\nNome: "<<student["name"]
		<<"\nEndereco: "<<student["address"]
		<<"\nData: "<<student["dayDate"]<<"\\"<<student["monthDate"]<<"\\"<<student["yearDate"]
		<<std::endl<<std::endl;
	}
	
    for(auto& course: retrievedData["courses"])
	{
		std::cout
		<<"Id do Curso: "<<course["id"]
		<<"\nId do Estudante: "<<course["studentId"]
		<<"\nCreditos: "<<course["credit"]
		<<"\nNota: "<<course["grade"]
		<<std::endl<<std::endl;
	}
    
    return 0;
}
