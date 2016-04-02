#ifndef MODELVALUES_H
#define MODELVALUES_H

// GUI values:
#define LINE_THICKNESS 7      // The thickness of the connection lines in schema
#define TEMP_FILE "75a93.xml" // The name of the file that is stored temporarly
                              // so as transfer the XML changes to the schema..
#define MAX_FILES 9           // The maximum allowed tabs
#define CUDA_ENABLED 0        // NeMo support of CUDA.

// Neurotransmitters
//enum{AMPA, GABAa, NMDA, GABAb, D1, D2, GABA_FSI, GABA_MSN, DA, GABA};

// Neuron models
//enum {SIMPLE, CORTICAL, THALAMIC, BG, SIMPLE_BG};

enum {COR_EXC, COR_INH, COR_RS, COR_IB, COR_CH, COR_LTS, COR_FS, COR_LS };
enum {BG_MSN, BG_STN };
enum {THAL_TC, THAL_RTN, THAL_INT };

#define NEUROTRANSM_TIME_WINDOW 100


typedef int neurotransmitter;
const int NEUROTRANSM_SIZE = 6;


namespace Values{


    // Parameters of a synaptic type -------------------------------------------
    // double tau;      // Decay rate.                        (0.01 to 1000 ms)
    // double g_max;    // Maximum synaptic conductance.
    // double E;        // Equilibrium (reversal) potential.  (-100 to 300 mV)
    // double p;        // Relative facilitation rate:        (0 to 10)
                            // Izhikevich 2008: Short-term facilitation
                            // A rate greater than 1 means that the synapse
                            // post-synaptic conductance tends to increase
                            // with high-frequency repetitive activation
                            // (facilitation), while a facilitation rate less
                            // than 1 means that the post-synaptic conductance
                            // tends to decrease with repetitive activation
                            // (decrement).
    // double w;        // Synaptic conductance (weight):     (0 to 100 uS/size)
                            // The amplitude of the post-synaptic conductance
                            // change which this synapse mediates. This value is
                            // the initial conductance (time zero) of the
                            // unfacilitated synapse. A value of 0 means that
                            // the synapse is switched off. Note: amplitude is
                            // defined per unit size (effectively per unit
                            // membrane area). This means that conductance
                            // scales proportionally with changes in the
                            // relative electrical size of the neuron, so that
                            // changes in neuron size make no difference to the
                            // amplitude of the synaptic response
                            // (for comparison see Electrical Synaptic Types).



    // Facilitation decay:
    // A time constant of the rate of decay of facilitation.
    // A long time constant means that the synapse shows facilitation or
    // decrement (according to the value of the Relative facilitation parameter)
    // at relatively low frequencies of activation, while a short time constant
    // means that the synapse only facilitates or decrements with high frequency
    // activation.  Acceptable range: (0.01 to 1000 ms)


    // TESTS:
    const double TEST_G_AMPA    = 50;
    const double TEST_G_NMDA    = 50;
    const double TEST_G_GABAa   = 50;
    const double TEST_G_GABAb   = 50;


    // Connectivity function ---------------------------------------------------
    // Type of Neuron           Value       Source              Comments
    // -------------------------------------------------------------------------
    const double Prob_a         = 2.01;     //      ---
    const double Prob_b         = 0.144;    //      ---
    const double Prob_c         = 0.01;     //      ---
    const double Prob_d         = 0.0;      //      ---         Not using
    const double Prob_L         = 1.0;      //      ---
    const double Prob_G         = 0.0;      //      ---
    const double Prob_minD      = 0.0;      //      ---
    const double Prob_maxD      = 1.0;      //      ---

    const double Delay_m        = 1.0;      //      ---
    const double Delay_n        = 0.0;      //      ---

    const double Weight_F       = 2.01;     //      ---
    const double Weight_a       = 0.144;    //      ---
    const double Weight_b       = 0.01;     //      ---
    const double Weight_c       = 1.0;      //      ---
    const double Weight_d       = 0.0;      //      ---         Not using
    const double Weight_L       = 1.0;      //      ---
    const double Weight_G       = 0.0;      //      ---
    const double Weight_minD    = 0.0;      //      ---
    const double Weight_maxD    = 1.0;      //      ---

