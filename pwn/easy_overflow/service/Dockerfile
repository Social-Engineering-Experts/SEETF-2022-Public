FROM ubuntu:16.04

RUN useradd -m test

WORKDIR /home/test

COPY easy_overflow .
COPY flag .
COPY run.sh .

RUN chmod 755 run.sh
RUN chmod 755 easy_overflow
RUN chmod +r flag
RUN apt update
RUN apt install -y socat

USER test

EXPOSE 1234

ENTRYPOINT socat tcp-l:1234,fork,reuseaddr exec:./run.sh

