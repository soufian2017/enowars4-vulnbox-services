FROM golang:1.14

RUN apt-get update && apt-get install -y make gcc

ENV GOPATH /go

EXPOSE 7890

RUN go get github.com/gorilla/mux
RUN go get github.com/gorilla/sessions
RUN go get github.com/go-sql-driver/mysql

COPY buggy /go/src/buggy
WORKDIR /go/src/buggy

ENTRYPOINT ["make", "run"]

