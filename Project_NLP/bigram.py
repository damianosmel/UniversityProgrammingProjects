import string
import os
import sys
LIST_WORDS = []
#<=========================0==============================>
#inform the user about the terminal character '.' 
def inform_term_symbol(ans) :
	print('\n')	
	if ans == 'no' :
		print('The symbol . indicates the terminal character of a sentence')
		print('So, the first row of table_N will be used to calculate the possibilities of form P(wordi|start)')
	print('\n')
#<=========================0==============================>
#declare a function to calculate the propability for a new sentence of user
	#input(1) : table with propabilities for all bigram (table_p)
	#input(2) : list of dinscinct words (list_dist_words)
	#output(1)   : none
def calculate_new_sentence(table_p,dist_word):
	#declare a variable to store the words of the new sentence
	list_word = []	
	#declare a variable to store the product of new sentence's propability
	prop_new_sentence = 1.0
	#declare a variable to store the length of sentence
	len_new_sentence = 0	
	#let user give a new sentence in order to calculate the product of it's bigram propability
	answer = input('\n Please, Give  a new sentence \t  \n> ')
	#capilatize the	characters of the given sentence
	new_sentence = answer.upper()
	#validate the user's choice
	print(' Sentence=',end=' ')
	print(new_sentence)	
	# use None to split at any whitespace regardless of length
	list_words = new_sentence.split(None)
	# append to this list the terminal character '.' not to forget calculating the propability of N(list_words|start), equivalent N(list_words|.) 
	list_words.insert(0,'.')	
	#get the length of new sentence 
	len_new_sentence = len(list_words)
	#search the table_P to find bigram of form N(list_word[i+1]|list_word[i])
	#if you find it then the corresponding propability is  P(list_word[i+1]|list_word[i]) = table_P(list_word[i+1]|list_word[i])
	#otherwise P(list_word[i+1]|list_word[i]) = 0.001	
	#and update P the new product of propabilities by type P = P' * P(list_word[i+1]|list_word[i]), where P' is the 'old' product of probalities
	#pass all pair (i,i+1) of words and update P respectively to previous concept(idea)
	for i in range(0,len_new_sentence-1):
		#print(i)		
		#try to find the word[i+1] after word[i] in the dist_word list
		try:
			#now, check if the pair of words(i,i+1) wasn't found and update the product of propabilities respectively
			current_row = dist_word.index(list_words[i])
			#print('exception in row')
			current_col = dist_word.index(list_words[i+1])
			#after pair words(i,i+1) found, multiply P with table_p[word(i+1)][word(i)]
			prop_new_sentence = prop_new_sentence * table_p[current_row][current_col]
		#except ValueError :
		except :
			#otherwise, multiply P with 0.001
			prop_new_sentence = prop_new_sentence * 0.001
			#print('hello exception')	
	#inform the user about the total product of propabilities
	print('\nThe propability of creating the given sentence is: {0:1.12f}'.format(prop_new_sentence),end='')
	print(' or {0:1.5e}'.format(prop_new_sentence))
#<=========================0==============================>	
#declare a function to calculate the propability for each bigram
	#input(1) : list of sentences (list_sent)
	#input(2) : list of dinscinct words (list_dist_words)
	#output(1) : table of all propabilities of model (table_P)
def calculate_propabilities(list_sent,list_dist_words) :
	global LIST_WORDS
	#inform the user about the status of preprocess
	print('\nStage of preprocess: Updating the table of bigram model.')
	print('Please Wait...')
	#declare a variable to store the number of distinct words
	num_dist_words = len(list_dist_words)
	#declare a variable to store the number of all words
	num_words = len(LIST_WORDS) 	
	#declare a table_N to store the occurences of all bigram including starting words (N(wordi|start), where start indicated by '.' )
	table_N = [[0 for col in range(num_dist_words)] for row in range(num_dist_words)]
	#declare a counter to print the distinct word of each row	
	j = 0
	#declare how many spaces will be used for print a distinct word and a separator
	spaces = 12
	for row in table_N:
		print(repr(list_dist_words[j]).ljust(spaces),end='')
		print(repr(row).rjust(3))	
		j += 1
	#show the representation of pointer to user
	answer=input('Are you familiar with character . representation ?  \n(yes/no) ')
	inform_term_symbol(answer)
	for i in range(0,num_words-1) :
		#find row of N table by searching the LIST_WORDS[i] in list of dinstinct words
			current_row = list_dist_words.index(LIST_WORDS[i])
		#find column of N table by searching the LIST_WORDS[i+1] in list of dinstinct words
			current_col = list_dist_words.index(LIST_WORDS[i+1])
 		#the current cell(current_row,current_col) will be updated, increasing it's value by 1,
		#to show that the endexomeno????? N(LIST_WORDS[i+1] | LIST_WORDS[i]) is found, at least one time 
			table_N[current_row][current_col] += 1
	#inform the user about ending this stage of preprocess.
	print('\nStage of preprocess: Updating the table of bigram model\t Done!')
	print('Table of N(word(column)|word(row)) follows: ')
	#clear the counter j
	j = 0	
	for row in table_N:
		print(repr(list_dist_words[j]).ljust(spaces),end='')
		print(repr(row).rjust(3))	
		j += 1
	#show the representation of pointer to user
	answer=input('Are you familiar with character . representation ?  \n(yes/no) ')
	inform_term_symbol(answer)
	#create a table_P to store the possibilities, corresponding to table_N 
	#using the type cell_P(i,j) = cell_N(i,j)/ Σ cell_N(i,0..num_dist_words) (1)
	#initialize the table P with 0.001
	table_P = [[0 for col in range(num_dist_words)] for row in range(num_dist_words)] 		
	#update all table_P's cells using the type:
	#table_P(i,j) = cell_N(i,j)/ Σ cell_N(i,0..num_dist_words) , if table_N(i,j) !=0
        #table_P(i,j) = 0.001                                      , if table_N(i,j) = 0
	#value 0.001 stands for the possibility of founding the N(LIST_WORDS[j] | LIST_WORDS[i]) is another corpus
	for i in range(0,num_dist_words) :
		sum_N = 0
		for j in range(0,num_dist_words) :
			#increase the sum of N ednexomena			
			sum_N = sum(table_N[i]) 
			#update the value of table_N[i][j], following the previous type 
			if(table_N[i][j] == 0) :
				table_P[i][j] = 0.001
			else :
				sum_N = sum(table_N[i])
				table_P[i][j] = table_N[i][j] / sum_N
			#format the cell's value to have 3 digit after comma (.) 			
			table_P[i][j] = round(table_P[i][j],3)
	print('The corresponding table_P of P(word(column)|word(row)) follows:')
	#clear the counter j
	j = 0	
	for row in table_P:
		print(repr(list_dist_words[j]).ljust(spaces),end='')
		#print(repr(row).rjust(8) 
		print(repr(row).zfill(6))	
		j += 1
	#return the table_P to find the propability of a new sentence 
	return table_P
