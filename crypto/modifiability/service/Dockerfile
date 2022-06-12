FROM python:3
WORKDIR /app

RUN useradd -m chall
RUN pip install pycryptodome
RUN apt update
RUN apt install -y socat

COPY modifiability.py .
COPY secret.py .

RUN chmod +r modifiability.py
RUN chmod +r secret.py

USER chall

ENTRYPOINT socat tcp-l:1339,fork,reuseaddr exec:"python modifiability.py"