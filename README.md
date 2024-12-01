Q1.cpp
----------------
This code is used reverse file based on the input value "flag"
if flag==0
    the code reverses the input file end to end
    input: ./a.out <input_file_name> <flag=0>
if flag==1
    the code reverses the input file from 0 to start_index and end_index to end of file
    input: ./a.out <input_file_name> <flag=1> <start_index> <end<index>

Data Flow for flag=0:
1.open the input file using open() system call in RD_ONLY mode and move the file pointer to end of the file using lseek() system call.
2.Create a output file in the directory named "Assignment1" with the name <flagvalue=0>_<input_file_name>
3.So the output file should look like Assignment1/0_<input_file_name>
4.create a buffer of optimal size and read the content of input file from last chunk of buffer size into buffer using read() system call.
5.Now reverse the content in buffer and write it to the output file using write() system call.
6.move the file pointer using lseek() such that everytime the read() occurs on the new chunk form last.

Data Flow for flag=1:
1.Logically divide the file into 3 parts:
    ..1st part is from [0, start_index-1]
    ..2nd part is from [start_index, end_index]
    ..3rd part is from [end_index+1,end_of the file]
2.Apply the same logic as of flag0 to the 1st part of file
3.move the file pointer to start_index using lseek() system call
4.read the content in the range [start_index, end_index] and write it as it is to output file using 
read() and write() system calls.
5.Again apply tha same login as of flag0 to the last part of the file.

Permissions for directory and output file:

The directory that we create to store the output file should be 0700, which means octal representation of permissions where user have all read, write and execute permissions and groups and others are not having any permission.

The output file that is generated should have read and write permissions to the use and no permissions for groups and others.So the mode will be 0600.

Progress bar:
Compute the current extent of file read and print progress dynamically on the screen


Q2.cpp
--------------------------
This code is used to check whether two files are reverse of each other nor not and to check the permissions of given direcotory and new and old files.

Reverse check:
1.Get the 1st chunk of old file from starting of file and first chunk of new file from ending of file and compare them.
2.do this until you read all the characters in the file
3.For checking permissions of directory and files we have stat() system call that stores the properties of directory or file in a buffer from which we can check the permissions respectively.

input: ./a.out <new_file_name> <old_file_name> <Directory_name>
output:
First 3 lines contain 
1.Directory is created/existing or not
2.Print whether the sizes of the files same or not
3.Print whether both the files are reverse of each other or not
Next 9 lines contain: Permissions of new file 
Next 9 lines contain : Permissions of old file
Next 9 lines contain: Permissions of Directory
