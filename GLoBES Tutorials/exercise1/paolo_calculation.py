import numpy as np
from paolo_probability import dotted_plot_data, plot_data

def calculate_oscillation_probabilities():
    theta12 = np.arcsin(np.sqrt(0.8)) / 2
    theta13 = np.arcsin(np.sqrt(0.001)) / 2
    theta23 = np.pi / 4
    deltacp = np.pi / 2
    sdm = 7e-5
    ldm = 2e-3
    A = 1.0

    E = np.arange(0.01, 1.20, 0.01)
    mumu_vacuum = (np.sin(2 * theta23)**2 * np.sin(deltacp) * np.sin(sdm * E)
    mumu_matter = np.sin(2 * theta23) * np.cos(deltacp) * np.sin(ldm * E)
    nue_vacuum = np.sin(2 * theta12) * np.sin(deltacp) * np.sin(sdm * E)
    nue_matter = np.sin(2 * theta12) * np.cos(deltacp) * np.sin(ldm * E)
    data = np.column_stack((E, mumu_vacuum, mumu_matter, nue_vacuum, nue_matter))
    return data

def main():
    data = calculate_oscillation_probabilities()
    dotted_plot_data(data)
    plot_data(data)

if __name__ == "__main__":
    main()