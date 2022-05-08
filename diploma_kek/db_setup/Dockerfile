FROM postgres:latest

RUN mkdir /home/main
WORKDIR /home/main

COPY . .

RUN apt-get update -y
RUN apt-get install software-properties-common -y
RUN sed -i "/^# deb.*universe/ s/^# //" /etc/apt/sources.list
RUN apt-get update -y
RUN apt-get install python3
RUN apt install python3-pip -y
RUN pip3 install -r requirements.txt
