import os, glob
import subprocess
import crawler

#<=========================0==============================>
#declare a function that tags the all pages in given directory and storing them into a separate directory (into the given one)
	#input(1): directory name of pages for tagging!
def pos_tagging(dir_name):
	dir_stored_pages = dir_name #get the name of directory with saved pages #'Saved_Pages--1296065139.33'	
	dir_name0,dir_name1 = dir_stored_pages.split('_') #get the two parts of directory name
	dir_tagged_name = 'Tagged_' + dir_name1 #change the stable first half name from 'Saved' to 'Tagged'

	dir_tagged_pages = crawler.make_dir(dir_tagged_name,1) #make a new directory for tagged pages with the same time stamp with saved pages
	basename = os.getcwd() + '/' + dir_tagged_pages + '/' #store the full path of tagged pages directory

	os.chdir(dir_stored_pages) #change directory to the current one

	for file_name in glob.glob( os.path.join('*.txt') ): #from each file of collection,  read tokenized text, tag it and write the tagged document to corresponding file 
		if ( file_name[0] != 'U' ): #if file name is not Url_Names.txt, Url_id_lists.txt, Url_Text_Statistics.txt and Url_Pure_Text_Statistics.txt, go on to tag it
			tagged_file_name = 'postag_' + file_name #construct a file name for current tagged document
			full_tag_file_name = basename + tagged_file_name #get the full path of file
			tag_file = open(full_tag_file_name,'w') #open file to write the tagged text
			p = subprocess.Popen(['gposttl',file_name],stdout=tag_file,shell=False) #run 'gposttl' command for tagging current read file and save it into tagged documents directory
			p.wait() #process waits to end the previous one to start
