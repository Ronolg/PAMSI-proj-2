#include <iostream>
#include <fstream>
#include <chrono>


long long int size = 20; //size of main array

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
        getline(file,line_skipped); // skip first line: ",movie,rating"S
        
        while(i<size && !file.eof())
        {   
            // reading id
            
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
            while(!raiting_found && character!='\n' && !next && !file.eof())
            {
                
                //character = file.get();
                
                if(character == '"') // title is put into " title ", when comma occurs in a title 
                    quotemark_count++; //when " is found, count it 
                

                if(character==',' && quotemark_count!=1 && !next)
                {
                    character = file.get();
                    if(character<=57 && character>47) // check if character is a number (based on ASCII table)
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
                while(i>0 && array[i-1].getRaiting()==-1) // check if in array are elements with wrong form ( raiting or id eguals -1) and then treat it  
                    i--;                                  // as a free spot for another element with correct form.
                
                array[i].setId(id_read);
                array[i].setRaiting(raiting_read);
                
            }
            
            raiting_found=0;
            next=0;
            id_read=-1;
            raiting_read=-1;
            quotemark_count=0;
 
            i++;
            id_string.clear();

        };

        if(i<size)//when created array is bigger than numbers of correct elements in file. In "projekt2_dane.csv" there are approx. 962 903 elements with correct form. 
        {
            i--;
            size = i;
        }
    }
    file.close();

    return array;
}

void swap(Movie& a, Movie& b) // swaps 2 objects of Movie
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

    // determine value of pivot
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
            if(bucket_array[(int)array[i].getRaiting()-1].size >= bucket_array[(int)array[i].getRaiting()-1].capacity) // if there is not enough space in a bucket for next element,
            {                                                                                                          // then allocate more memory for this bucket 
                Movie* copy = bucket_array[(int)array[i].getRaiting()-1].movies; // create a copy of right bucket

                bucket_array[(int)array[i].getRaiting()-1].capacity *= 2; // increase capacity of the bucket 
                bucket_array[(int)array[i].getRaiting()-1].movies = new Movie[bucket_array[(int)array[i].getRaiting()-1].capacity]; // allocate more memory for the backet, based on value of capacity

                for(int k=0; k<bucket_array[(int)array[i].getRaiting()-1].size; k++) 
                    bucket_array[(int)array[i].getRaiting()-1].movies[k] = copy[k];
                   
                delete[] copy;
            }
            bucket_array[(int)array[i].getRaiting()-1].movies[bucket_array[(int)array[i].getRaiting()-1].size] = array[i]; // put element in right bucket
            bucket_array[(int)array[i].getRaiting()-1].size++;
        }
    }
  
    // sort the array
    int idx=0;
    for(int i = 0; i<max_raiting; i++)
        for(int j = 0; j<bucket_array[i].size; j++)
            array[idx++]= bucket_array[i].movies[j];

    
    //free used memory
    for(int i = 0; i<max_raiting; i++)
        delete[] bucket_array[i].movies;

}

void merge(Movie *merged_array, Movie* array_left, Movie* array_right, int sizeLeft, int sizeRight) //merges two arrays into one (in sorted order)
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

void mergeSort(Movie *array, int array_size)
{
    int leftSize = array_size/2;
    int rightSize = array_size/2;
    
    if(array_size%2!=0)//check if array size is an odd number. If it is right "half" will be longer by one element
        rightSize++;
    
    Movie* leftHalf = new Movie[leftSize];
    Movie* rightHalf = new Movie[rightSize];
     
    for(int i=0; i<leftSize; i++) // creating left sub array        
        leftHalf[i] = array[i];

    for(int i=0; i<rightSize; i++) // creating right sub array 
        rightHalf[i] = array[array_size/2+i];
    
    if(array_size<2) 
        return;

    mergeSort(leftHalf, leftSize);
    mergeSort(rightHalf, rightSize);
    merge(array, leftHalf, rightHalf, leftSize, rightSize);

    delete leftHalf;
    delete rightHalf;

} 

double mean(Movie* array) // returns mean value of given array
{
    double mean;

    if(size%2==0)
        mean = (array[size/2].getRaiting() + array[size/2 - 1].getRaiting())/2;
    else
        mean = array[size/2].getRaiting();

    return mean;

}

double avarage(Movie* array) // returns avarage value of given array
{
    long double sum = 0;

    for(int i=0; i<size; i++)
        sum+= (double)array[i].getRaiting();

    return sum/size;

}

int main()
{
    Movie *array = new Movie[size];
    
    std::chrono::steady_clock::time_point begin_read = std::chrono::steady_clock::now(); //gets time when reading started
    
    read(array);
    
    std::chrono::steady_clock::time_point end_read = std::chrono::steady_clock::now(); //gets time when reading is finished

    std::cout << "Read time: " << std::chrono::duration_cast<std::chrono::microseconds>(end_read - begin_read).count() << " microseconds"<< std::endl;
    
    std::cout << "Unsorted array: "<<std::endl;
 
    for(int i=0;i<size; i++) //print array (unsorted)
        std::cout<< array[i].getRaiting() <<" ";

    std::cout << std::endl;
    std::cout << std::endl; 
 
    std::chrono::steady_clock::time_point begin_sort = std::chrono::steady_clock::now();


    //quickSort(array, 0, size-1);
    //bucketSort(array);
    mergeSort(array,size);

    std::chrono::steady_clock::time_point end_sort = std::chrono::steady_clock::now();

    std::cout << "Sorted array: "<<std::endl;

    for(int i=0;i<size; i++) //print sorted array
        std::cout<< array[i].getRaiting() <<" ";

    std::cout << std::endl; 

    std::cout <<"Mean: " << mean(array) << std::endl;
    std::cout << "Avr: " << avarage(array) << std::endl;
    std::cout << "Sorting time: " << std::chrono::duration_cast<std::chrono::microseconds>(end_sort - begin_sort).count() << " microseconds"<< std::endl;
  
    delete[] array; 

    return 0;
}