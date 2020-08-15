# Testing script for indexer.c and indextest.c
# Author: Maria Paula Mora
# Date: May, 2020
#
# usage: bash -v testing.sh

DATA1=../crawler/data1
DATA2=../crawler/data2
DATA3=../crawler/data3
DATA4=../crawler/data4
WIKI=../crawler/wiki1

RES=result

if [ ! -d $RES ]; then
    mkdir $RES
fi

# make sure crawler has been ran before 
# if it has not, then run it
if [ ! -d $DATA1 ]; then
    cd ../crawler
    make clean
    make
    make test &>/dev/null
    cd ../indexer
fi

echo "____________________________________________"
echo "TESTING INDEXER.c:"
echo 

####################################
####### Tests that should fail ######
echo "######################################"
echo "Test that should fail:"
echo 

# less than 4 arguments
echo "Less than 3 arguments given"
./indexer
./indexer $DATA1
echo

# more than 4 arguments
echo "More than 3 arguments given"
./indexer $DATA1 $RES/data1 extra
echo

# Nonexistent directory given
echo "Nonexistent directory given"
./indexer madeUpDir $RES/madeUpDir
echo

# Invalid filename given
echo "Invalid filename given"
./indexer $DATA1 $DATA1
echo

# Using indexer on a noncrawler directory
echo "Noncrawler directory given"
./indexer ../querier $RES/querie
echo

######################################
### These tests should pass ####
echo "######################################"
echo "Test that should pass"
echo

# Third parameter is a file that already exists
# Should overwrite file
echo "Given a file that already exists --> see test_file1"
touch $RES/test_file1
./indexer $DATA1 $RES/test_file1
echo

# given different pageDirectory formats (./dir/ ./dir)
echo "Given different pageDirectory format (../crawler/dir/) --> see test_file2"
./indexer $DATA1/ $RES/test_file2
echo

# indexing data1
echo "data 1 directory -> see result/data1"
./indexer $DATA1 $RES/data1
echo

# indexing data2
echo "data 2 directory -> see result/data2"
./indexer $DATA2 $RES/data2
echo

# indexing data3
echo "data 3 directory -> see result/data3"
./indexer $DATA3 $RES/data3
echo

# indexing data4
echo "data 4 directory -> see result/data4"
./indexer $DATA4 $RES/data4
echo

# indexing wiki 
echo "wiki directory -> see result/wiki"
./indexer $WIKI/ $RES/wiki
echo

# Testing indextest on previously built files
echo "____________________________________________"
echo "Testing INDEXTEST.c:"
echo 


####################################
####### Tests that should fail ######
echo "######################################"
echo "Test that should fail:"
echo 

# less than 4 arguments
echo "Less than 3 arguments given"
./indextest
./indextest $RES/data4
echo

# more than 4 arguments
echo "More than 3 arguments given"
./indextest $RES/data4 $RES/data4 extra
echo

# Nonexistent oldIndexFilename given
echo "Nonexistent oldIndexFilename given"
./indextest madeUpFile $RES/madeUpFile
echo

# Invalid filename given
echo "Invalid newIndexFilename given"
./indextest $RES/data4 $DATA1 
echo

# Empty file given as oldIndexFilename
echo "Empty file given as oldIndexFilename"
touch $RES/test_file
./indextest $RES/test_file $RES/test_file_copy
echo

######################################
### These tests should pass ####
echo "######################################"
echo "Test that should pass"
echo

echo "Testing indextest with the index file data1"
./indextest $RES/data1 $RES/data1_copy
echo

echo "Sorting the files..."
gawk -f ./indexsort.awk $RES/data1 > $RES/data1.sorted
gawk -f ./indexsort.awk $RES/data1_copy > $RES/data1_copy.sorted
echo 

echo "Comparing the files"
diff $RES/data1.sorted $RES/data1_copy.sorted &>/dev/null
if [ $? -eq 0 ]
    then
        echo "Result: No differences found between files"
else echo "Result: Files where different"
fi
echo

echo "Testing indextest with the index file data2"
./indextest $RES/data2 $RES/data2_copy
echo

echo "Sorting the files..."
gawk -f ./indexsort.awk $RES/data2 > $RES/data2.sorted
gawk -f ./indexsort.awk $RES/data2_copy > $RES/data2_copy.sorted
echo 

echo "Comparing the files"
diff $RES/data2.sorted $RES/data2_copy.sorted &>/dev/null
if [ $? -eq 0 ]
    then
        echo "Result: No differences found between files"
else echo "Result: Files where different"
fi
echo

echo "Testing indextest with the index file data3"
./indextest $RES/data3 $RES/data3_copy
echo

echo "Sorting the files..."
gawk -f ./indexsort.awk $RES/data3 > $RES/data3.sorted
gawk -f ./indexsort.awk $RES/data3_copy > $RES/data3_copy.sorted
echo 

echo "Comparing the files"
diff $RES/data3.sorted $RES/data3_copy.sorted &>/dev/null
if [ $? -eq 0 ]
    then
        echo "Result: No differences found between files"
else echo "Result: Files where different"
fi
echo

echo "Testing indextest with the index file data4"
./indextest $RES/data4 $RES/data4_copy
echo

echo "Sorting the files..."
gawk -f ./indexsort.awk $RES/data4 > $RES/data4.sorted
gawk -f ./indexsort.awk $RES/data4_copy > $RES/data4_copy.sorted
echo 

echo "Comparing the files"
diff $RES/data4.sorted $RES/data4_copy.sorted &>/dev/null
if [ $? -eq 0 ]
    then
        echo "Result: No differences found between files"
else echo "Result: Files where different"
fi
echo