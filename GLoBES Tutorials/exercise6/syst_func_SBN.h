/* RATES AND CHI-SQUARED FUNCTIONS FOR LSND */

/* Square of real number */
inline double square(double x)
{
    return x*x;
}

inline double likelihood(double true_rate, double fit_rate, double sqr_sigma)
{
    if (sqr_sigma > 0)
    return square(true_rate - fit_rate) / (sqr_sigma);
    else
    return 0.0;
}


/***************************************************************************
 * Calculate chi^2
 ***************************************************************************/

double chiSBN(int exp, int rule, int np, double *x, double *errors, void* user_data)
 {
  int n_bins = glbGetNumberOfBins(exp);
  double *true_rates_N = glbGetRuleRatePtr(exp+1, rule);
  double *true_rates_F = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates_N = glbGetSignalFitRatePtr(exp+1, rule);
  double *signal_fit_rates_F = glbGetSignalFitRatePtr(exp, rule);
  
  double signal_norm_N, signal_norm_F, bg_norm_F, bg_norm_N;
  int ew_low, ew_high;
  double fit_rate=0;;
  double chi2 = 0.0;
  int l,k;

  double bg_N[] = {1432.89, 1747.82, 1839.79, 1767.84, 1590.96, 1496.06, 1662.78, 1422.64, 1521.9, 1074.33, 1919.6};
  double bg_F[] = {184.895, 180.205, 173.963, 158.396, 153.707, 137.624, 160.664, 127.466, 133.365, 103.075, 204.35};

  /* Loop over all bins in energy window */
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);

  signal_norm_F = 1.0 + x[0];
  signal_norm_N = 1.0 + x[0];
  
  bg_norm_F = 1.0 + x[1];
  bg_norm_N = 1.0 + x[1];


  for (l=ew_low; l <= ew_high; l++){
    
    /* Statistical part of chi^2 for far detector */
    fit_rate  = signal_norm_F * signal_fit_rates_F[l] + bg_norm_F * bg_F[l];
    chi2 += likelihood((true_rates_F[l] + bg_F[l]), fit_rate, (true_rates_F[l] + bg_F[l]));

    /* Statistical part of chi^2 for near detector */
    fit_rate  = signal_norm_N * signal_fit_rates_N[l] + bg_norm_N * bg_N[l];
    chi2 += likelihood((true_rates_N[l] + bg_N[l]), fit_rate, (true_rates_N[l] + bg_N[l])); 
 }

  /* Systematical part of chi^2 (= priors) */
  for (k=0; k < np; k++){
    chi2 += square(x[k] / errors[k]);
    
    }
   
 
  return chi2;
}
