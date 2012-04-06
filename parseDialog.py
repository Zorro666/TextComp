#! /usr/bin/python

import os
import xml.etree.ElementTree

def debugPrintDialogFile(dialogFile):
	# Debug print info about the details
	for element in dialogFile:
		f = element[0]
		details = element[1]
		sizes=[]
		deltas=[]
		buildNumbers=[]
		for d in details:
			buildNumber = d['buildNumber']
			fileSize = d['fileSize']
			deltaSize = d['deltaSize']
			sizes.append(fileSize)
			deltas.append(deltaSize)
			buildNumbers.append(buildNumber)
			print f, "Build:", buildNumber, "Size:", fileSize, "Delta:", deltaSize

def parseXML(xmlSource):
	excelXML = xml.etree.ElementTree.XML(xmlSource)
	tag = str(excelXML.tag)
	if tag != "Workbook":
		tokens = tag.split('}')
		tag = tokens[-1]

	if tag != "Workbook":
		print("parseXML root tag should be Workbook found", tag)
		return None

	dialogFile = []
	for node in excelXML:
		tag = str(node.tag)
		if tag != "Worksheet":
			tokens = tag.split('}')
			tag = tokens[-1]

		if tag != "Worksheet":
#			print "Ignoring element", tag
			continue

		worksheet = node
		worksheetName = None
		for k in worksheet.attrib:
			keyValue = k
			if keyValue != "Name":
				tokens = keyValue.split('}')
				keyValue = tokens[-1]

			if keyValue == "Name":
				worksheetName = keyValue
				print "Found Worksheet", worksheet.attrib[k]

		for n in worksheet:
			tag = str(n.tag)
			if tag != "Table":
				tokens = tag.split('}')
				tag = tokens[-1]

			if tag != "Table":
#				print "Ignoring element", tag
				continue

			table = n
			for e in table:
				tag = str(e.tag)
				if tag != "Row":
					tokens = tag.split('}')
					tag = tokens[-1]

				if tag != "Row":
					continue

				row = e
				numRow = 0
				item = dict()
				for r in row:
					tag = str(r.tag)
					if tag != "Cell":
						tokens = tag.split('}')
						tag = tokens[-1]

					if tag != "Cell":
						continue

					cell = r
					for c in cell:
						tag = c.tag
						if tag != "Data":
							tokens = tag.split('}')
							tag = tokens[-1]

						if tag != "Data":
							continue

						if numRow == 0:
							item['tag'] = c.text
						if numRow == 2:
							item['value'] = c.text
						numRow += 1
				if numRow == 3:
					print item
				
	return dialogFile

def loadDialogFile(filename):
	if os.path.exists(filename) == False:
		print("File:'%s' not found", filename)
		return None

 	f = open(filename)
	xmlData = f.read()
	dialogFile = parseXML(xmlData)
	f.close()
	return dialogFile

if __name__ == '__main__':
	action = 'parse'

	if action == 'parse':
		filename = "text_mp_messages" + ".xml"
		dialogFile = loadDialogFile(filename)
		debugPrintDialogFile(dialogFile)

