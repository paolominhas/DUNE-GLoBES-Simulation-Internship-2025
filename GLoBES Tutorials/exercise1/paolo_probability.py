import numpy as np
import matplotlib.pyplot as plt

def import_file():
    data = np.loadtxt('probabilidade-t2k.dat')
    return data

def plot_data(data):
    fig, axs = plt.subplots(2, 2, figsize=(12, 8))
    axs = axs.ravel()
    axes =  ['Energy (GeV)', r"$\nu_\mu$ $\rightarrow$ $\nu_\mu$ Vacuum Oscillation Probability", r"$\nu_\mu$ $\rightarrow$ $\nu_\mu$ Matter Oscillation Probability", r"$\nu_\mu$ $\rightarrow$ $\nu_e$ Vacuum Oscillation Probability", r"$\nu_\mu$ $\rightarrow$ $\nu_e$ Matter Oscillation Probability"]

    for i in range(1, 5):
        axs[i-1].plot(data[:, 0], data[:, i])
        axs[i-1].set_xlabel(axes[0])
        axs[i-1].set_ylabel(axes[i])
        axs[i-1].set_title(f'{axes[i]} vs {axes[0]}')

def dotted_plot_data(data):
    fig, axs = plt.subplots(1, 2, figsize=(12, 5))
    energy = data[:, 0]
    
    axs[0].plot(energy, data[:, 1], label='Vacuum', color=(0.2, 0.4, 0.6, 0.7))
    axs[0].plot(energy, data[:, 2], linestyle='--', label='Matter', color=(0.2, 0.4, 0.6, 0.9))
    axs[0].set_xlabel('Energy (GeV)')
    axs[0].set_ylabel(r"$\nu_\mu \rightarrow \nu_\mu$ Probability")
    axs[0].set_title(r"$\nu_\mu \rightarrow \nu_\mu$ Oscillation")
    axs[0].legend()

    axs[1].plot(energy, data[:, 3], label='Vacuum', color=(0.6, 0.4, 0.2, 0.7))
    axs[1].plot(energy, data[:, 4], linestyle='--', label='Matter', color=(0.2, 0.4, 0.6, 0.9))
    axs[1].set_xlabel('Energy (GeV)')
    axs[1].set_ylabel(r"$\nu_\mu \rightarrow \nu_e$ Probability")
    axs[1].set_title(r"$\nu_\mu \rightarrow \nu_e$ Oscillation")
    axs[1].legend()

    plt.tight_layout()
    plt.show()
    
    plt.tight_layout()
    plt.show()

def main():
    data = import_file()
    dotted_plot_data(data)
    plot_data(data)

if __name__ == "__main__":
    main()