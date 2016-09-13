#!/usr/bin/python2.2
"""HTML2Text: Converts HTML to clean and readable plain text."""

__author__ = "Aaron Swartz, based on code by Aaron Swartz and Lars Pind"
__copyright__ = "(C) 2002 Aaron Swartz. GNU GPL 2"

import re, urlparse
import crawler, os, glob
re_ftag = re.compile(r'(/?)([^\s]+)(.*)', re.I|re.M|re.S)
re_href = re.compile(r'(href|src)\s*=\s*["\']([^"\']+)["\']', re.I|re.M|re.S)
re_href2 = re.compile(r'(href|src)\s*=\s*([^ ]+)', re.I|re.M|re.S)
re_title = re.compile(r'(title|alt)\s*=\s*["\']([^"\']+)["\']', re.I|re.M|re.S)
re_title2 = re.compile(r'(title|alt)\s*=\s*([^ ]+)', re.I|re.M|re.S)
re_comments = re.compile(r'<!--.*?-->', re.I|re.M|re.S)#regular expression for comments 29/1/2011
re_style = re.compile(r'<style fprolloverstyle>.*</style>', re.I|re.M|re.S)#regular expression for style 29/1/2011
re_ul = re.compile(r'<ul>.*</ul>', re.I|re.M|re.S)
def intEnt(m):
	m = int(m.groups(1)[0])
	if m > 128: return "&#" + `m` + ";"
	else: return chr(m)

def xEnt(m):
	m = int(m.groups(1)[0], 16)
	if m > 128: return "&x" + m.groups(1) + ";"
	else: return chr(m)

def expandEntities(text):
	text = text.replace("&lt;", "<")
	text = text.replace("&gt;", ">")
	text = text.replace("&quot;", '"')
	text = text.replace("&ob;", "{")
	text = text.replace("&cb;", "}")
	text = text.replace("&middot;", "*")
	text = re.sub("(&[rl]squo;|&#821[761];)", "'", text)
	text = re.sub("&[rl]dquo;", '"', text)
	text = re.sub("&([aeiou])(grave|acute|circ|tilde|uml|ring);", lambda m: m.groups(1)[0], text)
	text = re.sub(r'&#(\d+);', intEnt, text)
	text = re.sub(r'&#[Xx](\w+);', xEnt, text)
	text = re.sub("&(#169|copy);", "(C)", text)
	text = re.sub("&(mdash|[xX]2014);", "--", text)
	return text

class _html2text:
	def __call__(self, html, basehref, maxlen=80, showtags=0, showlinks=0):
		self.text, self.line, self.maxlen  = '', '', maxlen
		self.pre, self.p, self.br, self.blockquote, self.space = 0, 0, 0, 0, 0
		last_tag_end = 0
		href_urls, href_stack = [], []
		
		# remove comments
		html = re.sub(re_comments, "", html)
		
		# remove style and unsorted lists 29/1/2011
		html = re.sub(re_style, "", html)
		html = re.sub(re_ul, "", html)
		i = html.find('<')
		while i != -1:
			self.output(html[last_tag_end:i])
			# we're inside a tag, find the end
			# make i point to the char after the <
			tag_start = i + 1
			in_quote = 0
			for c in html[i:]:
				i += 1
				if c == ">" and not in_quote: break
				if c == '"' and not in_quote: in_quote = 1
				if c == '"' and in_quote: in_quote = 0
			i -= 1
			full_tag = html[tag_start:i]
			s = re.findall(re_ftag, full_tag)
			if s:
				s = s[0]
				slash, tagname, attributes = s[0], s[1], s[2]
				# valid tag
				t = tagname.lower()
				if t in ['p', 'ul', 'ol', 'table', 'div']:
					self.p = 1
				elif t == ["span", 'tbody']: pass
				elif t == 'br':
					self.text += self.line + '\n'
					self.line = "    " * self.blockquote
				elif t in ['tr', 'td', 'th']:
					self.br = 1
				elif t == "title":
					if slash:
						self.p = 1
					else:
						self.output("TITLE: ")
				elif re.match(r'h\d+', t):
					if not slash: self.p = 1
					out = "=" * int(t[1:])
					if slash:
						out = ' ' + out
					else:
						out += ' '
					self.output(out)
					del out
					if slash: self.p = 1
				elif t == 'li':
					self.br = 1
					if not slash:
						self.output(" -")
						self.line += ' '
				elif t in ['strong', 'b']:
					self.output('*')
				elif t in ['em', 'i', 'cite']:
					self.output('_')
				elif t == 'a' and showlinks:
					if not slash:
						href = re.findall(re_href, attributes) or re.findall(re_href2, attributes)
						title = re.findall(re_title, attributes) or re.findall(re_title2, attributes)
						if href:
							href = href[0][1].replace("\n", "").replace("\r", "")
							href_no = len(href_urls) + 1
							if title: 
								href_urls.append((href, expandEntities(title[0][1])))
							else:
								href_urls.append((href, ""))
							href_stack.append("["+`href_no`+"]")
						else:
							href_stack.append("")
					else:
						if len(href_stack) > 0: 
							if href_stack[-1]:
								self.output(href_stack[-1])
							href_stack.pop()
				elif t == 'pre':
					self.p = 1
					if not slash:
						self.pre += 1
					else:
						self.pre -= 1
				elif t == 'blockquote':
					self.p = 1
					if not slash:
						self.blockquote += 1
					else:
						self.blockquote -= 1
				elif t == "hr":
					self.p = 1
					self.output("-" * maxlen)
					self.p = 1
				elif t == "img":
					self.output("[IMG") 
					href = re.findall(re_href, attributes) or re.findall(re_href2, attributes)
					title = re.findall(re_title, attributes) or re.findall(re_title2, attributes)
					if href:
						href = urlparse.urljoin(basehref, href[0][1].replace("\n", "").replace("\r", ""))
						self.output(": " + href)
						if title: 
							self.output(" ("+ expandEntities(title[0][1]) + ")")
					self.output("]")
				else:
					if showtags:
						self.output("&lt;"+slash+tagname+attributes+"&gt;")
			# set end of last tag to the character following the >
			last_tag_end = i + 1
			i = html.find("<", i)
	
		# append everything after the last tag
		self.output(html[last_tag_end:])
		
		# close all pre tags
		self.pre, self.blockquote = 0, 0
		self.text += self.line + "\n"
		
		if showlinks:
			i = 0
			for u in href_urls:
				i += 1
				self.text += "\n[" + `i` + "]" + (' ' * (len(`len(href_urls)`) - len(`i`) + 1)) + \
				  urlparse.urljoin(basehref, u[0])
				if u[1]: 
					 self.text += "\n   " + (' ' * len(`len(href_urls)`)) + u[1]

		self.text = self.text.replace("&nbsp;", " ")
		self.text = self.text.replace("&amp;", "&")
		return self.text
			
	def output(self, text):
		text = expandEntities(text)
		if self.line == '' and text.isspace(): return
		
		# output the text:
		if self.pre <= 0:
			# we're not inside a PRE tag
			text = re.sub("\s+", " ", text)
			if text == ' ': self.space = 1; return
			if self.space and self.line != "    " * self.blockquote: self.line += " "; self.space = 0			
			i, l = 0, text.split(' ')
			self.dumpbuffer()
			for word in l:
				word = re.sub("&(nsbp|#160);", " ", word)
				if len(self.line) > 0:
					if len(self.line) + 1 + len(word) > self.maxlen:
						# the next word goes past our maxline, break here
						self.text += self.line + '\n'
						self.line = "    " * self.blockquote
				self.line = self.line + word
				if i != (len(l) - 1) and self.line != "    " * self.blockquote: self.line += " "
				i += 1
		else:
			self.text += self.line
			self.line = ''
			self.dumpbuffer()
			# we are inside a pre tag		
			if self.blockquote:
				# break up by lines and indent
				for line in text.split('\n')[:-1]:
					self.text += line + '\n' + ('    ' * self.blockquote)
				self.text += text.split('\n')[-1] # last line, don't add a line break
			else:
				self.text += text

	def dumpbuffer(self):
		if self.p or self.br:
			# we're going to add some newlines, so empty line buffer
			self.text += self.line
			
			if self.text != '': # not the first thing
				if self.p:
					self.text += "\n\n"
				elif self.br:
					self.text += "\n"
			self.line = "    " * self.blockquote
			self.p, self.br = 0, 0
			
