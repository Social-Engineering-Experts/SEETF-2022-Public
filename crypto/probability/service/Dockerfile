FROM python:3
WORKDIR /app

RUN useradd -m chall
RUN apt update
RUN apt install -y socat

COPY probability.py .
COPY secret.py .

RUN chmod +r probability.py
RUN chmod +r secret.py

USER chall

ENTRYPOINT socat tcp-l:1337,fork,reuseaddr exec:"python probability.py"