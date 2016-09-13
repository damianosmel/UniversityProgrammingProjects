import string
import os
import sys
import math
import sys
ID_NODE = -1
#<=========================0==============================>
#declare a class of tree's nodes
class TreeNode():

	#function for initiliazition of node
	#input(1): treenode instance
	#input(2): node's character
	#input(3): next letter of node's character in  the word 
	def __init__(self, node_character,next_letter):
		global ID_NODE	
		#define the node by a id		
		self.id = ID_NODE
		#set the its' letter occurences to 1
		self.letter_occurences = 1
		#set the character of the node
		self.character = node_character
		#define a empty dictionary of node's children
		self.children = {}
		#create a link: node child -> child's letter occurences 
		if next_letter != '':
			temp_id = self.id + 1			
			self.children[temp_id] = 1

#<=========================0==============================>
#declare a class of trees (trie)
class Tree():

	#<=========================0==============================>
	#function of initiliazition of tree
	#input(1): tree instance 
	def __init__(self):
		#declare the root of tree as none
		self.root = None
		#define a empty dictionary to store node_id -> node
		self.tree_dict={}
	
	#<=========================0==============================>
	#function of adding a new node to tree
	#input(1): tree instance
	#input(2): node's character
	#input(3): next letter of character in the word
	#output(1): new node	
	def add_node(self,character_data,next_letter):
		global ID_NODE
		#increase the global node id
		ID_NODE += 1
		#create the new node
		new_node = TreeNode(character_data,next_letter)
		#update the dictionary of nodes with the new one		
		self.tree_dict[ID_NODE]= new_node		
		return new_node

	#<=========================0==============================>
	#function to insert data to the tree
	#input(1): tree instance
	#input(2): word to insert
	#input(3): tree's root
	def insert_data(self,word,root):
		global ID_NODE
		#get the current node to go on insertion
		current_node = root
		#get the lenght of the word
		len_word= len(word)
		#for each character in the word
		for i in range(len_word):
			#at start assume that you didn't find the word's letter
			letter_not_found = True
			temp_node =current_node
			#examine the node's character and the current word's character
			#find the children of root with the same letter as the first one of word
			try:
				for id_node in current_node.children:
					temp_node = self.tree_dict[id_node]			
					if  temp_node.character == word[i]:
						#as now you find same character update the letter occurences of node
						current_node.children[id_node] += 1
						temp_node.letter_occurences += 1
						#set the flag to false to show that word's letter was found!
						letter_not_found = False
						#change the current node to the temporary one
						current_node = temp_node					
						#stop to examine the children of current node						
						break
			except KeyError:
				#if current node doesn't have children set the flag of letter founding to true
				letter_not_found = True
			#examine the flag
			if letter_not_found:			
				#as you are here the two characters are unequal
				#so add a new node with the word's character
				#check if there the current character is the last one
				if i==len_word-1:
					#if the character of word is the last one then the node is a leaf
					new_node = self.add_node(word[i],'')
				else:
					new_node = self.add_node(word[i],word[i+1])
				#update the current node's children list with the new node
				current_node.children[ID_NODE] = 1
				#now the current node is the new one
				current_node = new_node

	#<=========================0==============================>
	#function to print the data of each node
	#input(1): tree instance
	#input(2): tree root 	
	def print_data(self,root):
		#for each node of tree print its' id, character, letter occurences and the node children 
		for node_id in self.tree_dict:
			print('id= ',node_id)
			temp_node = self.tree_dict[node_id]
			#print(self.tree_dict[node_id])
			print('char= ',temp_node.character)
			print('#occur= ',temp_node.letter_occurences)
			print('children are',temp_node.children)

	#<=========================0==============================>
	#function  to find the 'best' suffix of given prefix (best: according to bayesian model)
	#input(1): tree instance
	#input(2): given prefix 
	#input(3): tree root
	#output(1): best suffix or -1 (failure)
	def find_suffix(self,prefix,root,correct_words_list):
			#at start iniliaze the suffix to be equal to the given prefix			
			suffix= prefix
			#set the current to the tree's root
			current_node = root
			#get the lenght of the given prefix
			len_prefix= len(prefix)
			#for each character in the word
			for i in range(len_prefix):
				#at start assume that you didn't find the word's letter
				letter_found = False
				#define a temporary node
				temp_node =current_node
				#examine the node's character and the current word's character
				#find the children of root with the same letter as the first one of word
				try:
					for id_node in current_node.children:
						#as now you find same character update the letter occurences of node
						temp_node = self.tree_dict[id_node]			
						if  temp_node.character == prefix[i]:
							#set the flag to true to show that the character was found!
							letter_found = True
							#set the current node to the temporary one
							current_node = temp_node					
							#stop to search for the specific prefix's letter							
							break
				except KeyError:
					#if the node doesn't have children set the flag to false
					letter_found = False
			#<=========================0==============================>
			#now check if the whole prefix was founded in the tree		
			if letter_found:
				#use a heuristic to assume the maximum lenght of the suffix				
				count=5 #0,694
				#count= 4 0,64
				#fortunately you find the given prefix, so go on an suggest the next letter according to the letters' occurences
				#traverse the tree from the current node untill you find a leaf or the lenght of the suffix is greater than the heuristic maximum lenght (equal to 5)			
				while(temp_node.children != {} and count>0):# and correct_words_list.count(suffix)== 0 ):
					#find the children of current node with maximum letter occurences
					next_node_id = max(temp_node.children,key = lambda node:temp_node.children.get(node))
					#get the next node from the tree dictionary
					next_node = self.tree_dict[next_node_id]
					#get the character of the next node and concatenate it with the previous suffix
					suffix = suffix + next_node.character
					#go on to the children of next node				
					temp_node = next_node
					count -=1
				#return the prefix concatenated with the founded suffix				
				return suffix

			else:
				#if the whole prefix wasn't founded return -1 indicating failure
				return -1

	#<=========================0==============================>
	#function to find the 'best' next letter of the given prefix (best: according to bayesian model)
	#input(1): tree instance
	#input(2): prefix to find
	#input(3): starting node index
	#output(1): prefix concatenated with the 'best' next letter or the given prefix (failure)	
	def find_prefix(self,prefix,start_index):
		#get the current node to start searching
		current_node = self.tree_dict[start_index]
		#get the lenght of the given prefix
		len_prefix= len(prefix)
		#for each character in the word
		for i in range(len_prefix):
			#at start assume that you didn't find the word's letter
			letter_found = False
			#get a temporary node
			temp_node =current_node
			#examine the node's character and the current word's character
			#find the children of root with the same letter as the first one of word
			try:
				for id_node in current_node.children:
					temp_node = self.tree_dict[id_node]			
					if  temp_node.character == prefix[i]:
						#set the flag to true to show that the specific prefix's letter was found						
						letter_found = True
						#change the current node to the temporary one
						current_node = temp_node					
						#stop searching the prefix letter						
						break
			
			except KeyError:
				#if the current node doesn't have children set the flag to false
				letter_found = False
		#examine if the given prefix is founded		
		if letter_found:
			#examine if the last node of search has children
			if(temp_node.children != {}):
				#find the children of current node with maximum letter occurences
				next_node_id = max(temp_node.children,key = lambda node:temp_node.children.get(node))
				#get the next node from the tree dictionary
				next_node = self.tree_dict[next_node_id]
				#get the character of the next node and concatenate it with the previous suffix
				next_prefix = prefix + next_node.character
				#return the prefix concatenated with the next letter				
				return next_prefix

			else:
				#as the temporary node doesn't have any children
				#return the given prefix indicating failure
				return prefix

		else:		
				#as the current node doesn't have any children
				#return the given prefix indicating failure
				return prefix

