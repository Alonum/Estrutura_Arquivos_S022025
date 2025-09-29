//Alona Nicolau 249664

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
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

void includeJson(std::string jsonFile, std::string type, nlohmann::json json)
{
	nlohmann::json data;
	//verifica se ja existe
	std::ifstream inFile(jsonFile);
	if (inFile.is_open()) {
		inFile >> data;    
		inFile.close();
	}
	//verifica se tem o tipo
	if (!data.contains(type)) {
		data[type] = nlohmann::json::array();
	}

	data[type].push_back(json);

	std::ofstream file(jsonFile);

	file << data.dump(0);
	file.close();

	 std::ifstream inFile("index.json");
	 if(inFile.is_open())
	 {
		inFile>>data;
		inFile.close();
	 }

	if (!data.contains(type)) {
		data[type] = nlohmann::json::array();
	}
	nlohmann::json dataIndex = {
								type:
									{"id" : json["id"],"index": json["cpf"]}
														};
	
}

nlohmann::json searchByIndex(std::string jsonFile, std::string index)
{

	std::ifstream arquivo(jsonFile);
	nlohmann::json data;
	arquivo >> data;

	return data;




	return json;
}


int main() {
    
    //vetores teste para guardar em memoria estudantes e registros de curso 
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
	
	for(auto& reg : registeredCourses)
	{
		includeJson("general.json", "courses", reg.getJson());
	}
	
	for(auto& reg : students)
	{
		includeJson("general.json", "students", reg.getJson());
	}
	
	nlohmann::json test = jsonReadInformation("general.json");

	for(auto& e: test["students"])
	{
		std::cout<<"Nome: "<<e["name"]<<std::endl;
	}
    
    std::cout << "Dados teste salvos com sucesso." << std::endl;
    
    while(true)
	{
		std::cout<<"1. Inserir aluno"<<std::endl;
		std::cout<<"2. Deletar aluno"<<std::endl;
		std::cout<<"3. Procurar aluno"<<std::endl;
		std::cout<<"4. Inserir curso"<<std::endl;
		std::cout<<"5. Deletar curso"<<std::endl;
		std::cout<<"6. Procurar curso"<<std::endl;

		std::string userChoice;
		std::cin>>userChoice;

		int choice = stoi(userChoice);

		Student* jsonFileStudent;
		CourseRegistration* jsonFileRegistration;
		nlohmann::json* recoveredJson;

		switch(choice)
		{
			case 1:
			
				break;
			case 2:
				
				break;
			case 3:


				std::cout<<"Insira o CPF: "<<std::endl;
				std::cin>>userChoice;
				break;
			case 4:
			
				break;
			case 5:

				break;
			case 6:



				std::cout<<"Insira o ID do curso: "<<std::endl;
				std::cin>>userChoice;
				break;
		}
	}
    
    return 0;
}