    const double Prob_exp_a     = 1.0;      //      ---
    const double Prob_exp_b     = 0.0;      //      ---         Not using
    const double Prob_exp_c     = 0.0;      //      ---
    const double Prob_exp_d     = 1.0;      //      ---
    const double Prob_exp_minD  = 0.0;      //      ---
    const double Prob_exp_maxD  = 1.0;      //      ---
    const double Prob_exp_L     = 0.95;     //      ---
    const double Prob_exp_G     = 0.05;     //      ---

    const double Hum_a          = 0.5567;   // Humphries 2009
    const double Hum_b          = 0.1212;   // Humphries 2009
    const double Hum_c          = 0.008;    // Humphries 2009
    const double Hum_d          = 10.0;     //      ---         The scale of dis
    const double Hum_minD       = 0.0;      //      ---
    const double Hum_maxD       = 1.0;      //      ---
    const double Hum_L          = 1.0;      //      ---
    const double Hum_G          = 0.0;      //      ---

    //f[0].setA(5.0f);
    //f[1].setA(4.0f);
    //f[0].setC(0.0f);
    //f[1].setC(1.6f);
    //f[0].setD(5.0f);
    //f[1].setD(10.0f);


    // Neurotransmitters -------------------------------------------------------
    // Type of Neuron           Value           Source              Comments
    // -------------------------------------------------------------------------
    //const int MSN_NEUROTR       = GABA_MSN;
    //const int RS_NEUROTR        = NULL;




    // Wildie and Shanahan 2012 ------------------------------------------------
    // Parameter                Value       Source                  Comments
    // -------------------------------------------------------------------------
    const double WIL_E_GABAa    = -80;      // Wildie 2012          Inhibitory
    const double WIL_tau_GABAa  = 10;       // Wildie 2012          Inhibitory

    const double WIL_E_AMPA     = 0;        // Wildie 2012          Excitatory
    const double WIL_tau_AMPA   = 2;        // Wildie 2012          Excitatory



    // Steward etal 2012 -------------------------------------------------------
    // Parameter                Value       Source                  Comments
    // -------------------------------------------------------------------------
    const double STEW_tau_AMPA  = 2;        // Spruston etal 1995   ms
    const double STEW_tau_GABA  = 8;        // Gupta etal 2000      ms
                                            // +Markram(715 citat.)

    // Humphries etal 2006 (SPAWN) ---------------------------------------------
    // Parameter                Value       Source                  Comments
    // -------------------------------------------------------------------------
    const double HUMP_tau_AMPA  = 2;        // Wrong citation       ms
    const double HUMP_tau_GABAa = 3;        // Wrong citation       ms
    const double HUMP_tau_NMDA  = 100;      // Wrong citation       ms


    // Eliasmith etal 2012 (SPAWN) ---------------------------------------------
    // Parameter                Value       Source                  Comments
    // -------------------------------------------------------------------------
    const double ELIA_tau_AMPA  = 10;       // Wrong citation       ms
    const double ELIA_tau_GABA  = 8;        // Wrong citation       ms
    const double ELIA_tau_NMDA  = 50;       // Wrong citation       ms
    const double ELIA_tau_DA    = 10;       // Wrong citation       ms
    // These are set by considering empirical data, not parameter fitting
    // AMPA glutamate receptors were used for most projections in the model,
    // except recurrent projections, which were taken to employ NMDA receptors.
    // Projections in the basal Ganglia: Inhibitory GABA / Dopamine

    // Humphries etal 2009 -----------------------------------------------------
    // Parameter                Value       Source                  Comments
    // -------------------------------------------------------------------------
  //const double MSN_a          = 0.05;     // This paper
  //const double MSN_b          = -20;      // Izhikevich 2007
  //const double MSN_c          = -55;      // Izhikevich 2007      mV
  //const double MSN_d          = 377;      // This paper
  //const double MSN_vpeak      = 40;       // Izhikevich 2007      mV
  //const double MSN_vr         = -80;      // Izhikevich 2007      mV
  //const double MSN_vt         = -33.8;    // This paper           mV
  //const double MSN_k          = 1.14;     // This paper
  //const double MSN_C          = 50;       // Izhikevich 2007      pF

    const double MSN_alpha      = 0.03;     // This paper
    const double MSN_G_DA       = 22.7;     // This paper           nS
    const double MSN_E_DA       = -68.4;    // This paper           mV
    const double MSN_bita1      = 3.75;     // This paper
    const double MSN_bita2      = 0.156;    // This paper

