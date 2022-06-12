FROM ubuntu:18.04
WORKDIR /app

RUN useradd -m chall
RUN apt update
RUN apt install -y socat

COPY pokemonbattle .
COPY flag.txt .

RUN chmod +x pokemonbattle

USER chall

ENTRYPOINT socat tcp-l:1335,fork,reuseaddr exec:./pokemonbattle