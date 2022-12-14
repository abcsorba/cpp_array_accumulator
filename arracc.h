#ifndef ARRACC_H
#define ARRACC_H

#include <vector>

template <class T, class BinPred >
void accumulate(T* const arr, const int &size) {
    for (T *p = arr + 1; p != arr + size; ++p) {
        *p = BinPred()(*(p-1), *p);
    }
}

template <class T>
class array_backup{
   
    private:
        T* arr;
        T* copy;
        int size;
    public:
        array_backup(T* const arr, const int &size){
            this->arr = arr;
            this->copy = new T[size];
            this->size = size;
            //saving the original array
            for(int i = 0; i < size; i++){
                this->copy[i] = arr[i];
            }
        }

        ~array_backup(){
            //reinstating the original array
            for(int j = 0; j < this->size; j++){
                this->arr[j] = this->copy[j];
            }
            //de-allocating memory
            delete[] this->copy;
        }
};

template <class T, class BinPred = std::plus<T> >
class array_accumulater{
    //dependant scope
    typedef typename std::vector<class array_backup<T>* >::const_iterator iter;
    private:
        std::vector<class array_backup<T>* > vec;
        int size_of_arracc;
        T* last_element;
    public:
        array_accumulater(T* const arr, const int &size){
            //constructing the class
            array_backup<T>* temp = new array_backup<T>(arr, size);
            //saving the combined length of the arrays stored in the vector
            this->size_of_arracc = size;
            //accumulating the array
            accumulate<T,BinPred>(arr, size);
            //initializing the last_element member
            this->last_element = new T;
            //saving the last element
            *(this->last_element) = *(arr + size - 1);
            //adding the array_backup to the vector
            this->vec.push_back(temp);
        }

        ~array_accumulater(){
            //de-allocating memory for the vector
            for (iter it = vec.begin(); it != vec.end(); ++it){
                delete *it;
            }
            //de-allocating memory for last_element
            delete this->last_element;
            //clearing the vector
            this->vec.clear();
        }

        unsigned long size() const {
            return size_of_arracc;
        }

        void add(T* const arr, const int &size) {
            //creating a new instance and adding the new array_backup to the vector
            array_backup<T>* temp = new array_backup<T>(arr, size);
            //updating the combined length of the arrays stored in the vector
            this->size_of_arracc += size;
            //setting the first element of the new array to the last element of the previous array
            *arr = *(this->last_element) + *(arr);
            //accumulating the array
            accumulate<T,BinPred>(arr, size);
            //updating the last element
            *(this->last_element) = *(arr + size - 1);
            //adding the array_backup to the vector
            this->vec.push_back(temp);
        }
};

#endif // ARRACC_H