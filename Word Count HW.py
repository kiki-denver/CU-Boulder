#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Name:        Word_Count_HW_5
#
# Purpose:     Counts the number of words in a file provided by the user and
#              gives the same result as wc -w.
#
# Author:      Monique Mitchell; monique.mitchell@colorado.edu; 720-301-6893
#
# Created:     Nov. 26, 2012
#-------------------------------------------------------------------------------

#Defines a master "count" variable as 0 which we will add to later on in the
#program as we learn the word count for each line.
count=0
#Asks user to enter a file name.
file=raw_input('Please enter the absolute path to a file:\n')
#Starts a 'for loop' that iterates through each line in the file that the user
#entered based on the criteria nested in said 'for loop.'
for line in open(file):
    #Splits the line into words by separating the string into pieces using the
    #blank space as a guide.
    words=line.split()
    #Defines a variable that is equal to the number of words in a line.
    count1=len(words)
    #Adds each new word count of the line to our "count" variable so that as the
    #'for loop' progresses, each new line's word count is appended to our master
    #"count" variable.
    count = count + count1
#Prints to the screen the total word count for the file, including a little line
#of text as introduction.
print "Your file's word count is:\n"
print count