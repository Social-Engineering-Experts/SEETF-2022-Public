#!/bin/bash

keystorePassword="tamper-gratify-arrive"
echo $keystorePassword > `pwd`/config/password.txt

{
    /usr/bin/expect << EOD

    set keystorePassword $keystorePassword

    ## 1. Create a sealer account

    set timeout 600

    spawn docker run -it --rm  -v `pwd`/config:/root/.ethereum ethereum/client-go account new

    expect "Password: "

    send -- "\$keystorePassword\r"

    expect "Repeat password: "

    send -- "\$keystorePassword\r"

    expect -re "0x(\[0-9A-Za-z\]+)\r\n.*key!"

    set address \$expect_out(1,string)

    spawn /home/zeyu/go/bin/puppeth

    expect ">"

    send -- "genesis\r"

    expect ">"

    send -- "2\r"

    expect ">"

    send -- "1\r"

    expect ">"

    send -- "2\r"

    expect ">"

    send -- "\r"

    expect "> 0x"

    send -- "\$address\r"

    expect "> 0x"

    send -- "\r"

    expect "> 0x"

    send -- "\$address\r"

    expect "> 0x"

    send -- "\r"

    expect ">"

    send -- "no\r"

    expect ">"

    send -- "1337\r"

    expect ">"

    send -- "2\r"

    expect ">"

    send -- "2\r"

    expect ">"

    send -- "config\r"

    expect "test"
EOD
}

