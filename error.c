#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "error.h"
#include "spin.h"

static volatile spin_lock msg_lock;
static char prog_name[256], last_msg[512], saved_msg[512];
static int last_code, saved_code, saved_errno;
static boolean with_ts;

const char *error_get_program_name(void)
{
	return prog_name;
}

void error_set_program_name(const char *name)
{
	const char *slash = strrchr(name, '/');
	if (slash)
		name = slash + 1;

	strncpy(prog_name, name, sizeof(prog_name) - 1);
	prog_name[sizeof(prog_name) - 1] = '\0';
}

boolean error_with_timestamp(boolean with_timestamp)
{
	boolean old = with_ts;
	with_ts = with_timestamp;
	return old;
}

int error_last_code(void)
{
	return last_code;
}

const char *error_last_msg(void)
{
	return last_msg;
}

void error_save_last(void)
{
	if (FAILED(spin_write_lock(&msg_lock, NULL)))
		abort();

	if (last_code != 0) {
		saved_errno = errno;
		saved_code = last_code;
		strcpy(saved_msg, last_msg);

		error_reset();
	}

	spin_unlock(&msg_lock, 0);
}

void error_restore_last(void)
{
	if (FAILED(spin_write_lock(&msg_lock, NULL)))
		abort();

	if (saved_code != 0) {
		errno = saved_errno;
		last_code = saved_code;
		strcpy(last_msg, saved_msg);

		saved_code = 0;
		saved_msg[0] = '\0';
	}

	spin_unlock(&msg_lock, 0);
}

void error_reset(void)
{
	last_code = 0;
	last_msg[0] = '\0';
}

int error_msg(const char *msg, int code, ...)
{
	char buf[256];
	va_list ap;

	if (!msg || code >= 0) {
		error_invalid_arg("error_msg");
		error_report_fatal();
	}

	va_start(ap, code);
	vsprintf(buf, msg, ap);
	va_end(ap);

	if (FAILED(spin_write_lock(&msg_lock, NULL)))
		abort();

	last_code = code;
	last_msg[0] = '\0';

	if (with_ts) {
		struct timeval tv;
		struct tm *ptm;
		char fract[32];

		if (gettimeofday(&tv, NULL) == -1 ||
			!(ptm = localtime(&tv.tv_sec)) ||
			!strftime(last_msg, sizeof(last_msg) - 8,
					  "%Y/%m/%d %H:%M:%S", ptm) ||
			sprintf(fract, "%.6f ", tv.tv_usec / 1000000.0) < 0)
			abort();

		strcat(last_msg, fract + 1);
	}

	if (prog_name[0] != '\0') {
		strcat(last_msg, prog_name);
		strcat(last_msg, ": ");
	}

	strncat(last_msg, buf, sizeof(last_msg) - strlen(last_msg) - 1);

	spin_unlock(&msg_lock, 0);
	return code;
}

static int format(const char *func, const char *desc, int code)
{
	return error_msg("error: %s: %s", code, func, desc);
}

int error_eof(const char *func)
{
	if (!func) {
		error_invalid_arg("error_eof");
		error_report_fatal();
	}

	return format(func, "end of file", EOF + CACHESTER_ERROR_BASE);
}

int error_errno(const char *func)
{
	if (!func) {
		error_invalid_arg("error_errno");
		error_report_fatal();
	}

	return format(func, strerror(errno),
				  ERRNO_ERROR_BASE_1 - (errno < 128
										? errno : errno + ERRNO_ERROR_BASE_2));
}

int error_invalid_arg(const char *func)
{
	errno = EINVAL;
	return error_errno(func);
}

int error_unimplemented(const char *func)
{
	errno = ENOSYS;
	return error_errno(func);
}

void error_append_msg(const char *text)
{
	if (!text || (strlen(last_msg) + strlen(text)) >= sizeof(last_msg))
		abort();

	strcat(last_msg, text);
}

void error_report_fatal(void)
{
	if (fputs(last_msg, stderr) == EOF || fputc('\n', stderr) == EOF)
		abort();

	exit(-last_code);
}
