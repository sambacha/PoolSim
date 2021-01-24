FROM alpine:3.12

VOLUME "/poolsim"

WORKDIR "/poolsim"

RUN apk update && \
    apk upgrade && \
    apk --update add \
        gcc \
        g++ \
        build-base \
        cmake \
        bash \
        libstdc++ \
        cppcheck \
        py-pip && \
        pip install conan && \
    rm -rf /var/cache/apk/*

RUN echo http://nl.alpinelinux.org/alpine/edge/main >> /etc/apk/repositories
RUN echo http://nl.alpinelinux.org/alpine/edge/community >> /etc/apk/repositories
RUN echo http://nl.alpinelinux.org/alpine/edge/testing >> /etc/apk/repositories
RUN apk add --update --no-cache \
 build-base \
 cmake \
 git \
 lcov \
 cpputest \
 cunit-dev \
 doxygen

ENTRYPOINT [ "bash", "-c" ]
