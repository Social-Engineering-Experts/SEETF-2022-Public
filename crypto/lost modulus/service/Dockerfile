FROM python:3
WORKDIR /app

RUN useradd -m chall
RUN pip install pycryptodome
RUN apt update
RUN apt install -y socat

COPY encrypt.py .
COPY flag.txt .

RUN chmod +r encrypt.py
RUN chmod +r flag.txt

USER chall

ENTRYPOINT socat tcp-l:1337,fork,reuseaddr exec:"python encrypt.py"