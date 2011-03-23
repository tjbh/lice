/* sockcheck.c By BigDawg dawg@nuthin.nu - [ http://www.rootshell.com/ ]
 * gatecheck.c By BigDawg - [ http://www.rootshell.com/ ]
 * Compile: gcc sockcheck.c -o sockcheck
 * 
 * DINOS AND CHARON AND MOURGOS AND DOCTORX: THANK YOU -HS
 * 
 * I'd like to say thanks to all who have helped me throughout the past years.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <arpa/inet.h>
extern int hexstr;
static int sockfd;
static int sockfd2;
#define socknull 5
#define socktimeout 8
#define longtime 10
#undef SKIP_GR
int flag;

static char sockip[1024],a[1024],b[1024],c[1024],d[1024];

void sigalrm_handler(int sig) {
	close(sockfd2);
	if (flag == 9) {
		printf("Y check timeout %d seconds exceeded\n", socktimeout);
	}
	else
	printf("N %s %s %s %s Do NOT attempt to grow a Brain! (%s)\n", a,b,c,d,sockip);
	exit(9);
}

int main(int argc, char *argv[])
{
//	char sockip[1024];
//	char a[1024],b[1024],c[1024],d[1024];
	if (argc < 3) {
		fprintf(stderr, "USAGE: sscan ident host [args]\n");
		return 1;
	}
	strcpy(sockip, argv[2]);
	if (argc >= 3) {
		strcpy(a, "");
		strcpy(b, "");
		strcpy(c, "");
		strcpy(d, "");
	}
	if (argc >= 4) strcpy(a, argv[3]);
	if (argc >= 5) strcpy(b, argv[4]);
	if (argc >= 6) strcpy(c, argv[5]);
	if (argc >= 7) strcpy(d, argv[6]);
#ifdef SKIP_GR
	if ((sockip[strlen(sockip)-2] == 103) && (sockip[strlen(sockip)-1] == 114) ) {
		printf("Y this is GR domain, skipping check\n");
		exit(0);
	}
#endif
	signal(SIGALRM, sigalrm_handler);
	flag = 9;
	alarm(longtime);
	sockcheck(sockip,a,b,c,d);
	printf("Y %s\n", sockip);
}

int sockcheck(char *host, char *ba, char *bb, char *bc, char *bd)
{
	int s = 1000;
	int c, d, r, e, f;
	int i = 0;
	char keystring[] = { 0x04, 0x01, 0x1A, 0x0B, 0xC7,
		0x02, 0xD2, 0xF1, 0x62, 0x6C,
		0x61, 0x68, 0x74, 0x65, 0x73, 0x74, 0x00
	};
	char hexstr[20];
	char tmpstr2[20] = "\0\0\0\0\0\0";
	char buf[20];
	fd_set gateset;
	struct timeval tv;
	struct in_addr MyHostAddr;
	struct hostent *he;
	struct sockaddr_in sin;
	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1080);
	sin.sin_addr.s_addr = inet_addr(host);
	if (sin.sin_addr.s_addr == INADDR_NONE) {
		he = gethostbyname(host);
		if (!he) {
			close(sockfd2);
			printf("Y %s was not an open socks server(1)\n",
			       host);
			exit(1);
		}
		memcpy(&sin.sin_addr, he->h_addr, he->h_length);
	}
	signal(SIGALRM, sigalrm_handler);
	flag = 9;
	alarm(longtime);
	e = connect(sockfd2, (struct sockaddr *) &sin, sizeof(sin));
	if (e == -1) {
		close(sockfd2);
		printf("Y %s was not an open socks server (2)\n",host);
		exit(2);
	}
	if (e == 0) {
		signal(SIGALRM, sigalrm_handler);
		flag = 3;
		alarm(socknull);

		FD_ZERO(&gateset);
		FD_SET(sockfd2, &gateset);
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		d = select(sockfd2 + 1, NULL, &gateset, NULL, &tv);
		if (d == 0) {
			close(sockfd2);
			printf("Y %s was not an open socks server (3)\n", host);
			exit(3);
		}
		send(sockfd2, keystring, 17, 0);
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		f = select(sockfd2 + 2, &gateset, NULL, NULL, &tv);
		if (f) {
			read(sockfd2, tmpstr2, 10);
			strcpy(hexstr, "HEX:");
			sprintf(buf, "%d ", tmpstr2[i]);
			strcat(hexstr, buf);
			if ((tmpstr2[0] == 0) && (tmpstr2[1] == 90) /*&& (tmpstr2[2] == 0)*/) {
				close(sockfd2);
				printf("N %s %s %s %s Don't IRC from open socks4+ servers (%s)\n", ba,bb,bc,bd,host);
				exit(0);
			}
			if (tmpstr2[0] == 5) {
				if (tmpstr2[1] == 0) {
					close(sockfd2);
					printf("N %s %s %s %s Don't IRC from open socks5 servers (%s)\n", ba,bb,bc,bd,host);
					exit(0);
				}
				else
				if (tmpstr2[1] != 2) {
					close(sockfd2);
					printf("N %s %s %s %s Don't IRC from open socks5b servers (%s)\n", ba,bb,bc,bd,host);
					exit(0);
				}
			}
//			printf("Y %s was not an open socks server (4)\n", host);
			close(sockfd2);
//			exit(4);
			wingate(host,ba,bb,bc,bd); // now scan for wingates
		}
	}
}

int wingate(char *host, char *ba, char *bb, char *bc, char *bd)
{
	int wgsock;
	int e, d, numbytes;
	char buf[1024];
	struct in_addr MyHostAddr;
	struct hostent *he;
	struct sockaddr_in sin;
	fd_set gateset;
	struct timeval tv;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(23);
	sin.sin_addr.s_addr = inet_addr(host);
	if (sin.sin_addr.s_addr == INADDR_NONE) {
		he = gethostbyname(host);
		if (!he) {
			close(sockfd);
			printf("Y %s was not an open wingate (1)\n",
			       host);
			exit(1);
		}
		memcpy(&sin.sin_addr, he->h_addr, he->h_length);
	}
	e = connect(sockfd, (struct sockaddr *) &sin, sizeof(sin));
	if (e == -1) {
		close(sockfd);
		printf("Y %s was not an open wingate (2)\n", host);
		exit(2);
	}
	if (e == 0) {
		signal(SIGALRM, sigalrm_handler);
		flag = 3;
		alarm(socknull);

		FD_ZERO(&gateset);
		FD_SET(sockfd, &gateset);
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		d = select(sockfd + 4, NULL, &gateset, NULL, &tv);
		if (d == 0) {
			close(sockfd);
			printf("Y %s was not an open wingate (3)\n", host);
			exit(3);
		}

		numbytes = read(sockfd, buf, sizeof(buf));
		buf[numbytes] = '\0';
		if (numbytes >= 9) {
			numbytes = read(sockfd, buf, sizeof(buf));
			buf[numbytes] = '\0';
			if ((strcmp(buf, "WinGate>") == 0) ||
			    (strncmp
			     (buf, "Too many connected users - try again later",
			      numbytes) == 0)) {
				close(sockfd);
				printf("N %s %s %s %s Don't abuse misconfigured WinGates (%s)\n", ba,bb,bc,bd,host);
				exit(0);
			}
		}
	}
}
