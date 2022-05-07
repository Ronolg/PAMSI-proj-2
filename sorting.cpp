#include <iostream>
#include <fstream>
#include <algorithm>


const long long int size = 2*262144; //rozmiar sortowanej tablicy
//const int max_id = 1010291; //id ostatniego elementu w danych

class Movie
{
    private:
        int id;
        double raiting;
    public:
    void setId(int a){this->id = a;}
    void setRaiting(double r){this->raiting = r;}

    int getId(){return this->id;}
    double getRaiting(){return this->raiting;}

    Movie()
    {
        this->id=-1;
        this->raiting=-1;
    }

    Movie operator=(Movie b)
    {
        this->setId(b.getId());
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
    double raiting_read;
    bool raiting_found=0;
    bool next=0;
    int i=0;
    int quotemark_count = 0;

    std::string raiting_string; // variable used to read raitings
    std::string id_string; // variable used to read id's



    if(file.is_open())
    {
        getline(file,line_skipped); // skip first line: ",movie,rating"
        
        while(i<size && !file.eof())
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
                
                //character = file.get();
                
                if(character == '"') 
                    quotemark_count++; //when " is found, count it 
                

                if(character==',' && quotemark_count!=1 && !next)
                {
                    character = file.get();
                    if(character<=57 && character>47)
                    {
                        file >> raiting_string;
                        raiting_read = std::stod(character+raiting_string);
                        raiting_found=1;
                    }
                    //next=1;

                }    
                if(character=='\n') //check if it's end of the line
                    next=1;
                
                if(!next)
                    character = file.get();
            }
            
            if(raiting_found && id_read!=-1)
            {  
                while(i>0 && array[i-1].getRaiting()==-1)
                    i--;
                
                array[i].setId(id_read);
                array[i].setRaiting(raiting_read);
                
              /*   if(i>0 && array[i-1].getRaiting()==-1)
                { 
                    array[i-1].setId(id_read);
                    array[i-1].setRaiting(raiting_read);
                    i--;
                }
                else
                {
                    array[i].setId(id_read);
                    array[i].setRaiting(raiting_read);
                } */
            }
            
            raiting_found=0;
            next=0;
            id_read=-1;
            raiting_read=-1;
            quotemark_count=0;
 

            
            //std::cout << "Id: " << array[i].getId()<< std::endl;
            //std::cout << "i: " << i << std::endl; 


             
            /* std::cout << "id_read: "<< id_read <<std::endl;
            std::cout << "raiting_read: "<< raiting_read <<std::endl;

            std::cout << std::endl;

            std::cout << "id_array: "<< array[i].getId()<<std::endl;
            std::cout << "raiting_array: "<< array[i].getRaiting() <<std::endl;

            std::cout << "===============================" <<std::endl;
  */    
            i++;
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
        
        if(array[i].getRaiting()<=10 && array[i].getRaiting()>=0)
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

    
    //free used memory
    for(int i = 0; i<max_raiting; i++)
        delete[] bucket_array[i].movies;

}

void merge(Movie *merged_array, Movie* array_left, Movie* array_right, int sizeLeft, int sizeRight)
{
    int i = 0;
    int j = 0;
    int k = 0;

    while(i<sizeLeft && j<sizeRight)
    {
        if(array_left[i].getRaiting() <= array_right[j].getRaiting())
        {
            merged_array[k] = array_left[i];
            i++;
        }
        else
        {
            merged_array[k] = array_right[j];
            j++;
        }
        k++;
    };

    while(i<sizeLeft) //add leftovers of left array to merged array if there are any
    {
        merged_array[k] = array_left[i];
        i++; k++;
    };

    while(j<sizeRight) //add leftovers of right array to merged array if there are any
    {
        merged_array[k] = array_right[j];
        j++; k++;
    };
}

Movie* divideArray(Movie *array, int divided_size , bool which_half /*0 -> 1st half, 1 -> 2nd half*/)
{
    Movie* divided_array = new Movie[divided_size];

    if(!which_half)
    {
        for(int i=0; i<divided_size; i++)
            divided_array[i] = array[i];
    }
    else
    {
        for(int i=0; i<divided_size; i++)
            divided_array[i] = array[divided_size+i];
    }

    
    return divided_array;
}   

void mergeSort(Movie *array, int array_size)
{
    int leftSize = array_size/2;
    int rightSize = array_size/2;

    Movie* leftHalf = divideArray(array, leftSize, 0);
    Movie* rightHalf = divideArray(array, rightSize, 1);
    
    if(array_size<2)
        return;


    mergeSort(leftHalf, leftSize);
    mergeSort(rightHalf, rightSize);
    merge(array, leftHalf, rightHalf, leftSize, rightSize);

} 



int main()
{
    Movie *array = new Movie[size];
    read(array);

    for(int i=0;i<size; i++)
        std::cout<< array[i].getRaiting() <<" ";

    std::cout << std::endl;
    std::cout << std::endl;

    //quickSort(array, 0, size-1);
    //bucketSort_object(array);
    mergeSort(array,size);

    for(int i=0;i<size; i++)
        std::cout<< array[i].getRaiting() <<" ";//<< "id: " << array[i].getId() << " ";

    std::cout << std::endl;

    delete[] array; 

    return 0;
}