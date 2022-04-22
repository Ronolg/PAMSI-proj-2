#include <iostream>

void swap(int& a, int& b)
{
    int tmp=a;
    a=b;
    b=tmp;
}
int main()
{
    int array[]={1,2,3,4};

    std::cout << array[0]<< std::endl;
    std::cout << array[1]<< std::endl;
    std::cout << array[2]<< std::endl;
    std::cout << array[3]<< std::endl;

    std::cout << "==================="<< std::endl;
    swap(array[1],array[2]);
    
    std::cout << array[0]<< std::endl;
    std::cout << array[1]<< std::endl;
    std::cout << array[2]<< std::endl;
    std::cout << array[3]<< std::endl;


    return 0;
}

