FROM python:3
WORKDIR /app

RUN useradd -m chall
RUN pip install pycryptodome
RUN apt update
RUN apt install -y socat

COPY univeRSAlity.py .
COPY secret.py .

RUN chmod +r univeRSAlity.py
RUN chmod +r secret.py

USER chall

ENTRYPOINT socat tcp-l:1338,fork,reuseaddr exec:"python univeRSAlity.py"