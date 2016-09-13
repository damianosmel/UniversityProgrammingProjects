import sys, os, glob, shutil
import re, time
import crawler, tokenizer2, pos_tagger, doc_id, ii_create, koukl

#<=========================0==============================>
#declare a function to print a message for total time of input stage
	#input(1): stage number
	#input(2): starting and finishing time of stage
def print_time_information(stage_num,str_time,fin_time):
	stage_time = fin_time - str_time #compute the total time of stage	
	stage_min = stage_time / 60 #get the minutes ot total time and the seconds of total time
	stage_sec = stage_time % 60

	print("Information:\t Total time of Stage %d = %d:%d (min:sec)"%(stage_num,stage_min,stage_sec)) #print a informative message for total time of input stage
	pressed_key = raw_input("\nPlease press any key, to continue to the next stage!") #wait user to read, continue the next stage
	os.system("clear") #clear screen as next stage coming!

#<=========================0==============================>
#declare main function
def main():
	#<=========================0==============================>
	#<======================1-Stage===========================>
	os.system("clear")#clear the screen of terminal for convenience
	print('\n1st Stage: Crawling the Web!!! ')
	print('Please Wait...')

	start_list = ["http://www.cbsnews.com/","http://www.usatoday.com/","http://www.washingtonpost.com/","http://www.nytimes.com/","http://www.fullbooks.com/"]#make a list of 5 good english sites to start and redirect crawling from
	#start_list = ["http://www.filmreference.com/","http://www.humanillnesses.com/","http://www.healthofchildren.com/","http://www.nationsencyclopedia.com/","http://www.referenceforbusiness.com/"]
	#start_list = ["http://www.technewsworld.com","http://news.cnet.com","http://www.wired.com","http://www.anandtech.com","http://www.arstechnica.com"]
	num_pages = input('Please, insert how many pages to download \n') #initialize number of crawled pages and number of character in each page
	num_char = input('Also insert minimum number of characters per page \n')
	
	start_time = time.time() #get starting time for crawling appropriate pages	
	crawled_pages, dir_name1 = crawler.crawl_pages(start_list,num_pages,num_char) #call the function to crawl pages
	finish_time = time.time() #get finishing time
	print_time_information(1, start_time, finish_time) #call function to print informative message for current stage

	#<=========================0==============================>
	#<======================2-Stage===========================>
	print('\n2nd Stage: Replace html text with plain text and tokenize it ')
	print('Please Wait...')

	start_time = time.time() #get starting time for making pure text and tokenizing it
	tokenizer2.tokenize_pages(dir_name1) #call function to tokenize all crawled pages
	finish_time = time.time() #get finishing time
	print_time_information(2, start_time, finish_time) #call function to print informative message for current stage
	
	#<=========================0==============================>
	#<======================3-Stage===========================>
	print('\n3rd Stage: Part of speech tagging on tokenized documents, using GPoSTTL ')
	print('Please Wait...')

	dir_stored_pages = 'Saved_' + dir_name1
	os.chdir('..')
	
	start_time = time.time() #get starting time for part of speech tagging of tokenized pages
	pos_tagger.pos_tagging(dir_stored_pages) #call function to tag the tokenized pages
	finish_time = time.time() #get finishing time
	print_time_information(3, start_time, finish_time) #call function to print informative message for current stage

	#<=========================0==============================>
	#<======================4,5-Stage===========================>
	print('\n4th & 5th Stage: Stop words removal & inverted index Creation ')
	print('Please Wait...')

	dir_stored_pages = 'Tagged_' + dir_name1
	os.chdir('..')
	
	start_time = time.time() #get starting time for removing stop words and computing tf-idf
	returned = ii_create.do_create(dir_stored_pages)
	finish_time = time.time() #get finishing time
	print_time_information(4, start_time, finish_time) #call function to print informative message for current stage

	#<=========================0==============================>
	#<======================6-Stage===========================>
	print('\n6th Stage: Inverted index storing ')
	print('Please Wait...')
	
	os.chdir('..')
	start_time = time.time() #get starting time for creating the inverted index
	doc_id.generate(returned)
	finish_time = time.time() #get finishing time
	print_time_information(6, start_time, finish_time) #call function to print informative message for current stage

	#<=========================0==============================>
	#<======================6-Stage===========================>
	print('\n6th Stage: Loading the inverted index')
	print('Please Wait...')

	path = raw_input('Enter the .xml inverted index file to load. Must be in the folder "Language Technology"\n')
	start_time = time.time() #get starting time for loading input inverted index
	iid = doc_id.load_ii(path)
	finish_time = time.time() #get finishing time

	total_word_sum = len(iid) #count the unique lemmata
	stat_file = 'Usefull_Words_Statistics.txt'
	stat_msg = '\nStatistics:\t Number of Unique Lemmata = ' + str(total_word_sum) + '(words) in Inverted Index \n'
	f = open(stat_file,'w')
	f.write(stat_msg)
	f.close()
	print(stat_msg)

	print_time_information(6, start_time, finish_time) #call function to print informative message for current stage

	##<=========================0==============================>
	##<======================7-Stage===========================>
	print('\n7th Stage: Searching Documents with inverted index')
	print('Please Wait...')
	
	koukl.interactive_search(iid) #call interactive search to let user give search query on the inverted index

if __name__ == "__main__":
    sys.exit(main())

