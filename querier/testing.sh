# Testing script for querier.
# Author: Maria Paula Mora
# Date: May, 2020
#
# usage: bash -v testing.sh

crawler_wiki=../crawler/wiki1
indexer_wiki=../indexer/result/wiki

# make sure crawler has been ran before 
# if it has not, then run it
if [ ! -d $crawler_wiki ]; then
    cd ../crawler
    make clean
    make
    make test &>/dev/null
    cd ../querier
fi

# make sure indexer has been ran before 
# if it has not, then run it
if [ ! -f $indexer_wiki ]; then
    cd ../indexer
    make clean
    make
    make test &>/dev/null
    cd ../querier
fi

####################################
####### Tests that should fail ######
echo "######################################"
echo "Test that should fail:"
echo 

# less than 4 arguments
echo "Less than 3 arguments given"
./querier
./querier $crawler_wiki
echo

# more than 4 arguments
echo "More than 3 arguments given"
./querier $crawler_wiki $indexer_wiki extra
echo

# Nonexistent directory given
echo "Nonexistent directory given"
./querier madeUpDir $indexer_wiki
echo

# Using indexer on a noncrawler directory
echo "Noncrawler directory given"
./querier ../querier $indexer_wiki
echo


####################################
####### Tests that should fail ######
echo "######################################"
echo "Test that should pass:"
echo "Testing different queries on wiki playground depth 1"
echo 
echo "######################################"
echo "WRONG QUERIES GIVEN"
echo 

./querier $crawler_wiki/ $indexer_wiki < inputs/wrong_input

echo
echo "######################################"
echo "RIGHT QUERIES GIVEN"
echo
./querier $crawler_wiki $indexer_wiki < inputs/correct_input

echo
echo "######################################"
echo "USING FUZZTESTING"

./fuzzquery $indexer_wiki 20 1 > inputs/fuzzquery_input1
./fuzzquery $indexer_wiki 10 2 > inputs/fuzzquery_input2
./fuzzquery $indexer_wiki 30 3 > inputs/fuzzquery_input3

./querier $crawler_wiki $indexer_wiki < inputs/fuzzquery_input1
./querier $crawler_wiki $indexer_wiki < inputs/fuzzquery_input2
./querier $crawler_wiki $indexer_wiki < inputs/fuzzquery_input3