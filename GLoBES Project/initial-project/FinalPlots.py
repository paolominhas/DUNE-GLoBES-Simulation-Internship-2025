import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

def inputdata(filename):
    # Load the data from the file
    data = np.loadtxt(filename)
    E = data[:, 0]                 # bin center
    dE = data[:, 1]                # bin width
    signal = data[:, 2]            # oscillated anti electron neutrino signal
    background = data[:, 3]        # background
    return E, dE, signal, background

def inputchi2(filename):
    # Load the chi2 scan data
    chi_data = np.loadtxt(filename)
    #log_sin2_2theta = chi_data[:, 0]
    #log_dm41_sq = chi_data[:, 1]
    #chi2_vals = chi_data[:, 2]
    return chi_data #log_sin2_2theta, log_dm41_sq, chi2_vals

def plotdata(E, dE, signal, background):
    # Create a bar plot for the data
    plt.figure(figsize=(8,5))
    plt.bar(E, signal, width=dE, label='Signal', alpha=0.3, zorder=2)
    plt.bar(E, background, width=dE, label='Background', alpha=0.5, bottom=signal, zorder=1)
    plt.xlabel("Neutrino Energy (MeV)")
    plt.ylabel("Events per Bin")
    plt.title("LSND Event Spectrum with Sterile Neutrinos (3+1)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plotchi2(chi_data):
    theta13 = chi_data[:,0]
    delta_cp = chi_data[:,1]
    chi_total = chi_data[:,2]

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

    #levels = [min_chi2 + 5.991, min_chi2 + 9.210]
    levels = [min_chi2 + 9.210]

    plt.figure(figsize=(8,6))
    cp = plt.contourf(TH13, DELTA, CHI2, levels=50, cmap='viridis')
    plt.colorbar(cp, label=r'$\chi^2$')

    # Plot confidence level contours
    contours = plt.contour(TH13, DELTA, CHI2, levels=levels, colors=['red', 'blue'], linestyles=['--', '-.'])
    plt.clabel(contours, fmt={levels[0]:'99%'}, fontsize=12)
    plt.clabel(contours, fmt={levels[0]:'99%'}, fontsize=12)

    # Plot best fit point
    plt.plot(best_theta13, best_delta, 'ko', label='Best fit')

    plt.xlabel(r'$\theta_{23}$ (degrees)')
    plt.ylabel(r'$\Delta$m$_{23}^{2}$ (degrees)')
    plt.title(r'$\chi^2$ confidence regions with $\theta_{23}$ vs $\Delta$m$_{23}^{2}$')
    plt.legend()
    plt.grid(True)

    plt.show()
    print(f"Best fit point: θ₂₃ = {best_theta13:.2f} degrees, Δm₂₃² = {best_delta:.2f} degrees, χ² = {min_chi2:.2f}")

def plotchi2compare(chi1, chi2, chicomb):
    # Extract parameters
    theta13 = chi1[:, 0]
    delta_cp = chi1[:, 1]

    # Meshgrid assumption: all datasets use the same theta13/delta_cp grid
    th13_vals = np.unique(theta13)
    delta_vals = np.unique(delta_cp)
    TH13, DELTA = np.meshgrid(th13_vals, delta_vals)

    # Reshape chi2 data
    CHI2_1 = chi1[:, 2].reshape(len(th13_vals), len(delta_vals)).T
    CHI2_2 = chi2[:, 2].reshape(len(th13_vals), len(delta_vals)).T
    CHI2_COMB = chicomb[:, 2].reshape(len(th13_vals), len(delta_vals)).T

    # Best fit points
    min_idx1 = np.argmin(chi1[:, 2])
    min_idx2 = np.argmin(chi2[:, 2])
    min_idx_comb = np.argmin(chicomb[:, 2])

    best1 = theta13[min_idx1], delta_cp[min_idx1], chi1[min_idx1, 2]
    best2 = theta13[min_idx2], delta_cp[min_idx2], chi2[min_idx2, 2]
    best_comb = theta13[min_idx_comb], delta_cp[min_idx_comb], chicomb[min_idx_comb, 2]

    # Confidence levels (2D, 2 parameters)
    #levels = [best_comb[2] + 5.991, best_comb[2] + 9.210]
    levels = [best_comb[2] + 9.210]

    plt.figure(figsize=(9, 7))
    
    # Combined chi2 color map
    cp = plt.contourf(TH13, DELTA, CHI2_COMB, levels=50, cmap='viridis')
    plt.colorbar(cp, label=r'$\chi^2$ (Combined)')

    # Confidence contours (combined)
    contours_comb = plt.contour(TH13, DELTA, CHI2_COMB, levels=levels, colors='white', linestyles=['--', '-.'])
    plt.clabel(contours_comb, fmt={levels[0]: '99%'}, fontsize=12)

    # Overplot individual contours for chi_data1 and chi_data2
    plt.contour(TH13, DELTA, CHI2_1, levels=[best1[2] + 5.991], colors='red', linestyles='--', label='DUNE')
    plt.contour(TH13, DELTA, CHI2_2, levels=[best2[2] + 5.991], colors='blue', linestyles='--', label='T2K')
    

    # Mark best-fit points
    plt.plot(best1[0], best1[1], 'ro', label='DUNE best fit')
    plt.plot(best2[0], best2[1], 'bo', label='T2K best fit')
    plt.plot(best_comb[0], best_comb[1], 'wo', label='Combined best fit')

    plt.xlabel(r'$\theta_{23}$ (degrees)')
    plt.ylabel(r'$\Delta$m$_{23}^{2}$ (degrees)')
    plt.title(r'$\chi^2$ Comparison: DUNE, T2K, and Combined')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    # Print best-fit results
    print(f"Best fit DUNE: θ₂₃ = {best1[0]:.2f}, Δm₂₃² = {best1[1]:.2f}, χ² = {best1[2]:.2f}")
    print(f"Best fit T2K: θ₂₃ = {best2[0]:.2f}, Δm₂₃² = {best2[1]:.2f}, χ² = {best2[2]:.2f}")
    print(f"Best fit (combined): θ₂₃ = {best_comb[0]:.2f}, Δm₂₃² = {best_comb[1]:.2f}, χ² = {best_comb[2]:.2f}")

def plotdelta(inputdata):
    data = np.loadtxt(inputdata)
    delta_cp = data[:, 0] #* np.pi   Convert from units of π to radians if you want to plot in radians
    chi2 = data[:, 1]

    plt.figure(figsize=(8, 5))
    plt.plot(delta_cp, chi2, label=r'$\chi^2(\delta_{CP})$')
    plt.xlabel(r'$\delta_{CP}$ (π radians)')
    plt.ylabel(r'$\chi^2$')
    plt.title(r'$\chi^2$ vs $\delta_{CP}$ (marginalized over $\theta_{23}$)')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def minchi(delta1, delta2):
    data1 = np.loadtxt(delta1)
    data2 = np.loadtxt(delta2)
    data = np.column_stack(((data1[:,0]), (np.minimum(data1[:,1], data2[:,1]))))
    delta_cp = data[:, 0]
    chi2 = data[:, 1]

    plt.figure(figsize=(8, 5))
    plt.plot(delta_cp, chi2, label=r'$\chi^2(\delta_{CP})$')
    plt.xlabel(r'$\delta_{CP}$ (π radians)')
    plt.ylabel(r'$\chi^2$')
    plt.title(r'$\chi^2$ vs $\delta_{CP}$ (marginalized over $\theta_{23}$)')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def chidifference(chi1, chi2):
    chi_1 = np.loadtxt(chi1)
    chi_2 = np.loadtxt(chi2)
    data = np.column_stack(((chi_1[:,0]), np.sqrt((chi_1[:,1] - chi_2[:,1]))))
    plt.figure(figsize=(8, 5))
    plt.plot(data[:,0], data[:,1], label=r'$\chi^2(\delta_{CP})$')
    plt.xlabel(r'$\delta_{CP}$ (π radians)')
    plt.ylabel(r'$\chi^2$')
    plt.title(r'$\chi^2$ vs $\delta_{CP}$ (looking at normal vs inverted hierarchy)')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()


def main():
    #plotdata(inputdata("ev_th13delta.dat"))
    #plotdelta("chi_delta_DUNE.dat")
    #plotdelta("delta_cp_pi.dat")
    #minchi("delta_cp_zero.dat", "delta_cp_pi.dat")
    chidifference("delta_m_inv_360.dat", "delta_m_norm_360.dat")
    chidifference("delta_m_norm_flip_360.dat", "delta_m_inv_flip_360.dat")
    #plotchi2(inputchi2("chi_atm_DUNE.dat"))
    #plotchi2(inputchi2("chi_atm_T2K.dat"))
    #plotchi2(inputchi2("chi_atm_T2K.dat"))
    #plotchi2compare(inputchi2("chi_atm_DUNE.dat"), inputchi2("chi_atm_T2K.dat"), inputchi2("chi_atm_DUNE_T2K.dat"))
    

if __name__ == "__main__":
    main()