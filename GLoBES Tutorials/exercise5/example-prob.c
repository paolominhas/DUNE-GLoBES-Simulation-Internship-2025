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
 * Example: Non-Standard-Interactions and user-defined priors
 * Compile with ``make example6''
 *
 * This example is similar to Chapter 4 of hep-ph/0502147
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <globes/globes.h>   /* GLoBES library */
#include "myio.h"            /* my input-output routines */

#define GLB_ALPHA 6        /* Index of non-standard parameter alpha */

/* If filename given, write to file; for empty filename write to screen */
char MYFILE1[]="test-chi-prob-comb.dat";
char MYFILE2[]="test-ev-prob.dat";

FILE *outfile = NULL;
FILE *outfile1 = NULL;


/* Square of real number */
inline double square(double x)
{
    return x*x;
}


/***************************************************************************
 *     U S E R - D E F I N E D   P R O B A B I L I T Y   E N G I N E       *
 ***************************************************************************/

double th12;
double th13;
double th23;
double deltacp;
double sdm;
double ldm;
double alpha; /* em GeV/km ou MeV/m */

/***************************************************************************
 * Store oscillation parameters in internal data structures.               *
 * For more sophisticated probability engines, this would be the right     *
 * place to pre-compute the mixing matrix and parts of the Hamiltonian in  *
 * order to speed up the calls to the actual probability matrix function.  *
 ***************************************************************************/
int my_set_oscillation_parameters(glb_params p, void *user_data)
{
  th12    = glbGetOscParams(p, GLB_THETA_12);
  th13    = glbGetOscParams(p, GLB_THETA_13);
  th23    = glbGetOscParams(p, GLB_THETA_23);
  deltacp = glbGetOscParams(p, GLB_DELTA_CP);
  sdm     = glbGetOscParams(p, GLB_DM_21);   
  ldm     = glbGetOscParams(p, GLB_DM_31);   
  alpha = glbGetOscParams(p, GLB_ALPHA);

  return 0;
}


/***************************************************************************
 * Write oscillation parameters from internal data structures into p.      *
 ***************************************************************************/
int my_get_oscillation_parameters(glb_params p, void *user_data)
{
  glbSetOscParams(p, th12, GLB_THETA_12);
  glbSetOscParams(p, th13, GLB_THETA_13);
  glbSetOscParams(p, th23, GLB_THETA_23);
  glbSetOscParams(p, deltacp, GLB_DELTA_CP);
  glbSetOscParams(p, sdm, GLB_DM_21);  
  glbSetOscParams(p, ldm, GLB_DM_31);  
  glbSetOscParams(p, alpha, GLB_ALPHA);

  return 0;
}


/***************************************************************************
 * Calculate oscillation probabilities.                                    *                 *
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
  double Delta21, Delta31, Delta32;
  double D3;
  double s13, c13, s12, c12;
  double t;
  
  /* Set all probabilities to zero initially */
  for (i=0; i < 3; i++)
    for (j=0; j < 3; j++)
      P[i][j] = 0.0;

  /* Calculate total baseline */
  L = 0.0;
  for (i=0; i < psteps; i++)
    L += length[i];


  /* Compute P_ee */
  s12 = sin(th12);
  c12 = cos(th12);
  s13 = sin(th13);
  c13 = cos(th13);
  t = (1.27*L) / (E);
  Delta21 = sdm * t;
  Delta31 = ldm * t;
  Delta32 = Delta31 - Delta21;
  D3 = exp( -alpha/2 * L/E );
 P[0][0] = square(square(c13)) * ( 1 - 2.0*square(s12*c12)*(1 - cos(2.0*Delta21)) )
               + 2.0*square(s13*c13) * ( D3*square(c12)*cos(2.0*Delta31)
                                       + D3*square(s12)*cos(2.0*Delta32) )
              + square(square(s13));
              
  /*P[0][0] = square(square(c13)) * ( 1 - 2.0*square(s12*c12)*(1 - D3*cos(2.0*Delta21)) )
               + 2.0*square(s13*c13) * ( square(c12)*cos(2.0*Delta31)
                                       + D3*square(s12)*cos(2.0*Delta32) )
              + square(square(s13));    */   

  return 0;
}

/***************************************************************************
 *                            M A I N   P R O G R A M                      *
 ***************************************************************************/

