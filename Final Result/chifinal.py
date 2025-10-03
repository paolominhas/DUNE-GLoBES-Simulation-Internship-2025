import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm # Import LogNorm for logarithmic color scale

def chi_final():
    # Load the data from the file
    data = np.loadtxt("chi_DUNE_3+1_theta14.dat")

    # Extract the second and third columns
    y = data[:, 1]
    chi_squared = data[:, 2]

    # Create the line plot
    plt.figure(figsize=(8, 6))
    plt.plot(y, chi_squared, marker='o', linestyle='-', label='Chi-Squared Values')

    # Calculate and add the 90% contour line
    max_chi_squared = np.max(chi_squared)
    contour_level = 0.9 * max_chi_squared
    plt.axhline(y=contour_level, color='red', linestyle='--', label='90% of Max Value')

    # Add labels, title, and a legend
    plt.xlabel('Column 2')
    plt.ylabel('Chi-Squared Value')
    plt.title('Chi-Squared Data with 90% Threshold')
    plt.legend()
    plt.grid(True)

    # Save the plot
    plt.savefig("chi_squared_1D_plot.png")

    print("The 1D plot has been saved as chi_squared_1D_plot.png")

def chiplot():
    # Load the data from the file
    data = np.loadtxt("chi_DUNE_3+1_theta14.dat")

    # Extract columns for plotting
    col1 = data[:, 0]
    col2 = data[:, 1]
    chi_squared = data[:, 2]

    # --- Define the Sensitivity Region (Particle Physics Convention) ---
    # Find the minimum chi-squared value (the best-fit point)
    chi_squared_min = np.min(chi_squared)

    # Define the 90% confidence level threshold for 2 degrees of freedom
    # This is the standard value for a 2D parameter space.
    delta_chi_squared = 4.61 
    sensitivity_threshold = chi_squared_min + delta_chi_squared

    # Find the points that are within the sensitivity region
    is_sensitive = chi_squared <= sensitivity_threshold
    sensitive_points = data[is_sensitive]

    # --- Create the Plot ---
    plt.figure(figsize=(8, 6))

    # Plot all data points with color representing chi-squared
    scatter = plt.scatter(col2, col1, c=chi_squared, cmap='viridis', s=150, zorder=2)

    # Add a color bar
    cbar = plt.colorbar(scatter)
    cbar.set_label('$\chi^2$ Value')

    # Highlight the sensitivity region with a thick red line
    if len(sensitive_points) > 0:
        plt.plot(sensitive_points[:, 1], sensitive_points[:, 0], 
                color='red', 
                linewidth=5, 
                label='90% C.L. Sensitivity Region',
                solid_capstyle='butt',
                zorder=3)

    # Add labels, title, and legend
    plt.xlabel('Column 2')
    plt.ylabel('Column 1')
    plt.title('Sensitivity Plot')
    plt.legend()
    plt.grid(True, zorder=0)

    # Ensure the y-axis has some visible range
    if len(np.unique(col1)) == 1:
        plt.ylim(col1[0] * 0.99, col1[0] * 1.01)

    # Save the plot
    plt.savefig("sensitivity_plot.png")

    print("The sensitivity plot has been saved as sensitivity_plot.png")

def chilog():
    data = np.loadtxt("chi_DUNE_3+1_theta14.dat")

    # Extract columns for plotting
    col1 = data[:, 0]
    col2 = data[:, 1]
    chi_squared = data[:, 2]

    # --- Define the Sensitivity Region (Particle Physics Convention) ---
    # Find the minimum chi-squared value (the best-fit point)
    chi_squared_min = np.min(chi_squared)

    # Define the 90% confidence level threshold for 2 degrees of freedom
    delta_chi_squared = 4.61
    sensitivity_threshold = chi_squared_min + delta_chi_squared

    # Find the points that are within the sensitivity region
    is_sensitive = chi_squared <= sensitivity_threshold
    sensitive_points = data[is_sensitive]

    # --- Create the Plot ---
    plt.figure(figsize=(8, 6))

    # Plot all data points with a logarithmic color scale for chi-squared
    # The key change is adding the norm=LogNorm() argument
    scatter = plt.scatter(col2, col1, c=chi_squared, cmap='viridis', s=150, zorder=2,
                        norm=LogNorm())

    # Add a color bar
    cbar = plt.colorbar(scatter)
    cbar.set_label(r'$\chi^2$ Value (Log Scale)') # Using a raw string for the label

    # Highlight the sensitivity region with a thick red line
    if len(sensitive_points) > 0:
        plt.plot(sensitive_points[:, 1], sensitive_points[:, 0],
                color='red',
                linewidth=5,
                label='90% C.L. Sensitivity Region',
                solid_capstyle='butt',
                zorder=3)

    # Add labels, title, and legend
    plt.xlabel('Column 2')
    plt.ylabel('Column 1')
    plt.title('Sensitivity Plot with Logarithmic Color Scale')
    plt.legend()
    plt.grid(True, zorder=0)

    # Ensure the y-axis has some visible range
    if len(np.unique(col1)) == 1:
        plt.ylim(col1[0] * 0.99, col1[0] * 1.01)

    # Save the plot
    plt.savefig("sensitivity_plot_log_scale.png")

    print("The sensitivity plot with a logarithmic color scale has been saved as sensitivity_plot_log_scale.png")

if __name__ == "__main__":
    #chi_final()
    #chiplot()
    chilog()