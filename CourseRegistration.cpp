//Alona Nicolau 249664

#include "CourseRegistration.hpp"

CourseRegistration::CourseRegistration() = default;

CourseRegistration::CourseRegistration(const std::string& id, const std::string& studentId, int credit, double grade)
    : creditHours(credit), grade(grade) , courseId(id), studentId(studentId){}

const std::string CourseRegistration::getId() {
    return this->courseId;
}

const std::string CourseRegistration::getStudentId() {
    return this->studentId;
}

int CourseRegistration::getCreditHours() {
    return this->creditHours;
}

double CourseRegistration::getGrade() {
    return this->grade;
}

void CourseRegistration::setId(const std::string id) {
    this->courseId = id;
}

void CourseRegistration::setStudentId(const std::string id) {
    this->studentId = id;
}

void CourseRegistration::setCreditHours(int creditHours) {
    this->creditHours = creditHours;
}

void CourseRegistration::setGrade(double grade) {
    this->grade = grade;
}

nlohmann::json CourseRegistration::getJson() const{
    nlohmann::json courseRegister = {
        {"id", this->courseId},
        {"studentId", this->studentId},
        {"credit", this->creditHours},
        {"grade", this->grade}
    };
    
    return courseRegister;
}

void CourseRegistration::show()
{
    std::cout<<"ID Curso: "<<this->courseId<<std::endl;
    std::cout<<"ID Estudante: "<<this->studentId<<std::endl;
    std::cout<<"Creditos: "<<this->creditHours<<std::endl;
    std::cout<<"Nota: "<<this->grade<<std::endl;
}

