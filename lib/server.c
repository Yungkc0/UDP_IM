#include "http.h"

/* 404 */
void notfound(int fd, char *buf, char *time)
{
	FILE *fp;

	fp = fopen("www/404.html", "r");
	sprintf(buf, "HTTP/1.1 404 Not Found\r\n"
			"Date: %s\r\n"
			"Server: Elder/1.0\r\n"
			"Content-Length: %lu\r\n"
			"Keep-Alive: timeout=5, max=98\r\n"
			"Connection: Keep-Alive\r\n"
			"Content-Type: text/html\r\n\r\n",
			time, fsize("www/404.html"));
	write(fd, buf, strlen(buf));
	while (fgets(buf, MAXLINE, fp) != NULL)
		write(fd, buf, strlen(buf));
	fclose(fp);
}

/* get filepath in HTTP request */
int getpath(const char *req, char *path)
{
	while (*++req != '/')
		;
	strcpy(path, "www");
	sscanf(req, "%s", path + 3);
	if (*req == '/' && *(req + 1) == ' ')
		strcpy(path, "www/index.html");
	return 0;
}

/* TCP_listen function */
int
tcp_listen(const char *port)
{
	int listenfd;
	const int on = 1;
	struct sockaddr_in servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		err_sys("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(port));

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		err_sys("setsockopt error");
	if (bind(listenfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("bind error");
	
	if (listen(listenfd, LISTENQ) < 0)
		err_sys("listen error");

	return listenfd;
}

/* select() */
void w_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	if (select(nfds, readfds, writefds, exceptfds, timeout) < 0)
		err_sys("select error");
}

/* fsize function */
off_t fsize(const char *filename)
{
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	return -1;
}

/* convert local time to RFC1123 format */
void rfctime(char *s, const struct tm *tm)
{
	static char *days[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static char *months[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
	};
	snprintf(s, TIMEBUF, "%s, %02d %s %04d %02d:%02d:%02d GMT",
			days[tm->tm_wday], tm->tm_mday, months[tm->tm_mon - 1],
			tm->tm_year, tm->tm_hour, tm->tm_min, tm->tm_sec);
}
