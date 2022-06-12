FROM python:3

RUN useradd -m random

WORKDIR /home/random

COPY asdf.py .
COPY flag /

RUN chmod +rx asdf.py
RUN chmod +r /flag

RUN apt update

RUN apt install -y \
    socat

USER random

EXPOSE 9001

ENTRYPOINT socat tcp-l:9001,fork,reuseaddr exec:"python asdf.py"
