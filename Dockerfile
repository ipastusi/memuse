FROM ubuntu:24.04 AS build

RUN apt-get update && \
    apt-get install --no-install-recommends -y make cmake clang && \
    apt-get upgrade -y && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /memuse
COPY src src
COPY CMakeLists.txt CMakeLists.txt

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build


FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install --no-install-recommends -y adduser && \
    apt-get upgrade -y && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN adduser memuse -u 11000
USER 11000

WORKDIR /memuse
COPY --chown=memuse --from=build /memuse/build/memuse memuse

ENTRYPOINT [ "./memuse" ]
