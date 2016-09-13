import string
import os
#have a variable to store the number of paths in the table
COUNT_PATH = 0
#have a list to store the transcript
TRANSCRIPT = []
#<===========================0==================================>

#functions needed for the string alignment (in-exact matching).

#<===========================0==================================>
#function to compute the cost of cell(i,j).
def compute_t( r,c ) :
	#assuming that S1(i) will be replaced by S2(j).	
	t=2	
	if s1[ r ] == s2[ c ]:
		#S1[i] is equal to S2[j] so t(i,j)=0.	
		t=0
	return t


#<===========================0==================================>
#function to view tables.
def print_table( mode,ans) :
	
	if ans == 'yes' :
		if mode == 0 :
			print('\n\n')
			#fil_res.write('\n\n')
			print('Table D for edit distance : \n')
			
			print(end='\t\t       ')
			for l in range( 0,s2_len ):
				print( s2[ l ],end='  ' )
			print()

			k=0
			for row in table_D:
				if k == 0 :
					print('\t\t  ',row)
				else :	
					print('\t\t',s1[ k-1 ],row )
				k=k+1
	
			#print('\n\n')
		else :
			print('\n\n')
			print('Table for traceback  : \n')
			#print to table where backpointers were stored.
			for row in table_4_traceback:
				print('\t\t  ',row)
				
			#print('\n\n')		
	print('\n\n')

#<===========================0==================================>
#function to write tables to a txt file.
def write_table( mode,file_desc ) :
	t = 0
	if mode == 0 :
		
		file_desc.write('\n\n')
		file_desc.write('Table D for edit distance : \n\n')	
		file_desc.write('\t          ')
		
		for l in range( 0,s2_len ):
			file_desc.write( s2[ l ] )
			file_desc.write('    ')
		file_desc.write('\n')

		k=0
		for i in range(0,s1_len+1) :
			for j in range(0,s2_len+1) :
				if i == 0 and j == 0 :
					file_desc.write('\t   ')
					#write spaces according to the lenght of the cell's value
					if(table_D[i][j] <= 9):
						#write two spaces
						file_desc.write('  ')
					elif(table_D[i][j] <= 99):
						#write one space
						file_desc.write(' ')
					temp_str = str(table_D[i][j])
					file_desc.write(temp_str)
					file_desc.write('  ')
					
				elif j == 0 :	
					
					file_desc.write(s1[k-1])
					
					file_desc.write('  ')
					#write spaces according to the lenght of the cell's value
					if(table_D[i][j] <= 9):
						#write two spaces
						file_desc.write('  ')
					elif(table_D[i][j] <= 99):
						#write one space
						file_desc.write(' ')
					
					temp_str = str(table_D[i][j])
					file_desc.write(temp_str)
					file_desc.write('  ')
					
				else :
					if(table_D[i][j] <= 9):
						file_desc.write('  ')
					elif(table_D[i][j] <= 99):
						file_desc.write(' ')
					temp_str = str(table_D[i][j])
					file_desc.write(temp_str)
					file_desc.write('  ')
			file_desc.write('\n')
			file_desc.write('\t')			
			k=k+1
	
			#print('\n\n')
	else :
		file_desc.write('\n\n')
		file_desc.write('Table for traceback  : \n\n')

		
		file_desc.write('\t   ')
		#print to table where backpointers were stored.
		#for row in table_4_traceback:
		for i in range(0,s1_len+1) :
			
			for j in range(0,s2_len+1) :
				#write spaces according to the lenght of the cell's value 
				if(table_4_traceback[i][j] <= 2):
					#write two spaces
					file_desc.write('  ')
				elif(table_4_traceback[i][j] <= 20):
					#write one space
					file_desc.write(' ')	
					
				temp_str = str(table_4_traceback[i][j])
				file_desc.write(temp_str)
				file_desc.write('  ')
							
			file_desc.write('\n')
			file_desc.write('\t   ')
		file_desc.write('\t\t  ')
			
		file_desc.write('\n')
				
	file_desc.write('\n\n')
#<===========================0==================================>
#function to show representation of backpointers.

