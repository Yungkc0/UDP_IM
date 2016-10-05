#include "http.h"

static void s_404(int, int, char *, char *);

/* execute status function */
void status(int code, int fd, char *buf, char *timebuf)
{
    int sfd;

	switch(code) {
		case 404:
			sfd = open("www/status/404.html", O_RDONLY);
			s_404(sfd, fd, buf, timebuf);
			break;
		default:
			;
	}
}

static void s_404(int sfd, int fd, char *buf, char *timebuf)
{
	sprintf(buf, "HTTP/1.1 404 Not Found\r\n"
			"Date: %s\r\n"
			"Server: Elder/%s\r\n"
			"Content-Length: %lu\r\n"
			"Keep-Alive: close\r\n"
			"Connection: close\r\n"
			"Content-Type: text/html\r\n\r\n",
			timebuf, VERSION, fsize("www/404.html"));
	w_write(fd, buf, strlen(buf));
	while (w_read(sfd, buf, MAXLINE) > 0)
		w_write(fd, buf, strlen(buf));
    close(sfd);
	close(fd);
}
