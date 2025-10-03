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
char MYFILE2[]="chi_DUNE_3+1_theta14.dat";
char MYFILE1[]="probability_DUNE_3+1.dat";


FILE *outfile1 = NULL;
FILE *outfile2 = NULL;

int main(int argc, char *argv[])
{ 
  /* Initialize libglobes */
  glbInit(argv[0]); 

  /* Initialize experiment */
 glbInitExperiment("DUNE_ND.glb",&glb_experiment_list[0],&glb_num_of_exps); 
 glbInitExperiment("DUNE_GLoBES.glb",&glb_experiment_list[0],&glb_num_of_exps); 

 int n_flavors = 4;
 int rotation_order[][2]= { {3,4}, {2,4}, {1,4}, {2,3}, {1,3}, {1,2} };
 int phase_order[]= { -1,  2, 1, -1,  0, -1};
 snu_init_probability_engine(n_flavors, rotation_order, phase_order);


glbRegisterProbabilityEngine(92,
&snu_probability_matrix,
&snu_set_oscillation_parameters,
&snu_get_oscillation_parameters,
NULL);

  /* Intitialize outputs */
  outfile1 = fopen(MYFILE1, "w");
  if (outfile1 == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }

    /* Intitialize outputs */
  outfile2 = fopen(MYFILE2, "w");
  if (outfile2 == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }

  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_projection delta_projection = glbAllocProjection();
  
  
 /* Defina os valores "true" para os parâmetros de oscilação (valores retirados de NuFit5.1 para ordenamento normal de massas)  */
  double theta12 = asin(sqrt(0.303));
  double theta13 = asin(sqrt(0.02246));
  double theta23 = asin(sqrt(0.450));
  double deltacp = 212*M_PI/180;
  double sdm = 7.42e-5;
  double ldm = 2.51e-3;

  glbSetOscParams(true_values, theta12, 0);
  glbSetOscParams(true_values, theta13, 1);
  glbSetOscParams(true_values, theta23, 2);
  glbSetOscParams(true_values, deltacp, 3);
  glbSetOscParams(true_values, sdm, 4);
  glbSetOscParams(true_values, ldm, 5);

  for (int i=6; i < glbGetNumOfOscParams(); i++) glbSetOscParams(true_values, 0.0, i);

  glbSetDensityParams(true_values,1.0, 0);

  // Sterile parameters 
 /*
  double stdm = 1.0;
  double theta14= asin(sqrt(0.01));
  double theta24= asin(sqrt(0.01));
  double theta34= asin(sqrt(0.01));

  // Sterile Parameters  
  glbSetOscParams(true_values, stdm, 6);
  glbSetOscParams(true_values, theta14, 7);
  glbSetOscParams(true_values, theta24, 8);
  glbSetOscParams(true_values, theta34, 9);*/

  for (int i=0; i < glbGetNumOfOscParams(); i++)
      printf("%s = %g\n", snu_param_strings[i], glbGetOscParams(true_values,i));
  
  // test_values
  glbCopyParams(true_values,test_values);
 
  // input_errors
  for (int i=0; i < glbGetNumOfOscParams(); i++) glbSetOscParams(input_errors, 0.0, i);

  glbSetDensityParams(input_errors,0.0, GLB_ALL);
  glbSetInputErrors(input_errors);
  glbSetCentralValues(true_values);

  // projection_flag
  for (int i=0; i < glbGetNumOfOscParams(); i++) glbSetProjectionFlag(delta_projection, GLB_FIXED, i);
  glbSetProjectionFlag(delta_projection, GLB_FREE, 7);
  //glbSetProjectionFlag(delta_projection, GLB_FREE, 2);
 // glbSetProjectionFlag(delta_projection, GLB_FREE, 5);

  glbSetDensityProjectionFlag(delta_projection, GLB_FIXED, GLB_ALL);
  glbSetProjection(delta_projection);
  
  /* Compute simulated data */
  glbSetOscillationParameters(true_values);
  glbSetRates();


 /* Item a */
 int n=0;
 double this_x, this_y, this_z;
  double res;
  
     for(this_y= -5; this_y<=-0.0001; this_y+=0.1){
     for(this_z= -4; this_z<=2; this_z+=0.05){
    

      glbSetOscParams(test_values, pow(10,this_z), 6);
      glbSetOscParams(test_values, asin(sqrt(pow(10,this_y))), 8);//24

      res = glbChiNP(test_values, NULL, GLB_ALL);
      n++;
      fprintf(outfile2, "%g %g %g\n", pow(10,this_y), pow(10,this_z), res);
      printf("%d \n", n);
    } 

  }
 
  /* Item b (e c) */
 /* double res;
   double x,z;
   int aux = 0;

  for(z=0.3; z<=0.7; z+=(0.7-0.3)*0.02)
  {
    for(x=2e-3; x<= 3.5e-3; x+=0.02*(3.5e-3-2e-3))
    {
      // Set vector of test=fit values 
      glbSetOscParams(test_values, asin(sqrt(z)), GLB_THETA_23);
      glbSetOscParams(test_values, x, GLB_DM_31);

  
      res = glbChiNP(test_values, NULL, GLB_ALL);
      fprintf(outfile, "%g %g %g\n", z, x/(1e-3), res);
    }
    printf("%d \n", aux);
    aux = aux +1;
  }*/
  
  fclose(outfile1);
  fclose(outfile2);

  
  /* Destroy parameter */
  glbFreeParams(true_values);
  glbFreeParams(test_values);
  glbFreeParams(input_errors);
  glbFreeProjection(delta_projection);
  snu_free_probability_engine();

  return 0;
}

