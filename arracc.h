#ifndef ARRACC_H
#define ARRACC_H

#include <vector>

template <class T>
class array_backup{

    private:
        T* arr;
        T* copy;
        int size;

    public:
        array_backup(T* const arr, int size) : arr(arr), copy(new T[size]), size(size) {
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
    
    private:
        std::vector<class array_backup<T>* > backup_storage;
        int size_of_arracc;
        T* last_element;

        void accumulate(T* const arr, int size) {
            for(int i = 1; i < size; i++){
                arr[i] = BinPred()(arr[i-1], arr[i]);
            }
        }

        void back_it_up(T* const arr, int size) {
            //adding a new array_backup to the backup_storage vector
            this->backup_storage.push_back(new array_backup<T>(arr, size));
        }

    public:
        array_accumulater(T* const arr, int size){
            back_it_up(arr, size);
            //saving the length of the first array stored in the vector
            this->size_of_arracc = size;
            //accumulating the array
            accumulate(arr, size);
            //pointing the last_element member to the last element of the array
            this->last_element = (arr + size -1);
        }

        ~array_accumulater(){
            //dependant scope
            typedef typename std::vector<class array_backup<T>* >::const_iterator iter;
            //de-allocating memory for the vector
            for (iter it = backup_storage.begin(); it != backup_storage.end(); ++it){
                delete *it;
            }
            //clearing the vector
            this->backup_storage.clear();
        }

        unsigned long size() const {
            return size_of_arracc;
        }

        void add(T* const arr, const int &size) {
            //creating a new instance and adding the new array_backup to the vector
            back_it_up(arr, size);
            //updating the combined length of the arrays stored in the vector
            this->size_of_arracc += size;
            //setting the first element of the new array to the last element of the previous array
            *arr = *(this->last_element) + *(arr);
            //accumulating the array
            accumulate(arr, size);
            //updating the last_element member
            this->last_element = arr + size - 1;
        }
};

#endif // ARRACC_H