    const double MSN_G_ampa     = 6.1;      // This paper           nS
    const double MSN_G_nmda     = 3.05;     // This paper           nS
    const double MSN_G_gaba_ms  = 4.36;     // This paper           nS
    const double MSN_G_gaba_fs  = 21.8;     // Koos 2004            nS (~5xMSN coductance)

    const double MSN_E_ampa     = 0;        // Moyer 2007           mV
    const double MSN_E_nmda     = 0;        // Moyer 2007           mV
    const double MSN_E_gaba_fs  = -60;      // Moyer 2007           mV
    const double MSN_E_gaba_ms  = 60;       // Moyer 2007           mV
    const double MSN_tau_ampa   = 6;        // Moyer 2007           ms
    const double MSN_tau_nmda   = 160;      // Moyer 2007           ms
    const double MSN_tau_gaba_fs= 4;        // Moyer 2007           ms
    const double MSN_tau_gaba_ms= 4;        // Moyer 2007           ms

    const double MSN_Mg2plus_0  = 1;        // Jahr & Stevens 1990  mM


    const double FSI_a          = 0.2;      // Izhikevich 2007
    const double FSI_b          = 0.025;    // Izhikevich 2007
    const double FSI_c          = -60;      // Tateno et al. 2004   mV
    const double FSI_d          = 0;        // Izhikevich 2007
    const double FSI_k          = 1;        // Izhikevich 2007
    const double FSI_C          = 80;       // Tateno et al. 2004   pF
    const double FSI_vr         = -70;      // Tateno et al. 2004   mV
    const double FSI_vt         = -50;      // Tateno et al. 2004   mV
    const double FSI_vpeak      = 25;       // Izhikevich 2007      mV
    const double FSI_vb         = -55;      // Izhikevich 2007      mV
    const double FSI_ita        = 0.1;      // fitted to Bracci & Panzeri 2006
    const double FSI_epsilon    = 0.625;    // fitted to Gorelova etal 2002
    const double FSI_E_ampa     = 0;        // n.d.;set as for MSNs mV
    const double FSI_E_nmda     = 0;        //   - '' -             mV
    const double FSI_E_gaba_fs  = -60;      //   - '' -             mV
    const double FSI_E_gaba_ms  = -60;      //   - '' -             mV
    const double FSI_tau_ampa   = 6;        //   - '' -             ms
    const double FSI_tau_gaba_fs= 4;        //   - '' -             ms
    const double FSI_g_ampa     = 61;       // nS n.d.; tuned to achieve realistic firing rates (Section 5.2)
    const double FSI_g_gaba_fs  = 20;       // nS n.d.; assumes equivalent effect of FSI-FSI contacts as FSI-MSN contacts
    const double FSI_g          = 30;       // nS     (Section 3.1.3)
    const double FSI_tau        = 11;       // ms


    // Humphries etal 2009B ----------------------------------------------------
    // Parameter                Value       Source                  Units
    // -------------------------------------------------------------------------
    const double MSN_a          = 0.01;     // Mahon etal 2000b, Izhikevich 2007
    const double MSN_b          = -20;      // Izhikevich 2007
    const double MSN_c          = -55;      // Izhikevich 2007      mV
    const double MSN_d          = 91;       // This paper
    const double MSN_vpeak      = 40;       // Izhikevich 2007      mV
    const double MSN_vr         = -80;      // Izhikevich 2007      mV
    const double MSN_vt         = -29.7;    // This paper           mV
    const double MSN_k          = 1;        // Izhikevich 2007
    const double MSN_C          = 15.2;     // This paper           pF

    const double MSN_u          = 0.0;      // ??
    const double MSN_v          = 0.0;      // ??

  //const double MSN_alpha      = 0.332;    // This paper
  //const double MSN_bita1      = 6.3;      // This paper
  //const double MSN_bita2      = 0.215;    // This paper
  //const double MSN_K          = 0.0289;   // This paper
  //const double MSN_L          = 0.331;    // This paper
  //const double MSN_G_ampa_nmda= 6.9;      // This paper           nS


