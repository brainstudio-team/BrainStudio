#include "stdpfunction.h"

STDPfunction::STDPfunction(){
    // Length (in cycles) of the part of the STDP window that precedes the
    // postsynaptic firing
    // Length (in cycles) of the part of the STDP window that comes after the
    // postsynaptic firing.

    _preArray = NULL;
    _postArray = NULL;

    // Set stdp function
    setStandardFunction();
}

void STDPfunction::setStandardFunction(){
    _mode = 0;
    _preLength = 20;
    _postLength = 20;

    _aPlus = 0.05; //0.1;
    _aMinus = 0.05;//0.12;
    _tPlus = 20;   //20
    _tMinus = 20;  //20

    _minExWeight = 0.0;
    _maxExWeight = 10.0;
    _minInWeight = 0.0;   // smallest in absolute terms
    _maxInWeight = -10.0; // largest in absolute terms

    buildArrayStandard();
}

void STDPfunction::setSymmetricFunction(){
    _mode = 1;
    // The time window between presynaptic and postsynaptic neurons that
    // are included in STDP is set to 50 msec.
    _preLength = 30;    //originally 50
    _postLength = 30;   //originally 50

    _Asym = 0.1;//0.07; // originally 0.05  // general amplifier
    _ta=2.5;            // originally 10.0   // for potentiation
    _tb=3.5;            // originally 10.0   // for depression

    _minExWeight = 0.001;  // originally 0
    _maxExWeight = 10.0;    // originally 10.0
    _minInWeight = -0.001; // originally 0
    _maxInWeight = -10.0;   // originally -10.0

    buildArraySymmetric();
}

void STDPfunction::setOtherFunction(){ // David's parameters
    _mode = 2;
    _preLength = 20;
    _postLength = 20;

    _aPlus = 0.1;
    _aMinus = 0.12;
    _tPlus = 20;
    _tMinus = 20;

    _minExWeight = 0.0;
    _maxExWeight = 10.0;
    _minInWeight = 0.0;   // smallest in absolute terms
    _maxInWeight = -10.0; // largest in absolute terms

    buildArrayOther();
}

void STDPfunction::setMode(int value){
    _mode = value;
    switch(_mode){
    case 0:
        buildArrayStandard();
        break;
    case 1:
        buildArraySymmetric();
        break;
    case 2:
        buildArrayOther();
        break;
    default:;
    }
}

void STDPfunction::buildArrayStandard(){
    if(_aPlus > _aMinus)    Amax = _aPlus;
    else                    Amax = _aMinus;
    float dt;    int i;

    //Build the arrays specifying the function
    if(_preArray != NULL) delete [] _preArray;
    if(_postArray != NULL) delete [] _postArray;
    _preArray = new float[_preLength];
    _postArray = new float[_postLength];

    for(i=0; i<_preLength; ++i){
        dt = float(i + 1);
        _preArray[i] = _aPlus * expf( (-1.0 * dt) / _tPlus);
    }
    for(i=0; i<_postLength; ++i){
        dt = float(i + 1);
        _postArray[i] = -1.0 * _aMinus * expf( (-1.0 * dt) / _tMinus);
    }
}

void STDPfunction::buildArraySymmetric(){
    Amax = _Asym;
    float dt;
    int i;

    //Build the arrays specifying the function
    if(_preArray != NULL) delete [] _preArray;
    if(_postArray != NULL) delete [] _postArray;
    _preArray = new float[_preLength];
    _postArray = new float[_postLength];

    for(i=0; i<_preLength; ++i){
        dt = float(i + 1);
        _preArray[i] = _Asym * ( 1 - (dt/_ta)*(dt/_ta) ) * expf( - dt / _tb);
    }
    for(i=0; i<_postLength; ++i){
        dt = float(i + 1);
        _postArray[i] = _Asym * ( 1 - (dt/_ta)*(dt/_ta) ) * expf( - dt / _tb);
    }
}

void STDPfunction::buildArrayOther(){ // David's parameters
    if(_aPlus > _aMinus)    Amax = _aPlus;
    else                    Amax = _aMinus;
    float dt;
    int i;

    //Build the arrays specifying the function
    if(_preArray != NULL) delete [] _preArray;
    if(_postArray != NULL) delete [] _postArray;
    _preArray = new float[_preLength];
    _postArray = new float[_postLength];

    for(i=0; i<_preLength; ++i) {
        dt = float(i + 1);
        _preArray[i] = _aPlus * expf( (-1.0 * dt) / _tPlus);

    }
    for(i=0; i<_postLength; ++i) {
        dt = float(i + 1);
        _postArray[i] = -1.0 * _aMinus * expf( (-1.0 * dt) / _tMinus);
    }
}


void STDPfunction::draw(QPainter *painter, int centreX, int centreY,
                        int width, int height){
    painter->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
    int i;
    for(i=0; i<_preLength; i++){
        painter->drawLine(centreX - (width/(2*_preLength))*i,
                          centreY - (height/(2*Amax))*_preArray[i],
                          centreX - (width/(2*_preLength))*(i+1),
                          centreY - (height/(2*Amax))*_preArray[i+1]);

    }
    for(i=0; i<_postLength; i++){
        painter->drawLine(centreX + (width/(2*_postLength))*i,
                          centreY - (height/(2*Amax))*_postArray[i],
                          centreX + (width/(2*_postLength))*(i+1),
                          centreY - (height/(2*Amax))*_postArray[i+1]);
    }

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));

    painter->drawText(centreX+30+width/2,centreY-height/2, "preLength: " +
                      QString::number(_preLength));
    painter->drawText(centreX+30+width/2,centreY-height/2+20, "postLength: " +
                      QString::number(_postLength));
    painter->drawText(centreX+30+width/2,centreY-height/2+40, "minExWeight: " +
                      QString::number(_minExWeight));
    painter->drawText(centreX+30+width/2,centreY-height/2+60, "maxExWeight: " +
                      QString::number(_maxExWeight));
    painter->drawText(centreX+30+width/2,centreY-height/2+80, "minInWeight: " +
                      QString::number(_minInWeight));
    painter->drawText(centreX+30+width/2,centreY-height/2+100, "maxInWeight: " +
                      QString::number(_maxInWeight));
    if(_mode == 1){
        painter->drawText(centreX+30+width/2,centreY-height/2+130, "Asym: " +
                          QString::number(_Asym));
        painter->drawText(centreX+30+width/2,centreY-height/2+150, "ta: " +
                          QString::number(_ta));
        painter->drawText(centreX+30+width/2,centreY-height/2+170, "tb: " +
                          QString::number(_tb));
    }
    else{
        painter->drawText(centreX+30+width/2,centreY-height/2+130, "aPlus: " +
                          QString::number(_aPlus));
        painter->drawText(centreX+30+width/2,centreY-height/2+150, "aMinus: " +
                          QString::number(_aMinus));
        painter->drawText(centreX+30+width/2,centreY-height/2+170, "tPlus: " +
                          QString::number(_tPlus));
        painter->drawText(centreX+30+width/2,centreY-height/2+190, "tMinus: " +
                          QString::number(_tMinus));
    }
}





















