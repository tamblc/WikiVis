import csv
import wikipedia

with open('data/link0.txt', 'rb') as csvfile:
	links = csv.reader(csvfile, delimiter=',', quotechar='|')
	found = {}
	# iterates through all rows in the csv
	for row in links:
		# checks if page_id has already been found
			article = wikipedia.page(row[0])
			if article in found:
				 found[article].add(row[1])
			else:
				found[article]= Set([row[1]])
		# writes the new pair to local map
	print "done! "+found.size()+ " links found!"