#!/bin/bash

# Script to be run on a manylinux2014 docker image to complete it for OPM usage.
# i.e. docker run -i -t quay.io/pypa/manylinux2014_x86_64 < setup-docker-image.sh

# A ready made Docker image is available at Dockerhub:
# docker run -i -t lindkvis/manylinux2014_opm:latest

yum-config-manager --add-repo \
https://www.opm-project.org/package/opm.repo
yum install -y cmake3 ccache python3-devel openssl-devel wget boost169-devel boost169-static

cd /tmp
wget https://www.python.org/ftp/python/3.7.7/Python-3.7.7.tgz
tar zxf Python-3.7.7.tgz
cd Python-3.7.7
./configure
make -j2
make altinstall
cd /tmp
rm -rf Python-3.7.7*

wget https://www.python.org/ftp/python/3.8.0/Python-3.8.0.tgz
tar zxf Python-3.8.0.tgz
cd Python-3.8.0
./configure
make -j2
make altinstall
cd /tmp
rm -rf Python-3.8.0*

/usr/local/bin/python3.7 -m pip install pip --upgrade
/usr/local/bin/python3.7 -m pip install wheel setuptools twine pytest-runner auditwheel
/usr/local/bin/python3.8 -m pip install pip --upgrade
/usr/local/bin/python3.8 -m pip install wheel setuptools twine pytest-runner auditwheel
/usr/bin/python3 -m pip install pip --upgrade
/usr/bin/python3 -m pip install wheel setuptools twine pytest-runner auditwheel