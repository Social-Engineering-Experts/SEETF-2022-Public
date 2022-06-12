FROM ubuntu:bionic

RUN apt-get update
RUN apt-get install -y xinetd build-essential
RUN echo "$user     hard    nproc       20" >> /etc/security/limits.conf

RUN adduser --disabled-password hall_of_famer
COPY ./service /etc/xinetd.d/

WORKDIR /home/hall_of_famer

COPY ./flag.txt /home/hall_of_famer/flag.txt
COPY ./hall_of_fame.c /home/hall_of_famer/hall_of_fame.c
COPY ./Makefile /home/hall_of_famer/Makefile

RUN make && \
    rm hall_of_fame.c && \
    rm Makefile

RUN chmod 711 /home/hall_of_famer/hall_of_fame && \
    chmod 744 /home/hall_of_famer/flag.txt && \
    chown -R root:root . && \
    chmod 755 .

USER hall_of_famer

EXPOSE 55555

CMD ["/usr/sbin/xinetd", "-dontfork"]
