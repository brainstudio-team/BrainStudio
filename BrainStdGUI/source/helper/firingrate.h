#ifndef FIRINGRATE_H
#define FIRINGRATE_H

#include <QList>

#include <iostream>

class FiringRate{
protected:
    QList<float> lastValues;
    int depth;
    float sum, a;
    float result;

public:
    FiringRate(int _depth = 20):depth(_depth),sum(0){depth=30; result = 0;}

    //MODIFIERS:
    void setDepth(int value){ depth = value; }
    void justSet(float value){ result = value; }

    //ACCESSORS:
    float getFR() const { return result;}
    int getDepth() const { return depth; }
    QList<float> getLastValues() const { return lastValues; }

    //METHODS:
    void nextFR(float newValue){
        if(lastValues.size() < depth){
            //std::cout << "LESS" << " depth=" << depth;
            sum+=newValue;
            lastValues.append(newValue);
            //std::cout << " sum=" << sum << " size=" << lastValues.size() << " returns: " << sum/(float)lastValues.size() << std::endl;
            result = sum/(float)depth;
        }
        else if(lastValues.size() == depth){
            //std::cout << "NORMAL" <<" depth=" << depth;
            a = lastValues.takeFirst();
            sum-=a;
            sum+=newValue;
            lastValues.append(newValue);
            //std::cout << " sum=" << sum << " size=" << lastValues.size() << " returns: " << sum/(float)lastValues.size() << std::endl;
            result = sum/lastValues.size();
        }
        else{
            //std::cout << "MORE" << " depth=" << depth;
            a = lastValues.takeFirst();
            sum-=a;
            //std::cout << " sum=" << sum << " size=" << lastValues.size() << " returns: " << sum/(float)lastValues.size() << std::endl;
            result = sum/(float)lastValues.size();
        }
    }

    bool fired() const { if(!lastValues.isEmpty() && lastValues.last()==1.0f) return true; return false;}
};

#endif // FIRINGRATE_H














