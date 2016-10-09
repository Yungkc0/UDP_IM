#include "lib/http.h"

void *webchild(void *);
void sigpipe(int);
int main(int argc, char **argv)
{
	int listenfd, connfd;
    pthread_t tid;
	socklen_t addrlen;
	struct sockaddr_in cliaddr;
    struct sigaction sa;
    char peeraddr[30];

	if (argc == 2)
		listenfd = tcp_listen(argv[1]);
	else if (argc == 1)
		listenfd = tcp_listen(SERVPORT);
	else
		err_quit("usage: http [port]");

    sa.sa_flags = 0;
    sa.sa_handler = sigpipe;
    sigaction(SIGPIPE, &sa, NULL);
    addrlen = sizeof(struct sockaddr_in);
	for (; ; ) {
		if ((connfd = accept(listenfd, (SA *) &cliaddr, &addrlen)) < 0)
			err_sys("accept error");
        printf("from %s:%hu\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, peeraddr, sizeof(peeraddr)), ntohs(cliaddr.sin_port));
        pthread_create(&tid, NULL, webchild, (void *) connfd);
        pthread_detach(tid);
	}
}

void sigpipe(int signo)
{
    return;
}

void *webchild(void *args)
{
    int fd;
	size_t nbytes;
	off_t flen;
	time_t t;
	char buf[MAXLINE], timebuf[TIMEBUF];
	char filepath[PATHMAX], type[50];
	FILE *fp;

    fd = (int) args;
	memset(buf, 0, MAXLINE);
	w_read(fd, buf, MAXLINE);
	t = time(NULL);      /* time for 'Date' header */
	rfctime(timebuf, localtime(&t));
	if ((fp = getpath(buf, filepath)) == NULL) {
		if (errno == ENOENT) {
			status(404, fd, buf, timebuf);
			return NULL;
		}
	}

	getype(filepath, type);
	if ((flen = fsize(filepath)) == -1)
		err_quit("fsize error");
	sprintf(buf, "HTTP/1.1 200 OK\r\n"
		"Date: %s\r\n"
		"Server: Elder/%s\r\n"
		"Content-length: %lu\r\n"
		"Vary: Accept-Encoding\r\n"
		"Connection: close\r\n"
		"Content-Type: %s\r\n\r\n",
		timebuf, VERSION, flen, type);
	w_write(fd, buf, strlen(buf));
	while ((nbytes = fread(buf, 1, MAXLINE, fp)) != 0)
		w_write(fd, buf, nbytes);

	fclose(fp);
	close(fd);

    return NULL;
}
