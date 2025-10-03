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
 /***************************************************************************
 *                          Curso GLoBES                                   *
 *                                                                         *
 *             29 de Agosto a 02 de Setembro de 2022                       *
 *                                                                         *
 ***************************************************************************
 *   Parte 3:  Neutrino estéril no LSND                                    *
 ***************************************************************************/
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




/***************************************************************************
 *     U S E R - D E F I N E D   P R O B A B I L I T Y   E N G I N E       *
 ***************************************************************************/



/***************************************************************************
 * Store oscillation parameters in internal data structures.               *
 * For more sophisticated probability engines, this would be the right     *
 * place to pre-compute the mixing matrix and parts of the Hamiltonian in  *
 * order to speed up the calls to the actual probability matrix function.  *
 ***************************************************************************/
int my_set_oscillation_parameters(glb_params p, void *user_data)
{



  return 0;
}

/***************************************************************************
 * Write oscillation parameters from internal data structures into p.      *
 ***************************************************************************/
int my_get_oscillation_parameters(glb_params p, void *user_data)
{




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
   
  char* MYFILE2="./results/lsnd-probability-teste.dat"; //chiME.dat";
  FILE* stream2;
  if(strlen(MYFILE2)>0) stream2=fopen(MYFILE2, "w+");
   else stream2 = stdout;

  /* Defina os valores true para seus parâmetros */
  double true_theta12 = 0.0;
  double true_theta13 = 0.0;
  double true_theta23 = 0.0;
  double true_deltacp = 0.0;
  double true_sdm = 0.0;
  double true_ldm = 0.0;



 
  glbInit(argv[0]); /* Initialize GLoBES and define chi^2 functions */
  glbDefineChiFunction(&chiLSND,     2,        "chiLSND",     NULL);

  /* Register non-standard probability engine. This has to be done
   * before any calls to glbAllocParams() or glbAllocProjections() */
  glbRegisterProbabilityEngine(6 /* Altere para 6 + N, onde N é o número de parâmetros que você adicionou*/,      
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
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  glbDefineParams(test_values,true_theta12,true_theta13,true_theta23, true_deltacp,true_sdm,true_ldm);       
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
  fprintf(stream2,"%f %f \n",ener,resul);

} */

 /* Calcule a taxa de eventos */
    int w;
    int n_bins = glbGetNumberOfBins(0);
    double *true_rates_N = glbGetSignalRatePtr(0,0);
    double *bg_rates_N = glbGetBGRatePtr(0,0);
    double *center_bin_N = glbGetBinCentersListPtr(0);
    double *size_bin_N = glbGetBinSizeListPtr(0); 
   for(w=0;w<n_bins;w++) fprintf(stream,"%g %g %g %g \n", center_bin_N[w], size_bin_N[w] ,true_rates_N[w], bg_rates_N[w]); 


   double z,y,chi2;    
    
   for(z=-4.0;z<=0.0;z=z+4.0/60){
   for(y=-2.0;y<=2.0;y=y+4.0/60){
   
    glbSetOscParams(test_values,    );
    glbSetOscParams(test_values,    ); 

      chi2 = glbChiSys(test_values,0,0);
      fprintf(stream1,"%g %g %g \n",z,y,chi2); 

      } 
     } 


  /* Destroy parameter vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors); 

  if(strlen(MYFILE)>0) fclose(stream); 
  if(strlen(MYFILE1)>0) fclose(stream1);
  if(strlen(MYFILE2)>0) fclose(stream2);
  
  exit(0);
}
