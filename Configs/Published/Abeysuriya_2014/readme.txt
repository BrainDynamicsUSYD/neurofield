The configuration files provided here generate the data used in Abeysuriya, R. G., Rennie, C. J., & Robinson, P. A. (2014). Prediction and verification of nonlinear sleep spindle harmonic oscillations. Journal of Theoretical Biology, 344, 70–77. doi:10.1016/j.jtbi.2013.11.013

The model spectrum can be generated using

nf = nf_run('figure_1.conf');
[f,P] = nf_spatial_spectrum(nf,[],[],32,1);
loglog(f,P);

The analytic spectrum can be generated using the formulas provided in the paper. The model parameters are stored in parameters.mat as objects using the classes defined in the Matlab_structures folder of the repository. These classes contain interfaces to generate the analytic spectrum shown in the figures, and the relevant code is also included in the Matlab_structures directory (in dispersion_calc.m). However, this code has not been prepared for general use yet and is essentially undocumented.

The remaining figures in the paper use the same model parameters, with with different noise powers. These can be set by changing the Psd quantity of the stimulus in the configuration file to match the quantity in the paper. For example, Figure 2 is generated using mu = 1e-4, and figure_2.conf has "Psd: 0.0001". To generate Figure 5, simply change Psd to each of the specified mu values in the figure caption. 
