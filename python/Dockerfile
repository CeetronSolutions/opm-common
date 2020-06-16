# Dockerfile to generate PyPI packages
# Example use:
# Use sudo docker build .

FROM lindkvis/manylinux2014_opm:latest
WORKDIR /tmp
COPY generate-pypi-package.sh .
RUN /bin/bash /tmp/generate-pypi-package.sh