def print_representation( ans ) :
	#inform the user about the used representation.
	if ans == 'no' :
		print('\n\n')
		print('#===============================================================#')
		print('#	#update respectively the back pointers			#')
		print('#	#using the following respresantation:			#')
		print('#								#')
		print('#	#only one pointer :					#')
		print('#	# |      = 1 						#')
		print('#	# <- 	 = 2						#')
		print('#	# \  	 = 0						#')
		print('#	#now combine them to get double pointers :		#')
		print('#	# |  <-  = 12						#')
		print('#	# |  \   = 10						#')
		print('#	# <- \   = 20						#')
		print('#	#finally combine the double to get the triple one :	#')
		print('#	# | <- \ = 120						#')
		print('#								#')
		print('#===============================================================#')
	print('\n\n')

#<===========================0==================================>
#function to write representation of backpointers to txt file.

def write_representation( file_desc ) :
	#inform the user about the used representation.
	
	file_desc.write('\n\n')
	file_desc.write('\tBackpointers Representation: \n')
	file_desc.write('#===========================================================#\n')
	file_desc.write('#   #update respectively the back pointers		    #\n')
	file_desc.write('#   #using the following respresantation:		    #\n')
	file_desc.write('#							    #\n')
	file_desc.write('#	only one pointer :				    #\n')
	file_desc.write('#	 |       = 1 				            #\n')
	file_desc.write('#	 <- 	 = 2					    #\n')
	file_desc.write('#	 \  	 = 0					    #\n')
	file_desc.write('#	now combine them to get double pointers :	    #\n')
	file_desc.write('#	 |  <-  = 12					    #\n')
	file_desc.write('#	 |  \   = 10					    #\n')
	file_desc.write('#	 <- \   = 20					    #\n')
	file_desc.write('#	finally combine the double to get the triple one :  #\n')
	file_desc.write('#	 | <- \ = 120					    #\n')
	file_desc.write('#							    #\n')
	file_desc.write('#===========================================================#\n')
	file_desc.write('\n\n')

#<===========================0==================================>
#function to fill the table D and the table with backpointers.

def update_cell (row,col) :
	
	if row > 0 and col > 0 :

		#first find the cost to continue from diagonially. 
		equal_or_not = compute_t(row-1,col-1)
		cost_diag = table_D[row-1][col-1] + equal_or_not
		
		#find the cost going vertically.
		#cost for deletion of S1(i) is 1.
		cost_vert = table_D[row-1][col] + 1

		#find the cost going horizontially.
		#cost for insertion of S2(j) is 1.
		cost_hor = table_D[row][col-1] + 1

		#find the minimum of the 3 costs
		minimum_cost = min( cost_vert,cost_diag,cost_hor )
		#and pass it to cell (i,j).
		table_D[row][col] = minimum_cost

	#===============================================================#
	#	#update respectively the back pointers			#
	#	#using the following respresantation:			#
	#								#
	#	#only one pointer					#
	#	# |      = 1 						#
	#	# <- 	 = 2						#
	#	# \  	 = 0						#
	#	#now combine them to get double pointers		#
	#	# |  <-  = 12						#
	#	# |  \   = 10						#
	#	# <- \   = 20						#
	#	#finally combine the double to get the triple one	#
	#	# | <- \ = 120						#
	#								#
	#===============================================================#								
		
		#creating pointers for each cell.
		if( minimum_cost == cost_vert ) :
			
			#make a pointer to the cell up. 			
			table_4_traceback[row][col] = table_4_traceback[row][col] + 1		
		
		if( minimum_cost == cost_hor) :
			
			#make a pointer to the cell right.
			table_4_traceback[row][col] = table_4_traceback[row][col] + 2
			#correct two possible values so as to correspond to above respresentation
			# when cost_vert = cost_hor=min
			if( table_4_traceback[row][col] == 3):
				table_4_traceback[row][col] = table_4_traceback[row][col] * 4 
	
		if (minimum_cost == cost_diag ) :
			
			#make a pointer to cell diagon.
			table_4_traceback[row][col] = table_4_traceback[row][col] * 10

		
	elif row == 0 :
		#apply base conditions for row 0.
		table_D[row][col] = col
		
		#update table with backpointers
		if col != 0:
			#cell with row 0 and colum >0 point vertically to (0,0)
			#cell (0,0) hasn't any pointer.
			table_4_traceback[row][col] = 2 
	
	else :
		#apply base conditions for column 0 
		table_D[row][col] = row
		
		#update table with backpointers
		#cell with row 0 point horizontially to (0,0)
		table_4_traceback[row][col] = 1 
				
