/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2007,  The GLoBES Team
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
 
 /* 
 * Example: GLoBES tour
 * Compile with ``make example-tour''
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gsl/gsl_math.h>    /* GNU Scientific library (required for root finding) */
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

#include <globes/globes.h>   /* GLoBES library */
#include "myio.h"            /* my input-output routines */

#include "syst_func_LSND.h"


#define GLB_THETA_ME 6        /* Index of non-standard parameter thme*/
#define GLB_DM_41 7          /* Index of non-standard parameter stdm*/


/***************************************************************************
 *     U S E R - D E F I N E D   P R O B A B I L I T Y   E N G I N E       *
 ***************************************************************************/

/*double th12;
double th13;
double th23;
double deltacp;
double sdm;
double ldm;*/
double thme;
double stdm;

/***************************************************************************
 * Store oscillation parameters in internal data structures.               *
 * For more sophisticated probability engines, this would be the right     *
 * place to pre-compute the mixing matrix and parts of the Hamiltonian in  *
 * order to speed up the calls to the actual probability matrix function.  *
 ***************************************************************************/
int my_set_oscillation_parameters(glb_params p, void *user_data)
{
/*  th12    = glbGetOscParams(p, GLB_THETA_12);
  th13    = glbGetOscParams(p, GLB_THETA_13);
  th23    = glbGetOscParams(p, GLB_THETA_23);
  deltacp = glbGetOscParams(p, GLB_DELTA_CP);
  sdm     = glbGetOscParams(p, GLB_DM_21);   
  ldm     = glbGetOscParams(p, GLB_DM_31);*/
  thme    = glbGetOscParams(p, GLB_THETA_ME);
  stdm    = glbGetOscParams(p, GLB_DM_41);

  return 0;
}

/***************************************************************************
 * Write oscillation parameters from internal data structures into p.      *
 ***************************************************************************/
int my_get_oscillation_parameters(glb_params p, void *user_data)
{
 /* glbSetOscParams(p, th12, GLB_THETA_12);
  glbSetOscParams(p, th13, GLB_THETA_13);
  glbSetOscParams(p, th23, GLB_THETA_23);
  glbSetOscParams(p, deltacp, GLB_DELTA_CP);
  glbSetOscParams(p, sdm, GLB_DM_21);
  glbSetOscParams(p, ldm, GLB_DM_31);*/
  glbSetOscParams(p, thme, GLB_THETA_ME);
  glbSetOscParams(p, stdm, GLB_DM_41);
  return 0;
}

/***************************************************************************
 * Calculate oscillation probabilities.                                    *
 * Since for our setup, only P_ee is required, all other entries of P are  *
 * set to zero for simplicity. Furthermore, we neglect matter effects and  *
 * the filter feature (parameter filter_sigma).                            *
 ***************************************************************************
 * Parameters:                                                             *
 *   P:            The buffer where the probabilities are to be stored     *
 *   cp_sign:      +1 if probalities for neutrinos are requested, -1 for   *
 *                 anti-neutrinos.                                         *
 *   E:            The neutrino energy in GeV                              *
 *   psteps:       Number of constant density layers in the matter profile *
 *   length:       The lengths of these layers in km                       *
 *   density:      The individual densities of these layers in g/cm^3      *
 *   filter_sigma: Width of low-pass filter as given in the AEDL file      *
 ***************************************************************************/
int my_probability_matrix(double P[3][3], int cp_sign, double E, int psteps,
                          const double *length, const double *density,
                          double filter_sigma, void *user_data)
{
  int i, j;
  double L;
  double Delta41;
  double sme;
  double t;
   
  /* Set all probabilities to zero initially */
  for (i=0; i < 3; i++)
    for (j=0; j < 3; j++)
      P[i][j] = 0.0;

  /* Calculate total baseline */
  L = 0.0;
  for (i=0; i < psteps; i++)
    L += length[i];
 
  t = (1.27 * L)/(E);
  Delta41 = stdm * t;
  
  /* Compute P_me */
  sme = sin(2*thme);
  P[1][0] = pow(sme,2) * pow(sin(Delta41),2);

   return 0;
}

/***************************************************************************
 *                            M A I N   P R O G R A M                      *
 ***************************************************************************/
