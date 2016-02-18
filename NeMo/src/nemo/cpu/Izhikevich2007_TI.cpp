#include <cassert>
#include <nemo/fixedpoint.hpp>
#include <nemo/plugins/Izhikevich2007Syn.h>

#include "neuron_model.h"
#include <iostream>

const unsigned SUBSTEPS = 4;
const float SUBSTEP_MULT = 0.25f;


extern "C"
NEMO_PLUGIN_DLL_PUBLIC
void
cpu_update_neurons(
		unsigned start, unsigned end,
		unsigned cycle,
		float* paramBase, size_t paramStride,
		float* stateBase, size_t stateHistoryStride, size_t stateVarStride,
		unsigned fbits,
		unsigned fstim[],
		RNG rng[],
		float currentEPSP[],
		float currentIPSP[],
		float currentExternal[],
		uint64_t recentFiring[],
		unsigned fired[],
		void* /* rcm */)
{
        const float* a = paramBase + PARAM_2007A * paramStride;
        const float* b = paramBase + PARAM_2007B * paramStride;
        const float* c = paramBase + PARAM_2007C * paramStride;
        const float* d = paramBase + PARAM_2007D * paramStride;
        const float* vpeak = paramBase + PARAM_2007VPEAK * paramStride;
        const float* Vr = paramBase + PARAM_2007VR * paramStride;
        const float* Vt = paramBase + PARAM_2007VT * paramStride;
        const float* k = paramBase + PARAM_2007K * paramStride;
        const float* C = paramBase + PARAM_2007CCAP * paramStride;
        const float* sigma = paramBase + PARAM_2007SIGMA * paramStride;
        const float* tMdtOt_exc = paramBase + PARAM_2007TauMinusDtOverTau_Exc * paramStride;
        const float* G_exc = paramBase + PARAM_2007G_Exc * paramStride;
        const float* E_exc = paramBase + PARAM_2007E_Exc * paramStride;
        const float* tMdtOt_inh = paramBase + PARAM_2007TauMinusDtOverTau_Inh * paramStride;
        const float* G_inh = paramBase + PARAM_2007G_Inh * paramStride;
        const float* E_inh = paramBase + PARAM_2007E_Inh * paramStride;


	const size_t historyLength = 1;

        /* Current state */
        size_t b0 = cycle % historyLength;
        const float* u0 = stateBase + b0 * stateHistoryStride + STATE_2007U * stateVarStride;
        const float* v0 = stateBase + b0 * stateHistoryStride + STATE_2007V * stateVarStride;
        const float* ge0 = stateBase + b0 * stateHistoryStride + STATE_2007Ge * stateVarStride;
        const float* gi0 = stateBase + b0 * stateHistoryStride + STATE_2007Gi * stateVarStride;

        /* Next state */
        size_t b1 = (cycle+1) % historyLength;
        float* u1 = stateBase + b1 * stateHistoryStride + STATE_2007U * stateVarStride;
        float* v1 = stateBase + b1 * stateHistoryStride + STATE_2007V * stateVarStride;
        float* ge1 = stateBase + b1 * stateHistoryStride + STATE_2007Ge * stateVarStride;
        float* gi1 = stateBase + b1 * stateHistoryStride + STATE_2007Gi * stateVarStride;


	/* Each neuron has two indices: a local index (within the group containing
	 * neurons of the same type) and a global index. */

	int nn = end-start;
	assert(nn >= 0);

#pragma omp parallel for default(shared)
	for(int nl=0; nl < nn; nl++) {

                unsigned ng = start + nl;

                float ge = ge0[nl];
                float gi = gi0[nl];
                float u = u0[nl];
                float v = v0[nl];

                // -- UPDATE SYNAPSES ----------------------------------------------------

                // STEP 1: Decrease g accordning to tau.
                ge = ge * tMdtOt_exc[nl];
                gi = gi * tMdtOt_inh[nl];

                // STEP 2: Increment g according to inputs. (and later to short-term facilitation)!
                ge += currentEPSP[ng];
		gi += currentIPSP[ng];

                // STEP 3: Boundaries.
                /*if(ge < 0.00001)           ge = 0;  // ZAF: Whould this restriction be plausible??
                else */if(ge > G_exc[nl])    ge = G_exc[nl];

                /*if(gi > 0.00001)           gi = 0;
                else */if(gi < -G_inh[nl])   gi = -G_inh[nl];
float big = 10000000.0f;
float small = 0.00001f;
if(gi < -big /*|| (gi > -small && gi != 0.0f)*/ ) std::cout << "AAAAAA: " << gi << std::endl; //gi = 0.0f;
if(ge > big /*|| (ge < small && ge != 0.0f)*/ ) std::cout << "OOOOOOOO: " << ge << std::endl;//ge = 0.0f;
                ge1[nl] = ge;
                gi1[nl] = gi;

                // Update input
                float I = ge*(E_exc[nl] - v) - gi*(E_inh[nl] - v) + currentExternal[ng];
                //float I = ge*(0.0f - v) - gi*(-80.0f - v) + currentExternal[ng];

//std::cout << I <<  " ge: "<< ge<< " gi: " << gi <<  " tau_I: " <<  tMdtOt_inh[nl]  <<  " E_I: " <<  E_inh[nl] << " v: " <<  v << " " << currentExternal[ng] <<  std::endl;
                // ZAF: SOS!!!!! TO CHANGE THIS BACK! - Update: It seems to work like that.
                //                                                                                                                              Justify why!!
                // -----------------------------------------------------------------------

                /* no need to clear current?PSP. */

                //! \todo clear this outside kernel. Make sure to do this in all kernels.
                currentExternal[ng] = 0.0f;


		        if(sigma[nl] != 0.0f) {
			        I += sigma[nl] * nrand(&rng[nl]);
		        }

		        fired[ng] = 0;

		float oneOverC = 1.0f/C[nl];

		for(unsigned t=0; t<SUBSTEPS; ++t) {
			if(!fired[ng]) {
				v += SUBSTEP_MULT * oneOverC * (k[nl] * (v - Vr[nl]) * (v - Vt[nl])  - u + I);
				if(v <= -65.0){
					u += SUBSTEP_MULT * (a[nl] * (b[nl] * (v + Vr[nl]) - u));
				}
				else{
					u += SUBSTEP_MULT * (a[nl] * (- u));
				}
				fired[ng] = v >= (vpeak[nl]-0.08*u);
			}
		}

		fired[ng] |= fstim[ng];
		fstim[ng] = 0;
		recentFiring[ng] = (recentFiring[ng] << 1) | (uint64_t) fired[ng];

		if(fired[ng]) {
			v = c[nl]+0.08*u;
			if( (u + d[nl]) < 530){
				u += d[nl];
			}
			else{
				u = 530;
			}
			// LOG("c%lu: n%u fired\n", elapsedSimulation(), m_mapper.globalIdx(n));
		}

		u1[nl] = u;
		v1[nl] = v;
	}
}


cpu_update_neurons_t* test = &cpu_update_neurons;


#include "default_init.c"

