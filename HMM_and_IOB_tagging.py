#-------------------------------------------------------------------------------
# Name:         HMM_and_IOB_tagging.py
# Orig. Name:   mitchell-monique-assgn3.py
#
# Purpose:      CSCI5832 Assignment 3:  Implement an HMM-based approach to named
#               entity recognition using IOB tags, specifically in references to
#               genes in a set of biomedical journal article abstracts.
#
# Author:       Monique Mitchell; monique.mitchell@colorado.edu; 720-301-6893
#
# Due:          December 20, 2013
#-------------------------------------------------------------------------------
#!/usr/bin/env python

import re
from datetime import datetime
from random import  randint
from collections import Counter
from operator import itemgetter
##from pprint import pprint

class Splitter(object):
    """This class Splits a file into separate training (80%), dev (10%) and test (10%) files"""
    def __init__(self, fileName):
        inFile = open(fileName, 'r')
        train = open(fileName[:-4]+"_train.txt", 'w')
        bigtrain = open(fileName[:-4]+"_bigtrain.txt", 'w')
        dev = open(fileName[:-4]+"_dev.txt", 'w')
        test = open(fileName[:-4]+"_test.txt", 'w')
        devkey = open(fileName[:-4]+"_devkey.txt", 'w')
        testkey = open(fileName[:-4]+"_testkey.txt", 'w')

        ln = inFile.readline()
        sentArrayTrain = ["<START>\t<START>\n"]
        sentArrayKey = []
        sentArray = []
        while ln:
            lns = ln.split()
            if len(lns)==2:
                sentArrayTrain.append(ln)
                sentArrayKey.append(ln)
                sentArray.append(lns[0]+"\n")
            #this means that we are at the end of the sentence
            else:
                rnum = randint(0, 9)
                if rnum<=7:
                    train.write("".join(sentArrayTrain)+"\n")
                    bigtrain.write("".join(sentArrayTrain)+"\n")
                elif rnum==8:
                    devkey.write("".join(sentArrayKey)+"\n")
                    dev.write("".join(sentArray)+"\n")
                    bigtrain.write("".join(sentArrayTrain)+"\n")
                else:
                    testkey.write("".join(sentArrayKey)+"\n")
                    test.write("".join(sentArray)+"\n")
                    bigtrain.write("".join(sentArrayTrain)+"\n")
                sentArrayTrain = ["<START>\t<START>\n"]
                sentArrayKey = []
                sentArray = []
            ln = inFile.readline()

        inFile.close()
        train.close()
        bigtrain.close()
        dev.close()
        test.close()
        devkey.close()
        testkey.close()


class Classifier(object):
    """This class does the Viterbi thing"""
    def __init__(self,algorithm="V",smoothing="L"):
        self.wordCounts = Counter() #observed input
        self.posTagCounts = Counter() #hidden states
        self.word_PosTag = Counter()
        self.posTag_PrevPosTag = Counter()
        self.transProbs = {} #det|N  ; N|VB ; etc. aka postag|prev_postag. indexed by tuples.
        self.obsProbs = {} # word|postag. indexed by tuples.
        self.startProbs = {} # postag|<START>. is substituted for the transProbs initially. indexed by the posTag
        self.unknowns = []
        self.smoothing = smoothing
        self.algorithm = algorithm
        self.viterbi = [{}]

    def train(self, fileName):
        'Here is where we populate the probabilities'

        #Populates the total word and posTag counts necessary for the probabilities
        with open(fileName) as f: #closes the file when done with the loop ?
            ln = f.readline()
            while ln:
                lns = ln.split()
                if len(lns) == 2:
                    word = lns[0]
                    posTag = lns[1]
                    #the keys are the words/posTags; their values, the number of times it appears
                    self.wordCounts[word] += 1
                    self.posTagCounts[posTag] += 1
                ln = f.readline()

        #Now to take care of the unknown words, take all things that only appear
        #once and instead make them an unknown word
        self.unknowns1 = []
##        self.unknowns2 = []
##        self.unknowns3 = []
        self.unknowns = []
        for word in self.wordCounts.keys():
            if  self.wordCounts[word]==1:
                # if it has a number in it
                if re.search('[0-9]', word):
                    self.unknowns1.append(word)
##                # if it is all upper case. Won't match single upper case letters (like I or sentence initial words)
##                if re.search('^[A-Z][A-Z]+$', word):
##                    self.unknowns2.append(word)
##                # if it is mixed case (but not camel case) only matches beginning of word
##                if re.search('^[A-Z]+[a-z]+[A-Z]+', word) or re.search('^[a-z]+[A-Z]+', word):
##                    self.unknowns3.append(word)
                # all the rest of the unknowns
                else:
                    self.unknowns.append(word)

                #remove the unknown word from the wordCounts since it's being added in under the key "<UNK>"
                del self.wordCounts[word]
        self.wordCounts["<UNK_num>"] = len(self.unknowns1)