#<=========================0==============================>
#declare a function to complete letter by letter a given sentence by user in interaction with him
	#input(1): the constructed trie which contains the letters' occurences
def letter_by_letter(trie):
	#as long as user wants to continue giving incomplete sentences,
	# process the sentence to give a suggestion for the next letter
	#at start assume that user wants to give at least one sentence which will be completed letter by letter
	answer = 'yes'
	while(answer =='yes'):
		#as user wants' to continue giving incomplete sentences, 
		#process its' new sentence to suggest its' next letter	
		#while user don't give '.'
		#initialize suggestion for the first letter of each incomplete sentence to blank string
		suggestion = None
		#at start assume that the model didn't suggest correct
		correct_indicator = '-'
		#clear the screen of terminal for convenience
		os.system("clear")
		while True:
			#check if the model suggest correct the previous letter
			if correct_indicator == '-':
				#if user validate the suggestion as incorrect, ask him to give again the sentence completing the letter by himself 
				#inform the user about procces of interaction
				print('\nInteraction mode: Enter a letter or a incomplete sentence each time ')
				#inform the user about the option of  type 0 to stop the current word	
				print('Please type: . to terminate current sentence ')
				#check if model suggest any letter as next of previous sentence
				if suggestion != None:
					#as the model suggested incorrect the previous word and suggested a letter
					#so incorrect letter is the the last in the suggested sequence
					#get the correct suggestion					
					correct_suggestion = suggestion[0:len(suggestion)-1:1]			
					#get the correct letter which completes the previous sentence by user	
					given_sentence = input(correct_suggestion)
					#concatenate the correct suggestion with the user correct letter, to go on completing the total sentence					
					given_sentence = correct_suggestion + given_sentence

				else:
					#here is the first time of interaction for the specific sentence
					#so ask user to give his starting letter or incomplete sentence
					given_sentence = input()
			else:
				#if model suggested a letter successfully, go on the next letter!!!
				#update the given sentence as the suggestion of model			
				given_sentence = suggestion
				#as user accepts the previous suggestion the next search is starting from the previous last index
			#<=========================0==============================>
			#now check the letter or symbol
			if given_sentence[-1] == ".":
			#if given_sentence[0] == '.':
				#as now suggestion found, concatenate  symbol '>' with the suggestion for pretty output
				if suggestion != None:
					suggestion = '>' + suggestion
				else:
					suggestion = ''
				#if user type symbol . stop processing current word
				print(suggestion,'.')
				#stop the internal while loop
				break

			else:
				#declare a variable to store the previous suggestion			
				previous_suggestion = suggestion
				#user eventually typed a letter so go on to suggest the next one
				sentence_len = len(given_sentence)
				#find the next suggestion of the current user's prefix
				suggestion= trie.find_prefix(given_sentence,0)	
				#for the specific prefix find the keys which contain that prefix							
				#show to user the suggestion for the next letter
				#if model didn't suggest any letter to be the next of the given sentence
				#let user to write it
				#before print the suggested letters' sequence, check if the new suggestion is different of the previous one
				if suggestion != previous_suggestion:
					#the model suggest a new letters' sequence
					print('>>',suggestion)
					#ask user if the suggested letter was right to continue suggesting
					correct_indicator = input('yes: +, no: -\n')

				else:
					#the model cannot suggest a new letter sequence for the given sentence so
					#first inform user
					print('Sorry, the model cannot suggest a new next letter for the given word')
					#before exit this sentence's interaction
					#as model can't suggest anything else stop the interaction for this sentence
					break
		#<=========================0==============================>	
		#ask user if she/he wants to continue giving a new incomplete sentence
		answer = input('\n\nDo you want to continue giving incomplete sentences?\t Please type yes or no \n>')

