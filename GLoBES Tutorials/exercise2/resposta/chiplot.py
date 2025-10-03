import numpy as np
import matplotlib.pyplot as plt
from matplotlib import ticker

data = np.loadtxt("chi_th13delta.dat")

theta13 = data[:,0]
delta_cp = data[:,1]
chi_total = data[:,2]

# Reshape to 2D grid for plotting - assuming uniform grid steps in theta13 and delta_cp
TH13, DELTA = np.meshgrid(np.unique(theta13), np.unique(delta_cp))

# Reshape chi2 to match the grid shape, note it has to be transposed so the axes end up the right way round.
CHI2 = chi_total.reshape(len(np.unique(theta13)), len(np.unique(delta_cp))).T


# Find best fit (minimum chi2)
min_idx = np.argmin(chi_total)
best_theta13 = theta13[min_idx]
best_delta = delta_cp[min_idx]
min_chi2 = chi_total[min_idx]

# Confidence levels for 2 parameters:
# 95% CL ≈ delta_chi2 = 5.991
# 99% CL ≈ delta_chi2 = 9.210
# We plot contours at chi2 = min_chi2 + delta_chi2

levels = [min_chi2 + 5.991, min_chi2 + 9.210]

plt.figure(figsize=(8,6))
cp = plt.contourf(TH13, DELTA, CHI2, levels=50, cmap='viridis')
plt.colorbar(cp, label=r'$\chi^2$')

# Plot confidence level contours
contours = plt.contour(TH13, DELTA, CHI2, levels=levels, colors=['red', 'blue'], linestyles=['--', '-.'])
plt.clabel(contours, fmt={levels[0]:'95%', levels[1]:'99%'}, fontsize=12)

# Plot best fit point
plt.plot(best_theta13, best_delta, 'ko', label='Best fit')

plt.xlabel(r'$\theta_{13}$ (degrees)')
plt.ylabel(r'$\delta_{CP}$ (degrees)')
plt.title(r'$\chi^2$ confidence regions with $\theta_{13}$ vs $\delta_{CP}$')
plt.legend()
plt.grid(True)

plt.show()