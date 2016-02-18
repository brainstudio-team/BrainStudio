//SpikeStream includes
#include "TransferEntropyCalculator.h"
//#include "SpikeStreamAnalysisException.h"
using namespace spikestream;


/*! Number of bits in an unsigned variable */
#define NUM_BITS 32

/*! Output debug information about probabilities */
//#define DEBUG_PROBABILITIES

/*! Output debug information about the calculation of the sum */
//#define DEBUG_EQUATION


/*! Constructor */
TransferEntropyCalculator::TransferEntropyCalculator(unsigned k_param,
                                                     unsigned l_param,
                                                     unsigned timeWindow){
#ifdef LOGS
    //Run some checks
	if(k_param == 0 || l_param == 0 || timeWindow == 0)
        //throw SpikeStreamAnalysisException("Parameters cannot be zero");
        cout << "Parameters cannot be zero" << endl;
	if(k_param < l_param)
        //throw SpikeStreamAnalysisException(
        //"k_param must be greater than or equal to l_param");
        cout << "k_param must be greater than or equal to l_param" << endl;
	if(k_param > timeWindow)
        //throw SpikeStreamAnalysisException(
        //"k_param must be less or equal to the time window");
        cout << "k_param must be less or equal to the time window" << endl;
#endif

	//Store parameters
	this->k_param = k_param;
	this->l_param = l_param;
	this->timeWindow = timeWindow;
}


/*! Destructor */
TransferEntropyCalculator::~TransferEntropyCalculator(){
}


/*----------------------------------------------------------*/
/*------                PUBLIC METHODS                ------*/
/*----------------------------------------------------------*/

/*! Calculates the transfer entropy between the two sets of data for the time window specified.
	fromNeuronData contains list of unsigned 1s and 0s indicating the spike activity in J.
	toNeuronData contains list of unsigned 1s and 0s indicating the spike activity in I. */
