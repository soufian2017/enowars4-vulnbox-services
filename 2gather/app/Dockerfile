FROM python:3.8.1

RUN apt-get update && apt-get install -y \
  python3-dev \
  libldap2-dev \
  libsasl2-dev

COPY requirements.txt .
RUN pip install --upgrade pip && \
    pip install --upgrade setuptools && \
    pip install -r requirements.txt

COPY . /service-2gather
WORKDIR /service-2gather

EXPOSE 8000

ENTRYPOINT ["/bin/bash", "./startup.sh"]
