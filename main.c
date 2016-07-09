#include "lib/http.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int main(int argc, char **argv)
{
	int listenfd, connfd;
	void *webchild(void *);
	pthread_t tid;
	socklen_t addrlen;
	struct sockaddr_in cliaddr;

	if (argc == 2)
		listenfd = tcp_listen(argv[1]);
	else if (argc == 1)
		listenfd = tcp_listen(SERVPORT);
	else
		err_quit("usage: serv [port]");

	if (pthread_mutex_init(&mutex, NULL) != 0)
		err_sys("pthread_mutex_init error");

	for (; ; ) {
		if ((connfd = accept(listenfd, (SA *) &cliaddr, &addrlen)) < 0)
			err_sys("accept error");
		if (pthread_create(&tid, NULL, webchild, (void *) &connfd) != 0)
			err_sys("pthread_create error");
	}
}

void *webchild(void *args)
{
	int fd, maxfdp1;
	off_t flen;
	time_t t;
	char buf[MAXLINE], timebuf[TIMEBUF];
	FILE *fp;
	fd_set rset;
	struct timeval tvl = {0, 20};

	fp = fopen("www/index.html", "r");
	fd = *(int *) args;
	FD_ZERO(&rset);
	FD_SET(fd, &rset);
	maxfdp1 = fd + 1;

	w_select(maxfdp1, &rset, NULL, NULL, &tvl);
	if (FD_ISSET(fd, &rset)) {
		read(fd, buf, MAXLINE);
		printf("%s", buf);
	}
	t = time(NULL);
	rfctime(timebuf, localtime(&t));
	if ((flen = fsize("www/index.html")) == -1)
		err_quit("get index.html error");
	sprintf(buf, "HTTP/1.1 200 OK\r\n"
			"Date: %s\r\n"
			"Server: Elder\r\n"
			"Content-length: %lu\r\n"
			"Vary: Accept-Encoding\r\n"
			"Connection: close\r\n"
			"Content-Type: text/html\r\n\r\n",
			timebuf, flen);
	write(fd, buf, strlen(buf));
	while (fgets(buf, MAXLINE, fp) != NULL)
		write(fd, buf, strlen(buf));
	close(fd);

	return args;
}