int main(int argc, char *argv[])
{ 

  /* Initialize libglobes */
  glbInit(argv[0]);
 
  /* Register non-standard probability engine. This has to be done
   * before any calls to glbAllocParams() or glbAllocProjections() */
  glbRegisterProbabilityEngine(7,      /* Number of parameters */
                               &my_probability_matrix,
                               &my_set_oscillation_parameters,
                               &my_get_oscillation_parameters,
                               NULL);

  /* Initialize reactor experiment */
  glbInitExperiment("Reactor1.glb",&glb_experiment_list[0],&glb_num_of_exps); 
  glbInitExperiment("TOYdraft_nue.glb",&glb_experiment_list[0],&glb_num_of_exps); 
  
    /* Intitialize outputs */
  outfile = fopen(MYFILE1, "w");
  if (outfile == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }
  
  outfile1 = fopen(MYFILE2, "w");
  if (outfile1 == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }
  
  
   /* Defina os valores "true" para os parâmetros de oscilação (valores retirados de NuFit5.1 para ordenamento normal de massas)  */
  double theta12 = asin(sqrt(0.303));
  double theta13 = asin(sqrt(0.02246));
  double theta23 = asin(sqrt(0.450));
  double deltacp = 230*M_PI/180;
  double sdm = 7.42e-5;
  double ldm = 2.51e-3;
  
  /* Defina o novo valor do parâmetro de decaimento alpha */
  double true_alpha = 0.0;



 
  /* Initialize parameter  vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();


  glbDefineParams(true_values,theta12,theta13,theta23,
                              deltacp,sdm,ldm);
  glbSetOscParams(true_values,true_alpha, GLB_ALPHA);   /* parâmetro não-padrão */
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  
  
  glbDefineParams(test_values,theta12,theta13,theta23,
                              deltacp,sdm,ldm);
  glbSetOscParams(test_values,true_alpha, GLB_ALPHA);   /* parâmetro não-padrão */
  glbSetDensityParams(test_values,1.0,GLB_ALL);
  
  
  glb_params input_errors = glbAllocParams();
  glbDefineParams(input_errors, 0.0, 0.1*theta13, 0.0, 0.0, 0.0, 0.0);  
  glbSetOscParams(input_errors, 0.0, GLB_ALPHA);      /* parâmetro não-padrão */
  glbSetDensityParams(input_errors, 0.0, GLB_ALL);
  glbSetCentralValues(true_values);
  glbSetInputErrors(input_errors);
  
  
  glb_projection alpha_projection = glbAllocProjection(); 
  glbDefineProjection(alpha_projection, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FIXED);
  glbSetDensityProjectionFlag(alpha_projection, GLB_FIXED, GLB_ALL);
  glbSetProjectionFlag(alpha_projection, GLB_FIXED, GLB_ALPHA);  /* parâmetro não-padrão */
  glbSetProjection(alpha_projection); 


  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetRates();


    /* Calcule a taxa de eventos */
    int w;
    int n_bins = glbGetNumberOfBins(0);
    double *true_rates_N = glbGetSignalRatePtr(0,0);
    double *bg_rates_N = glbGetBGRatePtr(0,0);
    double *center_bin_N = glbGetBinCentersListPtr(0);
    double *size_bin_N = glbGetBinSizeListPtr(0); 
    for(w=0;w<n_bins;w++) fprintf(outfile1,"%g %g %g %g \n", center_bin_N[w], size_bin_N[w] ,true_rates_N[w], bg_rates_N[w]);
   

   /* Calcule o chi^2 para theta_13 e alpha assumindo todos os outros parâmetros de oscilação fixos */
   /* Scan the alpha plane */
  double y;
  double res;
  

    for(y = 0; y<=0.009; y+=0.009*0.01)
    {
      /* Set vector of test=fit values */
      glbSetOscParams(test_values, y, GLB_ALPHA);

      /* Compute chi^2 assuming the normal mass hierarchy in the fit */
      res=glbChiNP(test_values,NULL,GLB_ALL);
      fprintf(outfile, "%g %g \n", y, res);
    }
 

  fclose(outfile);
  fclose(outfile1);

  
  /* Destroy parameter and projection vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors); 
  glbFreeProjection(alpha_projection);
    
  exit(0);
}

