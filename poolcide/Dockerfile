FROM python:3.8-buster

RUN apt-get update && apt-get install -y gcc-multilib wget
RUN groupadd poolcide
RUN useradd poolcide --gid poolcide

# downloading static age binary for private key crypto - age-encryption.org
WORKDIR /tmp
RUN wget https://github.com/FiloSottile/age/releases/download/v1.0.0-beta2/age-v1.0.0-beta2-linux-amd64.tar.gz \
    && tar -xvf age-v1.0.0-beta2-linux-amd64.tar.gz \
    && mv ./age ./age_dir && mv ./age_dir/age ./age && rm -rf ./age_dir \
    && rm -rf age-*

# RUN pip3 install gevent

COPY . /poolcide
WORKDIR /poolcide

RUN rm -rf ./age && mv /tmp/age . && chmod 777 ./age

# building poolcide
RUN make clean
# RUN make tests
RUN make

#USER poolcide

RUN chmod +x "./start.sh"

# POOL (hah, so funny!)
EXPOSE 9001

ENTRYPOINT ["./start.sh"]
