/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2004,  The GLoBES Team
 *
 * GLoBES is mainly intended for academic purposes. Proper
 * credit must be given if you use GLoBES or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <globes/globes.h>   /* GLoBES library */
#include "snu.h"

/* Output file */
// char MYFILE[]="inv_chi_out3_cp0.dat";
// char MYFILE1[]="inv_probabitity_out3_cp0.dat";
char MYFILE2[]="ND_events.dat";
char MYFILE3[]="ND_chi.dat";

// FILE *outfile = NULL;
// FILE *outfile1 = NULL;
FILE *outfile2 = NULL;
FILE *outfile3 = NULL;


int main(int argc, char *argv[])
{ 
  glbInit(argv[0]); // Initialize GLoBES library (libglobes)
  glbInitExperiment("DUNE_ND.glb",&glb_experiment_list[0],&glb_num_of_exps); // Initialize DUNE Near Detector experiment 
  //glbInitExperiment("DUNE_GLoBES.glb",&glb_experiment_list[0],&glb_num_of_exps); // Initialize DUNE experiment 
  int n_flavors = 4; // Number of neutrino flavors (3 active + 1 sterile)
  int rotation_order[][2]= {{3,4}, {2,4}, {1,4}, {2,3}, {1,3}, {1,2}}; // Order of rotations in the rotation matrix
  int phase_order[] = {-1, 2, 1, -1, 0, -1}; // Order of phases in the rotation matrix 
  snu_init_probability_engine(n_flavors, rotation_order, phase_order); // Initialize the probability engine from snu library
  glbRegisterProbabilityEngine(92, //6*sqrt(n_flavors)
  &snu_probability_matrix, 
  &snu_set_oscillation_parameters, 
  &snu_get_oscillation_parameters,
  NULL);

  /* Intitialize outputs */
  /*
  outfile = fopen(MYFILE, "w");
  if (outfile == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }
  
  outfile1 = fopen(MYFILE1, "w");
  if (outfile1 == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }
  */
  
  outfile2 = fopen(MYFILE2, "w");
  if (outfile2 == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }

   outfile3 = fopen(MYFILE3, "w");
  if (outfile3 == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }
  


  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_projection delta_projection = glbAllocProjection();


  /* Define the "true" values for the oscillation parameters (values taken from NuFit5.1 for normal mass ordering) */
 
  double param_values_test[12] = { // Inverted ordering values
    asin(sqrt(0.308)),    // asin(sqrt(0.308)),    // θ₁₂
    asin(sqrt(0.02224)),  // asin(sqrt(0.02224)),  // θ₁₃
    asin(sqrt(0.562)),    // asin(sqrt(0.562)),    // θ₂₃
    285.0,     // 285.0 * M_PI / 180.0, // δCP
    7.49e-5,              // 7.49e-5,              // Δm²₂₁
    -2.510e-3,            // -2.510e-3,            // Δm²₃₁
    0.0,                  // 0.0,                  // Δm²₄₁
    0.0,                  // 0.0,                  // θ₁₄
    0.0,                  // 0.0,                  // θ₂₄
    0.0,                  // 0.0,                  // θ₃₄
    0.0,                  // 0.0,                  // δCP1
    0.0                   // 1.0                   // δCP2
  };

  double param_values[12] = { // Normal ordering values
    asin(sqrt(0.307)),    // asin(sqrt(0.307)),    // θ₁₂ test
    asin(sqrt(0.02195)),  // asin(sqrt(0.02195)),  // θ₁₂ test
    asin(sqrt(0.561)),    // asin(sqrt(0.561)),    // θ₂₃ test
    177/180*M_PI,   // asin(sqrt(0.561)),    // δCP test
    7.49e-5,              // 7.49e-5,              // Δm²₂₁ test
    2.534e-3,             // 2.534e-3,             // Δm²₃₁ test
    0.0 ,                  // 0.0,                  // Δm²₄₁ test
    0.0,                  // 0.0,                  // θ₁₄ test
    0.0,                  // 0.0,                  // θ₂₄ test
    0.0,                  // 0.0,                  // θ₃₄ test
    0.0,                  // 0.0,                  // δCP1 test
    0.0                   // 1.0                   // δCP2 test
  };
  // Note the test values are those of the inverted heirachy (the ones commented out). The true were normal ordering values.
  
  /* TRUE */
  for (int i = 0; i < 12; ++i) {
    glbSetOscParams(true_values, param_values[i], i);
    printf("%g \n", glbGetOscParams(true_values, i));
  }
  for (int i = 12; i < 92; ++i) {
    glbSetOscParams(true_values, 0.0, i);
  }
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  /* TEST */
  for (int i = 0; i < 12; ++i) {
    glbSetOscParams(test_values, param_values[i], i);
    printf("%g \n", glbGetOscParams(test_values, i));
  }
  for (int i = 12; i < 92; ++i) {
    glbSetOscParams(test_values, 0.0, i);
  }
  glbSetDensityParams(test_values,1.0,0);

  /* Define an external error to your parameters: 3% error on the solar parameters.   *
   * In this case, θ₁₂ and Δm²₂₁ should be free parameters and minimized in the χ²     */
  double param_values_error[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  for (int i = 0; i < 12; ++i) {
    glbSetOscParams(input_errors, param_values_error[i], i);
    printf("%g \n", glbGetOscParams(input_errors, i));
  }
  for (int i = 12; i < 92; ++i) {
    glbSetOscParams(input_errors, 0.0, i);
  }
  glbSetInputErrors(input_errors);
  glbSetCentralValues(true_values);
  
  /*      Define the projection vector for θ₁₃ and δCP: we will again     *
   *      scan θ₁₃  and δCP, but this time marginalizing (minimizing)     *
   *      over θ₁₂ and Dm₂₁. The other parameters will remain fixed       *
   *      according to the true_values.                                   */
  //double projection_condition[12] = {GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED};
  for (int i = 0; i < 92; ++i) {
    if (i == 7) { //|| i == 1) { // θ₁₃ and Δm²₂₁
      glbSetProjectionFlag(delta_projection, GLB_FREE, i);
    } else {
      glbSetProjectionFlag(delta_projection, GLB_FIXED, i);
    } // θ₁₃ & Δm²₂₁ free, all others fixed
  }
  glbSetDensityProjectionFlag(delta_projection, GLB_FIXED, 0);
  glbSetProjection(delta_projection);

  glbSetOscillationParameters(true_values);
  glbSetRates();


  // Events Output //
    int w;
    int n_bins = glbGetNumberOfBins(0);
    double *true_rates_N = glbGetSignalRatePtr(0,0);
    double *bg_rates_N = glbGetBGRatePtr(0,0);
    double *center_bin_N = glbGetBinCentersListPtr(0);
    double *size_bin_N = glbGetBinSizeListPtr(0); 
    for(w=0;w<n_bins;w++) fprintf(outfile2,"%g %g %g %g \n", center_bin_N[w], size_bin_N[w] ,true_rates_N[w], bg_rates_N[w]);
    fclose(outfile2);

  // χ² Output //
  /*
    double this_delta;
    double res;
  
    for(this_delta= -1 * M_PI; this_delta<= M_PI; this_delta+=M_PI*0.01) {
      glbSetOscParams(test_values, this_delta, 3);
      res = glbChiNP(test_values, NULL, 0); // Compute chi^2 with the test values
      fprintf(outfile, "%g %g\n", this_delta/M_PI, res);
      
    }
    fclose(outfile);
  

  // Probability Output //
    double en = 0.0;
    double pVacme = 0.0;

    for(en= -1 ; en<= 1; en+=(2)*0.01) {
      pVacme = glbVacuumProbability(1, 2, +1 , pow(10, en), 1300); // Probability from ν_μ to ν_e
      fprintf(outfile1, "%g %g\n", pow(10, en), pVacme);
    }
    fclose(outfile1);
  */



/*
  // Sterile χ² Output FD// 
    double this_theta14, this_theta24, this_stdm, en14, en24, enstdm;
    double chi_now, min_chi = 1e30;
  
   // glbSetOscParams(test_values, this_theta14, 7); // Set θ₁₄ SUP
    for(en24 = -5; en24 <= -0.0001; en24 += 0.1) {
      this_theta24 = asin(sqrt(pow(10, en24)));
      //glbSetOscParams(test_values, this_theta24, 8); // Set θ₂₄
      
      for(enstdm = -4; enstdm <= 2; enstdm += 0.1) {
          this_stdm = pow(10, enstdm);
          //glbSetOscParams(test_values, this_stdm, 6); // Set Δm²₄₁    
          
          //for(en14= -4; en14 <= -0.0001; en14 += 0.05) {
            min_chi = 1e30; // Reset min_chi for each en14
            //this_theta14 = asin(sqrt(pow(10, en14))); 
            glbSetOscParams(test_values, this_theta24, 8); // Set θ₂₄
            //glbSetOscParams(test_values, this_theta14, 7); // Set θ₁₄
            glbSetOscParams(test_values, this_stdm, 6); // Set Δm²₄₁    
            chi_now = glbChiNP(test_values, NULL, 0);
            //chi_now = glbChiSys(test_values, GLB_ALL, GLB_ALL); // Compute chi^2 with the test values
            //if (chi_now < min_chi) {min_chi = chi_now;}
          
         // }
          fprintf(outfile3, "%g %g %g \n",  pow(10, en24), this_stdm, chi_now);
    }
    }
    fclose(outfile3);
*/

  
  // Sterile χ² Output ND// 
    double this_theta24, this_stdm;
    double chi_now;

    fprintf(outfile3, "# sin^2(theta_24)   Delta_m^2_41   chi^2\n");

    for (double en24 = -5.0; en24 <= -0.1; en24 += 0.1) {
        double sin2_th24 = pow(10, en24);
        this_theta24 = asin(sqrt(sin2_th24));
        glbSetOscParams(test_values, this_theta24, 8); // Set test th24

        for (double enstdm = -4.0; enstdm <= 2.0; enstdm += 0.1) {
            this_stdm = pow(10, enstdm);
            glbSetOscParams(test_values, this_stdm, 6); // Set test dm41

            // The other standard parameters in test_values can be copied from true_values as they are not being varied here
            for (int i=0; i<6; i++) glbSetOscParams(test_values, glbGetOscParams(true_values, i), i);


            // Calculate the chi^2 comparing the sterile hypothesis to the standard one
            chi_now = glbChiSys(test_values, GLB_ALL, GLB_ALL); // Use glbChiSys for clarity on which rule to use

            fprintf(outfile3, "%g %g %g\n", sin2_th24, this_stdm, chi_now);
        }
    }
    fclose(outfile3);




  /* Destroy parameters */
  glbFreeParams(true_values);
  glbFreeParams(test_values);
  glbFreeParams(input_errors);
  glbFreeProjection(delta_projection);
  snu_free_probability_engine();

  return 0;
}