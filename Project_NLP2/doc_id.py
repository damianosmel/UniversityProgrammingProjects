import os
import glob
import xml.etree.ElementTree as ET

#<=======================================================0=======================================================>
#declare a function that processes the svm files and creates the inverted index in the form requested
	#input(1): the inverted index that we want to write as a .xml file
def generate(iid):
	root = ET.Element("inverted_index") #define the root element of the .xml file
	for lemma in iid.keys(): #for all lemmas in the inverted index dictionary
		lemma_lvl = ET.SubElement(root, "lemma") #define each lemma as a sub element of the root element
		lemma_lvl.set("name", lemma) #use the set method to associate the attribute ("name" in this case) with a value (lemma in this case)
		for k, v in iid[lemma].iteritems(): #for all ids (k) and tfs (v) for this lemma
			docid_weight_lvl = ET.SubElement(lemma_lvl, "document") #define the last lvl (doc-id and weight lvl) as a sub element of the element "document"
			docid_weight_lvl.set("id", k)
			docid_weight_lvl.set("weight", str(iid[lemma][k]))
	tree = ET.ElementTree(root) #build the tree as described above
	tree.write("inverted_index.xml") #write the .xml file

#<=======================================================0=======================================================>
#declare a fuction that parses the .xml file given as input and loads the inverted index in the 2D dictionary structure which is then returned
	#input(1): name of the .xml inverted index file. The file must be in the "Language Technology"  directory
	#output(1): the inverted index in a 2D dictionary form
def load_ii(ii_path):
	iid = {} #create an empty dictionary to load the inverted index to
	tree = ET.parse(ii_path) #parse the .xml file requested
	inverted_index = tree.getroot()
	try:
		for lemma in inverted_index: #iterate through every lemma
			iid[lemma.items()[0][1]] = {} #the value of the lemma key is a dictionary itself
			for docid_weight in lemma: #iterate through every docid and tf-idf weight
				#iid[lemma.items()[0][1]][docid_weight.items()[0][1]] = docid_weight.items()[1][1] #the key of the 2nd level dictionary is the docid and the value is the tf-idf weight
				iid[lemma.items()[0][1]][int( docid_weight.items()[0][1] )] = docid_weight.items()[1][1] #DAMIANOS metatrepse ta string id kai weight se int gia na prospelash kai pros8esi
				#to weight den me afhnei na to kanw den kserw giati to bazw egw sto koukl.py na ginetai integer pff	
	except:
		print "Unexpected Error. Most likely the .xml file is not of the appropriate form"
		iid = {}
	return iid
