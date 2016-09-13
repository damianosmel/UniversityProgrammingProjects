import numpy as np
import matplotlib.pyplot as plt
from array import array
#==============================~==============================#
#==============================~==============================#
##Function to plot the histogram of estimated kappa and annotate the given kappa
  #Input(1): list with all simulated distances
  #Input(2): list with mean of estimated kappas for different distances
  #Input(3): list with deviation of estimated kappa
  #Input(4): specified kappa by user
def plot_hist_k_hat(d_list, mean_k_hat_list, dev_mean_k_list, input_k):
  
  sim_dist = array('f', [])
  #transform the list to an array (compatible format for hist())
  mean_k = array('f', [])
  
  #transform the list to an array
  dev_mean_k = array('f', [])
  
  for elem_idx in range(len(d_list)): #all lists have the same length as the distance list
    sim_dist.append(float(d_list[elem_idx]))
    mean_k.append(float(mean_k_hat_list[elem_idx]))
    dev_mean_k.append(float(dev_mean_k_list[elem_idx]))
  # the histogram of the data
  

  plt.figure()
  n, bins, patches = plt.errorbar(sim_dist, mean_k, yerr = dev_mean_k)
  plt.xlabel('Simulated distances')
  plt.ylabel('Mean Estimated Kappa')
  plt.title('Kappa Estimation Over Distance')
  plt.axhline(y = input_k, xmin = 0.0, xmax = 2.5, color = 'r')#, set_label("User specified kappa"))
  #plt.axvline(x = input_k, color = 'r')
  #plt.annotate('given kappa', xy=(input_k, 0.10), xytext=(input_k + 10.0, 0.01),
  #          arrowprops=dict(facecolor='black', shrink=0.05),
  #          )
  plt.savefig('Kappa_est_over_dist.jpg')
  
  print('->> Saving mean estimated kappa over distance in K_estimation.jpg!')
  ##pos = np.arange(len(K_hat_occur.keys()))
  ##width = 1.0     # gives histogram aspect to the bar diagram

  ##ax = plt.axes()
  ##ax.set_xticks(pos + (width / 2))
  ##ax.set_xticklabels(K_hat_occur.keys())
  
  ##print(K_hat_occur.values())
  ##plt.bar(pos, array(K_hat_occur.values()), width, color = 'r')
  ##plt.show()
