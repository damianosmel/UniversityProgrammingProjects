import sys, os, glob
import urllib2, urlparse
import re, time


#<=========================0==============================>
#declare a function to handle redirection
	#input(1): an object of http redirect handler
	#output(1): result of redirect handling
class SmartRedirectHandler(urllib2.HTTPRedirectHandler):
	def http_error_301(self, reg, fp, code, msg, headers):
		result = urllib2.HTTPRedirectHandler.http_error_301(self, req, fp, code, msg, headers)
		result.status = code
		return result

#<=========================0==============================>
#declare a function to handle default error
	#input(1): an object of default error
	#output(1): result of error handling
class DefaultErrorHandler(urllib2.HTTPDefaultErrorHandler):
	def http_error_default(self, req, fp, code, msg, headers):
		result = urllib2.HTTPError(req.get_full_url(), code, msg, headers, fp)
		result.status = code
		return result

#<=========================0==============================>
#declare a function that make a new directory for saving the url contents
	#input(1): first part of name
	#input(2): flag of usage 0-> create directory with current time stamp 1-> create directory with input name (time stamp included)
	#output(1): full name of created directory
def make_dir(name_dir,use_flag):
	full_dir_name = name_dir	
	if (use_flag == 0):
		full_dir_name = full_dir_name + str(time.time()) #append time stamp to directory name for having many directoris with same name
	if ( os.path.isdir(full_dir_name) ): #remove directory if already exists and it is empty
		os.rmdir(full_dir_name)
	os.makedirs(full_dir_name)
	return full_dir_name

#<=========================0==============================>
#declare a function that gets url content and saves it into a file
	#input(1): url
	#input(2): url content
	#input(3): url id
	#output(1): success = 1 or fail = 0
def save_url_content(url,url_content, url_id):#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	#suppose that file_name = URL.txt where in URL each '.' and '/' have been replaced by '*' and '_' respectively
	modified_url = url.replace(".","*")
	modified_url = modified_url.replace("/","_")
	success = 0
	file_name = str(url_id + 1) + '_' + modified_url + '.txt' #construct the name of file containing the page content
	if (len(file_name) <= 200 ): #if file name has medium length filesystem can save it
		f = open(file_name,'w') #open file to write page content on it
		f.write(url_content)
		f.close() #now, close file
		success = 1
	return success

#<=========================0==============================>
#declare a function that prints and saves the url names and statistics of downloaded pages with html content and without (mode:1,0 respectively)
	#input(1): dictionary of downloaded pages	
	#input(2): sum length of url pages
	#input(3): mode of function usage
	#input(4): number of url pages with pure text (mode -> 0)
def calculate_downloading_statistics(dict_pages,total_url_size, mode, num_pure_texts):	
	if ( mode == 1 ): #mode -> 1 processing downloaded pages with html content
		uniq_links = dict_pages.keys() #get a list of pages dictionary keys
		num_links = len(uniq_links)
		#open a file to write all downloaded pages one per line!
		basename = os.getcwd()
		os.chdir('..')
		names_id_file = 'Url_id_list.txt'
		f = open(names_id_file,'w')
		os.chdir(basename)
		names_file = 'Url_Names.txt'
		f1 = open(names_file,'w')

		for link in uniq_links: #write each link and link + page's id in one line of Url_Names.txt and Url_id_list.txt respectively
			link_id_per_line = link + '\t' + str(dict_pages[link]) + '\n'
			link_per_line = link + '\n'
			f.write(link_id_per_line)
			f1.write(link_per_line)
		
		f.close() #now, close the two files
		f1.close()

		stat_file = 'Url_Text_Statistics.txt' #make the name of file with statistics of pages with html content
		temp_msg = ' and average text length (with html content)= '
	
	else: #mode -> 0 processing downloaded pages without html content
		num_links = num_pure_texts #get the number of pure texts (= number of downloaded pages)
		stat_file = 'Url_Pure_Text_Statistics.txt' #make the name of file with statistics of pages without html content
		temp_msg = ' and average text length (without html content)= '
	
	med_url_size = total_url_size / float(num_links) #calculate medium length of unique url texts
	f = open(stat_file,'w')#open the file to write page statistics	
	
	#create a message with average length of downloaded/pure pages (in characters) to write it in the appropriate txt file and show it to standard output
	stat_msg = '\nStatistics:\t Number of Saved Pages= ' + str(num_links) + '(pages)' + temp_msg + str(med_url_size) + '(characters) \n'
	print(stat_msg)
	f.write(stat_msg)
	f.close()#now, close the statistics file

#<=========================0==============================>
#declare a function to crawl the web and get pages to process
	#input(1): starting list of crawling
	#input(2): desired number of stored pages
	#input(3): desired number of characters for each stored_pages page	
	#output(1): dictionary of crawled pages in pairs ("key": value) -> ("url_path": page_id)
	#output(2): Pages--(timestamp) for later use
