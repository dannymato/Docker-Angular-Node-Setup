FROM node:10-alpine

RUN mkdir -p /app
WORKDIR /app

COPY client/package.json /app
COPY client/ /app

RUN npm install
RUN npm install -g @angular/cli

CMD ["ng", "serve", "--host", "0.0.0.0"]