#<=========================0==============================>
#declare a function to split the sentences into differents words
	#input(1) : list of sentences (list_sent)
	#output(1) : list of distinct words (noDupes)
def split2words(list_sent) :
	global LIST_WORDS
	#inform the user about the status of preprocess
	print('\nStage of preprocess: Getting unique words.')
	print('Please Wait...')
	#declare a list to store all the dinstinct words of model, at first it is a empty list!
	noDupes = []
	#pass the length of list with sentences to variable
	len_list = len(list_sent)
	#have a word counter,initializes assigned to 0
	count_words = 0 
	#<=========================0==============================>
	#for each sentence split it to  words.
	for i in range(0,len_list) :
		# create a list of words
		# use None to split at any whitespace regardless of length
		# so for instance double space counts as one space
		#pass to temp string the i sentence		
		temp_str = list_sent[i]		
		tempwords_list = temp_str.split(None)
		#find the lenght of tempwords list
		len_tempwords = len(tempwords_list)
		#check if the previous word differs from the current one
		#if tempwords != list_words[words-1]		
		#increase the word total count
		count_words += 1
		LIST_WORDS.append('.')
		#append to list of words with each temporary word (from tempwords_list)
		for k in range(0,len_tempwords) :			
			LIST_WORDS.append(tempwords_list[k])
	#<=========================0==============================>
	#removing duplicate words from list
	#at first, sort the list
	#get a copy of the list
	temp_word_list = list(LIST_WORDS)
	temp_word_list.sort()
	#check all the words, deleting the same ones.
	[noDupes.append(i) for i in temp_word_list if not noDupes.count(i)]
	print('\nStage of preprocess: Getting unique words\t Done!')	
	print('The dinstinct words are:\n')
	#get the lenght of noDupes list
	len_noDupes = len(noDupes)
	#inform the user about the dinstinct words
	for i in range (0,len_noDupes) : 
		if noDupes[i] != '.' :
			print()
			print(noDupes[i]) 
	print('\n')
	return noDupes
#<=========================0==============================>
#declare main function
def main() :
	global LIST_WORDS
	#declare a variable to count the given sentences by user
	count_sentence = 1
	#declare a variable to user's answer
	# initialize it with '1' to run at least the following while-loop
	answer = '1'
	#declare a list to store all the sentences of model
	#at first it is a empty list!
	list_sentences = []
	#declare a list to store all the dinstinct of model
	distinct_words = []
	#clear the screen of terminal for convenience
	os.system("clear")
	#<=========================0==============================>
	#ask user about the sentences that will be part of the model
	print('Please, Give sentences which constitute the bi-gram model!\n')
	#ask him, until she/he gives '0'
	while(answer != '0'):
		answer = input('\n Please, Give  %dth sentence \t(type 0 for quit).  \n> '% count_sentence)
		if answer != '0':		
			#capilatize the	characters 
			cap_answer = answer.upper()	
			#validate the user's choice
			print(' Sentence_%d='%count_sentence,end=' ')
			print(cap_answer)
			#append the list of sentences, with this new one
			list_sentences.append(cap_answer)
			#increase the number of sentences in model!	
			count_sentence += 1
	#<=========================0==============================>
	#examine if the user didn't type 0 at start
	if count_sentence != 1 :
		#if user eventually didn't type 0 at start
		#continue finding dinstinct words
		#<=========================0==============================>
		#clear the screen of terminal for convenience
		os.system('clear')
		#retype all given sentences by user
		print('\nThe given sentences are:')
		#print each sentence in the list
		for i in list_sentences :
			print(i)
			print('\n\n')
		#<=========================0==============================>
		#call the function split2words() for spliting the sentences into words
		distinct_words = split2words(list_sentences)
		#call the function to calculate the propabilities for each bigram
		table_prop = calculate_propabilities(list_sentences,distinct_words)
		#call a new function to find the propability for a new sentence
		calculate_new_sentence(table_prop,distinct_words)
	#if you are here user type 0 at the beggining of program or the bigram model for the new sentence was calculated, so terminate the program 
	print('\nClosing Program')
	print('Goodbye!')

if __name__ == "__main__":
    sys.exit(main())

 
