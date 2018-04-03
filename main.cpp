#include <algorithm> // max
#include <iomanip> //streamsize
#include <iostream>
#include <stdexcept> // domain_error
#include <string>
#include <vector>

#include "Ptr.h"
#include "Student_info.h"

int main() {
    std::cout << "Enter the type of student (U or G), name,"
              << "midterm, final, (thesis if grad), and homework grades:" << std::endl;

    std::vector< Ptr<Core> > students;
    Ptr<Core> record;
    char ch;
    std::string::size_type maxlen = 0;

    while (std::cin >> ch){
        if (ch == 'U')
            record = new Core(std::cin);
        else if (ch == 'G')
            record = new Grad(std::cin);
        maxlen = std::max(maxlen, record->name().size());
        students.push_back(record);
    }

    std::sort(students.begin(), students.end(), compare_Core_handles);

    for (std::vector< Ptr<Core> >::size_type i = 0; i != students.size(); ++i){
        std::cout << students[i]->name() << std::string(maxlen + 1 - students[i]->name().size(), ' ');
        try{
            std::streamsize prec = std::cout.precision();
            std::cout << students[i]->grade() << ' '
                      << students[i]->letterGrade() << std::endl;
        } catch (const std::domain_error& e){
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}
