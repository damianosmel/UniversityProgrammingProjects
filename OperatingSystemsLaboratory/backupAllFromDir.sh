#first question
#this command is for execute the bash shell ,if the bash shell doesn't execute 
#!/bin/bash
#declare a new path  called MY_PATH as the /home/$USER 
#and use when path is needed
MY_PATH=/home/$USER
#arguments for find followed
#search in the path /home/$USER
#-not inverse the string
#-type d for directory
#-daystart start from 00:00 pm
#-mtime 0 is for the  first 24 hours
# > found.txt the returned list saved to a new file named found.txt
#so it found  list of files expect of directories changed after 00:00am during the day and saved it to found.txt 
find $MY_PATH  -type f -daystart -mtime 0 > $MY_PATH/found.txt

#arguments for grep followed
#-v invert the match of search for ./found.txt so will print all the others lines of found.txt(file)
#the result of grep is saved to a new .txt named found2.txt by the operator >
#so all files no directories,excluding found.txt will be saved to found2.txt
#i used two .txt files, as you can't read a txt and simultaneously write on it as we  would have if used grep -v "./found.txt" found.txt > found.txt
grep -v "./found.txt" $MY_PATH/found.txt > $MY_PATH/found2.txt

#print a informative  message about changed files during the day in the standard output 
echo ' Next files changed during the day '
#print the content of file found2.txt in the standard output using cat file 
#found2.txt includes all the not directories that changed during the day excluding ./found.txt(althouhgt it was changed) 
#and no including ./found2.txt as it was made after find  run
#i don't include those 2 files (found.txt and found2.txt) because of being assistant files
cat $MY_PATH/found2.txt
#newline (not knowning how else  can i do it  )
echo ' '

#second question
#make a variable which holds the string of Backup of current day
#the operator $example gives the value of example 
#date +%d=day (displaying 2 numbers)
#date +%m=month (displaying 2 numbers)
#date +%Y=year (displaying 4 numbers) 
Daily_Back="Backup_of_$(date +%d)_$(date +%m)_$(date +%Y)"
#message to standard output to inform user that .tar will be made 
echo 'Making the '$Daily_Back'.tar file'
#newline (not knowning how else  can i do it  )
echo ' '
#arguments for tar destination source  followed
#-c for creating a new archive
#-f as we create a .tar with file source (and not directory or something else)
#$Daily_Back gives the value of Daily_Back so Backup_of_'current date'
#-T file  for getting names from a file
#-P for no using the leading '/' in path names
#so tar read from found2.txt(include the changed not directories during the day) making a backup of them to Backup_of_'current date'.tar made
tar -Pcf $MY_PATH/$Daily_Back.tar -T $MY_PATH/found2.txt

#third question 
#pass to variable <<exist_bzip2>> the result of command which
#which exec  shows the full path of the executables(exec) in the standard output if there no such executable it returns -1 
exist_bzip2=`which bzip2`
#check if the value of exist_bzip2 ($exist_bzip2) is -1 this means that the executable bzip2 doesn't exist in the system
#i used the operator == for condition as the to part of equation are strings
if [ $exist_bzip2 == -1 ]
then 
	#if we are here means that there isn't bzip2 in system 
	#print a message to warn user that there isn't the bzip2  in this system  
	echo "Warning : Sorry,but there is no choice for bzip2 in this system"
	#newline (not knowing how else can i do it)
	echo " "
