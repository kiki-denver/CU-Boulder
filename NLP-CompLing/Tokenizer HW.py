#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Name:        Tokenizer_HW_5
#
# Purpose:     A tokenizer which separates words from punctuation in a string
#              provided by the user.
#
# Author:      Monique Mitchell; monique.mitchell@colorado.edu; 720-301-6893
#
# Created:     Nov. 26, 2012
#-------------------------------------------------------------------------------

#Imports the regular expression package.
import re
#Asks the user for a string to tokenize and defines it under the variable
#"string".)
string=raw_input("Please enter your sentence to tokenize:\n")
#Pulls out all the individual words in the string provided by the user and
#defines it to the variable matches.
matches = re.findall(r'\b\w+\'*\w*\b|\W', string)
#Prints the separated resulting string, item by item.
for match in matches:
    print match
