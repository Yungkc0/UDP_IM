#include "http.h"

static void s_404(FILE *, int, char *, char *);

/* execute status function */
void status(int code, int fd, char *buf, char *timebuf)
{
    FILE *fp;

    fp = NULL;
	switch(code) {
		case 404:
			fp = fopen("www/status/404.html", "r");
            if (fp == NULL)
                err_sys("fopen error");
			s_404(fp, fd, buf, timebuf);
			break;
		default:
			;
	}
    if (fp)
        fclose(fp);
}

static void s_404(FILE *fp, int fd, char *buf, char *timebuf)
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
	while (fgets(buf, MAXLINE, fp) != NULL)
		w_write(fd, buf, strlen(buf));

	close(fd);
}
