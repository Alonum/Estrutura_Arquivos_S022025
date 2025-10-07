//Alona Nicolau 249664

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
//biblioteca baixada de nlohmann https://github.com/nlohmann/json.git
#include "nlohmann/json.hpp"

//troquei as strings por char para conseguir salvar e resgatar de forma simples as informacoes
//se não for salvar string e outras coisas faria completamente de outra forma sobrecarregando operadores
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
    //verifica se ja existe o arquivo principal
    std::ifstream inFile(jsonFile);
    if (inFile.is_open()) {
        inFile >> data;    
        inFile.close();
    }

    //verifica se tem o tipo no arquivo principal
    if (!data.contains(type)) {
        data[type] = nlohmann::json::array();
    }

    //adiciona o objeto json no arquivo principal
    data[type].push_back(json);

    std::ofstream file(jsonFile);
    file << data.dump(0); //salva sem indentação
    file.close();

    //---- agora atualiza o arquivo de índice ----
    std::ifstream idxFile("index.json");
    nlohmann::json idxData;
    if(idxFile.is_open()) {
        idxFile >> idxData;    
        idxFile.close();
    }

    //verifica se tem o tipo no índice
    if(!idxData.contains(type)) {
        idxData[type] = nlohmann::json::array();
    }

    //prepara o objeto de índice
    nlohmann::json indexEntry;
    if(type != "students") //para cursos
    {
        indexEntry = { {"id", json["id"]} };
    }
    else //para estudantes
    {
        //CPF como chave principal, id como referência
        indexEntry = { {"CPF", json["CPF"]}, {"id", json["id"]} };
    }

    //adiciona no índice
    idxData[type].push_back(indexEntry);

    //salva o índice atualizado
    std::ofstream idxOut("index.json");
    idxOut << idxData.dump(0); //salva sem indentação
    idxOut.close();
}


//Handler para SAX parser do nlohmann::json
//Inspirado na documentação oficial: https://json.nlohmann.me/api/sax/
//Para usar SAX parser com essa função, é necessário **reimplementar toda a estrutura de leitura**.
//Não basta apenas chamar json_sax_t ou usar includeJson do jeito antigo, porque SAX trabalha
//com eventos e não com carregamento completo do arquivo.
//Ou seja, toda lógica de construção de objetos, verificação de campos e controle de profundidade
//precisa ser adaptada ao modelo de eventos do SAX para que funcione corretamente.

struct Handler : nlohmann::json::json_sax_t
{
    std::string targetValue; //valor que queremos encontrar (id ou CPF)
    std::string targetKey;   //chave do objeto a comparar ("id" ou "CPF")
    bool found = false;       //flag pra indicar se achamos o objeto
    nlohmann::json result;    //guarda o objeto encontrado
    std::string currentKey;   //chave atual lida pelo parser
    nlohmann::json currentObj;//objeto JSON atualmente sendo construído
    int depth = 0;            //controle de profundidade pra objetos aninhados

    //Construtor: recebe o valor que vamos buscar e a chave correspondente
    Handler(const std::string& value, const std::string& key) : targetValue(value), targetKey(key) {}

    //evento chamado quando começa um objeto no JSON
    bool start_object(std::size_t /*elements*/) override
    {
        depth++;
        if(depth == 2) currentObj = nlohmann::json::object(); //novo objeto que vamos analisar
        return true;
    }

    //evento chamado quando termina um objeto
    bool end_object() override
    {
        if(depth == 2)
        {
            //verifica se o objeto tem o campo desejado
            if(currentObj.contains(targetKey) && currentObj[targetKey] == targetValue)
            {
                result = currentObj; //guardamos o objeto
                found = true;
                return false; //para o parsing, já achamos
            }
        }
        depth--;
        return true;
    }

    //evento chamado quando parser encontra uma chave
    bool key(std::string& k) override { currentKey = k; return true; }

    //evento chamado quando parser encontra string
    bool string(std::string& val) override { if(depth == 2) currentObj[currentKey] = val; return true; }

    //evento chamado quando parser encontra números inteiros
    bool number_integer(int64_t val) override { if(depth == 2) currentObj[currentKey] = val; return true; }
    bool number_unsigned(uint64_t val) override { if(depth == 2) currentObj[currentKey] = val; return true; }
    bool number_float(double val, const std::string&) override { if(depth == 2) currentObj[currentKey] = val; return true; }