html2text = _html2text()

#<=========================0==============================>
#declare a function that tokenize the all pages in given directory and storing them into a separate directory
	#input(1): directory name of pages for tokenization!
def tokenize_pages(directory_name):

	sum_text_len = 0 #at start the aggregate size of texts is 0
	dir_name1 = directory_name #get the half name of directory with saved pages
	dir_stored_pages = 'Saved_' + dir_name1 #construct the full name of directory with saved pages
	os.chdir('..') #change directory to father of current directory
	os.chdir(dir_stored_pages) #go to the directory of saved pages
	saved_page_counter = 0 #at start the counter for saved pages is 0
	
	for file_name in glob.glob( os.path.join('*.txt') ): #from each file of collection, read html text and replace it with tokenized text

		if ( file_name[0] != 'U' ): #if file name is not Url_Names.txt, Url_Text_Statistics.txt and Url_Pure_Text_Statistics.txt, go on to tokenize it				
			tokenized_text = '' #before reading each page the tokenized text is null
			f = open(file_name,'r') #open each downloaded html page

			#construct the real url of page from text name
			doc_id, doc_nam = file_name.split('_', 1);
			temp_name = doc_nam.split('.txt') #get the modified url of html page
			url = temp_name[0].replace("*",".") #replace the character '*' with '.'
			url = url.replace("_","/") # and replace the character '_' with '/'
			plain_text = html2text(f.read(),url) #call function to remove html markup from text
			f.close()#close file of page
			
			plain_text = re.sub('[\n]{2,20}','\n',plain_text) #remove multiple new lines
			plain_text = plain_text.lower() #get the lower case of characters
			sum_text_len += len(plain_text) #get the length of plain text
			raw_list = plain_text.split('\n') #split text into lines

			#process each line of text to get the tokenize text
			for line in raw_list: #split each sentence into words 
				words_inline = re.split('(\W)',line) #split line into usefull(for tagging) and stop words
				
				for word in words_inline: #for each word in the line, check if is not null to add it to the tokenized text
					
					if (word !=' ' and word !=''): #check if word is not null or space character
						word = word.replace('_','') #change pattern "_word_" to word
						tokenized_text = tokenized_text + word + '\n' #add word to tokenized text of specific file
		
			f = open(file_name,'w') #open file of page to replace it's text with the tokenized text
			f.write(tokenized_text) #write the tokenized text to file 
			f.close() #now close file
			saved_page_counter += 1 #as the current file was processed, increase the counter of saved pages by one
	
	null_dict = [] #create a new null dictionary as a dictionary is needed to call function for statistics of tokenized texts
	crawler.calculate_downloading_statistics(null_dict,sum_text_len, 0, saved_page_counter) #calculate statistics of text length without html content
