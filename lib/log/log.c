// Copyright © 2011 Steve McCoy and Ethan Burns
// Licensed under the MIT License. See LICENSE for details.
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

enum { Bufsz = 256 };

static FILE *lfile;

int loginit(const char *fname){
	if(!fname){
		lfile = stderr;
		return 0;
	}

	lfile = fopen(fname, "w+");
	if(!lfile)
		return errno;
	return 0;
}

void logclose(void){
	if(lfile != stderr)
		fclose(lfile);
}

void prtime()
{
	char str[Bufsz];
	struct tm tm;
	time_t t = time(NULL);
	if (t == ((time_t) -1)) {
		perror("time");
		fprintf(stderr, "%s failed: time failed\n", __func__);
		abort();
	}
	if (!localtime_r(&t, &tm)) {
		fprintf(stderr, "%s failed: localtime failed\n", __func__);
		abort();
	}
	if (!strftime(str, Bufsz - 1, "[%T]", &tm)) {
		fprintf(stderr, "%s failed: strftime failed\n", __func__);
		abort();
	}
	fprintf(lfile, "%s ", str);
}


void prraw(const char *func, int err, const char *fmt, va_list ap)
{
	bool fname = func[0] != '\0';

	prtime();

	if (fname)
		fprintf(lfile, "%s: ", func);

	vfprintf(lfile, fmt, ap);

	if (err == 0)
		goto done;

	char str[Bufsz];
	if (strerror_r(err, str, Bufsz) == 0) {
		fprintf(lfile, "%s" , str);
	} else {
		perror("strerror_r");
		fprintf(stderr, "perr failed\n");
		abort();
	}
done:
	fprintf(lfile, "\n");
}

void pr(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	prraw("", 0, fmt, ap);
	va_end(ap);
}

void prerr(int err, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	prraw("", err, fmt, ap);
	va_end(ap);
}

void prfn(const char *func, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	prraw(func, 0, fmt, ap);
	va_end(ap);
}

void prfnerr(const char *func, int err, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	prraw(func, err, fmt, ap);
	va_end(ap);
}

void flushlog()
{
	fflush(lfile);
}
