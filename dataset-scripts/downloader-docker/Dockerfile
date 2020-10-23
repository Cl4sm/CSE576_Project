from debian:jessie

# enable source downloading
run echo "deb-src http://deb.debian.org/debian jessie main" >> /etc/apt/sources.list
run echo "deb-src http://security.debian.org/debian-security jessie/updates main" >> /etc/apt/sources.list
run echo "deb-src http://deb.debian.org/debian jessie-updates main" >> /etc/apt/sources.list
run apt-get update
run apt-get install -y dpkg-dev

# copy lists into the container
copy ./dataset-packages /dataset-packages
workdir dataset-packages
copy ./downloader.sh /downloader.sh

cmd /downloader.sh

