WELCOME TO my FIRST ASSIGNMENT AT IIITH AS AN LATERAL ENTRY 
MY LEARNINGS IN
Part 2: Process Creation and Management:

P0 start
│
├─ Task 1 fork → P1
│   ├─ P0: x = 25 - 5 = 20 → print "Parent"
│   │   continue to Task 2
│   └─ P1: x = 25 + 10 = 35 → print "Child"
│       exit(0)   ← stops here, never runs Task 2 or Task 3
│
├─ Task 2 (only P0)
│   ├─ P0 forks → C0
│   │   C0 exec("./writer") → write P0's PID to newfile.txt → exit
│   └─ P0 wait(C0)
│       print "Parent finished waiting"
│
├─ Task 3 (only P0)
│   ├─ P0 forks → T0
│   │   P0 print "Parent PID = ..."
│   │   exit(0)
│   └─ T0 sleeps(2)
│       print "Child PID, Parent PID"
│       (Parent PID will be 1 → init/systemd)
│
└─ End: all processes exit

🔍 Comparison – Task 2 vs Task 3
Task 2:

The child’s parent (P0) is alive when it runs.

newfile.txt contains P0’s PID.

Task 3:

The parent (P0) terminates before the child runs.

Child’s parent PID becomes 1 (init/systemd).

Why the difference?
In Task 2, the parent is alive, so getppid() returns the real parent’s PID.

In Task 3, the parent is gone, so the child is reparented to PID 1.

Role of init / systemd
PID 1 is the very first process started by the kernel.

Adopts all orphaned processes.

Waits on them when they terminate to prevent zombie processes.



vatsav@Srivastav:/mnt/c/IIITH/UG2/MONSOON/OPERATING SYSTEMS AND NETWORKS/MINIPROJECTS/Assignment-0/Part-2$ ./processManagement
Running Task 1:
Parent Process: PID = 958, x = 20
Child Process: PID = 959, x = 35

Running Task 2:
Parent process (PID = 958) finished waiting.

Running Task 3:
Parent Process: PID = 958
vatsav@Srivastav:/mnt/c/IIITH/UG2/MONSOON/OPERATING SYSTEMS AND NETWORKS/MINIPROJECTS/Assignment-0/Part-2$ Child Process: PID = 961, Parent PID = 536
ps aux
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root           1  0.0  0.1  21612 12916 ?        Ss   13:54   0:02 /sbin/init
root           2  0.0  0.0   2616  1444 ?        Sl   13:54   0:00 /init
root           7  0.0  0.0   2616   132 ?        Sl   13:54   0:00 plan9 --control-socket 6 --log-level 4 --server-fd 7
root          58  0.0  0.2 132280 17508 ?        S<s  13:54   0:00 /usr/lib/systemd/systemd-journald
root         108  0.0  0.0  24372  6520 ?        Ss   13:54   0:01 /usr/lib/systemd/systemd-udevd
systemd+     187  0.0  0.1  21452 11936 ?        Ss   13:54   0:00 /usr/lib/systemd/systemd-resolved
systemd+     190  0.0  0.0  91020  6572 ?        Ssl  13:54   0:00 /usr/lib/systemd/systemd-timesyncd
root         203  0.0  0.0   4236  2700 ?        Ss   13:54   0:00 /usr/sbin/cron -f -P
message+     204  0.0  0.0   9532  5144 ?        Ss   13:54   0:00 @dbus-daemon --system --address=systemd: --nofork --n
root         229  0.0  0.1  17976  8416 ?        Ss   13:54   0:00 /usr/lib/systemd/systemd-logind
root         237  0.0  0.2 1756096 16124 ?       Ssl  13:54   0:00 /usr/libexec/wsl-pro-service -vv
root         261  0.0  0.0   3160  1232 hvc0     Ss+  13:54   0:00 /sbin/agetty -o -p -- \u --noclear --keep-baud - 1152
syslog       263  0.0  0.0 222508  7248 ?        Ssl  13:54   0:00 /usr/sbin/rsyslogd -n -iNONE
root         270  0.0  0.0   3116  1232 tty1     Ss+  13:54   0:00 /sbin/agetty -o -p -- \u --noclear - linux
root         326  0.0  0.2 107024 23008 ?        Ssl  13:54   0:00 /usr/bin/python3 /usr/share/unattended-upgrades/unatt
root         390  0.0  0.0   2624   124 ?        Ss   13:54   0:00 /init
root         393  0.0  0.0   2624   132 ?        S    13:54   0:00 /init
vatsav       397  0.0  0.0   6068  5196 pts/0    Ss+  13:54   0:00 -bash
root         400  0.0  0.0   6688  4652 pts/1    Ss   13:54   0:00 /bin/login -f
vatsav       499  0.0  0.1  20052 11248 ?        Ss   13:54   0:00 /usr/lib/systemd/systemd --user
vatsav       500  0.0  0.0  21148  1728 ?        S    13:54   0:00 (sd-pam)
vatsav       513  0.0  0.0   6072  5260 pts/1    S+   13:54   0:00 -bash
root         535  0.0  0.0   2624   128 ?        Ss   13:55   0:00 /init
root         536  0.0  0.0   2624   136 ?        S    13:55   0:00 /init
vatsav       541  0.0  0.0   6324  5480 pts/2    Ss   13:55   0:00 -bash
vatsav       962  300  0.0   8284  4120 pts/2    R+   15:50   0:00 ps aux
vatsav@Srivastav:/mnt/c/IIITH/UG2/MONSOON/OPERATING SYSTEMS AND NETWORKS/MINIPROJECTS/Assignment-0/Part-2$



HERE CLEARLY WE CAN SEE ORPHANING 
root         536  0.0  0.0   2624   136 ?        S    13:55   0:00 /init

Part 1: File Management


vatsav@Srivastav:/mnt/c/IIITH/UG2/MONSOON/OPERATING SYSTEMS AND NETWORKS/MINIPROJECTS/Assignment-0/Part-1$ ./fileManagement
Enter command: INPUT
Unknown command.
Enter command:INPUT
Unknown command.
Enter command:INPUT
INPUT -> HI
Enter command:INPUT
INPUT ->    IHJHJ
Enter command:PRINT
HI
   IHJHJ
Enter command:FIRST    1
Unknown command.
Enter command:FIRST 1
HI
Enter command:FIRST 2
HI
   IHJHJ
Enter command:FIRST 3
HI
   IHJHJ
Enter command:LAST 2
HI
   IHJHJ
Enter command:LAST 1
   IHJHJ
Enter command:LOG 1
LAST 1
Enter command:LOG 5
FIRST 1
FIRST 2
FIRST 3
LAST 2
LAST 1
Enter command:LOG   8
Unknown command.
Enter command:STOP
vatsav@Srivastav:/mnt/c/IIITH/UG2/MONSOON/OPERATING SYSTEMS AND NETWORKS/MINIPROJECTS/Assignment-0/Part-1$



ALL MY LEARNING LINKS AND PROMPTS :
https://chatgpt.com/share/689788b5-c860-8003-b15f-85e007f8ca7d
https://chatgpt.com/share/68978976-f498-8003-b6fb-2b43708ac507
https://g.co/gemini/share/451ccb582c01
https://docs.google.com/document/d/1Lws8tKzp56_a9HEm4mXzP53asDq-3o8Wm8H4tcwjqXQ/edit?usp=sharing
https://chatgpt.com/share/68978acf-0944-8003-9047-848b83d30cb2
https://claude.ai/share/8be82dda-dfff-4cc3-b3da-b4f2516d81c0