def crawl_pages(starting_pages,des_num_pages,des_num_char):
	pages_2_crawl = list(starting_pages) #at start pages to crawl are the same as the starting pages, using list() to get a copy of starting_pages list #set([sys.argv[1]])
	crawled_pages = {} #initialize dictionary to store pairs ("url_path": page_id) for crawled pages 
	stored_pages = {} #initialize dictionary to store pairs ("url_path": page_id) for stored pages #set([])#kanto dictionary	
	linkregex = re.compile('<a\s*href=[\'|"](.*?)[\'"].*?>')
	crawled_page_count = 0	
	stored_page_count = 0
	temp_stored_count = stored_page_count #at start stored pages are equal to stores pages after 50 check
	start_index = len(starting_pages) - 1 #at start the start page index is equal to the last entry of input list
	sum_url_size = 0 #at start total size of crawled pages is 0
	
	#declare a string to store the succesive progress indicator
	progress_indicator = '|/-\|/-\|/'
	#declare a count to know what is the current progress index
	progress_index = 0

	dir_name = 'Saved_Pages--' #assume that the stable
	dir_stored_pages = make_dir(dir_name,0) #before start visiting pages, create the folder where pages' content will be saved
	dir_name0, dir_name1 = dir_stored_pages.split('_') #get the two parts of directory name
	os.chdir(dir_stored_pages)

	#clear the screen of terminal for convenience
	os.system("clear")
	#inform the user about the stage of preprocess
	print('\nStage 1: Crawling the WWW and Storing the appropriate pages! ')
	print('Please wait...')
	unwanted_type_pages = ['php','js','pdf','doc','xls']

	#visit and check new pages, poping pages from the start of pages_2_crawl stack, until desired number of pages was found
	while stored_page_count < des_num_pages:
		
		index = len(pages_2_crawl) - 1 #choose for page index to crawl the first page of stack
		
		#at every fifty visited words print a progress message and check the ratio between stored pages and visited page if the ratio < 0.2 pick a new starting page to start crawling 
		if(crawled_page_count % 50 == 0): 
			if progress_index == 10:
				progress_index = 0
			os.system("clear")
			print('Progress of fetching pages: %s'%progress_indicator[progress_index])
			print('#Stored Pages= %d'%stored_page_count)
			progress_index += 1

			if ( crawled_page_count != 0 ): #if no page has been crawled there is no ratio to check
				temp_ratio = (stored_page_count - temp_stored_count) / 50.0 #calculate the ratio between stored pages and visited page
				if ( temp_ratio < 0.2 ): #if the ratio of saved pages over total visited is lower tha 0.2, get a new starting page
					print('Minimum ratio of stored_page_number/crawled_page_number does not hold') #inform user why picking a new page to start crawling
					print('Current ratio = %f'%temp_ratio)
					print('Stored page counter = %d'%stored_page_count)
					print('Current stored counter = %d'%temp_stored_count)
					print('Pick a new starting web page')
					start_index -= 1 #get the next "good" starting page from the bottom of stack
					if ( start_index >= 0 ): #check to see if there is any "good" page to be used 
						index = start_index #if there us a good page pick it!
					else :
						index = len(pages_2_crawl) - 1 #if there is no "good" page anymore get the first the page from the start of stack
				temp_stored_count = stored_page_count #update the temporary counter of stored pages to be equal to the current one 
		
		#pop the last page to crawl from pages_2_crawl stack
		try:
			crawling = pages_2_crawl.pop(index) #delete entry of current crawled page
			crawled_page_count += 1 #increase the number of crawled pages by one
			crawled_pages[crawling] = crawled_page_count #append that page to crawled page dictionary
		except KeyError:
			raise StopIteration #if there is no page to crawl stop visiting pages

		#as a page picked to be crawled, start from parsing it and save some useful page's characteristics
		url = urlparse.urlparse(crawling) #parse crawled page url
		url_netloc = url.netloc.split('.')[-1] #get url network location
		url_path_list = url.path.split('.') #get url path
		url_type = url.path.split('.')[-1] #get the last element of url path
		
		#request the url content, if server doesn't respond in 2 seconds go to next link
		try:			
			request_site = urllib2.Request(crawling) #get the site of url
			opener = urllib2.build_opener( SmartRedirectHandler(), DefaultErrorHandler() ) #try to open the site
			datastream = opener.open(request_site, timeout=2) #open stream of site's data in no more than 2 seconds
			download_page = datastream.read() #store site's data into a string
			datastream.close() #close stream
		except:
			continue #as timeout has passed, go to the next link

		#check url and if it meets the requirements, save it into a file of new directory of stored pages
		#if link's domain is .com and it's type is not a unwanted site type, check its' number of characters			
		if (url_netloc == 'com' ) and ( url_type not in unwanted_type_pages):

			#if link doesn't belong to acm nor facebook nor twitter continue checking the link's page
			if ('acm' not in url_path_list and 'facebook' not in url_path_list and 'twitter' not in url_path_list):

				#if url content has at least the desired number of characters and it can be saved it into a file, add it to stored page dictionary!
				if ( len(download_page) > des_num_char ): #check the url content for satisfying desired number of characters criteria
					if (save_url_content(crawling, download_page, stored_page_count) == 1 ):#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						# as page is being saved, update statistics of downloading 
						stored_page_count += 1 #increase stored page counter
						sum_url_size += len(download_page) #add to sum the length of current page
						print('Currently stored page: ' + crawling) #inform user for the currently saved page!
						stored_pages[crawling] = stored_page_count #add clawling page url to stored pages dictionary#stored_pages.add(crawling)

		#now process the links of url content to find new links to visit!
		links = linkregex.findall(download_page)
		links = links[::-1] #get the download page links from top to bottom as best links are at the top or the middle of a page generally

		#pass through all links to modify its' url to be accessible afterwards
		for link in ( links.pop(0) for _ in xrange( len(links) ) ):
			if link.startswith('/'):
				link = 'http://' + url[1] + link
			elif link.startswith('#'):
				link = 'http://' + url[1] + url[2] + link
			elif not link.startswith('http'):
				link = 'http://' + url[1] + '/' + link
			if (link not in crawled_pages) and (link not in pages_2_crawl): #if link(page) has not been crawled and still is not to be visited, add link for crawling
				pages_2_crawl.append(link) #add link to pages_2_crawl stack in order to be visited

	calculate_downloading_statistics(stored_pages,sum_url_size, 1, 0) #calculate the url texts statistics with html content (mode -> 1)
	return stored_pages, dir_name1 #return a dictionary with pairs ("url_path": page_id) for each stored page and the part of the directory name with the timestamp

