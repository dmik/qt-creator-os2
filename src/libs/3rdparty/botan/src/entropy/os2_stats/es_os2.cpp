/**
* OS/2 EntropySource
* (C) 2011 Ruediger Ihle
*
* Distributed under the terms of the Botan license
*/

#include <botan/es_os2.h>

#define INCL_DOS
#include <os2.h>

#define   CMD_KI_RDCNT    (0x63)

typedef struct _CPUUTIL {
    ULONG ulTimeLow;            /* Low 32 bits of time stamp      */
    ULONG ulTimeHigh;           /* High 32 bits of time stamp     */
    ULONG ulIdleLow;            /* Low 32 bits of idle time       */
    ULONG ulIdleHigh;           /* High 32 bits of idle time      */
    ULONG ulBusyLow;            /* Low 32 bits of busy time       */
    ULONG ulBusyHigh;           /* High 32 bits of busy time      */
    ULONG ulIntrLow;            /* Low 32 bits of interrupt time  */
    ULONG ulIntrHigh;           /* High 32 bits of interrupt time */
} CPUUTIL;


#define ARRAY_SIZE(a)   (sizeof(a) / sizeof(a[0]))


namespace Botan {

/**
* OS/2 entropy poll
*/
void OS2_EntropySource::poll(Entropy_Accumulator& accum)
   {
      static ULONG sysValIds[] =
      {
         QSV_FOREGROUND_PROCESS, QSV_MAXPRMEM,
         QSV_MS_COUNT,           QSV_MAXSHMEM,
         QSV_FOREGROUND_PROCESS, QSV_MAXHPRMEM,
         QSV_MS_COUNT,           QSV_MAXHSHMEM
      };

      struct
      {
         ULONG          sysVals[ARRAY_SIZE(sysValIds)];
         CPUUTIL        perfVals;
         QWORD          qwTime;
      } e;

      int       cnt;

      for(;;) {
         for(cnt = 0; cnt < (int)ARRAY_SIZE(sysValIds); cnt++) {
            DosQuerySysInfo(sysValIds[cnt], sysValIds[cnt],
                            &e.sysVals[cnt], sizeof(e.sysVals[0]));
         }

         if(DosPerfSysCall(CMD_KI_RDCNT, (ULONG)&e.perfVals, 0, 0) == 0) {
            cnt += sizeof(e.perfVals) / sizeof(e.sysVals[0]);
         }

         DosTmrQueryTime(&e.qwTime);
         cnt += sizeof(e.qwTime) / sizeof(e.sysVals[0]);

         while( --cnt >= 0 ) {
            accum.add(e.sysVals[cnt], (cnt & 1) ? 1 : 4);
            if(accum.polling_goal_achieved())
               break;
         }

         DosSleep(0);
      }
   }

}
