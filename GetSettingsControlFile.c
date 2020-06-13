
#include <stdlib.h>
#include <string.h>

#include "typedefinitions.h"
#include "controlfile.h"

// Provided prototypes
settings GetSettingsControlFile(char* inputfile, settings defaults);


settings GetSettingsControlFile(char* inputfile, settings defaults){

    int i;
    char * optarg;
// create new settings struct "preferences"
//  and initialize it with the default values
    settings preferences = defaults;

// create keyword list:
    keyword keylist[] = {
    // Boolian values:
        {"Analyze",                 0,  'a',  NULL },
        {"Dipole",                  0,  'd',  NULL },
        {"Check_Spacing",           0,  'T',  NULL },
    // integer values:
        {"Dimensionality",          0,  'D',  NULL },
        {"Stencil_Size",            0,  'n',  NULL },
        {"Interpolation_points",    0,  's',  NULL },
        {"N_Eigenstates",           0,  'N',  NULL },
    // double values:
        {"Kin_E_Factor",            0,  'k',  NULL },
        {"Pot_E_Factor",            0,  'v',  NULL },
        {"Dipole_Factor",           0,  'f',  NULL },
        {"IMOI_Factor",             0,  'M',  NULL },
        {"Spacing_Threshold",       0,  't',  NULL },
        {"Lower_Bound",             0,  'l',  NULL },
        {"Upper_Bound",             0,  'u',  NULL },
    // string values:
        {"Reduced_Masses",          0,  'm',  NULL },
        {"Input_File",              0,  'i',  NULL },
        {"Output_File",             0,  'o',  NULL },
        {"Coriolis_File",           0,  'c',  NULL },
    // other:
        {"Eigensolver",             0,  'E',  NULL },
    // requires zero termination
        { NULL , 0 , 0 , NULL }
    };

// get keyword values
    keyword * kl = ControlFileParser(inputfile, keylist);

// assign values to variables
    i = 0;
    while(kl[i].keyword != NULL){

    // only iterate over the values set by the parsing function
        if( kl[i].value == NULL){
            ++i;
            continue;
        }
        optarg = kl[i].value[0];

        switch(kl[i].identifier){

        // Boolian values
            case 'a':
                preferences.analyze     = atoi(optarg);
                if(preferences.analyze == 0){
                    if(strncasecmp("true", optarg, 4) == 0) { preferences.analyze = 1; }
                    else                                    { preferences.analyze = 0; }
                }
                break;

            case 'd':
                preferences.dipole      = atoi(optarg);
                if(preferences.dipole == 0){
                    if(strncasecmp("true", optarg, 4) == 0) { preferences.dipole = 1; }
                    else                                    { preferences.dipole = 0; }
                }
                break;

            case 'T':
                preferences.check_spacing = atoi(optarg);
                if(preferences.check_spacing == 0){
                    if(strncasecmp("true", optarg, 4) == 0) { preferences.check_spacing = 1; }
                    else                                    { preferences.check_spacing = 0; }
                }
                break;


        // integer values
            case 'D':
                preferences.dimension   = atoi(optarg);
                break;

            case 'n':
                preferences.n_stencil   = atoi(optarg);
                break;

            case 's':
                preferences.n_spline    = atoi(optarg);
                break;

            case 'N':
                preferences.n_out       = atoi(optarg);
                break;


        // double values
            case 'k':
                preferences.ekin_factor = atof(optarg);
                break;

            case 'v':
                preferences.epot_factor = atof(optarg);
                break;

            case 'f':
                preferences.DipToAsm    = atof(optarg);
                break;

            case 'M':
                preferences.mu_factor   = atof(optarg);
                break;

            case 't':
                preferences.threshold   = atof(optarg);
                break;

            case 'l':
                preferences.e_min       = atof(optarg);
                break;

            case 'u':
                preferences.e_max       = atof(optarg);
                break;


        // string values
            case 'm':
            // copy optarg to string and ensure zero termination
                strncpy(preferences.masses_string, optarg, _MaxSettingsStringLength_);
                preferences.masses_string[_MaxSettingsStringLength_ - 1] = '\0';
                break;

            case 'i':
            // copy optarg to string and ensure zero termination
                strncpy(preferences.input_file, optarg, _MaxSettingsStringLength_);
                preferences.input_file[_MaxSettingsStringLength_ - 1] = '\0';
                break;

            case 'c':
            // copy optarg to string and ensure zero termination
                strncpy(preferences.coriolis_file, optarg, _MaxSettingsStringLength_);
                preferences.coriolis_file[_MaxSettingsStringLength_ - 1] = '\0';
                break;

            case 'o':
            // copy optarg to string and ensure zero termination
                strncpy(preferences.output_file, optarg, _MaxSettingsStringLength_);
                preferences.output_file[_MaxSettingsStringLength_ - 1] = '\0';
                break;


        // other
            case 'E':
                preferences.Eigensolver = atoi(optarg);
                if(preferences.Eigensolver == 0){

                    if(strncasecmp(optarg, "Intel_MKL_FEAST", strlen("Intel_MKL_FEAST")) == 0){
                        preferences.Eigensolver = 1;
                    }
                    else if(strncasecmp(optarg, "ARMADILLO_ARPACK", strlen("ARMADILLO_ARPACK")) == 0){
                        preferences.Eigensolver = 2;
                    }

                }
                break;
        }
        ++i;
    }

    free_keywordlistvalues(kl);

// return new settings struct preferences
    return preferences;
}
