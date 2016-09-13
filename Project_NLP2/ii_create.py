import os
import glob
import crawler
import math

#<=======================================================0=======================================================>
#declare a function that removes the stop words and counts the frequency of each lemma in a tagged page and stores the result in a separate directory
	#input(1): name of the directory that contains tagged pages
	#output(1): the inverted index created
def do_create(dir_name):
	os.chdir(dir_name) #change directory to that path
	pos_tags = {'JJ':0, 'JJR':1, 'JJS':2, 'RB':3, 'RBR':4, 'RBS':5, 'NN':6, 'NNS':7, 'NNP':8, 'NNPS':9, 'VB':10, 'VBD':11, 'VBG':12, 'VBN':13, 'VBP':14, 'VBZ':15, 'FW':16} #list of all the tags we are keeping
	iid = {}
	doc_count = 0
	for file_name in glob.glob(os.path.join('*.txt')): #read tagged text from each file, remove the stop words, count frequency of each lemma and write the svm to corresponding file
		doc_count = doc_count + 1
		postag_part, docid, url_part = file_name.split('_', 2)
		tagged_file = open(file_name, 'r') #open the tagged file to process it (remove stop words etc..)
		word_sum = 0 #initialize the variable that counts the usefull words in a .txt
		d = {} #define a dictionary in which we will store the lemma of each useful word as a key and its appearance count as value
		for line in tagged_file: # for every line in each tagged .txt file
			word, tag, lemma = line.split('\t') #split the line in its 3 parts using the TAB as a separator
			lemma0, lemma1 = lemma.split('\n') #split the lemma from the \n
			if ( tag in pos_tags and '<' not in lemma0 and chr(18) not in lemma0): # if the tag for the word in this line is one the tags we want #occ.issuperset(s_tag)
				word_sum += 1 #we found a usefull word
				if ( lemma0 in d ): #if its already in the dictionary
					d[lemma0] += 1 #increase it's appearance count by 1
				else: #if its a new word
					d[lemma0] = 1 #add it to the dictionary and set it's appearance count to 1
		for k, v in d.iteritems(): #for every key (k) - value (v) pair in the dictionary
			v = v/float(word_sum) #count the frequency of the lemma (k) by dividing its value (v) with the word_sum
			if ( k in iid ):
				iid[k][docid] = v
			else:
				iid[k] = {}
				iid[k][docid] = v
		tagged_file.close() #close the tagged file we processed
	for lemma in iid.keys():
		for k, v in iid[lemma].iteritems():
			iid[lemma][k] = float(v) * math.log( doc_count / float(len(iid[lemma])))
	return iid