    //os outros eventos do SAX são ignorados, mas precisam estar implementados
    bool null() override { return true; }
    bool boolean(bool) override { return true; }
    bool start_array(std::size_t) override { return true; }
    bool end_array() override { return true; }
    bool parse_error(std::size_t, const std::string&, const nlohmann::json::exception&) override { return false; }
    //implementa a função pura binary pra classe não ser mais abstrata
    bool binary(nlohmann::json::binary_t&) override { return true; }
};

//funcao pra procurar sequencialmente, vou tirar dps mas só pra mostrar que demora mais
nlohmann::json searchSequential(std::string jsonFile, std::string keyValue, std::string type)
{
    std::ifstream arquivo(jsonFile);
    nlohmann::json data;
    arquivo >> data;
    arquivo.close();

    for(auto& entry : data[type])
    {
        if(type == "students")
        {
            if(entry["CPF"] == keyValue)
                return entry;
        }
        else
        {
            if(entry["id"] == keyValue)
                return entry;
        }
    }

    std::cout << "Valor nao encontrado na busca sequencial!\n";
    return nlohmann::json(); //objeto vazio
}

//Função para buscar um objeto no arquivo JSON principal usando o índice
nlohmann::json searchByIndex(std::string jsonFile, std::string index, std::string type)
{
    //abre o arquivo de índice
    std::ifstream idxFile("index.json");
    nlohmann::json idxData;
    if(idxFile.is_open())
    {
        idxFile >> idxData;    
        idxFile.close();
    }
    else
    {
        std::cout << "Arquivo de índice não encontrado!" << std::endl;
        return nullptr;
    }

    //verifica se tem o tipo
    if(!idxData.contains(type))
    {
        std::cout << "Tipo não encontrado no índice!" << std::endl;
        return nullptr;
    }

    std::string targetKey; //key que vamos procurar no arquivo grande

    //define a key a buscar de acordo com o tipo
    if(type == "students")
        targetKey = "CPF"; //para estudantes, buscamos pelo CPF
    else
        targetKey = "id";  //para cursos, buscamos pelo id

    //procura no índice o valor informado e recupera o id/cpf correspondente
    bool foundInIndex = false;
    for(auto& entry : idxData[type])
    {
        if(entry.contains(targetKey) && entry[targetKey] == index)
        {
            //encontramos a key no índice
            index = entry[targetKey]; //garante que usamos exatamente o valor correto
            foundInIndex = true;
            break;
        }
    }

    if(!foundInIndex)
    {
        std::cout << "Valor não encontrado no índice!" << std::endl;
        return nullptr;
    }

    //--- agora abrimos o arquivo grande ---
    std::ifstream arquivo(jsonFile);
    if(!arquivo.is_open())
    {
        std::cout << "Arquivo JSON principal não encontrado!" << std::endl;
        return nullptr;
    }

    //cria o handler passando a key correta (CPF ou id)
    Handler handler(index, targetKey);

    //usa SAX parser pra ler só o objeto necessário
    nlohmann::json::sax_parse(arquivo, &handler);

    arquivo.close();

    if(handler.found)
        return handler.result; //retorna o objeto encontrado
    else
        return nullptr;       //não encontrado
}
int main() {
    
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
    
    for(auto& reg : registeredCourses)
        includeJson("general.json", "courses", reg.getJson());
    
    for(auto& reg : students)
        includeJson("general.json", "students", reg.getJson());

    nlohmann::json test = jsonReadInformation("general.json");

    for(auto& e: test["students"])
        std::cout<<"Nome: "<<e["name"]<<std::endl;
    
    std::cout << "Dados teste salvos com sucesso." << std::endl;
    
    while(true)
    {
        std::cout << "\n===== MENU =====" << std::endl;
        std::cout << "1. Inserir aluno" << std::endl;
        std::cout << "2. Procurar aluno (indice)" << std::endl;
        std::cout << "3. Inserir curso" << std::endl;
        std::cout << "4. Procurar curso (indice)" << std::endl;
        std::cout << "5. Procurar aluno (sequencial)" << std::endl;
        std::cout << "6. Procurar curso (sequencial)" << std::endl;
        std::cout << "0. Sair" << std::endl;

        std::string userChoice;
        std::cin >> userChoice;
        int choice = stoi(userChoice);

        switch(choice)
        {
            case 1: //Inserir aluno
            {
                std::string id, cpf, name, address;
                int day, month, year;
                std::cout << "Insira ID: ";
                std::cin >> id;
                std::cout << "Insira CPF: ";
                std::cin >> cpf;
                std::cout << "Insira Nome: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Insira Endereco: ";
                std::getline(std::cin, address);
                std::cout << "Data de nascimento (dia mes ano): ";
                std::cin >> day >> month >> year;

                Student s(cpf, id, name, address, day, month, year);
                students.push_back(s);

                includeJson("general.json", "students", s.getJson());
                std::cout << "Aluno inserido com sucesso!\n";
                break;
            }
            case 2: //Procurar aluno (indice)
            {
                std::cout << "Insira o CPF: ";
                std::cin >> userChoice;

                auto start = std::chrono::high_resolution_clock::now();
                nlohmann::json foundStudent = searchByIndex("general.json", userChoice, "students");
                auto end = std::chrono::high_resolution_clock::now();

                if(!foundStudent.is_null())
                    std::cout << "Aluno encontrado: " << foundStudent.dump(3) << std::endl;
                else
                    std::cout << "Aluno nao encontrado.\n";

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "Tempo de execução (indice): " << duration << " microssegundos\n";
                break;
            }
            case 3: //Inserir curso
            {
                std::string id, studentId;
                int courseNum, grade;
                std::cout << "Insira ID do curso: ";
                std::cin >> id;
                std::cout << "Insira ID do estudante: ";
                std::cin >> studentId;
                std::cout << "Numero do curso: ";
                std::cin >> courseNum;
                std::cout << "Nota: ";
                std::cin >> grade;

                CourseRegistration c(id, studentId, courseNum, grade);
                registeredCourses.push_back(c);

                includeJson("general.json", "courses", c.getJson());
                std::cout << "Curso inserido com sucesso!\n";
                break;
            }
            case 4: //Procurar curso (indice)
            {
                std::cout << "Insira o ID do curso: ";
                std::cin >> userChoice;

                auto start = std::chrono::high_resolution_clock::now();
                nlohmann::json foundCourse = searchByIndex("general.json", userChoice, "courses");
                auto end = std::chrono::high_resolution_clock::now();

                if(!foundCourse.is_null())
                    std::cout << "Curso encontrado: " << foundCourse.dump(3) << std::endl;
                else
                    std::cout << "Curso nao encontrado.\n";

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "Tempo de execução (indice): " << duration << " microssegundos\n";
                break;
            }
            case 5: //Procurar aluno (sequencial)
            {
                std::cout << "Insira o CPF: ";
                std::cin >> userChoice;

                auto start = std::chrono::high_resolution_clock::now();
                nlohmann::json foundStudentSeq = searchSequential("general.json", userChoice, "students");
                auto end = std::chrono::high_resolution_clock::now();

                if(!foundStudentSeq.is_null())
                    std::cout << "Aluno encontrado (sequencial): " << foundStudentSeq.dump(3) << std::endl;
                else
                    std::cout << "Aluno nao encontrado.\n";

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "Tempo de execução (sequencial): " << duration << " microssegundos\n";
                break;
            }
            case 6: //Procurar curso (sequencial)
            {
                std::cout << "Insira o ID do curso: ";
                std::cin >> userChoice;

                auto start = std::chrono::high_resolution_clock::now();
                nlohmann::json foundCourseSeq = searchSequential("general.json", userChoice, "courses");
                auto end = std::chrono::high_resolution_clock::now();

                if(!foundCourseSeq.is_null())
                    std::cout << "Curso encontrado (sequencial): " << foundCourseSeq.dump(3) << std::endl;
                else
                    std::cout << "Curso nao encontrado.\n";

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "Tempo de execução (sequencial): " << duration << " microssegundos\n";
                break;
            }
            case 0:
                std::cout << "Saindo...\n";
                return 0;
            default:
                std::cout << "Opcao invalida!\n";
                break;
        }
    }

    return 0;
}



//observações finais. Muito engraçado que a gente acaba cometendo uns errinhos bem bobos, e descobri hoje que
//minha maior sabedoria é saber que eu sou potencialmente uma imbecil
//procurando pelo código main.cpp inteiro o por que o index.json tava salvando id no lugar do cpf e vice versa pra estudante
//na verdade é pq em algum momento eu alterei a classe de estudante e troquei a posição dos parametros pq eu achei mais bonitinho
//mas não atualizei no restante ^^
//moral da historia, o problema as vezes realmente ta entre a cadeira, e o computador.

//Outra observação é de que ler json em C++ é mt mais complicado e não vale tanto a pena. Na verdade tava pensando que talvez fosse
//melhor realmente fazer em Java, mas quero mostrar como seria em C++ então vou seguir em frente.