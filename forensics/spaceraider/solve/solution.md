## Solution:

1. This challenge consist of 2 parts, first part is RAID 6 recovery. 3 
disks images are provided, the first step is for the player to identify 
this is a RAID 6 config. 
2. Once the player has identified the config, he/she needs to be able to 
look for the superblock, both sda1.img and sdb1.img magic bytes has been 
wiped, sdc1.img has half of the magic bytes wiped. The player just needs 
to recover 2 disks. The superblock can be found at 0x00001000, it needs to 
be repopulated with fc 4e 2b a9.
3. Once the player has completed that, he/she can then reassemble the 
disks with mdadm --assemble, completing part 1.
4. Part 2 will require the player to understand the "Falloni" CVE. There 
is a docx file within the recovered disk which has an exploit written into its doc_xml. Player needs 
to retrieve the exploit endpoint which will lead to Cyberleague website, inspect source and retrieve the flag.

Flag: `SEE{Y0U_@CYB3RL34GU3_M4J0R_2}`
