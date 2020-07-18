FROM node:12.16.3-slim

WORKDIR /app
ENV PATH app/node_modules/.bin:$PATH
COPY ./package.json /app/package.json
RUN npm install
RUN npm install -g @angular/cli

COPY . /app

EXPOSE 4200

ENTRYPOINT ["ng","serve","--host", "0.0.0.0","--disableHostCheck", "true"]
