#ifndef STDPFUNCTION_H
#define STDPFUNCTION_H

#include <QtGui/QPainter>
#include <math.h>

/*if(mode == Block::modeG){
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine));
    painter.drawLine(stdpCentreX, stdpCentreY - stdpGraphHeight/2,
                     stdpCentreX, stdpCentreY + stdpGraphHeight/2);

    painter.drawLine(stdpCentreX - stdpGraphWidth/2, stdpCentreY,
                     stdpCentreX + stdpGraphWidth/2, stdpCentreY);

    //A->stdp.draw(&painter, centreX, centreY, graphWidth, graphHeight);
    if(STDP != NULL)
        STDP->draw(&painter, stdpCentreX, stdpCentreY, stdpGraphWidth,
                   stdpGraphHeight);
    return;
}*/



class STDPfunction{
    int _preLength, _postLength;
    double _aPlus, _aMinus, _tPlus, _tMinus, _minExWeight, _maxExWeight;
    double _Asym, _ta, _tb, Amax;
    double _minInWeight, _maxInWeight;  // smallest(largest) in absolute terms
    float* _preArray;
    float* _postArray;
    int _mode;

public:
    STDPfunction();
    void draw(QPainter *painter, int centreX,int centreY,int width,int height);

    void setStandardFunction();
    void setSymmetricFunction();
    void setOtherFunction();
    void setMode(int value);
    void buildArrayStandard();
    void buildArraySymmetric();
    void buildArrayOther();

    // ACCESSORS
    float* preArray(){ return _preArray; }
    float* postArray(){ return _postArray; }
    int preLength(){ return _preLength; }
    int postLength(){ return _postLength; }
    double aPlus(){ return _aPlus; }
    double aMinus(){ return _aMinus; }
    double tPlus(){ return _tPlus; }
    double tMinus(){ return _tMinus; }
    double Asym(){ return _Asym; }
    double ta(){ return _ta; }
    double tb(){ return _tb; }
    double minExWeight(){ return _minExWeight; }
    double maxExWeight(){ return _maxExWeight; }
    double minInWeight(){ return _minInWeight; }
    double maxInWeight(){ return _maxInWeight; }
    int mode(){ return _mode; }

    // MODIFIERS
    void setPreLength(int value){ _preLength = value; }
    void setPostLength(int value){ _postLength = value; }
    void setAPlus(double value){ _aPlus = value; }
    void setAMinus(double value){ _aMinus = value; }
    void setTPlus(double value){ _tPlus = value; }
    void setTMinus(double value){ _tMinus = value; }
    void setAsym(double value){ _Asym = value; }
    void setTa(double value){ _ta = value; }
    void setTb(double value){ _tb = value; }
    void setMinExWeight(double value){ _minExWeight = value; }
    void setMaxExWeight(double value){ _maxExWeight = value; }
    void setMinInWeight(double value){ _minInWeight = value; }
    void setMaxInWeight(double value){ _maxInWeight = value; }
};

#endif // STDPFUNCTION_H


