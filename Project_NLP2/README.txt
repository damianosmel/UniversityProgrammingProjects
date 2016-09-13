In collaboration with my colleague Christos Mitsakos, we create an example of web search engine:
crawler.py - starting from initial "hubs" crawls webpages
html2text.py - for a given page cleans html to get plain text
pos_tagger.py - postag the plain text of a page
ii_create.py - represent each page in vector space and create the inverted index {.., wordi->{.., pagej:wordFrequencyij * InvertedDocumentFrequencyi,..}, ..}
koukl.py - interactive search engine for (every time) web pages.
(Project on "Language Technology" course at Univ. of Patras)
