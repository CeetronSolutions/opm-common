#!/bin/bash

# Run in docker container
# docker run -e PLAT=manylinux2014_x86_64 -it lindkvis/manylinux2014_opm:latest

yum install -y blas-devel suitesparse-devel trilinos-openmpi-devel

cd /tmp
git clone https://github.com/CeetronSolutions/opm-common.git
cd opm-common
git checkout system-python-pypi

mkdir build && cd build
cmake3 -DPYTHON_EXECUTABLE=/usr/bin/python3 -DBOOST_INCLUDEDIR=/usr/include/boost169 -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
-DOPM_ENABLE_PYTHON=ON -DOPM_ENABLE_DYNAMIC_BOOST=OFF -DOPM_ENABLE_DYNAMIC_PYTHON_LINKING=OFF ..

# make step is necessary until the generated ParserKeywords/*.hpp are generated in the Python step
make -j2

# Run setup-package.sh three times (Python 3.6, 3.7 and 3.8)
./setup-package.sh
cmake3 -DPYTHON_EXECUTABLE=/usr/local/bin/python3.7 -DBOOST_INCLUDEDIR=/usr/include/boost169 -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
-DOPM_ENABLE_PYTHON=ON -DOPM_ENABLE_DYNAMIC_BOOST=OFF -DOPM_ENABLE_DYNAMIC_PYTHON_LINKING=OFF ..
./setup-package.sh
cmake3 -DPYTHON_EXECUTABLE=/usr/local/bin/python3.8 -DBOOST_INCLUDEDIR=/usr/include/boost169 -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
-DOPM_ENABLE_PYTHON=ON -DOPM_ENABLE_DYNAMIC_BOOST=OFF -DOPM_ENABLE_DYNAMIC_PYTHON_LINKING=OFF ..
./setup-package.sh

/usr/bin/python3 -m auditwheel repair dist/*.whl

# Example of upload
# /usr/bin/python3 -m twine upload --repository testpypi wheelhouse/*