#<===========================0==================================>
#function for backtracking. 

def go_backtrack(inter_i,inter_j,cur_i,cur_j,trac,trac_backup,file_desc) :

	
	row = cur_i
	col = cur_j
	
	#going back according to pointers until you find the cell (0,0)
	#while row != 0 or col != 0 :
	while row + col != 0 :
		#check if there is only one pointer
		if table_4_traceback[row][col] <= 2 :
			
			#if it is true follow the pointer!
			#first if it points diagonial
			if table_4_traceback[row][col] == 0 :
				row = row - 1
				col = col - 1
				#examine if there was a match or unmatch
				if compute_t(row,col) == 0 :			
					#write m for match in transcript					
					trac.append('m')
				else :	
					#write r for replace respectively
					trac.append('r')				
			
			#second if it points vertical 
			elif table_4_traceback[row][col] == 1:
				
				row = row - 1
				#write d into transcript for deletion.
				trac.append('d')
			#finally if it points horizontial 			
			else :
				
				col = col - 1
				#write i into transcript for insertion.
				trac.append('i')
			

		#if there are double pointer choose one of them randomly and go on
		elif table_4_traceback[row][col] <= 20 :

			#inform the user that a double pointer has just found
			print('Double intersection found in (',row,end=' , ')
			print(col,end=' ).\n\n')
			
			#mark this cell as an intersection
			intersection_i = row
			intersection_j = col
			#store the current path to trac_backup in order to have start traversal to cell (0,0) from this intersection.
			trac_backup = list(trac)

			#as the representation of double pointer is 10 or 12 or 20
			#take the div(representation,10) to get the first one!			
			first_pointer = table_4_traceback[row][col] // 10
			# change the double pointer to one			
			table_4_traceback[row][col] = table_4_traceback[row][col]-(first_pointer*10)
			
			#now follow the first pointer going back			
			if first_pointer == 1 :
				row = row - 1
				#write d into transcript for deletion 
				trac.append('d')
			else :
				
				col= col - 1
				#write i into transcript for insertion
				trac.append('i')	
			
			#update respectively the two current positions of i and j
			cur_i = row
			cur_j = col		
						
			#now call recrusively the function to go back
			go_backtrack(intersection_i,intersection_j,cur_i,cur_j,trac,trac_backup,file_desc)
			print('\n\nAfter double intersection...\n')
			
			#it's time to continue the path with the other pointer
			row = intersection_i 
			col = intersection_j 

			#now the trace is the path to this intersection before following the first pointer (first_pointer)
			trac = list(trac_backup)	
			
			#skip this cycle of loop, for going back with a new path
			continue
	
		
		#if there is a triple pointer choose one of them randomly and go on		
		else :
			#inform the user that a triple pointer has just found
			print('Triple intersection found in (',row,end=' , ')
			print(col,end=' ).\n')
			
			#mark this cell as an intersection
			intersection_i = row
			intersection_j = col
			#store the current path to trac_backup in order to have start traversal to cell (0,0) from this intersection.
			trac_backup = list(trac)
			
			#as the only representation of triple pointer is  120
			#get the 1 to go up or div(120,100) 
			first_pointer = table_4_traceback[row][col] // 100
			table_4_traceback[row][col] = table_4_traceback[row][col] - (first_pointer * 100)
			
			#now follow the first pointer(first_pointer) going back
			#peritto if 			
			if first_pointer == 1 :
				row = row - 1
				#write d into transcript for deletion 
				trac.append('d')
			
			#update respectively the two current positions of i and j
			cur_i = row
			cur_j = col

			#now call recrusively the function to go back
			go_backtrack(intersection_i,intersection_j,cur_i,cur_j,trac,trac_backup,file_desc)
			print('\n\nAfter  triple intersection...\n')

			#it's time to continue the path with the other pointer
			row = intersection_i 
			col = intersection_j 
			
			#now the trace is the path to this intersection before following the first pointer (first_pointer)
			trac = list(trac_backup)
			
			#skip this cycle of loop, for going back with a new path
			continue			
	
	print_transcript(trac,file_desc)
	
	
