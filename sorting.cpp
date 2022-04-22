#include <iostream>
#include <fstream>

const int size = 10;

class Movie
{
    private:
        int id;
        std::string title;
        double raiting;
    public:
    void setId(int a){this->id = a;}
    void setTitle(std::string t){this->title = t;}
    void setRaiting(double r){this->raiting = r;}

    int getId(){return this->id;}
    std::string getTitle(){return this->title;}
    double getRaiting(){return this->raiting;}

    Movie()
    {
        this->id=-1;
        this->raiting=-1;
    }

    Movie operator=(Movie b)
    {
        this->setId(b.getId());
        this->setTitle(b.getTitle());
        this->setRaiting(b.getRaiting());

        return *this;
    }
};

Movie* read(Movie *array)
{
    std::ifstream file("projekt2_dane.csv");
    std::string line_skipped;
    char character;
    int count = 0;
    int id_read;
    std::string title_read;
    double raiting_read;
    bool raiting_found=0;
    bool next=0;
    int i=0;
    std::string raiting_string;
    std::string id_string;
    std::string title_string;



    if(file.is_open())
    {
        getline(file,line_skipped);
        
        while(i<size)
        {   
            character = file.get();

            if(character<=57 && character>47)
            {    
                while(character !=',')
                {
                    id_string+=character;
                    character = file.get(); 
                }

                id_read = std::stoi(id_string);
            }
            else 
                file >> id_string; // skip element with wrong form

            
            character = file.get();

            while(!raiting_found && character!='\n' && !next)
            {
                if(character == ',')
                {    
                    character = file.get();
                    if(character=='\n')
                        next=1;

                   
                    if(character<=57 && character>47)
                    {
                        raiting_found=1;  
                        file >> raiting_string;
                        raiting_read = std::stod(character+raiting_string);
                        count++;
                    }
                    character='\0';
                }
                if(!next)
                {
                    title_read+=character;
                    character = file.get();
                }
            }
            
            if(raiting_found && id_read!=-1)
            {  
                array[i].setId(id_read);
                array[i].setTitle(title_read);
                array[i].setRaiting(raiting_read);
            }
            raiting_found=0;
            next=0;
            id_read=-1;
            raiting_read=-1;
 

            /* 
            std::cout << "id_read: "<< id_read <<std::endl;
            std::cout << "title_read: "<< title_read <<std::endl;
            std::cout << "raiting_read: "<< raiting_read <<std::endl;

            std::cout << std::endl;

            std::cout << "id_array: "<< array[i].getId()<<std::endl;
            std::cout << "title_array: "<< array[i].getTitle() <<std::endl;
            std::cout << "raiting_array: "<< array[i].getRaiting() <<std::endl;

            std::cout << "===============================" <<std::endl; */

            i++;
            title_read.clear();
            id_string.clear();
        };
    }
    file.close();
    
   /*  if(count==size)
        std::cout << "All data is correct" << std::endl;
    else
        std::cout << "Only " << count << " is correct" << std::endl; */

    
    
    return array;
}

void swap(Movie& a, Movie& b)
{
    Movie tmp = a;
    a = b;
    b = tmp;
}

void quickSort(Movie *array, int first, int last)
{
    int i,j;
    int pivot=first;
    int count = 0;
    
    for (int k = first + 1; k <= last; k++) 
    {
        if (array[k].getRaiting() <= array[pivot].getRaiting())
            count++;
    }
    pivot = first + count;
    swap(array[pivot], array[first]);
    
    i = first;
    j = last;
    
    if(first>=last)
        return;
    else
    {
        while (i < pivot && j > pivot)
        {
            
            while (array[i].getRaiting() <= array[pivot].getRaiting())
                i++; 
        
            while (array[j].getRaiting() > array[pivot].getRaiting())
                j--;
            
            if(i < pivot && j > pivot)
            {
                swap(array[i], array[j]);
                i++; 
                j--;
            }
        }    
        quickSort(array, first, pivot-1); //sort left part
        quickSort(array, pivot+1, last); //sort right part
    }
}

int main()
{
    Movie *array = new Movie[size];
    read(array);

    for(int i=0;i<size; i++)
        std::cout<< array[i].getRaiting() <<" ";

    std::cout << std::endl;

    quickSort(array, 0, size-1);

    for(int i=0;i<size; i++)
        std::cout<< array[i].getRaiting() <<" ";

    std::cout << std::endl; 

    return 0;
}