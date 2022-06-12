1. Open the file in WireShark
2. File -> Export Objects -> HTTP to download the file ("thingamajig.zip")
3. The ZIP file is password protected
4. In WireShark, filter the src and dest to be the same subnet `ip.src == 192.168.112.0/24 & ip.dst == 192.168.112.0/24`
5. Navigate further down to find the telnet conversation between the two parties. It will contain the password to the zip
6. `binwalk -e stuff` to extract the hidden zipfile
7. use john to crack the zipfile (the password is john, so it should be pretty fast)
