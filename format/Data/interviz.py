import csv
import wikipedia

with open('link0.txt', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
	for row in spamreader:
			print ', '.join(row)