  //const double MSN_E_ampa     = 0;        // Moyer etal 2007      mV
  //const double MSN_E_nmda     = 0;        // Moyer etal 2007      mV
  //const double MSN_E_gaba     = -60;      // Moyer etal 2007      mV
  //const double MSN_tau_ampa   = 6;        // Moyer etal 2007      ms
  //const double MSN_tau_nmda   = 160;      // Moyer etal 2007      ms
  //const double MSN_tau_gaba   = 4;        // Moyer etal 2007      ms
  //const double MSN_G_ampa_nmda= 2.0;      // Moyer etal 2007      nS
  //const double MSN_G_ampa_gaba= 1.4;      // Moyer etal 2007      nS

  //const double MSN_Mg2plus_0  = 1;        // Jahr & Stevens 1990b mM

    // Izhikevich 2007 ---------------------------------------------------------
    // Parameter                Value       Unit        Comments
    // -------------------------------------------------------------------------

    const double RS_a           = 0.03;
    const double RS_b           = -2.0;
    const double RS_c           = -50.0;
    const double RS_d           = 100.0;
    const double RS_vpeak       = 35.0;     // mV
    const double RS_vr          = -60.0;    // mV
    const double RS_vt          = -40.0;    // mV
    const double RS_k           = 0.7;
    const double RS_C           = 100.0;    // pF
    const double RS_v           = -70.0;    // ?????????? IT'S MINE SO TO CHECK!
    const double RS_u           = 140.0;    // b*v

    const double IB_a           = 0.01;
    const double IB_b           = 5.0;
    const double IB_c           = -56.0;
    const double IB_d           = 130.0;
    const double IB_vpeak       = 50.0;
    const double IB_vr          = -75.0;
    const double IB_vt          = -45.0;
    const double IB_k           = 1.2;
    const double IB_C           = 150.0;
    const double IB_v           = 0.0;      // ????
    const double IB_u           = 0.0;      // ????

    const double CH_a           = 0.01;
    const double CH_b           = 5.0;
    const double CH_c           = -40.0;
    const double CH_d           = 150.0;
    const double CH_vpeak       = 25.0;
    const double CH_vr          = -60.0;
    const double CH_vt          = -40.0;
    const double CH_k           = 1.5;
    const double CH_C           = 50.0;
    const double CH_v           = 0.0;      // ????
    const double CH_u           = 0.0;      // ????

    const double FS_a           = 0.2;
    const double FS_b           = 0.025;
    const double FS_c           = -45.0;
    const double FS_d           = 0.0;
    const double FS_vpeak       = 25.0;
    const double FS_vr          = -55.0;
    const double FS_vt          = -40.0;
    const double FS_k           = 1.0;
    const double FS_C           = 20.0;
    const double FS_vb          = -55.0;
    const double FS_v           = -80.0;                           // ZAF: Mine!
    const double FS_u           = 1600;     // b*v ???

    const double LTS_a          = 0.03;
    const double LTS_b          = 8.0;
    const double LTS_c          = 0.0;
    const double LTS_d          = 0.0;
    const double LTS_vpeak      = 0.0;
    const double LTS_vr         = -56.0;
    const double LTS_vt         = -42.0;
    const double LTS_k          = 1.0;
    const double LTS_C          = 100.0;
    const double LTS_v          = 0.0;      // ????
    const double LTS_u          = 0.0;      // ????

    const double LS_a           = 0.17;
    const double LS_b           = 5.0;
    const double LS_c           = -45.0;
    const double LS_d           = 100.0;
    const double LS_vpeak       = 30.0;
    const double LS_vr          = -66.0;
    const double LS_vt          = -40.0;
    const double LS_k           = 0.3;
    const double LS_C           = 20.0;
    const double LS_v           = 0.0;      // ????
    const double LS_u           = 0.0;      // ????

  //const double MSN_a          = 0.01;
  //const double MSN_b          = -20.0;
  //const double MSN_c          = -55.0;
  //const double MSN_d          = 150.0;
  //const double MSN_vpeak      = 40.0;
  //const double MSN_vr         = -80.0;
  //const double MSN_vt         = -25.0;
  //const double MSN_k          = 1.0;
  //const double MSN_C          = 50.0;
  //const double MSN_v          = -80.0;
  //const double MSN_u          = 1600;     // b*v