double TransferEntropyCalculator::getTransferEntropy(unsigned startTimeStep,
                                                     QVector<unsigned>& iVector,
                                                     QVector<unsigned>& jVector){
#ifdef LOGS
	//Run checks
	if(startTimeStep + timeWindow > iVector.size())
        //throw SpikeStreamAnalysisException(
        //"Start time step + time window exceeds available data");
        cout << "Start time step + time window exceeds available data" << endl;
	if(iVector.size() != jVector.size())
        //throw SpikeStreamAnalysisException(
        //"From neuron data size does not match to neuron data size");
        cout <<"From neuron data size does not match to neuron data size"<<endl;
	if(startTimeStep + k_param >= iVector.size() || startTimeStep + l_param >= jVector.size())
        //throw SpikeStreamAnalysisException(
        //"StartTimeStep + parameter window is greater than vector size.");
        cout<<"StartTimeStep + parameter window is greater than vector size.\n";
#endif

	//Load all probabilities for the time sequence into hash maps
	load_I_k_probabilities(startTimeStep, iVector);
	load_I_k_plus_1_probabilities(startTimeStep, iVector);
	load_I_k_J_l_probabilities(startTimeStep, iVector, jVector);
	load_I_k_plus_1_J_l_probabilities(startTimeStep, iVector, jVector);


	//----------------------------------------------------------------
	//  Bit masks - these methods are copied from the load methods
	//----------------------------------------------------------------
	unsigned i_k_bitmask = 1<<k_param;
	--i_k_bitmask;

	unsigned i_k_plus_1_bitmask = 1<<(k_param+1);
	--i_k_plus_1_bitmask;

	unsigned i_k_j_l_bitmask = 1<<(k_param+l_param);
	--i_k_j_l_bitmask;
	i_k_j_l_bitmask ^= (1<<l_param);

	unsigned i_k_plus_1_j_l_bitmask = 1<<(k_param + 1 + l_param);
	--i_k_plus_1_j_l_bitmask;
	i_k_plus_1_j_l_bitmask ^= (1<<l_param);


    //--------------------------------------------------------------------------
    // Starting versions of numbers -
    //                            these methods are copied from the load methods
    //--------------------------------------------------------------------------
	//Load in first i_k number
	unsigned i_k = 0;
	for(unsigned b = startTimeStep; b < startTimeStep+k_param; ++b){
		i_k <<= 1;
		i_k |= iVector.at(b);
	}

	//Load in first i_k_plus_1 number
	unsigned i_k_plus_1 = 0;
	for(unsigned b = startTimeStep; b < startTimeStep+k_param+1; ++b){
		i_k_plus_1 <<= 1;
		i_k_plus_1 |= iVector.at(b);
	}

	//Load in first i_k_j_l number
	unsigned i_k_j_l = 0;
	for(unsigned b = startTimeStep; b < startTimeStep+k_param; ++b){
		i_k_j_l <<= 1;
		i_k_j_l |= iVector.at(b);
	}
	for(unsigned b = startTimeStep + k_param - l_param; b < startTimeStep+k_param; ++b){
		i_k_j_l <<= 1;
		i_k_j_l |= jVector.at(b);
	}

	//Load in first i_k_plus_1_j_l number
	unsigned i_k_plus_1_j_l = 0;
	for(unsigned b = startTimeStep; b < startTimeStep + k_param + 1; ++b){
		i_k_plus_1_j_l <<= 1;
		i_k_plus_1_j_l |= iVector.at(b);
	}
	for(unsigned b = startTimeStep + k_param - l_param; b < startTimeStep+k_param; ++b){
		i_k_plus_1_j_l <<= 1;
		i_k_plus_1_j_l |= jVector.at(b);
	}


	//---------------------------------------------------------
	//  Work through sequence and calculate transfer entropy
	//---------------------------------------------------------
	double p1, p2, p3, sum=0.0;
	for(unsigned b = (startTimeStep+k_param); b < startTimeStep+timeWindow; ++b){

		#ifdef DEBUG_EQUATION
			cout<<"i_k: "<<getBitString(i_k).toStdString()<<endl;
			cout<<"i_k_plus_1: "<<getBitString(i_k_plus_1).toStdString()<<endl;
			cout<<"i_k_j_l: "<<getBitString(i_k_j_l).toStdString()<<endl;
			cout<<"i_k_plus_1_j_l: "<<getBitString(i_k_plus_1_j_l).toStdString()<<endl;
		#endif//DEBUG_EQUATION

		//Get joint probability p(i_n+1, i_n^k, j_n^l)
		p1 = I_k_plus_1_J_1_probs[i_k_plus_1_j_l];

		// Calculate conditional probability p(i_n+1 | i_n^k, j_n^l): p(A|B) = p(A and B)/P(B); p(i_n+1 | i_n^k, j_n^l) = p(i_n+1, i_n^k, j_n^l) / p(i_n^k, j_n^l)
		p2 = p1/I_k_J_l_probs[i_k_j_l];//Should not =0 because sequence will have occurred at least once

		// Get conditional probability p(i_n+1 | i_n^k): p(i_n+1 | i_n^k) = p( i_n+1, i_n^k) / p(i_n^k)
		p3 = I_k_plus_1_probs[i_k_plus_1]/I_k_probs[i_k];//Should not =0 because sequence will have occurred at least once

		//Update sum using probabilities
		sum += p1 * log2 (p2 / p3);

		#ifdef DEBUG_EQUATION
			cout<<"p1: "<<p1<<"; p2: "<<p2<<"; p3: "<<p3<<"; result="<<(p1 * log2 (p2 / p3))<<endl;
		#endif//DEBUG_EQUATION

		//Avoid final advance in numbers, which could put data vectors out of bounds
		if(b == startTimeStep+timeWindow-1)
			break;

		//Advance numbers
		i_k <<= 1;
		i_k |= iVector.at(b);
		i_k &= i_k_bitmask;

		i_k_plus_1 <<= 1;
		i_k_plus_1 |= iVector.at(b+1);
		i_k_plus_1 &= i_k_plus_1_bitmask;

		i_k_j_l <<= 1;
		i_k_j_l &= i_k_j_l_bitmask;
		i_k_j_l |= (iVector.at(b)<<l_param);
		i_k_j_l |= jVector.at(b);

		i_k_plus_1_j_l <<= 1;
		i_k_plus_1_j_l &= i_k_plus_1_j_l_bitmask;
		i_k_plus_1_j_l |= (iVector.at(b+1)<<l_param);
		i_k_plus_1_j_l |= jVector.at(b);
	}

	//Return sum
	return sum;
}


/*! Loads probabilites, p(i_n^k), into I_k_probs hash map.
	Works through entire sequence, totals the number of times that each pattern occurs, and converts this into a probability.
	Made public to facilitate unit testing. */
void TransferEntropyCalculator::load_I_k_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector){
	unsigned patternCount = 0;
	I_k_probs.clear();

	//Mask to clear bits after shift
	unsigned clearEndBits = 1<<k_param;
	--clearEndBits;

	//Load in first number and count it
	unsigned tmpNum = 0;
	for(unsigned b = startTimeStep; b < startTimeStep+k_param; ++b){
		tmpNum <<= 1;
		tmpNum |= iVector.at(b);
	}
	I_k_probs[tmpNum] = 1.0;
	++patternCount;

	//Load in subsequent numbers
	for(unsigned b = startTimeStep+k_param; b < startTimeStep+timeWindow; ++b){
		tmpNum <<= 1;
		tmpNum |= iVector.at(b);
		tmpNum &= clearEndBits;

		//Add to hash map
		if(!I_k_probs.contains(tmpNum))
			I_k_probs[tmpNum] = 1.0;
		else
			++I_k_probs[tmpNum];
		++patternCount;//Could work this out, but safer just to add
	}

	//Convert count of instances into probabilites
	for(QHash<unsigned, double>::iterator iter = I_k_probs.begin(); iter != I_k_probs.end(); ++iter)
		iter.value() /= patternCount;

	#ifdef DEBUG_PROBABILITIES
		printMap(I_k_probs, "I_k_probs");
	#endif//DEBUG_PROBABILITIES
}


