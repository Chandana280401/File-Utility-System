#include<unistd.h>
#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 
#include<cstdio>
#include<stdio.h>

using namespace std;
//to check if the content in the new file is reverse of old file
bool areReverse(const char *inp_file, const char *out_file){

    int inp_file_dsp=open(inp_file, O_RDONLY);
    int out_file_dsp=open(out_file, O_RDONLY);

    off_t inp_file_size= lseek(inp_file_dsp,0,SEEK_END); 
    off_t out_file_size= lseek(out_file_dsp,0,SEEK_END); // pointer for output file is beyond last char

    if(inp_file_size==0 && out_file_size==0){ //handling empty file
        return true;
    }

    if(inp_file_size != out_file_size)
        return false;

    int buff_size;
    if(inp_file_size>100000)
        buff_size=inp_file_size/1000;
    else
        buff_size=inp_file_size;
        
    char inp_buff[buff_size];
    char out_buff[buff_size];

    lseek(inp_file_dsp,0,SEEK_SET); //pointer for input file is pointing to 1st char
   
    long long int bytes_read=0,rd_off_set,n1,n2,idx;
    rd_off_set=out_file_size-buff_size;
    while(rd_off_set>=0){

        idx=lseek(out_file_dsp,rd_off_set,SEEK_SET);

        n1=read(inp_file_dsp,inp_buff,buff_size);
        n2=read(out_file_dsp,out_buff,buff_size);

        if(n1!=n2)
            return false;
        if(n1==-1 || n2==-1){
            perror("Error while reading");
            close(inp_file_dsp);
            close(out_file_dsp);
            return false;
        }

        for(long long int i=0;i<n1;i++){
            if(inp_buff[i]!=out_buff[n1-1-i])
                return false;
        }
        bytes_read+=n1;
        rd_off_set-=buff_size;
    }
    if(bytes_read<inp_file_size){
        lseek(out_file_dsp,0,SEEK_SET);
        n1=read(inp_file_dsp,inp_buff,idx);
        n2=read(out_file_dsp,out_buff,idx);

        if(n1!=n2)
            return false;
        if(n1==-1 || n2==-1){
            perror("Error while reading");
            close(inp_file_dsp);
            close(out_file_dsp);
            return false;
        }

        for(long long int i=0;i<n1;i++){
            if(inp_buff[i]!=out_buff[n1-1-i])
                return false;
        }
        bytes_read+=n1;
    }
    if(bytes_read!=inp_file_size)
        return false;
    
    return true;

}
int main(int argc,char * argv[]){


    char *new_file=argv[1]; //outputfile
    char *old_file=argv[2]; //inputfile
    char *dir_name=argv[3];


    int slashidx=-1,i=0;
    string out_file_path=new_file;
    while(out_file_path[i]!='\0'){
        if(out_file_path[i]=='/'){
            slashidx=i;
        }
        i++;
    }
    string out_file_name="";
    i=slashidx+1;
    while(out_file_path[i]!='\0'){
        out_file_name+=out_file_path[i];
        i++;
    }

    string slash;
    slash="/";
    string oldfile =old_file;
    string newfile= dir_name+slash+out_file_name;

    const char *inp_file= oldfile.c_str();
    const char *out_file= newfile.c_str();

   
    struct stat statbuff;
    if(stat(dir_name,&statbuff)==0 && S_ISDIR(statbuff.st_mode))
    write(1,"Directory is created: Yes\n",26);
    else
    write(1,"Directory is created: No\n",25);

    if(!(stat(inp_file,&statbuff)==0 && S_ISREG(statbuff.st_mode)))
    perror("Old file not found");
   
    if(!(stat(out_file,&statbuff)==0 && S_ISREG(statbuff.st_mode)))
    perror("New file not found");

    if(areReverse(inp_file,out_file)){
        cout<<"Whether file contents are reversed in new file: Yes\n";
        cout<<"Both File Sizes are Same : Yes\n";
    }
    else{
        cout<<"Whether file contents are reversed in new file: No\n";
        cout<<"Both File Sizes are Same : No\n";
    }

    cout<<endl;
    //Checking permissions of new file
    if(stat(out_file, &statbuff)==0){
        //check if it is a file or directory
        if(!S_ISREG(statbuff.st_mode)){
            perror("New file isn't a regular file");
            return 0;
        }
        

        //User permissions
        if(statbuff.st_mode & S_IRUSR)
        cout<<"User has read permissions on newfile: Yes\n";
        else
        cout<<"User has read permissions on newfile: No\n";

        if(statbuff.st_mode & S_IWUSR)
        cout<<"User has write permission on newfile: Yes\n";
        else
        cout<<"User has write permission on newfile: No\n";

        if(statbuff.st_mode & S_IXUSR)
        cout<<"User has execute permission on newfile: Yes\n";
        else
        cout<<"User has execute permission on newfile: No\n";

        //Group permissions
        if(statbuff.st_mode & S_IRGRP)
        cout<<"Group has read permissions on newfile: Yes\n";
        else
        cout<<"Group has read permissions on newfile: No\n";

        if(statbuff.st_mode & S_IWGRP)
        cout<<"Group has write permission on newfile: Yes\n";
        else
        cout<<"Group has write permission on newfile: No\n";

        if(statbuff.st_mode & S_IXGRP)
        cout<<"Group has execute permission on newfile: Yes\n";
        else
        cout<<"UGroup has execute permission on newfile: No\n";
        
        //Other permissions

        if(statbuff.st_mode & S_IROTH)
        cout<<"Others has read permissions on newfile: Yes\n";
        else
        cout<<"Others has read permissions on newfile: No\n";

        if(statbuff.st_mode & S_IWOTH)
        cout<<"Others has write permission on newfile: Ye\ns";
        else
        cout<<"Others has write permission on newfile: No\n";

        if(statbuff.st_mode & S_IXOTH)
        cout<<"Others has execute permission on newfile: Yes\n";
        else
        cout<<"Others has execute permission on newfile: No\n";
    }
    cout<<endl;
    //checking permissions of Old file
    if(stat(inp_file, &statbuff)==0){
        //check if it is a file or directory
        if(!S_ISREG(statbuff.st_mode)){
            perror("Old file isn't a regular file");
            return 0;
        }
        //User permissions
        if(statbuff.st_mode & S_IRUSR)
        cout<<"User has read permissions on Oldfile: Yes\n";
        else
        cout<<"User has read permissions on Oldfile: No\n";

        if(statbuff.st_mode & S_IWUSR)
        cout<<"User has write permission on Oldfile: Yes\n";
        else
        cout<<"User has write permission on Oldfile: No\n";

        if(statbuff.st_mode & S_IXUSR)
        cout<<"User has execute permission on Oldfile: Yes\n";
        else
        cout<<"User has execute permission on Oldfile: No\n";
    
        //Group permissions
        if(statbuff.st_mode & S_IRGRP)
        cout<<"Group has read permissions on Oldfile: Yes\n";
        else
        cout<<"Group has read permissions on Oldfile: No\n";

        if(statbuff.st_mode & S_IWGRP)
        cout<<"Group has write permission on Oldfile: Yes\n";
        else
        cout<<"Group has write permission on Oldfile: No\n";

        if(statbuff.st_mode & S_IXGRP)
        cout<<"Group has execute permission on Oldfile: Yes\n";
        else
        cout<<"UGroup has execute permission on Oldfile: No\n";
        
        //Other permissions

        if(statbuff.st_mode & S_IROTH)
        cout<<"Others has read permissions on Oldfile: Yes\n";
        else
        cout<<"Others has read permissions on Oldfile: No\n";

        if(statbuff.st_mode & S_IWOTH)
        cout<<"Others has write permission on Oldfile: Yes\n";
        else
        cout<<"Others has write permission on Oldfile: No\n";

        if(statbuff.st_mode & S_IXOTH)
        cout<<"Others has execute permission on Oldfile: Yes\n";
        else
        cout<<"Others has execute permission on Oldfile: No\n";
        
    }
    cout<<endl;
    //checking permissions of Directory
    if(stat(dir_name, &statbuff)==0){
        //check if it is a file or directory
        if(!S_ISDIR(statbuff.st_mode)){
            perror("Old file isn't a Directory");
            return 0;
        }
        //User permissions
        if(statbuff.st_mode & S_IRUSR)
        cout<<"User has read permissions on Directory: Yes\n";
        else
        cout<<"User has read permissions on Directory: No\n";

        if(statbuff.st_mode & S_IWUSR)
        cout<<"User has write permission on Directory: Yes\n";
        else
        cout<<"User has write permission on Directory: No\n";

        if(statbuff.st_mode & S_IXUSR)
        cout<<"User has execute permission on Directory: Yes\n";
        else
        cout<<"User has execute permission on Directory: No\n";

        //Group permissions
        if(statbuff.st_mode & S_IRGRP)
        cout<<"Group has read permissions on Directory: Yes\n";
        else
        cout<<"Group has read permissions on Directory: No\n";

        if(statbuff.st_mode & S_IWGRP)
        cout<<"Group has write permission on Directory: Yes\n";
        else
        cout<<"Group has write permission on Directory: No\n";

        if(statbuff.st_mode & S_IXGRP)
        cout<<"Group has execute permission on Directory: Yes\n";
        else
        cout<<"UGroup has execute permission on Directory: No\n";
        
        //Other permissions

        if(statbuff.st_mode & S_IROTH)
        cout<<"Others has read permissions on Directory: Yes\n";
        else
        cout<<"Others has read permissions on Directory: No\n";

        if(statbuff.st_mode & S_IWOTH)
        cout<<"Others has write permission on Directory: Yes\n";
        else
        cout<<"Others has write permission on Directory: No\n";

        if(statbuff.st_mode & S_IXOTH)
        cout<<"Others has execute permission on Directory: Yes\n";
        else
        cout<<"Others has execute permission on Directory: No";
    }
    return 0;
}