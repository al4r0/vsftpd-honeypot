## vsftpd 2.3.4 Honeypot
A simple honeypot using vsftpd 2.3.4 backdoor, and false shell in port 6200 than original backdoor.

Work on Linux for now.


To do:
- [x] Principal socket
- [x] Save the log in file
- [x] Response to ftp commands
- [ ] Add date in log
- [ ] Port 6200 socket
- [ ] Improve ftp answers
- [ ] Make multithread
- [ ] Make compatible to others SO

```
In linux ports under 1024 need root.
I use 2121 and redirect to 21 on the NAT of the gateway.
```
