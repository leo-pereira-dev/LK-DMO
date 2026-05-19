FROM mcr.microsoft.com/dotnet/sdk:7.0 AS build

WORKDIR /src
COPY . .

ARG PROJECT_PATH
RUN dotnet publish "${PROJECT_PATH}" -c Release -o /app/publish /p:UseAppHost=false

FROM mcr.microsoft.com/dotnet/aspnet:7.0 AS runtime

WORKDIR /app
COPY --from=build /app/publish .

ENV ASPNETCORE_ENVIRONMENT=Development
ENV DOTNET_ENVIRONMENT=Development

ENTRYPOINT ["sh", "-c", "dotnet \"$APP_DLL\""]