##        self.wordCounts["<UNK_upper>"] = len(self.unknowns2)
##        self.wordCounts["<UNK_mix>"] = len(self.unknowns3)
        self.wordCounts["<UNK>"] = len(self.unknowns)

        #Getting the counts of the tuples necessary for the probabilities
        with open(fileName) as f:
            #First: Separate it into a Sentence (don't go across sentence boundaries)
            ln = f.readline()
            sentArrayWords = []
            sentArrayPosTags = []
            while ln:
                lns = ln.split()
                if len(lns)==2:
                    #sentArraryWords and sentArrayPosTag will index at the same timestep since they are being appended at the same stage
                    if lns[0] in self.unknowns1:
                        sentArrayWords.append("<UNK_num>")
##                    elif lns[0] in self.unknowns2:
##                        sentArrayWords.append("<UNK_upper>")
##                    elif lns[0] in self.unknowns3:
##                        sentArrayWords.append("<UNK_mix>")
                    elif lns[0] in self.unknowns:
                        sentArrayWords.append("<UNK>")
                    else:
                        sentArrayWords.append(lns[0])
                    sentArrayPosTags.append(lns[1])
            #Second: this means that we are at the end of the sentence, now we can get our tuple counts
                else:
                    #skips 0 because that is <START>_<START> which is a probability of 1 and this way we can do i-1 without falling
                    #off the front. range starts at the first number, increments forward until but NOT INCLUDING the last number
                    for i in range(1, len(sentArrayWords)):
                        self.word_PosTag[(sentArrayWords[i], sentArrayPosTags[i])] += 1
                        self.posTag_PrevPosTag[(sentArrayPosTags[i], sentArrayPosTags[i-1])] += 1 #will get the <START> when i==1
                    sentArrayWords = []
                    sentArrayPosTags = []
                ln = f.readline()

        #Accounting for smoothing in probabilities
        if self.smoothing == "L":
            print "The probabilities have been altered with a modified Laplace smoothing by adding .001 to all counts."
            for word in self.wordCounts.keys():
                for pos in self.posTagCounts.keys():
                    self.word_PosTag[(word, pos)] += .001 #.001
                    self.wordCounts[word] += .001
            for pos in self.posTagCounts.keys():
                for prevPos in self.posTagCounts.keys():
                    self.posTag_PrevPosTag[(pos, prevPos)] += .001 #.001
                    self.posTagCounts[pos] += .001

        #Actually calculating and populating the probabilities. Also writing to a file
        #Store the probabilities somewhere for safe keeping so we don't have to re-get them everytime if we don't want to.
        transP = open(fileName[:-4]+"_TransProbs.txt", 'w')
        obsP = open(fileName[:-4]+"_ObsProbs.txt", 'w')
        startP = open(fileName[:-4]+"_StartProbs.txt", 'w')

        for key in self.word_PosTag.keys():
            # number of times word|PosTag occurred divided by the number of times PosTag occurred
            self.obsProbs[key] = float(self.word_PosTag[key]) / float(self.posTagCounts[key[1]])
            obsP.write('{0}\t{1}\t{2}\n'.format(key[0], key[1], self.obsProbs[key]))

            # If we're looking at the start probability...
            if key[1]=="<START>":
                #number of times posTag|<START> occurred divided by the number of times posTag occurred
                self.startProbs[key[0]] = float(self.posTag_PrevPosTag[key]) / float(self.posTagCounts[key[1]]) #this is the same thing as <START>
                startP.write('{0}\t{1}\n'.format(key[0], self.startProbs[key[0]]))
            # Else we're just looking at a normal transition probability
            else:
                #number of times posTag|prevPosTag occurred divided by the number of times posTag occurred
                self.transProbs[key] = float(self.posTag_PrevPosTag[key]) / float(self.posTagCounts[key[1]])
                transP.write('{0}\t{1}\t{2}\n'.format(key[0], key[1], self.transProbs[key]))
        #Close all the files we've been writing to
        transP.close()
        obsP.close()
        startP.close()

    def classifyFile(self, fileName):
        """Here is where we classify a file with POS tags"""
        #This is the file I will APPEND (not write over) to. First I clear it out (just in case)
        with open(fileName[:-4]+"results.txt", "w"):
            pass
        #Then I open it for appending. Will close at the end of this fucntion
        results = open(fileName[:-4]+"results.txt", 'a')

        #
        with open(fileName) as f:
            #First: Separate it into a Sentence since we'll be classifying one sentence at a time
            ln = f.readline()
            sentArrayWords = []
            sentArrayPosTags = []
            while ln:
                lns = ln.split()
                if len(lns)==1:
                    sentArrayWords.append(lns[0])
            #Second: this means that we are at the end of the sentence, now we can get our tuple counts
                elif len(sentArrayWords)<1:
                    pass
                else:
                    #1st, get the tags.

                    #We start with column 1 (viterbi[0])
                    backpointer = []
                    #If it's an already existing word
                    if (sentArrayWords[0] in self.wordCounts.keys()):
                        word = sentArrayWords[0]
                    #Else if it's an unknown word...
                    # if it has a number in it
                    elif re.search('[0-9]', sentArrayWords[0]):
                        word = "<UNK_num>"
