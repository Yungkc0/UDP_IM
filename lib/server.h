#ifndef SERVCLI_H
#	define SERVCLI_H

/* 404 */
void notfound(int fd, char *buf, char *time);

/* get filepath in HTTP request */
int getpath(const char *req, char *path);

/* TCP_listen function */
int tcp_listen(const char *port);

void w_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

/* fsize function */
off_t fsize(const char *filename);

/* convert local time to RFC1123 format */
void rfctime(char *s, const struct tm *tm);

#endif
