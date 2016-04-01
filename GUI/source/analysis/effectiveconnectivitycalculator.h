#ifndef EFFECTIVECONNECTIVITYCALCULATOR
#define EFFECTIVECONNECTIVITYCALCULATOR

#include<random>

/*
 *          TODO : Finish this file, then add values here in updatebot(), then
 *                 calculate MI or anything else also in updatebot() instead of
 *                 paintevent..!
 */


class EffectiveConnectivityCalculator{
    unsigned _history; // The duration that we need to keep information here.
    unsigned _size; // The size of the one dimension of the histogram
    unsigned long **data;
    unsigned long counter; // Counts all elements so we know when we exceed the
                           // size of the buffer (history).

    std::mt19937 generator;
    std::uniform_real_distribution<double> dis;

public:
    EffectiveConnectivityCalculator():
        _history(100), _size(100), counter(0), generator(123),
        dis(0.0, 1.0){
        this->init();
    }

    EffectiveConnectivityCalculator(const unsigned &num_of_elem, const unsigned history):
        _history(history), _size(num_of_elem),
        counter(0),generator(123), dis(0.0, 1.0){
        this->init();
    }

    ~EffectiveConnectivityCalculator(){
        for(int i=0; i<_size; i++){
            delete[] data[i];
        }
        delete[] data;
    }

    void init(){
        data = new unsigned long * [_size];
        for(int i=0; i<_size; i++){
            data[i] = new unsigned long[_size];
            for(int j=0; j<_size; j++){
                data[i][j] = 0;
            }
        }
    }

    void add(const double &source_fr, const double &target_fr){
        // Find the correct cell
        // data[i][j]++;

        if(counter >= _history){
            // select randomly x, y
            unsigned x=0, y=0;

            do{
                x = (unsigned)(dis(generator)*(double)_size);
                y = (unsigned)(dis(generator)*(double)_size);
            } while(x >= _size || y >= _size);

            data[x][y]--;
        }
        else{
            // Update time counter
            counter++;
        }
    }

    unsigned size(){ return _size; }
    unsigned history(){ return _history; }

};

#endif // EFFECTIVECONNECTIVITYCALCULATOR


