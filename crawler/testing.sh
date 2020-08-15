# Testing script for crawler.c
# Author: Maria Paula Mora
# Date: April, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL1=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
seedURL2=http://old-www.cs.dartmouth.edu/~cs50/index.html
seedURL3=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
seedURL4=http://old-www.cs.dartmouth.edu/index.html
seedURL5=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html

externalURL=www.google.com
nonExistentServer=www.myNameIsMariaPaulaMora.com
nonExistentPage=http://old-www.cs.dartmouth.edu/~cs50/mariaMora.html


#####################################
####### Tests that should fail ######
mkdir data
echo "######################################"
echo "Test that should fail:"
echo 

# less than 4 arguments
echo "Less than 4 arguments given"
./crawler
./crawler $seedURL1
./crawler $seedURL1 data
echo

# more than 4 arguments
echo "More than 4 arguments given"
./crawler $seedURL1 data 6 7 8 
echo

# Negative max depth
echo "Negative max depth"
./crawler $seedURL1 data -2
echo

# Negative max depth
echo "Max depth is too big"
./crawler $seedURL1 data 1000
echo

# 4 using external url
echo "Using external URL"
./crawler $externalURL data 2
echo

# Non existing directory
echo "Non-existent directory"
./crawler $seedURL1 badDIR 2
echo

# Incorrect third parameter
echo "Incorrect max depth given (string instead of integer)"
./crawler $seedURL1 data number
echo

# Non existent server
echo "Non existent server given"
./crawler $nonExistentServer data 2
echo

#valid server but non-existent page
echo "Valid server but non-existent page"
./crawler $nonExistentPage data 2
echo

######################################
### These tests should pass ####
echo "######################################"
echo "Test that should pass"
echo

# at depth 0
echo "$seedURL1 at depth 0 -> see data1 directory"
mkdir data1
./crawler $seedURL1 data1 0
echo

# at depth 2
echo "$seedURL2 at depth 2 -> see data2 directory"
mkdir data2
./crawler $seedURL2 data2 2
echo

# at depth 5
echo "$seedURL1 at depth 5 -> see data3 directory"
echo "This is a closed set of cross-linked web pages"
mkdir data3
./crawler $seedURL1 data3 5
echo


# at depth 5
echo "$seedURL5 at depth 5 -> see data4 directory"
echo "Different seed page in that same closed set of cross-linked web pages"
mkdir data4
./crawler $seedURL5 data4 5
echo

# given different pageDirectory formats (./dir/ ./dir)
echo "Given different pageDirectory formats (./dir/ ./dir) -> see data5 and data6 directory"
mkdir data5 
./crawler $seedURL4 ./data5/ 2
mkdir data6
./crawler $seedURL4 ./data6 2
echo

echo "Wikipedia playground at depth 1 -> see wiki1 directory"
mkdir wiki1
./crawler $seedURL3 wiki1 1
echo

# UNCOMMENT TO CHECK WIKIPEDIA PLAYGROUD AT LARGER DEPTH:
# Commented because it takes too long and then testing.out is about 1000 lines long
# Didn't want such a long testing file 
# Point the crawler at our Wikipedia playground. Explore at depths 0, 1, 2.

# echo "Wikipedia playground"
# echo "Wikipedia playground at depth 2 -> see wiki2 directory"
# mkdir wiki2
# ./crawler $seedURL3 wiki2 2
# echo