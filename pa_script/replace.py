#!/bin/python3
import docx
import sys

doc = docx.Document(sys.argv[1])
for p in doc.paragraphs:
    if sys.argv[2] in p.text:
        print (sys.argv[2] + " found in " + sys.argv[1] + " replacing with " + sys.argv[3])
        p.text = p.text.replace(sys.argv[2], sys.argv[3])
doc.save(sys.argv[1])