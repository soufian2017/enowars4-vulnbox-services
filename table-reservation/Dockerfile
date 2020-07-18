# For DroneCI usage
FROM debian:buster
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get install -y default-jdk libpostgresql-jdbc-java
COPY TableReservation.jar /home/TableReservation.jar
EXPOSE 55555
WORKDIR /home

CMD [ "java", "-jar", "TableReservation.jar" ]