##                    # if it is all upper case. Won't match single upper case letters (like I or sentence initial words)
##                    elif re.search('^[A-Z][A-Z]+$', sentArrayWords[0]):
##                        word = "<UNK_upper>"
##                    # if it is mixed case (but not camel case) only matches beginning of word
##                    elif re.search('^[A-Z]+[a-z]+[A-Z]+', sentArrayWords[0]) or re.search('^[a-z]+[A-Z]+', sentArrayWords[0]):
##                        word = "<UNK_mix>"
                    # all the rest of the unknowns
                    else:
                        word = "<UNK>"
                    for posTag in self.posTagCounts.keys(): #iterates over the states
                        self.viterbi[0][posTag] = self.startProbs[posTag] * self.obsProbs[word, posTag] #this initializes the first column
                        backpointer.append((posTag, self.viterbi[0][posTag]))
                    sentArrayPosTags.append([b for a,b in sorted(((tup[1], tup) for tup in backpointer), reverse=True)][0][0])
                    backpointer = []

                    #Now we move on to the other columns
                    for i in range(1, len(sentArrayWords)):
                        self.viterbi.append({})
                        #If it's an already existing word
                        if sentArrayWords[i] in self.wordCounts.keys():
                            word = sentArrayWords[i]
                        #Else if it's an unknown word...
                        # if it has a number in it
                        elif re.search('[0-9]', sentArrayWords[0]):
                            word = "<UNK_num>"
##                        # if it is all upper case. Won't match single upper case letters (like I or sentence initial words)
##                        elif re.search('^[A-Z][A-Z]+$', sentArrayWords[0]):
##                            word = "<UNK_upper>"
##                        # if it is mixed case (but not camel case) only matches beginning of word
##                        elif re.search('^[A-Z]+[a-z]+[A-Z]+', sentArrayWords[0]) or re.search('^[a-z]+[A-Z]+', sentArrayWords[0]):
##                            word = "<UNK_mix>"
                        # all the rest of the unknowns
                        else:
                            word = "<UNK>"
                        for posTag in self.posTagCounts.keys():
                            self.viterbi[i][posTag] = self.viterbi[i-1][sentArrayPosTags[i-1]] * self.transProbs[(posTag, sentArrayPosTags[i-1])] * self.obsProbs[word, posTag]
                            backpointer.append((posTag, self.viterbi[i][posTag]))
                        sentArrayPosTags.append([b for a,b in sorted(((tup[1], tup) for tup in backpointer), reverse=True)][0][0])
                        backpointer = []

                    #2nd, write the tags to a results file.
                    for i in range(0, len(sentArrayWords)):
                        results.write('{0}\t{1}\n'.format(sentArrayWords[i], sentArrayPosTags[i]))
                    results.write("\n")
                    #Clear out the lists so that we can classify the next sentence.
                    sentArrayWords = []
                    sentArrayPosTags = []
                ln = f.readline()
            results.close()



#Use this below to Split into separate training (80%), dev (10%) and test (10%) files
start = datetime.now()
Splitter("gene.train.txt")
print "**************************PROCESSING TIME**************************"
print "Splitting: {}".format((datetime.now()-start).seconds)

fileName = "gene.train_bigtrain.txt"
v = Classifier("V", "L")

#Use this below if you want to (re)calculate percentages.
start = datetime.now()
v.train(fileName)
print "**************************PROCESSING TIME**************************"
print "Training: {}".format((datetime.now()-start).seconds)

#Use this below to classify a file and write the results to a new file
start = datetime.now()
v.classifyFile("gene.train_dev.txt")
print "**************************PROCESSING TIME**************************"
print "Classifying: {}".format((datetime.now()-start).seconds)

#Use this below to classify a file and write the results to a new file
##start = datetime.now()
##v.classifyFile("gene.train_test.txt")
##print "**************************PROCESSING TIME**************************"
##print "Classifying: {}".format((datetime.now()-start).seconds)

#Use this below to classify the test file and write the results to a new file
##start = datetime.now()
##v.classifyFile("NER-test.txt")
##print "**************************PROCESSING TIME**************************"
##print "Classifying the test: {}".format((datetime.now()-start).seconds)

