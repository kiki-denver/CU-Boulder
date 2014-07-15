#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Nombre:           gramfinalproject.py
#
# PropOsito:        Sacar todos los ejemplos de presente perfecto (e.g., "he comido")
#                   y todos los ejemplos de presente simple (e.g., "comI") de unas
#                   entrevistas del espaNol puertorriqueNo en formato de texto con
#                   todas las letras acentuados reemplazados con esa letra en mayUsculas.
#
# Autora:           Monique Mitchell; monique.mitchell@colorado.edu; 720-301-6893
#
# Hecho:            9 de noviembre, 2013
#-------------------------------------------------------------------------------

import re
from pprint import pprint

def getSentences(aText):
    aSentences = []
    apreSentences = re.split(r'(?<!\WDr|\WMr|\WMs|\Weg|\Wex|Mrs|D\.A|D\.J)(\.|!|\?|:|\n)(?=[-"\'\)\]]*\s+\W*[A-Z])', aText)
    for apresentence in apreSentences:
        if re.search(r'\w', apresentence):
            aSentences.append(apresentence)
    return (aSentences, len(aSentences))

def getPP(sentArray):
    presPerfect = []
    for sent in sentArray:
        if re.search(r'\ h[e|as|a|an|emos]\ ', sent):
            presPerfect.append(sent)
    return (presPerfect, len(presPerfect))

def getPreterito(sentArray):
    preterito = []
    for sent in sentArray:
        if any(pWord in sent for pWord in pWords):
            preterito.append(sent)
    return (preterito, len(preterito))

#Got the data for the verbs from the master Preterite lists on http://www.intro2spanish.com/
verbfile = open('C:\Users\Monique\Documents\School\SPAN5440 - Gramaticalizacion\Final Project\PreteriteRootsAndEndings.txt', 'r')
pWords = []
word = ""
for line in verbfile.readlines():
    #ln[0] will be the root all the following are the endings
    ln = line.split()
    #range includes the first number but not the last number
    for ending in range(1,len(ln)):
        word = ''.join((" ", ln[0], ln[ending], " "))
        pWords.append(word)


entrevistas = open('C:\Users\Monique\Documents\School\SPAN5440 - Gramaticalizacion\Final Project\Entrevistas 1-5.txt', 'r')
inputText = ""
for line in entrevistas.readlines():
    inputText = inputText + line

Sentences, numSentences = getSentences(inputText)
presSent, numpresSent = getPP(Sentences)
preteritoSent, numpreteritoSent = getPreterito(Sentences)

resultsPP = open("PPresults.txt", "w")
for sent in presSent:
    if (re.search(r'\?', sent)) and (re.search(r'ya', sent)):
        resultsPP.write(sent+'\tPresente Perfecto\tInterrogativa\tSI\n')
    elif re.search(r'\?', sent):
        resultsPP.write(sent+'\tPresente Perfecto\tInterrogativa\n')
    elif re.search(r'ya', sent):
        resultsPP.write(sent+'\tPresente Perfecto\t\tSI\n')
    else:
        resultsPP.write(sent+'\tPresente Perfecto\n')

resultsPreterito = open("Preteritoresults.txt", "w")
for sent in preteritoSent:
    if (re.search(r'\?', sent)) and (re.search(r'[y|Y]a', sent)):
        resultsPreterito.write(sent+'\tPreterito\tInterrogativa\tSI\n')
    elif re.search(r'\?', sent):
        resultsPreterito.write(sent+'\tPreterito\tInterrogativa\n')
    elif re.search(r'ya', sent):
        resultsPreterito.write(sent+'\tPreterito\t\tSI\n')
    else:
        resultsPreterito.write(sent+'\tPreterito\n')

resultsPP.close()
resultsPreterito.close()
entrevistas.close()
verbfile.close()