int main(int argc, char *argv[])
{
  /* char* MYFILE=""; */ 
  char* MYFILE="./results/lsnd-ev-osc-teste.dat"; //events.dat"; 
  FILE* stream;
  if(strlen(MYFILE)>0) stream=fopen(MYFILE, "w+");
   else stream = stdout;
 
  char* MYFILE1="./results/lsnd-chi-osc-teste.dat"; //chiME.dat";
  FILE* stream1;
  if(strlen(MYFILE1)>0) stream1=fopen(MYFILE1, "w+");
   else stream1 = stdout;

  /* Define my standard oscillation parameters */
  double true_theta12 = 0.0;
  double true_theta13 = 0.0;
  double true_theta23 = 0.0;
  double true_deltacp = 0.0;
  double true_sdm = 0.0;
  double true_ldm = 0.0;
  double true_thetame = asin(sqrt(0.00316228))/2;
  double true_stdm = 1.12;

 
  glbInit(argv[0]); /* Initialize GLoBES and define chi^2 functions */
  glbDefineChiFunction(&chiLSND,     2,        "chiLSND",     NULL);

  /* Register non-standard probability engine. This has to be done
   * before any calls to glbAllocParams() or glbAllocProjections() */
  glbRegisterProbabilityEngine(8,      /* Number of parameters */
                               &my_probability_matrix,
                               &my_set_oscillation_parameters,
                               &my_get_oscillation_parameters,
                               NULL);

  /* Initialize one experiment .glb */
 glbInitExperiment("LSND.glb",&glb_experiment_list[0],&glb_num_of_exps);

  
  /* Initialize a number of parameter vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
    
  /* Assign standard oscillation parameters */
  glbDefineParams(true_values,true_theta12,true_theta13,true_theta23, true_deltacp,true_sdm,true_ldm);  
  glbSetOscParams(true_values,true_thetame, GLB_THETA_ME);  
  glbSetOscParams(true_values,true_stdm, GLB_DM_41);  
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  glbDefineParams(test_values,true_theta12,true_theta13,true_theta23, true_deltacp,true_sdm,true_ldm);   
  glbSetOscParams(test_values,true_thetame, GLB_THETA_ME); 
  glbSetOscParams(test_values,true_stdm, GLB_DM_41);         
  glbSetDensityParams(test_values,1.0,GLB_ALL);

  /* The simulated data are computed with "true_values" */
  glbSetOscillationParameters(true_values);
  glbSetRates(); 


  /* Return some low level information */
/*  double ener=0;
  double i;
  for(i=10;i<70;i=i+0.005){
  ener=i;
  double resul=glbProfileProbability(0,2,1,-1,ener);
  fprintf(stream,"%f %f \n",ener,resul);

} */

/*  fprintf(stream,"Simulated rates, experiment 0, rule 0: \n"); */
    int w;
    int n_bins = glbGetNumberOfBins(0);
    double *true_rates_N = glbGetSignalRatePtr(0,0);
    double *bg_rates_N = glbGetBGRatePtr(0,0);
    double *center_bin_N = glbGetBinCentersListPtr(0);
    double *size_bin_N = glbGetBinSizeListPtr(0); 
   for(w=0;w<n_bins;w++) fprintf(stream,"%g %g %g %g \n", center_bin_N[w], size_bin_N[w] ,true_rates_N[w], bg_rates_N[w]); 


   double z,y,chi2; 
   int strovet=1;   
    
   for(z=-4.0;z<=0.0;z=z+4.0/60){
   for(y=-2.0;y<=2.0;y=y+4.0/60){
   
    glbSetOscParams(test_values, asin(sqrt(pow(10,z)))/2, GLB_THETA_ME);
    glbSetOscParams(test_values,pow(10,y), GLB_DM_41); 

      chi2 = glbChiSys(test_values,0,0);
      fprintf(stream1,"%g %g %g \n",z,y,chi2); 

      printf("%i\n",strovet);
      strovet=strovet+1;
      } 
     } 


  /* Destroy parameter vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors); 

  if(strlen(MYFILE)>0) fclose(stream); 
  if(strlen(MYFILE1)>0) fclose(stream1);

  exit(0);
}
