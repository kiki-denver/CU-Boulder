#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Name:         mitchell-monique-assgn1updated.py
#
# Purpose:      CSCI5832 Assignment 1: A program that accepts as input a plain
#               text newspaper article and returns the number of paragraphs,
#               sentences, and words contained in the article.
#
# Author:       Monique Mitchell; monique.mitchell@colorado.edu; 720-301-6893
#
# Due:          September 17, 2013
#-------------------------------------------------------------------------------

import re

def getParagraphs(aText):
    #UPDATED the below line to include \S* so that paragraphs that start with " or (, etc. are included
    aParagraphs = re.split(r'\n\n(?=\S*\w)', aText)
    return (aParagraphs, len(aParagraphs))

def getSentences(aText):
    aSentences = []
    apreSentences = re.split(r'(?<!\WDr|\WMr|\WMs|\Weg|\Wex|Mrs|D\.A|D\.J)(\.|!|\?|:)(?=["\'\)]*\s+\W*[A-Z])', aText)
    for apresentence in apreSentences:
        if re.search(r'\w', apresentence):
            aSentences.append(apresentence)
    return (aSentences, len(aSentences))

def getWords(aText):
    apreWords = re.split(r'\s', aText)
    aWords = []
    for aword in apreWords:
        #sacrificed the ' at the end of sisters' and included words like m.p.h. but
        #sacrificed the final . also counted things like 48% and me.com as a single word
        #Microsoft word counts "-" by itself as a word...for shame!
        aword = aword.rstrip('\'",!?;:\)')
        aword = aword.lstrip('\'",!?;:\(.')
        aword = re.sub(r'\.(?!\w)', "", aword)
        if re.search('\w', aword):
            aWords.append(aword)
    return (aWords, len(aWords))

#Use the inputFile line below for testing.
##inputFile = 'C:\Users\Monique\Documents\School\CSCI5832 - NLP\knight2.txt'
inputFile = str(raw_input("""Please enter the absolute path to the file
##you wish to learn the number of paragraphs, sentences, and words of:\n"""))
inputObject = open(inputFile, 'r')
inputText = ""
for line in inputObject.readlines():
    inputText = inputText + line

Paragraphs, numParagraphs = getParagraphs(inputText)
Sentences, numSentences = getSentences(inputText)
Words, numWords = getWords(inputText)

outputFile = "mitchell-monique-assgn1.txt"
outputObject = open(outputFile, "w")
outputObject.write("The number of paragraphs in this article is: "+str(numParagraphs)+'\n')
outputObject.write("The number of sentences in this article is: "+str(numSentences)+'\n')
outputObject.write("The number of words in this article is: "+str(numWords)+'\n')
outputObject.close()
inputObject.close()
#Use the portion below for testing.
##num = 1
##for paragraph in Paragraphs:
##    print "#:", num, paragraph
##    num = num+1
##num = 1
##for sentence in Sentences:
##    print "#:", num, sentence
##    num = num+1
print """You can find the information you seek in the present directory in the
file entitled 'mitchell-monique-assn1.txt'. Thank you and have a great day! If
for some reason that doesn't work, the number of paragraphs, sentences, and words,
respectively are also listed below."""
print (numParagraphs, numSentences, numWords)