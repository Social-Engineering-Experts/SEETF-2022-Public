FROM ethereum/client-go:v1.10.15

RUN apk add --no-cache jq

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

EXPOSE 8545

COPY ./config /config

ENTRYPOINT ["/entrypoint.sh"]
