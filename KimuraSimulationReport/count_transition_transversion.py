from math import log

#==============================~==============================#
#==============================~==============================#
##Function to count the ratio of observed transitions and transversion between the two input sequences
  #Input(1): (initial_seq) first sequence
  #Input(2): (second_seq) second sequence
  #Output(1): (obs_S) observed transition ratio S
  #Output(1): (obs_V) observed transversion ratio V

def count_transition_transversion(initial_seq,second_seq):
  obs_S = 0
  obs_V = 0
  
  for character_index in range(len(initial_seq)):
    characters_first_sec = ""
    characters_first_sec += initial_seq[character_index]
    characters_first_sec += second_seq[character_index]
    if (initial_seq[character_index] == second_seq[character_index]): # if the two character in current position are equal do nothing
      continue
    else:
      #observe transition {"T"} <--> {"C"} or {"A"} <--> {"G"}
      if (characters_first_sec == "TC" or characters_first_sec == "CT" or characters_first_sec == "AG" or characters_first_sec == "GA"):
	obs_S += 1
      #now count the number of transversions {"A","G"} -> {"T"} or {"C"} and {"T","C"} -> {"A"} or {"G"}
      elif (characters_first_sec == "AT" or characters_first_sec == "AC" or characters_first_sec == "GT" or characters_first_sec == "GC" 
      or characters_first_sec == "TA" or characters_first_sec == "TG" or characters_first_sec == "CA" or characters_first_sec == "CG"):
	obs_V += 1

  
  #finally compute the S and V ratios
  obs_S = obs_S / float(len(initial_seq))
  obs_V = obs_V / float(len(initial_seq))
  return obs_S, obs_V



#==============================~==============================#
#==============================~==============================#
##Function to estimate k and d given the observed ratio S(transition) and V(transversion)
##Please also refer to eq. 1.11 of Computational Molecular Evolution
  #Input(1): (obs_S) observed transition ratio
  #Input(2): (obs_V) observed transversion ratio
  #Output(1): (k_hat) estimated k
  #Output(2): (d_hat) estimated d

#def estimate_k_d_parameters(obs_S,obs_V):
#  
#  if( 1.0 - 2.0 * obs_S - obs_V > 0 and 1.0 - 2.0 * obs_V): #????????????
#    #The log has base 10 or e??????????
#    d_hat = "{0:.2f}".format( -0.5 * log(1.0 - 2.0 * obs_S - obs_V) -0.25 * log(1.0 - 2.0 * obs_V) )
#    k_hat =  "{0:.2f}".format( 2.0 * log(1.0 - 2.0 * obs_S - obs_V) / log(1.0 - 2.0 * obs_V) - 1.0 )
#  else:
#    d_hat = -1.0
#    k_hat = -1.0
#    
#  return d_hat, k_hat

def estimate_k_d_parameters(obs_S,obs_V):
  
  if( 1.0 - 2.0 * obs_S - obs_V > 0 and 1.0 - 2.0 * obs_V > 0 and obs_V != 0.0):
    
    d_hat = "{0:.2f}".format( -0.5 * log(1.0 - 2.0 * obs_S - obs_V) -0.25 * log(1.0 - 2.0 * obs_V) )
    k_hat =  "{0:.2f}".format( 2.0 * log(1.0 - 2.0 * obs_S - obs_V) / log(1.0 - 2.0 * obs_V) - 1.0 )
  else:
    #print("obs_V is " + str(obs_V))
    d_hat = 0.0
    k_hat = 0.0
    
  return d_hat, k_hat