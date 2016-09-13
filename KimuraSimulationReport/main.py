import sys
import generate_initial_seq
import evolve_second_seq
import count_transition_transversion
import write_log
import plot_hist_k_hat
import compute_accuracy_statistics
import random
def main(argv):
    
    if len(sys.argv) != 3:
      print("Usage: Please Give initial kappa and number of replicates. (Distance will range from 0.1 to 2.5 with 0.2 step)")
      sys.exit()

    k = float(sys.argv[1])
    #d = float(sys.argv[2])
    num_replicates = int(sys.argv[2])
    seq_length = 1000
    
    random.seed(seq_length)#set the random seed to the sequence length for consistency
    
    d = 0.1
    sim_d_list = [] #list to store the simulated distances
    mean_k_hat_list = [] #list to store the estimated k for each simulation of distnace
    dev_k_hat_list = [] #list to store the deviation of kappa estimation
    experiments_log = "" #at start create an empty log to add on
    print("->> Please wait, simulation just started..")
    while (d <= 2.5):
      sim_d_list.append(d)
      #k_hat_freq = {} # dictionary to store {key -> value} -> {estimated k -> occurence times}
      k_hat_list = [] # list to store the estimated k for each iteration
      experiments_log += "Given model parameters k = " + str(k) + " and d = " + str(d) + ", with " + str(num_replicates) + " replicates\n"
    
      print("\t running simulation for " + str(d))
      for replicate in range(num_replicates): #for current distance 
	#experiments_log += "Replicate " + str(replicate) + ":\n"
	initial_seq = generate_initial_seq.generate_initial_seq(seq_length)
	#print(initial_seq)
	second_seq =  evolve_second_seq.evolve_second_seq(k,d,initial_seq)
	#print("\n\n")
	#print(second_seq)
	observed_S, observed_V = count_transition_transversion.count_transition_transversion(initial_seq, second_seq)
	d_hat, k_hat = count_transition_transversion.estimate_k_d_parameters(observed_S,observed_V)
	#experiments_log += "Sequence1: " + initial_seq + "\n\n" + "Sequence2: " + second_seq + "\n\n"
	#experiments_log += "Estimated k = " + str(k_hat) + "\t\t" + "Estimated d = " + str(d_hat) + "\n\n\n"
      
	if (float(k_hat) > 0.00): #exclude the k hat if it is equal to 0.0 (transversion)
	  k_hat_list.append(k_hat)#append the current k hat into the list
	#else:
	#  print("i found k  0.00")
      mean_k, sqrt_var, sqrt_mse = compute_accuracy_statistics.compute_accuracy_statistics(k_hat_list,k)#compute the mean of kappa and the accuracy of estimation
      experiments_log += "Estimation statistics on " + str(len(k_hat_list)) + " replicates: \n"
      experiments_log += "Mean of estimated kappa = " + mean_k + "\t\t" + "Standard deviation = " + sqrt_var + "\t" + "Square root of MSE = " + sqrt_mse + "\n\n\n"
      mean_k_hat_list.append(mean_k) #append mean kappa hat
      dev_k_hat_list.append(sqrt_var) #append deviation of kappa hat
      
      d += 0.1 #step of distance 
      #print(d)
    print("->> Simulation ended successfully!")
    write_log.write_log(experiments_log)#write simulation log into a file
      
    #plot the mean estimation of kappa as function of distance
    plot_hist_k_hat.plot_hist_k_hat(sim_d_list, mean_k_hat_list, dev_k_hat_list, k)#freq)

if __name__ == "__main__":
    main(sys.argv[1:])