#<===========================0==================================>
#function for printing a edit transcript.

def print_transcript(trans,file_desc):

	#use the global variable COUNT_PATH
	global COUNT_PATH

	#use one index for each string	
	s1_iter = 0
	s2_iter = 0

	#declare to lists to store the edited strings according to given transcript. 
	l1_temp = []
	l2_temp = []
	
	#reverse the path as you have been going backwards  		
	path = trans[::-1]
	
	#increase the number of founded transcripts
	COUNT_PATH = COUNT_PATH + 1

	#inform the user about the transcript
	print('#===============================================================#\n')
	print('',COUNT_PATH,end='')
	print(' Transcript is :',path)
	print('\n')	
	
	#write the above information in the file
	file_desc.write('\n#===============================================================#\n')
	count_str = str(COUNT_PATH)
	file_desc.write(count_str)
	file_desc.write(' Transcript is :')
	path_str = str(path)
	file_desc.write(path_str)
	file_desc.write('\n\n') 
	#get the size of transcript	
	path_len = len(path)
	
	#for each character in path edit the two strings.
	for k in range(0,path_len) :
		
		if path[k] == 'i' :
			#i stands for insertion
			#write an asterisk to l1 and l2
			l1_temp.append('*')
			
			l2_temp.append('*')
			#increase the s2's index
			s2_iter = s2_iter + 1
			
		elif path[k] == 'd' :
			#d stands for deletion
			##write an asterisk to l1 and l2
			l2_temp.append('*')
			
			l1_temp.append('*')
			#increase the s1's index
			s1_iter = s1_iter+ 1

		elif path[k] == 'm' :
			#in match write the responded characters of each s1 and s2 to l1 and l2 respectively.
			l1_temp.append(s1[s1_iter])
			l2_temp.append(s2[s2_iter])
			#increase both s1's and s2's indeces
			s2_iter = s2_iter + 1
			s1_iter = s1_iter + 1
		
		else :
			#in replace write asterisks to both l1 and l2.
			l1_temp.append('*')
			l2_temp.append('*')
			#increase both s1's and s2's indeces
			s2_iter = s2_iter + 1
			s1_iter = s1_iter + 1

		
	print('The ',COUNT_PATH,end='')
	print(' Form of the Longest Common Subsequence is ')
	print(' S1:',l1_temp)
	print(' S2:',l2_temp)
	#print(' \tis',l1_temp)
	print('\n')
	print('#===============================================================#')
	
	#write those information into the file
	file_desc.write('The ')
	
	#use the above cast of integer COUNT_PATH to string	
	file_desc.write(count_str)
	file_desc.write(' Form of the Longest Common Subsequence is \n')
	
	file_desc.write(' S1:')
	l1_str = str(l1_temp)
	file_desc.write(l1_str)
	file_desc.write('\n')

	file_desc.write(' S2:')
	l2_str = str(l2_temp)
	file_desc.write(l2_str)
	file_desc.write('\n')

	file_desc.write('\n')
	file_desc.write('#===============================================================#\n')


#<==========================0========================>#
#=============Longest Common Subsequence==============#

#<==========================0========================>#
#initialize two variables for the while loop

#declare user_choice variable to run the program at least one time
user_choice = 'yes'

#declare first_time variable to know if it is the first time of loop execution
first_time = 1

