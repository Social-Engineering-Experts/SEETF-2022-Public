FROM ubuntu:18.04
WORKDIR /app

RUN useradd -m chall
RUN apt update
RUN apt install -y socat

COPY huffbleed .
COPY flag.txt .

RUN chmod +x huffbleed

USER chall

ENTRYPOINT socat tcp-l:1334,fork,reuseaddr exec:./huffbleed