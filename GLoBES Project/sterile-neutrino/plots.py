import numpy as np
import matplotlib.pyplot as plt

def readfile(file):
    data = np.loadtxt(file)
    return data

def inputchi2(filename):
    chi_data = np.loadtxt(filename)
    return chi_data

def osc_prob(L, E, theta, delta_m2):
    sin2_2theta = np.sin(2 * theta)**2
    delta = 1.267 * delta_m2 * L / E  # 1.267 from natural units conversion
    return sin2_2theta * np.sin(delta)**2

def calc_prob(L, E, theta, delta_m2):
    Length = np.linspace(0.1, L, 1000)
    P = osc_prob(Length, E, theta, delta_m2)
    return np.column_stack((Length, P))

def plot_probability(data):
    plt.figure(figsize=(8, 5))
    plt.plot(data[:,0], data[:,1])
    plt.xlabel("Baseline L (km)")
    plt.ylabel("Oscillation Probability $P(\\nu_\\alpha \\rightarrow \\nu_\\beta)$")
    plt.title("Two-Neutrino Oscillation Probability vs Distance")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def plot_2probability(data1, data2):
    plt.figure(figsize=(8, 5))
    plt.plot(data1[:,0], data1[:,1], label='Data 1')
    plt.plot(data2[:,0], data2[:,1], label='Data 2', linestyle='--')
    plt.xlabel("Baseline L (km)")
    plt.ylabel("Oscillation Probability $P(\\nu_\\alpha \\rightarrow \\nu_\\beta)$")
    plt.title("Two-Neutrino Oscillation Probability vs Distance")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def chi_cp(data):
    plt.figure(figsize=(8, 5))
    plt.plot(data[:,0], data[:,1])
    plt.xlabel("delta_CP (degrees)")
    plt.ylabel("Chi^2")
    plt.title("Chi^2 vs delta_CP")
    plt.ylim(0, 20)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def chi_compare_cp(data1, data2):
    plt.figure(figsize=(8, 5))
    plt.plot(data1[:,0], data1[:,1], label='3 neutrino model')
    plt.plot(data2[:,0], data2[:,1], label='3+1 using 0 for 4th neutrino parameters', linestyle='--')
    plt.xlabel("delta_CP (degrees)")
    plt.ylabel("Chi^2")
    plt.title("Chi^2 vs delta_CP")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def plotchi2(chi_data):
    theta13 = chi_data[:,0]
    delta_cp = chi_data[:,1]
    chi_total = chi_data[:,2]
    TH13, DELTA = np.meshgrid(np.unique(theta13), np.unique(delta_cp))
    CHI2 = chi_total.reshape(len(np.unique(theta13)), len(np.unique(delta_cp))).T
    min_idx = np.argmin(chi_total)
    best_theta13 = theta13[min_idx]
    best_delta = delta_cp[min_idx]
    min_chi2 = chi_total[min_idx]
    levels = [min_chi2 + 9.210]

    plt.figure(figsize=(8,6))
    cp = plt.contourf(TH13, DELTA, CHI2, levels=50, cmap='viridis')
    plt.colorbar(cp, label=r'$\chi^2$')

    contours = plt.contour(TH13, DELTA, CHI2, levels=levels, colors=['red', 'blue'], linestyles=['--', '-.'])
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

def plot_events(events):
    left_edges = events[:,0] - (events[:,1] / 2)
    right_edges = events[:,0] + (events[:,1] / 2)
    #all_bin_edges = np.append(left_edges[0], right_edges)

    plt.bar(events[:,0], events[:,3], width=events[:,1], align='center', label='Background Events', color='lightcoral')
    plt.bar(events[:,0], events[:,2] - events[:,3], width=events[:,1], align='center', bottom=events[:,3], label='Signal Events', color='skyblue')
    plt.xlabel('Bin Centre Value')
    plt.ylabel('Counts')
    plt.title('Histogram of Events and Background')
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.xlim(min(left_edges), max(right_edges)) # Set x-axis limits to cover all bins
    plt.tight_layout() # Adjust layout to prevent labels from overlapping
    plt.show()

def compare_events(events1, events2):
    left_edges = events1[:,0] - (events1[:,1] / 2)
    right_edges = events1[:,0] + (events1[:,1] / 2)
    #all_bin_edges = np.append(left_edges[0], right_edges)

    #plt.bar(events1[:,0], events[:,3], width=events[:,1], align='center', label='Background Events', color='lightcoral')
    plt.bar(events1[:,0], events1[:,2] + events1[:,3], width=events1[:,1], align='center', label='Normal Heirachy', color='skyblue') #bottom=events1[:,3]
    plt.bar(events2[:,0], events2[:,2] + events2[:,3], width=events2[:,1], align='center', label='Inverted Heirachy', color='red', alpha=0.5)  #bottom=events2[:,3]
    plt.xlabel('Bin Centre Value')
    plt.ylabel('Counts')
    plt.title('Histogram of Events and Background')
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.xlim(0, 8.0) # Set x-axis limits to cover all bins
    plt.tight_layout() # Adjust layout to prevent labels from overlapping
    plt.show()

def calculate_chi_squared(data):
    bin_centre = data[:, 0]
    width = data[:, 1]
    observed = data[:, 2]
    expected = data[:, 3]

    epsilon = 1e-9 # A very small number
    expected_safe = np.where(expected == 0, epsilon, expected)
    chi_squared_contributions = (observed - expected_safe)**2 / expected_safe
    total_chi_squared = np.sum(chi_squared_contributions)
    results_array = np.column_stack((
        bin_centre,
        width,
        observed,
        expected,
        chi_squared_contributions
    ))
    print(f"Total Chi-squared: {total_chi_squared:.2f}")
    print(results_array)
    return results_array, total_chi_squared



def main():
    #plot_probability(calc_prob(100000, 0.6, np.radians(33.0), 7.5e-5))
    #plot_probability(readfile("probabitity_out3.dat"))
    #plot_probability(readfile("probabitity_out4.dat"))
    #plot_2probability(readfile("probabitity_out3.dat"), readfile("probabitity_out4.dat"))
    #chi_compare_cp(readfile("chi_out3.dat"), readfile("chi_out4.dat"))
    #chi_cp(readfile("chi_out3_cp0.dat"))
    #calculate_chi_squared(readfile("events_out3_cp_0.dat"))
    compare_events(readfile("evs_probe4.dat"), readfile("ND_events.dat"))
    #plot_events(readfile("events_out3.dat"))
    #compare_events(readfile("events_out3.dat"), readfile("events_out4.dat"))
    #plotchi2(inputchi2("probability_output_2v.dat"))


if __name__ == "__main__":
    main()