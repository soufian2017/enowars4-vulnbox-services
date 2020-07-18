FROM mcr.microsoft.com/dotnet/core/sdk:3.1 AS build
WORKDIR /src
COPY Gamemaster.csproj .
RUN dotnet restore Gamemaster.csproj
COPY [^Frontend]* ./
RUN dotnet publish Gamemaster.csproj -c Release -o /app

FROM node as frontend
WORKDIR /src
COPY package.json .
COPY package-lock.json .
RUN npm install
COPY tsconfig.json .
COPY Frontend ./Frontend
COPY *.js ./
RUN npm run release

FROM mcr.microsoft.com/dotnet/core/aspnet:3.1
WORKDIR /app
COPY --from=build /app .
COPY --from=frontend /src/wwwroot ./wwwroot
COPY appsettings.json .
ENTRYPOINT ["dotnet", "Gamemaster.dll"]
