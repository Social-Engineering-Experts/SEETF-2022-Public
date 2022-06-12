FROM python:3

RUN useradd -m wayyang

WORKDIR /home/wayyang

COPY wayyang.py .
COPY run.sh .
COPY FLAG .
COPY news .

RUN chmod +rx wayyang.py
RUN chmod +rx run.sh

RUN apt update

RUN apt install -y \
    socat

RUN chown -R wayyang:wayyang .

RUN mkdir /home/wayyang/bin && \
    cp /bin/ls /home/wayyang/bin && \
    cp /bin/cat /home/wayyang/bin

USER wayyang

EXPOSE 5000

ENTRYPOINT socat tcp-l:5000,fork,reuseaddr exec:./run.sh