/*! Loads probabilites, p(i_n+1^k), into I_k_plus_1_probs hash map.
	Works through entire sequence, totals the number of times that each pattern occurs, and converts this into a probability.
	Made public to facilitate unit testing. */
void TransferEntropyCalculator::load_I_k_plus_1_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector){
	unsigned patternCount = 0;
	I_k_plus_1_probs.clear();

	//Mask to clear bits after shift
	unsigned clearEndBits = 1<<(k_param+1);
	--clearEndBits;

	//Load in first number and count it
	unsigned tmpNum = 0;
	for(unsigned b = startTimeStep; b < startTimeStep+k_param+1; ++b){
		tmpNum <<= 1;
		tmpNum |= iVector.at(b);
	}
	I_k_plus_1_probs[tmpNum] = 1.0;
	++patternCount;

	//Load in subsequent numbers
	for(unsigned b = startTimeStep+k_param+1; b < startTimeStep+timeWindow; ++b){
		tmpNum <<= 1;
		tmpNum |= iVector.at(b);
		tmpNum &= clearEndBits;

		//Add to hash map
		if(!I_k_plus_1_probs.contains(tmpNum))
			I_k_plus_1_probs[tmpNum] = 1.0;
		else
			++I_k_plus_1_probs[tmpNum];
		++patternCount;//Could work this out, but safer just to add
	}

	//Convert count of instances into probabilites
	for(QHash<unsigned, double>::iterator iter = I_k_plus_1_probs.begin(); iter != I_k_plus_1_probs.end(); ++iter)
		iter.value() /= patternCount;

	#ifdef DEBUG_PROBABILITIES
		printMap(I_k_plus_1_probs, "I_k_plus_1_probs");
	#endif//DEBUG_PROBABILITIES
}


/*! Loads probabilities p(i_n^k, j_n^l) into I_k_J_l_probs hash map.
	Works through the entire sequence, totals the number of times that each pattern occurs and converts this into a probability.
	Made publicly accessible to facilitate unit testing. */
void TransferEntropyCalculator::load_I_k_J_l_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector, QVector<unsigned>& jVector){
	if(startTimeStep + k_param >= iVector.size() || startTimeStep + l_param >= jVector.size())
        //throw SpikeStreamAnalysisException(
        //"StartTimeStep + parameter window is greater than vector size.");
        cout <<"StartTimeStep + parameter window is greater than vector size.\n";
	if(startTimeStep+timeWindow > iVector.size() || startTimeStep+timeWindow > jVector.size())
        //throw SpikeStreamAnalysisException("Analysis time window range is
        // greater than size of vectors with data");
        cout << "Analysis time window range is greater than "
             << "size of vectors with data" << endl;

	unsigned patternCount = 0;
	I_k_J_l_probs.clear();

	//Mask to clear bits after shift. Needs to clear end bits as well as the least significant bit in i
	unsigned clearBits = 1<<(k_param+l_param);
	--clearBits;//This will now clear any bits after the end of i
	clearBits ^= (1<<l_param);//XOR to clear LSB value in i

	//Load in first number from i
	unsigned tmpNum = 0;
	for(unsigned b = startTimeStep; b < startTimeStep+k_param; ++b){
		tmpNum <<= 1;
		tmpNum |= iVector.at(b);
	}

	//Add first number from j, need to offset this if l_param < k_param (cannot be >=)
	for(unsigned b = startTimeStep + k_param - l_param; b < startTimeStep+k_param; ++b){
		tmpNum <<= 1;
		tmpNum |= jVector.at(b);
	}
	I_k_J_l_probs[tmpNum] = 1.0;
	++patternCount;

	//Load in subsequent numbers. Both iVector and jVector are stepped through adding a 1 or 0 to the pattern and shaving off the end
	for(unsigned b = startTimeStep+k_param; b < startTimeStep+timeWindow; ++b){
		tmpNum <<= 1;
		tmpNum &= clearBits;

		//Add in i value
		tmpNum |= (iVector.at(b)<<l_param);

		//Add in j value
		tmpNum |= jVector.at(b);

		//Add to hash map
		if(!I_k_J_l_probs.contains(tmpNum))
			I_k_J_l_probs[tmpNum] = 1.0;
		else
			++I_k_J_l_probs[tmpNum];
		++patternCount;//Could work this out, but add for the moment
	}

	//Convert count of instances into probabilites
	for(QHash<unsigned, double>::iterator iter = I_k_J_l_probs.begin(); iter != I_k_J_l_probs.end(); ++iter)
		iter.value() /= patternCount;

	#ifdef DEBUG_PROBABILITIES
		printMap(I_k_J_l_probs, "I_k_J_l_probs");
	#endif//DEBUG_PROBABILITIES
}


