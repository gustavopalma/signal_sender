#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <iostream>


int main(int argc, char** argv)
{
   std::cout << "Porta selecionada: /dev/serial0" << std::endl;

    int serial = open("/dev/serial0", O_RDWR| O_NOCTTY );

    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if ( tcgetattr ( serial, &tty ) != 0 ) 
    {
        std::cout << "Erro: " << errno << " ao recuperar parâmetros da porta serial: " << strerror(errno) << std::endl;
    }   

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)B115200);
    cfsetispeed (&tty, (speed_t)B115200);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( serial, TCIFLUSH );
    if ( tcsetattr ( serial, TCSANOW, &tty ) != 0) 
    {
        std::cout << "Erro " << errno << " ao Configurar parâmetros da porta serial" << std::endl;
    }   

    char cmd[] = "FINISHED \r\n";
    int n_written = 0;
    int len = strlen(cmd);
    do {
        n_written = write( serial, &cmd, len );
    } while (n_written != len);

    return 0;
}
