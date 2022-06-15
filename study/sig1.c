#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>

void sig_hnd(int sig){ (void)sig; printf("(VINTR)"); }

int main(){
  setvbuf(stdout,NULL,_IONBF,0);

  struct termios old_termios, new_termios;
  tcgetattr(0,&old_termios);

  signal( SIGINT, sig_hnd );

  new_termios             = old_termios;
  new_termios.c_cc[VEOF]  = 3; // ^C
  new_termios.c_cc[VINTR] = 4; // ^D
  tcsetattr(0,TCSANOW,&new_termios);

  char line[256]; int len;
  do{
    len=read(0,line,256); line[len]='\0';
    if( len <0 ) printf("(len: %i)",len);
    if( len==0 ) printf("(VEOF)");
    if( len >0 ){
      if( line[len-1] == 10 ) printf("(line:'%.*s')\n",len-1,line);
      if( line[len-1] != 10 ) printf("(partial line:'%s')",line);
    }
  }while( line[0] != 'q' );

  tcsetattr(0,TCSANOW,&old_termios);
}
