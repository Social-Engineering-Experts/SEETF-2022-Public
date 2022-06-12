FROM ubuntu:16.04

RUN useradd -m format

WORKDIR /home/format

COPY vuln.c .
COPY flag .

RUN chmod +r flag
RUN apt update
RUN apt install -y socat gcc libc6-dev-i386

RUN gcc -m32 -o vuln vuln.c
RUN chmod 755 vuln

USER format

EXPOSE 1234

ENTRYPOINT socat tcp-l:1234,fork,reuseaddr exec:./vuln