    // Izhikevich 2008 - Large scale model of human brain ----------------------
    // Parameter                Value       Unit        Comments
    // -------------------------------------------------------------------------
    const double IZ_tau_AMPA    = 5;    //  ms        all values from
    const double IZ_tau_NMDA    = 150;  //  ms   Dayan P and Abbott L F (2001)
    const double IZ_tau_GABAa   = 6;    //  ms             and
    const double IZ_tau_GABAb   = 150;  //  ms      Izhikevich etal (2004)
    // SOS: In this model, (maybe a typo):
    //              I_z = g_z * (v - E_z);     where g_z' = g_z / tau_z



    // Papamichalis 2010 - MSc Thesis ------------------------------------------
    // Parameter                        Value       Comments
    // -------------------------------------------------------------------------
    const double STN_simple_a      = 0.005;    //
    const double STN_simple_b      = 0.265;    //
    const double STN_simple_c      = -65;      //
    const double STN_simple_d      = 1.5;      //
    const double STN_simple_v      = -70;      //
    const double STN_simple_u      = -18.55;   // b*V;
    const double STN_simple_vpeak  = 30;       //

  //const double STN_simple_aVar   = 0.005;    //
  //const double STN_simple_bVar   = 0.265;    //
  //const double STN_simple_cVar   = -65;      //
  //const double STN_simple_dVar   = 1.5;      //

    // Transformation for C = 100
  //const double STN_a             = 0.005;    //
  //const double STN_b             = 26.5;     //
  //const double STN_c             = -65;      //
  //const double STN_d             = 150;      //
  //const double STN_v             = -70;      //
  //const double STN_u             = -1855;    // b*V;
  //const double STN_C             = 100;      //
  //const double STN_k             = 4;        //
  //const double STN_vr            = -60.9652; // -60.9652 or -57.4098
  //const double STN_vt            = -64.0348; // -64.0348 or -67.5902
  //const double STN_vpeak         = 30;       //

    // Transformation for C = 50
    const double STN_a             = 0.005;    //
    const double STN_b             = 13.25;    //
    const double STN_c             = -65.0;    //
    const double STN_d             = 75.0;     //
    const double STN_v             = 70.0;     //
    const double STN_u             = -927.5;   // b*V;
    const double STN_C             = 50.0;     //
    const double STN_k             = 2.0;      //
    const double STN_vr            = -57.4098; // -57.4098 or -60.9652
    const double STN_vt            = -67.5902; // -67.5902 or -64.0348
    const double STN_vpeak         = 30.0;     //

    // Transformation rules:
    // k = 0.04*C;  a_new = a;  b_new = C*b;  c_new = c;  d_new = C*d;
    // vpeak_new = vpeak; vr = ( b-5 +- sqrt((5-b)^2-22.4) )/0.08; vt = -vr-125;

    // SOS: C_m is membrane capacitance and takes 1 �F/cm^2
    //      2004 -Otsuka etal - Conductance-Based Model of the Voltage-Dependent
    //      Generation of a Plateau Potential in Subthalamic Neurons
    // 1 micro = 1.000.000 pico   /   1 cm = 10 mm   /   => C_m = 10.000 pF/mm^2


    // ZAF ---------------------------------------------------------------------
    // Parameter                Value       Comments
    // -------------------------------------------------------------------------
    // General pyramidal neuron
    const double GEN_EXC_a      = 0.02;     // Transf. from Izhikevich 2003
    const double GEN_EXC_b      = 20.0;     // Transf. from Izhikevich 2003
    const double GEN_EXC_c      = -65.0;    // Transf. from Izhikevich 2003
    const double GEN_EXC_d      = 800.0;    // Transf. from Izhikevich 2003
    const double GEN_EXC_vpeak  = 30.0;     // Transf. from Izhikevich 2003
    const double GEN_EXC_vr     = -50.0;    // Transf. from Izhikevich 2003
    const double GEN_EXC_vt     = -70.0;    // Transf. from Izhikevich 2003
    const double GEN_EXC_k      = 4.0;      // Transf. from Izhikevich 2003
    const double GEN_EXC_C      = 100.0;    // Transf. from Izhikevich 2003
    const double GEN_EXC_v      = -70;      // Mine
    const double GEN_EXC_u      = -50;      // Mine !!!! ??? !!!