/*! Loads probabilities p(i_n+1, i_n^k, j_n^l) into I_k_plus_1_J_1_probs hash map.
	Works through the entire sequence, totals the number of times that each pattern occurs and converts this into a probability.
	Made publicly accessible to facilitate unit testing. */
void TransferEntropyCalculator::load_I_k_plus_1_J_l_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector, QVector<unsigned>& jVector){
	if(startTimeStep + k_param + 1 >= iVector.size() || startTimeStep + l_param >= jVector.size())
        //throw SpikeStreamAnalysisException(
        //"StartTimeStep + parameter window is greater than vector size.");
        cout <<"StartTimeStep + parameter window is greater than vector size.\n";
	if(startTimeStep+timeWindow > iVector.size() || startTimeStep+timeWindow > jVector.size())
        //throw SpikeStreamAnalysisException("Analysis time window range
        // is greater than size of vectors with data");
        cout << "Analysis time window range  is greater than size of vectors "
             << "with data" << endl;
	unsigned patternCount = 0;
	I_k_plus_1_J_1_probs.clear();

	//Mask to clear bits after shift. Needs to clear end bits as well as the least significant bit in i
	unsigned clearBits = 1<<(k_param + 1 + l_param);
	--clearBits;//This will now clear any bits after the end of i
	clearBits ^= (1<<l_param);//XOR to clear LSB value in i

	//Load in first number from i
	unsigned tmpNum = 0;
	for(unsigned b = startTimeStep; b < startTimeStep + k_param + 1; ++b){
		tmpNum <<= 1;
		tmpNum |= iVector.at(b);
	}

	//Add first number from j. Offset it if k_param > l_param
	for(unsigned b = startTimeStep + k_param - l_param; b < startTimeStep+k_param; ++b){
		tmpNum <<= 1;
		tmpNum |= jVector.at(b);
	}
	I_k_plus_1_J_1_probs[tmpNum] = 1.0;
	++patternCount;

	//Load in subsequent numbers.
	for(unsigned b = startTimeStep + k_param + 1; b < startTimeStep+timeWindow; ++b){
		tmpNum <<= 1;
		tmpNum &= clearBits;

		//Add in next i value
		tmpNum |= (iVector.at(b)<<l_param);

		//Add in j value - this is one step back from the current i index
		tmpNum |= jVector.at(b-1);//the '-1' compensates for the fact that we are reading in the next i+1

		//Add to hash map
		if(!I_k_plus_1_J_1_probs.contains(tmpNum))
			I_k_plus_1_J_1_probs[tmpNum] = 1.0;
		else
			++I_k_plus_1_J_1_probs[tmpNum];
		++patternCount;//Could work this out, but add for the moment
	}

	//Convert count of instances into probabilites
	for(QHash<unsigned, double>::iterator iter = I_k_plus_1_J_1_probs.begin(); iter != I_k_plus_1_J_1_probs.end(); ++iter)
		iter.value() /= patternCount;

	#ifdef DEBUG_PROBABILITIES
		printMap(I_k_plus_1_J_1_probs, "I_k_plus_1_J_1_probs");
	#endif//DEBUG_PROBABILITIES
}


/*! Converts the number to a string showing its 1's and 0's */
QString TransferEntropyCalculator::getBitString(unsigned num){
	//Starting mask
	unsigned mask = 1<<(NUM_BITS-1);

	QString tmpStr = "";
	for(int i=0; i<NUM_BITS; ++i){
		//Space out the bits to make them more legible
		if(i && i%8 == 0)
			tmpStr += " ";

		if(mask&num)
			tmpStr += "1";
		else
			tmpStr += "0";

		mask >>= 1;
	}
	return tmpStr;
}



/*----------------------------------------------------------*/
/*------                PRIVATE METHODS               ------*/
/*----------------------------------------------------------*/

/*! Prints out a map for debugging purposes */
void TransferEntropyCalculator::printMap(QHash<unsigned, double> &map, QString name){
	cout<<"-------------------"<<name.toStdString()<<"-------------------------"<<endl;
	for(QHash<unsigned, double>::iterator iter = map.begin(); iter != map.end(); ++iter){
		cout<<"Key: "<<iter.key()<<"/"<<getBitString(iter.key()).toStdString()<<"; value: "<<iter.value()<<endl;
	}
	cout<<endl;
}

