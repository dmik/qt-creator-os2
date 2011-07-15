/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <share.h>

#define INCL_BASE
#include <os2.h>

extern char **environ;

static int qtcFd;
static char *sleepMsg;

static void __attribute__((noreturn)) doExit(int code)
{
    puts(sleepMsg);
    fgets(sleepMsg, 2, stdin); /* Minimal size to make it wait */
    exit(code);
}

static void sendMsg(const char *msg, int num)
{
    int pidStrLen;
    int ioRet;
    char pidStr[64];

    pidStrLen = sprintf(pidStr, msg, num);
    if ((ioRet = write(qtcFd, pidStr, pidStrLen)) != pidStrLen) {
        fprintf(stderr, "Cannot write to creator comm socket: %s\n",
                        (ioRet < 0) ? strerror(errno) : "short write");
        doExit(3);
    }
}

enum {
    ArgCmd = 0,
    ArgAction,
    ArgSocket,
    ArgMsg,
    ArgDir,
    ArgExe,
    ArgParms,
    ArgCount
};

/* syntax: $0 {"run"|"debug"} <pid-socket> <continuation-msg> <workdir> <exe> <args...> */
/* exit codes: 0 = ok, 1 = invocation error, 3 = internal error */
int main(int argc, char *argv[])
{
    APIRET rc;
    BYTE prio;
    PPIB pPIB;
    BOOL bDebug;
    STARTDATA sd;
    HQUEUE hTermQ;
    PULONG tqElemPtr;
    PCSZ extLibPath;
    CHAR termQName[64];
    CHAR msgBuffer[256];
    REQUESTDATA queueReq;
    RESULTCODES retVal;
    int i, j, l, envSize;
    char *envPtr, *argPtr;
    ULONG appType, sid, pid, tqElemSize;
    struct sockaddr_un sau;
    static struct { ULONG type; const char *name; } vars[] = {
        { BEGIN_LIBPATH, "QTC_BEGINLIBPATH" },
        { END_LIBPATH,   "QTC_ENDLIBPATH" },
        { LIBPATHSTRICT, "QTC_LIBPATHSTRICT" },
    };

    if (argc != ArgCount) {
        fprintf(stderr, "This is an internal helper of Qt Creator. Do not run it manually.\n");
        return 1;
    }
    sleepMsg = argv[ArgMsg];

    /* Connect to the master, i.e. Creator. */
    if ((qtcFd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("Cannot create creator comm socket");
        doExit(3);
    }
    sau.sun_family = AF_UNIX;
    strcpy(sau.sun_path, argv[ArgSocket]);
    if (connect(qtcFd, (struct sockaddr *)&sau, sizeof(sau))) {
        fprintf(stderr, "Cannot connect creator comm socket %s: %s\n", sau.sun_path, strerror(errno));
        doExit(1);
    }

    if (chdir(argv[ArgDir])) {
        /* Only expected error: no such file or direcotry */
        sendMsg("err:chdir %d\n", errno);
        return 1;
    }

    /* Query type of application */
    rc = DosQueryAppType((PSZ)argv[ArgExe], &appType);
    if (rc != NO_ERROR) {
        fprintf(stderr, "Unable to determine application type\n");
        sendMsg("err:exec %d\n", rc);
        doExit(3);
    }

    /* Set LIBPATH extensions */
    for (i = 0; i < sizeof(vars)/sizeof(vars[0]); i++) {
        extLibPath = (PCSZ)getenv(vars[i].name);
        if (extLibPath)
            DosSetExtLIBPATH(extLibPath, vars[i].type);
    }

    /* Strip LIBPATH variables from environment */
    envSize = 1;
    for (i = 0; environ[i]; i++)
        envSize += 1 + strlen(environ[i]);
    envPtr = (char *)alloca(envSize);
    envSize = 0;
    for (i = 0; environ[i]; i++) {
        for (j = sizeof(vars)/sizeof(vars[0]) - 1; j >= 0 ; j--) {
            l = strlen(vars[j].name);
            if( memcmp(environ[i], vars[j].name, l) == 0 && environ[i][l] == '=')
                break;
        }

        if (j < 0) {
            strcpy(envPtr+envSize, environ[i]);
            envSize += strlen(environ[i]) + 1;
        }
    }
    envPtr[envSize++] = '\0';

    /* Debug mode */
    //bDebug = strcmp(argv[ArgAction], "debug") == 0;
    bDebug = FALSE;

    /* Find out if we need a new session */
    if ((appType & FAPPTYP_EXETYPE) == FAPPTYP_WINDOWCOMPAT) {
        i = strlen(argv[ArgExe]) + 1;
        j = strlen(argv[ArgParms]) + 1;

        argPtr = (char *)alloca(i + j + 1);
        memcpy(argPtr, argv[ArgExe], i);
        memcpy(argPtr+i, argv[ArgParms], j);
        argPtr[i+j] = '\0';

        rc = DosExecPgm(msgBuffer,  sizeof(msgBuffer),
                        bDebug ? EXEC_ASYNCRESULTDB : EXEC_ASYNCRESULT,
                        (PSZ)argPtr, (PSZ)envPtr, &retVal, (PSZ)argv[ArgExe]);
        if (rc != NO_ERROR) {
            fprintf(stderr, "Cannot start child process due to %s\n", msgBuffer);
            sendMsg("err:exec %d\n", rc);
            doExit(3);
        }
        pid = retVal.codeTerminate;

        /* Send client PID to creator */
        sendMsg("pid %d\n", pid);

        rc = DosWaitChild(DCWA_PROCESS, DCWW_WAIT, &retVal, &pid, pid);
        if (rc != NO_ERROR) {
            fprintf(stderr, "Cannot obtain return codes from child %u\n", (unsigned)rc);
            doExit(3);
        }
    } else {
        /* Prepare start data */
        memset(&sd, 0, sizeof(sd));
        sd.Length = sizeof(sd);
        sd.Related = SSF_RELATED_CHILD;
        sd.FgBg = SSF_FGBG_FORE;
        sd.TraceOpt = bDebug ? SSF_TRACEOPT_TRACE : SSF_TRACEOPT_NONE;
        sd.PgmName  = (PSZ)argv[ArgExe];
        sd.PgmInputs = (PSZ)argv[ArgParms];
        sd.Environment = (PBYTE)envPtr;
        sd.InheritOpt = SSF_INHERTOPT_PARENT;
        sd.SessionType = SSF_TYPE_DEFAULT;
        sd.TermQ = (PBYTE)termQName;
        sd.PgmControl = SSF_CONTROL_VISIBLE | SSF_CONTROL_NOAUTOCLOSE;
        sd.ObjectBuffer = (PSZ)msgBuffer;
        sd.ObjectBuffLen = sizeof(msgBuffer);

        DosGetInfoBlocks(NULL, &pPIB);
        sprintf(termQName, "\\QUEUES\\QTC-STUB.%u", (unsigned)pPIB->pib_ulpid);
        rc = DosCreateQueue(&hTermQ, QUE_FIFO, (PSZ)termQName);
        if (rc != NO_ERROR) {
            fprintf(stderr, "Cannot create %s\n", termQName);
            doExit(3);
        }
        rc = DosStartSession(&sd, &sid, &pid);
        if (rc != NO_ERROR) {
            fprintf(stderr, "Cannot start child session due to %s\n", msgBuffer);
            sendMsg("err:exec %d\n", rc);
            doExit(3);
        }

        /* Send client PID to creator */
        sendMsg("pid %d\n", pid);

        /* Wait for client termination */
        rc = DosReadQueue(hTermQ, &queueReq,
                          &tqElemSize, (PPVOID)&tqElemPtr,
                          0, DCWW_WAIT, &prio, NULLHANDLE);
        DosCloseQueue(hTermQ);

        if (rc != NO_ERROR) {
            fprintf(stderr, "Cannot obtain return codes from child %u\n", (unsigned)rc);
            doExit(3);
        }

        /* We cannot determine the termination code */
        retVal.codeTerminate = TC_EXIT;
        retVal.codeResult = HIUSHORT(*tqElemPtr);

        /* Free data retrieved from termination queue */
        DosFreeMem(tqElemPtr);
    }

    /* Tell creator, that we are finshed */
    switch (retVal.codeTerminate) {
    case TC_TRAP:
    case TC_EXCEPTION:
        sendMsg("crash %d\n", SIGSEGV);
        break;

    case TC_HARDERROR:
    case TC_KILLPROCESS:
        sendMsg("crash %d\n", SIGKILL);
        break;

    default:
        sendMsg("exit %d\n", retVal.codeResult);
    }

    doExit(0);
}

