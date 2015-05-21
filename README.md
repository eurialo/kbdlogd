kbdlogd
=======

Introduction
------------

kbdlogd is a case study purpose daemon to capture stream 
data from the keyboard, that uses a /dev/input/* kbd device.
It needs the root privileges in order to capture data.

Install
-------

```bash
deftcode ~ $ git clone https://github.com/eurialo/kbdlogd.git
deftcode ~ $ cd kbdlogd/ && make && sudo make install
```

Example
-------

Run in debug-mode and update the PTS(s) list every 20 seconds:

```bash
deftcode kbdlogd $ sudo kbdlogd -d -u 20
kbdlogd has been started.

Current active PTS(s):
'eurialo' at ':0'
'eurialo' at '/0'
'eurialo' at '/4'
'eurialo' at '/11'
'eurialo' at '/16'
'eurialo' at '/25'

13:45:07 05/21/15 >> test keylogger
```
