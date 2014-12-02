import csv
import json

from wikitools import page
from wikitools import wiki

from sets import Set

class SetEncoder(json.JSONEncoder):
	def default(self, obj):
		if isinstance(obj,Set):
			return list(obj)
		return json.JSONEncoder.default(self, obj)


letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

site = wiki.Wiki("http://en.wikipedia.org/w/api.php")
# login - required for read-restricted wikis
site.login("tamblc", "26T-3L4-ZbW-ALB")


for index in range(0,28):
	found = {}
	for letter in letters:
		found[letter]= {}
	with open('data/link'+str(index)+'.txt', 'rb') as csvfile:
		links = csv.reader(csvfile, delimiter=',', quotechar='|')
		# iterates through all rows in the csv
		count=0
		for row in links:
			# checks if page_id has already been found
			p_id = int(row[0])
			try:
				article = page.Page(site, pageid=int(row[0])).title
				first = article[0].lower()	                         
				if article in found[first]: found[first][article].add(row[1])
				else:
					print article
					found[first][article]= Set([row[1]])
			except wikitools.wiki.WikiError:
				pass
			# writes the new pair to local map
		print "done! links found!"
	
	for letter in letters:
		out_dict = found[letter]
		out = json.dumps(out_dict, cls=SetEncoder)
		f = open("alpha/"+index+"/"+letter+ '.json', 'w')
	 	print >> f, out
		f.close()
		print "done"

