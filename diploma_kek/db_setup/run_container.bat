ECHO on

docker image build -t postgres_server .
start docker run --name postgres-container -e POSTGRES_PASSWORD=123 postgres_server

timeout 7

docker exec -it postgres-container bash

timeout 3

docker kill postgres-container
docker rm -f postgres-container
