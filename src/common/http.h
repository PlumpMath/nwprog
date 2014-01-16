#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

struct http;

#define HTTP_VERSION "HTTP/1.0"

/*
 * Create a new HTTP connect using the given socket.
 */
int http_create (struct http **httpp, int sock);

/*
 * Send a HTTP request.
 */
int http_write_request_start (struct http *http, const char *method, const char *path);
int http_write_request_start_path (struct http *http, const char *method, const char *fmt, ...);
int http_write_request_header (struct http *http, const char *header, const char *value);
int http_write_request_headerf (struct http *http, const char *header, const char *fmt, ...);

/*
 * End the HTTP headers.
 *
 * XXX: rename
 */
int http_write_request_end (struct http *http);

/*
 * Send a HTTP request body.
 *
 * The available data in the given buffer is passed in as *lenp, and the number of bytes sent out is returned in *lenp.
 *
 * Returns 1 on EOF, <0 on error.
 */
int http_write_request_body (struct http *http, char *buf, size_t *lenp);




/*
 * Read a HTTP request.
 */
int http_read_request (struct http *http, const char **methodp, const char **pathp, const char **versionp);

/*
 * Read a HTTP response.
 */
int http_read_response (struct http *http, const char **versionp, unsigned *statusp, const char **reasonp);

/*
 * Read next header as {*headerp}: {*valuep}.
 *
 * In case of a folded header, *headerp is left as-is.
 *
 * Returns 1 on end-of-headers, 0 on header, <0 on error.
 */
int http_read_header (struct http *http, const char **headerp, const char **valuep);

/*
 * Read (part of) the response body.
 *
 * The size of the given buffer is passed in *lenp, and the number of bytes read in returned in *lenp.
 *
 * Returns 1 on EOF, <0 on error.
 */
int http_read_body (struct http *http, char *buf, size_t *lenp);






/*
 * Release all associated resources.
 */
void http_destroy (struct http *http);

#endif
