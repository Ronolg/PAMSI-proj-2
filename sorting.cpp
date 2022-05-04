#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>


const long long int size = 400000;

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

struct Bucket
{
    int size;     // number of elements in the bucket
    int capacity; // memory of the bucket
    Movie* movies; // data

};


Movie* read(Movie *array)
{
    std::ifstream file("projekt2_dane.csv");
    std::string line_skipped;
    char character; 
    int id_read; 
    std::string title_read;
    double raiting_read;
    bool raiting_found=0;
    bool next=0;
    int i=0;
    int j=0;

    std::string raiting_string; // variable used to read raitings
    std::string id_string; // variable used to read id's
    std::string title_string; // variable used to read titles



    if(file.is_open())
    {
        getline(file,line_skipped); // skip first line: ",movie,rating"
        
        while(i<size)
        {   
            character = file.get();

            if(character<=57 && character>47) // check whether read character is a number
            {    
                while(character !=',')//reading id as a string
                {
                    id_string+=character;
                    character = file.get(); 
                }

                id_read = std::stoi(id_string);//convert string to int
            }
            else 
                file >> id_string; // skip element with wrong form

            
            character = file.get();

            // reading title and raiting
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
                for(j=0; j<=i ; j++)
                {
                    if(array[j].getRaiting()==-1)
                    { 
                        array[j].setId(id_read);
                        array[j].setTitle(title_read);
                        array[j].setRaiting(raiting_read);
                    }
                }
            }
            
            raiting_found=0;
            next=0;
            id_read=-1;
            raiting_read=-1;
 

             
            /* std::cout << "id_read: "<< id_read <<std::endl;
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
        if(pivot>first)
            quickSort(array, first, pivot-1); //sort left part
        
        if(pivot<last)
            quickSort(array, pivot+1, last); //sort right part
    }
}

void bucketSort(Movie *array)
{
    Movie* bucket_array[10];
    int max_raiting=10;
    int counters[10]={};
    int default_size = size/5;
    int current_size = default_size;
    
    for(int i = 0; i<max_raiting; i++)
        bucket_array[i] = new Movie[default_size];
 
    for(int i = 0; i<size; i++)
    {
        
        if(array[i].getRaiting()<=10)
        {

                if(counters[(int)array[i].getRaiting()-1]>=current_size) //check if bucket has enough memory to store another element
                {
                    Movie* copy = bucket_array[(int)array[i].getRaiting()-1]; // create a copy of right bucket
                    current_size = counters[(int)array[i].getRaiting()-1]+default_size;
                    bucket_array[(int)array[i].getRaiting()-1] = new Movie[current_size]; // allocate more memory for the bucket
                    
                    for(int k=0; k<counters[(int)array[i].getRaiting()-1]; k++)
                        bucket_array[(int)array[i].getRaiting()-1][k] = copy[k];
                   
                    delete[] copy;
                }
                bucket_array[(int)array[i].getRaiting()-1][counters[(int)array[i].getRaiting()-1]] = array[i];
                counters[(int)array[i].getRaiting()-1]++;   
        }
    }
  
    
    int idx=0;
    for(int i = 0; i<max_raiting; i++)
        for(int j = 0; j<counters[i]; j++)
            array[idx++]= bucket_array[i][j];
    
    //free used memory
    for(int i = 0; i<max_raiting; i++)
        delete[] bucket_array[i];

}

void bucketSort_object(Movie *array)
{
    Bucket bucket_array[10];
    int max_raiting=10;
    int default_size = size/5;
    
    for(int i = 0; i<max_raiting; i++)
    {
        bucket_array[i].movies = new Movie[default_size];
        bucket_array[i].capacity = default_size;
        bucket_array[i].size = 0;
    }
    for(int i = 0; i<size; i++)
    {
        
        if(array[i].getRaiting()<=10)
        {
            if(bucket_array[(int)array[i].getRaiting()-1].size >= bucket_array[(int)array[i].getRaiting()-1].capacity)
            {
                Movie* copy = bucket_array[(int)array[i].getRaiting()-1].movies; // create a copy of right bucket

                bucket_array[(int)array[i].getRaiting()-1].capacity *= 2;
                bucket_array[(int)array[i].getRaiting()-1].movies = new Movie[bucket_array[(int)array[i].getRaiting()-1].capacity];

                for(int k=0; k<bucket_array[(int)array[i].getRaiting()-1].size; k++)
                    bucket_array[(int)array[i].getRaiting()-1].movies[k] = copy[k];
                   
                delete[] copy;
            }
            bucket_array[(int)array[i].getRaiting()-1].movies[bucket_array[(int)array[i].getRaiting()-1].size] = array[i];
            bucket_array[(int)array[i].getRaiting()-1].size++;
        }
    }
  
    int idx=0;
    for(int i = 0; i<max_raiting; i++)
        for(int j = 0; j<bucket_array[i].size; j++)
            array[idx++]= bucket_array[i].movies[j];

    std::cout << "Sizes: "<< std::endl;
    
    for(int i = 0; i<max_raiting; i++)
        std::cout << bucket_array[i].size << " ";
    std::cout << std::endl;
    
    //free used memory
    for(int i = 0; i<max_raiting; i++)
        delete[] bucket_array[i].movies;

}
int main()
{
    Movie *array = new Movie[size];
    read(array);

    for(int i=0;i<size; i++)
        std::cout<< array[i].getRaiting() <<" ";

    std::cout << std::endl;
    std::cout << std::endl;



    /* for(int i=0;i<size; i++)
        std::cout<< i <<": "<< array[i].getTitle()<<std::endl;

    std::cout << std::endl;
 */
    /* std::array<Movie,size> arr;
    for(int i=0; i<size; i++)
        arr[i]=array[i];
    
    std::sort(arr.begin(), arr.end(),[](Movie& a, Movie& b) -> bool {return a.getRaiting() > b.getRaiting();}); */
 
    quickSort(array, 0, size-1);
    //bucketSort(array);
    //bucketSort_object(array);
    
    for(int i=0;i<size; i++)
        std::cout<< array[i].getRaiting() <<" "; //<< "id: " << array[i].getId() << " ";

    std::cout << std::endl;

    /* for(int i=0;i<size; i++)
        std::cout<< i <<": "<< array[i].getTitle()<<std::endl;
    
    std::cout << std::endl;
    */
    delete[] array;
    return 0;
}