    const double GEN_EXC_aVar   = 0.0;      // Transf. from Izhikevich 2003
    const double GEN_EXC_bVar   = 0.0;      // Transf. from Izhikevich 2003
    const double GEN_EXC_cVar   = 15.0;     // Transf. from Izhikevich 2003
    const double GEN_EXC_dVar   = -600.0;   // Transf. from Izhikevich 2003
    const double GEN_EXC_vrVar  = 0.0;      // Transf. from Izhikevich 2003
    const double GEN_EXC_vtVar  = 0.0;      // Transf. from Izhikevich 2003
    const double GEN_EXC_kVar   = 0.0;      // Transf. from Izhikevich 2003
    const double GEN_EXC_CVar   = 0.0;      // Transf. from Izhikevich 2003

    // General Interneuron
    const double GEN_INH_a      = 0.02;
    const double GEN_INH_b      = 0.25;
    const double GEN_INH_c      = -50.0;
    const double GEN_INH_d      = 100.0;
    const double GEN_INH_vpeak  = 35.0;
    const double GEN_INH_vr     = -60.0;
    const double GEN_INH_vt     = -40.0;
    const double GEN_INH_k      = 1.0;
    const double GEN_INH_C      = 100.0;
    const double GEN_INH_v      = 0;        // ???????????????
    const double GEN_INH_u      = 0;        // ???????????????

    const double GEN_INH_aVar   = 0.08;     // or 0.03 + ..R
    const double GEN_INH_bVar   = -0.05;    // or -2 + ..R
    const double GEN_INH_cVar   = 0.0;
    const double GEN_INH_dVar   = 0.0;
    const double GEN_INH_vrVar  = 0.0;
    const double GEN_INH_vtVar  = 0.0;
    const double GEN_INH_kVar   = 0.0;
    const double GEN_INH_CVar   = 0.0;

    const double SIMP_MSN_a     = 0.05;     // Transf. from Humphries 2009
    const double SIMP_MSN_b     = -0.4;     // Transf. from Izhikevich 2003
    const double SIMP_MSN_c     = -55.0;    // Transf. from Izhikevich 2003
    const double SIMP_MSN_d     = 7.54;     // Transf. from Humphries 2009
    const double SIMP_MSN_vpeak = 40.0;     // Transf. from Izhikevich 2003

    // Izhikevich 2003 - A simple model ----------------------------------------
    // Parameter                   Value       Comments
    // -------------------------------------------------------------------------
    const double SIMPLE_EXC_a      = 0.02;     //
    const double SIMPLE_EXC_b      = 0.2;      //
    const double SIMPLE_EXC_c      = -65.0;    //
    const double SIMPLE_EXC_d      = 8.0;      //
    const double SIMPLE_EXC_vpeak  = 30.0;     //
    const double SIMPLE_EXC_v      = -65;      // c
    const double SIMPLE_EXC_u      = -13;      // b*c

    const double SIMPLE_EXC_aVar   = 0.0;      //
    const double SIMPLE_EXC_bVar   = 0.0;      //
    const double SIMPLE_EXC_cVar   = 15.0;     //
    const double SIMPLE_EXC_dVar   = -6.0;     //

    const double SIMPLE_INH_a      = 0.02;     //
    const double SIMPLE_INH_b      = 0.25;     //
    const double SIMPLE_INH_c      = -65.0;    //
    const double SIMPLE_INH_d      = 2.0;      //
    const double SIMPLE_INH_vpeak  = 30.0;     //
    const double SIMPLE_INH_v      = -65;      // c
    const double SIMPLE_INH_u      = -16.25;   // b*c

    const double SIMPLE_INH_aVar   = 0.08;     //
    const double SIMPLE_INH_bVar   = -0.05;    //
    const double SIMPLE_INH_cVar   = 0.0;      //
    const double SIMPLE_INH_dVar   = 0.0;      //


    const double SIMPLE_sandard_a   = 0.02;     //
    const double SIMPLE_standard_b  = 0.2;      //
    const double SIMPLE_standard_c  = -65;      //
    const double SIMPLE_standard_d  = 2;        //

    const double SIMPLE_RS_a        = 0.02;     //
    const double SIMPLE_RS_b        = 0.2;      //
    const double SIMPLE_RS_c        = -65;      //
    const double SIMPLE_RS_d        = 8;        //
    const double SIMPLE_RS_u        = 0;        //
    const double SIMPLE_RS_v        = 0;        //

