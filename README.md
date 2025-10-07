# DUNE-GLoBES-Simulation-Internship-2025 🔬
## DUNE Sterile Neutrino Sensitivity Analysis 🔬

This repository contains the C code and analysis scripts developed during Career Develepmont Summer Programme at the University of Edinburgh mainly in May, June and July but with some work till September. The project aims to forecast the sensitivity of the Deep Underground Neutrino Experiment (DUNE) to the oscillation parameters of a 3+1 sterile neutrino model. The simulations are performed using the **GLoBES (General Long Baseline Experiment Simulator)** library.

---

## Abstract

The Standard Model describes neutrino oscillations with three massive neutrino states. However, some experimental anomalies (such as that at LSND - the `reactor anomaly') hint at the possible existence of additional, non-weakly-interacting neutrinos, known as "sterile" neutrinos. In this project, we investigate the potential of the DUNE experiment to probe a simple extension of the Standard Model with one sterile neutrino (the 3+1 model). By simulating the propagation of neutrinos from LBNF at Fermilab (near Chicago) to the Sanford Underground Research Facility (Arizona), as the final part to this project we calculate the sensitivity of DUNE to the sterile mixing angle $\theta_{24}$ and the new mass-squared splitting $\Delta m^2_{41}$. Read the [report](./Report.pdf) for more information.

---

## Key Features

-   **DUNE Simulation:** Implements the DUNE experimental setup, including baseline, detector mass, and beam power configurations.
-   **Sterile Neutrino Model (3+1):** Uses C code to define the oscillation probabilities in a four-neutrino framework.
-   **Sensitivity Calculation:** Leverages the GLoBES library to perform a $\chi^2$ statistical analysis.
-   **Parameter Scans:** Includes Makefiles and scripts to perform scans over sterile neutrino oscillation parameters.

---

## Prerequisites

Before you can compile and run this code, you will need the following installed on your system:

1.  **A C Compiler:** A standard C compiler like `gcc` or `clang`.
2.  **Make:** The `make` build automation tool.
3.  **GLoBES Library:** This code is entirely dependent on GLoBES.
    -   You must have it installed and compiled.
    -   Ensure that the environment variables (e.g., `$GLB_DIR`) are set correctly in your shell configuration (`.bashrc`, `.zshrc`, etc.).
    -   Official website for installation: [GLoBES Homepage](https://www.mpi-hd.mpg.de/personalhomes/globes/)
4.  **Plotting Software (Optional):** To visualize the results, you'll need a tool like **gnuplot**, **ROOT**, **Mathematic** or **Python** (with Matplotlib/NumPy which is what is used in this projects).

---

## Installation & Setup

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/paolominhas/DUNE-GLoBES-Simulation-Internship.git](https://github.com/paolominhas/DUNE-GLoBES-Simulation-Internship.git)
    cd your-repository-name
    ```

2.  **Verify GLoBES Installation:**
    Confirm that your GLoBES installation is working and that the necessary environment variables are loaded into your current shell session.

---

## Usage 🚀

The project is organized into directories, each corresponding to a specific parameter scan. To run a simulation:

1.  **Navigate to a simulation directory:**
    ```bash
    # Example for scanning the theta_34 mixing angle
    cd ./simulation_theta34/
    ```

2.  **Compile the code:**
    The Makefile is configured to find your GLoBES installation and link against the required libraries.
    ```bash
    make
    ```
    This will create an executable file (e.g., `run_scan`).

3.  **Run the executable:**
    ```bash
    ./run_scan
    ```

4.  **Check the Output:**
    The program will generate `.dat` or `.txt` files in the `../results/` directory. These files contain the $\chi^2$ values for the parameter points scanned.

---

## Project Structure

```
├── README.md                    # This file
├── Poster.pdf                   # Final poster 
├── Report.pdf                   # Final report
├── Examples/                    # GLoBES provided examples
├── GLoBES Project/              # Directory for all work done in project
│   ├── adding-near-detector/     # Adding the ND to final sims
│   ├── initial-project           # The project without the sterile neutrino
│   ├── sterile-neutrino          # Sterile at the FD only
│   └── NotesOnGLoBESCourse.pdf   # Rough notes taken in the course
├── Final Result/.               # Directory for the sterile neutrino scan
│   ├── NDSterile.c               # Main file
│   ├── DUNE_ND.glb               # Near detector GLoBES file
│   ├── DUNE_GLoBES.glb           # Far detector GLoBES file
│   ├── dune_proj_3+1-2.c         # Gabriela's final file
│   ├── plots.py                  # The python plotting file
│   ├── readme.md                 # The project readme.md
│   └── Makefile
├── GLoBES Tutorials/            # Gabriela's course on GLoBES
└── Not for GitHub/              # Should be removed if I did the .gitignore correctly
```

---

## Results

The output data is contained in the [report](./Report.pdf). A current silly limitation of this repository is that I didnt save the plots so in the future hopefully htis will be amended.

![Example sensitivity plot showing the 90% C.L. exclusion region for sterile neutrino mixing at the far detector only](./GLoBES%20Project/ProjectPhoto.png)

---

## Acknowledgments

 I would like to thank Dr Gabriela Vitti Stenico, my supervisor, for her help and guidance with this project, and for providing me with the resources to do this project. I would also like to thank Dr Miquel Nebot-Guinot, Dr Cheryl Patrick and Dr Pedro Pasquini for their further assistance on the DUNE project and the physics behind it, as well as supervising a previous project on DUNE that greatly helped me with this one. Thank you to the School of Physics and Astronomy at the University of Edinburgh for the funding this project via the Career Development Summer Scholarships, and the DUNE collaboration.


---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details