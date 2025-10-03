import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm 

def chimin(data):
    for i in range (0, data.shape[0]):
        print(np.len(data))

def plotchi(data):
    TH13, DELTA = np.meshgrid(np.unique(data[:,0]), np.unique(data[:,1]))
    CHI2 = (data[:,2]).reshape(len(np.unique(data[:,0])), len(np.unique(data[:,1]))).T
    min = np.argmin(data[:,2])
    best_theta13, best_delta, min_chi2, levels = data[min,0], data[min,1], data[min,2], [min + 9.210]

    plt.figure(figsize=(8,6))
    cp = plt.contourf(TH13, DELTA, CHI2, levels=50, cmap='viridis')
    contours = plt.contour(TH13, DELTA, CHI2, levels=levels, colors=['red', 'blue'], linestyles=['--', '-.'])
    plt.colorbar(cp, label=r'$\chi^2$')

    plt.clabel(contours, fmt={levels[0]:'99%'}, fontsize=12)
    plt.clabel(contours, fmt={levels[0]:'99%'}, fontsize=12)
    plt.plot(best_theta13, best_delta, 'ko', label='Best fit')
    plt.xlabel(r'$\theta_{23}$ (degrees)')
    plt.ylabel(r'$\Delta$m$_{23}^{2}$ (degrees)')
    plt.title(r'$\chi^2$ confidence regions with $\theta_{23}$ vs $\Delta$m$_{23}^{2}$')
    plt.legend()
    plt.grid(True)
    plt.show()
    print(f"Best fit point: θ₂₃ = {best_theta13:.2f} degrees, Δm₂₃² = {best_delta:.2f} degrees, χ² = {min_chi2:.2f}")

def plotchi2(chi_data):
    theta14 = chi_data[:,0]
    delta_cp = chi_data[:,1]
    chi_total = chi_data[:,2]
    TH13, DELTA = np.meshgrid(np.unique(theta14), np.unique(delta_cp))
    CHI2 = chi_total.reshape(len(np.unique(theta14)+1), len(np.unique(delta_cp))).T
    min_idx = np.argmin(chi_total)
    best_theta14 = theta14[min_idx]
    best_delta = delta_cp[min_idx]
    min_chi2 = chi_total[min_idx]
    #levels = [min_chi2 + 9.210] # 99% confidence level for 2 degrees of freedom
    #levels = [min_chi2 + 4.605] # 90% confidence level for 2 degrees of freedom
    levels = [min_chi2 + 9.210] # 99% confidence level for 2 degrees of freedom

    plt.figure(figsize=(10,8))
    cp = plt.contourf(TH13, DELTA, CHI2, levels=50, cmap='viridis')
    plt.colorbar(cp, label=r'$\chi^2$')

    contours = plt.contour(TH13, DELTA, CHI2, levels=levels, colors=['white', 'blue'], linestyles=['--', '-.'])
    #plt.clabel(contours, fmt={levels[0]:'99%'}, fontsize=12)
    plt.clabel(contours, fmt={levels[0]:'99%'}, fontsize=12)
    #plt.plot(best_theta14, best_delta, 'ko', label='Best fit')

    plt.xlabel(r'$\sin^2 \theta_{24}$ (degrees)')
    plt.ylabel(r'$\Delta$m$_{14}^{2}$ (degrees)')
    plt.xscale('log')
    plt.yscale('log')
    plt.title(r'$\chi^2$ confidence regions with $\theta_{24}$ vs $\Delta$m$_{14}^{2}$')
    plt.legend()
    plt.grid(True)
    plt.show()
    print(f"Best fit point: θ₂₃ = {best_theta14:.2f} degrees, Δm₂₃² = {best_delta:.2f} degrees, χ² = {min_chi2:.2f}")

def main():
    #chimin(np.loadtxt("chi_sterile.dat"))
    #plotchi2(np.loadtxt("23finechimin_probe4.dat"))
    plotchi2(np.loadtxt("ND_chi_final.dat"))
    plotchi2(np.loadtxt("chi_dune_3+1_theta14.dat"))
    #plotchi2(np.loadtxt("ND_chiNEW3.dat"))
    #plotchi2(np.loadtxt("ND_chi.dat"))
    #plotchi2(np.loadtxt("new_sml_chimin_probe4.dat"))
    #plotchi2(np.loadtxt("old_sml_chimin_probe4.dat"))


if __name__ == "__main__":
    main()