    const double SIMPLE_IB_a        = 0.02;     //
    const double SIMPLE_IB_b        = 0.2;      //
    const double SIMPLE_IB_c        = -55;      //
    const double SIMPLE_IB_d        = 4;        //
    const double SIMPLE_IB_u        = 0;        //
    const double SIMPLE_IB_v        = 0;        //

    const double SIMPLE_CH_a        = 0.02;     //
    const double SIMPLE_CH_b        = 0.2;      //
    const double SIMPLE_CH_c        = -50;      //
    const double SIMPLE_CH_d        = 2;        //
    const double SIMPLE_CH_u        = 0;        //
    const double SIMPLE_CH_v        = 0;        //

    const double SIMPLE_FS_a        = 0.1;      //
    const double SIMPLE_FS_b        = 0.2;      //
    const double SIMPLE_FS_c        = -65;      //
    const double SIMPLE_FS_d        = 2;        //
    const double SIMPLE_FS_u        = 0;        //
    const double SIMPLE_FS_v        = 0;        //

    const double SIMPLE_LTS_b       = 0.25;     // ????? To achieve a better
                                                    // quantitative fit with
                                                    // real LTS neurons, other
                                                    // parameters of the model
                                                    // need to be changed aswell




    // ---------------------------------------------------------------------- //
    //                          Connectivity                                  //
    // ---------------------------------------------------------------------- //

    // the factors w_ij / tau_s are calculated to give currents that yield PSPs
    // of measured size. These are then *relative* weighting factors multiplied
    // by w_ij / tau_s


    // Humphries etal 2006 -----------------------------------------------------
    // Parameter                Value       Comments
    // -------------------------------------------------------------------------
    const double SD1_w          = -4.0; //
    const double SD2_w          = -4.0; //
    const double STN_GPi_w      = 1.0;  //

    // STN-GP loop
    const double STN_GPe_w      = 1.0;  //  1.5
    const double GPe_STN_w      = -1.0; //  (1) set to -2
    // -----------                      //  to get consistent bursting

    const double GPe_GPi_w      = -1.0; //

    const double GPe_GPe_w      = -1;   //  These (recurrent) connections
    const double GPi_GPi_w      = -1;   //  represent the local collaterals!
                                        //  From yahoo:
                                        //  An axon collateral is a branch off
                                        //  of the main axon that usually feeds
                                        //  back onto the soma. The function is
                                        //  to provide modulation of cell firing.
                                        //  Source:PhD Biochemistry/Neuroscience

    const double EXT_w          = 1.0;  //  ctx input 1.0
    const double STN_ext_ratio  = 1.0;  //  cortico-subthalamic weight is (1)
                                        //  EXT_STN_w =EXT_w*STN_ext_ratio=1*1=1

    // AXONAL DELAYS
    const double SD1_GPi_d      = 4;    //  ms
    const double SD2_GPe_d      = 5;	//  ms
    const double STN_GPe_d      = 2;	//  ms
    const double STN_GPi_d      = 1.5;  //  ms
    const double GPe_STN_d      = 4;    //  ms
    const double GPe_GPi_d      = 3;	//  ms
    const double GPe_GPe_d      = 1;	//  ms    !!!!!!!!!!!
    const double GPi_GPi_d      = 1;	//  ms    !!!!!!!!!!!
    const double EXT_SD1_d      = 10;   //  ms
    const double EXT_SD2_d      = 10;   //  ms
    const double EXT_STN_d      = 2.5;  //  ms

#ifdef DO_URETHANE  // do urethane manipulation
    const double glut_scale = 0.65; // turn all glut weights down (0.65? - KG)
    const double gaba_scale = 1.5;  // turn all gaba weights up

    const double SD1_w = SD1_w .* gaba_scale;
    const double SD2_w = SD2_w .* gaba_scale;
    const double GPe_STN_w = GPe_STNw .* gaba_scale;
    const double GPe_GPi_w = GPe_GPiw .* gaba_scale;
    const double GPe_GPe_w = GPe_GPew .* gaba_scale;
    const double GPi_GPi_w = GPi_GPiw .* gaba_scale;

    const double STN_GPi_w = STN_GPiw .* glut_scale;
    const double STN_GPe_w = STN_GPew .* glut_scale;
    const double EXT_w = EXT_w .* glut_scale;
#else
    const double glut_scale = 1.0;
    const double gaba_scale = 1.0;
#endif


}

#endif // MODELVALUES_H






