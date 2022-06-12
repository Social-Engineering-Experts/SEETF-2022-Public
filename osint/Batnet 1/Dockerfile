FROM python:3

RUN useradd -m random

WORKDIR /home/random

COPY checker.py .

RUN chmod +rx checker.py

RUN apt update

RUN apt install -y \
    socat

USER random

EXPOSE 9001

ENTRYPOINT socat tcp-l:9001,fork,reuseaddr exec:"python checker.py"
