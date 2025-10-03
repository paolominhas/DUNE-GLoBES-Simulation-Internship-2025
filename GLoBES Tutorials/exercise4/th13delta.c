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
char MYFILE[]="chi_th13delta_atm.dat";
char MYFILE1[]="ev_th13delta.dat";
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
  
  
   /* Defina um erro externo aos seus parâmetros: 10% de erro         *  
   * nos parâmetros solares. Nesse caso, \theta_12 e \Delta m^2_{21}  *
   * deverão ser parâmetros livres e minimizados no \chi^2            */
  glb_params input_errors = glbAllocParams();
  glbDefineParams(input_errors, theta12*0.1, 0, 0, 0, sdm*0.1, 0);
  glbSetInputErrors(input_errors);
  glbSetCentralValues(true_values);
  
  /* Defina o vetor projeção de theta_13 e delta_CP: faremos novamente
   * a verredura de theta_13 e delta_CP, mas dessa vez marginalizando 
   * (minimizando) sob theta_12 and Dm21. Os outros parâmetros
   * permanecerão fixados de acordo com os true_values  */
  glb_projection th13delta_projection = glbAllocProjection();
  glbDefineProjection(th13delta_projection,GLB_FIXED,GLB_FIXED,GLB_FREE,GLB_FIXED,GLB_FIXED,GLB_FREE);
  glbSetDensityProjectionFlag(th13delta_projection, GLB_FIXED, GLB_ALL);
  glbSetProjection(th13delta_projection);  
  
  
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
   
  /* Calcule a taxa total de eventos e imprima na tela */
  double *rule_nue  = glbGetRuleRatePtr(0, 0);
  double *rule_anue  = glbGetRuleRatePtr(0, 1);
  double total_nue = 0.0;
  double total_anue = 0.0;
 
  int a;
  for(a=0;a<n_bins;a++){
  total_nue = total_nue + rule_nue[a];
  total_anue = total_anue + rule_anue[a];
  }
  printf("O número total de eventos de nu_e é de: % g \n ", total_nue);
  printf("O número total de eventos de anu_e é de: % g \n", total_anue);

  /* Scan the th13-delta plane */
  double this_th13, this_delta;
  double th13_lower  = 6*M_PI/180.;
  double th13_upper  = 10*M_PI/180.;
  double th13_steps  = 20;
  double delta_lower = 180*M_PI/180.;
  double delta_upper = 320*M_PI/180.;
  double delta_steps = 20;
  double res;
  
  
  for(this_th13=th13_lower; this_th13<=th13_upper; this_th13+=(th13_upper-th13_lower)/th13_steps)
  {
    for(this_delta=delta_lower; this_delta<=delta_upper; this_delta+=(delta_upper-delta_lower)/delta_steps)
    {
      /* Set vector of test=fit values */
      glbSetOscParams(test_values, this_th13, GLB_THETA_13);
      glbSetOscParams(test_values, this_delta, GLB_DELTA_CP);

      /* Compute chi^2 assuming the normal mass hierarchy in the fit */
      res = glbChiNP(test_values, NULL, 0);
      fprintf(outfile, "%g %g %g\n", this_th13*180.0/M_PI, this_delta*180.0/M_PI, res);
    }
  }
  
  fclose(outfile);
  fclose(outfile1);
  
  /* Destroy parameter */
  glbFreeParams(true_values);
  glbFreeParams(test_values);
  glbFreeParams(input_errors);
  glbFreeProjection(th13delta_projection);

  return 0;
}

