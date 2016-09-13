import random

#==============================~==============================#
#==============================~==============================#
##Function to generate the initial sequence of input length
  #Input(1): (seq_length) desired sequence length
  #Output(2): (initial_seq) sequence generated assuming stationary distribution for the 4 nucleotide bases
  
def generate_initial_seq(seq_length):

  initial_seq = ""
  
  #for each nucleotide of the sequence draw a random number from [0,1) uniformly
  # |----A----|----C----|----G----|----T----|
  #0.0       0.25      0.5       0.75      1.0
  for seq_index in range(seq_length):
    random_nucl_base = pick_nucl_base_equal()
    
    initial_seq += random_nucl_base
  
  return initial_seq

#==============================~==============================#
#==============================~==============================#    
#Function to get a nucleotide based on equal probability (0.25) for each  nucleotide base

  #Output(1): Randomly selected nucleotide
  
def pick_nucl_base_equal():
   
   random_number = random.random()
   
   if (random_number <= 0.25):
     return "A"
   elif (random_number <= 0.5):
     return "C"
   elif (random_number <= 0.75):
     return "G"
   else:
     return "T"