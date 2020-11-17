FROM debian:jessie

RUN (echo "deb-src http://deb.debian.org/debian jessie main" && \
     echo "deb-src http://security.debian.org/debian-security jessie/updates main" && \
     echo "deb-src http://deb.debian.org/debian jessie-updates main") | tee -a /etc/apt/sources.list

RUN apt-get update && apt-get dist-upgrade -y && apt-get install -y python3-pip clang-6.0 build-essential bear && apt-get clean
RUN pip3 install delegator.py typing && pip3 install clang==6.0.0.2
RUN ln -s /usr/lib/llvm-6.0/lib/libclang.so.1 /usr/lib/x86_64-linux-gnu/libclang.so

CMD ["/bin/bash"]
