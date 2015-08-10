# Sample mDNS with TCP/UDP and Network Time

## NOTE THIS CODE ACTUALLY DEMOS A POSSIBLE BUG IN ESP8266 SDK ##

This code demos a bug, possibly in the NTP code or the ESP8266 SDK of the mDNS functions. 

To use the code, make sure you set the SSID and Wifi password in `user_config.h`.

Also, the ntp server is set in `ntp.c`

When NTP is called before mDNS, nDNS fails.   

Compile with `make BROKEN=1`, and this will include the NTP call, and mDNS is will not work.

The serial output looks like this:

```
f 0, scandone
          12            0 wifi connect status -  1/0
          12            0 wifi connect status -  1/0
          12            0 wifi connect status -  1/0
          12            0 wifi connect status -  1/0
add 0
aid 2
pm open phy_2,type:2 0 0
cnt 

connected with wap.dpdtech.com, channel 2
dhcp client start...
          14            0 wifi connect status -  1/0
          17            0 wifi connect status -  1/0
          19            0 wifi connect status -  1/0
ip:192.168.1.226,mask:255.255.255.0,gw:192.168.1.1
          22            0 wifi connect status -  5/0
18:fe:34:f1:76:a7 192.168.1.226
          24   1439183724 wifi connect status -  5/1
          24   1439183724 Boot Status -  5/2
host_name = HostName8266
server_name = _http._tcp.local
udp_join_multigrup failed!
          24   1439183724 Setup Complete -  5/2
          34   1439183725 Running -  5/2
          44   1439183726 Running -  5/2
          54   1439183727 Running -  5/2
```	


Recompile with  `make clean  && make ` and mDNS will work fine:


```
add if0
f 0, scandone
          12            0 wifi connect status -  1/0
          12            0 wifi connect status -  1/0
          12            0 wifi connect status -  1/0
          12            0 wifi connect status -  1/0
add 0
aid 2
pm open phy_2,type:2 0 0
cnt 

connected with wap.dpdtech.com, channel 2
dhcp client start...
          14            0 wifi connect status -  1/0
          17            0 wifi connect status -  1/0
          19            0 wifi connect status -  1/0
          22            0 wifi connect status -  1/0
          24            0 wifi connect status -  1/0
          27            0 wifi connect status -  1/0
          29            0 wifi connect status -  1/0
ip:192.168.1.226,mask:255.255.255.0,gw:192.168.1.1
          32            0 wifi connect status -  5/0
18:fe:34:f1:76:a7 192.168.1.226
          32   1439183355 Boot Status -  5/2
host_name = HostName8266
server_name = _http._tcp.local
          32   1439183355 Setup Complete -  5/2
          42   1439183356 Running -  5/2
          52   1439183357 Running -  5/2

```