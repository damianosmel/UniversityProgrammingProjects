#==============================~==============================#
#==============================~==============================#
##Function to write the simulation log into a file
  #Input(1): (simulation_log) log text of simulation
  ##Input(2): given kappa
  ##Input(3): given distance
  #Output(1): 0 successful write, -1: IO error

def write_log(simulation_log):
  try:
    #file_name = "Kimura_Simulation_" + str(k) + "_" + str(d) + ".txt"
    f = open("Kimura_Simulation.txt","w")
    try:
      f.write(simulation_log)
    finally:
      f.close()
  except IOError:
    pass
    print("IOError: Simulation log file could not be created")
    return -1
  
  print("->> Simulation log was saved in Kimura_Simulation.txt!")	
  return 0