#<=========================0==============================>
#declare a function to test the constructed bayesian model completing prefixes from TEST_CORPUS_SHORT.txt
#and compare the completed words with the TEST_CORPUS_FULL.txt
#input(1): the constructed trie which contains the letters' occurrences for each word
#input(2): the root of the trie
def test_model(trie,root_trie):
	#declare a list to store the prefixes
	prefix_list = []
	#declare a counter of prefixes
	count_prefix = 0
	#declare a list to store the correct suggestion 
	correct_indicator_list = []
	#declare a variable to store the number of calculated words 	
	count_calculated_words = 0
	#declare a variable to store the number of correct calculated words
	count_correct_words = 0
	#<=========================0==============================>
	#inform the user that the constructed model will be tested
	print('\nStage of testing: Compute a suggestion for the suffix of each word prefix ')
	print('Please Wait...') 
	
	#add all prefixes stored in the TEST_CORPUS_SHORT.txt to the list of prefixes	
	with open('TEST_CORPUS_SHORT.txt',"r") as fin:
		for line in fin:
			
			#clear the terminal symbol of newline 
			line = line.rstrip("\n")
			#the prefixes are in lowercase format so no edit needed
			prefix_list.append(line)
	temp_list = prefix_list

		
	#now get the correct suggestions appears into the TEST_CORPUS_FULL.txt
	with open('TEST_CORPUS_FULL.txt',"r") as fin:
		#read each right suggestion from the file		
		for line in fin:
			#clear the terminal symbol of newline 
			line = line.rstrip("\n")
			#append the correct suggestion to the correspond list
			correct_indicator_list.append(line)

	#<=========================0==============================>
	#now examine each prefix with all the keys of dictionary
	for prefix in temp_list:
		#at start assume that the model didn't suggest any suffix to complete the specific prefix
		suggestion = None
		#find the suffix for each prefix of the temporary list
		suggestion = trie.find_suffix(prefix,root_trie,correct_indicator_list)
		#concatenate the given prefix and the founded suffix to create the suggestion	
		#suggestion = prefix + suffix

		#check if the suggestion is equal to correct word		
		if suggestion != correct_indicator_list[count_prefix] :
			#if the suggestion is -1, the model didn't suggest the correct word or it didn't give any suggestion so inform the user about the failure
			#show to user the details of correct suggestion
			print('prefix=',prefix_list[count_prefix])
			print('\tcalculated word=',suggestion)
			print('\tcorrect word=',correct_indicator_list[count_prefix])
			#inform the user about the wrong suggestion
			print('->Failure of suggestion :(')

		else:
			#if you are here the model did suggested the word correctly
			#so increase the number of correct calculated words
			count_correct_words +=1
			#show to user the details of incorrect suggestion
			print('prefix=',prefix_list[count_prefix])
			print('\tcalculated word=',suggestion)
			print('\tcorrect word=',correct_indicator_list[count_prefix])
			#inform the user about the succeed suggestion
			print('->Success of suggestion :)')

		#at the end of each external for loop increase the prefix counter
		count_prefix += 1
		#do the same for the counter of calculated words
		count_calculated_words += 1

	#<=========================0==============================>
	#as all correct suggestions were compared with the caclulated ones,
	#find the precision of the model, following the type precision = #correct_calculated_words / #calculated_words
	#note that the number of suggestion is one for each prefix, so the number of suggestions is equal to number of prefixes	 	 
	try:
		precision = count_correct_words / count_calculated_words
	except ZeroDivisionError as zero_precision:
		precision = 0
	#inform the user that the constructed model have been tested
	print('\nStage of testing: Compute a suggestion for the suffix of each word prefix \tCompleted! ')	
	#inform about the precision of model
	print('The model has final precision= ',precision)
	input('\nPlease, press any key to continue...')			
		
