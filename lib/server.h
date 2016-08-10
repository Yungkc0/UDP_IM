#ifndef SERVER_H
# define SERVER_H

/* get content-type of file */
void getype(const char *path, char *type);

/* get filepath in HTTP request */
FILE *getpath(const char *req, char *path);

/* fsize function */
off_t fsize(const char *filename);

/* convert local time to RFC1123 format */
void rfctime(char *s, const struct tm *tm);

/* TCP_listen function */
int tcp_listen(const char *port);

/* wrapped functions */
void w_read(int fd, char *buf, size_t n);
void w_write(int fd, const char *buf, size_t n);
void w_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#endif
