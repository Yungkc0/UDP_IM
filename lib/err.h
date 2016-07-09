#ifndef IMERR_H
# define IMERR_H

void err_ret(const char *fmt, ...);

void err_sys(const char *fmt, ...);

void err_dump(const char *fmt, ...);

void err_msg(const char *fmt, ...);

void err_quit(const char *fmt, ...);

#endif