#<=========================0==============================>
#declare a function to preprocess the TRAIN_CORPUS.txt to clear symbol , . - or short words and numbers
#and train the bayesian model from word given in the TRAIN_CORPUS.txt constructing a trie
	#input(1): the just initialized trie
	#input(2): the trie's root	
	#ouput(1): trie which holds all the words with lenght over 4(words) and the letter occurences of each character of corresponding words
def get_words(trie,root_trie):
	#declare a variable to know how many words have been preprocessed
	prepro_word_counter = 0
	#declare a string to store the succesive progress indicator
	progress_indicator = '|/-\|/-\|-'
	#declare a count to know what is the current progress index
	progress_index = 0
	#declare a variable to hold the number of stored words after the preprocessing
	stored_words = 0 
	#clear the screen of terminal for convenience
	os.system("clear")
	#inform the user about the stage of preprocess
	print('\nStage of preprocess: Clear all symbols such as , . - $ ( ), numbers or  short words(length<4) ')
	print('\n\tStage of training: Compute N(s1,s2,..,sn) for each word by Naive Bayesian model ')
	print('Please wait...')
	#<=========================0==============================>	
	#at first open the TRAIN_CORPUS.txt and clear the symbol and short words
	#pass through all the lines of file as each one is constitued by one word  
	with open('TRAIN_CORPUS.txt',"r") as fin:
		#the symbols have length 1 so they will be cleared as the short words
		for line in fin:

			#increase the counter of preprocessed words
			prepro_word_counter +=1
			#print a progress message at each thousand words
			if(prepro_word_counter % 1000 == 0): 
				if progress_index == 10:
					progress_index = 0
				os.system("clear")
				print('Progress of Preprocessing: %s'%progress_indicator[progress_index])
				print('#Preprocessed Words= %d'%prepro_word_counter)
				progress_index += 1

			#clear the terminal symbol of newline 
			line = line.rstrip("\n")
			#now check if there is a - in middle of the word,if there isn't splitted word is equal to line
			splitted_word = line.split('-')

			#split the each splitted word which have . to get real words
			for i in splitted_word:
				
				splitted_word2 = i.split('.')

				#now check each of the twice-slpitted word
				for j in splitted_word2:

					#at last split the twice-slpitted word if there is a comma in it
					splitted_word3 = j.split(',')

					#now process the three-times splitted word
					for z in splitted_word3:
 						#examine the lenght of each member of splitted_word2 and if the word is a number,
						#if it's length is over 4 (containing the terminal character \0) and it isn't a number store it to words list
						if len(z) >= 4:
							
							if isinstance(z, (int, float, complex)) == False :
								stored_words +=1
								#before store the lowercase format of the word
								temp_string = z.lower()
								key = temp_string[0]
								#check if the first character is a true character and not a symbol or a number
								if key.isalpha() == True:
									#the two conditions are true, so store the processed word into the trie
									trie.insert_data(temp_string,root_trie)
		
	
	#inform the user about the stage of preprocess
	print('\nStage of preprocess: Clear all symbols such as , . - $ ( ), numbers or  short words(length<4)\t Completed! ')
	print('\n\tStage of training: Compute N(s1,s2,..,sn) for each word by Naive Bayesian model\t Completed! ')
	print('*** #Stored Words(in trie)= %d ***'%stored_words)	
	answer = input('Do you want to print the trie?\t Please type yes or no \n>')
	if(answer == 'yes'):
		#show to user all the words list stored in the dictionary of trie
		trie.print_data(root_trie)
		input('\nPlease, press any key to continue...')
	print('\n\n')				
	
	#return the constructed trie and the trie's root 	
	return trie,trie.tree_dict[0]

#<=========================0==============================>
#declare main function
def main():

	#create a new instance of Tree
	trie = Tree()

	#create its' root
	root_trie = trie.add_node('','')

	#call the appropriate function to clear symbol as . , - or words with length under 4
	#and build the trie holding the letters' occurences of preprocessed words
	#get the updated trie and its' root to go on testing the model and make interactive suggestion 
	updated_trie,updated_root = get_words(trie,root_trie)
	
	#call the appropriate function to test the model, giving as input the dictionary of lists of letters' occurences
	test_model(updated_trie,updated_root)

	#now call the last function which suggests next letter of an given incomplete sentence
	#letter_by_letter(dict_letter_counters)
	letter_by_letter(updated_trie)

	#as all functions were called close main and terminate program
	print('\nClosing Program')
	print('Goodbye!')
	
if __name__ == "__main__":
    sys.exit(main())
