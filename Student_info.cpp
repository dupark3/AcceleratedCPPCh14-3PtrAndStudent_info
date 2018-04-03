#include <algorithm> // sort, min
#include <iostream>
#include <cstdio> // EOF
#include <stdexcept> // domain_error
#include <string>

#include "Student_info.h"

    /*****************************
    * BASE CORE MEMBER FUNCTIONS *
    ******************************/
std::istream& Core::read_exams(std::istream& in){
    in >> midterm >> final;
    return in;
}

std::istream& Core::read_name(std::istream& in){
    in >> n;
    return in;
}

std::istream& Core::read(std::istream& in){
    read_name(in);
    read_exams(in);
    read_hw(in, homework);
    return in;
}

double Core::grade() const{
    return ::grade(midterm, final, homework);
}

std::string Core::letterGrade() const{
    static std::string letterGrades[] = {"A+", "A", "A-", "B+", "B", "B-", "C+", "C", "C-", "D", "F"};
    static int gradeBenchMarks[] =      { 97,   92,  90,   87,   82,  80,   77,   72,  70,   60,  0 };
    static int sizeOfLetterGrades = sizeof(letterGrades) / sizeof(std::string);
    if (this){
        double finalGrade = this->grade();
        for (int i = 0; i != sizeOfLetterGrades; ++i)
            if (finalGrade >= gradeBenchMarks[i]) return letterGrades[i];
    } else return "Cannot find letter grade of uninitialized student";
}

    /********************************
    * DERIVED GRAD MEMBER FUNCTIONS *
    *********************************/

std::istream& Grad::read(std::istream& in){
    Core::read_name(in);
    Core::read_exams(in);
    in >> thesis;
    read_hw(in, homework);
    return in;
}

double Grad::grade() const{
    // grading policy is the minimum between regular grade and thesis grade
    return std::min(Core::grade(), thesis);
}

    /***************************************
    * HANDLE STUDENT_INFO MEMBER FUNCTIONS *
    ****************************************/

Student_info::Student_info(const Student_info& s) : cp(0){
    if (s.cp) cp = s.cp->clone();
}

Student_info& Student_info::operator=(const Student_info& s){
    if (&s != this){
        cp.~Ptr();
        if (s.cp)
            cp = s.cp->clone();
        else
            cp = 0;
    }
    return *this;
}

std::istream& Student_info::read(std::istream& in){
    char ch;
    in >> ch;

    // allocate memory for new object of the appropriate type
    // new Core(in) will return a *Core, but we have a constructor for Handle class that implicitly converts *Core to Handle<Core>
    // Then we use the assignment operator to assign the newly allocated and built Handle<Core> object to the lhs
    if (ch == 'U') cp = new Core(in);
    else if (ch == 'G') cp = new Grad(in);

    return in;
}

    /*********************
    * NON MEMBER CLASSES *
    **********************/

double grade(double midterm, double final, const std::vector<double>& homework){
    if (homework.size() == 0)
        throw std::domain_error("This student has done no homework");

    return (midterm * 0.2) + (final * 0.4) + (median(homework) * 0.4);
}

bool compare_Core_handles(const Ptr<Core>& p1, const Ptr<Core>& p2){
    return p1->name() < p2->name();
}

std::istream& read_hw(std::istream& is, std::vector<double>& vec){
    if (is){
        vec.clear();
        double x;
        while (is.peek() != '\n'){
            is >> x;
            vec.push_back(x);
        }
        is.clear();
    }
    return is;
};

double median(std::vector<double> vec){
    std::sort(vec.begin(), vec.end());
    size_t size = vec.size();
    size_t mid = size / 2;
    if (size % 2 == 0) return ((vec[mid - 1] + vec[mid]) / 2);
    else return vec[mid];
};