#<==========================0========================>#
#======================while loop=====================#
#go on until user doesn't want to continue giving sequences... 
while( user_choice != 'no' ) :
	#clear the screen of terminal for convenience
	os.system("clear")
	#as start the operation make COUNT_PATH equal to 0	
	COUNT_PATH = 0
	#<==========================0========================>#
	#==Get the two user's choices for the two Sequences===#

	#read the user's choice for the first sequence.
	print('\n\n')
	s = input('Please,enter the first sequence: ')

	#remove all newline in right of s
	s1 = s.rstrip("\n")

	#input's validation
	s1_len=len(s1)
	print( 'S1 =',s1.upper(),'   n=',s1_len )

	s1 = s1.upper()
	#read the user's choice for the second sequence.
	print('\n\n')
	s = input('Please,enter the second sequence: ')

	#remove all newline in right of s
	s2 = s.rstrip("\n")

	#input's validation
	s2_len=len(s2)
	print( 'S2 =',s2.upper(),'   m=',s2_len )
	s2 = s2.upper()
	#<==========================0========================>#
	#open a file to write the results of lcs

	fil_res = open('lcs_results.txt','w')
	#in the first execution the txt will be created 	
	if first_time == 1 :
		print('\nThe lcs_result.txt was created successfully!\n') 
	else :
		#the next times the file will be modified
		#so warn the user about lossing his data
		print('\nWarning: The lcs_result.txt will be modified!\n')
	
	fil_res.write('#<===============================0=============================>#\n')
	fil_res.write('#==================Longest Common Subsequence===================#\n\n\n')
	fil_res.write('S1 = ')
	fil_res.write(s1)
	fil_res.write('\n')
	fil_res.write('S2 = ')
	fil_res.write(s2)
	fil_res.write('\n')

	#<==========================0========================>#
	#==Initialize edit distance table & backpointer table=#

	#create the table to store all the D(i,j)
	table_D = [[0 for col in range(s2_len+1)] for row in range(s1_len+1)]	

	#inform user that the table was created
	print('\nTable D was created successfully!')

	#create a table to store backpointers.
	table_4_traceback = [[0 for col in range(s2_len+1)] for row in range(s1_len+1)]

	#inform user that the table was created
	print('\nTable with backpointers was created successfully!')

	#<==========================0========================>#
	#=======Pass appropriate values to two tables=========#

	#inform user about the status of processing
	print('\nNow filling the two tables')
	print('Please wait...')

	#now, fill the table by completing row.(legetai etsi na sumplhrwnw oli thn grammh kai meta na phgainw sthn epomeni????)
	for i in range(0,s1_len+1) :
		for j in range(0,s2_len+1) :
			#fill the table with edit distance values and simultaneously the table with backpointers	
			update_cell(i,j)

	#<==========================0========================>#
	#===================printing tables===================#


	#inform user that the filling of table D was done
	print('\nTable with edit distance values has just been computed.')
	answer = input('Do you want to print it ? \n(yes/no) ')
	#print the updated table D
	#write to this file the table D
	print_table(0,answer)


	#write to this file the table D
	write_table(0,fil_res)


	#inform user that the filling of the table with traceback pointers was done
	print('\nTable with traceback pointers has just been computed.')
	answer = input('Do you want to print it ? \n(yes/no) ')
	#print the updated table of backpointers
	print_table(1,answer)

	#write the table with backpointers to the txt file.
	write_table(1,fil_res)

	#if user wanted to see the backpointer table show him the representation (if it is needed)
	if(answer == 'yes'):
		#show the representation of pointer to user
		answer=input('Are you familiar with this backpointers representation ?  \n(yes/no) ')
		print_representation(answer)
	else :
		print('\n\n')

	#write the representation of backpointers to the file
	write_representation(fil_res)

	#<==========================0========================>#
	#==declare some useful variables for backtracking=====#

	#two list to store the reversed transcript
	traces = []
	traces_backup = traces
	#declare two dimension intersection point of the table
	#at start this point is the cell(n,m) of the table.
	intersection_i=s1_len
	intersection_j=s2_len

	#<==========================0========================>#
	#===========call backtracking function================#
	#inform the user that the backtracking is starting
	print('Tables computed successfully, now going backwards...')
	print('Please wait...\n')
	#at start the intersection i,j are equal to their current equivalent
	go_backtrack(intersection_i,intersection_j,intersection_i,intersection_j,traces,traces_backup,fil_res)

	#<==========================0========================>#
	#close file and inform the user about the saved result
	fil_res.write('\n#<==============================END============================>#\n')
	fil_res.write('#<===============================0=============================>#')
	fil_res.close()
	print('Results of operation stored in lcs_result.txt .\n')
	
	#<==========================0========================>#	
	#ask user if he likes to do an another test
	#pass his will to the user_choice variable
	user_choice = input('Do you want to continue ?\n (yes/no) ') 
	
	#<==========================0========================>#	
	#as you end of a loop first time of loop's execution is done
	first_time = 0	

#<==========================0========================>#
#before close program say goodbye to user.
print('\nClosing Program...')
print('Please wait...')
print('\nGoodbye!')
