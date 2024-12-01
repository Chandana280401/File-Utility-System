#include<unistd.h>
#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 
#include<cstdio>


using namespace std;


void reverseContentflag1(const char* inp_file_name,int out_file_dsp,int st_idx,int ed_idx){


    if(st_idx>ed_idx){
        perror("Invalid Indexes: start index cannot be greater than end index");
        return;
    }
    if(st_idx<0 || ed_idx <0)
    {
        perror("Reading bad address: Invalid indexes");
        return;
    }
    
    int inp_file_dsp=open(inp_file_name, O_RDONLY,0400);
  
    if(inp_file_dsp==-1){
       
        perror("Error while opening input file");
        return;
    }
    //calculate the size of input file
    
    off_t inp_file_size= lseek(inp_file_dsp,0,SEEK_END); //after this, fd is pointing to file end

    if(st_idx > inp_file_size || ed_idx >inp_file_size){
        perror("Index out of bounds");
        return;
    }

    if(inp_file_size==0){  //handling empty file
        cout<<"Progress:100%";
        return;
    }
    
    if(st_idx >= inp_file_size || ed_idx >= inp_file_size){
        perror("Index out of bounds");
        return;
    }

    off_t inp_size1=st_idx;
    off_t inp_size2=ed_idx-inp_size1+1;
    off_t inp_size3=inp_file_size-(inp_size1+inp_size2);

    
  
    int percent;
    int buff_size;
    if(inp_size1<100000)
        buff_size=inp_size1 >0 ? inp_size1 : inp_size2>0 ?inp_size2: inp_size3;
    else
        buff_size=(inp_size1)/10000; //dividing to 1000 chunks

    char read_buff[buff_size];
   

    off_t read_offset=st_idx-buff_size;
    ssize_t idx;

    //Reversing the first part
    ssize_t bytes_written1=0;
    while(read_offset>=0 && inp_size1>0){

            idx=lseek(inp_file_dsp,(read_offset),SEEK_SET);
            ssize_t n=read(inp_file_dsp,read_buff,buff_size);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
           
            //reversing the content in read_buff

            char temp;
            for(ssize_t i=0;i<n/2;i++){
                temp=read_buff[i];
                read_buff[i]=read_buff[n-i-1];
                read_buff[n-i-1]=temp;
            }

            ssize_t bytes_written= write(out_file_dsp,read_buff,n);
            bytes_written1+=bytes_written;
            
            if(bytes_written== -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            
            read_offset-=buff_size;
            percent=double(bytes_written1)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;
    }
    if(bytes_written1<inp_size1 && inp_size1>0)
    {
        lseek(inp_file_dsp,0,SEEK_SET);
        ssize_t n=read(inp_file_dsp,read_buff,idx);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
            //reversing the content in read_buff
            char temp;
            for(ssize_t i=0;i<n/2;i++){
                temp=read_buff[i];
                read_buff[i]=read_buff[n-i-1];
                read_buff[n-i-1]=temp;
            }
            read_buff[n]='\0';
            ssize_t bytes_written= write(out_file_dsp,read_buff,n);
            bytes_written1+=bytes_written;
           
            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            percent=double(bytes_written1)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;

    }

    //Copying the Range as it is
    read_offset=st_idx;

    ssize_t bytes_written2=bytes_written1;
    while(ed_idx-read_offset>=buff_size && inp_size2>0){

            idx=lseek(inp_file_dsp,read_offset,SEEK_SET);
            ssize_t n=read(inp_file_dsp,read_buff,buff_size);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
           //writing content as it is
            ssize_t bytes_written= write(out_file_dsp,read_buff,n);
            bytes_written2+=bytes_written;

            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
    
            read_offset+=buff_size;
            percent=double(bytes_written2)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;
    }
    if(bytes_written2<(inp_size1+inp_size2) && inp_size2>0)
    {
        lseek(inp_file_dsp,read_offset,SEEK_SET);
        ssize_t n=read(inp_file_dsp,read_buff,ed_idx-read_offset+1);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
            read_buff[n]='\0';
            ssize_t bytes_written = write(out_file_dsp,read_buff,n);
            bytes_written2+=bytes_written;
          
            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            percent=double(bytes_written2)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;

    }
    //reversing the last part
    ssize_t bytes_written3=bytes_written2;
    read_offset=inp_file_size;
    while(read_offset-ed_idx-1>= buff_size && inp_size3>0){

            read_offset-=buff_size;
            idx=lseek(inp_file_dsp,read_offset,SEEK_SET);
        
            ssize_t n=read(inp_file_dsp,read_buff,buff_size);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
            //reversing the content in read_buff
            char temp;
            for(ssize_t i=0;i<n/2;i++){
                temp=read_buff[i];
                read_buff[i]=read_buff[n-i-1];
                read_buff[n-i-1]=temp;
            }

            ssize_t bytes_written = write(out_file_dsp,read_buff,n);
           bytes_written3+=bytes_written;
            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            percent=double(bytes_written3)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;
    }
    if(bytes_written3<inp_file_size && inp_size3>0)
    {
        lseek(inp_file_dsp,ed_idx+1,SEEK_SET);
        ssize_t n=read(inp_file_dsp,read_buff,read_offset-ed_idx-1);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
            //reversing the content in read_buff
            char temp;
            for(ssize_t i=0;i<n/2;i++){
                temp=read_buff[i];
                read_buff[i]=read_buff[n-i-1];
                read_buff[n-i-1]=temp;
            }

            read_buff[n]='\0';
            ssize_t bytes_written = write(out_file_dsp,read_buff,n);
            bytes_written3+=bytes_written;
            
            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            percent=double(bytes_written3)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;
    }
    if(close(inp_file_dsp)==-1){
        write(1,"Error while closing input file",30);
        return;
    }
    if(close(out_file_dsp)==-1)
    {
        write(1,"Error while closing output file",31);
        return;
    }
}

void reverseContentflag0(const char* inp_file_name,int out_file_dsp){

    int inp_file_dsp=open(inp_file_name, O_RDONLY,0400);

    if(inp_file_dsp==-1){
        perror("Error while opening input file");
        return;
    }
    int percent;

    //calculate the size of input file
    off_t inp_file_size= lseek(inp_file_dsp,0,SEEK_END); //after this, fd is pointing to file end
    if(inp_file_size==0){
        cout<<"Progress:100%";
        return;
    }
    int buff_size;
    if(inp_file_size<100000)
        buff_size=inp_file_size;
    else
        buff_size=inp_file_size/1000; //lets say we are diving into 1000 chunks
    
    char read_buff[buff_size];
    off_t read_offset=inp_file_size-buff_size;
    ssize_t idx;
    ssize_t bytes_written=0;
    while(read_offset>=0){

            idx=lseek(inp_file_dsp,read_offset,SEEK_SET);
            ssize_t n=read(inp_file_dsp,read_buff,buff_size);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
            //reversing the content in read_buff
            char temp;
            for(ssize_t i=0;i<n/2;i++){
                temp=read_buff[i];
                read_buff[i]=read_buff[n-i-1];
                read_buff[n-i-1]=temp;
            }

            bytes_written+= write(out_file_dsp,read_buff,n);
            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            read_offset-=buff_size;
            percent=double(bytes_written)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;
    }
    if(bytes_written<inp_file_size)
    {
        lseek(inp_file_dsp,0,SEEK_SET);
        ssize_t n=read(inp_file_dsp,read_buff,idx);
            if(n==-1){
                perror("Error while reading input file");
                close(inp_file_dsp);
                return;
            }
            //reversing the content in read_buff
            char temp;
            for(ssize_t i=0;i<n/2;i++){
                temp=read_buff[i];
                read_buff[i]=read_buff[n-i-1];
                read_buff[n-i-1]=temp;
            }
            bytes_written+= write(out_file_dsp,read_buff,n);
            
            if(bytes_written == -1){
                perror("Error while writting to output file");
                close(out_file_dsp);
                close(inp_file_dsp);
                return;
            }
            percent=double(bytes_written)/double(inp_file_size)*100;
            cout<<"\rProgress:"<<percent<<"%"<<flush;
    }
    if(close(inp_file_dsp)==-1){
        write(1,"Error while closing input file",30);
        return;
    }
    if(close(out_file_dsp)==-1)
    {
        write(1,"Error while closing output file",31);
        return;
    }
}

int main(int argc,char *argv[]){
    const char* dir_name="Assignment1";
    const char* input_file_path=argv[1];

    struct stat statbuff;
    if(!(stat(input_file_path,&statbuff)==0 && S_ISREG(statbuff.st_mode))){
        perror("Input file not found");
        return 1;
    }
  
    //Directory Creation
    int return_value=mkdir(dir_name,0700); //0700->assigning rwx permissions to user and others no permissions
  
    int slashidx=-1,i=0;
   
    string inp_file_path=input_file_path;
    while(inp_file_path[i]!='\0'){
        if(inp_file_path[i]=='/'){
            slashidx=i;
        }
        i++;
    }
    string inp_file_name="";
    i=slashidx+1;
    while(inp_file_path[i]!='\0'){
        inp_file_name+=inp_file_path[i];
        i++;
    }
    string flag;
    if(argc==3)
        flag="/0_";
    else if(argc==5)
        flag="/1_";

    string out_file=dir_name+flag+inp_file_name;
    const char* output_file_name=out_file.c_str();


    
    int out_file_dsp=open(output_file_name,O_CREAT | O_RDWR | O_TRUNC , 0600);//user has read and write permissions

    if(out_file_dsp==-1){
        perror("Error creating output file");
    }
    
    
    //Calling the function to perform reverse action
    if(argc==3)// This means flag=0
        reverseContentflag0(input_file_path,out_file_dsp);
    else if(argc==5){ //This means flag=1, so pass the [start,end] index of the content to be reversec
        int start_idx=stoi(argv[3]);
        int end_idx=stoi(argv[4]);
        reverseContentflag1(input_file_path,out_file_dsp,start_idx,end_idx);
    }
    return 0; 
}
