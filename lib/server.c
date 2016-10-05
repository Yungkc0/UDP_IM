#include "http.h"

/* get file suffix */
static void getsuffix(const char *path, char *suffix)
{
	size_t size;

	size = strlen(path);
	for (; size > 0; --size) {
		if (path[size - 1] == '.') {
			strcpy(suffix, path + size - 1);
			return;
		}
	}
	strcpy(suffix, "*");
}

/* get content-type of file */
void getype(const char *path, char *type)
{
	int i;
	char suffix[20];
	static char *typelist[][2] = {
		{"*", "application/octet-stream"},
		{".html", "text/html"},
		{".htm", "text/html"},
		{".htx", "text/html"},
		{".avi", "video/avi"},
		{".bmp", "application/x-bmp"},
		{".doc", "application/msword"},
		{".dot", "application/msword"},
		{".exe", "application/x-msdownload"},
		{".fo", "text/xml"},
		{".ico", "image/x-icon"},
		{".img", "application/x-img"},
		{".java", "java/*"},
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".jsp", "text/html"},
		{".mp2", "audio/mp2"},
		{".mp3", "auido/mp3"},
		{".mp4", "video/mpeg4"},
		{".mpeg", "video/mpg"},
		{".pdf", "application/pdf"},
		{".png", "image/png"},
		{".xhtml", "text/html"},
		{".css", "text/css"},
		{".xml", "text/xml"},
		{".txt", "text/plain"},
		{".js", "text/javascript"},
		{".", "x-"},
		{NULL, NULL},
	};

	getsuffix(path, suffix);
	type[0] = 0;
	for (i = 0; typelist[i][0] != NULL; ++i) {
		if (!strcmp(typelist[i][0], suffix))
			strcpy(type, typelist[i][1]);
	}
	if (!type[0])
		strcpy(type, typelist[0][1]);
}

/* get filepath in HTTP request */
FILE *getpath(const char *req, char *path)
{
	FILE *fp;

	while (*++req != '/')
		;
	strcpy(path, "www");
	sscanf(req, "%s", path + 3);
	if (*req == '/' && *(req + 1) == ' ')
		strcpy(path, "www/index.html");
	if ((fp = fopen(path, "r")) == NULL)
		;
	return fp;
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

/* wrapped functions */
void w_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	if (select(nfds, readfds, writefds, exceptfds, timeout) < 0)
		err_sys("select error");
}

ssize_t w_write(int fd, const char *buf, size_t n)
{
	ssize_t nwrite;

	while (1) {
		if ((nwrite = write(fd, buf, n)) < 0) {
			if (errno == EINTR) {
				nwrite = 0;
			} else if (errno == EPIPE) {
                break;
            } else {
				err_sys("w_write error");
				return -1;
			}
		} else {
			break;
		}
	}
	return nwrite;
}

ssize_t w_read(int fd, char *buf, size_t n)
{
	ssize_t nread;

	while (1) {
		if ((nread = read(fd, buf, n)) < 0) {
			if (errno == EINTR) {
				nread = 0;
			} else {
				perror("w_read error");
				return -1;
			}
		} else {
			break;
		}
	}
	return nread;
}
