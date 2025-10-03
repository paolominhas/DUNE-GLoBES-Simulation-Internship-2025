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
    log_sin2_2theta = chi_data[:, 0]
    log_dm41_sq = chi_data[:, 1]
    chi2_vals = chi_data[:, 2]
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
    print(f"Best fit point: θ13 = {best_theta13:.2f} degrees, δCP = {best_delta:.2f} degrees, χ² = {min_chi2:.2f}")

def main():
    E, dE, signal, background = inputdata("ev_th13delta.dat")
    plotdata(E, dE, signal, background)
    plotchi2(inputchi2("chi_th13delta_31.dat"))

if __name__ == "__main__":
    main()