else
	#if we are here means that there is bzip2 in system
	#echo "The command bzip2 exists"
	#newline (not knowing how else can i do it)
	echo " "
	# $# is the numbers of arguments given to this script(backup.sh) when it was called
	#use -ne as != because we have integers in the two part of inequality
	#check if the number of arguments given isn't one 
	if [ $# -ne 1 ]
 	then 
		#if we are here means that the user gave at least two arguments to script so run bzip2  with the default level of compress 
		#information about the effect of the given arguments by user to running bzips with default argument,printed in the standard output 
		echo "Running bzip2 with default value,due to lack of argument"
		#arguments fir bzip2 followed
		#-z for forcing the compression of file.tar without regard the invocation names
		#-f argument for overwritting option,consequently we replace any old Backup_of_'current_date'.tar.bz2
		#so we compress the Backup_of_'current_date'.tar to Backup_of_'current_date'.tar.bz2 with the default block size as there no valid given argument   
		bzip2 -fz $MY_PATH/$Daily_Back.tar
	else
		#if we are here means that the user gave one argument to script ,so check if the argument is between 0...9
		#so is it,user gave a valuable argument which will passed to bzip2
		#the $n gives the nth given argument with the rang of n between 0 and 9,so $1 gives the first given argument
		#the [[ ]] are for two conditions are checked
		if [[ $1 -gt 0 && $1 -le 9 ]]
		then	
			#so is it,user gave a valuable argument which will passed to bzip2
			#print a informative message to user to tell him that bzip2 will run with given argument 
			echo "Running bzip2 with  your valid argument"
			#arguments fir bzip2 followed
	                #-z for forcing the compression of file.tar without regard the invocation names
        	        #-f argument for overwritting option,consequently we replace any old Backup_of_'current_date'.tar.bz2
          	 	#-$1 for selecting the block size of compression 
			#so we compress the Backup_of_'current_date'.tar to Backup_of_'current_date'.tar.bz2 with the valid given argument
			bzip2 -fz$1 $MY_PATH/$Daily_Back.tar
		else
			#so if we are here ,user gave one argument which is no valuable argument
			#print a warning  message to user to tell him that bzip2 will run with given argument 
			echo "Warning : Running bzip2 with default value,due to invalid argument"
			#arguments fir bzip2 followed
                        #-z for forcing the compression of file.tar without regard the invocation names
                        #-f argument for overwritting option,consequently we replace any old Backup_of_'current_date'.tar.bz2
                        #so we compress the Backup_of_'current_date'.tar to Backup_of_'current_date'.tar.bz2 with the default value
			bzip2 -fz $MY_PATH/$Daily_Back.tar	
		fi
	fi 
fi

#fourth question
##newline (not knowing how else can i do it) 
echo " "
#check if directory called Backup is not exists(operator -x)
# the symbol ./-file means the relative path of file
if [ -x $MY_PATH/Backup ]
then
	#if we are here means that there is Directory named Backup
        #check  the flag of existence of bzip2 in order to see if we have Backup_of_current_date.tar or Backup_of_current_date.tar.bz2
        if [ $exist_bzip2 == -1 ]
        then
		#inform the user about the moving of Backup_of_current_date.tar to Backup Directory
                echo "Moving the $Daily_Back.tar to Backup/"
                echo "Please wait"
                #mv source destination  used to move the source file to destination Directory
                mv  $MY_PATH/$Daily_Back.tar $MY_PATH/Backup
                #inform the user that the moving of Backup_of_current_date was over
                echo "Done"
	else
                #inform the user about the moving of Backup_of_current_date.tar.bz2 to Backup Directory
                echo "Moving the $Daily_Back.tar.bz2 to Backup/"
                echo "Please wait"
                #mv source destination  used to move the source file to destination Directory
                mv  $MY_PATH/$Daily_Back.tar.bz2 $MY_PATH/Backup
                #inform the user that the moving of Backup_of_current_date was over
                echo "Done"
        fi

else
        #if we are here means that there isn't  Directory named Backup
        #warn user that there no such Directory and so make it
        echo "Warning : There isn't such Directory"
        echo "Initialization of Backup/ "
        #so make this Directory using mkdir path_of_file_name again i used relative path for the file(./-Backup)
        mkdir $MY_PATH/Backup
        #check  the flag of existence of bzip2 in order to see if we have Backup_of_current_date.tar or Backup_of_current_date.tar.bz2
        if [ $exist_bzip2 == -1 ]
        then
                #inform the user about the moving of Backup_of_current_date.tar to Backup Directory
                echo "Moving the $Daily_Back.tar to Backup/"
                echo "Please wait"
                #mv source destination  used to move the source file to destination Directory
                #inform the user that the moving of Backup_of_current_date was over
                mv  $MY_PATH/$Daily_Back.tar $MY_PATH/Backup
                echo "Done"
	else
		 #inform the user about the moving of Backup_of_current_date.tar.bz2 to Backup Directory
                echo "Moving the $Daily_Back.tar.bz2 to Backup/"
                echo "Please wait"
                #mv source destination  used to move the source file to destination Directory
                mv  $MY_PATH/$Daily_Back.tar.bz2 $MY_PATH/Backup
                #inform the user that the moving of Backup_of_current_date was over
                echo "Done"
        fi
fi
#deleting assistant file such as found.txt and found2.txt and the Backup_of_current_date.tar as it was moved to Backup Directory
#using rm file (executable) for removing a file and the argument -f used to ignore nonexistant files and never ask the user for the delete 
rm -f $MY_PATH/$Daily_Back.tar
rm -f $MY_PATH/found.txt
rm -f $MY_PATH/found2.txt
