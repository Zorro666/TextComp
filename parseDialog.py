#! /usr/bin/python

import os
import xml.etree.ElementTree

def debugPrintDialogFile(dialogFile):
	# Debug print info about the details
	tagSize = 0
	valueSize = 0
	for item in dialogFile:
		tag = unicode(item['tag'])
		value = unicode(item['value'])
		print str("Tag:'")+tag+str("' Value:'")+value+str("'")
		tagSize += len(tag)
		valueSize += len(value)
	print "Num Items", len(dialogFile), "TagSize", tagSize, "ValueSize", valueSize
	print "CRC Tag Size",4*len(dialogFile),"Saving", tagSize-4*len(dialogFile)

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
				foundIt = 0
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
							foundIt += 1
						if numRow == 2:
							item['value'] = c.text
							foundIt += 1
						numRow += 1

				if foundIt == 2:
					dialogFile.append(item)
				
	return dialogFile

def saveDialogFile(dialogFile, filename, writeTags=True, writeValues=True):
	f = open(filename, "wb")
	for item in dialogFile:
		tag = unicode(item['tag'])
		value = unicode(item['value'])
		if writeTags == True:
			f.write(tag)
			f.write('\0')
		if writeValues == True:
			f.write(value)
			f.write('\0')
	f.close()

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
		masterDialogFile = []
		inputFileNames = []
		totalInputFileSize = 0

#		inputFileNames += ["dialog_ai_recording_list.xml"]
#		inputFileNames += ["dialog_mp_recording_list.xml"]
#		inputFileNames += ["dialog_recording_list.xml"]
		inputFileNames += ["text_game_controls.xml"]
		inputFileNames += ["text_mp_messages.xml"]
#		inputFileNames += ["text_platformspecific.xml"]
#		inputFileNames += ["text_ui_credit_list.xml"]
#		inputFileNames += ["text_ui_database.xml"]
#		inputFileNames += ["text_ui_messages.xml"]
#		inputFileNames += ["text_ui_mp_messages.xml"]
		inputFileNames += ["text_ui_objectives.xml"]

		for inputFileName in inputFileNames:
			dialogFile = loadDialogFile(inputFileName)
#			debugPrintDialogFile(dialogFile)
			inputFileSize = os.path.getsize(inputFileName)
			print "Input File:", inputFileName, inputFileSize, "bytes", len(dialogFile), "lines"
			totalInputFileSize = inputFileSize;
			masterDialogFile += dialogFile

#		debugPrintDialogFile(masterDialogFile)
		outputFileName = "jake.dia"
		saveDialogFile(masterDialogFile, outputFileName, True, True)

		print "Input Files:", totalInputFileSize, "bytes", len(masterDialogFile), "lines"
		print "Output File:", outputFileName, os.path.getsize(outputFileName), "bytes"

