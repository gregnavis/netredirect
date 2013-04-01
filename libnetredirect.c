/* The definition below is required to define RTLD_NEXT by including dlfcn.h */
#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <dlfcn.h>


typedef int (*connect_function)(int, const struct sockaddr *, socklen_t);


static const char *host_variable = "NETREDIRECT_HOST";
static const char *port_variable = "NETREDIRECT_PORT";


static int is_redirectable(int sockfd, struct sockaddr_in *sin)
{
	int result, type;
	socklen_t type_length = sizeof(type);

	if (AF_INET != sin->sin_family) {
		goto out0;
	}

	if (getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &type, &type_length)) {
		goto out0;
	}

	if (SOCK_STREAM != type) {
		goto out0;
	}

	result = 1;

out:
	return result;

out0:
	result = 0;
	goto out;
}

static void redirect_host(int sockfd, struct sockaddr_in *sin)
{
	char *host = getenv(host_variable);

	if (!host) {
		goto out;
	}

	if (!inet_aton(host, &sin->sin_addr)) {
		goto out;
	}

out:
	return;
}

static void redirect_port(int sockfd, struct sockaddr_in *sin)
{
	char *port = getenv(port_variable);

	if (!port) {
		goto out;
	}

	sin->sin_port = htons(atoi(port));

out:
	return;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	connect_function system_connect = dlsym(RTLD_NEXT, "connect");
	struct sockaddr addr_copy;
	struct sockaddr_in *sin = (struct sockaddr_in *) &addr_copy;

	memcpy(&addr_copy, addr, sizeof(addr_copy));

	if (!is_redirectable(sockfd, sin)) {
		goto out;
	}

	redirect_host(sockfd, sin);
	redirect_port(sockfd, sin);

out:
	return system_connect(sockfd, &addr_copy, addrlen);
}
