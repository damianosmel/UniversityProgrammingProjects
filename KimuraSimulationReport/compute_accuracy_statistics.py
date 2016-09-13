from math import pow,sqrt

#==============================~==============================#
#==============================~==============================#
##Function to compute square root of variance and mean square error
  #Input(1): list with the estimated k
  #Input(2): given k
  #Output(1): mean kappa hat
  #Output(2): square root of variance (deviation)
  #Output(3): square root of mean square error

def compute_accuracy_statistics(estimated_k_list, true_k):

  num_samples = len(estimated_k_list)
  for i in range(num_samples):
    estimated_k_list[i] = float(estimated_k_list[i])
  
  sum_k = 0.0 #firstly compute the mean of estimated k
  
  for estimated_k in estimated_k_list:
    #estimated_k = float(estimated_k) #change the type of each list element to float
    sum_k += estimated_k#float(estimated_k)
    
  print(num_samples)
  mean_k = sum_k / num_samples
  
  sum_sq_diff = 0.0
  
  for estimated_k in estimated_k_list:
    sum_sq_diff += pow(estimated_k - mean_k, 2.0)#float(estimated_k) - mean_k, 2.0)
  
  var_k = sum_sq_diff / (num_samples - 1) #divided by n - 1 lose one degree of freedom
  #compute the mean square error by sum_i( (k_hat_i - k_input)^2 ), where kappa input is the "true" kappa
  sum_sq_diff = 0.0
  for estimated_k in estimated_k_list:
    sum_sq_diff += pow(estimated_k - true_k, 2.0)#float(estimated_k) - true_k, 2.0)
  
  mse_k = sum_sq_diff / num_samples
  
  
  
  return "{0:.2f}".format(mean_k), "{0:.2f}".format(sqrt(var_k)), "{0:.2f}".format(sqrt(mse_k))