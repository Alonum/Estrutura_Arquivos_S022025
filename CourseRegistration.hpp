//Alona Nicolau 249664

#ifndef COURSEREGISTRATION_HPP
#define COURSEREGISTRATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include "nlohmann/json.hpp"

class CourseRegistration {
private:
    std::string courseId;
    std::string studentId;
    int creditHours;
    double grade;

public:
    CourseRegistration();
    CourseRegistration(const std::string& id, const std::string& studentId, int credit, double grade);
    
    const std::string getId();
    const std::string getStudentId();
    int getCreditHours();
    double getGrade();
    nlohmann::json getJson() const;


    
    void setId(const std::string id);
    void setStudentId(const std::string id);
    void setCreditHours(int creditHours);
    void setGrade(double grade);
    void show();

};

#endif
