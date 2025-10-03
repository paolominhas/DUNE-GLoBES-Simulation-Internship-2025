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

/***************************************************************************
 *                          Curso GLoBES                                   *
 *                                                                         *
 *             29 de Agosto a 02 de Setembro de 2022                       *
 *                                                                         *
 *  (Baseado no Tutorial de Mark Rolinec dado no                           *
 *   Workshop GLoBES - Physics and Application disponível em:              *
 *   https://www.mpi-hd.mpg.de/lin/events/globes07/program.html)           *
 ***************************************************************************
 *   Parte 2:  Implementando um experimento de neutrinos                   *
 ***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <globes/globes.h>   /* GLoBES library */

/* Output file */
char MYFILE1[]="ev_th13delta.dat";
char MYFILE[]="chi_th13delta.dat";
char AEDLFILE[]="TOYdraft.glb";

FILE *outfile = NULL;
FILE *outfile1 = NULL;

int main(int argc, char *argv[])
{ 
  /* Initialize libglobes */
  glbInit(argv[0]); 

  /* Initialize experiment */
  glbInitExperiment(AEDLFILE,&glb_experiment_list[0],&glb_num_of_exps); 


  /* Intitialize outputs */
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
  

 /* Defina os valores "true" para os parâmetros de oscilação (valores retirados de NuFit5.1 para ordenamento normal de massas)  */
  double theta12 = asin(sqrt(0.303));
  double theta13 = asin(sqrt(0.02246));
  double theta23 = asin(sqrt(0.450));
  double deltacp = 230*M_PI/180;
  double sdm = 7.42e-5;
  double ldm = 2.51e-3;
  
  /* Define "true" oscillation parameter vector */
  glb_params true_values = glbAllocParams();
  glbDefineParams(true_values,theta12,theta13,theta23,deltacp,sdm,ldm);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
 
  /* Define initial guess for the fit values */ 
  glb_params test_values = glbAllocParams();
  glbDefineParams(test_values,theta12,theta13,theta23,deltacp,sdm,ldm);  
  glbSetDensityParams(test_values,1.0,GLB_ALL);
  
  /* Compute simulated data */
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
   

  fclose(outfile);
  fclose(outfile1);
  
  /* Destroy parameter */
  glbFreeParams(true_values);
  glbFreeParams(test_values);

  return 0;
}

