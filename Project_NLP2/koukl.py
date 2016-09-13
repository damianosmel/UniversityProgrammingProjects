import os,time
import operator

#<=======================================================0=======================================================>
#declare a function to print search results in standard output
	#input(1): starting time of search
	#input(2): finishing time of search
	#input(3): a tuple with pairs (page url, aggregate weight) in increasing order
	#input(4): number of query searchs
def print_results(str_time,fin_time,sorted_results,search_count):
	search_time = fin_time - str_time #find total search time
	avg_search_time = search_time / float(search_count)
	#search_min = search_time / 60 #get the minutes ot total time and the seconds of total time
	#search_sec = search_time % 60
	#start_index = 0 #num_results#at start assume that user wants the first 10 results pages
	#end_index =
	num_results = len(sorted_results)#get the size of results' tuple

	if (num_results == 0): #if no results have been found inform user
		print('%d Results in average time %f (sec)\n'%(num_results, avg_search_time))#search_min, search_sec))

	else: #if there is at least one search result, show results to user in pages
		doc_count = 0 #counter to index a document in specific page result
		size_pages = 10 #assume that each page has size_pages documents
		num_pages = num_results / size_pages #calculate the pages of results
		num_doc_last_page = num_results % size_pages #calculate the number of results in the last page
		results_in_page = [] #this is a list with results divided into pages 

		#create pages of size_pages results
		for page_count in range(0,num_pages + 1): #for each page write appropriate results into it
			docs_in_page = [] #create a list to store the results of current page
			while( doc_count < size_pages * ( page_count + 1) and doc_count < num_results): #fill the page with results of input sorted_results
				docs_in_page.append( sorted_results[doc_count] ) #add the current sorted result to page
				doc_count += 1 #increase the counter of result-documents
			results_in_page.append( docs_in_page ) #append the list of page to the list of total results 
	
		user_input = '+'#at start assume that user browse down the results
		page_count = -1#at start page_count is -1

		while( user_input != 'q' ): #while user doesn't press 'q', let him to browse the results
			
			if ( ( user_input == '+' and num_doc_last_page == 0 and page_count == ( num_pages - 1) ) or ( user_input == '+' and num_doc_last_page != 0 and page_count == num_pages ) ):
				user_input = raw_input('Warning: Please, press appropriate key to browse or press q to quit browsing!\n>') #inform user that he pressed wrong key to browse current page
			
			elif ( user_input == '-' and page_count == 0 ):
				user_input = raw_input('Warning: Please, press appropriate key to browse or press q to quit browsing!\n>') #inform user that he pressed wrong key to browse current page

			elif ( user_input != '+' and user_input != '-'):
				user_input = raw_input('Warning: Please, press +/- to browse results or press q to quit browsing\n>') #inform user that he pressed wrong key to browse results

			else: #user pressed right key to browse results so print the results of requested page
				os.system("clear") #clear the screen of terminal for convenience
				print('%d Results in average time %f (sec)\n'%(num_results, avg_search_time))#inform user for the number of results and average time of searching		
				user_input = user_input + '1' #concatenate user input with character 1
				offset = int(user_input) #get the integer equivalent of modified user input
				page_count += offset #user input = + -> browse up pages and user input = - -> browse down pages
				results = results_in_page[page_count] #get the results of requested page

				for document_weight in results: #for each document in result page print its url and tf-idf weight
					print(document_weight[0] + '\t' + str(document_weight[1]))
			
				start_index = page_count * size_pages + 1 #get the index of first page shown
				end_index = (page_count + 1) * size_pages #get the index of last page shown
				if( end_index > num_results ): #if index of lasta page is greater than can be, correct it to the maximum of its value
					end_index = num_results
				print('\t\t\t %d ~ %d '%(start_index,end_index)) #print the index of first and last page shown
				user_input = raw_input('\nPlease, press +/- to browse results or press q to quit browsing\n>')

