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

/* Output file */
//char MYFILE[]="chi_atm_DUNE_T2K.dat";
//char MYFILE2[]="chi_delta_DUNE1.dat";
char MYFILE[]="delta_m_inv_flip_360.dat";

FILE *outfile = NULL;
//FILE *outfile2 = NULL;

int main(int argc, char *argv[])
{ 
  /* Initialize libglobes */
  glbInit(argv[0]); 

  /* Initialize experiment */
 glbInitExperiment("DUNE_GLoBES.glb",&glb_experiment_list[0],&glb_num_of_exps); 
 glbInitExperiment("T2K.glb",&glb_experiment_list[0],&glb_num_of_exps); 


  /* Intitialize outputs */
  outfile = fopen(MYFILE, "w");
  if (outfile == NULL)
  {
    printf("Error opening output file.\n");
    return -1;
  }
  
  

 /* Defina os valores "true" para os parâmetros de oscilação (valores retirados de NuFit5.1 para ordenamento normal de massas)  */
  double theta12 = asin(sqrt(0.308));
  double theta13 = asin(sqrt(0.02224));
  double theta23 = asin(sqrt(0.562));
  double deltacp = 285.0 * M_PI / 180.0; // Convert to radians
  double sdm = 7.49e-5;
  double ldm = -2.510e-3;
  
  /* Define "true" oscillation parameter vector */
  glb_params true_values = glbAllocParams();
  glbDefineParams(true_values,theta12,theta13,theta23,deltacp,sdm,ldm);
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  /*
  double theta12_test = asin(sqrt(0.307));
  double theta13_test = asin(sqrt(0.02195));
  double theta23_test = asin(sqrt(0.561));
  double deltacp_test = 177.0 * M_PI / 180.0; // Convert to radians
  double sdm_test = 7.49e-5;
  double ldm_test = 2.534e-3;
  */
 
  /* Define initial guess for the fit values */ 
  glb_params test_values = glbAllocParams();
  glbDefineParams(test_values,theta12,theta13,theta23,deltacp,sdm,ldm);  
  glbSetDensityParams(test_values,1.0,GLB_ALL);
  
  
   /* Defina um erro externo aos seus parâmetros: 10% de erro         *  
   * nos parâmetros solares. Nesse caso, \theta_12 e \Delta m^2_{21}  *
   * deverão ser parâmetros livres e minimizados no \chi^2            */
  glb_params input_errors = glbAllocParams();
  glbDefineParams(input_errors, 0.0, 0.03*theta13, 0.0, 0.0, 0.0, 0.0);
  glbSetInputErrors(input_errors);
  glbSetCentralValues(true_values);
  
  /* Defina o vetor projeção de theta_13 e delta_CP: faremos novamente
   * a verredura de theta_13 e delta_CP, mas dessa vez marginalizando 
   * (minimizando) sob theta_12 and Dm21. Os outros parâmetros
   * permanecerão fixados de acordo com os true_values  */
  glb_projection delta_projection = glbAllocProjection();
  glbDefineProjection(delta_projection,GLB_FIXED,GLB_FIXED,GLB_FREE,GLB_FIXED,GLB_FIXED,GLB_FIXED);
  glbSetDensityProjectionFlag(delta_projection, GLB_FIXED, GLB_ALL);
  glbSetProjection(delta_projection);  
  
  
  /* Compute simulated data */
  glbSetOscillationParameters(true_values);
  glbSetRates();
  

 /* Item a */ 
 /*
    double this_delta;
    double res1;
    int aux = 0;
  
     for(this_delta= -1 * M_PI; this_delta<= M_PI; this_delta+=M_PI*0.01)
    {

      glbSetOscParams(test_values, this_delta, GLB_DELTA_CP);

      res1 = glbChiNP(test_values, NULL, 0);
      fprintf(outfile, "%g %g\n", this_delta/M_PI, res1);

      printf("%d \n", aux);
      aux = aux +1;
    }
  fclose(outfile);
  */



  /* Item b (e c) */
  /*
   double res;
   double x,z;

  
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
  }
  
  fclose(outfile);
  
  */

  /*
  // Sweep delta_CP from -π to π and marginalize over θ_23
  double delta, res2;
  double theta23_test;
  double min_chi2;
  int steps_theta23 = 50;

  for (delta = -M_PI; delta <= M_PI; delta += M_PI * 0.02)
  {
      min_chi2 = 1e10; // large number to start min comparison

      for (int i = 0; i < steps_theta23; i++)
      {
          // Try values for theta_23 between 0.3 and 0.7 (sin^2)
          double sin2_theta23 = 0.3 + i * ((0.7 - 0.3) / (steps_theta23 - 1));
          theta23_test = asin(sqrt(sin2_theta23));

          // Set test parameters
          glbSetOscParams(test_values, delta, GLB_DELTA_CP);
          glbSetOscParams(test_values, theta23_test, GLB_THETA_23);

          // Calculate χ²
          res2 = glbChiNP(test_values, NULL, GLB_ALL);

          if (res2 < min_chi2)
              min_chi2 = res2;
      }

      // Output: δ_CP (in units of π), χ² marginalized over θ_23
      fprintf(outfile2, "%g %g\n", delta / M_PI, min_chi2);

      printf("%d \n", aux);
      aux = aux +1;
  }
  fclose(outfile2);
  */
  
  /* Destroy parameter */
  glbFreeParams(true_values);
  glbFreeParams(test_values);
  glbFreeParams(input_errors);
  glbFreeProjection(delta_projection);

  return 0;
}

