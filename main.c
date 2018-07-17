
#include <stdio.h>

#include "typedefinitions.h"

// Prerequisites
settings GetSettingsControlFile(char* inputfile, settings defaults);
int OutputSettings(FILE* fd, settings prefs);


int main(int argc, char **argv){

    settings defaults, prefs;

// fill defaults struct with default values
    defaults = (struct settings) {
    // integer values
        .dimension = 2,     // dimension of the problem
        .n_stencil = 9,     // 1D stencil size
        .n_spline  = 0,     // number of interpolation points
    // Flags
        .analyze = 0,
        .dipole  = 0,
        .check_spacing = 1,

        .n_out  = 5,        // Number of output eigenstates (ARPACK)

    // double precision values
        .masses_string = "",
        .masses        = NULL,    // g/mol

        .ekin_factor = 1.0/4.184,       // (kcal/mol) / (kJ/mol)
        .epot_factor = 1.0,             // (output unit) / (input unit)
        .mu_factor   = 1.0E20,
        .DipToAsm    = 3.33564E-30,     // A.s.m / Debye
        .threshold   = 1.0E-10,         // abs(q[i] - q[i+1])



    // Eigensolver specific values
        .Eigensolver = 1,   // 1 = MKL FEAST; 2 = ARMADILLO ARPACK
        .e_min  = 0.0,      // minimal energy in output energy unit (FEAST)
        .e_max  = 400.0,    // maximal energy in output energy unit (FEAST)

    // file names
        .input_file    = "",
        .coriolis_file = "",
        .output_file   = "/dev/stdout",
    };


    char * filename = NULL;
    if(argc > 1){
        filename = argv[1];
    }

    prefs = GetSettingsControlFile(filename, defaults);

    OutputSettings(stdout, prefs);

    return 0;
}