#<=======================================================0=======================================================>
#declare a function for user interactive search
	#input(1): half name of directory with saved pages
	#input(2): 2-dimensional dictionary containing { lemma{ doc_id: url, ... }, ... } 
def interactive_search(inverted_index):#dir_name,
	first_time = 1 #declare a variable to show if the first seach query is being processed
	user_input = 'yes' #at start assume that user input is yes

	while( user_input == 'yes'): #while user wants to continue searching, process its query
		os.system("clear") #clear the screen of terminal for convenience
		query = raw_input("Please insert your search query!\n> ") #ask user to input search query
		start_time = time.time() #get starting time for processing user query

		for search_counter in range(0,100):
			query_low = query.lower() #get the lower case of query's characters
			list_lemmata = query_low.split(" ") #save lemmata of query into a list
			list_lemmata = [ lemma for lemma in list_lemmata if lemma != "" and len(lemma) > 1] #append only no-null and sized greater than 1 lemmata to list
			
			search_dict = {}
			search_dict1 = {} #make an empty dictionary storing the aggregate weights of each lemma

			first_found_lemma = 0 #at start no lemma has been found
			for lemma in list_lemmata: #for each lemma of user query search inverted index to found results

				if( lemma in inverted_index ): #if you found lemma in inverted index append it to dictionary of results appropriately

					if( first_found_lemma == 0 ): #if current lemma is the first found in current search initiliaze dictionary of results with it
						search_dict = inverted_index[lemma] #result-dictionary is equal to the dictionary of lemma in inverted index
						for doc_id in search_dict: #for each document id in lemma-dictionary modify and add its' weight to result dictionary
							float_weight = float(search_dict[doc_id])
							search_dict1[doc_id] = float_weight
						first_found_lemma = 1 #as the first lemma was found update variable from 0 to 1
					else :
						#if current lemma is not the first found one, append its weights to the current result-dictionary
						temp_dict = inverted_index[lemma] #save to temporary dictionary the dictionary of lemma in inverted index
						for doc_id in temp_dict: #for each document id in lemma-dictionary append its weight to corresponding weight in result-dictionary
							doc_weight = float( temp_dict[doc_id] ) #transform string variable to float one

							if ( doc_id in search_dict1 ): #if document id is already in the result-dictionary then add its weight to the corresponding weight
								search_dict1[doc_id] += doc_weight #search_dict[doc_id] +
							else: #if document id is now added to result-dictionary then append dictionary with (doc_id: doc_weight)
								search_dict1[doc_id] = doc_weight

			if ( first_time == 1): #if current search is the first, transfer from text to memory a dictionary with pairs (id:url)
			
				saved_path = os.getcwd() + '/Url_id_list.txt' #get the path of saved pages #+ '/Saved_' + dir_name 
				doc_id_file = open(saved_path, 'r') #open the Url_id_list.txt file that contains the id - url pairs for all the pages
				url_id = {} #create an empty dictionary to load the file above into
			
				for line in doc_id_file: #for every line of the file append its contents to the dictionary
					doc, doc_id = line.split ('\t') #doc contains the url and docid contains the id (with a trailing \n)
					doc_id, empty = doc_id.split ('\n') #remove the trailing \n
					url_id[int(doc_id)] = doc #load these values in the dictionary. The key is the id and the value is the doc
				doc_id_file.close() #close the file

				first_time = 0# the first time has passed so update first_time variable from 1 to 0
			
			final_dict = dict( ( url_id[doc_id], sum_weight ) for ( doc_id, sum_weight ) in search_dict1.items() )#make a new dictionary with page's url instead of page's id
			sort_results = sorted( final_dict.iteritems(), key = operator.itemgetter(1) )[::-1]#order final dictionary increasingly
			
		finish_time = time.time() #get finishing time
		print_results(start_time,finish_time,sort_results,search_counter + 1) #call function to print the search results
		
		user_input = raw_input('\nPlease, type yes/no to continue or quit searching\n>') #ask user if he wants to continue searching
