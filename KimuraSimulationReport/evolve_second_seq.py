from math import exp
import random
#==============================~==============================#
#==============================~==============================#
##Function to evolve the second sequence from the initial one.The model K80 is used with given k and d
  #Input(1): (k) given transition / transversion rate
  #Intput(2): (d) given distance of two sequences
  #Input(3): (initial_seq) given initial sequence
  #Output(1): (second_seq) the resulted evolved sequence
  
def evolve_second_seq(k,d, initial_seq):
  prob_stay, prob_transt, prob_transv = compute_evolving_probabilities(k,d)
  second_seq = ""
  
  for nucleotide_base in initial_seq:
    evolution_action = pick_character_evolution(prob_stay, prob_transt, prob_transv)
    #print(evolution_action)
    second_seq += pick_evolved_character(nucleotide_base,evolution_action)
  
  return second_seq


#==============================~==============================#
#==============================~==============================#
##Fuction to compute the evolving probabilities for a nucleotide base (based on Chapter 1 of Computational Molecular Evolution, eq 1.9)
  #Input(1): (k) given transition / transversion rate
  #Intput(2): (d) given distance of two sequences
  #Output(1): probability of staying in the same base (no change)
  #	      probability of transition
  #	      probability of transversion
def compute_evolving_probabilities(k,d):
  exponent1 = -4.0 * d/(k+2)
  exponect2 = -2.0 * d * ((k+1)/(k+2))
  #do we need optimize the number of calculations?
  #please refer to Chapter 1 of Computational Molecular Evolution for details of probabilities computation
  probability_no_change = 0.25 * ( 1.0 + exp(exponent1)) + 0.5 * exp(exponect2)
  probability_transition = 0.25 * (1.0 + exp(exponent1)) - 0.5 * exp(exponect2)
  probability_transversion = 0.25 * (1.0 - exp(exponent1))
  ##print("<~~~~~~~~~~>")
  ##print("prob no change: " + str(probability_no_change))
  ##print("prob trans: " + str(probability_transition))
  ##print("prob tranv: " + str(probability_transversion))
  ##print("<~~~~~~~~~~>")
  return probability_no_change, probability_transition, probability_transversion
  

#==============================~==============================#
#==============================~==============================#
##Function to choose character evolution using the following scheme (based on Chapter 3 of Computational Molecular Evolution, eq 1.9 and 1.10)
  # |----Transversion_1----|----Transversion_2----|------Transition------|--------No Change--------|
  #0.0			prob transv 1     	prob transv 2		prob trans		prob no change

  #Input(1): probability of no change
  #Input(2): probability of transition
  #Input(3): probability of transversion
  #Output(1): selected evolution action (transition or transversion or no change)
def pick_character_evolution(prob_no_change, prob_transt, prob_transv):
  
  random_number = random.random()
   
  if (random_number <= prob_transv):
    return "TRV1"
  elif (random_number <= 2 * prob_transv):
    return "TRV2"
  elif (random_number <= prob_transt + 2 * prob_transv):
    return "TRS"
  else:
    return "NOCH"


#==============================~==============================#
#==============================~==============================#
##Function to pick the evolved character (of second sequence) from current evolution action (transition, transversion and no change)
#Input(1): (current_character) current character of initial (first) sequence
#Input(2): (evolution_action) evolution action selected for current character
#Output(1): evolved character for the second sequence
def pick_evolved_character(current_character, evolution_action):
  
  if (evolution_action == "NOCH"): # if no change is selected just output the current character
    return current_character
  elif (evolution_action == "TRV1"): # assuming that transversion type 1 is: {"A","G"} -> {"T"} and {"T","C"} -> {"A"}
    if (current_character == "A" or current_character == "G"):
      return "T"
    else:
      return "A"
  elif (evolution_action == "TRV2"): # assuming that transversion type 2 is: {"A","G"} -> {"C"} and {"T","C"} -> {"G"}
    if (current_character == "A" or current_character == "G"):
      return "C"
    else:
      return "G"
  else: #at transition we have "A" <-> "G" and "T" <-> "C"
    if (current_character == "A"):
      return "G"
    elif (current_character == "G"):
      return "A"
    elif (current_character == "T"):
      return "C"
    else:
      return "T"
      