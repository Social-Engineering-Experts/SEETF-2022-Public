FROM python:3.10-slim-buster

WORKDIR /home
ADD . .
RUN python3 -m pip install -r requirements.txt

CMD uvicorn main:app --host 0.0.0.0