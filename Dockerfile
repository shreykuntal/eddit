FROM ubuntu:24.04 AS exec

WORKDIR /app

RUN apt update -y && apt install -y cmake && apt install -y build-essential

COPY include include
COPY CMakeLists.txt .
COPY src src

RUN cmake -S . -B build

RUN cmake --build build

FROM ubuntu:24.04

COPY --from=exec app/build/eddit eddit

EXPOSE 8080

ENTRYPOINT ["./eddit"]